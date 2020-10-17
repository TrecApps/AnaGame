#pragma once
#include "TWindow.h"

#include "WebPage.h"

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
class TWebWindow : public TWindow
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
	 * Parameters: void
	 * Returns: void
	 */
	void AddNewTab(const TString& url);


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


protected:

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
	D2D1_RECT_F mainPage, tabs, webPage;
};

