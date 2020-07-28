#include "NameDialog.h"
#include <DirectoryInterface.h>
#include "OkayHandler.h"


/**
 * Method: NameDialog::NameDialog
 * Purpose: Constructor
 * Parameters: TString& name - Name to give the Window
 *				TString& winClass - Name of the Window Class to send to Windows
 *				UINT style - the Style of the Window desired
 *				HWND parent - Handle to the Parent Window
 *				int commandShow - How to show the Window once created
 *				TrecPointer<TInstance> ins - The Anagame Instance associated with thi DIalog
 *				TDialogMode mode - How the Dialog should approach its parent
 *				TString& caption - The message to present to the User once the Dialog is drawn
 * Returns: New Name Dialog Object
 */
NameDialog::NameDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption):
	TDialog(name, winClass, style, parent, commandShow, ins, mode)
{
	this->caption.Set(caption);
}

/**
 * Method: NameDialog::NameDialog
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
NameDialog::~NameDialog()
{
}

/**
 * Method: NameDialog::CompileView
 * Purpose: Performs initialization of the dialog's contents
 * Parameters: TrecComPointer<ID2D1Factory1> fact -  the resource needed to create drawing boards
 * Returns: int - error code (0 for success)
 */
int NameDialog::CompileView(TrecComPointer<ID2D1Factory1> fact)
{
	TString file = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

	file.Append(L"Resources\\TextDialog.tml");

	TrecPointer<EventHandler> eh = TrecPointerKey::GetNewTrecPointerAlt<EventHandler, OkayHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance));

	int returnable = TWindow::CompileView(file, eh);

	if (returnable)return returnable;

	if (!mainPage.Get())
		return 10;

	TrecPointer<TControl> control = mainPage->GetRootControl();
	TLayout* layout = dynamic_cast<TLayout*>(control.Get());

	assert(layout);

	textField = layout->GetLayoutChild(0, 0);

	TTextField* tf = dynamic_cast<TTextField*>(textField.Get());

	assert(tf);

	tf->SetText(caption);
	tf->LockText();

	textField = layout->GetLayoutChild(0, 1);

	tf = dynamic_cast<TTextField*>(textField.Get());

	assert(tf);

	//tf->SetText(caption);
	tf->UnlockText();

	userInput = TrecPointerKey::GetNewTrecPointer<TString>();

	return 0;
}

/**
 * Method: NameDialog::OnDestroy
 * Purpose: Allows the Dialog to capture the User Input before destruction
 * Parameters: void
 * Returns: whether the Dialog can be destroyed
 */
bool NameDialog::OnDestroy()
{
	bool ret = TDialog::OnDestroy();

	if (!textField.Get() || !userInput.Get())
		return ret;

	TTextField* tf = dynamic_cast<TTextField*>(textField.Get());

	// Set the Pointer String so that clients of this dialog will still have the data collected
	// even after this dialog is destroyed
	userInput->Set(tf->GetText());

	return ret;
}

/**
 * Method: NameDialog::GetText
 * Purpose: Retrieves the Text provided by the User
 * Parameters: void
 * Returns: TrecPointer<TString> - the String captured from the User
 */
TrecPointer<TString> NameDialog::GetText()
{
	return userInput;
}
