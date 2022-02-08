#pragma once
#include "EventHandler.h"
/**
 * Class: OkayHandler
 * Purpose: Extends the EventHandler to handle Okay Dialog events
 * 
 * SuperClass: EventHandler
 */
class OkayHandler :
	public TapEventHandler
{
public:

	/**
	 * Method: OkayHandler::OkayHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
	 * Returns: new OkayHandler Object
	 */
	OkayHandler(TrecPointer<TProcess> instance);

	/**
	 * Method: OkayHandler::~OkayHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~OkayHandler();

	/**
	 * Method: OkayHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: OkayHandler::Initialize
	 * Purpose: Basic Handler initialization
	 * Parameters: TrecPointer<Page> page - the page the Handler is to associate with
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	void Initialize(TrecPointer<TPage> page) override;

	/**
	 * Method: OkayHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)override;

	/**
	 * Method: OkayHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 * 
	 * Attributes: override
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
	 * 
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

