#pragma once
#include "TLayout.h"

class TFlyout;

/**
 * Class: TFlyoutParentHolder
 * Purpose: Allows a TControl to interact with a TFlyout as if it was the Control's parent
 */
class TFlyoutParentHolder : public TParentHolder
{
public: 

	/**
	 * Method: TFlyoutParentHolder::TFlyoutParentHolder
	 * Purpose: Constructor
	 * Parameters: void
	 * Returns: New TFlyoutParentHolder
	 */
	TFlyoutParentHolder(TrecPointerSoft<TFlyout> flyout);

	/**
	 * Method: TFlyoutParentHolder::SwitchChildControl
	 * Purpose: Allows the TControl to insert a new control between itse;f and it's parent (most likely a TScrollerControl)
	 * Parameters: TrecPointerSoft<TControl> c1 - the current child control (lets the parent control know which control to replace it with
	 *              TrecPointer<TControl> c2 - the control to swap it with
	 * Returns: void
	 */
	virtual void SwitchChildControl(TrecPointerSoft<TControl> c1, TrecPointer<TControl> c2)override;
private:
	/**
	 * The Flyout serving as the TControl's parent
	 */
	TrecPointer<TFlyout> flyout;
};


/**
 * Class TFlyout
 * Purpose: Base class for Controls intended to be drawn after the general Anaface is drawn, allowing
 *	Windows to draw them last and over other content as necessary
 */
class _ANAFACE_DLL TFlyout
{
public:
	/**
	 * Method: TFlyout::TFlyout
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TControl> cont -  the control to hold
	 * Returns: New TFlyout object
	 */
	TFlyout(TrecPointer<TControl> cont);
	
	/**
	 * Method: TFlyout::~TFlyout
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TFlyout();

	/**
	 * Method: TFlyout::AfterDraw
	 * Purpose: Allows a Window Object to Draw the Flyout last, while being able to set it's drawing space
	 * Parameters: D2D1_RECT_F& contain - the location it is allowed to draw in
	 * Returns: void
	 */
	void AfterDraw(D2D1_RECT_F& contain);
	

	/**
	 * Method: TFlyout::AfterDraw
	 * Purpose: Allows a Window Object to Draw the Flyout last, while being able to set it's drawing space
	 * Parameters: D2D1_RECT_F& contain - the location it is allowed to draw in
	 * Returns: void
	 */
	void AfterDraw();


	/**
	 * Method: TFlyout::SwitchChildControl
	 * Purpose: Allows the TControl to insert a new control between itsef and the flyout (most likely a TScrollerControl)
	 * Parameters: TrecPointerSoft<TControl> c1 - the current child control (lets the parent control know which control to replace it with)
	 *              TrecPointer<TControl> c2 - the control to swap it with
	 * Returns: void
	 */
	void SwitchChildControl(TrecPointerSoft<TControl> c1, TrecPointer<TControl> c2);


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
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls);

	/*
	 * Method: TFlyout::OnLButtonDown
	 * Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons);

	/*
	 * Method: TFlyout::OnLButtonUp
	 * Purpose: Allows control to catch the Left Button Up event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: TFlyout::SetSelf
	 * Purpose: Enables the TFlyout to hold a reference to itself
	 * Parameters: TrecPointer<TFlyout> self - the TrecReference to itself
	 * Returns: void
	 */
	void SetSelf(TrecPointer<TFlyout> self);

protected:
	TrecPointer<TControl> control;
	TrecPointerSoft<TFlyout> tThis;
	TrecPointer<TParentHolder> holder;
};

