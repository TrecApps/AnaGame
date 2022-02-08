#include "AlertDialog.h"
#include <DirectoryInterface.h>
#include "OkayHandler.h"
#include <AnafacePage.h>
#include <TLayout.h>
#include <TTextInput.h>

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
AlertDialog::AlertDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins, TDialogMode mode, TString& caption) :
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
 * Method: AlertDialog::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString AlertDialog::GetType()
{
	return TString(L"AlertDialog;") + TDialog::GetType();
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

	TrecPointer<TPage::EventHandler> eh = TrecPointerKey::GetNewTrecPointerAlt<TPage::EventHandler, OkayHandler>(TrecPointerKey::GetTrecPointerFromSoft<>(windowInstance));

	int returnable = TWindow::CompileView(file, eh);

	if (returnable)return returnable;

	ThreadLock();
	int ret = 10;

	if (mainPage.Get())
	{

		TrecPointer<TPage> control = dynamic_cast<AnafacePage*> (mainPage.Get())->GetRootControl();
		TLayout* layout = dynamic_cast<TLayout*>(control.Get());

		assert(layout);

		textField = layout->GetPage(0, 0);

		TTextInput* tf = dynamic_cast<TTextInput*>(textField.Get());

		assert(tf);

		tf->SetText(caption);
		//tf->LockText();
		ret = 0;
	}
	ThreadRelease();
	return ret;
}
