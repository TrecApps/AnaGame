
#include "TFlyout.h"



/*
* Method: (TFlyout) (Constructor) 
* Purpose: Sets up the basic features of the TFlyout
* Parameters: TrecPointer<DrawingBoard>cd, 
*			TrecPointer<TArray<styleTable>> ta - the list of styles to draw from
* Returns: void
*/
TFlyout::TFlyout(TrecPointer<TControl> cont)
{
    if (!cont.Get())
        throw L"Don't create a TFlyout with a Null Control!";
    control = cont;


}

/*
* Method: (TFlyout) (Destructor)
* Purpose: Cleans up the TFlyout array list
* Parameters: void
* Returns: void
*/
TFlyout::~TFlyout()
{

}





/**
 * Method: TFlyout::AfterDraw
 * Purpose: Allows a Window Object to Draw the Flyout last, while being able to set it's drawing space
 * Parameters: D2D1_RECT_F& contain - the location it is allowed to draw in
 * Returns: void
 */
void TFlyout::AfterDraw(D2D1_RECT_F& contain)
{
	if (control.Get())
	{
		control->Resize(contain);
		control->onDraw();
	}
}

/**
 * Method: TFlyout::AfterDraw
 * Purpose: Allows a Window Object to Draw the Flyout last, while being able to set it's drawing space
 * Parameters: D2D1_RECT_F& contain - the location it is allowed to draw in
 * Returns: void
 */
void TFlyout::AfterDraw()
{
	if (control.Get())
	{
		control->onDraw();
	}
}

/**
 * Method: TFlyout::SwitchChildControl
 * Purpose: Allows the TControl to insert a new control between itsef and the flyout (most likely a TScrollerControl)
 * Parameters: TrecPointerSoft<TControl> c1 - the current child control (lets the parent control know which control to replace it with)
 *              TrecPointer<TControl> c2 - the control to swap it with
 * Returns: void
 */
void TFlyout::SwitchChildControl(TrecPointerSoft<TControl> c1, TrecPointer<TControl> c2)
{
	if (c1.Get() == control.Get() && c1.Get() && c2.Get())
	{
		control = c2;
	}
}

/*
 * Method: TFlyout::OnMouseMove
 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
 * Returns: void
 */
void TFlyout::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
    if (control.Get())
        control->OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
}

/*
 * Method: TFlyout::OnLButtonDown
 * Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 */
void TFlyout::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
    if (control.Get())
        control->OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
}

/*
 * Method: TFlyout::OnLButtonUp
 * Purpose: Allows control to catch the Left Button Up event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 */
void TFlyout::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
    if (control.Get())
        control->OnLButtonUp(nFlags, point, mOut, eventAr);
}
/**
 * Method: TFlyout::SetSelf
 * Purpose: Enables the TFlyout to hold a reference to itself
 * Parameters: TrecPointer<TFlyout> self - the TrecReference to itself
 * Returns: void
 */
void TFlyout::SetSelf(TrecPointer<TFlyout> self)
{
    if(self.Get() != this)
        throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
    tThis = TrecPointerKey::GetSoftPointerFromTrec<TFlyout>(self);

    holder = TrecPointerKey::GetNewTrecPointerAlt<TParentHolder, TFlyoutParentHolder>(tThis);

    if (control.Get())
        control->setParent(holder);
}


/**
 * Method: TFlyoutParentHolder::TFlyoutParentHolder
 * Purpose: Constructor
 * Parameters: void
 * Returns: New TFlyoutParentHolder
 */
TFlyoutParentHolder::TFlyoutParentHolder(TrecPointerSoft<TFlyout> flyout)
{
	this->flyout = TrecPointerKey::GetTrecPointerFromSoft<TFlyout>(flyout);
}

/**
 * Method: TFlyoutParentHolder::SwitchChildControl
 * Purpose: Allows the TControl to insert a new control between itse;f and it's parent (most likely a TScrollerControl)
 * Parameters: TrecPointerSoft<TControl> c1 - the current child control (lets the parent control know which control to replace it with
 *              TrecPointer<TControl> c2 - the control to swap it with
 * Returns: void
 */
void TFlyoutParentHolder::SwitchChildControl(TrecPointerSoft<TControl> c1, TrecPointer<TControl> c2)
{
	if (flyout.Get())
		flyout->SwitchChildControl(c1, c2);
}
