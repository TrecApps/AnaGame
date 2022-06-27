#pragma once
#include "EventHandler.h"
class TInterpretorEventHandler :
    public TapEventHandler
{
public:
	/**
	 * Method: TInterpretorTInterpretorEventHandler::TInterpretorEventHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
	TInterpretorEventHandler(TrecPointer<TProcess> instance);


	/**
	 * Method: TInterpretorEventHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<TPage> page) override;


	/**
	 * Method: TInterpretorEventHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr) override;

	/**
	 * Method: TInterpretorEventHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) override;

	/**
	 * Method: TInterpretorEventHandler::HandleEvents
	 * Purpose: Handles Expressions produced from a set of WebNdes
	 * Parameters: const TDataArray<TString>& script - script to follow
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void HandlerEvents(const TDataArray<TString>& script) = 0;


protected:
	/**
	 * Method: TInterpretorEventHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 *
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

