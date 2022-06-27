#pragma once

#include <TEnvironment.h>
#include <HtmlBuilder.h>
#include <TabBar.h>

#ifndef ag_msg
#define ag_msg
#endif
#include <AnafacePage.h>

/**
 * Class: WebPage
 * Purpose: Extends Page to Support Web-oriented Content
 * 
 * SuperClass: Page
 */
class WebPage : public TPage
{
	friend class TWebWindow;
public:

	/**
	 * Method: WebPage::WebPage
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> board - the board to draw upon
	 * Returns: New WebPage object
	 */
	WebPage(TrecPointer<DrawingBoard> board, TrecPointerSoft<TWindow> win);

	/**
	 * Method: WebPage::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType() override;


	/**
	 * Method: WebPage::~WebPage
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~WebPage();

	void SetEnvironment(TrecPointer<TEnvironment> env);


	/**
	 * Method: TPage::HandlesEvents
	 * Purpose: Whether the object is of a Page type that Handles Events (i.e. has what would be called an Event Handler, an object that
	 *      runs it's own methods in response to receiving an "event"
	 * Parameters: void
	 * Returns: bool - whether the page had an "Event Handler" (Top level Page types, such as 'TAnafacePage' or 'TWebPage' should report true while sub pages such as
	 *              'TControl' or 'TWebNode' should report false)
	 *
	 * Attributes: abstract
	 */
	virtual bool HandlesEvents() override;

	/**
	 * Method: TPage::PrepPage
	 * Purpose: Allows top-level Pages to set up subordinate Pages and manage event handling
	 * Parameters: TrecPointer<TFileShell> file - the file to read
	 *				TrecPointer<EventHandler> handler - the event handler to use
	 * Returns: TString - error information (empty string means no errors)
	 *
	 * Attributes: virtual
	 */
	virtual TString PrepPage(TrecPointer<TFileShell> file, TrecPointer<EventHandler> handler)override;


	/**
	 * Method: TPage::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;


	/**
	 * Method: TPage::OnRButtonUp
	 * Purpose: Responds to the Right Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: TPage::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: TPage::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: TPage::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TPage::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TPage::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TPage::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: Page::OnDestroy
	 * Purpose: Reports whether the Page is ready to be destroyed
	 * Parameters: void
	 * Returns: bool - true if the Page doesn't have a handler or that handler is ready to be destroyed
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual bool OnDestroy() override;


	/**
	 * Method: TPage::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual bool OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&) override;


	// Because this Object more or less represents Anagame's version of the JavaScript Web DOM, here are methods
	// Supporting that API, which need to be wrapped around by TNativeInterpretor functions from the TcDataStruct library

	/**
	 * Method: WebPage::GetElementById
	 * Purpose: Internal Anagame implementation of the DOM.getElementById method
	 * Parameters: const TString& id - the ID of the requested HTML node
	 * Returns: TrecPointer<TWebNode> - the Requested node
	 */
	TrecPointer<TWebNode> GetElementById(const TString& id);

	/**
	 * Method: WebPage::GetElementsByTag
	 * Purpose: Internal Anagame implementation of the DOM.getElementsByTagName method
	 * Parameters: const TString& id - the tag name of the requested HTML nodes
	 * Returns: TDataArray<TrecPointer<TWebNode>> - the list of Requested nodes
	 */
	TDataArray<TrecPointer<TWebNode>> GetElementsByTag(const TString& id);

	/**
	 * Method: WebPage::GetElementsByClass
	 * Purpose: Internal Anagame implementation of the DOM.getElementsByClassName method
	 * Parameters: const TString& id - the class name of the requested HTML nodes
	 * Returns: TDataArray<TrecPointer<TWebNode>> - the list of Requested nodes
	 */
	TDataArray<TrecPointer<TWebNode>> GetElementsByClass(const TString& id);

	void Open();

	void Write(const TString& markup);

	void Close();

	/**
	 * Method: WebPage::GetTitle
	 * Purpose: Retirves the title of this web page
	 * Parameters: void
	 * Returns TString - the title of the web page
	 * 
	 * Note: This value could be an empty String. It is up to the caller to check for that and handle it accordingly
	 */
	TString GetTitle();

protected:

	void PrepFiles(TDataArray<TcWebNode::FileRequest> req);

	TString directory;

	TString SetUpCSS();

	TString PrepScripts();

	TrecPointer<TArray<styleTable>> styles;

	TrecPointer<HtmlBuilder> htmlBuilder;

	TrecPointer<TEnvironment> environment;

	TrecSubPointer<TPage, TcWebNode> rootNode, focusNode;

	TrecSubPointer<TPage, AnafacePage> anafaceFallBack;

	TrecSubPointer<TPage, TabBar::Tab> tab;

	TrecPointer<EventHandler> handler;

	TDataArray<TrecPointer<TWebNode>> clickNodes, moveNodes;

	TrecPointerSoft<TWindow> windowHandle;
};
