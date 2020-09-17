#include "FileDialog.h"
#include <DirectoryInterface.h>
#include "TInstance.h"

FileDialog::FileDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TrecSubPointer<EventHandler, FileDialogHandler> handler) :
	TDialog(name, winClass, style, parent, commandShow, ins, mode)
{
	this->handler = handler;
}

FileDialog::~FileDialog()
{
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

	int returnable = TWindow::CompileView(file, TrecPointerKey::GetTrecPointerFromSub<EventHandler, FileDialogHandler>( handler));

	if (returnable)return returnable;

	if (!mainPage.Get())
		return 10;
	return 0;
}

bool FileDialog::OnDestroy()
{
	bool ret = TDialog::OnDestroy();

	if (handler.Get())
		file = TFileShell::GetFileInfo(handler->GetPath());

	return false;
}

TrecPointer<TFileShell> FileDialog::GetFile()
{
	return file;
}

TrecPointer<TFileShell> BrowseForFile(TrecPointer<TInstance> in, HWND parent, TrecPointer<TFileShell> directory, const TString& extensions, bool allowCreateFile, file_node_filter_mode filter_mode)
{
	if (!in.Get() || !parent)
		return TrecPointer<TFileShell>();

	TString name(L"Set Anagame Environment");
	TString winClass(L"Dialog");
	UINT style = WS_OVERLAPPEDWINDOW;

	auto handler = TrecPointerKey::GetNewTrecSubPointer<EventHandler, FileDialogHandler>(in);

	handler->SetAttributes(directory, extensions, allowCreateFile, filter_mode);

	auto dialog = TrecPointerKey::GetNewSelfTrecSubPointer<TWindow, FileDialog>(name, winClass, style, parent, 10, in, TDialogMode::dialog_mode_hard_model, handler);

	dialog->CompileView(in->GetFactory());

	dialog->PrepareWindow();

	dialog->Run();

	return dialog->GetFile();
}
