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
#include "TSpriteControl.h"

#include "TTabBar.h"

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


/**
 * Class: TabControlContent
 * Purpose: Version of the TabContent class that holds a regular TControl as it's content
 * Note: Messages sent through here will be forwarded to the underlying TCOntrol, and Events that the TControls report will be sent back up the pipeline
 * 
 * SuperClass: TabContent - interface with the tab
 */
class _ANAFACE_DLL TabControlContent: public TabContent
{
public:
	/**
	 * Method: TabControlContent::TabControlContent
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new TabContent object
	 */
	TabControlContent();

	/**
	 * Method: TabControlContent::~TabControlContent
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TabControlContent();

	/**
	 * Method: TabControlContent::Resize
	 * Purpose: Calls Resize on the underlying content object
	 * Parameters: const D2D1_RECT_F& loc - the new location the content is expected to occupy
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Resize(const D2D1_RECT_F& loc) override;

	/**
	 * Method: TabControlContent::GetContentType
	 * Purpose: Returns the Content Type, allowing users of this object to cast it into the right sub-type and take it from there
	 * Parameters: void
	 * Returns: TabContentType - the type of content this is expected to yield
	 *
	 * Attributes: override
	 */
	virtual TabContentType GetContentType() override;

	/*
	 * Method: TabControlContent::OnRButtonUp
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
	* Method: TabControlContent::OnLButtonDown
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
	* Method: TabControlContent::OnRButtonDown
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
	* Method: TabControlContent::OnMouseMove
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
	 * Method: TabControlContent::OnMouseLeave
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
	 * Method: TabControlContent::OnLButtonDblClk
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
	* Method: TabControlContent::OnLButtonUp
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
	* Method: TabControlContent::OnChar
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
	 * Method: TabControlContent::SetControl
	 * Purpose: Sets up the Control to hold
	 * Parameters: TrecPointer<TControl> control - the control to hold
	 * Returns: void
	 */
	void SetControl(TrecPointer<TControl> control);

	/**
	 * Method: TabControlContent:GetControl
	 * Purpose: Retrieves the underlying Control
	 * Parameters: void
	 * Returns: TrecPointer<TControl> - the control this object holds
	 */
	TrecPointer<TControl> GetControl();

	/**
	 * Method: TabControlContent::HasContent
	 * Purpose: Reports whether this object has the desired content
	 * Parameters: void
	 * Returns: bool - whether the Tab Content is populated or not
	 *
	 * Attributes: override
	 */
	virtual bool HasContent() override;

	/**
	 * Method: TabControlContent::Draw
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
	TrecPointer<TControl> control;
};



/*
* Class: AnafaceUI
* Purpose: enables holding multiple controls in the same space, one at a time.
*	Use for different pages
* 
* SuperClass: TControl
*/
class _ANAFACE_DLL AnafaceUI : public TControl
{
public:

	/**
	 * Method: AnafaceUI::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

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
	 * 
	 * Attributes: override; message
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
	 * 
	 * Attributes: override; message
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
	 * 
	 * Attributes: override; message
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
	 * 
	 * Attributes: override; message
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
	 * 
	 * Attributes: override; message
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
	 * 
	 * Attributes: override; message
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
	 * 
	 * Attributes: override; message
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
	 * 
	 * Attributes: override
	 */
	virtual void SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl)override;


	/*
	* Method: AnafaceUI::onCreate
	* Purpose: Sets up the switching control with AnafaceUI specific attributes
	* Parameters: RECT container - the location the control will be stored
	* Returns: bool
	 * 
	 * Attributes: override
	*/
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	/*
	* Method: AnafaceUI::addControl
	* Purpose: Adds a new control to the list of child controls
	* Parameters: TrecPointer<TabContent> control - the control to add
	*				TString tabName - the name to give the tab
	* Returns: int - the index of the control (-1 if failed)
	*/
	int addControl(TrecPointer<TabContent> control, TString tabName);

	/*
	* Method: AnafaceUI::onDraw
	* Purpose: Draws the control and any tabs present
	* Parameters: void
	* Returns: void
	 * 
	 * Attributes: override
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
	* Returns: TrecPointer<TabContent> - the current control active
	*/
	TrecPointer<TabContent> GetCurrentChild();

	virtual UCHAR* GetAnaGameType() override;


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
	 * 
	 * Attributes: override
	 */
	void Resize(D2D1_RECT_F& r)override;

private:
	D2D1_RECT_F CalculateClientSpace();

	/**
	 * Current Content being Displayed, replaces the "currentControl" (TrecPointer<TControl>) attribute
	 */
	TrecPointer<TabContent> currentContent;

	/**
	 * Holds all of the tabs
	 */
	TTabBar bar;

	/**
	 * How to show the tab bar (0 for don't show, 1 for top, 2 for bottom)
	 */
	UCHAR tabShow;

	/**
	 * Allows AnafaceUI to "Create" controls
	 */
	TrecPointer<TWindowEngine> windowEngine;

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

};

