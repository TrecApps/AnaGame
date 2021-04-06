#pragma once
#include "TWindow.h"

#include "WebPage.h"

/**
 * Class TabContent
 * Purpose: Abstract Class meant to hold the contents of a given tab
 *
 * Attributes: Abstract as the tab content could be a TControl, Page, Web Node, or something else
 */
class _TAP_DLL TabWebPageContent: public TabContent
{
public:
	/**
	 * Method: TabWebPageContent::TabWebPageContent
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new TabContent object
	 */
	TabWebPageContent();

	/**
	 * Method: TabWebPageContent::~TabWebPageContent
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TabWebPageContent();

	/**
	 * Method: TabWebPageContent::Resize
	 * Purpose: Calls Resize on the underlying content object
	 * Parameters: const D2D1_RECT_F& loc - the new location the content is expected to occupy
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Resize(const D2D1_RECT_F& loc)override;

	/**
	 * Method: TabWebPageContent::Draw
	 * Purpose: Applies the Draw Operation to the content
	 * Parameters: TObject* obj - any data associated with the draw
	 * Returns: void
	 * Attributes: Abstract
	 */
	virtual void Draw(TObject* obj)override;

	/**
	 * Method: TabWebPageContent::GetContentType
	 * Purpose: Returns the Content Type, allowing users of this object to cast it into the right sub-type and take it from there
	 * Parameters: void
	 * Returns: TabContentType - the type of content this is expected to yield
	 *
	 * Attributes: Abstract
	 */
	virtual TabContentType GetContentType()override;

	/**
	 * Method: TabWebPageContent::HasContent
	 * Purpose: Reports whether this object has the desired content
	 * Parameters: void
	 * Returns: bool - whether the Tab Content is populated or not
	 *
	 * Attributes: Abstract
	 */
	virtual bool HasContent()override;


	/**
	 * Method: TabControlContent::TookTab
	 * Purpose: Allows Tab Content with Tab Bars to take in a tab
	 * Parameters: TrecPointer<Tab> tab - the tab to take
	 * Returns: bool - whether the tab was taken in
	 */
	virtual bool TookTab(TrecPointer<Tab> tab)override;

	/*
	 * Method: TabWebPageContent::OnRButtonUp
	 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 *
	 * Attributes: virtual; message
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	/*
	* Method: TabWebPageContent::OnLButtonDown
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
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)override;

	/*
	* Method: TabWebPageContent::OnRButtonDown
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
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls)override;

	/*
	* Method: TabWebPageContent::OnMouseMove
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
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)override;



	/**
	 * Method: TabWebPageContent::OnMouseLeave
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
	afx_msg virtual bool OnMouseLeave(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	/*
	* Method: TabWebPageContent::OnLButtonDblClk
	* Purpose: Allows control to catch the DOuble Click event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	/*
	* Method: TabWebPageContent::OnLButtonUp
	* Purpose: Allows control to catch the Left Button Up event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*
	* Attributes: virtual; message
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	/*
	* Method: TabWebPageContent::OnChar
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
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	/**
	 * Method: TabWebPageContent::GetWebPage
	 * Purpose: Retrieves the WebPage held by the content
	 * Parameters: void
	 * Returns: TrecSubPointer<Page, WebPage> - the web page held by the content object
	 */
	TrecSubPointer<Page, WebPage> GetWebPage();

	/**
	 * Method: TabWebPageContent::SetWebPage
	 * Purpose: Retrieves the WebPage held by the content
	 * Parameters: TrecSubPointer<Page, WebPage> webPage - the web page to be held by the content object
	 * Returns: void
	 */
	 void SetWebPage(TrecSubPointer<Page, WebPage> webPage);

protected:
	TrecSubPointer<Page, WebPage> webPage;
};


/**
 * Class: EnvironmentGenerator
 * Purpose: Supplies the TWebWindow with an acceptable WebEnvironment
 */
class EnvironmentGenerator
{
public:
	virtual TrecPointer<TEnvironment> GetEnvironment(TrecPointer<TFileShell> shell) = 0;
};

/**
 * Class: TWebWindow
 * Purpose: Specialized Window designed to hold Web-Content
 * 
 * SuperClass: TWindow
 */
class _TAP_DLL TWebWindow : public TWindow
{
public:

	/**
	 * Method: TWebWindow::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TWebWindow::TWebWindow
	 * Purpose: Constructor
	 * Parameters: TString& name - the name of the WIndow
	 *				TString& winClass - the name to tell Windows to use for this Window type
	 *				UINT style - the Style of window to use
	 *				HWND parent - the Handle to the Parent Window (if any)
	 *				int commandShow - How to show the Window (provided in WinMain)
	 *				TrecPointer ins - pointer to the TInstance involved (hence why TInstance has a SetSelf method)
	 *				UINT mainViewSpace - how much drawing space to devote to the main Page
	 *				UINT pageBarSpace - how much tab space to allow the tab bars in the IDE-Pages
	 * Returns: New IDE Window
	 */
	TWebWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, UINT mainViewSpace, UINT pageBarSpace);

	/**
	 * Method: TWebWindow::~TWebWindow
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TWebWindow();

	/**
	 * Method: TWebWindow::PrepareWindow
	 * Purpose: Sets the Window up for viewing
	 * Parameters: void
	 * Returns: int - error code (0 = success)
	 *
	 * Attributes: override
	 */
	virtual int PrepareWindow()override;

	/**
	 * Method: TWebWindow::AddNewTab
	 * Purpose: Adds a New Tab to the Window
	 * Parameters: void
	 * Returns: void
	 */
	void AddNewTab();

	/**
	 * Method: TWebWindow::AddNewTab
	 * Purpose: Adds a New Tab to the Window
	 * Parameters: const TString& url - the URL of the resources to present
	 *			bool createTab - whether to create a new tab to hold the results (true is default)
	 * Returns: void
	 * 
	 * Note: A New Tab will be created if 'createTab' is true or if there is no current Tab to do it with
	 */
	void AddNewTab(const TString& url, bool createTab = true);


	/**
	 * Method: TWebWindow::CompileView
	 * Purpose: Compiles the Main View while also preparing all of the individual IDE pages
	 * Parameters: TString& file - path of the TML file holding the Anaface
	 *				TrecPointer<EventHandler> eh - the Handler to the Main page
	 * Returns: int - error (0 == success)
	 *
	 * Attributes: override
	 */
	int CompileView(TString& file, TrecPointer<EventHandler> eh)override;

	/**
	 * Method: TWebWindow::SetEnvironmentGenerator
	 * Purpose: Allows the Web Window to generate eb environments
	 * Parameters: TrecPointer<EnvironmentGenerator> - the object to generate Environments with 
	 * Returns: void
	 */
	void SetEnvironmentGenerator(TrecPointer<EnvironmentGenerator> gen);

	/**
	 * Method: TWebWindow::DrawOtherPages
	 * Purpose: Draws the tab page and the Current Web Page
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void DrawOtherPages();

	/**
	 * Method: TWebWindow::OnLButtonDown
	 * Purpose: Manages the Left Button Down Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point);


	/**
	 * Method: TWindow::OnMouseMove
	 * Purpose: Manages the Mouse Move Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 *
	 * Attributes: message
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnLButtonUp
	 * Purpose: Manages the Left Button Up Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 *
	 * Attributes: message; virtual
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point);

protected:

	TrecSubPointer<TControl, TTabBar> webPages;

	TString FixUrl(const TString& url);

	TrecSubPointer<Page, WebPage> GetWebPage(const TString& url);

	TrecPointer<EnvironmentGenerator> envGenerator;

	/**
	 * Space to offer the Tab Bars
	 */
	UINT pageBarSpace;

	/**
	 * the Amount of space to offer the main Page
	 */
	UINT mainViewSpace;

	/**
	 * Tabs which lead to the Web Pages themselves
	 */
	TDataArray<TrecPointer<WebPageHolder>> tabList;

	/**
	 * Locations to keep track of
	 */
	D2D1_RECT_F mainPageSpace, tabs, webPage;

};

