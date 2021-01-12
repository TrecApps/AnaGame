#pragma once
#include "TControl.h"
#include <Dragable.h>
#include "Anaface.h"

class TTabBar;

/**
 * Enum Class: TabContentType
 * Purpose: Tracks the Content type held within a tab
 */
typedef enum class TabContentType
{
	tct_control,  // indicates a regular TControl is the type
	tct_ide_page, // indicates that content is an IDE Page (located in the Tap Library)
	tct_web_page, // indicates that content is a web page (also in the Tab library)
	tct_web_node, // provided in case HTML development in AnaWeb library calls for html based tabs
	tct_reg_page, // provided in case a regular page is the content
	tct_third_p1, // Used by third party projects that introduce one new content type
	tct_third_p2, // Used by third party projects that introduce a second new content type
	tct_third_p3  // Used by third party projects that introduce a third new content type
}TabContentType;

typedef enum class TabClickMode
{
	tcm_not_clicked,   // Tab was not clicked
	tcm_exit,          // Tab's exit section was clicked
	tcm_regular_click, // Tab was clicked
	tcm_new_tab,       // the New Tab was clicked
	tcm_right_tab,     // Right Tab was Clicked
	tcm_left_tab       // Left Tab was Clicked
}TabClickMode;


/**
 * Class TabContent
 * Purpose: Abstract Class meant to hold the contents of a given tab
 * 
 * Attributes: Abstract as the tab content could be a TControl, Page, Web Node, or something else
 */
class _ANAFACE_DLL TabContent
{
public:
	/**
	 * Method: TabContent::TabContent
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new TabContent object
	 */
	TabContent();

	/**
	 * Method: TabContent::~TabContent
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TabContent();

	/**
	 * Method: TabContent::Resize
	 * Purpose: Calls Resize on the underlying content object
	 * Parameters: const D2D1_RECT_F& loc - the new location the content is expected to occupy
	 * Returns: void
	 * 
	 * Attributes: abstract 
	 */
	virtual void Resize(const D2D1_RECT_F& loc) = 0;

	/**
	 * Method: TabContent::Draw
	 * Purpose: Applies the Draw Operation to the content
	 * Parameters: TObject* obj - any data associated with the draw
	 * Returns: void
	 * Attributes: Abstract
	 */
	virtual void Draw(TObject* obj) = 0;

	/**
	 * Method: TabContent::GetContentType
	 * Purpose: Returns the Content Type, allowing users of this object to cast it into the right sub-type and take it from there
	 * Parameters: void
	 * Returns: TabContentType - the type of content this is expected to yield
	 * 
	 * Attributes: Abstract
	 */
	virtual TabContentType GetContentType() = 0;

	/**
	 * Method: TabContent::HasContent
	 * Purpose: Reports whether this object has the desired content
	 * Parameters: void
	 * Returns: bool - whether the Tab Content is populated or not
	 * 
	 * Attributes: Abstract
	 */
	virtual bool HasContent() = 0;

	/*
	 * Method: TabContent::OnRButtonUp
	 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 *
	 * Attributes: virtual; message
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)=0;

	/*
	* Method: TabContent::OnLButtonDown
	* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)=0;

	/*
	* Method: TabContent::OnRButtonDown
	* Purpose: Allows Control to catch the RightmessageState::mouse button down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls)=0;

	/*
	* Method: TabContent::OnMouseMove
	* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)=0;



	/**
	 * Method: TabContent::OnMouseLeave
	 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	 * Returns: bool - whether the leave occured
	*
	* Attributes: virtual; message
	 */
	afx_msg virtual bool OnMouseLeave(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)=0;

	/*
	* Method: TabContent::OnLButtonDblClk
	* Purpose: Allows control to catch the DOuble Click event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)=0;

	/*
	* Method: TabContent::OnLButtonUp
	* Purpose: Allows control to catch the Left Button Up event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)=0;

	/*
	* Method: TabContent::OnChar
	* Purpose: Allows Controls to repond to character input
	* Parameters: bool fromChar - can be called either from on Key Down or OnChar
	*				UINT nChar - The ID of the character that was pressed
	*				UINT nRepCnt - how many times the character was processed for this event
	*				UINT nFlags - flags provided by MFC's Message system, not used
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)=0;
};

/**
 * Class Tab
 * Purpose: Renders a basic Tab in a Tab Bar, responding to clicks and holding the contents of the tab for presentation
 *	by what ever object is holding the Tab-Bar
 */
class _ANAFACE_DLL Tab
{
	friend class TTabBar;
public:
	/**
	 * Method: Tab::Tab
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New Tab Object
	 */
	Tab();



	/**
	 * Method: Tab::SetBrush
	 * Purpose: Sets the Brush of this tab
	 * Parameters: TrecPointer<TBrush> brush - the brush to draw with 
	 * Returns: void
	 */
	void SetBrush(TrecPointer<TBrush> brush);
	/**
	 * Method: Tab::SetText
	 * Purpose: Sets the Text to display
	 * Parameters: const TString& text - the text to display
	 * Returns: void
	 */
	void SetText(const TString& text);
	/**
	 * Method: Tab::SetLocation
	 * Purpose: Established the boundaries of the tab button
	 * Parameters: const D2D1_RECT_F& location - 
	 * Returns:
	 */
	D2D1_RECT_F SetLocation(const D2D1_RECT_F& location);

	/**
	 * Method: Tab::Draw
	 * Purpose: Proceeds to Draw
	 * Parameters: void
	 * Returns: void
	 */
	void Draw();

	/**
	 * Method: Tab::MovePoint
	 * Purpose: Keeps track of where the tab is being moved, such that the framework will know when it is being moved
	 * Parameters: float x - the x coordinate of the mouse cursor
	 *				float y - the y coordinate of the mouse cursor
	 * Returns: void
	 */
	virtual void MovePoint(float x, float y);

	/**
	 * Method: Tab::GetContent
	 * Purpose: Retireves the Content relevant to this tab
	 * Parameters: void
	 * Returns: TrecPointer<TabContent> - the content this tab holds
	 */
	TrecPointer<TabContent> GetContent();

	/**
	 * Method: Tab::SetContent
	 * Purpose: Sets the Content of the Tab
	 * Parameters: TrecPointer<TabContent> cont - the content to store
	 * Returns: void
	 */
	void SetContent(TrecPointer<TabContent> cont);

	/**
	 * Method: Tab::AttemptClick
	 * Purpose: Allows the tab to report the click status
	 * Parameters: const TPoint& point - the click point to test
	 * Returns: TabClickMode - the results of the attempted Click
	 */
	TabClickMode AttemptClick(const TPoint& point);

	/**
	 * Method: Tab::GetText
	 * Purpose: Retireves the Text of the Tab
	 * Parameters: void
	 * Returns: TString - the text of the tab
	 */
	TString GetText();

	/**
	 * Method: Tab::MovedOutside
	 * Purpose: Reports whether the tab would have been moved outside of the original location
	 * Parameters: void
	 * Returns: bool - whether the Tab would have been moved outside the original location
	 */
	bool MovedOutside();

	/**
	 * Method: Tab::ResetTabMovementVars
	 * Purpose: Resets the Resources used to track Tab movement
	 * Parameters: bool resetLoc - whether to reset the location variable (if it moved)
	 * Returns: void
	 */
	void ResetTabMovementVars(bool resetLoc);

	/**
	 * Method: Tab::GetCurrentPoint
	 * Purpose: Retrieves the current point held by the Tab
	 * Parameters: void
	 * Returns: TPoint - the current point
	 */
	TPoint GetCurrentPoint();

private:

	TPoint initial, current;
	bool movedOutside;

	/**
	 * Holds a reference to the actual content to present
	 */
	TrecPointer<TabContent> fContent;

	/**
	 * The Text Object to Render
	 */
	TrecPointer<TText> text;
	/**
	 * The Contents to render
	 */
	TrecPointer<TContent> content1, content2;
	/**
	 * Any extra brush to work with
	 */
	TrecPointer<TBrush> brush;
	/**
	 * The location that the tab is expected to draw in
	 */
	D2D1_RECT_F location;
	/**
	 * The Tab Bar holding the tab
	 */
	TrecSubPointer<TControl, TTabBar> currentTabBar;
	/**
	 * Whether to Draw the first content or the second
	 */
	bool draw1;
	/**
	 * Whether this is the "Add" tab
	 */
	bool isAdd;
};


/**
 * Class: TTabBar
 * Purpose: Holds a series of tabs and manages them
 * 
 * SuperClass: TControl
 */
class _ANAFACE_DLL TTabBar :
	public TControl
{
public:
	/**
	 * Method: TTabBar::TTabBar
	 * Purpose:  Constructors
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Returns: New TabBar Object
	 */
	TTabBar(TrecPointer<DrawingBoard> db,  TrecPointer<TArray<styleTable>> styles);
	/**
	 * Method: TTabBar::~TTabBar
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TTabBar();

	/**
	 * Method: TTabBar::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: RECT contain - the area it can use
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: override
	 */
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	/*
	 * Method: TTabBar::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: RECT r - the new location for the control
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Resize(D2D1_RECT_F&)override;

	/**
	 * Method: TTabBar::onDraw
	 * Purpose: Draws the control
	 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void onDraw(TObject* obj = nullptr) override;

	/*
	* Method: TTabBar::OnMouseMove
	* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*
	* Attributes: override; message
	*/
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)override;
	/*
	* Method: TTabBar::OnLButtonDown
	* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*
	* Attributes: override; message
	*/
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)override;
	/*
	* Method: TTabBar::OnLButtonUp
	* Purpose: Allows control to catch the Left Button Up event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: override; message
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;


	/**
	 * Method: TTabBar::AddTab
	 * Purpose: Adds a new Tab with the given text
	 * Parameters: const TString& text - The text to add to the tab
	 * Returns: void
	 */
	TrecPointer<Tab> AddTab(const TString& text);

	/**
	 * Method: TTabBar::AddTab
	 * Purpose: Adds an Existig tab to this tab bar
	 * Parameters: TrecPointer<Tab> tab - the tab to Add
	 * Returns: bool - whether it worked
	 * 
	 * Note: The tab to be added must already exist (so not null), and it's point must be in the region of the Tab bar
	 */
	bool AddTab(TrecPointer<Tab> tab);

	/**
	 * Method: TTabBar::GetContentSize
	 * Purpose: Reports the number of tabs (presumably) with content
	 * Parameters: void
	 * Returns: UINT - number of content tabs available
	 */
	UINT GetContentSize();

	/**
	 * Method: TTabBar::GetTabAt
	 * Purpose: Retrieves the Tab at the given index or null if out of bounds
	 * Parameters: UINT index - the index to get
	 * Returns: TrecPointer<Tab> - the requested tab
	 */
	TrecPointer<Tab> GetTabAt(UINT index);

	/**
	 * Method: TTabBar::RemoveTabAt
	 * Purpose: Removes a tab
	 * Parameters: UINT Index - index of the tab to remove
	 * Returns: void
	 */
	void RemoveTabAt(UINT index);

	/**
	 * Method: TTabBar::GetCurrentTab
	 * Purpose: Retrieves the Tab currently considered "Selected"
	 * Parameters: void
	 * Returns: TrecPointer<Tab> - te current tab selected
	 */
	TrecPointer<Tab> GetCurrentTab();

protected:

	/**
	 * Method: TTabBar::SetTabSizes
	 * Purpose: Establishes the sizes of the constituent tabs
	 * Parameters: void
	 * Returns: void
	 */
	void SetTabSizes();
	/**
	 * The first tab shown in the bar
	 */
	UINT startTab;
	/**
	 * List of tabs
	 */
	TDataArray<TrecPointer<Tab>> tabs;

	/**
	 * Special Tabs used for scrolling
	 */
	Tab leftTab, rightTab;
	/**
	 * sets the width of the left Tabs
	 */
	UINT selectionSize;
	/**
	 * Whether to have an Add tab
	 */
	bool haveAdd;

	/**
	 * Whether a tab is currently being clicked
	 */
	bool onClick;

	/**
	 * The Tab currently being Clicked
	 */
	TrecPointer<Tab> currentlyClickedTab;

	/**
	 * Keeps track of the Tab Click data
	 */
	TabClickMode clickMode;
};

