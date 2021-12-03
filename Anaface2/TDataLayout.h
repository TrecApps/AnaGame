#pragma once
#include "TRandomLayout.h"

typedef enum class data_org
{
	do_default,			// Go by Scrolling to handle overfill (allows for 2D Arrays)
	do_limit_by_space,	// Use height/width but only allow certain elements per row/col before pushing onto nex row/col
	do_limit_by_item_count	// Limit by number of items per row/col
};

class _ANAFACE_DLL2 TDataLayout :
    public TRandomLayout
{
public:

	/**
	 * Method: TDataLayout::TDataLayout
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TDataLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);

	/**
	 * Method: TDataLayout::SetData
	 * Purpose: Sets the Data for this Control Binding
	 * Parameters: TrecPointer<TVariable> var - the Variable to bind to
	 * Returns: void
	 */
	void SetData(TrecPointer<TVariable> var);

	/**
	 * Method: TDataLayout::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;

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
	 * Method: TPage::GetArea
	 * Purpose: Retrieves the Area held by the Page
	 * Parameters: void
	 * Returns: D2D1_RECT_F -  the Region the Page claims it is drawing to
	 *
	 * Attributes: virtual
	 */
	virtual D2D1_RECT_F GetArea() override;

	/**
 * Method: TPage::OnRButtonUp
 * Purpose: Responds to the Right Button Up Message
 * Parameters: UINT nFlags - flags associated with the message
 *				const TPoint& point - the point included in the message
 *				message_output& mOut -  the result of the message
 *				TDataArray<EventID_Cred>& - list of events to be handled
 * Returns: void
 *
 * Attributes: message; abstract
 */
	ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: TPage::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: TPage::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: TPage::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TPage::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TPage::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& args) override;

	/**
	 * Method: TPage::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;

protected:

	bool GetIndex(const TPoint& point, int& row, int& col);

	D2D1_RECT_F GetAreaDefault();
	D2D1_RECT_F GetAreaByContainer();
	D2D1_RECT_F GetAreaByCount();

	int ConvertCoordinates(int row, int col);

	/**
	 * Width and Height of the control within
	 */
	UINT width, height;


	/**
	 * Max Primary row
	 */
	int maxPrimaryCount;

	/**
	 * How to handle overflow on the primary direction
	 */
	data_org org;

	/**
	 * the Variable to bind to
	 */
	TrecPointer<TVariable> var;

	/**
	 * Whether the vertical direction is the primary one
	 */
	bool stackFirst;
};

