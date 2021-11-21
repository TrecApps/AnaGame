#include "TCodeHandler.h"
#include <AnafacePage.h>
#include <TScrollerPage.h>
#include <TLayout.h>



TCodeHandler::TCodeHandler(TrecPointer<TProcess> in) : DocumentHandler(in)
{
	onFocusString.Set(L"Focus CodeHandler");
}

TCodeHandler::TCodeHandler(TrecPointer<TProcess> in, const TString& name) : DocumentHandler(in, name)
{
	onFocusString.Set(L"Focus CodeHandler");
}

/**
 * Method: TCodeHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TCodeHandler::GetType()
{
	return TString(L"TCodeHandler;") + EventHandler::GetType();
}

void TCodeHandler::Initialize(TrecPointer<TPage> page)
{
	ThreadLock();
	if (!page.Get())
	{
		ThreadRelease();
		return;
	}
	auto control = dynamic_cast<AnafacePage*>(page.Get())->GetRootControl();

	if (!control.Get())
	{
		ThreadRelease();
		return;
	}
	if(dynamic_cast<TScrollerPage*>(control.Get()))
		control = dynamic_cast<TScrollerPage*>(control.Get())->GetChildPage();

	TLayout* lay = dynamic_cast<TLayout*>(control.Get());
	if (!lay)
	{
		ThreadRelease();
		return;
	}
	lines = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(lay->GetPage(0, 0));
	code = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(lay->GetPage(1, 0));

	this->page = page;

	if (filePointer.Get())
	{
		TFile realFile(filePointer->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);

		if (realFile.IsOpen())
		{
			TString line;
			TString codeData;
			while (realFile.ReadString(line))
			{
				if (codeData.GetSize())
				{
					codeData.AppendFormat(L"\n%ws", line.GetConstantBuffer().getBuffer());
				}
				else
					codeData.Set(line);
			}

			code->SetText(codeData);
		}
	}
	ThreadRelease();
}

void TCodeHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
}

void TCodeHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

void TCodeHandler::OnSave()
{
	ThreadLock();
	SetSaveFile();

	if (filePointer.Get() && !filePointer->IsDirectory())
	{
		TFile saver(filePointer->GetPath(), TFile::t_file_write | TFile::t_file_create_always);
		OnSave(saver);
	}
	ThreadRelease();
}



bool TCodeHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}

void TCodeHandler::OnSave(TFile& file)
{
	ThreadLock();
	if (file.IsOpen() && code.Get()) 
		file.WriteString(code->GetText());
	ThreadRelease();
}

void TCodeHandler::OnLoad(TFile&)
{
}
