#pragma once
#include <Drawer.h>
#include "Page.h"
#include <TWindowEngine.h>
#include <TControl.h>

bool IsD2D1RectEqual(const D2D1_RECT_F& r1, const  D2D1_RECT_F& r2, float difference);
#include "TAnimationManager.h"

/**
 * Class: TWindow
 * Purpose: Base class for managing a Window in ANagame
 */
class _TAP_DLL TWindow : public Drawer
{
	friend class Page;
public:

	/**
	 * Method: TWindow::TWindow
	 * Purpose: Constructor
	 * Parameters: TString& name - the name of the Window
     *				TString& winClass - the name to tell Windows to use for this Window type
     *				UINT style - the Style of window to use
     *				HWND parent - the Handle to the Parent Window (if any)
     *				int commandShow - How to show the Window (provided in WinMain)
	 *				TrecPointer ins - pointer to the TInstance involved (hence why TInstance has a SetSelf method)
	 * Returns: New Window
	 */
	TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins);

	/**
	 * Method: TWindow::~TWindow
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TWindow();


	/**
	 * Method: TWindow::PrepareWindow
	 * Purpose: Sets the Window up for viewing
	 * Parameters: void
	 * Returns: int - error code (0 = success)
	 */
	virtual int PrepareWindow();


	/**
	 * Method: TWindow::CompileView
	 * Purpose: Compiles the Main View
	 * Parameters: TString& file - path of the TML file holding the Anaface
	 *				TrecPointer<EventHandler> eh - the Handler to the Main page
	 * Returns: int - error (0 == success)
	 */
	virtual int CompileView(TString& file, TrecPointer<EventHandler> eh);


	/**
	 * Method: TWindow::MovePageToTop
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * DEPRECATED - 
	 */
	bool MovePageToTop(TrecPointer<Page> p);

	/**
	 * Method: TWindow::GetWinClass
	 * Purpose: Retrievs the Window-level class of this window
	 * Parameters: void
	 * Returns: TString - the class name registered with Windows
	 */
	TString GetWinClass();

	/**
	 * Method: TWindow::GetWinName
	 * Purpose: Retrievs the Window-level name of this window
	 * Parameters: void
	 * Returns: TString - the name registered with Windows
	 */
	TString GetWinName();


	// Action Listeners

	/**
	 * Method: TWindow::Draw
	 * Purpose: Draws the window
	 * Parameters: void
	 * Returns: void
	 */
	virtual void Draw() override;

	/**
	 * Method: TWindow::Draw
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * DEPRICATED
	 */
	void Draw(Page& draw);

	/**
	 * Method: TWindow::InduceDraw
	 * Purpose: Sends a Message to Windows to send the Draw message
	 * Parameters: void
	 * Returns: void
	 * 
	 * Note: this method is provided to be called by nimation threads as Draw should only be called by the Message thread, not by Animation threads
	 */
	void InduceDraw();
	

	/**
	 * Method: TWindow::OnRButtonUp
	 * Purpose: Manages the Right Button Up Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 */
	afx_msg void OnRButtonUp(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnLButtonDown
	 * Purpose: Manages the Left Button Down Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 * Parameters:
	 * Returns:
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnRButtonDown
	 * Purpose: Manages the Right Button Down Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 */
	afx_msg void OnRButtonDown(UINT nFlags, TPoint);

	/**
	 * Method: TWindow::OnMouseMove
	 * Purpose: Manages the Mouse Move Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnLButtonDblClk
	 * Purpose: Manages the Double Click Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 */
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnLButtonUp
	 * Purpose: Manages the Left Button Up Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnChar
	 * Purpose: Manages the Char Message
	 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
	 *				UINT nChar - the Character provided
	 *				UINT nRepCnt - the number of times to add it 
	 *				UINT nFlags - flags associated with the message
	 * Returns: bool 
	 */
	afx_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags);

	/**
	 * Method: TWindow::OnWindowResize
	 * Purpose: Resizes the Window
	 * Parameters: UINT width - the new width of the window
	 *				UINT height - the new height of the Window
	 * Returns: void
	 */
	afx_msg void OnWindowResize(UINT width, UINT height);

	/**
	 * Method: TWindow::OnDestroy
	 * Purpose: Reports whether it is ready for destruction of not
	 * Parameters: void
	 * Returns: bool - whether it is ready for destruction
	 */
	afx_msg virtual bool OnDestroy();


	/**
	 * Method: TWindow::GetHandlePage
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED in favor of the Ide Window/IDE Page
	 */
	TrecPointer<Page> GetHandlePage(bool singleton);

	/**
	 * Method: TWindow::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED in favor of the Ide Window/IDE Page
	 */
	TrecPointer<Page> GetHandlePage(const TString& name);

	/**
	 * Method: TWindow::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED in favor of the Ide Window/IDE Page
	 */
	TrecPointer<Page> Get3DPage(bool singleton, TString& engineId);

	/**
	 * Method: TWindow::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED in favor of the Ide Window/IDE Page
	 */
	TrecPointer<Page> Get3DPage(bool singleton, TrecPointer<TArenaEngine> engine);


	/**
	 * Method: TWindow::LockWindow
	 * Purpose: Locks the Window from handling Messages
	 * Parameters: void
	 * Returns: void
	 */
	void LockWindow();

	/**
	 * Method: TWindow::UnlockWindow
	 * Purpose: Unlocks the Window allowing it to handle messages
	 * Parameters: void
	 * Returns: void
	 */
	void UnlockWindow();


	/**
	 * Method: TWindow::SetSelf
	 * Purpose: Allows the Window to hold a reference to itself
	 * Parameters: TrecPointer<TWindow> win - the self window to set to
	 * Returns: void
	 */
	void SetSelf(TrecPointer<TWindow> win);


	/**
	 * Method: TWindow::GetPageByArea
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED in favor of the Ide Window/IDE Page
	 */
	TrecPointer<Page> GetPageByArea(D2D1_RECT_F r);

	/**
	 * Method: TWindow::GetInstance
	 * Purpose: Retrievs the instance associated with this window
	 * Parameters: void 
	 * Returns: TrecPointer<TInstance> - the Instance this window is under
	 */
	TrecPointer<TInstance> GetInstance();


	/**
	 * Method: TWindow::SetUp3D
	 * Purpose: Configures the Window to support 3D drawing (if it doesn't already)
	 * Parameters: void
	 * Returns: whether the window is now set for 3D
	 */
	bool SetUp3D();

	/**
	 * Method: TWindow::CleanUp
	 * Purpose: Deletes the Page marked for deletion
	 * Parameters: void
	 * Returns: void
	 */
	void CleanUp();


	/**
	 * Method: TWindow::GetWindowEngine
	 * Purpose: Retirves the 3D Engine associated with this window
	 * Parameters: void 
	 * Returns: TrecPointer<TWindowEngine> - the 3D window engine
	 */
	TrecPointer<TWindowEngine> GetWindowEngine();

	/**
	 * Method: TWindow::GetNewArenaEngine
	 * Purpose: Retirves a new Arena Engine 
	 * Parameters: void
	 * Returns: TrecPointer<TArenaEngine> - the Arena Engine produced
	 */
	TrecPointer<TArenaEngine> GetNewArenaEngine();

	/**
	 * Method: TWindow::GetNewArenaEngine
	 * Purpose: Retirves a new Arena Engine by name
	 * Parameters: TString& name - the name of the Arena Engine
	 * Returns: TrecPointer<TArenaEngine> - the Arena Engine produced
	 */
	TrecPointer<TArenaEngine> GetNewArenaEngine(TString& name);


	/**
	 * Method: TWindow::GetDrawingBoard
	 * Purpose: Retrieves the drawing board associated with this window
	 * Parameters: void 
	 * Returns: TrecPointer<DrawingBoard> - the drawing board held by the WIndow
	 */
	TrecPointer<DrawingBoard> GetDrawingBoard();


	/**
	 * Method: TWindow::PrepAnimations
	 * Purpose: Prepares animations held by the provided page
	 * Parameters: TrecPointer<Page> page - the page to prepare the animations for
	 * Returns: bool - whether the page was set and the Window matched
	 */
	bool PrepAnimations(TrecPointer<Page> page);

	/**
	 * Method:: TWindow::SetFlyout
	 * Pupose: Allows Flyouts in an Anaface to be set
	 * Parameters: TrecPointer<TFlyout> fly - the flyout to Draw
	 * Returns: void
	 *
	 * Note: This method is intended to be called by the Page Class. The Page looks for TFlyouts to draw and the Window takes it from there
	 */
	void SetFlyout(TrecPointer<TFlyout> fly);

protected:

	// Draw Other pages that are special to the Window


	/**
	 * Method: TWindow::DrawOtherPages
	 * Purpose: Draws other pages registered in this window
	 * Parameters: void
	 * Returns: void
	 */
	virtual void DrawOtherPages();

	/**
	 * instance managing the window
	 */
	TrecPointerSoft<TInstance> windowInstance;

	/**
	 * command used by Windows
	 */
	int command;

	/**
	 * Main content of the window
	 */
	TrecPointer<Page> mainPage;
	/**
	 * List of Pages
	 */
	TDataArray<TrecPointer<Page>> pages;
	/**
	 * Names used by Windows
	 */
	TString name, winClass;
	

	/**
	 * Used when multiple objects might want to access the same page by ID
	 */
	TMap<Page> keyPages;
	// Singleton Pages
	/**
	 * DEPRICATED
	 */
	TrecPointer<Page> _3DPage, handlePage;


	/**
	 * Whether the Window should ignore user interactivity. Effectivley disables Windows at the AnaGame level without disabling them at the Windows level
	 */
	bool locked;


	/**
	 * marker idicating whether a call to Draw should proceed or not
	 */
	UCHAR safeToDraw;

	//

	/**
	 * Self reference
	 */
	TrecPointerSoft<TWindow> self;

	/**
	 * Page to delete
	 */
	TrecPointer<Page> deletePage;


	// 3D Resource

	/**
	 * the 3D Window engine
	 */
	TrecPointer<TWindowEngine> d3dEngine;

	/**
	 * List of Arena Engines
	 */
	TTrecPointerArray<TArenaEngine> engines;


	/**
	 * allows animations to be done on this window
	 */
	TAnimationManager animationCentral;

	/**
	 * The Board that enables 2D drawing
	 */
	TrecPointer<DrawingBoard> drawingBoard;

	// Managing Scrolling

	/**
	 * Scroll Bar currently focusing on
	 */
	TrecPointer<TScrollBar> currentScrollBar;


	// Manage Flyouts

	/**
	 * Holds any flyout that needs to be drawn after everything else is
	 */
	TrecPointer<TFlyout> flyout;


};

