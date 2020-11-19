#include "WebToursHandler.h"
#include <Page.h>

/**
 * Method: WebToursHandler::EventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New EventHandler Object
 */
WebToursHandler::WebToursHandler(TrecPointer<TInstance> instance) :EventHandler(instance)
{

}

/**
 * Method: WebToursHandler::~WebToursHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
WebToursHandler::~WebToursHandler()
{
}

/**
 * Method: WebToursHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 *
 * Attributes: override
 */
TString WebToursHandler::GetType()
{
	return TString(L"WebToursHandler;") + EventHandler::GetType();
}

/**
 * Method: WebToursHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 *
 * Attributes: override
 */
void WebToursHandler::Initialize(TrecPointer<Page> page)
{
	this->page = page;

	window = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TWebWindow>(page->GetWindowHandle());

	TrecSubPointer<TControl, TLayout> root = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(page->GetRootControl());

	assert(root.Get() && window.Get());

	urlBox = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(root->GetLayoutChild(3, 0));
}

/**
 * Method: WebToursHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 *
 * Attributes: override
 */
void WebToursHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

/**
 * Method: WebToursHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 *
 * Attributes: override
 */
void WebToursHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

bool WebToursHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	if (urlBox.Get() && urlBox->isOnFocus())
	{
		if (fromChar && nChar == VK_RETURN)
		{
			window->AddNewTab(urlBox->GetText());
			TTextField::RemoveFocus();
		}


		
	}
	return false;
}

/**
 * Method: EventHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 *
 * Attributes: override
 */
bool WebToursHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}
