#pragma once
#include "TControl.h"

/**
 * Class: ChildControl
 * Purpose: Holds a Control
 */
class _ANAFACE_DLL2 ChildControl
{
public:
	ChildControl();
	ChildControl(const ChildControl& copy);
    TrecPointer<TPage> control;
    UINT row, col;
};

/**
 * Class: TRandomLayout
 * Purpose: Extends Anaface 2's TControls to support Child Controls
 */
class _ANAFACE_DLL2 TRandomLayout :
    public TControl
{
public:

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
	 * Method: TRandomLayout::TRandomLayout
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TRandomLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);

	virtual ~TRandomLayout();

	/**
	 * Method: TControl::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;


	/**
	 * Method: TControl::OnRButtonUp
	 * Purpose: Responds to the Right Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


	/**
	 * Method: TControl::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


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
	 * Method: TControl::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

	/**
	 * Method: TControl::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&) override;

	/**
	 * Method: TControl::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>& args) override;

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

	/**
	 * Method: TControl::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&) override;

	/**
	 * Method: TRandomLayout::AddPage
	 * Purpose: Adds a Page to the list of Child Controls
	 * Parameters: TrecPointer<TPage> page - the page to add
	 *			UINT row - the row value
	 *			UINT col - the col value
	 *			bool doOverride - if a conflict is detected, replace the current page with the new one
	 * Returns: bool - whether the operation was added
	 * 
	 * Attributes: virtual
	 * 
	 * Note: the TRandomLayout class only looks at the first parameter; However, subclasses that organize their children will use the other parameters
	 */
	virtual bool AddPage(TrecPointer<TPage> page, UINT row, UINT col, bool doOverride = false);

	/**
	 * Method: TRandomLayout::RemovePage
	 * Purpose: Removes a Page by Page reference
	 * Parameters: TrecPointer<TPage> page - the page to remove
	 * Returns: bool - whether the page was removed
	 */
	bool RemovePage(TrecPointer<TPage> page);

	/**
	 * Method: TRandomLayout::RemovePage
	 * Purpose: Removes a page by index/coordinate
	 * Parameters: UINT row - the row value (or index in the case of a base TRandomLayout)
	 *				UINT col - the the column value (ignored by the base-TRandomLayout)
	 * Returns: bool - whether the page was removed
	 * 
	 * Attributes: virtual
	 */
	virtual bool RemovePage(UINT row, UINT col);

	/**
	 * Method: TRandomLayout::GetControlCapacity
	 * Purpose: Gets Capacity of the Control
	 * Parameters: UINT& row - holds the row (or index attribute)
	 *				UINT& col - holds the col level
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual void GetControlCapacity(UINT& row, UINT& col);

	/**
	 * Method: TRandomLayout::GetPage
	 * Purpose: Gets the Control at the specified coordinates (or index for the TRandomLayout)
	 * Parameters: UINT row
	 *				UINT col
	 * Returns: TrecPointer<TPage> - the page held t the location
	 * 
	 * Attributes: virtual
	 */
	virtual TrecPointer<TPage> GetPage(UINT row, UINT col);

protected:

    /**
     * Child Controls
     */
    TDataArray<ChildControl> childControls;
};

