#include "WebToursHandler.h"
#include <TPage.h>
#include <TLayout.h>

/**
 * Method: WebToursHandler::EventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New EventHandler Object
 */
WebToursHandler::WebToursHandler(TrecPointer<TProcess> instance) :TapEventHandler(instance)
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
void WebToursHandler::Initialize(TrecPointer<TPage> page)
{
	this->page = page;

	assert(dynamic_cast<AnafacePage*>(page.Get()));

	TrecSubPointer<TPage, TLayout> rootLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>( dynamic_cast<AnafacePage*>(page.Get())->GetRootControl() );

	assert(rootLayout.Get());

	tabs = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TSwitchControl>(rootLayout->GetPage(1, 0));

	assert(tabs.Get());
	PrepWindow();

	rootLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(rootLayout->GetPage(0, 0));

	assert(rootLayout.Get());

	urlBox = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(rootLayout->GetPage(0, 2));
}

/**
 * Method: WebToursHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 *
 * Attributes: override
 */
void WebToursHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	TapEventHandler::HandleEvents(eventAr);
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

void WebToursHandler::SetWindow(TrecPointer<TWindow> window)
{
	TapEventHandler::SetWindow(window);
	PrepWindow();
}

bool WebToursHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, message_output* mOut)
{
	if (urlBox.Get() && textIntercepter.Get() && urlBox->GetInterceptor().Get() == textIntercepter.Get())
	{
		if (fromChar && nChar == VK_RETURN)
		{
			if (dynamic_cast<TWebWindow*>(window.Get()) && tabs.Get())
			{
				dynamic_cast<TWebWindow*>(window.Get())->AddNewTab(urlBox->GetText(), false);
				return true;
			}
		}
	}

	/*if (urlBox.Get() && urlBox->isOnFocus())
	{
		if (fromChar && nChar == VK_RETURN)
		{
			window->AddNewTab(urlBox->GetText(), false);
			TTextField::RemoveFocus();
		}



	}*/
	return TapEventHandler::OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
}

void WebToursHandler::PrepWindow()
{
	if (dynamic_cast<TWebWindow*>(window.Get()) && tabs.Get())
		dynamic_cast<TWebWindow*>(window.Get())->SetTabBar(tabs->GetTabBar());
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
