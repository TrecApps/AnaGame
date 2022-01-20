#pragma once
#include "TabPage.h"
#include "TEnvironment.h"


/**
 * Enum Class: anagame_page
 * Purpose: Specifies the Type of built-in page to produce
 */
typedef enum class anagame_page
{
	anagame_page_custom,          // a Custom page, the TML file is expected to be provided
	anagame_page_file_node,       // Presents a Page that holds a File node control
	anagame_page_command_prompt,  // Presents a page that holds a Command Prompt
	anagame_page_console,         // Presents a Page that holds a Console that Programs can write to
	anagame_page_code_explorer,   // Presents a page that holds a node control that focuses on code elements
	anagame_page_object_explorer, // Presents a Page intended to present the properties of a given object
	anagame_page_code_file,       // Presents a Page that holds a Text Edit control intended to hold a code file
	anagame_page_project_explorer,// Presents a Page that holds a node control that Explores the Project
	anagame_page_arena,           // Presents a Page that holds a 3D arena control
	anagame_page_camera           // Presents a Page that provides a Camera Panel
};

class MiniApp;

/**
 * Class: TIdeWindow
 * Purpose: Extends the WIndow class and partitions it's drawing space
 * 
 * SuperClass: TWindow
 */
class _TAP_DLL TIdeWindow :
	public TWindow
{
public:

	/**
	 * Method: TIdeWindow::TIdeWindow
	 * Purpose: Constructor
	 * Parameters: TString& name - the name of the WIndow
     *				TString& winClass - the name to tell Windows to use for this Window type
     *				UINT style - the Style of window to use
     *				HWND parent - the Handle to the Parent Window (if any)
     *				int commandShow - How to show the Window (provided in WinMain)
	 *				TrecPointer ins - pointer to the TInstance involved (hence why TInstance has a SetSelf method)
	 *				UINT mainViewSpace - how much drawing space to devote to the main Page
	 *				UINT pageBarSpace - how much tab space to allow the tab bars in the IDE-Pages
	 *				const TString& pageName - name of the main Page to enter in the top Tab Bar (if empty, main page will be a regular Anaface Page)
	 * Returns: New IDE Window
	 */
	TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins, UINT mainViewSpace, UINT pageBarSpace, const TString& pageName);

	/**
	 * Method: TIdeWindow::~TIdeWindow
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TIdeWindow();


	/**
	 * Method: TIdeWindow::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;


	/**
	 * Method: TIdeWindow::PrepareWindow
	 * Purpose: Prepares the Window while also managing the links between the IDE-Pages
	 * Parameters: void
	 * Returns: int - error code (0 = success)
	 * 
	 * Attributes: override
	 */
	virtual int PrepareWindow()override;


	/**
	 * Method: TIdeWindow::OnWindowResize
	 * Purpose: Resizes the Window
	 * Parameters: UINT width - the new width of the window
	 *				UINT height - the new height of the Window
	 * Returns: void
	 *
	 * Attributes: message, override
	 */
	ag_msg virtual void OnWindowResize(UINT width, UINT height)override;

	/**
	 * Method: TIdeWindow::OnLButtonUp
	 * Purpose: Manages the Left Button Up Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 * 
	 * Attributes: override; message
	 */
	void OnLButtonUp(UINT nFlags, TPoint point);

	/**
	 * Method: TIdeWindow::OnMouseMove
	 * Purpose: Manages the Mouse Move Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that the mouse is now
	 * Returns: void
	 * 
	 * Attributes: override; message
	 */
	void OnMouseMove(UINT nFlags, TPoint point)override;

	/**
	 * Method: TIdeWindow::OnLButtonDown
	 * Purpose: Manages the Left Button Down Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 * 
	 * Attributes: override; message
	 */
	void OnLButtonDown(UINT nFlags, TPoint point)override;


	/**
	 * Method: TIdeWindow::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: virtual
	 */
	ag_msg virtual bool OnScroll(const TPoint& point, const TPoint& direction)override;


	/**
	 * Method: TIdeWindow::AddNewMiniApp
	 * Purpose: Adds a MiniApp to the list of Apps being managed
	 * Parameters: TrecPointer<MiniApp> app - the App to add
	 * Returns: void
	 */
	void AddNewMiniApp(TrecPointer<MiniApp> app);


	/**
	 * Method: TIdeWindow::AddNewPage
	 * Purpose: Creates a new Page and returns it to the caller
	 * Parameters: anagame_page pageType - type of built-in page
	 *				ide_page_type pageLoc - the location to place the new Page
	 *				TString name - name of the page to write on the Tab
	 *				TString tmlLoc - the File location of the TML file (if the page type is built-in, this can be empty)
	 *				TrecPointer<EventHandler> handler - the Handler to provide the Page
	 *				bool pageTypeStrict - whether the caller is strict when it comes to the location of the Page
	 * Returns: TrecSubPointer<TPage, TabPage> -  the Page generated
	 */
	TrecPointer<TPage> AddNewPage(anagame_page pageType, ide_page_type pageLoc, TString name, TString tmlLoc, TrecPointer<TPage::EventHandler> handler, bool pageTypeStrict = false);

	/**
	 * Method: TIdeWindow::AddPage
	 * Purpose: Creates a built-in Page
	 * Parameters: anagame_page pageType - type of built-in page
	 *				ide_page_type pageLoc - the location to place the new Page
	 *				TString name - name of the page to write on the Tab
	 * Returns: TrecSubPointer<TPage, TabPage> -  the Page generated
	 */
	TrecPointer<TPage> AddPage(anagame_page pageType, ide_page_type pageLoc, TString name);


	/**
	 * Method: TIdeWindow::CompileView
	 * Purpose: Compiles the Main View while also preparing all of the individual IDE pages
	 * Parameters: TString& file - path of the TML file holding the Anaface
	 *				TrecPointer<EventHandler> eh - the Handler to the Main page
	 * Returns: int - error (0 == success)
	 * 
	 * Attributes: override
	 */
	int CompileView(TString& file, TrecPointer<TPage::EventHandler> eh)override;


	/**
	 * Method: TIdeWindow::SetCurrentHolder
	 * Purpose: Marks a Page Holder as being dragged by the User
	 * Parameters: TrecPointer<Tab> holder - the Page holder believed to be dragged
	 * Returns: void
	 */
	void SetCurrentHolder(TrecSubPointer<TPage, TabBar::Tab> holder, TrecPointer<TPage> page);


	/**
	 * Method: TIdeWindow::GetEnvironmentDirectory
	 * Purpose: Retrievs the Workign Directory of the TEnvironment
	 * Parameters: void 
	 * Returns: TrecPointer<TFileShell> - the Environment's working directory (could be NULL)
	 */
	TrecPointer<TFileShell> GetEnvironmentDirectory();

	
	/**
	 * Method: TIdeWindow::SetEnvironment
	 * Purpose: Sets the Environment of the Window
	 * Parameters: TrecPointer<TEnvironment> env - the Environment to manage
	 * Returns: void
	 */
	virtual void SetEnvironment(TrecPointer<TEnvironment> env) override;

	/**
	 * Method: TIdeWindow::SaveAll
	 * Purpose: Sends the Save signal to all MiniAPps registered
	 * Parameters: void
	 * Returns: void
	 */
	void SaveAll();

	/**
	 * Method: TIdeWindow::SaveCurrent
	 * Purpose: Sends the Save signal to the current MiniApp, if set
	 * Parameters: void
	 * Returns: void
	 */
	void SaveCurrent();

	/**
	 * Method: TIdeWindow::SetCurrentApp
	 * Purpose: Sets the focus to the provided MiniApp
	 * Parameters: TrecPointer<MiniApp> app - the MiniApp to focus on
	 * Returns: void
	 */
	void SetCurrentApp(TrecPointer<MiniApp> app);

	/**
	 * Method: TIdeWindow::OpenFile
	 * Purpose: Allows a File Handler to command the Window to open a new file (will currently just log the fileto open for now)
	 * Parameters: TrecPointer<TFileShell> - representation of the file to open
	 * Returns: UINT - error code (0 for success)
	 */
	UINT OpenFile(TrecPointer<TFileShell> shell);
	
	TrecPointer<TPage> AddNewPage(TPageEnvironment::handler_type hType, TString name);

protected:

	TString GetTargetPageType(TDataArray<TString> handlerTypes);

	TrecPointer<TPage> AddNewPage();

	/**
	 * Whether the main Page is a Tab Bar
	 */
	TString mainPageName;

	/**
	 * Space to offer the Tab Bars
	 */
	UINT pageBarSpace;

	/**
	 * the Amount of space to offer the main Page
	 */
	UINT mainViewSpace;


	/**
	 * Holder being dragged
	 */
	TrecSubPointer<TPage, TabBar::Tab> currentHolder;


	/**
	 * Method: TIdeWindow::DrawOtherPages
	 * Purpose: Draws the other page it has set up
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void DrawOtherPages()override;


	/**
	 * the main body Page
	 */
	TrecSubPointer<TPage, TabPage> body;

	/**
	 * the basic console Page
	 */
	TrecSubPointer<TPage, TabPage> basicConsole;

	/**
	 * the deep console Page
	 */
	TrecSubPointer<TPage, TabPage> deepConsole;

	/**
	 * the upper right Panel Page
	 */
	TrecSubPointer<TPage, TabPage> upperRight;

	/**
	 * the Lower right Panel Page
	 */
	TrecSubPointer<TPage, TabPage> lowerRight;

	/**
	 * the upper Left Panel Page
	 */
	TrecSubPointer<TPage, TabPage> upperLeft;

	/**
	 * the Lower Left Panel Page
	 */
	TrecSubPointer<TPage, TabPage> lowerLeft;

	/**
	 * Page to focus on (used when shifting the boundaries between two or three pages)
	 */
	TrecSubPointer<TPage, TabPage> focusPage, tabPage;


	/**
	 * Brush to draw the Tab Spce with
	 */
	TrecPointer<TBrush> panelbrush;


	/**
	 * The current MiniApp to focus on
	 */
	TrecPointer<MiniApp> currentApp;

	/**
	 * List of MiniApps
	 */
	TDataArray<TrecPointer<MiniApp>> apps;
};

