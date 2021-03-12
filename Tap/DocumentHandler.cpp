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

/**
 * Method: DocumentHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString DocumentHandler::GetType()
{
	return TString(L"DocumentHandler;") + EventHandler::GetType();
}

void DocumentHandler::OnSave()
{
	ThreadLock();
	TrecPointer<TWindow> win;
	if (!file.Get() && page.Get() && (this->page->GetWindowHandle()).Get() && app.Get())
	{
		win = this->page->GetWindowHandle();
		TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));

		OPENFILENAMEW fileInfo;
		ZeroMemory(&fileInfo, sizeof(fileInfo));

		fileInfo.lStructSize = sizeof(OPENFILENAMEW);
		fileInfo.hwndOwner = win->GetWindowHandle();
		fileInfo.hInstance = TrecPointerKey::GetTrecPointerFromSoft<TInstance>(app)->GetInstanceHandle();
		fileInfo.lpstrFilter = nullptr;
		fileInfo.lpstrInitialDir = initialSearch.GetConstantBuffer().getBuffer();
		fileInfo.lpstrFile = new WCHAR[255];
		fileInfo.nMaxFile = 230;

		bool gotName = false;
		TString path;
		if (gotName = GetSaveFileNameW(&fileInfo))
		{
			path.Set(fileInfo.lpstrFile);
		}

		delete[] fileInfo.lpstrFile;
		if (!gotName) {
			ThreadRelease();
			return;
		}
		file = TFileShell::GetFileInfo(path);
	}

	if (!file.Get())
	{
		ThreadRelease();
		return;
	}
	TFile saveFile;

	saveFile.Open(file->GetPath(), TFile::t_file_write | TFile::t_file_create_always);

	if (saveFile.IsOpen())
	{
		OnSave(saveFile);
		saveFile.Close();
	}
	ThreadRelease();
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
	ThreadLock();
	TString ret(file.Get() ?  file->GetPath(): TString());
	ThreadRelease();
}

TrecPointer<TFileShell> DocumentHandler::GetFile()
{
	ThreadLock();
	auto ret = file;
	ThreadRelease();
	return ret;
}
