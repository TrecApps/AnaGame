#include "OkayHandler.h"
#include "TWindow.h"

/**
 * Method: OkayHandler::OkayHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
 * Returns: new OkayHandler Object
 */
OkayHandler::OkayHandler(TrecPointer<TProcess> instance): TapEventHandler(instance, TString(L"Okay"))
{

}

/**
 * Method: OkayHandler::~OkayHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
OkayHandler::~OkayHandler()
{
	int e = 0;
}

/**
 * Method: OkayHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString OkayHandler::GetType()
{
	return TString(L"OkayHandler;") + EventHandler::GetType();
}

/**
 * Method: OkayHandler::Initialize
 * Purpose: Basic Handler initialization
 * Parameters: TrecPointer<Page> page - the page the Handler is to associate with
 * Returns: void
 */
void OkayHandler::Initialize(TrecPointer<TPage> page)
{
	ThreadLock();
	assert(page.Get());
	this->page = page;
}

/**
 * Method: OkayHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void OkayHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	ThreadLock();
	bool markDestroy = false; EventArgs ea;
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		auto cont = eventAr[Rust].args;
		if (!cont.Get())
			continue;
		if (!cont->methodID.Compare(L"OnOkay"))
			markDestroy = true;
		eventAr[Rust].args.Nullify();
	}

	if (markDestroy)
		OnOkay(nullptr, ea);
	ThreadRelease();
}

/**
 * Method: OkayHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 */
void OkayHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{

}

/**
 * Method: OkayHandler::OnOkay
 * Purpose: Prepares the Attached Dialog for destruction
 * Parameters: TControl* control - Pointer to control that raised the event
 *				EventArgs ea - information about the event
 * Returns: void
 */
void OkayHandler::OnOkay(TControl* control, EventArgs ea)
{
	ThreadLock();
	if (!page.Get())
	{
		ThreadRelease();
		throw L"Error! Handler expected pointer to a page!";
	}
	if (!window.Get())
	{
		ThreadRelease();
		throw L"Error! Handler's Page Object returned a NULL window handle";
	}
	DestroyWindow(window->GetWindowHandle());
	ThreadRelease();
}

/**
 * Method: OkayHandler::ShouldProcessMessageByType
 * Purpose: Reports whether the message is of the Okay Handler type
 * Parameters: TrecPointer<HandlerMessage> message -  the message to examine
 * Returns: bool - whether the message porports to belong the Okay Handler
 */
bool OkayHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	ThreadLock();
	bool ret = (!message.Get()) ? false : message->GetHandlerType() == handler_type::handler_type_okay;
	ThreadRelease();
	return ret;
}
