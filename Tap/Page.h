#pragma once
#include "Tap_dll.h"
#include <AnafaceUI.h>
#include <TObject.h>
#include "EventHandler.h"
//#include "MiniHandler.h"
#include <TWindowEngine.h>
#include <DrawingBoard.h>

class TWindow;

class MiniHandler;
class TAnimationManager;

typedef enum class RenderTargetType
{
	render_target_unknown,
	render_target_device_context,
	render_target_hwnd,
	render_target_dc,
	render_target_dxgi
}RenderTargetType;


/**
 * Class: TPageParentHolder
 * Allows Root TControls to reference the Page as a Parent, event though Pages are not known to the TControl
 */
class TPageParentHolder: public TParentHolder
{
public:

	/**
	 * Method: TPageParentHolder::TPageParentHolder
	 * Purpose: Constructor
	 * Parameters: TrecPointerSoft<class Page> page -  the Page to serve as the Parent
	 * Returns: New Page Holder Object
	 */
	TPageParentHolder(TrecPointerSoft<class Page> page);

	/**
	 * Method: TPageParentHolder::SwitchChildControl
	 * Purpose: Instructs the Page to treat the provided TControl as the new Root Control
	 * Parameters: TrecPointerSoft<TControl> cur - the current root control
	 *				TrecPointer<TControl> newTControl - the new control to set as root
	 * Returns: void
	 */
	virtual void SwitchChildControl(TrecPointerSoft<TControl> cur, TrecPointer<TControl> newTControl)override;
private:
	/**
	 * The Parent Page holding the Control
	 */
	TrecPointerSoft<class Page> parent;
};


/* Class: Page
* Purpose: Provide the foundation through which Drawing resources can be set up on a certain Window or device context,
*	Allows for refactoring of code that is essentially repeated
*/
class _TAP_DLL Page : public TObject
{
	friend class TrecPointerKey;
	friend class TPageParentHolder;
public:

	/**
	 * Method: Page::~Page
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~Page();


	/**
	 * Method: static Page::GetWindowPage
	 * Purpose: Proides a new page set for just 2D drawing
	 * Parameters: TrecPointer<TInstance> - instance under which this page is created
	 *				TrecPointer<TWindow> - the WIndow to hold this page
	 *				TrecPointer<EventHandler> - the Event Handler to work with this page
	 * Returns: TrecPointer<Page> - the page object requested
	 */
	static TrecPointer<Page> GetWindowPage(TrecPointer<TInstance>, TrecPointer<TWindow>,  TrecPointer<EventHandler>);


	/**
	 * Method: Page::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - in favor of the IDE Page in the TIdeWindow
	 */
	static TrecPointer<Page> GetSmallPage(TrecPointer<TInstance> in, TrecPointer<TWindow> window, D2D1_RECT_F area);


	/**
	 * Method: Page::SetAnaface
	 * Purpose: Sets up the Anaface of this Page as well as the Handler
	 * Parameters: TrecPointer<TFile> file - the file holding the TML file
	 *				TrecPointer<EventHandler> eh -  the handler that goes with this TML Anaface
	 * Returns: int error code (0 for success)
	 *
	 * Note: the file is expected to already be open
	 */
	int SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh);

	/**
	 * Method: Page::SetAnaface
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: Might be marked for Deprication
	 */
	int SetAnaface(TrecPointer<TFile> file, TDataArray<eventNameID>& id);


	/**
	 * Method: Page::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void SetAnaface(TrecPointer<TControl> newRoot);


	/**
	 * Method: Page::GetRootControl
	 * Purpose: Retrieves the Root Control of this page
	 * Parameters: void 
	 * Returns: TrecPointer<TControl> - the Root control of this Page
	 */
	TrecPointer<TControl> GetRootControl();

	/**
	 * Method: Page::ExtractRootControl
	 * Purpose: Retrieves the Root Control of this page, while also removing it from the Page
	 * Parameters: void
	 * Returns: TrecPointer<TControl> - the (now former) Root control of this Page
	 */
	TrecPointer<TControl> ExtractRootControl();


	/**
	 * Method: Page::GetWindowHandle
	 * Purpose: Retrieves the TWindow holding this page
	 * Parameters: void
	 * Returns: TrecPointer<TWindow> - the TWindow associated with this page
	 */
	TrecPointer<TWindow> GetWindowHandle();


	/**
	 * Method: Page::GetHandler
	 * Purpose: Retrieves the Handler held by this Page
	 * Parameters: void 
	 * Returns: TrecPointer<EventHandler> - the Event Handler responding to Events for this page
	 */
	TrecPointer<EventHandler> GetHandler();


	/**
	 * Method: Page::SetHandler
	 * Purpose: Sets the Handler for this Page
	 * Parameters: TrecPointer<EventHandler> eh - the Handler to set to
	 * Returns: void
	 */
	void SetHandler(TrecPointer<EventHandler> eh);

	/**
	 * Method: Page::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED 
	 */
	virtual UCHAR* GetAnaGameType()override;

	/**
	 * Method: Page::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - Use Resize or SetArea instead
	 */
	afx_msg void OnSize(UINT nType, int cx,	int cy);

	/**
	 * Method: Page::GetArenaEngine
	 * Purpose: Gets the Arena Engine Used by this Page
	 * Parameters: void
	 * Returns: TrecPointer<TArenaEngine> - the Arena Engine used by one of this Page's Controls
	 */
	TrecPointer<TArenaEngine> GetArenaEngine();

	/**
	 * Method: Page::CreateLayout
	 * Purpose: Calls OnCreate for the Root Control
	 * Parameters: void
	 * Returns: void
	 */
	void CreateLayout();

	/**
	 * Method: Page::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TWindowEngine* twe - the 3D Window Engine (not sure why this parameter is here)
	 * Returns: void
	 */
	virtual void Draw(TWindowEngine* twe = nullptr);

	/**
	 * Method: Page::GetArea
	 * Purpose: Retrieves the Area held by the Page
	 * Parameters: void
	 * Returns: D2D1_RECT_F -  the Region the Page claims it is drawing to
	 */
	D2D1_RECT_F GetArea();

	/**
	 * Method: Page::SetArea
	 * Purpose: Sets the Region of the Page
	 * Parameters: const D2D1_RECT_F& loc -  the location to provide this Page
	 * Returns: void
	 */
	void SetArea(const D2D1_RECT_F& loc);


	/**
	 * Method: Page::OnRButtonUp
	 * Purpose: Responds to the Right Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);

	/**
	 * Method: Page::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TrecPointer<TFlyout> fly -  any flyout that should have the chance to intercept the message first
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly);

	/**
	 * Method: Page::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 */
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);

	/**
	 * Method: Page::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TrecPointer<TFlyout> fly -  any flyout that should have the chance to intercept the message first
	 * Returns: void
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly);

	/**
	 * Method: Page::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);

	/**
	 * Method: Page::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TrecPointer<TFlyout> fly -  any flyout that should have the chance to intercept the message first
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly);

	/**
	 * Method: Page::OnChar
	 * Purpose: Responds to the Char Message
	 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
	 *				UINT nChar - the Character provided
	 *				UINT nRepCnt - the number of times to add it 
	 *				UINT nFlags - flags associated with the message
	 *				messageOutput* mOut - the result of the event
	 * Returns: bool 
	 */
	afx_msg virtual bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut);

	/**
	 * Method: Page::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 * Returns: void
	 */
	afx_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TrecPointer<TWindowEngine>);



	/**
	 * Method: Page::OnRButtonUp
	 * Purpose: Responds to the Right Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: Page::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 *				TrecPointer<TFlyout> fly -  any flyout that should have the chance to intercept the message first
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly);

	/**
	 * Method: Page::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: Page::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 *				TrecPointer<TFlyout> fly -  any flyout that should have the chance to intercept the message first
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly);

	/**
	 * Method: Page::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: Page::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				messageOutput* mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 *				TrecPointer<TFlyout> fly -  any flyout that should have the chance to intercept the message first
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly);

	/**
	 * Method: Page::OnChar
	 * Purpose: Responds to the Char Message
	 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
	 *				UINT nChar - the Character provided
	 *				UINT nRepCnt - the number of times to add it 
	 *				UINT nFlags - flags associated with the message
	 *				messageOutput* mOut - the result of the event
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 * Returns: bool
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: Page::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events 
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 */
	afx_msg void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: Page::OnDestroy
	 * Purpose: Reports whether the Page is ready to be destroyed
	 * Parameters: void
	 * Returns: bool - true if the Page doesn't have a handler or that handler is ready to be destroyed
	 */
	afx_msg bool OnDestroy();


	/**
	 * Method: Page::SetSelf
	 * Purpose: Allows TrecPointerKey to set a Trec Reference to itself
	 * Parameters: TrecPointer<Page> - self reference to the page 
	 * Returns: void
	 */
	void SetSelf(TrecPointer<Page>);

	/**
	 * Method: Page::GetInstance
	 * Purpose: Retrieves the TInstance associiated with this page
	 * Parameters: void
	 * Returns: TrecPointer<TInstance> - the TInstance under which this page was created
	 */
	TrecPointer<TInstance> GetInstance();


	/**
	 * Method: Page::PrepAnimations
	 * Purpose: Allows the Page to Prepare Animations requiested by the TControls it has generated
	 * Parameters: TAnimationManager& aManager - the Animation Manager that oversees all Story Board within a Window
	 * Returns: void
	 */
	void PrepAnimations(TAnimationManager& aManager);

	/**
	 * Method: Page::SetMiniHandler
	 * Purpose: Sets the Mini Handler for this Page
	 * Parameters: TrecSubPointer<EventHandler, MiniHandler> mh - the miniHandler to set
	 * Returns: void
	 *
	 * Note: DEPRICATED - MiniHandler class is depricated so this method is as well
	 */
	void SetMiniHandler(TrecSubPointer<EventHandler, MiniHandler> mh);

	/**
	 * Method: Page::AddStory
	 * Purpose: Adds to the list of StoryBoards to manage
	 * Parameters: TString& story - the name of the Story Board
	 *				bool persistent - whether this Story Board should be "persistent" or not
	 * Returns:
	 */
	void AddStory(TString& story, bool persistent = false);

	/**
	 * Method: Page::AddAnimations
	 * Purpose: Enables the Page to Create an Animation with an Animation Builder
	 * Parameters: TrecPointer<AnimationBuilder> builder - the Builder for the Animation
	 * Returns: void
	 */
	void AddAnimations(TrecPointer<AnimationBuilder> builder);

	// Handle Focus

	/**
	 * Method: Page::OnFocus
	 * Purpose: Lets the Page know that User is now focusing on it
	 * Parameters: void
	 * Returns: void
	 */
	void OnFocus();

protected:

	/**
	 * Method: Page::Page
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> -  the Drawing Board for All Drawing operations
	 * Returns: new Page object
	 */
	Page(TrecPointer<DrawingBoard>);

	/**
	 * Method: Page::GetAnimationByName
	 * Purpose: Retrieves the Animation Bulder by name
	 * Parameters: TString& name - name of the Animation Builder
	 * Returns: TrecPointer<AnimationBuilder> - the builder requested, null if not found
	 */
	TrecPointer<AnimationBuilder> GetAnimationByName(TString& name);

	/*
	 * The scale of the Page (meant to help with zooming)
	 */
	float scale;

	/**
	 * The Drawing Board to use
	 */
	TrecPointer<DrawingBoard> drawingBoard;
	
	/**
	 * Not used any more
	 */
	TrecPointer<TBrush> solidBrush;

	// Regular Resources
	

	/**
	 * Handle to the Context Device used by the Window
	 */
	HDC deviceH;

	/**
	 * Area Within the Window to draw
	 */
	D2D1_RECT_F area;

	// Anagame Specific Resources

	/**
	 * Incase we are using 3D Resources
	 */
	TrecPointer<TArenaEngine> engine;

	/**
	 * The Control to Draw when
	 */
	TrecPointer<TControl> rootControl;

	/**
	 * The class that manages specific actions
	 */
	 TrecPointer<EventHandler> handler;

	/**
	 * Intended for tabs whise contents have specific hanling specs
	 */
	TrecSubPointer<EventHandler, MiniHandler> miniHandler;

	/**
	 * The TInstance underwhich this Page is maintained
	 */
	 TrecPointer<TInstance> instance;

	 /**
	  * the Adjustment to make to the Drawing Board before sending the Draw command to the Controls
	  */
	D2D1_MATRIX_3X2_F adjustMatrix;

	/**
	 * the Window attached to the page
	 */
	TrecPointer<TWindow> windowHandle;


	/**
	 * Method: Page::SwitchChildControl
	 * Purpose: Allows the root control to insert a new control (most likely a TScollerControl) between the page and itself
	 * Parameters: TrecPointer<TControl> newRoot - the control to set as the new root
	 * Returns: void
	 */
	void SwitchChildControl(TrecPointer<TControl> newRoot);

	/**
	 * The Self reference to Provide
	 */
	TrecPointerSoft<Page> self;

	/**
	 * The Version of the Page to provide to the Root control to id it's parent
	 */
	TrecPointer<TParentHolder> selfHolder;

	// Anaface Resources


	/**
	 * List of Controls tht were clicked
	 */
	TDataArray<TControl*> clickedControl, rClickedControl, mouseFocusControl;


	// Animation Resources


	/**
	 * List of Aniation Builders reuested by a TControl and defined by the TML file
	 */
	TDataArray<TrecPointer<AnimationBuilder>> animations;

	/**
	 * List of Basic Story boards defined by the Anaface file
	 */
	TDataArray<TString> basicStoryBoards;

	/**
	 * List of Persistent Story Boards defined by the Anaface File
	 */
	TDataArray<TString> persistentStoryBoards;


};

