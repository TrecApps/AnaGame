#include "AlertDialog.h"
#include <DirectoryInterface.h>
#include "OkayHandler.h"

/**
 * Method: AlertDialog::AlertDialog
 * Purpose: Constructor
 * Parameters: TString& name - Name to give the Window
 *				TString& winClass - Name of the Window Class to send to Windows
 *				UINT style - the Style of the Window desired
 *				HWND parent - Handle to the Parent Window
 *				int commandShow - How to show the Window once created
 *				TrecPointer<TInstance> ins - The Anagame Instance associated with thi DIalog
 *				TDialogMode mode - How the Dialog should approach its parent
 *				TString& caption - The message to present to the User once the Dialog is drawn
 * Returns: New AlertDialog instance
 */
AlertDialog::AlertDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption) :
	TDialog(name, winClass, style, parent, commandShow, ins, mode)
{
	this->caption.Set(caption);
}

/**
 * Method: AlertDialog::~AlertDialog
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
AlertDialog::~AlertDialog()
{
}

/**
 * Method: AlertDialog::CompileView
 * Purpose: Performs initialization of the dialog's contents
 * Parameters: TrecComPointer<ID2D1Factory1> fact -  the resource needed to create drawing boards
 * Returns: int - error code (0 for success)
 */
int AlertDialog::CompileView(TrecComPointer<ID2D1Factory1> fact)
{
	TString file = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

	file.Append(L"Resources\\TextDialog.tml");

	TrecPointer<EventHandler> eh = TrecPointerKey::GetNewTrecPointerAlt<EventHandler, OkayHandler>(windowInstance);

	int returnable = TWindow::CompileView(file, eh);

	if (returnable)return returnable;

	if (!mainPage.Get())
		return 10;

	TrecPointer<TControl> control = mainPage->GetRootControl();
	TLayout* layout = dynamic_cast<TLayout*>(control.Get());

	assert (layout);

	textField = layout->GetLayoutChild(0, 0);

	TTextField* tf = dynamic_cast<TTextField*>(textField.Get());

	assert(tf);

	tf->SetText(caption);
	tf->LockText();

	return 0;
}
