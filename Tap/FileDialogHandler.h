#pragma once
#include "EventHandler.h"
#include <TFileNode.h>
#include <TTextInput.h>
#include <TLayout.h>

class FileDialogHandler;


typedef void (FileDialogHandler::* FileDialogEvents)(TrecPointer<TPage> tc, EventArgs ea);


/**
 * Class: FileDialogHandler
 * Purpose: Handler for a dialog intended to offer a nice File Browsing Experience
 * 
 * SuperClass: EventHandler
 */
class FileDialogHandler :  public TapEventHandler
{
public:
	/**
	 * Method: FileDialogHandler::EventHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
	FileDialogHandler(TrecPointer<TProcess> instance);


	/**
	 * Method: FileDialogHandler::~EventHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~FileDialogHandler();


	/**
	 * Method: FileDialogHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 * 
	 * Attributes: override
	 */
	virtual TString GetType()override;


	/**
	 * Method: FileDialogHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<TPage> page) override;


	/**
	 * Method: FileDialogHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr) override;


	/**
	 * Method: FileDialogHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) override;

	/**
	 * Method: FileDialogHandler::GetPath
	 * Purpose: Reports the selected directory
	 * Parameters: void
	 * Returns: TString - the String the user selected (empty if operation was cancelled
	 */
	TString GetPath();

	/**
	 * Method: FileDialogHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 */
	void SetAttributes(TrecPointer<TFileShell> directory, const TString& extensions, bool allowCreateFile = false, file_node_filter_mode filter_mode = file_node_filter_mode::fnfm_block_current);

protected:

	TDataMap<UINT> events;

	// Relevent controls once controls are generated from a TML file

	TrecSubPointer<TObjectNode, TFileNode> fileNode;

	TrecSubPointer<TPage, TTextInput> directoryText;

	//TrecSubPointer<TPage, TTreeDataBind> browserControl;
	//TrecSubPointer<TPage, TTreeDataBind> favoritesControl;

	TrecSubPointer<TPage, TTextInput> fileText;

	TrecSubPointer<TPage, TLayout> browserLayout;

	TrecPointer<TPage> okayControl;


	TrecPointer<TFileShell> startDirectory;

	TrecSubPointer<TPage, TTextInput> toggleFavoriteDirectory;



	TString chosenPath, extensions;

	file_node_filter_mode filter_mode;

	TrecPointer<TWindow> window;

	bool allowCreateFile;


	/**
	 * Method: FileDialogHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 * 
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	bool isFavorite;
	void RefreshFavoriteToggle();


	TDataArray<FileDialogEvents> fileEvents;

	void OnSelectNode(TrecPointer<TPage> tc, EventArgs ea);

	void OnCancel(TrecPointer<TPage> tc, EventArgs ea);

	void OnOkay(TrecPointer<TPage> tc, EventArgs ea);

	void OnFileNameChange(TrecPointer<TPage> tc, EventArgs ea);

	void OnClickNode(TrecPointer<TPage> tc, EventArgs ea);

	void OnNewFolder(TrecPointer<TPage> tc, EventArgs ea);

	void OnToggleFavoriteFolder(TrecPointer<TPage> tc, EventArgs ea);
};

