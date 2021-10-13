#pragma once
#include "TLayout.h"

/**
 * Class: TGrid
 * Purpose: Adds a second dimension for layout to the TLayout control
 */
class TGrid :
    public TLayout
{
public:

	/**
	 * Method: TGrid::onCreate
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
	 * Method: TGrid::TRandomLayout
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TGrid(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);

	virtual ~TGrid();

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
	virtual bool AddPage(TrecPointer<TPage> page, UINT row, UINT col, bool doOverride = false)override;


	/**
	 * Method: TRandomLayout::RemovePage
	 * Purpose: Removes a page by index/coordinate
	 * Parameters: UINT row - the row value (or index in the case of a base TRandomLayout)
	 *				UINT col - the the column value (ignored by the base-TRandomLayout)
	 * Returns: bool - whether the page was removed
	 *
	 * Attributes: virtual
	 */
	virtual bool RemovePage(UINT row, UINT col)override;

	/**
	 * Method: TRandomLayout::GetControlCapacity
	 * Purpose: Gets Capacity of the Control
	 * Parameters: UINT& row - holds the row (or index attribute)
	 *				UINT& col - holds the col level
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void GetControlCapacity(UINT& row, UINT& col) override;


	/**
	 * Method: TLayout::AddRow
	 * Purpose: Adds a New Row to the Layout
	 * Parameters: UINT space - the default Space
	 *				bool isFlex - whether this row can bend according to space
	 * Returns: int - the row space added (in regular TLayout, can be -1 if 'primaryStack' flag is false)
	 *
	 * Attributes: virtual
	 */
	virtual int AddRow(UINT space, bool isFlex);


	/**
	 * Method: TLayout::AddCol
	 * Purpose: Adds a New Column to the Layout
	 * Parameters: UINT space - the default Space
	 *				bool isFlex - whether this row can bend according to space
	 * Returns: int - the column space added (in regular TLayout, can be -1 if 'primaryStack' flag is true)
	 *
	 * Attributes: virtual
	 */
	virtual int AddCol(UINT space, bool isFlex);
protected:

	void RefreshChildControls(bool newRow);

	/**
	 * Spacing Information of the second Dimension
	 */
	TDataArray<LayoutSpace> secDem;
};

