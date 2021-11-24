#include "TConsoleLayout.h"
#include "TTextLayout.h"

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
    AddPage(TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TTextLayout>(drawingBoard, styles), 0, 0);

    if (isInput)
    {
        AddRow(20, false);
        AddRow(30, false);
        auto directoryInput = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TTextInput>(drawingBoard, styles);
        dynamic_cast<TTextInput*>(directoryInput.Get())->LockText(true);
        AddPage(directoryInput, 1, 0);
        AddPage(TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TTextInput>(drawingBoard, styles), 2, 0);
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
    TDataArray<EventArgs> args;

    OnResize(area, 0, cred, args);

    if (restrictDraw)
    {
        drawingBoard->AddLayer(area);
        TLayout::Draw(object);
        drawingBoard->PopLayer();
    }
    else
        TLayout::Draw(object);

}

void TConsoleLayout::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&args)
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
        page->OnResize(cArea, 0, eventAr, args);
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

    page->OnResize(rec, 0, eventAr, args);
}


void TConsoleLayout::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TConsoleLayout::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>& args)
{
}
