#pragma once
#include "TWindow.h"
#include "TEnvironment.h"

/**
 * Enum Class: ide_page_type
 * Purpose: Specifies the IDE Page's location in an IDE Window
 */
typedef enum class ide_page_type
{
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
 * Enum Class: anagame_page
 * Purpose: Specifies the Type of built-in page to produce
 */
typedef enum class anagame_page
{
	anagame_page_custom,          // a Custom page, the TML file is expected to be provided
	anagame_page_file_node,       // Presents a Page that holds a File node control
	anagame_page_command_prompt,  // Presents a page that holds a Command Prompt
	anagame_page_code_explorer,   // Presents a page that holds a node control that focuses on code elements
	anagame_page_object_explorer, // Presents a Page intended to present the properties of a given object
	anagame_page_code_file,       // Presents a Page that holds a Text Edit control intended to hold a code file
	anagame_page_arena,           // Presents a Page that holds a 3D arena control
	anagame_page_camera           // Presents a Page that provides a Camera Panel
};

class IDEPage;
class IDEPageHolder;
class MiniApp;

/**
 * Class: TIdeWindow
 * Purpose: Extends the WIndow class and partitions it's drawing space
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
	 * Returns: New IDE Window
	 */
	TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, UINT mainViewSpace, UINT pageBarSpace);

	/**
	 * Method: TIdeWindow::PrepareWindow
	 * Purpose: Prepares the Window while also managing the links between the IDE-Pages
	 * Parameters: void
	 * Returns: int - error code (0 = success)
	 */
	virtual int PrepareWindow()override;

	/**
	 * Method: TIdeWindow::OnLButtonUp
	 * Purpose: Manages the Left Button Up Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 */
	void OnLButtonUp(UINT nFlags, TPoint point);

	/**
	 * Method: TIdeWindow::OnMouseMove
	 * Purpose: Manages the Mouse Move Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that the mouse is now
	 * Returns: void
	 */
	void OnMouseMove(UINT nFlags, TPoint point)override;

	/**
	 * Method: TIdeWindow::OnLButtonDown
	 * Purpose: Manages the Left Button Down Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 */
	void OnLButtonDown(UINT nFlags, TPoint point)override;


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
	 * Returns: TrecSubPointer<Page, IDEPage> -  the Page generated
	 */
	TrecSubPointer<Page, IDEPage> AddNewPage(anagame_page pageType, ide_page_type pageLoc, TString name, TString tmlLoc, TrecPointer<EventHandler> handler, bool pageTypeStrict = false);

	/**
	 * Method: TIdeWindow::AddPage
	 * Purpose: Creates a built-in Page
	 * Parameters: anagame_page pageType - type of built-in page
	 *				ide_page_type pageLoc - the location to place the new Page
	 *				TString name - name of the page to write on the Tab
	 * Returns: TrecSubPointer<Page, IDEPage> -  the Page generated
	 */
	TrecSubPointer<Page, IDEPage> AddPage(anagame_page pageType, ide_page_type pageLoc, TString name);


	/**
	 * Method: TIdeWindow::CompileView
	 * Purpose: Compiles the Main View while also preparing all of the individual IDE pages
	 * Parameters: TString& file - path of the TML file holding the Anaface
	 *				TrecPointer<EventHandler> eh - the Handler to the Main page
	 * Returns: int - error (0 == success)
	 */
	int CompileView(TString& file, TrecPointer<EventHandler> eh)override;


	/**
	 * Method: TIdeWindow::SetCurrentHolder
	 * Purpose: Marks a Page Holder as being dragged by the User
	 * Parameters: TrecPointer<IDEPageHolder> holder - the Page holder believed to be dragged
	 * Returns: void
	 */
	void SetCurrentHolder(TrecPointer<IDEPageHolder> holder);

	/**
	 * Method: TIdeWindow::SetEnvironment
	 * Purpose: Sets the Environment of the Window
	 * Parameters: TrecPointer<TEnvironment> env - the Environment to manage
	 * Returns: void
	 */
	void SetEnvironment(TrecPointer<TEnvironment> env);

	/**
	 * Method: TIdeWindow::GetEnvironmentDirectory
	 * Purpose: Retrievs the Workign Directory of the TEnvironment
	 * Parameters: void 
	 * Returns: TrecPointer<TFileShell> - the Environment's working directory (could be NULL)
	 */
	TrecPointer<TFileShell> GetEnvironmentDirectory();


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

protected:
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
	TrecPointer<IDEPageHolder> currentHolder;

	/**
	 * the Environment to manage a Project
	 */
	TrecPointer<TEnvironment> environment;


	/**
	 * Method: TIdeWindow::DrawOtherPages
	 * Purpose: Draws the other page it has set up
	 * Parameters: void
	 * Returns: void
	 */
	virtual void DrawOtherPages()override;


	/**
	 * the main body Page
	 */
	TrecSubPointer<Page, IDEPage> body;

	/**
	 * the basic console Page
	 */
	TrecSubPointer<Page, IDEPage> basicConsole;

	/**
	 * the deep console Page
	 */
	TrecSubPointer<Page, IDEPage> deepConsole;

	/**
	 * the upper right Panel Page
	 */
	TrecSubPointer<Page, IDEPage> upperRight;

	/**
	 * the Lower right Panel Page
	 */
	TrecSubPointer<Page, IDEPage> lowerRight;

	/**
	 * the upper Left Panel Page
	 */
	TrecSubPointer<Page, IDEPage> upperLeft;

	/**
	 * the Lower Left Panel Page
	 */
	TrecSubPointer<Page, IDEPage> lowerLeft;

	/**
	 * Page to focus on (used when shifting the boundaries between two or three pages)
	 */
	TrecSubPointer<Page, IDEPage> focusPage;


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

