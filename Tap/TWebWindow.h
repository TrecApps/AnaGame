#pragma once
#include "TWindow.h"

#include "WebPage.h"
#include <TabBar.h>


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
	TWebWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins, UINT mainViewSpace, UINT pageBarSpace);

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
	 * Method: TWebWindow::OnWindowResize
	 * Purpose: Resizes the Window
	 * Parameters: UINT width - the new width of the window
	 *				UINT height - the new height of the Window
	 * Returns: void
	 *
	 * Attributes: message, override
	 */
	ag_msg virtual void OnWindowResize(UINT width, UINT height)override;

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
	 * Method: TWebWindow::SetTabBar
	 * Purpose: Allows main handler to set it's own tab bar
	 * Parameters: TrecSubPointer<TPage, TabBar> tempTabs - the tabs to use
	 * Returns: void
	 */
	void SetTabBar(TrecSubPointer<TPage, TabBar> tempTabs);


	/**
	 * Method: TWebWindow::CompileView
	 * Purpose: Compiles the Main View while also preparing all of the individual IDE pages
	 * Parameters: TString& file - path of the TML file holding the Anaface
	 *				TrecPointer<EventHandler> eh - the Handler to the Main page
	 * Returns: int - error (0 == success)
	 *
	 * Attributes: override
	 */
	int CompileView(TString& file, TrecPointer<TPage::EventHandler> eh)override;

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
	ag_msg virtual void OnLButtonDown(UINT nFlags, TPoint point);


	/**
	 * Method: TWindow::OnMouseMove
	 * Purpose: Manages the Mouse Move Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 *
	 * Attributes: message
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnLButtonUp
	 * Purpose: Manages the Left Button Up Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 *
	 * Attributes: message; virtual
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, TPoint point);


	/**
	 * Method: TWebWindow::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: virtual
	 */
	ag_msg virtual bool OnScroll(const TPoint& point, const TPoint& direction)override;

protected:

	TrecSubPointer<TPage, TabBar> webPages;

	TrecSubPointer<TPage, WebPage> currentPage;

	TString FixUrl(const TString& url);

	TrecSubPointer<TPage, WebPage> GetWebPage(const TString& url);

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
	 * Locations to keep track of
	 */
	D2D1_RECT_F mainPageSpace, tabs, webPage;

};

