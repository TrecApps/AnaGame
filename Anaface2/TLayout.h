#pragma once
#include "TRandomLayout.h"

class _ANAFACE_DLL2 LayoutSpace
{
public:
	LayoutSpace();
	LayoutSpace(const LayoutSpace& copy);

	bool isFlex;
	UINT space, actualSpace;
};

typedef enum class overflow_layout_approach
{
	ola_contain, // The layout will simply place a scroller between itself and the child control requesting more space (Default)
	ola_vertical // The layout will expand Vertically upon encountering a child control that needs more space. 
};

/**
 * Class: TLayout
 * Purpose: Adds one dimension of Structure to the TRandomLayout
 */
class _ANAFACE_DLL2 TLayout : public TRandomLayout
{
	friend class AnafacePage;
public:

	/**
	 * Method: TLayout::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: const D2D1_RECT_F& loc- the location this control is bound by
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: virtual
	 */
	virtual bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d, TrecPointer<TFileShell> d)override;

	/**
	 * Method: TLayout::TRandomLayout
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);

	virtual ~TLayout();


	/**
	 * Method: TLayout::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;

	/**
	 * Method: TLayout::OnResize
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
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: TLayout::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TLayout::AddPage
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
	 * Method: TLayout::RemovePage
	 * Purpose: Removes a page by index/coordinate
	 * Parameters: UINT row - the row value (or index in the case of a base TRandomLayout)
	 *				UINT col - the the column value (ignored by the base-TRandomLayout)
	 * Returns: bool - whether the page was removed
	 *
	 * Attributes: virtual
	 */
	virtual bool RemovePage(UINT row, UINT col)override;

	/**
	 * Method: TLayout::GetControlCapacity
	 * Purpose: Gets Capacity of the Control
	 * Parameters: UINT& row - holds the row (or index attribute)
	 *				UINT& col - holds the col level
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void GetControlCapacity(UINT& row, UINT& col) override;

	/**
	 * Method: TLayout::GetPage
	 * Purpose: Gets the Control at the specified coordinates (or index for the TRandomLayout)
	 * Parameters: UINT row
	 *				UINT col
	 * Returns: TrecPointer<TPage> - the page held t the location
	 *
	 * Attributes: virtual
	 */
	virtual TrecPointer<TPage> GetPage(UINT row, UINT col) override;

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


	/**
	 * Method: TLayout::InjectScrollerPage
	 * Purpose: Inserts a Scrolling Page between the parent page and the calling Page
	 * Parameters: const D2D1_RECT_F& bounds - the bounds of the calling page
	 *				TrecPointer<TPage> page - the calling page
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void InjectScrollerPage(const D2D1_RECT_F& bounds, const D2D1_RECT_F& needs, TrecPointer<TPage> page)override;

protected:

	overflow_layout_approach overflowResponse;

	/**
	 * Method: TLayout::ParseDimensions
	 * Purpose: Goes through Dimension Arrays and Updates the actual value on flexible children
	 * Parameters: TDataArray<LayoutSpace>& dimension - the dimension array to go through
	 *				UINT totalSpace - the total space available for that dimension
	 * Returns: bool needsScrolling - whether a scrollbar is needed to deal with overflow
	 */
	bool ParseDimensions(TDataArray<LayoutSpace>& dimension, UINT totalSpace);

	/**
	 * Spacing Information of the Dimension
	 */
	TDataArray<LayoutSpace> primDem;

	/**
	 * The Dimension direction to aim the Primary dimension of layout (vertical if true)
	 */
	bool primaryStack;
};

