#pragma once
#include "Page.h"
#include "TIdeWindow.h"

/**
 * Enum Class: page_move_mode
 * Purpose: Informs the IDE Page how it is expected to move
 */
typedef enum class page_move_mode
{
	page_move_mode_normal, // Move not expected
	page_move_mode_drag,   // Page is being dragged around the IDE WIndow
	page_move_mode_top,    // Page is expected to move it's top border
	page_move_mode_bottom, // Page is expected to move it's bottom border
	page_move_mode_left,   // Page is expected to move it's left border
	page_move_mode_right   // Page is expected to move it's right border
}page_move_mode;

/**
 * Class: TabPageContent
 * Purpose: Version of the TabContent class that holds a regular TControl as it's content
 * Note: Messages sent through here will be forwarded to the underlying TCOntrol, and Events that the TControls report will be sent back up the pipeline
 *
 * SuperClass: TabContent - interface with the tab
 */
class _ANAFACE_DLL TabPageContent : public TabContent
{
public:
	/**
	 * Method: TabPageContent::TabPageContent
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new TabContent object
	 */
	TabPageContent();

	/**
	 * Method: TabPageContent::~TabPageContent
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TabPageContent();

	/**
	 * Method: TabPageContent::Resize
	 * Purpose: Calls Resize on the underlying content object
	 * Parameters: const D2D1_RECT_F& loc - the new location the content is expected to occupy
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Resize(const D2D1_RECT_F& loc) override;

	/**
	 * Method: TabPageContent::GetContentType
	 * Purpose: Returns the Content Type, allowing users of this object to cast it into the right sub-type and take it from there
	 * Parameters: void
	 * Returns: TabContentType - the type of content this is expected to yield
	 *
	 * Attributes: override
	 */
	virtual TabContentType GetContentType() override;


	/**
	 * Method: TabPageContent::TookTab
	 * Purpose: Allows Tab Content with Tab Bars to take in a tab
	 * Parameters: TrecPointer<Tab> tab - the tab to take
	 * Returns: bool - whether the tab was taken in
	 */
	virtual bool TookTab(TrecPointer<Tab> tab) override;

	/*
	 * Method: TabPageContent::OnRButtonUp
	 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 *
	 * Attributes: override; message
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	/*
	* Method: TabPageContent::OnLButtonDown
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
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons) override;

	/*
	* Method: TabPageContent::OnRButtonDown
	* Purpose: Allows Control to catch the RightmessageState::mouse button down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*
	* Attributes: override; message
	*/
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls) override;

	/*
	* Method: TabPageContent::OnMouseMove
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
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls) override;



	/**
	 * Method: TabPageContent::OnMouseLeave
	 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	 * Returns: bool - whether the leave occured
	 *
	 * Attributes: override; message
	 */
	afx_msg virtual bool OnMouseLeave(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: TabPageContent::OnLButtonDblClk
	 * Purpose: Allows control to catch the DOuble Click event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 *
	 * Attributes: override; message
	 */
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	/*
	* Method: TabPageContent::OnLButtonUp
	* Purpose: Allows control to catch the Left Button Up event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: override; message
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	/*
	* Method: TabPageContent::OnChar
	* Purpose: Allows Controls to repond to character input
	* Parameters: bool fromChar - can be called either from on Key Down or OnChar
	*				UINT nChar - The ID of the character that was pressed
	*				UINT nRepCnt - how many times the character was processed for this event
	*				UINT nFlags - flags provided by MFC's Message system, not used
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: override; message
	*/
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: TabPageContent::SetPage
	 * Purpose: Sets up the Page to hold
	 * Parameters: TrecPointer<Page> control - the page to hold
	 * Returns: void
	 */
	void SetPage(TrecPointer<Page> control);

	/**
	 * Method: TabPageContent:GetPage
	 * Purpose: Retrieves the underlying Page
	 * Parameters: void
	 * Returns: TrecPointer<Page> - the page this object holds
	 */
	TrecPointer<Page> GetPage();

	/**
	 * Method: TabPageContent::HasContent
	 * Purpose: Reports whether this object has the desired content
	 * Parameters: void
	 * Returns: bool - whether the Tab Content is populated or not
	 *
	 * Attributes: override
	 */
	virtual bool HasContent() override;

	/**
	 * Method: TabPageContent::Draw
	 * Purpose: Applies the Draw Operation to the content
	 * Parameters: TObject* obj - any data associated with the draw
	 * Returns: void
	 * Attributes: Abstract
	 */
	virtual void Draw(TObject* obj) override;



protected:
	/**
	 * The TControl to Hold
	 */
	TrecPointer<Page> page;
};



/**
 * Class: IDEPageHolder
 * Purpose: Holds an IDE Page set for drag
 * 
 * Note: This class could be used as inspiration when writing the Tab Class in Anaface
 */
class IDEPageHolder
{
	friend class IDEPage;
	friend class IDEPageHolder;
public:

	/**
	 * Method: IDEPageHolder::IDEPageHolder
	 * Purpose: Constructor
	 * Parameters: TString name, - name to present of the Tab
	 *				TrecPointer<DrawingBoard> rt - the Drawing Board to draw with
	 *				UINT barSpace - the size of the bar
	 *				TrecPointer<EventHandler> handler - the handler to the new page
	 *				TrecPointer<TWindow> win - the Window to draw against
	 *				D2D1_RECT_F initLoc -  the initial location available (might be smaller than what's provided here)
	 * Returns: New Page Holder Object
	 */
	IDEPageHolder(TString name, TrecPointer<DrawingBoard> rt, UINT barSpace, TrecPointer<EventHandler> handler, TrecPointer<TWindow> win, D2D1_RECT_F initLoc);


	/**
	 * Method: IDEPageHolder::~IDEPageHolder
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~IDEPageHolder();





	/**
	 * Method: IDEPageHolder::GetBasePage
	 * Purpose: Retrieves the Base Reference to the Page it is holding
	 * Parameters: void
	 * Returns: TrecPointer<Page> - the Base Page reference 
	 */
	TrecPointer<Page> GetBasePage();

	/**
	 * Method: IDEPageHolder::GetPage
	 * Purpose: Retrieves the IDE Page Reference this object is holding
	 * Parameters: void
	 * Returns: TrecSubPointer<Page, IDEPage> - the IDE Page reference
	 */
	TrecSubPointer<Page, IDEPage> GetPage();

	/**
	 * Method: IDEPageHolder::GetName
	 * Purpose: Retrievs the Name printed on the Tab
	 * Parameters: void
	 * Returns: TString - the Name held by the holder
	 */
	TString GetName();

	/**
	 * Method: IDEPageHolder::GetLocation
	 * Purpose: Retrieves the Location of the Tab
	 * Parameters: void
	 * Returns: D2D1_RECT_F - the Rectable occupied by the Tab
	 */
	D2D1_RECT_F GetLocation();
	/**
	 * Method: IDEPageHolder::SetLocation
	 * Purpose: Sets the boundaries of the Tab
	 * Parameters: const D2D1_RECT_F& newLoc - reference to the space this tab is being allocated
	 * Returns: D2D1_RECT_F - the Rectable occupied by the Tab
	 */
	D2D1_RECT_F SetLocation(const D2D1_RECT_F& newLoc);
	/**
	 * Method: IDEPageHolder::Draw
	 * Purpose: Draws the Tab
	 * Parameters: void
	 * Returns: void
	 */
	void Draw();
	/**
	 * Method: IDEPageHolder::Move
	 * Purpose: Moves the Tab by the specifed amount
	 * Parameters: TPoint& moveBy - the amount to move by
	 * Returns: void
	 */
	void Move(TPoint& moveBy);

	/**
	 * Method: IDEPageHolder::SetCurrentPoint
	 * Purpose: Sets the Current Point of the Tab
	 * Parameters: TPoint& p - the starting position of the tab
	 * Returns: void
	 */
	void SetCurrentPoint(TPoint& p);

protected:

	/**
	 * Method: IDEPageHolder::SetPage
	 * Purpose:sets the Page for this Tab
	 * Parameters: TrecSubPointer<Page, IDEPage> p - the page to set this to
	 * Returns: void
	 */
	void SetPage(TrecSubPointer<Page, IDEPage> p);

	/**
	 * the page this tab is holding on to
	 */
	TrecSubPointer<Page, IDEPage> page;
	
	/**
	 * The Text to print out
	 */
	TrecPointer<TText> text;

	/**
	 * the location this tab occupies
	 */
	D2D1_RECT_F location;

	/**
	 * Used for moving
	 */
	TPoint curPoint;
};

/**
 * Class: IDEPage
 * Purpose: Extends the page class to allow it to brush up against other pages
 * 
 * SuperClass: Page
 */
class _TAP_DLL IDEPage :
	public Page
{
	friend class TIdeWindow;
	friend class TrecPointerKey;
protected:

	/**
	 * Method: IDEPage::IDEPage
	 * Purpose: Constructor
	 * Parameters: ide_page_type type - the type of Page this will be
	 *				UINT barSpace - how much space to give the Tab Bar
	 *				TrecPointer<DrawingBoard> board - The Drawing Board to draw on
	 * Returns: New IDEPage object
	 */
	IDEPage(ide_page_type type, UINT barSpace, TrecPointer<DrawingBoard> board);



	/**
	 * Method: IDEPage::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType() override;

	/**
	 * Method: IDEPage::~IDEPage
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~IDEPage();

	/**
	 * Method: IDEPage::SetResources
	 * Purpose: Sets up resources for this Page to be a 2D Page
	 * Parameters: TrecPointer<TInstance> in, TrecPointer<TWindow> window
	 * Returns: void
	 */
	void SetResources(TrecPointer<TInstance> in, TrecPointer<TWindow> window);
	/**
	 * Method: IDEPage::SetResources
	 * Purpose: Sets up resources for 3D Support
	 * Parameters: TrecPointer<TInstance> in - instance associate with this Page
	 *				TrecPointer<TWindow> window - the Window this will be added to
	 *				TrecPointer<TWindowEngine> engine - the Engine to provide 3D support
	 * Returns: void
	 */
	void SetResources(TrecPointer<TInstance> in, TrecPointer<TWindow> window, TrecPointer<TWindowEngine> engine);

	/**
	 * Method: IDEPage::MoveBorder
	 * Purpose: Shofts the border when called upon by a neighboring page
	 * Parameters: float& magnitude - the amount to shift the border by
	 *				page_move_mode mode - which border to shift
	 * Returns: void
	 */
	void MoveBorder(float& magnitude, page_move_mode mode);


	/**
	 * Method: IDEPage::SetArea
	 * Purpose: Sets the Region of the Page
	 * Parameters: const D2D1_RECT_F& loc -  the location to provide this Page
	 * Returns: void
	 */
	virtual void SetArea(const D2D1_RECT_F& loc) override;


	/**
	 * Method: IDEPage::TookTab
	 * Purpose: Allows Page with Tab Bars to take in a tab
	 * Parameters: TrecPointer<Tab> tab - the tab to take
	 * Returns: bool - whether the tab was taken in
	 */
	virtual bool TookTab(TrecPointer<Tab> tab)override;

	/**
	 * Method: IDEPage::OnRButtonUp
	 * Purpose: Responds to the Right Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)override;
	/**
	 * Method: IDEPage::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)override;
	/**
	 * Method: IDEPage::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut) override;
	/**
	 * Method: IDEPage::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)override;
	/**
	 * Method: IDEPage::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut) override;
	/**
	 * Method: IDEPage::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly) override;
	/**
	 * Method: IDEPage::OnChar
	 * Purpose: Responds to the Char Message
	 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
	 *				UINT nChar - the Character provided
	 *				UINT nRepCnt - the number of times to add it 
	 *				UINT nFlags - flags associated with the message
	 *				messageOutput* mOut - the result of the event
	 * Returns: bool 
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut) override;
	/**
	 * Method: IDEPage::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg virtual void OnResize(const D2D1_RECT_F& newLoc, UINT nFlags, TrecPointer<TWindowEngine>) override;

public:

	/**
	 * Method: IDEPage::OnLButtonDown
	 * Purpose: Captures the message and sees if it needs to prepare to shift borders before processing the message normally
	 * Parameters: UINT nFlags - flags associated with the click
	 *				TPoint point - Location of the mouse
	 *				messageOutput* mOut - result of the click
	 *				TDataArray<EventID_Cred>& eventAr - list of events to feed the Handler if applicable
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly)override;
	/**
	 * Method: IDEPage::OnMouseMove
	 * Purpose: Captures the message and sees if it needs to shift borders before processing the message normally
	 * Parameters: UINT nFlags - flags associated with the movement
	 *				TPoint point - Location of the mouse
	 *				messageOutput* mOut - result of the movement
	 *				TDataArray<EventID_Cred>& eventAr - list of events to feed the Handler if applicable
	 * Returns: void
	 * 
	 * Attributes: message; override
	 */
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly)override;
	/**
	 * Method: IDEPage::OnLButtonUp
	 * Purpose: Used to confirm whether a proper click was done
	 * Parameters: TPoint& point - location where the mouse is when the user releases
	 * Returns: bool - whether the mouse was in the Page's region
	 * 
	 * Attributes: message
	 */
	afx_msg bool OnLButtonUp(TPoint& point);


	/**
	 * Method: IDEPage::Draw
	 * Purpose: Draws the Tab Bar and then proceeds to draw its contents
	 * Parameters: TrecPointer<TBrush> color - the Brush to draw the TabBar with
	 *				TWindowEngine* twe = nullptr -  the TWindowEngine to feed the COntrols
	 * Returns: void
	 */
	void Draw(TrecPointer<TBrush> color, TWindowEngine* twe = nullptr);
	/**
	 * Method: IDEPage::SetLink
	 * Purpose: Allows the IDE Window to link up it's pages so that they can directly communicate with each other
	 * Parameters: TrecSubPointer<Page, IDEPage> p - The Page to link to
	 *				ide_page_type t - the Specific purpose of the Page
	 * Returns: void 
	 */
	void SetLink(TrecSubPointer<Page, IDEPage> p, ide_page_type t);


	// Methods for the Drag version of the Page

	/**
	 * Method: IDEPage::GetName
	 * Purpose: Retireves the Name associated with this page
	 * Parameters: void
	 * Returns: TSTring - name held by the page
	 */
	TString GetName();

	/**
	 * Method: IDEPage::SetNewParentPage
	 * Purpose: Sets the Parent of this Page (assumes this is a Drag Page)
	 * Parameters: TrecPointer<Page> p -  the Page that holds this page
	 * Returns: void
	 */
	void SetNewParentPage(TrecPointer<Page> p);


	// Methods for Parent Pages

	/**
	 * Method: IDEPage::AddNewPage
	 * Purpose: Add an Existing Page holder to THIS page
	 * Parameters: TrecPointer<Page> pageHolder - The Tab and Page to add to
	 *				 const TString& name - name of the tab
	 * Returns: void
	 */
	void AddNewPage(TrecPointer<Page> pageHolder, const TString& name);
	/**
	 * Method: IDEPage::AddNewPage
	 * Purpose: Submits a new Page to the IDE Page to be tabbed
	 * Parameters: TrecPointer<TInstance> ins - the instance associated with this operation
	 *				TrecPointer<TWindow> win - The Window this Page is in
	 *				TString name - Te Name to present of the Tab
	 *				TrecPointer<EventHandler> h - the EventHandler associated with the Page
	 * Returns: TrecPointer<Page> - the page generated
	 */
	TrecPointer<Page> AddNewPage(TrecPointer<TInstance> ins, TrecPointer<TWindow> win, TString name, TrecPointer<EventHandler> h);
	/**
	 * Method: IDEPage::RemovePage
	 * Purpose: Removes the Page Holder and page from this IDE Page
	 * Parameters: TrecPointer<Page> pageHolder - The Page Holder to Remove
	 * Returns: void
	 */
	void RemovePage(TrecPointer<Tab> pageHolder);

	/**
	 * Method: IDEPage::IsDrawing
	 * Purpose: Reports whether the Page is currently able to draw
	 * Parameters: void
	 * Returns: bool - if the region is not zeroed
	 */
	bool IsDrawing();

protected:

	/**
	 * Link to the Body Page
	 */
	TrecSubPointer<Page, IDEPage> body;

	/**
	 * Link to the Basic Console Page
	 */
	TrecSubPointer<Page, IDEPage> basicConsole;

	/**
	 * Link to the Deep Console Page
	 */
	TrecSubPointer<Page, IDEPage> deepConsole;

	/**
	 * Link to the Upper Right Page
	 */
	TrecSubPointer<Page, IDEPage> upperRight;

	/**
	 * Link to the Lower Right Page
	 */
	TrecSubPointer<Page, IDEPage> lowerRight;

	/**
	 * Link to the Upper Left Page
	 */
	TrecSubPointer<Page, IDEPage> upperLeft;

	/**
	 * Link to the Lower Left Page
	 */
	TrecSubPointer<Page, IDEPage> lowerLeft;

	/**
	 * List of pages held by this page
	 */
	// TDataArray<TrecPointer<IDEPageHolder>> pages;
	TTabBar pages;
	
	/**
	 * Current Page to draw
	 */
	TrecPointer<Page> currentPage;

	/**
	 * Tabs
	 */
	TrecPointer<Tab> focusTab;

	/**
	 * Method: IDEPage::GetFocusPage
	 * Purpose: Retrieves the Page to set the focus to if the User Clicks on the Tab Bar
	 * Parameters:TPoint& point
	 * Returns: TrecPointer<IDEPageHolder> - the Holder referenced by the Tab
	 */
	TrecPointer<Tab> GetFocusPage(TPoint& point);

	/**
	 * Window hoding this page
	 */
	TrecSubPointerSoft<TWindow, TIdeWindow> parentWindow;

	/**
	 * Type of IDE Page this object is
	 */
	ide_page_type type;
	
	/**
	 * amount of Space to offer the Tab Bar
	 */
	UINT barSpace;
	/**
	 * What border is being moved, if any
	 */
	page_move_mode moveMode;

	/**
	 * Whether this page should be drawn 
	 */
	bool draw;

	/**
	 * the current Point being held in a Border Shift operation
	 */
	TPoint curPoint;

	/**
	 * Name of this page
	 */
	TString name;


private:

	/**
	 * Method: IDEPage::MouseMoveBody
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Body Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveBody(TPoint& diff);
	/**
	 * Method: IDEPage::MouseMoveBasicConsole
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Basic-Console Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveBasicConsole(TPoint& diff);
	/**
	 * Method: IDEPage::MouseMoveDeepConsole
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Deep-Console Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveDeepConsole(TPoint& diff);
	/**
	 * Method: IDEPage::MouseMoveUpperRight
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Right Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveUpperRight(TPoint& diff);
	/**
	 * Method: IDEPage::MouseMoveLowerRight
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Right Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveLowerRight(TPoint& diff);
	/**
	 * Method: IDEPage::MouseMoveUpperLeft
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Left Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveUpperLeft(TPoint& diff);
	/**
	 * Method: IDEPage::MouseMoveLowerLeft
	 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Left Page
	 * Parameters: TPoint& diff - the amount of units to move by
	 * Returns: void
	 */
	void MouseMoveLowerLeft(TPoint& diff);
};

