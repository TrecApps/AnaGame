#include "TConsoleText.h"
#include <DirectoryInterface.h>

class TConsoleInputTextInterceptor : public TTextIntercepter
{
protected:

	TrecPointer<TTextIntercepter> baseInterceptor;
	TrecSubPointerSoft<TPage, TConsoleText> layout;

public:

	TConsoleInputTextInterceptor(TrecPointer<TTextIntercepter> baseInterceptor,
		TrecSubPointerSoft<TPage, TConsoleText> layout)
	{
		assert(baseInterceptor.Get());
		assert(layout.Get());

		this->layout = layout;
		this->baseInterceptor = baseInterceptor;
	}

	virtual void OnChar(UINT ch, UINT count, UINT flags) override
	{
		TrecSubPointer<TPage, TConsoleText> textLayout = TrecPointerKey::GetSubPointerFromSoft<>(layout);
		assert(textLayout.Get());

		if (!textLayout->isInput)return;

		UINT curCarLoc = dynamic_cast<TInputTextElement*>(textLayout->text.Get())->GetCarotLoc();
		UINT start = 0;
		if (dynamic_cast<TInputTextElement*>(textLayout->text.Get())->FindString(L"-->", start, false) &&
			start > curCarLoc)
		{
			dynamic_cast<TInputTextElement*>(textLayout->text.Get())->UpdateCarotPoisition(start);
			curCarLoc = start;
		}
		if (ch == 13)
		{
			auto real = TrecPointerKey::GetSubPointerFromSoft<>(layout);
			if (real.Get() && real->SubmitInput())
				return;
			ch = L'\n';
		}
		baseInterceptor->OnChar(ch, count, 0);

		UINT inputTarget = textLayout->text->GetTextLength() - curCarLoc;

		for(UINT Rust = 0; Rust < count; Rust++)
			textLayout->input.Insert(inputTarget, ch);
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


class TConsoleTextHolder : public TConsoleHolder {
	friend class TConsoleText;
	friend class TrecPointerKey;
public:
	/**
	 * Method: TPromptHolder::Warn
	 * Purpose: A warning message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Warn(TrecPointer<TVariable> var) override {
		LockDrawing();
		TrecSubPointer<TPage, TConsoleText> lay = TrecPointerKey::GetSubPointerFromSoft<>(layout);
		if (lay.Get())
		{
			TString out(TString(L"WARNING: ") + (var.Get() ? var->GetString() : L"null"));
			lay->ProcessShellOutput(out);
		}
		UnlockDrawing();
	}

	/**
	 * Method: TPromptHolder::Error
	 * Purpose: An error message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Error(TrecPointer<TVariable> var) override {
		LockDrawing();
		TrecSubPointer<TPage, TConsoleText> lay = TrecPointerKey::GetSubPointerFromSoft<>(layout);
		if (lay.Get())
		{
			TString out(TString(L"ERROR: ") + (var.Get() ? var->GetString() : L"null"));
			lay->ProcessShellOutput(out);
		}
		UnlockDrawing();
	}

	/**
	 * Method: TPromptHolder::Info
	 * Purpose: An information message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Info(TrecPointer<TVariable> var) override {
		LockDrawing();
		TrecSubPointer<TPage, TConsoleText> lay = TrecPointerKey::GetSubPointerFromSoft<>(layout);
		if (lay.Get())
		{
			TString out(TString(L"INFO: ") + (var.Get() ? var->GetString() : L"null"));
			lay->ProcessShellOutput(out);
		}
		UnlockDrawing();
	}

	/**
	 * Method: TPromptHolder::Log
	 * Purpose: A regular message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Log(TrecPointer<TVariable> var) override {
		LockDrawing();
		TrecSubPointer<TPage, TConsoleText> lay = TrecPointerKey::GetSubPointerFromSoft<>(layout);
		if (lay.Get())
		{
			TString out((var.Get() ? var->GetString() : L"null"));
			lay->ProcessShellOutput(out);
		}
		UnlockDrawing();
	}

protected:
	TConsoleTextHolder(TrecSubPointer<TPage, TConsoleText> layout) {
		this->layout = TrecPointerKey::GetSoftSubPointerFromSub<>(layout);
	}
	TrecSubPointerSoft<TPage, TConsoleText> layout;
};


bool TConsoleText::SubmitInput()
{
	UINT slashCount = 0;
	for (UINT Rust = input.GetSize() - 1; Rust < input.GetSize(); Rust--)
	{
		if (input[Rust] == L'\\')
			slashCount++;
		else break;
	}

	if (slashCount % 2) return false;

	WCHAR quote = L'\0';
	slashCount = 0;
	for (UINT Rust = 0; Rust < input.GetSize(); Rust++)
	{
		WCHAR curQuote = input[Rust];


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
	if (quote)
		return false;
	shell.SubmitCommand(input);
	input.Empty();
    return false;
}

void TConsoleText::ProcessShellOutput(TString& output)
{
    // To-Do: Handle Formatting

    // End to-do

    AppendText(output);
}

TrecPointer<TConsoleHolder> TConsoleText::GetConsoleHolder()
{
	return TrecPointerKey::GetNewTrecPointerAlt<TConsoleHolder, TConsoleTextHolder>(
		TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TConsoleText>(
			TrecPointerKey::GetTrecPointerFromSoft<>(this->self)));
}

bool TConsoleText::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d, TrecPointer<TFileShell> d)
{
    TString valpoint;
    if (attributes.retrieveEntry(L"IsInput", valpoint) && !valpoint.CompareNoCase(L"false"))
        isInput = false;
    bool ret = TGadget::onCreate(loc, d3d, d);
	if (text.Get())
	{
		text->SetHorizontallignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		text->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		TextFormattingDetails details;
		if (text->GetBasicFormatting(details))
		{
			details.color = drawingBoard->GetBrush(TColor(L"white"));
			text->SetBasicFormatting(details);
		}
	}
	return ret;
}

TConsoleText::TConsoleText(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles)
    : TTextLayout(drawingBoard, styles, drawingBoard->GetWindowHandle())
{
    isInput = true;
	processRunning = false;
}

TConsoleText::~TConsoleText()
{
}

void TConsoleText::Draw(TrecPointer<TVariable> object)
{
    TObjectLocker lock(&thread);
	TString shellOutput(shell.GetOutput());

	if (shellOutput.GetSize())
	{
		TString sText;
		text->GetText(sText);
		ProcessShellOutput(shellOutput);

		// Check to see if we are getting a password 
		TString lowerOutput(shellOutput.GetLower());
		if (lowerOutput.FindLast(L'\n') != -1)
			lowerOutput.Set(lowerOutput.SubString(lowerOutput.FindLast(L'\n')));
		if (lowerOutput.Find(L"password") != -1 ||
			lowerOutput.Find(L"passphrase") != -1 ||
			lowerOutput.Find(L"passcode") != -1)
			this->usePassword = true;

	}

	bool processCheck = shell.CheckProcess();
	if (isInput)
	{
		if (processRunning && !processCheck)
		{
			processRunning = false;
			TString add;

			TString output(shell.GetOutput());
			if (output.GetSize())
				output.Set(TString(L"\n") + output + L"\n");

			ProcessShellOutput(output);

			if (this->editEnabled)
				AppendText(shell.GetWorkingDirectory() + L"\n-->");
		}
	}
	TTextLayout::Draw(object);
}

void TConsoleText::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&cred)
{
	TTextLayout::OnLButtonUp(nFlags, point, mOut, cred);

	if (cred.Size() && cred[cred.Size() - 1].textIntercepter.Get() && cred[cred.Size() - 1].textIntercepter->TakesInput())
	{
		cred[cred.Size() - 1].textIntercepter = TrecPointerKey::GetNewTrecPointerAlt<TTextIntercepter, TConsoleInputTextInterceptor>(
			cred[cred.Size() - 1].textIntercepter, TrecPointerKey::GetSoftSubPointerFromSoft<TPage, TConsoleText>(self));
	}
}

void TConsoleText::SetDirectory(TrecPointer<TFileShell> directory)
{
	if (!directory.Get() || !directory->IsDirectory())
		directory = TFileShell::GetFileInfo(GetDirectory(CentralDirectories::cd_Documents));

	assert(directory.Get());
	shell.SetWorkingDirectory(directory);
}
