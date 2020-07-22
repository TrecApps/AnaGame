#include "DocumentHandler.h"
#include <DirectoryInterface.h>
#include "Page.h"
#include "TWindow.h"
#include "TInstance.h"
#include <TFile.h>

DocumentHandler::DocumentHandler(TrecPointer<TInstance> in): EventHandler(in)
{
}

DocumentHandler::DocumentHandler(TrecPointer<TInstance> in, const TString& name): EventHandler(in, name)
{
}

void DocumentHandler::OnSave()
{
	TrecPointer<TWindow> win;
	if (!file.Get() && page.Get() && (this->page->GetWindowHandle()).Get() && app.Get())
	{
		win = this->page->GetWindowHandle();
		TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));

		OPENFILENAMEW fileInfo;
		ZeroMemory(&fileInfo, sizeof(fileInfo));

		fileInfo.lStructSize = sizeof(OPENFILENAMEW);
		fileInfo.hwndOwner = win->GetWindowHandle();
		fileInfo.hInstance = app->GetInstanceHandle();
		fileInfo.lpstrFilter = nullptr;
		fileInfo.lpstrInitialDir = initialSearch.GetConstantBuffer();
		fileInfo.lpstrFile = new WCHAR[255];
		fileInfo.nMaxFile = 230;

		bool gotName = false;
		TString path;
		if (gotName = GetSaveFileNameW(&fileInfo))
		{
			path.Set(fileInfo.lpstrFile);
		}

		delete[] fileInfo.lpstrFile;
		if (!gotName) return;

		file = TFileShell::GetFileInfo(path);
	}

	if (!file.Get())
		return;

	TFile saveFile;

	saveFile.Open(file->GetPath(), TFile::t_file_write | TFile::t_file_create_always);

	if (!saveFile.IsOpen())
		return;

	OnSave(saveFile);
	saveFile.Close();
}

void DocumentHandler::OnLoad()
{
}

void DocumentHandler::OnShow()
{
}

void DocumentHandler::onHide()
{
}

TString DocumentHandler::GetFilePath()
{
	if (file.Get())
		return file->GetPath();
	return TString();
}

TrecPointer<TFileShell> DocumentHandler::GetFile()
{
	return file;
}
