#pragma once
#include "TControl.h"

/**
 * Class: TScrollerControl
 * Purpose: Specializes in managing scrolling
 */
class _ANAFACE_DLL TScrollerControl : public TControl
{
public:

	/**
	 * Method: TScrollerControl::TScrollerControl
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Returns: New Tree Object
	 */
	TScrollerControl(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> styles);

	/**
	 * Method: TScrollerControl::onDraw
	 * Purpose: Draws the control
	 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
	 * Returns: void
	 */
	void onDraw(TObject* obj)override;

	/**
	 * Method: TScrollerControl::SetChildControl
	 * Purpose: Sets the control to move around
	 * Parameters: TrecPointer<TControl> cont - The Control to scroll along
	 * Returns: void
	 */
	void SetChildControl(TrecPointer<TControl> cont);

	/**
	 * Method: TScrollerControl::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: RECT r - the new location for the control
	 * Returns: void
	 */
	virtual void Resize(D2D1_RECT_F&)override;

	/**
	 * Method: TScrollerControl::OnLButtonDown
	* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)override;
	/**
	 * Method: TScrollerControl::OnMouseMove Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;
	/**
	 * Method: TScrollerControl::OnLButtonUp
	 * Purpose: Allows control to catch the Left Button Up event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	/**
	 * Method: TScrollerControl::GetChildControl
	 * Purpose: Rturns the control that the scroller is manipulating
	 * Parameters: void
	 * Returns: TrecPointer<TControl> - the control held by this control
	 */
	TrecPointer<TControl> GetChildControl();

	/* Purpose: to allow the TControl to shift it's contents according to how it is scrolled.
	 * Caller: Member TScrollBars, either vScroll or hScroll
	 * Parameter: int (x) how to shift itself horizontally
	 * Parameter: int (y) how to shift itself vertically
	 * return: bool, just because
	 * Details: Changes the contents location on the board, reflected in the changes to
	 *		TControl's location, but not where it draws, hence why it's own snip stays the same
	 *		However, the contents might include child locations so their snips might have to be
	 *		updated
	 */
	virtual bool onScroll(int x, int y)override;

protected:
	/**
	 * The control to move around
	 */
	TrecPointer<TControl> childControl;
	/**
	 * Method: TScrollerControl::RefreshScroll
	 * Purpose: Refreshes the Scroll bars
	 * Parameters: void
	 * Returns: void
	 */
	void RefreshScroll();
	/**
	 * Whether focus is on a scroll bar or not
	 */
	bool onScrollFocus;
};

