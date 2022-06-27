#include "FileDialog.h"
#include <DirectoryInterface.h>
#include "TInstance.h"

FileDialog::FileDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins, TDialogMode mode, TrecSubPointer<TPage::EventHandler, FileDialogHandler> handler) :
	TDialog(name, winClass, style, parent, commandShow, ins, mode)
{
	this->handler = handler;
}

FileDialog::~FileDialog()
{
	handler.Delete();
}

/**
 * Method: FileDialog::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString FileDialog::GetType()
{
	return TString(L"FileDialog;") + TDialog::GetType();
}

int FileDialog::CompileView(TrecComPointer<ID2D1Factory1> fact)
{
	TString file = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

	file.Append(L"Resources\\FileDialog.tml");
	ThreadLock();
	int returnable = TWindow::CompileView(file, TrecPointerKey::GetTrecPointerFromSub<TPage::EventHandler, FileDialogHandler>( handler));

	if (!returnable && !mainPage.Get())
	{
		returnable = -10;
	}
	ThreadRelease();
	return returnable;
}

bool FileDialog::OnDestroy()
{
	ThreadLock();
	bool ret = TDialog::OnDestroy();

	if (handler.Get())
		file = TFileShell::GetFileInfo(handler->GetPath());

	ThreadRelease();
	return true;
}

TrecPointer<TFileShell> FileDialog::GetFile()
{
	ThreadLock();
	auto ret = file;
	ThreadRelease();
	return ret;
}

TrecPointer<TFileShell> BrowseForFile(TrecPointer<TProcess> in, HWND parent, TrecPointer<TFileShell> directory, const TString& extensions, bool allowCreateFile, file_node_filter_mode filter_mode)
{
	if (!in.Get() || !parent)
		return TrecPointer<TFileShell>();

	TString name(L"Set Anagame Environment");
	TString winClass(L"Dialog");
	UINT style = WS_OVERLAPPEDWINDOW;

	auto handler = TrecPointerKey::GetNewTrecSubPointer<TPage::EventHandler, FileDialogHandler>(in);

	handler->SetAttributes(directory, extensions, allowCreateFile, filter_mode);

	auto dialog = TrecPointerKey::GetNewSelfTrecSubPointer<TWindow, FileDialog>(name, winClass, style, parent, 10, in, TDialogMode::dialog_mode_hard_model, handler);
	dialog->PrepareWindow();
	dialog->CompileView(in->GetFactory());



	dialog->Run();

	TrecPointer<TFileShell> ret = dialog->GetFile();

	dialog.Delete();

	return ret;
}
