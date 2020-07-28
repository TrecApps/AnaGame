#pragma once
#include "TControl.h"
#include "TLayoutEx.h"
#include "TTextField.h"
#include "TCheckBox.h"
#include "TRadioButton.h"
#include "TComboBox.h"
#include "TCheckBox.h"
#include "TFlyout.h"
#include "TArena.h"
//#include "TVideo.h"
#include "TContextMenu.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TPromptControl.h"
#include "TTreeDataBind.h"

class AnafaceUI;
typedef struct _ANAFACE_DLL auiHold {
	TArray<TControl> children;
	RECT location = { 0,0,0,0 };
	TrecPointer<TControl> current;
	TArray<TString> source;
}auiHold;

typedef struct tempHold
{
	TrecPointer<TString> file;
	int index;
}tempHold;

/*
* class AnafaceUI
* Purpose: enables holding multiple controls in the same space, one at a time.
*	Use for different pages
*/
class _ANAFACE_DLL AnafaceUI : public TControl
{
public:

	/*
	* Method: AnaFaceUI::AnafaceUI
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> rt - the render target to use
	*				TrecPointer <TArray<styleTable>> ta - the styles for Anaface
	*				HWND win - the window handle to use
	* Returns void
	*/
	AnafaceUI(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta, HWND);
	/*
	* Method: AnafaceUI::~AnafaceUI
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~AnafaceUI();

	/*
	 * Method: AnafaceUI::OnRButtonUp
	 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;
	/*
	 * Method: AnafaceUI::OnLButtonDown
	 * Purpose: Allows the AnafaceUI to catch when a tab is
	 * Parameters: UINT nFlags - flags provided by MFC
	 *			CPoint point - the point of the click
	 *			messageOutput * mOut - the result of the click (was it in the control?)
	 *			TDataArray<EventID_Cred>& eventAr - events and their handlers documented by the control
	 *			TDataArray<TControl*>& clickedControl - allows the control to report that this is a contestant for clicks
	 * Returns: void
	 */	
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)override;

	/*
	* Method: AnafaceUI::OnRButtonDown
	* Purpose: Allows Control to catch the RightmessageState::mouse button down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*/
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)override;
	/*
	* Method: AnafaceUI::OnMouseMove
	* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*/
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)override;

	/*
	* Method: AnafaceUI::OnLButtonDblClk
	* Purpose: Allows control to catch the DOuble Click event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;
	/*
	* Method: AnafaceUI::OnLButtonUp
	* Purpose: Allows control to catch the Left Button Up event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;
	/*
	* Method: AnafaceUI::OnChar
	* Purpose: Allows Controls to repond to character input
	* Parameters: bool fromChar - can be called either from on Key Down or OnChar
	*				UINT nChar - The ID of the character that was pressed
	*				UINT nRepCnt - how many times the character was processed for this event
	*				UINT nFlags - flags provided by MFC's Message system, not used
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;


	/*
	* Method: AnafaceUI::switchView
	* Purpose: Switches the view towards the specified control
	* Parameters: UINT x - the index of the control to use
	* Returns: bool - whether the operation is successful
	*/
	bool switchView(UINT x);

	/**
	 * Method: AnafaceUI::SwitchChildControl
	 * Purpose: Allows a child control to insert another control between it and THIS control (usually a scroller control)
	 * Parameters: TrecPointerSoft<TControl> curControl - the Control making the call (used to help parent control identify which child to replace)
	 *				 TrecPointer<TControl> newControl - the Control to replace it with
	 * Returns: void
	 */
	virtual void SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl)override;


	/*
	* Method: AnafaceUI::onCreate
	* Purpose: Sets up the switching control with AnafaceUI specific attributes
	* Parameters: RECT container - the location the control will be stored
	* Returns: bool
	*/
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	/*
	* Method: AnafaceUI::addControl
	* Purpose: Adds a new control to the list of child controls
	* Parameters: TrecPointer<TControl> control - the control to add
	*				TString tabName - the name to give the tab
	* Returns: int - the index of the control (-1 if failed)
	*/
	int addControl(TrecPointer<TControl> control, TString tabName);
	/*
	* Method: AnafaceUI::addControl
	* Purpose: Adds a new control that needs to be read into memory
	* Parameters: CArchive * arch - the file to read from
	* Returns: int - 0 if successful
	* Note: INCOMPLETE - not properly implimented, DEPRECIATED - CArchive should be replaced with TFile
	*/
	int addControl(TFile* arch);
	/*
	* Method: AnafaceUI::setDontAddControl
	* Purpose: Adds a control without adding it to the
	* Parameters: TrecPointer<TControl> control - the control to draw
	* Returns: void
	*/
	void setDontAddControl(TrecPointer<TControl> control);
	/*
	* Method: AnafaceUI::onDraw
	* Purpose: Draws the control and any tabs present
	* Parameters: void
	* Returns: void
	*/
	void onDraw(TObject* obj) override;
	/*
	* Method: AnafaceUI::GetChildAt
	* Purpose: Retrieves the child control at the given location
	* Parameters: UINT c - the index of the control to look for
	* Returns: TrecPointer<TControl> - the control at the given index
	*/
	TrecPointer<TControl> GetChildAt(UINT c);
	/*
	* Method: AnafaceUI::GetCurrentChild
	* Purpose: Retrieves the currently active body control
	* Parameters: void
	* Returns: TrecPointer<TControl> - the current control active
	*/
	TrecPointer<TControl> GetCurrentChild();

	virtual UCHAR* GetAnaGameType() override;

	/*
	* Method: AnafaceUI::AddNewTab
	* Purpose: Adds a new tab to the layout
	* Parameters: TString - tab name to display
	* Returns: void
	*/
	void AddNewTab(TString t);


	/*
	* Method: AnafaceUI::GetControlArea
	* Purpose: Retrieves the location of the main body control (tabs might reduce the actual size)
	* Parameters: void
	* Returns: RECT - the location of the main body area
	*/
	D2D1_RECT_F GetControlArea();

	/**
	 * Method: AnafaceUI::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: D2D1_RECT_F& r - the new location for the control
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r)override;

private:

	/**
	 * Control that's currently on
	 */
	TrecPointer<TControl> currentControl;
	/**
	 * Tabs (should become a TrecPointerSub)
	 */
	TrecPointer<TControl> tabs_base;

	/**
	 * Allows AnafaceUI to "Create" controls
	 */
	TrecPointer<TWindowEngine> windowEngine;

	/**
	 * Tab pointer (should be replaced with a TrecPointerSub)
	 */
	TLayoutEx* tabs;
	/**
	 * how much space to allocate for tabs
	 */
	int tabHeight;
	/**
	 * Window Handle
	 */
	HWND winHandle;
	/**
	 * How many unamed tabs there are
	 */
	UINT unknownTab;
	/**
	 * Whether to continue sending messages to the m=child control or not (DEPRECATED now that TIdeWindow and IDEPage exist)
	 */
	bool proceed;
};

