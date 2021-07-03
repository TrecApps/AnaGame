#include <Windows.h>
#include "TDialog.h"
#include "TML_Reader_.h"
#include "AnafaceParser.h"
#include <Windows.h>
#include <d2d1.h>
#include "TInstance.h"

#include "AlertDialog.h"
#include "NameDialog.h"

// #include <afxglobals.h>


/**
 * Method: TDialog::TDialog
 * Purpose: Constructor
 * Parameters: TString& name - Name to give the Window
 *				TString& winClass - Name of the Window Class to send to Windows
 *				UINT style - the Style of the Window desired
 *				HWND parent - Handle to the Parent Window
 *				int commandShow - How to show the Window once created
 *				TrecPointer<TInstance> ins - The Anagame Instance associated with thi DIalog
 *				TDialogMode mode - How the Dialog should approach its parent
 * Returns: New Dialog Object
 */
TDialog::TDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode) :
	TWindow(name, winClass, style | WS_POPUP | WS_DLGFRAME, parent, commandShow, ins)
{
	dialogMode = mode;
	breakLoop = false;
}

/**
 * Method: TDialog::~TDialog
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TDialog::~TDialog()
{
	int e = 9;
}

/**
 * Method: TDialog::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TDialog::GetType()
{
	return TString(L"TDialog;") + TWindow::GetType();
}

/**
 * Method: TDialog::PrepareWindow
 * Purpose: Sets up the Dialog Window, restricting the Parent Window as called for
 * Parameters: void
 * Returns: int - error code (o for success)
 */
int TDialog::PrepareWindow()
{
	ThreadLock();
	if (!currentWindow)
	{
		ThreadRelease();
		return 1;
	}
	if (parent)
	{
		if (windowInstance.Get() && dialogMode == TDialogMode::dialog_mode_soft_modal)
		{
			TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance)->LockWindow(parent);
		}
		else if (dialogMode == TDialogMode::dialog_mode_hard_model)
		{
			EnableWindow(parent, FALSE);
		}
	}
	int ret = TWindow::PrepareWindow();
	ThreadRelease();
	return ret;
}

/**
 * Method: TDialog::OnDestroy
 * Purpose: Prepares the Dialog for destruction, releasing the Parent Window if necessary
 * Parameters: void
 * Returns: bool - whether the dialog can be destroyed yet
 */
bool TDialog::OnDestroy()
{
	ThreadLock();
	bool returnable = TWindow::OnDestroy();
	if (returnable && parent)
	{
		if (windowInstance.Get() && dialogMode == TDialogMode::dialog_mode_soft_modal)
		{
			TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance)->UnlockWindow(parent);
		}
		else if (dialogMode == TDialogMode::dialog_mode_hard_model)
		{
			EnableWindow(parent, TRUE);
		}
	}
	bool ret = breakLoop = returnable;
	ThreadRelease();
	return ret;
}

/**
 * Method: TDialog::Run
 * Purpose: Runs a New Loop for intercepting Messages
 * Parameters: void
 * Returns: void
 */
void TDialog::Run()
{
	ThreadLock();
	MSG msg;
	while (!breakLoop && GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ThreadRelease();
}

/**
 * Function: ActivateAlertDialog
 * Purpose: Runs the Course of an Alert Dialog
 * Parameters: TrecPointer<TInstance> ins - Instance under which the Dialog is being created
 *				 HWND parent - Handle to the Parent Window
 *				 TString& caption - the Text to present to the User
 * Returns: void
 */
void ActivateAlertDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption)
{
	//  UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption;
	TString name(L"Alert");
	TString winClass(L"Dialog");
	UINT style = WS_OVERLAPPEDWINDOW;
	// HWND parent
	// 0
	// ins
	// dialog_mode_hard_modal

	TrecPointer<TWindow> dialog = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, AlertDialog>(name, winClass, style, parent, 10, ins, TDialogMode::dialog_mode_hard_model, caption);
	dynamic_cast<AlertDialog*>(dialog.Get())->CompileView(ins->GetFactory());

	dialog->PrepareWindow();

	dynamic_cast<AlertDialog*>(dialog.Get())->Run();

	// dialog.Delete();
}

/**
 * Function: ActivateNameDialog
 * Purpose: Runs the Course of a Name Dialog
 * Parameters: TrecPointer<TInstance> ins - Instance under which the Dialog is being created
 *				 HWND parent - Handle to the Parent Window
 *				 TString& caption - the Text to present to the User
 * Returns: TString - the Input the User entered
 */
TString ActivateNameDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption)
{
	TString name(L"Alert");
	TString winClass(L"Dialog");
	UINT style = WS_OVERLAPPEDWINDOW;
	// HWND parent
	// 0
	// ins
	// dialog_mode_hard_modal


	TrecPointer<TWindow> dialog = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, NameDialog>(name, winClass, style, parent, 10, ins, TDialogMode::dialog_mode_hard_model, caption);
	dynamic_cast<NameDialog*>(dialog.Get())->CompileView(ins->GetFactory());

	TrecPointer<TString> sharedData = dynamic_cast<NameDialog*>(dialog.Get())->GetText();

	dialog->PrepareWindow();

	dynamic_cast<NameDialog*>(dialog.Get())->Run();

	TString ret(sharedData.Get());

	//dialog.Delete();

	return ret;
}
