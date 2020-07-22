#include "SourceCodeApp.h"
#include <DirectoryInterface.h>

SourceCodeApp::SourceCodeApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance> r): MiniHandler(m, o, e, r)
{
	lines = code = nullptr;
	col_keyword = D2D1::ColorF(D2D1::ColorF::Blue); col_type_id = D2D1::ColorF(D2D1::ColorF::LightBlue);
	col_var_id = D2D1::ColorF(D2D1::ColorF::Gray); col_fun_id = D2D1::ColorF(D2D1::ColorF::Gold);

	// Time to make sure parameters are valid
	if (!m.Get() || !o.Get() || !e.Get())
		return;

	// Start interpret as null so was don't have dangling pointer
	//interpret = nullptr;

	// Now set the structure to link the listeners to their text name
	eventNameID enid;


	TString fileBase(GetDirectory(CentralDirectories::cd_Executable));
	TString fileBody = fileBase;
	fileBody.Append( L"\\Resources\\LineTextEditor.txt");
	window = r->GetMainWindow();

	
	if (!m.Get() || !dynamic_cast<AnafaceUI*>(m.Get()))
		return;
	D2D1_RECT_F rect = dynamic_cast<AnafaceUI*>(m.Get())->GetControlArea();
	mainPage = window->GetPageByArea(rect);

	TrecPointer<TFile> file = TrecPointerKey::GetNewTrecPointer<TFile>();
	bool fileOpened = file->Open(fileBody, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
	if (!fileOpened)
		return;
	mainPage->SetAnaface(file, handleList);
	file->Close();
	mainControl = mainPage->GetRootControl();

	if (!mainControl.Get())
		return;
	dynamic_cast<AnafaceUI*>(m.Get())->addControl(mainControl, TString());


	if (!o.Get() || !dynamic_cast<AnafaceUI*>(o.Get()))
		return;
	rect = dynamic_cast<AnafaceUI*>(o.Get())->GetControlArea();
	outputPane = window->GetPageByArea(rect);

	if (!e.Get() || !dynamic_cast<AnafaceUI*>(e.Get()))
		return;
	rect = dynamic_cast<AnafaceUI*>(e.Get())->GetControlArea();
	explorerPane = window->GetPageByArea(rect);
}

SourceCodeApp::~SourceCodeApp()
{
	mainControl.Delete();
	//outputControl.Delete();
	//explorerControl.Delete();
}

bool SourceCodeApp::InitializeControls()
{
	try
	{
		if (!mainPage.Get())return false;
		TLayout* lay = dynamic_cast<TLayout*>(mainPage->GetRootControl().Get());
		if (!lay)
			return false;
		lines = dynamic_cast<TTextField*>(lay->GetLayoutChild(0, 0).Get());
		code = dynamic_cast<TTextField*>(lay->GetLayoutChild(1, 0).Get());
		return lines && code;
	}
	catch (std::bad_cast& e)
	{

	}
	return false;

}

void SourceCodeApp::SetContents(TFile& content)
{
	if (!content.IsOpen())
		return;
	if (code)
	{
		TString strContent, line;
		while (content.ReadString(line))
		{
			if (strContent.GetSize())
				strContent.Append(L'\n');
			strContent.Append(line);
		}

		code->SetText(strContent);

	}

	filePath.Set(content.GetFilePath());

	PrepLanguage(content);
}

/*
void SourceCodeApp::OnSave()
{
	if (!code)
		return;
	if (!filePath.GetSize())
	{
		OPENFILENAMEW fileStruct;
		ZeroMemory(&fileStruct, sizeof(fileStruct));


		GetSaveFileNameW(&fileStruct);

		filePath.Set(fileStruct.lpstrFile);
	}

	TFile file;
	file.Open(filePath, TFile::t_file_open_always | TFile::t_file_write);
	if (!file.IsOpen())
		return;

	TString write = code->GetText();
	file.WriteString(write);


	PrepLanguage(file);

	file.Close();
}
*/

void SourceCodeApp::OnShow()
{
	MiniHandler::OnShow();
}

void SourceCodeApp::onHide()
{
}

UINT SourceCodeApp::ProcessIntLanguage()
{
	return 0;
}

void SourceCodeApp::HandleEvents(TDataArray<EventID_Cred>&)
{
}

void SourceCodeApp::PrepLanguage(TFile& sourceCode)
{
}

void SourceCodeApp::OnSave(TFile& file)
{
	if (!file.IsOpen() || !code) return;
	file.WriteString(code->GetText());
}
