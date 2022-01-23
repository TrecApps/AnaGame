#pragma once
#include <TPage.h>
#include <TWindowEngine.h>
#include <TControl.h>
#include <TArenaEngine.h>
#include <TPage.h>

#include "TAnimationManager.h"

bool IsD2D1RectEqual(const D2D1_RECT_F& r1, const  D2D1_RECT_F& r2, float difference);


/**
 * Class: TProcess
 * Purpose: Serves as the Interface for the TInstance Class for the TWindow to Access
 */
class _TAP_DLL TProcess : public TObject
{
public:

	/**
	 * Method: TInstance::~TInstance
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TProcess();


	/**
	 * Method: TInstance::GetInstanceHandle
	 * Purpose: Retrievs the raw instance handle
	 * Parameters: void
	 * Returns: HINSTANCE - instance provided during construction
	 */
	virtual HINSTANCE GetInstanceHandle() = 0;

	/**
	 * Method: TInstance::GetFactory
	 * Purpose: retirevs the factory used for Direct2D
	 * Parameters: void
	 * Returns: TrecComPointer<ID2D1Factory1> - the factory used for Direct 2D drawing
	 */
	virtual TrecComPointer<ID2D1Factory1> GetFactory() = 0;

	virtual void SetSelf(TrecPointer<TProcess> s) = 0;
};



/**
 * Class: TWindow
 * Purpose: Base class for managing a Window in ANagame
 * 
 * SuperClass: Drawer
 */
class _TAP_DLL TWindow : public TObject
{
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
	TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins);

	/**
	 * Method: TWindow::~TWindow
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TWindow();


	/**
	 * Method: TWindow::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;


	/**
	 * Method: TWindow::PrepareWindow
	 * Purpose: Sets the Window up for viewing
	 * Parameters: void
	 * Returns: int - error code (0 = success)
	 * 
	 * Attributes: virtual
	 */
	virtual int PrepareWindow();


	/**
	 * Method: TWindow::CompileView
	 * Purpose: Compiles the Main View
	 * Parameters: TString& file - path of the TML file holding the Anaface
	 *				TrecPointer<EventHandler> eh - the Handler to the Main page
	 * Returns: int - error (0 == success)
	 * 
	 * Attributes: virtual
	 */
	virtual int CompileView(TString& file, TrecPointer<TPage::EventHandler> eh);



	/**
	 * Method: TWindow::GetWinClass
	 * Purpose: Retrieves the Window-level class of this window
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
	 * 
	 * Attributes: override
	 */
	virtual void Draw();



	/**
	 * Method: TWindow::InduceDraw
	 * Purpose: Sends a Message to Windows to send the Draw message
	 * Parameters: void
	 * Returns: void
	 * 
	 * Note: this method is provided to be called by Animation threads as Draw should only be called by the Message thread, not by Animation threads
	 */
	void InduceDraw();
	
	ag_msg void OnVideoEvent(WPARAM param);

	/**
	 * Method: TWindow::OnRButtonUp
	 * Purpose: Manages the Right Button Up Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 * 
	 * Attributes: message
	 */
	ag_msg void OnRButtonUp(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnLButtonDown
	 * Purpose: Manages the Left Button Down Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 * 
	 * Attributes: message; virtual
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, TPoint point);

	/**
	 * Method: TWindow::OnRButtonDown
	 * Purpose: Manages the Right Button Down Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 * 
	 * Attributes: message
	 */
	ag_msg void OnRButtonDown(UINT nFlags, TPoint);

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
	 * Method: TWindow::OnLButtonDblClk
	 * Purpose: Manages the Double Click Message
	 * Parameters: UINT nFlags - the flags associated with the message
	 *				TPoint point - the point that was clicked
	 * Returns: void
	 * 
	 * Attributes: message
	 */
	ag_msg void OnLButtonDblClk(UINT nFlags, TPoint point);

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
	 * Method: TWindow::OnChar
	 * Purpose: Manages the Char Message
	 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
	 *				UINT nChar - the Character provided
	 *				UINT nRepCnt - the number of times to add it 
	 *				UINT nFlags - flags associated with the message
	 * Returns: bool 
	 * 
	 * Attributes: message
	 */
	ag_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags);

	/**
	 * Method: TWindow::OnWindowResize
	 * Purpose: Resizes the Window
	 * Parameters: UINT width - the new width of the window
	 *				UINT height - the new height of the Window
	 * Returns: void
	 * 
	 * Attributes: message
	 */
	ag_msg virtual void OnWindowResize(UINT width, UINT height);

	/**
	 * Method: TWindow::OnDestroy
	 * Purpose: Reports whether it is ready for destruction of not
	 * Parameters: void
	 * Returns: bool - whether it is ready for destruction
	 * 
	 * Attributes: message
	 */
	ag_msg virtual bool OnDestroy();


	/**
	 * Method: TWindow::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: virtual
	 */
	ag_msg virtual bool OnScroll(const TPoint& point, const TPoint& direction);




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
	 * Method: TWindow::GetInstance
	 * Purpose: Retrievs the instance associated with this window
	 * Parameters: void 
	 * Returns: TrecPointer<TInstance> - the Instance this window is under
	 */
	TrecPointer<TProcess> GetInstance();


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
	bool PrepAnimations(TrecPointer<TPage> page);

	/**
	 * Method:: TWindow::SetFlyout
	 * Pupose: Allows Flyouts in an Anaface to be set
	 * Parameters: TrecPointer<TFlyout> fly - the flyout to Draw
	 * Returns: void
	 *
	 * Note: This method is intended to be called by the Page Class. The Page looks for TFlyouts to draw and the Window takes it from there
	 */
	void SetFlyout(TrecPointer<TPage> fly);


	/**
	 * Method: TWindow::GetWindowHandle
	 * Purpose: Returns the Window handle
	 * Parameters: void
	 * Returns: HWND - the handle to the Window object in the OS
	 */
	HWND GetWindowHandle();

	/**
	 * Method: TWindow::GetFactory
	 * Purpose: Returns the Direct2D Factory used by the window
	 * Parameters: void
	 * Returns: TrecComPointer<ID2D1Factory1> - the Pointer to the factory
	 */
	TrecComPointer<ID2D1Factory1> GetFactory();


	HDC GetTWindowDc();

	void FlushDc();

	/**
	 * Method: TIdeWindow::SetEnvironment
	 * Purpose: Sets the Environment of the Window
	 * Parameters: TrecPointer<TEnvironment> env - the Environment to manage
	 * Returns: void
	 */
	virtual void SetEnvironment(TrecPointer<TEnvironment> env);

	/**
	 * Method: TIdeWindow::GetEnvironment
	 * Purpose: Gets the Environment held by the Window
	 * Parameters: void
	 * Returns: TrecPointer<TEnvironment> - the Environment to manage
	 */
	TrecPointer<TEnvironment> GetEnvironment();


protected:

	/**
	 * the Environment to manage a Project
	 */
	TrecPointer<TEnvironment> environment;

	void HandleWindowEvents(TDataArray<TPage::EventID_Cred>& cred);

	virtual void RunWindowCommand(const TString& command);


	// Draw Other pages that are special to the Window
	/**
	 * the Direct2D factory used by this drawer
	 */
	TrecComPointer<ID2D1Factory1> directFactory;

	/**
	 * parent: the Handle to the window owning this window
	 * currentWindow: the Handle to the window
	 */
	HWND parent, currentWindow;

	/**
	 * Method: TWindow::DrawOtherPages
	 * Purpose: Draws other pages registered in this window
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual void DrawOtherPages();

	/**
	 * instance managing the window
	 */
	TrecPointerSoft<TProcess> windowInstance;

	/**
	 * command used by Windows
	 */
	int command;

	/**
	 * Used to track whether the Window has drawn or not
	 */
	bool hasDrawn;

	RECT size;

	/**
	 * Main content of the window
	 */
	TrecPointer<TPage> mainPage;
	/**
	 * List of Pages
	 */
	TDataArray<TrecPointer<TPage>> pages;
	/**
	 * Names used by Windows
	 */
	TString name, winClass;



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
	TrecPointer<TPage::TScrollBar> currentScrollBar;


	// Manage Flyouts

	/**
	 * Holds any flyout that needs to be drawn after everything else is
	 */
	TrecPointer<TPage> flyout;


};

/**
 * Class: TPageEnvironment
 * Purpose: Extends the TEnvironment to Support TPage and Handler Generation
 */
class _TAP_DLL TPageEnvironment : public TEnvironment
{
public:

	typedef enum class handler_type {
		ht_singular,
		ht_multiple,
		ht_ribbon,
		ht_file
	}handler_type;


	TPageEnvironment(TrecPointer<TFileShell> shell);

	/**
	 * Method: TPageEnvironment::GetPageAndHandler
	 * Purpose: Retrieves the Pages by the Specified name
	 * Parameters: const TString& name, TrecPointer<TPage> page, TrecPointer<TPage::EventHandler> handler
	 * Returns: void
	 *
	 * Note: If successful, the page and handler params should be assigned once the method returns
	 */
	void GetPageAndHandler(handler_type hType, const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler, TrecPointer<DrawingBoard> board, TrecPointer<TProcess> proc);

	/**
	 * Method: TPageEnvironment::GetPageList
	 * Purpose: Retrieves the available Page Names by extension
	 * Parameters: const TString& ext, TDataArray<TString>& extensions
	 * Returns: void
	 *
	 * Note: if ext is an empty String, then return all that the Environment has to offer
	 */
	void GetPageList(handler_type hType, const TString& ext, TDataArray<TString>& extensions);

protected:


	/**
	 * Method: TPageEnvironment::GetPageAndHandler
	 * Purpose: Retrieves the Pages by the Specified name
	 * Parameters: const TString& name, TrecPointer<TPage> page, TrecPointer<TPage::EventHandler> handler
	 * Returns: void
	 *
	 * Note: If successful, the page and handler params should be assigned once the method returns
	 */
	virtual void GetPageAndHandler_(handler_type hType, const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler, TrecPointer<DrawingBoard> board, TrecPointer<TProcess> proc) = 0;

	/**
	 * Method: TPageEnvironment::GetPageList
	 * Purpose: Retrieves the available Page Names by extension
	 * Parameters: const TString& ext, TDataArray<TString>& extensions
	 * Returns: void
	 *
	 * Note: if ext is an empty String, then return all that the Environment has to offer
	 */
	virtual void GetPageList_(handler_type hType, const TString& ext, TDataArray<TString>& extensions) = 0;
};