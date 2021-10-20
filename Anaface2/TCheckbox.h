#pragma once
#include "TGadget.h"
class TCheckbox :
    public TGadget
{
public:

	/**
	 * Method: TCheckBox::TCheckBox
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - the render target to draw to
	 *				TrecPointer<TArray<styleTable>> ta - the style list for Anaface
	 * Returns: void
	 */
	TCheckbox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);
	/**
	 * Method: TCheckBox::~TCheckBox
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TCheckbox();

	/**
	 * Method: TControl::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: const D2D1_RECT_F& loc- the location this control is bound by
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: virtual
	 */
	virtual bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d);


	/**
	 * Method: TControl::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;

	/*
	 * Method: TCheckBox::IsChecked
	 * Purpose: Reports whether the Check-box is in a clicked state
	 * Parameters: void
	 * Returns: bool - whether the check-box has been "selected" or not
	 */
	bool IsChecked();

	/**
	 * Method: TControl::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

	/**
	 * Method: TControl::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&) override;

protected:

	bool isChecked;
};

