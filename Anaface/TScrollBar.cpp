#include "TScrollBar.h"
#include "TControl.h"

#define BOX_SIZE 20
D2D1_COLOR_F end_box, body_box, middle_box_click, middle_box;
int* pointer = nullptr;
int value = 255;

/**
 * Method: TScrollBar::TScrollBar
 * Purpose: Constructor
 * Parameters: TControl& control - The control holding this scroll bar
 *				ScrollOrient so - whether this is a horizontal or vertical scroll bar 
 * Returns: new Scroll Bar
 */
TScrollBar::TScrollBar(TControl& control, ScrollOrient so)
{
	scrollAlignment = so;
	parent = &control;


	if (!pointer)
		EstablishScrollColors();
}

/**
 * Method: TScrollBar::~TScrollBar
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TScrollBar::~TScrollBar()
{
}

/**
 * Method: TScrollBar::onDraw
 * Purpose: Draws the scroll bar
 * Parameters: ID2D1RenderTarget* target -  the target to draw with
 * Returns: void
 */
void TScrollBar::onDraw(ID2D1RenderTarget* target)
{
	if (!target || !parent || !(widthFactor < 1.0f)) return;

	D2D1_RECT_F location = parent->getLocation();

	ID2D1SolidColorBrush* brush = nullptr;
	HRESULT res = target->CreateSolidColorBrush(body_box, &brush);

	if (FAILED(res)) return;

	D2D1_RECT_F d_box = body_rect;
	target->FillRectangle(d_box, brush);

	brush->SetColor(end_box);
	if (scrollAlignment == ScrollOrient::so_horizontal)
	{
		d_box.right = d_box.left + BOX_SIZE;

		target->FillRectangle(d_box, brush);

		float move = (body_rect.right - body_rect.left) - BOX_SIZE;
		d_box.right += move;
		d_box.left += move;


	}
	else
	{
		d_box.bottom = d_box.top + BOX_SIZE;

		target->FillRectangle(d_box, brush);

		float move = (body_rect.bottom - body_rect.top) - BOX_SIZE;
		d_box.bottom += move;
	}
	target->FillRectangle(d_box, brush);

	if (onFocus)
		brush->SetColor(middle_box_click);
	else
		brush->SetColor(middle_box);

	target->FillRectangle(scroll_rect, brush);
	brush->Release();
}

/**
 * Method: TScrollBar::OnLButtonDown
 * Purpose: Allows bars to assume focus
 * Parameters:UINT nFlags - redundant
 *				TPoint point - The point where the user clicked
 *				messageOutput* mOut - redundant
 * Returns: whether the focus is on this control or not
 */
bool TScrollBar::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (isContained(&point, &body_rect))
	{
		onFocus = true;
		*mOut = messageOutput::positiveScroll;
		prevPoint = point;
	}
	else
		onFocus = false;

	return onFocus;
}

/**
 * Method: TScrollBar::OnLButtonUp
 * Purpose: Lets the scroll bar know that the user has unclicked and thus can no longer have any focus
 * Parameters: UINT nFlags - redundant
 *				TPoint point - redundant
 *				messageOutput* mOut - redundant
 * Returns: void
 */
void TScrollBar::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	onFocus = false;
}

/**
 * Method: TScrollBar::OnMouseMove
 * Purpose: Allows the scroll bar to shift the contents along with the user (if focused upon)
 * Parameters: UINT nFlags - redundant
 *				TPoint point - current point the mouse is at
 *				messageOutput* mOut - redundant
 * Returns: void
 */
void TScrollBar::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (!onFocus) return;

	if (scrollAlignment == ScrollOrient::so_horizontal)
	{
		if (point.x > (body_rect.right - BOX_SIZE))
			point.x = body_rect.right - BOX_SIZE;

		if (point.x < (body_rect.left + BOX_SIZE))
			point.x = body_rect.left + BOX_SIZE;

		float move = -MovedContent(point.x - prevPoint.x);
		parent->onScroll(move / widthFactor, 0);
	}
	else
	{
		if (point.y > (body_rect.bottom - BOX_SIZE))
			point.y = body_rect.bottom - BOX_SIZE;

		if (point.y < (body_rect.top + BOX_SIZE))
			point.y = body_rect.top + BOX_SIZE;

		float move = -MovedContent(point.y - prevPoint.y);
		parent->onScroll(0, move / widthFactor);
	}
	prevPoint = point;
}

/**
 * Method: TScrollBar::MovedContent
 * Purpose: Calculates how much to move the content by
 * Parameters: float degree - how much the user moved the mouse
 * Returns: float - how much the content itself has to move
 */
float TScrollBar::MovedContent(float degree)
{
	if (degree > 0.0f)
	{
		if (degree > diff2)
		{
			degree -= (degree - diff2);
		}
		diff2 -= degree;
		diff1 += degree;
	}
	else if (degree < 0.0f)
	{
		if (-degree > diff1)
			degree += (degree + diff1);
		diff2 -= degree;
		diff1 += degree;
	}

	return degree;
}

/**
 * Method: TScrollBar::Refresh
 * Purpose: Refreshes the scroll bars so that they are callibrated correctly
 * Parameters: const D2D1_RECT_F& location - the location of the area that is allowed to be drawn
 *				 const D2D1_RECT_F& area - the location of the region that the control[s] underneath actually take up
 * Returns: void
 */
void TScrollBar::Refresh(const D2D1_RECT_F& location, const D2D1_RECT_F& area)
{
	body_rect = location;

	if (scrollAlignment == ScrollOrient::so_horizontal)
	{
		body_rect.top = body_rect.bottom - BOX_SIZE;

		diff1 = location.left - area.left;
		diff2 = area.right - location.right;

		float barSpace = body_rect.right - body_rect.left - (2 * BOX_SIZE);
		float areaSpace = area.right - area.left;
		float locationSpace = location.right - location.left;
		widthFactor = (locationSpace / areaSpace);

		scroll_rect.bottom = body_rect.bottom;
		scroll_rect.top = body_rect.top;

		scroll_rect.left = body_rect.left + BOX_SIZE + diff1 * widthFactor;
		scroll_rect.right = body_rect.right - BOX_SIZE - (diff2 * widthFactor);

		if (scroll_rect.left < (body_rect.left + BOX_SIZE))
		{
			float diff = (body_rect.left + BOX_SIZE) - scroll_rect.left;
			scroll_rect.left += diff;
			scroll_rect.right += diff;

			parent->onScroll(diff / widthFactor, 0);
		}
		else if (scroll_rect.right > (body_rect.right - BOX_SIZE))
		{
			float diff = (body_rect.right - BOX_SIZE) - scroll_rect.right;
			scroll_rect.left += diff;
			scroll_rect.right += diff;

			parent->onScroll(diff / widthFactor, 0);
		}

	}
	else
	{
		body_rect.left = body_rect.right - BOX_SIZE;

		diff1 = location.top - area.top;
		diff2 = area.bottom - location.bottom;

		float barSpace = body_rect.bottom - body_rect.top - (2 * BOX_SIZE);
		float areaSpace = area.bottom - area.top;
		float locationSpace = location.bottom - location.top;
		widthFactor = (locationSpace / areaSpace);

		scroll_rect.left = body_rect.left;
		scroll_rect.right = body_rect.right;

		scroll_rect.top = body_rect.top + BOX_SIZE + diff1 * widthFactor;
		scroll_rect.bottom = body_rect.bottom - BOX_SIZE - (diff2 * widthFactor);



		if (scroll_rect.top < (body_rect.top + BOX_SIZE))
		{
			float diff = (body_rect.top + BOX_SIZE) - scroll_rect.top;
			scroll_rect.top += diff;
			scroll_rect.bottom += diff;

			parent->onScroll(0, -(diff / widthFactor));
		}
		else if (scroll_rect.bottom > (body_rect.bottom - BOX_SIZE))
		{
			float diff = (body_rect.bottom - BOX_SIZE) - scroll_rect.bottom;
			scroll_rect.top += diff;
			scroll_rect.bottom += diff;

			parent->onScroll(0, -(diff / widthFactor));
		}
	}
}

/**
 * Method: TScrollBar::EstablishScrollColors
 * Purpose: Establishes the colors used in the scroll bars
 * Parameters: void
 * Returns: void
 *
 * Note: Called once by the constructor
 */
void TScrollBar::EstablishScrollColors()
{
	body_box = D2D1::ColorF(0x00000011);
	middle_box = D2D1::ColorF(0x00000033);
	middle_box_click = D2D1::ColorF(0x00000099);
	end_box = D2D1::ColorF(0x00000066);
	pointer = &value;
}

/**
 * Function: GetScrollbarBoxSize
 * Purpose: Reports the with/height of the vertical/horzontal scroll bar
 * Parameters: void
 * Returns: UINT - the size used in the creation of the box
 */
UINT GetScrollbarBoxSize()
{
	return BOX_SIZE;
}
