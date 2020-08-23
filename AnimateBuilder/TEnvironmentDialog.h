#pragma once
#include <TDialog.h>
#include <TEnvironment.h>

/**
 * Class: TEnvironmentDialog
 * Purpose: Extends the TDialog class so that the Dialog prompts the user to selects a new environment, a blank one, or an existing one
 */
class TEnvironmentDialog :
    public TDialog
{
public:
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
	TEnvironmentDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode);

	/**
	 * Method: TEnvironmentDialog::~TEnvironmentDialog
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TEnvironmentDialog();

	/**
	 * Method: TEnvironmentDialog::CompileView
	 * Purpose: Performs initialization of the dialog's contents
	 * Parameters: TrecComPointer<ID2D1Factory1> fact -  the resource needed to create drawing boards
	 * Returns: int - error code (0 for success)
	 */
	int CompileView(TrecComPointer<ID2D1Factory1> fact);

	/**
	 * Method: TEnvironmentDialog::OnDestroy
	 * Purpose: Prepares the Dialog for destruction, releasing the Parent Window if necessary
	 * Parameters: void
	 * Returns: bool - whether the dialog can be destroyed yet
	 */
	bool OnDestroy() override;

	/**
	 * Method: TEnvironmentDialog::GetEnvironment
	 * Purpose: PRetrieves the environment procured byt the Dialog
	 * Parameters: void
	 * Returns: TrecPointer<TEnvironment> - the environment selected by the user
	 */
	TrecPointer<TEnvironment> GetEnvironment();

private:
	TrecPointer<TEnvironment> env;
};

TrecPointer<TEnvironment> ActivateEnvironmentDialog(TrecPointer<TInstance> ins, HWND parent);