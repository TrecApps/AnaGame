#pragma once
#include "EventHandler.h"

/**
 * Class: TerminalHandler
 * Purpose: Extends the EventHandler class to manage Terminal Pages
 * 
 * SuperClass: EventHandler
 */
class _TAP_DLL TerminalHandler :
	public TapEventHandler
{
public:

	/**
	 * Method: TerminalHandler::TerminalHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New Terminal Handler Object
	 */
	TerminalHandler(TrecPointer<TProcess> instance, TrecPointer<TFileShell> wDirectory);

	/**
	 * Method: TerminalHandler::~TerminalHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TerminalHandler();


	/**
	 * Method: TerminalHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TerminalHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<TPage> page) override;

	/**
	 * Method: TerminalHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)override;

	/**
	 * Method: TerminalHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;


	/**
	 * Method: TerminalHandler::GetTerminal
	 * Purpose: Returns the Current Prompt held by the handler
	 * Parameters: void
	 * Returns: TrecSubPointer<TControl, TPromptControl> - the Command Prompt to work with
	 */
	TrecPointer<TConsoleHolder> GetTerminal();

private:

	TrecPointer<TFileShell> wDirectory;
	
	/**
	 * The current Command Prompt to fucus on
	 */
	TrecPointer<TConsoleHolder> currentTerminal;

	/**
	 * Method: TerminalHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 * 
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

