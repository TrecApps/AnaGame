#include "TerminalHandler.h"
#include "Page.h"
/**
 * Method: TerminalHandler::TerminalHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New Terminal Handler Object
 */
TerminalHandler::TerminalHandler(TrecPointer<TInstance> instance): EventHandler(instance)
{
}

/**
 * Method: TerminalHandler::~TerminalHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TerminalHandler::~TerminalHandler()
{
}

/**
 * Method: TerminalHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TerminalHandler::GetType()
{
	return TString(L"TerminalHandler;") + EventHandler::GetType();
}

/**
 * Method: TerminalHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 */
void TerminalHandler::Initialize(TrecPointer<Page> page)
{
	ThreadLock();
	if (!page.Get())
	{
		ThreadRelease();
		return;
	}
	auto root = page->GetRootControl();

	if (root.Get())
		currentTerminal = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TPromptControl>(root);
	ThreadRelease();
}

/**
 * Method: TerminalHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void TerminalHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

/**
 * Method: TerminalHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 */
void TerminalHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

/**
 * Method: TerminalHandler::GetTerminal
 * Purpose: Returns the Current Prompt held by the handler
 * Parameters: void
 * Returns: TrecSubPointer<TControl, TPromptControl> - the Command Prompt to work with
 */
TrecSubPointer<TControl, TPromptControl> TerminalHandler::GetTerminal()
{
	ThreadLock();
	auto ret = currentTerminal;
	ThreadRelease();
	return ret;
}

/**
 * Method: TerminalHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 */
bool TerminalHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	ThreadLock();
	bool ret =(!message.Get()) ? false : message->GetHandlerType() == handler_type::handler_type_console;
	ThreadRelease();
	return ret;
}
