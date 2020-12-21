#pragma once
#include "TControl.h"
#include <Dragable.h>

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

/**
 * Class TabContent
 * Purpose: Abstract Class meant to hold the contents of a given tab
 * 
 * Attributes: Abstract as the tab content could be a TControl, Page, Web Node, or something else
 */
class TabContent
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
	 * Method: TabContent::GetContentType
	 * Purpose: Returns the Content Type, allowing users of this object to cast it into the right sub-type and take it from there
	 * Parameters: void
	 * Returns: TabContentType - the type of content this is expected to yield
	 * 
	 * Attributes: Abstract
	 */
	virtual TabContentType GetContentType() = 0;
};

/**
 * Class Tab
 * Purpose: Renders a basic Tab in a Tab Bar, responding to clicks and holding the contents of the tab for presentation
 *	by what ever object is holding the Tab-Bar
 */
class Tab 
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

private:
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
class TTabBar :
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
	 * Method: TControl::onCreate
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
	 * Method: TControl::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: RECT r - the new location for the control
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Resize(D2D1_RECT_F&)override;

	/**
	 * Method: TControl::onDraw
	 * Purpose: Draws the control
	 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void onDraw(TObject* obj = nullptr) override;

	/*
	* Method: TControl::OnMouseMove
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
	* Method: TControl::OnLButtonDown
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
	* Method: TControl::OnLButtonUp
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
	void AddTab(const TString& text);

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
};

