#include "TPage.h"

#define BOX_SIZE 20
D2D1_COLOR_F end_box, body_box, middle_box_click, middle_box;
int* pointer = nullptr;
int value = 255;


/**
 * Function: IsContained
 * Purpose: Checks of a point is within a given Direct2D Rectangle
 * Parameters: const TPoint* - the point to check 
 *				const D2D1_RECT_F* - the rectangle to check
 * Returns: bool - whether the point is withing the bounds
 */
bool IsContained(const TPoint& cp, const D2D1_RECT_F& r)
{
	return cp.x >= r.left && cp.x <= r.right &&
		cp.y >= r.top && cp.y <= r.bottom;
}

/**
 * Method: TScrollBar::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TPage::TScrollBar::GetType()
{
    return TString(L"TScrollBar;") + TObject::GetType();
}

/**
 * Method: TScrollBar::TScrollBar
 * Purpose: Constructor
 * Parameters: TControl& control - The control holding this scroll bar
 *				ScrollOrient so - whether this is a horizontal or vertical scroll bar
 * Returns: new Scroll Bar
 */
TPage::TScrollBar::TScrollBar(TrecPointer<TPage> control, ScrollOrient so)
{
    this->parent = TrecPointerKey::GetSoftPointerFromTrec<>(control);
    this->scrollAlignment = so;


    if (!pointer)
        EstablishScrollColors();
}

/**
 * Method: TScrollBar::~TScrollBar
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TPage::TScrollBar::~TScrollBar(){}

/**
 * Method: TScrollBar::onDraw
 * Purpose: Draws the scroll bar
 * Parameters: ID2D1RenderTarget* target -  the target to draw with
 * Returns: void
 */
void TPage::TScrollBar::onDraw(ID2D1RenderTarget* target)
{
	if (!target || !parent.Get() || !(widthFactor < 1.0f)) return;

	D2D1_RECT_F location = parent.Get()->GetArea();

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
 *				message_output* mOut - redundant
 * Returns: whether the focus is on this control or not
 *
 * Attributes: virtual
 */
bool TPage::TScrollBar::OnLButtonDown(UINT nFlags, TPoint point, message_output& mOut)
{
	if (IsContained(&point, body_rect))
	{
		onFocus = true;
		mOut = message_output::mo_positive_scroll;
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
 *				message_output* mOut - redundant
 * Returns: void
 *
 * Attributes: virtual
 */
void TPage::TScrollBar::OnLButtonUp(UINT nFlags, TPoint point, message_output& mOut)
{
	onFocus = false;
}

/**
 * Method: TScrollBar::OnMouseMove
 * Purpose: Allows the scroll bar to shift the contents along with the user (if focused upon)
 * Parameters: UINT nFlags - redundant
 *				TPoint point - current point the mouse is at
 *				message_output* mOut - redundant
 * Returns: void
 *
 * Attributes: virtual
 */
void TPage::TScrollBar::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>& args)
{
	if (!onFocus) return;

	if (scrollAlignment == ScrollOrient::so_horizontal)
	{
		if (point.x > (body_rect.right - BOX_SIZE))
			point.x = body_rect.right - BOX_SIZE;

		if (point.x < (body_rect.left + BOX_SIZE))
			point.x = body_rect.left + BOX_SIZE;

		float move = -MovedContent(point.x - prevPoint.x);
		parent.Get()->OnScroll(point, TPoint(move / widthFactor, 0), args);
	}
	else
	{
		if (point.y > (body_rect.bottom - BOX_SIZE))
			point.y = body_rect.bottom - BOX_SIZE;

		if (point.y < (body_rect.top + BOX_SIZE))
			point.y = body_rect.top + BOX_SIZE;

		float move = -MovedContent(point.y - prevPoint.y);
		parent.Get()->OnScroll(point, TPoint(0, move / widthFactor),args);
	}
	prevPoint = point;
}

/**
 * Method: TScrollBar::MovedContent
 * Purpose: Calculates how much to move the content by
 * Parameters: float degree - how much the user moved the mouse
 * Returns: float - how much the content itself has to move
 */
float TPage::TScrollBar::MovedContent(float degree)
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
void TPage::TScrollBar::Refresh(const D2D1_RECT_F& location, const D2D1_RECT_F& area)
{
	body_rect = location;
	TDataArray<EventArgs> args;

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

			parent.Get()->OnScroll(TPoint(-1,-1), TPoint(diff / widthFactor, 0),args);
		}
		else if (scroll_rect.right > (body_rect.right - BOX_SIZE))
		{
			float diff = (body_rect.right - BOX_SIZE) - scroll_rect.right;
			scroll_rect.left += diff;
			scroll_rect.right += diff;

			parent.Get()->OnScroll(TPoint(-1,-1), TPoint(diff / widthFactor, 0),args);
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

			parent.Get()->OnScroll(TPoint(-1,-1), TPoint(0, -(diff / widthFactor)),args);
		}
		else if (scroll_rect.bottom > (body_rect.bottom - BOX_SIZE))
		{
			float diff = (body_rect.bottom - BOX_SIZE) - scroll_rect.bottom;
			scroll_rect.top += diff;
			scroll_rect.bottom += diff;

			parent.Get()->OnScroll(TPoint(-1,-1), TPoint(0, -(diff / widthFactor)),args);
		}
	}
}

TPage::EventID_Cred::EventID_Cred()
{
	this->eventType = R_Message_Type::On_Hold_Click;
}

TPage::EventID_Cred::EventID_Cred(const EventID_Cred& copy)
{
	this->control = copy.control;
	this->eventType = copy.eventType;
	this->scroll = copy.scroll;
	this->textIntercepter = copy.textIntercepter;
}

TPage::EventID_Cred::EventID_Cred(R_Message_Type t, TrecPointer<TPage> c)
{
	this->eventType = t;
	this->control = c;
}

TPage::EventID_Cred::EventID_Cred(R_Message_Type t, TrecPointer<TPage> c, TrecPointer<TPage::TScrollBar> sb)
{
	this->eventType = t;
	this->control = c;
	this->scroll = sb;
}

TPage::EventID_Cred::EventID_Cred(R_Message_Type t, TrecPointer<TPage> c, TrecPointer<TTextIntercepter> i)
{
	this->eventType = t;
	this->control = c; 
	this->textIntercepter = i;
}

void TPage::SetSelf(TrecPointer<TPage> self)
{
	if (this != self.Get())
		throw L"Not Self Reference";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<>(self);
}

TrecPointer<TPage> TPage::GetParentPage()
{
	if (parent.Get())
		return TrecPointerKey::GetTrecPointerFromSoft<>(parent);
    return TrecPointer<TPage>();
}

void TPage::SetParentPage(TrecPointer<TPage> parent)
{
	this->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parent);
}

D2D1_RECT_F TPage::GetArea()
{
	return area;
}

void TPage::RotateDegrees(float degrees)
{
}

void TPage::RotateRadians(float radians)
{
}

void TPage::InjectScrollerPage(const D2D1_RECT_F& bounds, const D2D1_RECT_F& needs, TrecPointer<TPage> page)
{
	// If no children, do nothing
}

TPage::TPage(TrecPointer<DrawingBoard> board)
{
	this->drawingBoard = board;
}


/**
 * Method: TScrollBar::EstablishScrollColors
 * Purpose: Establishes the colors used in the scroll bars
 * Parameters: void
 * Returns: void
 *
 * Note: Called once by the constructor
 */
void TPage::TScrollBar::EstablishScrollColors()
{
	body_box = D2D1::ColorF(0x00000011);
	middle_box = D2D1::ColorF(0x00000033);
	middle_box_click = D2D1::ColorF(0x00000099);
	end_box = D2D1::ColorF(0x00000066);
	pointer = &value;
}

EventArgs::EventArgs()
{
	Reset();
}
void EventArgs::Reset()
{
	isClick = false;
	isLeftClick = false;
	methodID = -1;
	point.x = 0.0f;
	point.y = 0.0f;
	positive = false;
	text.Empty();
	type = L'\0';
	object.Nullify();
	oldSize = newSize = { 0,0,0,0 };
}
EventArgs::EventArgs(const EventArgs& args)
{
	text.Set(args.text);
	positive = args.positive;
	point = args.point;
	isClick = args.isClick;
	isLeftClick = args.isLeftClick;
	eventType = args.eventType;
	methodID = args.methodID;
	arrayLabel = args.arrayLabel;
	type = args.type;
	object = args.object;
	oldSize = args.oldSize;
	newSize = args.newSize;
}

Dimensions::Dimensions()
{
	this->height = this->maxHeight = this->minHeight =
		this->width = this->maxWidth = this->minWidth = 0;
}


D2D1_RECT_F ConvertStringToD2D1Rect(const TString& str)
{
	D2D1_RECT_F returnable = D2D1_RECT_F{ 0,0,0,0 };

	TrecPointer<TDataArray<TString>> strSpl = str.split(",");


	int res[4] = { 0,0,0,0 };
	if (strSpl.Get() == NULL || strSpl->Size() != 4)
	{
		return returnable;
	}
	res[0] = strSpl->at(0).ConvertToFloat(returnable.top);
	res[1] = strSpl->at(1).ConvertToFloat(returnable.left);
	res[2] = strSpl->at(2).ConvertToFloat(returnable.bottom);
	res[3] = strSpl->at(3).ConvertToFloat(returnable.right);
	if (res[0] > 0 || res[1] > 0 || res[2] > 0 || res[3] > 0)
		ZeroMemory(&returnable, sizeof(returnable));
	return returnable; LONG i;
}

