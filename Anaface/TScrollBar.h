#pragma once
#include <d2d1.h>
#include <TObject.h>
#include "Anaface.h"
#include <TPoint.h>

class TControl;

/**
 * Enum Class: messageOScrollOrientutput
 * Purpose: Tracks the orientation of the scroll bar
 */
typedef enum class ScrollOrient
{
	so_vertical,
	so_horizontal
}ScrollOrient;

/**
 * Enum Class: messageOutput
 * Purpose: Allows individual Controls to communicate with the message engine that called it
 */
typedef enum class messageOutput
{
	negative,
	negativeUpdate,
	positiveOverride,
	positiveContinue,
	positiveOverrideUpdate,
	positiveContinueUpdate,
	positiveScroll
}messageOutput;

class _ANAFACE_DLL TScrollBar : public TObject
{
public:
	/**
	 * Method: TScrollBar::TScrollBar
	 * Purpose: Constructor
	 * Parameters: TControl& control - The control holding this scroll bar
	 *				ScrollOrient so - whether this is a horizontal or vertical scroll bar
	 * Returns: new Scroll Bar
	 */
	TScrollBar(TControl& control, ScrollOrient so);
	/**
	 * Method: TScrollBar::~TScrollBar
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TScrollBar();

	/**
	 * Method: TScrollBar::onDraw
	 * Purpose: Draws the scroll bar
	 * Parameters: ID2D1RenderTarget* target -  the target to draw with
	 * Returns: void
	 */
	void onDraw(ID2D1RenderTarget* target);

	/**
	 * Method: TScrollBar::OnLButtonDown
	 * Purpose: Allows bars to assume focus
	 * Parameters:UINT nFlags - redundant
	 *				TPoint point - The point where the user clicked
	 *				messageOutput* mOut - redundant
	 * Returns: whether the focus is on this control or not
	 */
	virtual bool OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	/**
	 * Method: TScrollBar::OnLButtonUp
	 * Purpose: Lets the scroll bar know that the user has unclicked and thus can no longer have any focus
	 * Parameters: UINT nFlags - redundant
	 *				TPoint point - redundant
	 *				messageOutput* mOut - redundant
	 * Returns: void
	 */
	virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	/**
	 * Method: TScrollBar::OnMouseMove
	 * Purpose: Allows the scroll bar to shift the contents along with the user (if focused upon)
	 * Parameters: UINT nFlags - redundant
	 *				TPoint point - current point the mouse is at
	 *				messageOutput* mOut - redundant
	 * Returns: void
	 */
	virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);

	/**
	 * Method: TScrollBar::MovedContent
	 * Purpose: Calculates how much to move the content by
	 * Parameters: float degree - how much the user moved the mouse
	 * Returns: float - how much the content itself has to move
	 */
	float MovedContent(float degree);

	/**
	 * Method: TScrollBar::Refresh
	 * Purpose: Refreshes the scroll bars so that they are callibrated correctly
	 * Parameters: const D2D1_RECT_F& location - the location of the area that is allowed to be drawn
	 *				 const D2D1_RECT_F& area - the location of the region that the control[s] underneath actually take up
	 * Returns: void
	 */
	void Refresh(const D2D1_RECT_F& location, const D2D1_RECT_F& area);

private: // Static attributes, styles that should be shared across all Scroll Bars

	/**
	 * Method: TScrollBar::EstablishScrollColors
	 * Purpose: Establishes the colors used in the scroll bars
	 * Parameters: void
	 * Returns: void
	 *
	 * Note: Called once by the constructor
	 */
	static void EstablishScrollColors();

	/**
	 * Two imporant rectangles to draw
	 */
	D2D1_RECT_F body_rect, scroll_rect;

	/**
	 * Parent holding this Scroll Bar
	 */
	TControl* parent;
	/**
	 * Used to track movement of the mouse to determine how much to shift by
	 */
	TPoint prevPoint;
	/**
	 * Whether the bar is being focused on (drawable currently not used)
	 */
	bool onFocus, drawable;
	/**
	 * Distance between the regions and the ratio provided
	 */
	float diff1, diff2, widthFactor;
	/**
	 * Vertical or horizontal
	 */
	ScrollOrient scrollAlignment;
};

/**
 * Function: GetScrollbarBoxSize
 * Purpose: Reports the with/height of the vertical/horzontal scroll bar
 * Parameters: void
 * Returns: UINT - the size used in the creation of the box
 */
UINT GetScrollbarBoxSize();