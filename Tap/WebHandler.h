#pragma once
#include "EventHandler.h"
#include <TEnvironment.h>
class WebHandler :
    public EventHandler
{
public:

	/**
	 * Method: WebHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<Page> page) override;

	/**
	 * Method: WebHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) override;


	/**
	 * Method: WebHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) override;

	/**
	 * Method: WebHandler::WebHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TEnvironment> env - the environment to get interpretors from
	 *				TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New Web Handler Obejct
	 */
	WebHandler(TrecPointer<TEnvironment> env, TrecPointer<TInstance> instance);

	/**
	 * Method: WebHandler::HandleWebEvents
	 * Purpose: Web Based means of handling events, by taking a series of command
	 * Parameters: const TString& exp - the expression to run
	 *				TrecPointer<TVariable> ev - the variable representing the event
	 * Returns: void
	 *
	 * Note: The JavaScript Interpretor is assumed to be the default. To specify another interpretor type, prepend the expression with "[type] expression"
	 */
	void HandleWebEvents(const TString& exp, TrecPointer<TVariable> ev);


protected:

	/**
	 * Method: EventHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 *
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	/**
	 * The enviroment to get resources from
	 */
	TrecPointer<TEnvironment> environment;
};

