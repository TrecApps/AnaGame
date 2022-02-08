#pragma once
#include <EventHandler.h>
#include <TWebWindow.h>
#include <TSwitchControl.h>
class WebToursHandler :
    public TapEventHandler
{
public:
	/**
	 * Method: WebToursHandler::WebToursHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
	WebToursHandler(TrecPointer<TProcess> instance);

	/**
	 * Method: WebToursHandler::~WebToursHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~WebToursHandler();


	/**
	 * Method: WebToursHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 *
	 * Attributes: override
	 */
	virtual TString GetType()override;


	/**
	 * Method: WebToursHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<TPage> page) override;


	/**
	 * Method: WebToursHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr) override;

	/**
	 * Method: WebToursHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) override;

	/**
	 * Method: TapEventHandler::SetWindow
	 * Purpose: Sets the Window of this Handler
	 * Parameters: TrecPointer<TWindow> window - the window to set
	 * Returns: void
	 */
	virtual void SetWindow(TrecPointer<TWindow> window)override;


	virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, message_output* mOut)override;

protected:

	void PrepWindow();

	/**
	 * Method: EventHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 *
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	TrecSubPointer<TPage, TTextInput> urlBox;
	TrecSubPointer<TPage, TSwitchControl> tabs;
};

