#pragma once
#include "DocumentHandler.h"

/**
 * Class: TCodeHandler
 * Purpose: Extends the Handler to hold a specific code file
 */
class _TAP_DLL TCodeHandler : public DocumentHandler
{
public:

	/**
	 * Method: TCodeHandler::TCodeHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
	TCodeHandler(TrecPointer<TInstance> in);

	/**
	 * Method: TCodeHandler::TCodeHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 *				const TString& name - the Name to give this handler
	 * Returns: New EventHandler Object
	 */
	TCodeHandler(TrecPointer<TInstance> in, const TString& name);


	/**
	 * Method: TCodeHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 */
	virtual void Initialize(TrecPointer<Page> page) override;

	/**
	 * Method: TCodeHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) override;


	/**
	 * Method: TCodeHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) override;


	/**
	 * Method: TCodeHandler::OnSave
	 * Purpose: Provides a Save method in case there is a way to save
	 * Parameters: void
	 * Returns: void
	 */
	virtual void OnSave()override;

protected:

	/**
	 * Method: TCodeHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	TrecSubPointer<TControl, TTextField> code, lines;


	/**
	 * Method: TCodeHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void OnSave(TFile& file)override;

	/**
	 * Method: TCodeHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	virtual void OnLoad(TFile&)override;
};

