#pragma once
#include "TLayoutEx.h"
class _ANAFACE_DLL TDataBind : public TControl
{
public:

	/**
	 * Method: TDataBind::TDataBind
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Returns: New Tree Object
	 */
	TDataBind(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
	/**
	 * Method: TDataBind::~TDataBind
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TDataBind();

	/**
	 * Method: TDataBind::onDraw
	 * Purpose: Draws the control
	 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
	 * Returns: void
	 */
	virtual void onDraw(TObject* obj = nullptr) override;
	/**
	 * DEPRECATED
	 */
	virtual UCHAR* GetAnaGameType()override;

	/**
	 * Method: TDataBind::setData
	 * Purpose: Sets the data of the Control so that when it is time to draw, the control has content to produce
	 * Parameters: TDataArrayBase* data - the array of the data to depict when drawing
	 * Returns: void
	 */
	void setData(TDataArrayBase* data);

	/**
	 * Method: TDataBind::setData
	 * Purpose: Sets the data of the Control so that when it is time to draw, the control has content to produce
	 * Parameters: TArrayBase* data - the array of the data to depict when drawing
	 * Returns: void
	 */
	void setData(TArrayBase* data);
	/**
	 * Method: TDataBind::onCreate
	 * Purose: the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: RECT contain - the area it can use
	 * Returns: bool - success
	 */
	bool onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d) override;

	/**
	 * Method: TDataBind::OnLButtonDown
	 * Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons) override;
	/**
	 * Method: TDataBind::OnLButtonUp
	 * Purpose: Allows control to catch the Left Button Up event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;
	/**
	 * Method: TDataBind::OnMouseMove 
	 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	 * Returns: void
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls) override;

	/**
	 * Method: TDataBind::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: RECT r - the new location for the control
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r)override;

	/**
	 * Method: TDataBind::getLocation
	 * Purpose: Reports how much space this object will actually need to draw
	 * Parameters: void
	 * Returns: D2D1_RECT_F -  the Rectangle of the content that would be drawn (even if it was officially allocated less space)
	 */
	D2D1_RECT_F getLocation()override;
protected:
	/**
	 * Pointer to a TDataArray (note, this should eventually be replace by a holder object to the array)
	 */
	TDataArrayBase* dataRaw;
	/**
	 * Pointer to a TArray (note, this should eventually be replace by a holder object to the array)
	 */
	TArrayBase* dataWrap;
	/**
	 * Whether to move vertically or horizontally
	 */
	bool isStack;
	/**
	 * How much drawing space to allocate for each element
	 */
	UINT widthHeight;
	/**
	 * Used to track the mouse and determine a true click
	 */
	TPoint mouseMovePoint;
	/**
	 * Currently not used
	 */
	bool isScrolling;
};

