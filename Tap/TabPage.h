#pragma once
#include <TabBar.h>
#include "TWindow.h"


/**
 * Enum Class: ide_page_type
 * Purpose: Specifies the IDE Page's location in an IDE Window
 */
typedef enum class ide_page_type
{
	ide_page_type_main,				// This is the main Page
	ide_page_type_body,           // represents the main body of the Window
	ide_page_type_basic_console,  // represents the lower console, that fits between the side panels
	ide_page_type_deep_console,   // represents the deep lower console region
	ide_page_type_upper_right,    // represents the upper section of the right panel
	ide_page_type_lower_right,    // represents the lower section of the right panel
	ide_page_type_upper_left,     // represents the upper section of the left panel
	ide_page_type_lower_left,     // represents the lower section of the left panel
	ide_page_type_drag            // represents a Page that is held by the other pages and can be dragged around
}ide_page_type;

/**
 * Enum Class: page_move_mode
 * Purpose: Informs the IDE Page how it is expected to move
 */
typedef enum class tab_page_move_mode
{
	normal, // Move not expected
	drag,   // Page is being dragged around the IDE WIndow
	top,    // Page is expected to move it's top border
	bottom, // Page is expected to move it's bottom border
	left,   // Page is expected to move it's left border
	right   // Page is expected to move it's right border
}tab_page_move_mode;

/**
 * Class: TabPage
 * Purpose: Represents a Tabbable Page designed to hold other pages 
 * 
 * Note: Replaces the IDEPage class from this very library
 */
class TabPage : public TPage
{
	friend class TIdeWindow;
protected:
	/**
	 * Tab Bar holding the child Pages
	 */
	TabBar tabBar;

	/**
	 * height of the tab bar
	 */
	UCHAR tabHeight;

	/**
	 * Links to the Other Pages
	 */
	TrecSubPointer<TPage, TabPage> body;
	TrecSubPointer<TPage, TabPage> basicConsole;
	TrecSubPointer<TPage, TabPage> deepConsole;
	TrecSubPointer<TPage, TabPage> upperRight;
	TrecSubPointer<TPage, TabPage> lowerRight;
	TrecSubPointer<TPage, TabPage> upperLeft;
	TrecSubPointer<TPage, TabPage> lowerLeft;
	/**
	 * Current Page to draw
	 */
	TrecPointer<TPage> currentPage;

	/**
	 * History of Tabs
	 */
	TDataArray<TrecPointer<TPage>> previousPages;

	/**
	 * Window hoding this page
	 */
	TrecPointerSoft<TWindow> parentWindow;

	/**
	 * Type of IDE Page this object is
	 */
	ide_page_type type;
	/**
	 * What border is being moved, if any
	 */
	tab_page_move_mode moveMode;

	/**
	 * Border Brush
	 */
	TrecPointer<TBrush> borderBrush;

public:

	D2D1_RECT_F GetChildSpace();

	///
	/// Mandatory Methods in order to make this Class Non-abstract
	/// 

	/**
	 * Method: TabPage::TabPage
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> board - the drawing board to draw to
	 * Returns: new Tab Bar
	 */
	TabPage(ide_page_type t,TrecPointer<DrawingBoard> board, UCHAR tabHeight = 0);

	/**
	 * Method: TabPage::HandlesEvents
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
	 * Method: TabPage::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;


	/**
	 * Method: TabPage::OnRButtonUp
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
	 * Method: TabPage::OnRButtonDown
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
	 * Method: TabPage::OnLButtonUp
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
	 * Method: TabPage::OnLButtonDown
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
	 * Method: TabPage::OnMouseMove
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
	 * Method: TabPage::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TabPage::OnResize
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
	 * Method: TabPage::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&) override;


	///
	/// Methods Borrowed from Anaface 2 TSwitchControl
	/// 
	void SetView(TrecPointer<TPage> page, const TPoint& point);

	void RemovePage(TrecPointer<TPage> page);

	void SetView(TrecPointer<TPage> page, const TString& name);

	///
	///	Methods Borrowed from IDEPage Class
	/// 


	/**
	 * Method: IDEPage::IsDrawing
	 * Purpose: Reports whether the Page is currently able to draw
	 * Parameters: void
	 * Returns: bool - if the region is not zeroed
	 */
	bool IsDrawing();

	/**
	 * Method: TabPage::MoveBorder
	 * Purpose: Shofts the border when called upon by a neighboring page
	 * Parameters: float& magnitude - the amount to shift the border by
	 *				page_move_mode mode - which border to shift
	 * Returns: void
	 */
	void MoveBorder(float& magnitude, tab_page_move_mode mode);


	/**
	 * Method: TabPage::SetLink
	 * Purpose: Allows the IDE Window to link up it's pages so that they can directly communicate with each other
	 * Parameters: TrecSubPointer<Page, IDEPage> p - The Page to link to
	 *				ide_page_type t - the Specific purpose of the Page
	 * Returns: void
	 */
	void SetLink(TrecSubPointer<TPage, TabPage> p, ide_page_type t);

	void ClearPages();


	/**
	 * Method: Page::SetSelf
	 * Purpose: Allows TrecPointerKey to set a Trec Reference to itself
	 * Parameters: TrecPointer<Page> - self reference to the page
	 * Returns: void
	 */
	virtual void SetSelf(TrecPointer<TPage>)override;

	void SetTabActive(TrecPointer<TPage> page, bool active);

private:


	/**
	 * Method: TabPage::MouseMoveBody
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Body Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveBody(TPoint& diff);
	/**
	 * Method: TabPage::MouseMoveBasicConsole
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Basic-Console Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveBasicConsole(TPoint& diff);
	/**
	 * Method: TabPage::MouseMoveDeepConsole
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Deep-Console Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveDeepConsole(TPoint& diff);
	/**
	 * Method: TabPage::MouseMoveUpperRight
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Right Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveUpperRight(TPoint& diff);
	/**
	 * Method: TabPage::MouseMoveLowerRight
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Right Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveLowerRight(TPoint& diff);
	/**
	 * Method: TabPage::MouseMoveUpperLeft
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Left Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveUpperLeft(TPoint& diff);
	/**
	 * Method: TabPage::MouseMoveLowerLeft
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Left Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveLowerLeft(TPoint& diff);
};
