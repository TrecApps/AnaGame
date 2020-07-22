#pragma once
#include "EventHandler.h"

class FileHandler;


typedef void (FileHandler::* FileHandlerEvents)(TrecPointer<TControl> tc, EventArgs ea);

/**
 * Class: FileHandler
 * Purpose: Extends the Event Handler to Support File Management
 */
class _TAP_DLL FileHandler :	public EventHandler
{
public:

	/**
	 * Method: FileHandler::FileHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New FileHandler Object
	 */
	FileHandler(TrecPointer<TInstance> instance);
	/**
	 * Method: FileHandler::~FileHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~FileHandler();

	/**
	 * Method: FileHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 */
	virtual void Initialize(TrecPointer<Page> page) override;
	/**
	 * Method: FileHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	/**
	 * Method: FileHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;


protected:
	/**
	 * Method: FileHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
	
	/**
	 * The control that actualy browses the File System
	 */
	TrecSubPointer<TControl, TTreeDataBind> browser;

	/**
	 * Holds list of EventHandlers specific for this Handler type
	 */
	TDataArray<FileHandlerEvents> fileHandlers;

	
	/**
	 * Method: FileHandler::OnOpenFile
	 * Purpose: Responds to a Double Click from the Control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnOpenFile(TrecPointer<TControl> tc, EventArgs ea);
};

