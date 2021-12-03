#include "TConsoleLayout.h"
#include "TTextLayout.h"
#include <TTextElement.h>

class TConsoleTextInterceptor : public TTextIntercepter
{
protected:

    TrecPointer<TTextIntercepter> baseInterceptor;
    TrecSubPointerSoft<TPage, TConsoleLayout> layout;

public:

    TConsoleTextInterceptor(TrecPointer<TTextIntercepter> baseInterceptor,
        TrecSubPointerSoft<TPage, TConsoleLayout> layout)
    {
        assert(baseInterceptor.Get());
        assert(layout.Get());

        this->layout = layout;
        this->baseInterceptor = baseInterceptor;
    }

    virtual void OnChar(UINT ch, UINT count, UINT flags) override
    {
        if (ch == 13)
        {
            auto real = TrecPointerKey::GetSubPointerFromSoft<>(layout);
            if (real.Get() && real->SubmitInput())
                return;
            ch = L'\n';
        }
        baseInterceptor->OnChar(ch, count, 0);
    }

    virtual void OnKey(UINT ch, UINT count, UINT flags) override
    {
        if (ch == 13)
        {
            auto real = TrecPointerKey::GetSubPointerFromSoft<>(layout);
            if (real.Get() && real->SubmitInput())
                return;
            ch = L'\n';
        }
        baseInterceptor->OnKey(ch, count, flags);
    }


    virtual void OnLoseFocus()
    {
        baseInterceptor->OnLoseFocus();
    }

    virtual void OnCopy()
    {
        baseInterceptor->OnCopy();
    }

    virtual void OnCut()
    {
        baseInterceptor->OnCut();
    }
    virtual void* GetTarget() override
    {
        return baseInterceptor.Get();
    }

    virtual bool TakesInput() override
    {
        return baseInterceptor->TakesInput();
    }
};




bool TConsoleLayout::SubmitInput()
{
    TString str(dynamic_cast<TTextLayout*>(GetPage(0, 0).Get())->GetText());

    bool doSubmit = true;
    UINT slashCount = 0;
    for (UINT Rust = str.GetSize() - 1; Rust < str.GetSize(); Rust--)
    {
        if (str[Rust] == L'\\')
            slashCount++;
        else break;
    }

    if (slashCount % 2) return false;

    WCHAR quote = L'\0';
    slashCount = 0;
    for (UINT Rust = 0; Rust < str.GetSize(); Rust++)
    {
        WCHAR curQuote = str[Rust];


        switch (curQuote)
        {
        case L'\\':
            slashCount++;
        case L'\'':
        case L'\"':
            if (!(slashCount % 2))
            {
                if (quote == curQuote)
                    quote = L'\0';
                else if (!quote)
                    quote = curQuote;
            }
        default:
            slashCount = 0;
        }
    }

    if (!quote)
    {
        shell.SubmitCommand(str);
        dynamic_cast<TTextLayout*>(GetPage(0, 0).Get())->SetText(L"");
        return true;
    }

    return false;
}

void TConsoleLayout::ProcessShellOutput(TString& output)
{
    // To-Do: Handle Formatting

    // End to-do

    dynamic_cast<TTextLayout*>(GetPage(0, 0).Get())->AppendText(output);
}

bool TConsoleLayout::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
    TString valpoint;
    if (attributes.retrieveEntry(L"IsInput", valpoint) && !valpoint.CompareNoCase(L"false"))
        isInput = false;

    this->childControls.RemoveAll();

    AddRow(1, true);
    auto topText = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TTextLayout>(drawingBoard, styles, drawingBoard->GetWindowHandle());
    dynamic_cast<TControl*>(topText.Get())->AddAttribute(L"IsLocked", L"true");
    AddPage(topText, 0, 0);

    if (isInput)
    {
        AddRow(20, false);
        AddRow(30, false);
        auto directoryInput = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TTextInput>(drawingBoard, styles, drawingBoard->GetWindowHandle());
        dynamic_cast<TTextInput*>(directoryInput.Get())->LockText(true);
        AddPage(directoryInput, 1, 0);
        AddPage(TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TTextInput>(drawingBoard, styles, drawingBoard->GetWindowHandle()), 2, 0);
    }

    return TRandomLayout::onCreate(loc, d3d);
}

TConsoleLayout::TConsoleLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles) : TLayout(drawingBoard, styles)
{
    isInput = doDraw = true;
    restrictDraw = processRunning = false;
}

TConsoleLayout::~TConsoleLayout()
{
}

void TConsoleLayout::Draw(TrecPointer<TVariable> object)
{
    TObjectLocker lock(&thread);

    TString shellOutput(shell.GetOutput());

    bool pass = false;

    if (shellOutput.GetSize())
    {
        TString lowerOutput(shellOutput.GetLower());
        if (lowerOutput.FindLast(L'\n') != -1)
            lowerOutput.Set(lowerOutput.SubString(lowerOutput.FindLast(L'\n')));
        if (lowerOutput.Find(L"password") != -1 ||
            lowerOutput.Find(L"passphrase") != -1 ||
            lowerOutput.Find(L"passcode") != -1)
        {
            pass = true;
        }

        ProcessShellOutput(shellOutput);
    }

    if (isInput)
    {
        auto pInput = GetPage(2,0);

        dynamic_cast<TTextInput*>(pInput.Get())->SetPasswordMode(pass);

        bool processCheck = shell.CheckProcess();

        if (processRunning && !processCheck)
        {
            processRunning = false;

        }

        dynamic_cast<TTextInput*>(GetPage(1, 0).Get())->SetText(shell.GetWorkingDirectory());
    }

    TDataArray<EventID_Cred> cred;

    OnResize(area, 0, cred);

    if (restrictDraw)
    {
        drawingBoard->AddLayer(area);
        TLayout::Draw(object);
        drawingBoard->PopLayer();
    }
    else
        TLayout::Draw(object);

}

void TConsoleLayout::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& args)
{
    restrictDraw = false;

    float cBottom = newLoc.bottom;
    if (isInput)
    {
        auto page = GetPage(2, 0);
        D2D1_RECT_F cArea = page->GetArea();
        float hDif = newLoc.bottom - cArea.bottom;

        page->OnScroll(false, TPoint(0, 0), TPoint(0, hDif), args);

        cArea = page->GetArea();

        if (cArea.top < newLoc.top)
        {
            doDraw = false;
            return;
        }

        cArea.left = newLoc.left;
        cArea.right = newLoc.right;
        page->OnResize(cArea, 0, args);
        float bottom = cArea.top;

        page = GetPage(1, 0);
        cArea = page->GetArea();

        hDif = bottom - cArea.bottom;

        page->OnScroll(false, TPoint(0, 0), TPoint(0, hDif), args);

        cArea = page->GetArea();
        if (cArea.top < newLoc.top)
            restrictDraw = true;

        cBottom = cArea.top;
    }

    auto page = GetPage(0, 0);

    auto rec = newLoc;
    rec.bottom = cBottom;

    page->OnResize(rec, 0, args);
}


void TConsoleLayout::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&cred)
{
    TLayout::OnLButtonUp(nFlags, point, mOut, cred);

    if (cred.Size() && cred[cred.Size() - 1].textIntercepter.Get() && cred[cred.Size() - 1].textIntercepter->TakesInput())
    {
        cred[cred.Size() - 1].textIntercepter = TrecPointerKey::GetNewTrecPointerAlt<TTextIntercepter, TConsoleTextInterceptor>(
            cred[cred.Size() - 1].textIntercepter, TrecPointerKey::GetSoftSubPointerFromSoft<TPage, TConsoleLayout>(self));
    }
}

void TConsoleLayout::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
    TLayout::OnLButtonDown(nFlags, point, mOut, cred);
}
