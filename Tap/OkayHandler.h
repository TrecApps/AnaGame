#pragma once
#include "EventHandler.h"
/**
 * Class: OkayHandler
 * Purpose: Extends the EventHandler to handle Okay Dialog events
 */
class OkayHandler :
	public EventHandler
{
public:

	/**
	 * Method: OkayHandler::OkayHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
	 * Returns: new OkayHandler Object
	 */
	OkayHandler(TrecPointer<TInstance> instance);

	/**
	 * Method: OkayHandler::~OkayHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~OkayHandler();


	/**
	 * Method: OkayHandler::Initialize
	 * Purpose: Basic Handler initialization
	 * Parameters: TrecPointer<Page> page - the page the Handler is to associate with
	 * Returns: void
	 */
	void Initialize(TrecPointer<Page> page) override;

	/**
	 * Method: OkayHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	void HandleEvents(TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: OkayHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

protected:

	/**
	 * Method: OkayHandler::OnOkay
	 * Purpose: Prepares the Attached Dialog for destruction
	 * Parameters: TControl* control - Pointer to control that raised the event
	 *				EventArgs ea - information about the event
	 * Returns: void
	 */
	void OnOkay(TControl* control, EventArgs ea);

	/**
	 * Method: OkayHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether the message is of the Okay Handler type
	 * Parameters: TrecPointer<HandlerMessage> message -  the message to examine
	 * Returns: bool - whether the message porports to belong the Okay Handler
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

