#include "TScrollerControl.h"

/**
 * Method: TScrollerControl::TScrollerControl
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
 * Returns: New Tree Object
 */
TScrollerControl::TScrollerControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> styles) :TControl( rt, styles)
{
	onScrollFocus = false;
}

/**
 * Method: TScrollerControl::onDraw
 * Purpose: Draws the control
 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
 * Returns: void
 */
void TScrollerControl::onDraw(TObject* obj)
{
	if (!isActive || !childControl.Get())
		return;

	if ((location.bottom - location.top) < 1 || (location.right - location.left) < 1)
		return;



	RefreshScroll();


	if (vScroll.Get() || hScroll.Get())
	{

		D2D1_RECT_F clipRect = location;
		clipRect.bottom -= GetScrollbarBoxSize();
		clipRect.right -= GetScrollbarBoxSize();

		// Need to generate a geometry
		drawingBoard->AddLayer(clipRect);

		if (vScroll.Get())
			vScroll->Refresh(clipRect, childControl->getLocation());
		if (hScroll.Get())
			hScroll->Refresh(clipRect, childControl->getLocation());
	}
	childControl->onDraw(obj);

	if (vScroll.Get() || hScroll.Get())
	{
		drawingBoard->PopLayer();
	}
	if (vScroll.Get())
		vScroll->onDraw(drawingBoard->GetRenderer().Get());
	if (hScroll.Get())
		hScroll->onDraw(drawingBoard->GetRenderer().Get());

}

/**
 * Method: TScrollerControl::SetChildControl
 * Purpose: Sets the control to move around
 * Parameters: TrecPointer<TControl> cont - The Control to scroll along
 * Returns: void
 */
void TScrollerControl::SetChildControl(TrecPointer<TControl> cont)
{
	childControl = cont;
	children.Clear();
	TControl::addChild(cont);
}

/**
 * Method: TScrollerControl::Resize
 * Purpose: Resizes the control upon the window being resized
 * Parameters: RECT r - the new location for the control
 * Returns: void
 */
void TScrollerControl::Resize(D2D1_RECT_F& loc)
{
	location = loc;
	RefreshScroll();
}

/**
 * Method: TScrollerControl::OnLButtonDown
* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
 */
void TScrollerControl::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	if (!isContained(&point, &location))
		return;

	if (vScroll.Get() && vScroll->OnLButtonDown(nFlags, point, mOut))
	{
		resetArgs();
		args.eventType = R_Message_Type::On_Select_Scroller;
		args.point = point;
		args.methodID = -1;
		args.isClick = args.isLeftClick = true;
		args.control = nullptr;

		eventAr.push_back(EventID_Cred(R_Message_Type::On_Select_Scroller, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis), vScroll));
		return;
	}


	if (hScroll.Get() && hScroll->OnLButtonDown(nFlags, point, mOut))
	{
		resetArgs();
		args.eventType = R_Message_Type::On_Select_Scroller;
		args.point = point;
		args.methodID = -1;
		args.isClick = args.isLeftClick = true;
		args.control = nullptr;

		eventAr.push_back(EventID_Cred(R_Message_Type::On_Select_Scroller, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis), hScroll));
		return;
	}

	if (childControl.Get())
		childControl->OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
}
/**
 * Method: TScrollerControl::OnMouseMove Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
 * Returns: void
 */
void TScrollerControl::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	if (onScrollFocus)
	{
		if (vScroll.Get())
			vScroll->OnMouseMove(nFlags, point, mOut);
		if (hScroll.Get())
			hScroll->OnMouseMove(nFlags, point, mOut);
		return;
	}

	if (childControl.Get())
		childControl->OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
}
/**
 * Method: TScrollerControl::OnLButtonUp
 * Purpose: Allows control to catch the Left Button Up event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 */
void TScrollerControl::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (vScroll.Get())
		vScroll->OnLButtonUp(nFlags, point, mOut);
	if (hScroll.Get())
		hScroll->OnLButtonUp(nFlags, point, mOut);

	if (childControl.Get())
		childControl->OnLButtonUp(nFlags, point, mOut, eventAr);
	onScrollFocus = false;
}

/**
 * Method: TScrollerControl::GetChildControl
 * Purpose: Rturns the control that the scroller is manipulating
 * Parameters: void
 * Returns: TrecPointer<TControl> - the control held by this control
 */
TrecPointer<TControl> TScrollerControl::GetChildControl()
{
	return childControl;
}
bool TScrollerControl::onScroll(int x, int y)
{
	if (childControl.Get())
	{
		childControl->onScroll(x, y);
	}

	return true;
}
/**
 * Method: TScrollerControl::RefreshScroll
 * Purpose: Refreshes the Scroll bars
 * Parameters: void
 * Returns: void
 */
void TScrollerControl::RefreshScroll()
{
	if (!childControl.Get())
		return;
	D2D1_RECT_F childLocation = childControl->getLocation();
	if (!vScroll.Get() && ((location.bottom - location.top) < (childLocation.bottom - childLocation.top)))
	{
		// We need to create the Vertical ScrollBar
		vScroll =  TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this, ScrollOrient::so_vertical);
	}

	if (!hScroll.Get() && ((location.right - location.left) < (childLocation.right - childLocation.left)))
	{
		// We need to create the horizontal ScrollBar
		hScroll =  TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this, ScrollOrient::so_horizontal);
	}

	//if (vScroll && ((location.bottom - location.top) > (childLocation.bottom - childLocation.top)))
	//{

	//}


	D2D1_RECT_F clipRect = location;
	if(vScroll.Get())
		clipRect.bottom -= GetScrollbarBoxSize();
	if(hScroll.Get())
		clipRect.right -= GetScrollbarBoxSize();
	
	if (vScroll.Get())
		vScroll->Refresh(clipRect, childLocation);
	if (hScroll.Get())
		hScroll->Refresh(clipRect, childLocation);
}
