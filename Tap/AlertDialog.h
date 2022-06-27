#pragma once
#include "TDialog.h"

/**
 * Class: AlertDialog
 * Purpose: Extends the TDialog class so that the Dialog gives the User a message
 * 
 * SuperClass: TDialog
 */
class AlertDialog :	public TDialog
{
public:
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
	AlertDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins, TDialogMode mode, TString& caption);
	/**
	 * Method: AlertDialog::~AlertDialog
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~AlertDialog();

	/**
	 * Method: AlertDialog::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: AlertDialog::CompileView
	 * Purpose: Performs initialization of the dialog's contents
	 * Parameters: TrecComPointer<ID2D1Factory1> fact -  the resource needed to create drawing boards
	 * Returns: int - error code (0 for success)
	 * 
	 * Attributes: override
	 */
	int CompileView(TrecComPointer<ID2D1Factory1> fact)override;
private:
	/**
	 * The Text field holding the message for the user
	 */
	TrecPointer<TPage> textField;
	
	/**
	 * Holds the message to send to the user, especially before an Anaface is created for the dialog
	 */
	TString caption;
};
