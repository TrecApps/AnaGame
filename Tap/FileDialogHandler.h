#pragma once
#include "EventHandler.h"
#include <TFileNode.h>

class FileDialogHandler;


typedef void (FileDialogHandler::* FileDialogEvents)(TrecPointer<TControl> tc, EventArgs ea);


/**
 * Class: FileDialogHandler
 * Purpose: Handler for a dialog intended to offer a nice File Browsing Experience
 */
class FileDialogHandler :  public EventHandler
{
public:
	/**
	 * Method: FileDialogHandler::EventHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
	FileDialogHandler(TrecPointer<TInstance> instance);


	/**
	 * Method: FileDialogHandler::~EventHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~FileDialogHandler();


	/**
	 * Method: FileDialogHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 */
	virtual void Initialize(TrecPointer<Page> page) override;


	/**
	 * Method: FileDialogHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) override;


	/**
	 * Method: FileDialogHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
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
	// Relevent controls once controls are generated from a TML file

	TrecSubPointer<TControl, TTextField> directoryText;

	TrecSubPointer<TControl, TTreeDataBind> browserControl;

	TrecSubPointer<TControl, TTextField> fileText;

	TrecSubPointer<TControl, TLayout> browserLayout;

	TrecPointer<TControl> okayControl;


	TrecPointer<TFileShell> startDirectory;


	TString chosenPath, extensions;

	file_node_filter_mode filter_mode;

	TrecPointer<TWindow> window;

	bool allowCreateFile;


	/**
	 * Method: FileDialogHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;


	TDataArray<FileDialogEvents> fileEvents;

	void OnSelectNode(TrecPointer<TControl> tc, EventArgs ea);

	void OnCancel(TrecPointer<TControl> tc, EventArgs ea);

	void OnOkay(TrecPointer<TControl> tc, EventArgs ea);

	void OnFileNameChange(TrecPointer<TControl> tc, EventArgs ea);

	void OnClickNode(TrecPointer<TControl> tc, EventArgs ea);
};

