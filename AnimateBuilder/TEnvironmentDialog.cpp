#include "TEnvironmentDialog.h"
#include <DirectoryInterface.h>
#include <TInstance.h>
#include "EnvironmentHandler.h"


/**
 * Method: TEnvironmentDialog::TEnvironmentDialog
 * Purpose: Constructor
 * Parameters: TString& name - Name to give the Window
 *				TString& winClass - Name of the Window Class to send to Windows
 *				UINT style - the Style of the Window desired
 *				HWND parent - Handle to the Parent Window
 *				int commandShow - How to show the Window once created
 *				TrecPointer<TInstance> ins - The Anagame Instance associated with thi DIalog
 *				TDialogMode mode - How the Dialog should approach its parent
 *				TString& caption - The message to present to the User once the Dialog is drawn
 * Returns: New TEnvironmentDialog instance
 */
TEnvironmentDialog::TEnvironmentDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode):
	TDialog(name, winClass, style, parent, commandShow, ins, mode)
{

}

/**
 * Method: TEnvironmentDialog::~TEnvironmentDialog
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TEnvironmentDialog::~TEnvironmentDialog()
{

}

/**
 * Method: TEnvironmentDialog::CompileView
 * Purpose: Performs initialization of the dialog's contents
 * Parameters: TrecComPointer<ID2D1Factory1> fact -  the resource needed to create drawing boards
 * Returns: int - error code (0 for success)
 */
int TEnvironmentDialog::CompileView(TrecComPointer<ID2D1Factory1> fact)
{
	TString file = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

	file.Append(L"Resources\\Environments_Dialog.tml");

	TrecPointer<EventHandler> eh = TrecPointerKey::GetNewTrecPointerAlt<EventHandler, EnvironmentHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance));

	int returnable = TWindow::CompileView(file, eh);

	if (returnable)return returnable;

	if (!mainPage.Get())
		return 10;
}

/**
 * Method: TEnvironmentDialog::OnDestroy
 * Purpose: Prepares the Dialog for destruction, releasing the Parent Window if necessary
 * Parameters: void
 * Returns: bool - whether the dialog can be destroyed yet
 */
bool TEnvironmentDialog::OnDestroy()
{
	bool ret = TDialog::OnDestroy();

	if (!mainPage.Get() || !mainPage->GetHandler().Get())
		return false;

	auto handler = mainPage->GetHandler();

	assert(dynamic_cast<EnvironmentHandler*>(handler.Get()));

	env = dynamic_cast<EnvironmentHandler*>(handler.Get())->GetEnvironment();

	return false;
}

/**
 * Method: TEnvironmentDialog::GetEnvironment
 * Purpose: PRetrieves the environment procured byt the Dialog
 * Parameters: void
 * Returns: TrecPointer<TEnvironment> - the environment selected by the user
 */
TrecPointer<TEnvironment> TEnvironmentDialog::GetEnvironment()
{
	return env;
}

TrecPointer<TEnvironment> ActivateEnvironmentDialog(TrecPointer<TInstance> ins, HWND parent)
{
	if (!ins.Get() || !parent)
		return TrecPointer<TEnvironment>();

	TString name(L"Set Anagame Environment");
	TString winClass(L"Dialog");
	UINT style = WS_OVERLAPPEDWINDOW;

	TrecPointer<TWindow> dialog = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, TEnvironmentDialog>(name, winClass, style, parent, 10, ins, TDialogMode::dialog_mode_hard_model);
	dynamic_cast<TEnvironmentDialog*>(dialog.Get())->CompileView(ins->GetFactory());

	dialog->PrepareWindow();

	dynamic_cast<TEnvironmentDialog*>(dialog.Get())->Run();

	return dynamic_cast<TEnvironmentDialog*>(dialog.Get())->GetEnvironment();
}
