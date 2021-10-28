#pragma once
#include <TPage.h>


/**
 * Class: TabBar
 * Purpose: Provides a means to manage tabs in Anagame
 */
class TabBar :
    public TPage
{
public:

    /**
     * Class: TabBar::TabBarHolder
     * Purpose: Provides the Tab Bar with the means to send the contents of the tab to which-ever control is in charge of hosting it
     */
    class TabBarHolder
    {
	public:
        /**
         * Method: TabBarHolder::SetView
         * Purpose: Called by TabBar when a new Tab is selected
         * Parameters: TrecPointer<TPage> page - the content to display
         *              bool onNew - Relevent if first parameter is null, true if caaused by a click on the new Tab-Tab
         * Returns: void
         * 
         * Attributes: abstract
         */
        virtual void SetView(TrecPointer<TPage> page, bool onNew = false) = 0;

		/**
		 * Method: TabBarHolder::RemoveView
		 * Purpose: Called by TabBar when a tab Requests to be deleted
		 * Parameters: TrecPointer<TPage> page - the content to remove
		 * Returns: void
		 *
		 * Attributes: abstract
		 */
		virtual void RemoveView(TrecPointer<TPage> page) = 0;
    };

	/**
	 * Class: Tab
	 * Purpose: Represents an Actual Tab
	 */
	class Tab : public TPage
	{
		friend class TabBar;
	protected:
		/**
		 * Bar currently holding Tab
		 */
		TrecSubPointerSoft<TPage, TabBar> tabBar;

		/**
		 * The Text To Present
		 */
		TrecPointer<TTextElement> text;

		/**
		 * Images to present (color will present a color under any text or other image, image will move text off to side, exit will present an 'X' option to close the tab
		 */
		TrecPointer<TBrush> color, image, exit;

		/**
		 * the sub Rectangles
		 */
		D2D1_RECT_F iArea, tArea, xArea;

		/**
		 * The Content to hold
		 */
		TrecPointer<TPage> content;
	public:

		/**
		 * Method: Tab::SetBrush
		 * Purpose: allows users to set either the color, or the icon associated with the tab
		 * Parameters: TrecPointer<TBrush> brush - the brush to use
		 *				bool doIcon - true then brush sets the icon brush, false then brush sets the 'color' brush
		 * Returns: void
		 */
		void SetBrush(TrecPointer<TBrush> brush, bool doIcon);

		/**
		 * Method: Tabr::Tabr
		 * Purpose: Constructor
		 * Parameters: TrecPointer<DrawingBoard> board - the drawing board to draw to
		 * Returns: new Tab Bar
		 */
		Tab(TrecPointer<DrawingBoard> board);

		/**
		 * Method: TPage::HandlesEvents
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
		 * Method: TPage::Draw
		 * Purpose: Draws the Page to the Window
		 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
		 * Returns: void
		 *
		 * Attributes: override
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
		 * Attributes: message; override
		 */
		ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


		/**
		 * Method: TPage::OnRButtonDown
		 * Purpose: Responds to the Right Button Down Message
		 * Parameters: UINT nFlags - flags associated with the message
		 *				const TPoint& point - the point included in the message
		 *				message_output& mOut -  the result of the message
		 *				TDataArray<EventID_Cred>& - list of events to be handled
		 * Returns: void
		 *
		 * Attributes: message; override
		 */
		ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


		/**
		 * Method: TPage::OnLButtonUp
		 * Purpose: Responds to the Left Button Up Message
		 * Parameters: UINT nFlags - flags associated with the message
		 *				const TPoint& point - the point included in the message
		 *				message_output& mOut -  the result of the message
		 *				TDataArray<EventID_Cred>& - list of events to be handled
		 * Returns: void
		 *
		 * Attributes: message; override
		 */
		ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


		/**
		 * Method: TPage::OnLButtonDown
		 * Purpose: Responds to the Left Button Down Message
		 * Parameters: UINT nFlags - flags associated with the message
		 *				const TPoint& point - the point included in the message
		 *				message_output& mOut -  the result of the message
		 *				TDataArray<EventID_Cred>& - list of events to be handled
		 * Returns: void
		 *
		 * Attributes: message; override
		 */
		ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

		/**
		 * Method: TPage::OnMouseMove
		 * Purpose: Responds to the Mouse Move Message
		 * Parameters: UINT nFlags - flags associated with the message
		 *				TPoint point - the point included in the message
		 *				message_output& mOut -  the result of the message
		 * Returns: void
		 *
		 * Attributes: message; override
		 */
		ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

		/**
		 * Method: TPage::OnLButtonDblClk
		 * Purpose: Responds to the Left Button Double CLick Message
		 * Parameters: UINT nFlags - flags associated with the message
		 *				TPoint point - the point included in the message
		 *				message_output& mOut -  the result of the message
		 * Returns: void
		 *
		 * Attributes: message; override
		 */
		ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&) override;

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
		 * Attributes: message; override
		 */
		ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&) override;

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
		 * Method: TPage::OnScroll
		 * Purpose: Sends Scroll Command to controls
		 * Parameters: const TPoint& point - point of the mouse
		 *				const TPoint& direction - how far to send the scroll
		 * Returns: bool - whether message was recieved
		 *
		 * Attributes: message; override
		 */
		ag_msg virtual bool OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&) override;
	};

protected:


	typedef enum class tab_mode
	{
		tm_not_set,
		tm_regular,
		tm_left,
		tm_right,
		tm_exit
	};

	UINT unknownCount;

	tab_mode tabMode;

	/**
	 * The currently selected tab having been clicked
	 */
	TrecPointer<TPage> currentTab;


    TrecPointer<TabBarHolder> holder;
	/**
	 * The first tab shown in the bar
	 */
	UINT startTab;
	/**
	 * List of tabs
	 */
	TDataArray<TrecPointer<TPage>> tabs;

	/**
	 * Special Tabs used for scrolling
	 */
	Tab leftTab, rightTab;

	/**
	 * Method: TTabBar::SetTabSizes
	 * Purpose: Establishes the sizes of the constituent tabs
	 * Parameters: void
	 * Returns: void
	 */
	void SetTabSizes();

	/**
	 * Whether Tabs can be dragged around to other tab bars
	 */
	bool draggableTabs;

	/**
	 * Whether the tabs are overfowing the bar
	 */
	bool tabOverflow;


	void InjectTabAt(TrecPointer<TPage> page, UINT index);


public:

	/**
	 * Method: TabBar::InjectTabAt
	 * Purpose: Adds a Tab via point
	 * Parameters: const TPoint& point - the point to add
	 *			TrecPointer<TPage> page - the tab to add (will assert that object is a tab)
	 * Returns: bool - whether the tab was added or not
	 */
	bool InjectTabAt(const TPoint& point, TrecPointer<TPage> page);


	/**
	 * Method: TabBar::RemoveTab
	 * Purpose: Adds a Tab via point
	 * Parameters: TrecPointer<TPage> page - the tab to add (will assert that object is a tab)
	 * Returns: bool - whether the tab was removed or not
	 */
	bool RemoveTab(TrecPointer<TPage> page);


	/**
	 * Method: TabBar::InjectTabAt
	 * Purpose: Adds a Tab via point
	 * Parameters: const TString& name - the name to call it (ignored if page param is a tab)
	 *			TrecPointer<TPage> page - the tab to add (will assert that object is not null)
	 *			bool exit - whether the exit should be enabled or not (ignored on actul tabs)
	 * Returns: TrecPointer<TPage> - the tab that was added
	 */
	TrecPointer<TPage> AddNewTab(const TString& name, TrecPointer<TPage> page, bool exit);


	/**
	 * Method: TabBar::TabBar
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> board - the drawing board to draw to
	 * Returns: new Tab Bar
	 */
	TabBar(TrecPointer<DrawingBoard> board);

    /**
     * Method: TPage::HandlesEvents
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
	 * Method: TPage::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
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
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


	/**
	 * Method: TPage::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


	/**
	 * Method: TPage::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


	/**
	 * Method: TPage::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

	/**
	 * Method: TPage::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

	/**
	 * Method: TPage::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&) override;

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
	 * Attributes: message; override
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&) override;

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
	 * Method: TPage::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&) override;
};

