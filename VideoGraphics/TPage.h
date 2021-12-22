#pragma once

#include <TVariable.h>
#include "DrawingBoard.h"
#include <TPoint.h>
#include "TTextIntercepter.h"
#include <TMap.h>
#include <TObjectNode.h>
#include "TTextElement.h"
#define ag_msg

/**
 * Enum Class: R_Message_Type
 * Purpose: Determines the message type for UI Responsiveness
 */
typedef enum class R_Message_Type
{
	On_L_Button_Down,  // User presses down on left mouse button
	On_L_Button_Up,    // User releases left mouse button
	On_R_Button_Down,  // User presses down on right mouse button
	On_R_Button_Up,    // User releases right mouse button
	On_Click,          // User presses down and releases left mouse button
	On_Hold_Click,     // (Not implemented)
	On_Hover,          // Mouse is over the control
	On_Hover_Enter,		// Mouse just enetered the control's space
	On_Hover_Leave,    // Mouse leaves the control
	On_Right_Click,    // User presses down and releases right mouse button
	On_Click_Release,  // (deprecated)
	On_Text_Change,    // The text of this control has changed
	On_Right_Release,  // (deprecated)
	On_sel_change,     // A Selection has changed
	On_check,          // a Checkbox status has been changed
	On_radio_change,   // A Radio Button group has been updated
	On_Char,           // A Character has been entered
	On_Focus,          // A Control is now the focus
	On_Lose_Focus,     // A Control no longer has focus
	On_Select_Scroller,// A Scroll bar is involved
	On_Flyout,         // A control is to appear in a flyout
	On_LDoubleClick,    // Double Click detected
	On_Scrolled,		// Control was scrolled
	On_Resized,
	On_SubmitDrag		// Added to Event-Cred when a Page is being dragged around
}R_Message_Type;

/**
 * Class: EventArgs
 * Purpose: Event Arguements, used to supply Event Handlers with the necessary information to
 *		do their job properly
 */
class _VIDEO_GRAPHICS EventArgs
{
public:
	EventArgs();
	void Reset();
	EventArgs(const EventArgs& args);

	TString text;
	bool positive;
	TPoint point;
	bool isClick;
	bool isLeftClick;
	R_Message_Type eventType;
	// TrecPointer<TPage> control;
	TString methodID;
	int arrayLabel;
	WCHAR type;
	TrecPointer<TObjectNode> object;
	D2D1_RECT_F newSize, oldSize;
};

/**
 * Class: styleTable
 * Purpose: Holds the list of styles according to name, useful in declaring 'classes' in Anaface
 */
class _VIDEO_GRAPHICS styleTable : public TObject
{
public:
	TString style;
	TMap<TString> names;
};


/**
 * Class: Dimensions
 * Purpose: Controls the size of a Given TControl
 */
class _VIDEO_GRAPHICS Dimensions
{
public:
	Dimensions();
	UINT width;
	UINT height;
	UINT minHeight;
	UINT minWidth;
	UINT maxWidth;
	UINT maxHeight;
};

/** 
 * Enum Class: message_output
 * Purpose: enables easy tracking of the results of sending a message
 */
typedef enum class message_output
{
    mo_negative, // message has not been intercepted
    mo_positive_continue, // message has been intercepted by a child page but parent page is free to add its own events
    mo_positive_override, // message has been intercepted by a child page and parent should take no further action
    mo_positive_handled, // message has been intercepted by a child page and has handled events generated
	mo_positive_scroll   // Message was intercepted by a Scroll Bar
}message_output;

/**
 * Enum Class: messageOScrollOrientutput
 * Purpose: Tracks the orientation of the scroll bar
 */
typedef enum class ScrollOrient
{
	so_vertical, // ScrollBar is Vertical (most common)
	so_horizontal // Scrollbar is Horizontal (less common but still possible)
}ScrollOrient;



/**
 * Class: TPage
 * Purpose: Establishes the Interface for managing content and allows Anagame and other vendors the ability to build their own UI Engines
 *  in addition to Anagame's built-in Anaface (Basic) or AnaWeb (HTML-based) engines and establish a means of responding to input directed at the page's
 *  space
 * 
 * Note: This Class was added to add abstraction and modularity to how Anagame manages controls and UI/UX, thus making it possible for Anagame to interact with 
 *      a UI without regards for whether it is dealing with a built-in UI (such as Anaface or AnaWeb) or third-party UI (such as a hypothetical 'React for Anagame')
 */
class _VIDEO_GRAPHICS TPage :
    public TVObject
{
public:
    // Classes that support the TPage

	/**
	 * Class: TScrollBar
	 * Purpose: Implements and renders the scroll-bar
	 *
	 * SuperClass: TObject
	 */
	class _VIDEO_GRAPHICS TScrollBar : public TObject
	{
	public:

		/**
		 * Method: TScrollBar::GetType
		 * Purpose: Returns a String Representation of the object type
		 * Parameters: void
		 * Returns: TString - representation of the object type
		 */
		virtual TString GetType()override;

		/**
		 * Method: TScrollBar::TScrollBar
		 * Purpose: Constructor
		 * Parameters: TControl& control - The control holding this scroll bar
		 *				ScrollOrient so - whether this is a horizontal or vertical scroll bar
		 * Returns: new Scroll Bar
		 */
		TScrollBar(TrecPointer<TPage> control, ScrollOrient so);
		/**
		 * Method: TScrollBar::~TScrollBar
		 * Purpose: Destructor
		 * Parameters: void
		 * Returns: void
		 */
		~TScrollBar();

		/**
		 * Method: TScrollBar::onDraw
		 * Purpose: Draws the scroll bar
		 * Parameters: ID2D1RenderTarget* target -  the target to draw with
		 * Returns: void
		 */
		void onDraw(ID2D1RenderTarget* target);

		/**
		 * Method: TScrollBar::OnLButtonDown
		 * Purpose: Allows bars to assume focus
		 * Parameters:UINT nFlags - redundant
		 *				TPoint point - The point where the user clicked
		 *				message_output* mOut - redundant
		 * Returns: whether the focus is on this control or not
		 *
		 * Attributes: virtual
		 */
		virtual bool OnLButtonDown(UINT nFlags, TPoint point, message_output& mOut);
		/**
		 * Method: TScrollBar::OnLButtonUp
		 * Purpose: Lets the scroll bar know that the user has unclicked and thus can no longer have any focus
		 * Parameters: UINT nFlags - redundant
		 *				TPoint point - redundant
		 *				message_output* mOut - redundant
		 * Returns: void
		 *
		 * Attributes: virtual
		 */
		virtual void OnLButtonUp(UINT nFlags, TPoint point, message_output& mOut);
		/**
		 * Method: TScrollBar::OnMouseMove
		 * Purpose: Allows the scroll bar to shift the contents along with the user (if focused upon)
		 * Parameters: UINT nFlags - redundant
		 *				TPoint point - current point the mouse is at
		 *				message_output* mOut - redundant
		 * Returns: void
		 *
		 * Attributes: virtual
		 */
		virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut);

		/**
		 * Method: TScrollBar::MovedContent
		 * Purpose: Calculates how much to move the content by
		 * Parameters: float degree - how much the user moved the mouse
		 * Returns: float - how much the content itself has to move
		 */
		float MovedContent(float degree);

		/**
		 * Method: TScrollBar::Refresh
		 * Purpose: Refreshes the scroll bars so that they are callibrated correctly
		 * Parameters: const D2D1_RECT_F& location - the location of the area that is allowed to be drawn
		 *				 const D2D1_RECT_F& area - the location of the region that the control[s] underneath actually take up
		 * Returns: void
		 */
		void Refresh(const D2D1_RECT_F& location, const D2D1_RECT_F& area);

	private: // Static attributes, styles that should be shared across all Scroll Bars

		/**
		 * Method: TScrollBar::EstablishScrollColors
		 * Purpose: Establishes the colors used in the scroll bars
		 * Parameters: void
		 * Returns: void
		 *
		 * Note: Called once by the constructor
		 */
		static void EstablishScrollColors();

		/**
		 * Two imporant rectangles to draw
		 */
		D2D1_RECT_F body_rect, scroll_rect;

		/**
		 * Parent holding this Scroll Bar
		 */
		TrecPointerSoft<TPage> parent;
		/**
		 * Used to track movement of the mouse to determine how much to shift by
		 */
		TPoint prevPoint;
		/**
		 * Whether the bar is being focused on (drawable currently not used)
		 */
		bool onFocus, drawable;
		/**
		 * Distance between the regions and the ratio provided
		 */
		float diff1, diff2, widthFactor;
		/**
		 * Vertical or horizontal
		 */
		ScrollOrient scrollAlignment;
	};


	/**
	 * Class: EventID_Cred
	 * Purpose: Used by controls to register themselves in the message queue if they have a message handler
	 */
	class _VIDEO_GRAPHICS EventID_Cred
	{
	public:
		EventID_Cred();
		EventID_Cred(const EventID_Cred& copy);
		EventID_Cred(R_Message_Type t, TrecPointer<TPage> c);
		EventID_Cred(R_Message_Type t, TrecPointer<TPage> c, TrecPointer<TScrollBar> sb);
		EventID_Cred(R_Message_Type t, TrecPointer<TPage> c, TrecPointer<TTextIntercepter> i);


		R_Message_Type eventType;
		TrecPointer<TPage> control;
		TrecPointer<TScrollBar> scroll;
		// TrecPointer<TFlyout> flyout; // To-Do: Re add later
		TrecPointer<TTextIntercepter> textIntercepter;
		TrecPointer<TVariable> data;		// Holds either a variable, or an interpretor to handle an event
		TString expression;

		TrecPointer<EventArgs> args;
	};

	/**
	 * Class: EventHandler
	 * Purpose: Provides means of responding to events generated by Pages and their contents
	 */
	class _VIDEO_GRAPHICS EventHandler: public TObject
	{
	public:

		/**
		 * Method: EventHandler::Initialize
		 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
		 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
		 * Returns: void
		 *
		 * Attributes: abstract
		 */
		virtual void Initialize(TrecPointer<TPage> page) = 0;

		/**
		 * Method: EventHandler::OnChar
		 * Purpose: Handles Character input from the Instance of Anagame
		 * Parameters: bool fromChar - true if it came form On_Char from windows, false if it came from a key_down 
		 *				UINT nChar - the character submitted
		 *				UINT nRepCnt - number od times to add the character
		 *				UINT nFlags - unused
		 *				message_output* mOut - result
		 * Returns: bool
		 */
		virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, message_output* mOut) = 0;

		virtual void SetSelf(TrecPointer<TPage::EventHandler> handleSelf) = 0;

		virtual TrecPointer<TPage> GetPage() = 0;

		virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr) = 0;

		virtual TrecPointer<TTextIntercepter> GetTextIntercepter() = 0;
	};


public:


	/**
	 * Method: TPage::GetVariable
	 * Purpose: Revtrieves a value in Variable form
	 * Parameters: const TString& name - the name of the variable
	 *              TrecPointer<TVariable>& var - holds the variable retrieved if valid
	 * Returns: bool - whether the variable was valid or not
	 *
	 * Attributes: override
	 */
	virtual bool GetVariable(const TString& name, TrecPointer<TVariable>& var) override;

	/**
	 * Method: TPage::SetVariable
	 * Purpose: Sets a value in Variable form
	 * Parameters: const TString& name - the name of the variable
	 *              TrecPointer<TVariable> var - the value to set
	 * Returns: bool - whether the variable was valid or not
	 *
	 * Attributes: override
	 */
	virtual bool SetVariable(const TString& name, TrecPointer<TVariable> var) override;

	/**
	 * Method: TPage::IsScroller
	 * Purpose: Reports whether this page supports Scollong
	 * Parameters: void
	 * Returns: bool - whether this Page scrolls
	 * 
	 * Attributes: virtual
	 */
	virtual bool IsScroller();

    /**
     * Method: TPage::TPage
     * Purpose: Constructor
     * Parameters: TrecPointer<DrawingBoard> board - the drawing board to draw against
     * Returns: new TPage Object
     */
    TPage(TrecPointer<DrawingBoard> board);

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
    virtual bool HandlesEvents() = 0;

	/**
	 * Method: TPage::PrepPage
	 * Purpose: Allows top-level Pages to set up subordinate Pages and manage event handling
	 * Parameters: TrecPointer<TFileShell> file - the file to read
	 *				TrecPointer<EventHandler> handler - the event handler to use
	 * Returns: TString - error information (empty string means no errors)
	 * 
	 * Attributes: virtual
	 */
	virtual TString PrepPage(TrecPointer<TFileShell> file, TrecPointer<EventHandler> handler);


    /**
     * Method: TPage::Draw
     * Purpose: Draws the Page to the Window
     * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
     * Returns: void
     * 
     * Attributes: abstract
     */
    virtual void Draw(TrecPointer<TVariable> object) = 0;


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
    ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) = 0;


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
	ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) = 0;


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
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) = 0;


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
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) = 0;

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
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) = 0;

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
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& eventAr) = 0;

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
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) = 0;

	/**
	 * Method: Page::OnDestroy
	 * Purpose: Reports whether the Page is ready to be destroyed
	 * Parameters: void
	 * Returns: bool - true if the Page doesn't have a handler or that handler is ready to be destroyed
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual bool OnDestroy() = 0;


	/**
	 * Method: TPage::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual bool OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&) = 0;


	/**
	 * Method: Page::SetSelf
	 * Purpose: Allows TrecPointerKey to set a Trec Reference to itself
	 * Parameters: TrecPointer<Page> - self reference to the page
	 * Returns: void
	 */
	void SetSelf(TrecPointer<TPage>);

    /**
     * Method: TPage::GetParentPage
     * Purpose: Reports the Parent Page responsible for holding this page
     * Parameters: void
     * Returns: TrecPointer<TPage> - the Parent Page (Null if top level or of the Parent Page has not been Set)
     */
    TrecPointer<TPage> GetParentPage();

    /**
     * Method: TPage::SetParentPage
     * Purpose: Set the Parent Page holding this Page
     * Parameters: TrecPointer<TPage> parent - the page to use as a Parent
     * Returns: void
     */
    void SetParentPage(TrecPointer<TPage> parent);

    /**
     * Method: TPage::GetArea
     * Purpose: Retrieves the Area held by the Page
     * Parameters: void
     * Returns: D2D1_RECT_F -  the Region the Page claims it is drawing to
	 * 
	 * Attributes: virtual
     */
    virtual D2D1_RECT_F GetArea();


	/**
	 * Method: TPage::RotateDegrees
	 * Purpose: Rotates the Control
	 * Parameters: float degrees - the angle to rotate the Control by (in degrees)
	 * Returns: void
	 */
	void RotateDegrees(float degrees);
	/**
	 * Method: TPage::RotateRadians
	 * Purpose: Rotates the Control
	 * Parameters: float radians - the angle to rotate the Control by (in degrees)
	 * Returns: void
	 */
	void RotateRadians(float radians);


	/**
	 * Method: TPage::InjectScrollerPage
	 * Purpose: Inserts a Scrolling Page between the parent page and the calling Page
	 * Parameters: const D2D1_RECT_F& bounds - the bounds of the calling page
	 *				TrecPointer<TPage> page - the calling page
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void InjectScrollerPage(const D2D1_RECT_F& bounds, const D2D1_RECT_F& needs, TrecPointer<TPage> page);

	/**
	 * Method: TPage::GetHandler
	 * Purpose: Retrieves the Handler he page holds (if the page is holding a handler)
	 * Parameters: void
	 * Returns: TrecPointer<EventHandler> - the handler the page holds
	 * 
	 * Attributes: virtual
	 * 
	 * Note: Not all page types are capable of holding an event Handler. A proper implementation of a page that does would return true
	 *		upon a call to 'HandlesEvents' and return the attribute it has
	 */
	virtual TrecPointer<EventHandler> GetHandler();

protected:


    /**
     * The Drawing Board to Draw against
     */
    TrecPointer<DrawingBoard> drawingBoard;

    /**
     * The Area to which this Page is assigned
     */
	D2D1_RECT_F area;

	/**
	 * Self-Reference and Parent
	 */
	TrecPointerSoft<TPage> self, parent;

	/**
	 * How much to rotate by before drawing
	 */
	float rotate;
};

D2D1_RECT_F _VIDEO_GRAPHICS ConvertStringToD2D1Rect(const TString& str);

D2D1_RECT_F _VIDEO_GRAPHICS ConvertRectToD2D1Rect(const RECT& rec);

RECT _VIDEO_GRAPHICS ConvertD2D1RectToRect(const D2D1_RECT_F& rec);

UINT GetScrollbarBoxSize();
