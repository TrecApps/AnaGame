#pragma once
#include "EventHandler.h"
#include <TDataLayout.h>

class FileHandler;

/**
 * Enum Class: handler_data_source
 * Purpose: Instructs Handler on where to get its data
 */
typedef enum class handler_data_source {
	hds_files,
	hds_project
}handler_data_source;

typedef void (FileHandler::* FileHandlerEvents)(TrecPointer<TPage> tc, EventArgs ea);

/**
 * Class: FileHandler
 * Purpose: Extends the Event Handler to Support File Management
 * 
 * SuperClass: EventHandler
 */
class _TAP_DLL FileHandler :	public TapEventHandler
{
public:

	/**
	 * Method: FileHandler::FileHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New FileHandler Object
	 */
	FileHandler(TrecPointer<TProcess> instance, handler_data_source dataSource = handler_data_source::hds_files);
	/**
	 * Method: FileHandler::~FileHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~FileHandler();


	/**
	 * Method: FileHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: FileHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<TPage> page) override;
	/**
	 * Method: FileHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)override;

	/**
	 * Method: FileHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;


protected:
	/**
	 * Method: FileHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 * 
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
	
	/**
	 * The control that actualy browses the File System
	 */
	TrecSubPointer<TPage, TDataLayout> browser;

	/**
	 * Holds list of EventHandlers specific for this Handler type
	 */
	TDataArray<FileHandlerEvents> fileHandlers;

	/**
	 * How the Handler should get its data for it's source
	 */
	handler_data_source dataSource;

	/**
	 * Method: FileHandler::OnOpenFile
	 * Purpose: Responds to a Double Click from the Control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnOpenFile(TrecPointer<TPage> tc, EventArgs ea);
};

