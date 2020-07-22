#pragma once
#include "Tap_dll.h"
#include "TObject.h"
#include <TString.h>
#include <AnafaceUI.h>
#include "AnafaceParser.h"
#include "TWindow.h"
#include <TrecReference.h>

/**
 * Enum Class: TDialogMode
 * Purpose: Specifies how the Dialog handles it's main parent Window
 */
typedef enum class TDialogMode
{
	dialog_mode_free,		// Other widnows could still be interacted with
	dialog_mode_soft_modal,	// Other Windows will not respond to user interaction, but they could still be moved around
	dialog_mode_hard_model	// Other Windows will not respond to users
}TDialogMode;

/**
 * Class: TDialog
 * Purpose: Extends the TWIndow class to support Dialogs
 */
class _TAP_DLL TDialog : public TWindow
{
public:

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
	TDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode);

	/**
	 * Method: TDialog::~TDialog
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TDialog();

	/**
	 * Method: TDialog::PrepareWindow
	 * Purpose: Sets up the Dialog Window, restricting the Parent Window as called for
	 * Parameters: void
	 * Returns: int - error code (o for success)
	 */
	int PrepareWindow() override;

	/**
	 * Method: TDialog::OnDestroy
	 * Purpose: Prepares the Dialog for destruction, releasing the Parent Window if necessary
	 * Parameters: void
	 * Returns: bool - whether the dialog can be destroyed yet
	 */
	bool OnDestroy() override;


	/**
	 * Method: TDialog::CompileView
	 * Purpose: Sets up the Content of the Dialog
	 * Parameters: TrecComPointer<ID2D1Factory1> fact =  the Factory to Provide the Dialog
	 * Returns: int - error code (0 for no error)
	 */
	virtual int CompileView(TrecComPointer<ID2D1Factory1> fact) = 0;


	/**
	 * Method: TDialog::Run
	 * Purpose: Runs a New Loop for intercepting Messages
	 * Parameters: void
	 * Returns: void
	 */
	void Run();

protected:
	TDialogMode dialogMode;
	bool breakLoop;
};

/**
 * Function: ActivateAlertDialog
 * Purpose: Runs the Course of an Alert Dialog
 * Parameters: TrecPointer<TInstance> ins - Instance under which the Dialog is being created
 *				 HWND parent - Handle to the Parent Window
 *				 TString& caption - the Text to present to the User
 * Returns: void
 */
_TAP_DLL void ActivateAlertDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption);

/**
 * Function: ActivateNameDialog
 * Purpose: Runs the Course of a Name Dialog
 * Parameters: TrecPointer<TInstance> ins - Instance under which the Dialog is being created
 *				 HWND parent - Handle to the Parent Window
 *				 TString& caption - the Text to present to the User
 * Returns: TString - the Input the User entered
 */
_TAP_DLL TString ActivateNameDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption);