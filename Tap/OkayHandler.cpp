#include "OkayHandler.h"
#include "Page.h"
#include "TWindow.h"

/**
 * Method: OkayHandler::OkayHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
 * Returns: new OkayHandler Object
 */
OkayHandler::OkayHandler(TrecPointer<TInstance> instance): EventHandler(instance, TString(L"Okay"))
{
	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(L"OnOkay");
	events.push_back(enid);

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
 * Method: OkayHandler::Initialize
 * Purpose: Basic Handler initialization
 * Parameters: TrecPointer<Page> page - the page the Handler is to associate with
 * Returns: void
 */
void OkayHandler::Initialize(TrecPointer<Page> page)
{
	assert(page.Get());
	this->page = page;
	auto tempApp = page->GetInstance();
	app = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(tempApp);
}

/**
 * Method: OkayHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void OkayHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
	bool markDestroy = false; EventArgs ea;
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		auto cont = eventAr[Rust].control;
		if (!cont.Get()) continue;

		ea = cont->getEventArgs();

		if (ea.methodID == 0)
		{
			markDestroy = true;
		}
	}

	if (markDestroy)
		OnOkay(nullptr, ea);
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
	if (!page.Get())
		throw L"Error! Handler expected pointer to a page!";
	TrecPointer<TWindow> windHandle = page->GetWindowHandle();
	if (!windHandle.Get())
		throw L"Error! Handler's Page Object returned a NULL window handle";

	DestroyWindow(windHandle->GetWindowHandle());
}

/**
 * Method: OkayHandler::ShouldProcessMessageByType
 * Purpose: Reports whether the message is of the Okay Handler type
 * Parameters: TrecPointer<HandlerMessage> message -  the message to examine
 * Returns: bool - whether the message porports to belong the Okay Handler
 */
bool OkayHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;

	return message->GetHandlerType() == handler_type::handler_type_okay;
}
