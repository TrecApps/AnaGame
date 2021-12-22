#pragma once
#include <TPage.h>
#include "Anaface2.h"

typedef struct LayoutData
{
	bool isLayout;
	int col, row, endCol, endRow;
}LayoutData;

/**
 * Class: AnafacePage
 * Purpose: Serves as the root page for Anaface 2 - UIs and holds an Event Handler
 */
class _ANAFACE_DLL2 AnafacePage : public TPage
{
public:
	/**
	 * Method: AnafacePage::AnafacePage
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> board - the drawing board to draw against
	 * Returns: new AnafacePage Object
	 */
	AnafacePage(TrecPointer<DrawingBoard> board);

	/**
	 * Method: AnafacePage::HandlesEvents
	 * Purpose: Whether the object is of a Page type that Handles Events (i.e. has what would be called an Event Handler, an object that
	 *      runs it's own methods in response to receiving an "event"
	 * Parameters: void
	 * Returns: bool - whether the page had an "Event Handler" (Top level Page types, such as 'TAnafacePage' or 'TWebPage' should report true while sub pages such as
	 *              'TControl' or 'TWebNode' should report false)
	 *
	 * Attributes: override
	 */
	virtual bool HandlesEvents() override;

	/**
	 * Method: AnafacePage::PrepPage
	 * Purpose: Allows top-level Pages to set up subordinate Pages and manage event handling
	 * Parameters: TrecPointer<TFileShell> file - the file to read
	 *				TrecPointer<EventHandler> handler - the event handler to use
	 * Returns: TString - error information (empty string means no errors)
	 *
	 * Attributes: virtual
	 */
	virtual TString PrepPage(TrecPointer<TFileShell> file, TrecPointer<EventHandler> handler) override;

	/**
	 * Method: AnafacePage::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;


	/**
	 * Method: AnafacePage::OnRButtonUp
	 * Purpose: Responds to the Right Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: AnafacePage::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: AnafacePage::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: AnafacePage::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: AnafacePage::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: AnafacePage::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& ) override;

	/**
	 * Method: AnafacePage::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: Page::OnDestroy
	 * Purpose: Reports whether the Page is ready to be destroyed
	 * Parameters: void
	 * Returns: bool - true if the Page doesn't have a handler or that handler is ready to be destroyed
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnDestroy() override;


	/**
	 * Method: AnafacePage::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool,const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: AnafacePage::InjectScrollerPage
	 * Purpose: Inserts a Scrolling Page between the parent page and the calling Page
	 * Parameters: const D2D1_RECT_F& bounds - the bounds of the calling page
	 *				TrecPointer<TPage> page - the calling page
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void InjectScrollerPage(const D2D1_RECT_F& bounds, const D2D1_RECT_F& needs, TrecPointer<TPage> page) override;

	/**
	 * Method: AnafacePage::GetRootControl
	 * Purpose: Retrieves the Root Control for operations
	 * Parameters: void
	 * Returns: TrecPointer<TPage> page - the page generated
	 */
	TrecPointer<TPage> GetRootControl();


	/**
	 * Method: TPage::GetHandler
	 * Purpose: Retrieves the Handler he page holds (if the page is holding a handler)
	 * Parameters: void
	 * Returns: TrecPointer<EventHandler> - the handler the page holds
	 *
	 * Attributes: override

	 */
	virtual TrecPointer<EventHandler> GetHandler() override;

protected:
	/**
	 * The root Page (TControl)
	 */
	TrecPointer<TPage> rootPage;

	/**
	 * Handles Events
	 */
	TrecPointer<EventHandler> handler;

	/**
	 * List of Styles
	 */
	TrecPointer<TArray<styleTable>> styles;


protected:
	// Methods used for construction

	void HandleNode(const TString& name, TString& result, TrecPointer<TVariable> var, TrecPointer<TPage>& curPage, LayoutData& ld);

	TrecPointer<TPage> HandleControl(const TString& name, TString& result, TrecPointer<TVariable> var, LayoutData& ld);

	void HandleAttributes(TString& result, TrecPointer<TPage>& curPage, TrecPointer<TVariable> var, LayoutData& ld);

	void HandleDimensionList(bool row, TString& result, TrecPointer<TPage>& curPage, TrecPointer<TVariable> var);

	//void 
};

