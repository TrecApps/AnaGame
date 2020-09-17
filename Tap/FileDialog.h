#pragma once
#include "TDialog.h"
#include "FileDialogHandler.h"
class _TAP_DLL FileDialog :
    public TDialog
{
public:
	/**
	 * Method: FileDialog::FileDialog
	 * Purpose: Constructor
	 * Parameters: TString& name - Name to give the Window
	 *				TString& winClass - Name of the Window Class to send to Windows
	 *				UINT style - the Style of the Window desired
	 *				HWND parent - Handle to the Parent Window
	 *				int commandShow - How to show the Window once created
	 *				TrecPointer<TInstance> ins - The Anagame Instance associated with thi DIalog
	 *				TDialogMode mode - How the Dialog should approach its parent
	 *				TString& caption - The message to present to the User once the Dialog is drawn
	 *				TrecSubPointer<EventHandler, FileDialogHandler> handler - the handler to use
	 * Returns: New FileDialog instance
	 */
	FileDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TrecSubPointer<EventHandler, FileDialogHandler> handler);

	/**
	 * Method: FileDialog::~FileDialog
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~FileDialog();


	/**
	 * Method: FileDialog::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: FileDialog::CompileView
	 * Purpose: Performs initialization of the dialog's contents
	 * Parameters: TrecComPointer<ID2D1Factory1> fact -  the resource needed to create drawing boards
	 * Returns: int - error code (0 for success)
	 */
	int CompileView(TrecComPointer<ID2D1Factory1> fact);

	/**
	 * Method: FileDialog::OnDestroy
	 * Purpose: Prepares the Dialog for destruction, releasing the Parent Window if necessary
	 * Parameters: void
	 * Returns: bool - whether the dialog can be destroyed yet
	 */
	bool OnDestroy() override;

	/**
	 * Method: FileDialog::GetEnvironment
	 * Purpose: PRetrieves the environment procured byt the Dialog
	 * Parameters: void
	 * Returns: TrecPointer<TEnvironment> - the environment selected by the user
	 */
	TrecPointer<TFileShell> GetFile();

private:
	TrecPointer<TFileShell> file;

	TrecSubPointer<EventHandler, FileDialogHandler> handler;
};

_TAP_DLL TrecPointer<TFileShell> BrowseForFile(TrecPointer<TInstance> in, HWND parent, TrecPointer<TFileShell> directory, const TString& extensions, bool allowCreateFile = false, file_node_filter_mode filter_mode = file_node_filter_mode::fnfm_block_current);