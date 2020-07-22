#pragma once
#include "EventHandler.h"

/**
 * Class: TerminalHander
 * Purpose: Extends the EventHandler class to manage Terminal Pages
 */
class _TAP_DLL TerminalHandler :
	public EventHandler
{
public:

	/**
	 * Method: TerminalHandler::TerminalHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New Terminal Handler Object
	 */
	TerminalHandler(TrecPointer<TInstance> instance);

	/**
	 * Method: TerminalHandler::~TerminalHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TerminalHandler();


	/**
	 * Method: TerminalHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 */
	virtual void Initialize(TrecPointer<Page> page) override;

	/**
	 * Method: TerminalHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	/**
	 * Method: TerminalHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

private:
	/**
	 * List of Terminals held 
	 */
	TMap<TControl> terminals;
	
	/**
	 * The current Command Prompt to fucus on
	 */
	TrecPointer<TControl> currentTerminal;

	/**
	 * Method: TerminalHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

