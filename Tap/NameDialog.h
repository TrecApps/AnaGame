#pragma once
#include "TDialog.h"
/**
 * Class: NameDialog
 * Purpsoe: Allows users to enter a name into a Dialog for use in a main application
 */
class NameDialog :	public TDialog
{
public:

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
	NameDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption);

	/**
	 * Method: NameDialog::NameDialog
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~NameDialog();

	/**
	 * Method: NameDialog::CompileView
	 * Purpose: Performs initialization of the dialog's contents
	 * Parameters: TrecComPointer<ID2D1Factory1> fact -  the resource needed to create drawing boards
	 * Returns: int - error code (0 for success)
	 */
	int CompileView(TrecComPointer<ID2D1Factory1> fact);

	/**
	 * Method: NameDialog::OnDestroy
	 * Purpose: Allows the Dialog to capture the User Input before destruction
	 * Parameters: void
	 * Returns: whether the Dialog can be destroyed
	 */
	bool OnDestroy() override;


	/**
	 * Method: NameDialog::GetText
	 * Purpose: Retrieves the Text provided by the User
	 * Parameters: void
	 * Returns: TrecPointer<TString> - the String captured from the User
	 */
	TrecPointer<TString> GetText();
private:
	/**
	 * The Text Control the User enters input into
	 */
	TrecPointer<TControl> textField;

	/**
	 * The Message to present to the User
	 */
	TString caption;
	/**
	 * TString to retrieve the User Input from
	 */
	TrecPointer<TString> userInput;
};

