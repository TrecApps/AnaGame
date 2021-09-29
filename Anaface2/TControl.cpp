
#include "TControl.h"



/**
 * Method: TControlComponent::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TControlComponent::GetType()
{
	return TString(L"TControlComponent;") + TObject::GetType();
}

/**
 * Method:  TControlComponent::TControlComponent
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render target to draw against
 *				TControl* tc - the TControl to which the TControlComponent is a party to
 * Returns: New TControlComponent Object
 */
TControlComponent::TControlComponent(TrecPointer<DrawingBoard>rtp, TrecPointer<TPage> parent)
{
	drawingBoard = rtp;
	this->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parent);

	thickness = 1.0;
	isContent = false;

	if (drawingBoard.Get())
	{
		BuilderFocusBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Red));
	}
	else
		BuilderFocusBrush.Nullify();
}

/**
 * Method: TControlComponent:TControlComponent
 * Purpose: Constructor
 * Parameters: TrecPointer<TControlComponent> & rBord - Reference to the Border to copy
 *				TControl* tc_holder - the Control to work for
 * Returns: New TControlComponent Object
 */
TControlComponent::TControlComponent(TrecPointer<TControlComponent>& rBord, TrecPointer<TPage> parent)
{
	if (!rBord.Get())return;

	thickness = rBord->thickness;
	style = rBord->style;
	stopCollection = rBord->stopCollection;
	drawingBoard = rBord->drawingBoard;

	this->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parent);
	shape = rBord->shape;

	useRadial = rBord->useRadial;

	this->isContent = rBord->isContent;

	// Alternate shapes
	loci = rBord->loci;
	circle = rBord->circle;
	roundedRect = rBord->roundedRect;
}

/**
 * Method: TControlComponent::~TControlComponent
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TControlComponent::~TControlComponent()
{
	brush.Delete();

	BuilderFocusBrush.Delete();
	image.Delete();
}

/**
 * Method: TControlComponent::onCreate
 * Purpose: Sets up a basic Rectangular border
 * Parameters: RECT location - the location the Border should draw on
 * Returns: bool - success
 */
bool TControlComponent::onCreate(D2D1_RECT_F location)
{
	TObjectLocker threadLock(&thread);
	if (!drawingBoard.Get())
	{

		return false;
	}
	brush.Nullify();


	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stopCollRaw;
	shape = TShape::T_Rect;
	loci.bottom = location.bottom;
	loci.left = location.left;
	loci.right = location.right;
	loci.top = location.top;

	ResetBrush();


	return true;
}

/**
 * Method: TControlComponent::onCreate
 * Purpose: Sets up a border optimized for dawing ovals
 * Parameters: D2D1_ELLIPSE e - the oval to draw
 * Returns: bool - success
 */
bool TControlComponent::onCreate(D2D1_ELLIPSE e)
{
	TObjectLocker threadLock(&thread);

	shape = TShape::T_Ellipse;
	if (!drawingBoard.Get())
	{

		return false;
	}
	loci.left = e.point.x - e.radiusX;
	loci.right = e.point.x + e.radiusX;
	loci.top = e.point.y - e.radiusY;
	loci.bottom = e.point.y + e.radiusY;

	ResetBrush();
	circle = e;

	return true;
}

/**
 * Method: TControlComponent::onCreate
 * Purpose: Sets up a Border that has a rectangle with rounded corners
 * Parameters: D2D1_ROUNDED_RECT rr - the border to draw
 * Returns: bool - success
 */
bool TControlComponent::onCreate(D2D1_ROUNDED_RECT rr)
{
	TObjectLocker threadLock(&thread);
	shape = TShape::T_Rounded_Rect;
	if (!drawingBoard.Get())
	{

		return false;
	}
	roundedRect = rr;

	loci.left = rr.rect.left;
	loci.right = rr.rect.right;
	loci.top = rr.rect.top;
	loci.bottom = rr.rect.bottom;

	ResetBrush();

	return true;
}


/**
 * Method: TControlComponent::onDraw
 * Purpose: Draws the border
 * Parameters: void
 * Returns: void
 */
void TControlComponent::onDraw(D2D1_RECT_F& f_loc)
{
	TObjectLocker threadLock(&thread);
	if (drawingBoard.Get() && brush.Get() && parent.Get())
	{
		bool drawRect = true;

		if (isContent)
		{
			bool isLayered = false;
			switch (shape)
			{
			case TShape::T_Rect:
				
				break;
			case TShape::T_Rounded_Rect:
				drawingBoard->AddLayer(roundedRect);
				isLayered = true;
				break;
			case TShape::T_Ellipse:
				circle.point.x = (f_loc.right + f_loc.left) / 2;
				circle.point.y = (f_loc.bottom + f_loc.top) / 2;
				circle.radiusX = f_loc.right - f_loc.left;
				circle.radiusY = f_loc.bottom - f_loc.top;
				drawingBoard->AddLayer(circle);
				isLayered = true;
				break;
			case TShape::T_Custom_shape:
				break;
			}
			brush->FillRectangle(loci);
			if (image.Get())
				image->FillRectangle(loci);

			if (isLayered)
				drawingBoard->PopLayer();

		}
		else
		{
			switch (shape)
			{
			case TShape::T_Rect:
				brush->DrawRectangle(loci, thickness);
				break;
			case TShape::T_Rounded_Rect:
				brush->DrawRoundedRectangle(roundedRect, thickness);
				break;
			case TShape::T_Ellipse:
				circle.point.x = (f_loc.right + f_loc.left) / 2;
				circle.point.y = (f_loc.bottom + f_loc.top) / 2;
				circle.radiusX = f_loc.right - f_loc.left;
				circle.radiusY = f_loc.bottom - f_loc.top;
				brush->DrawEllipse(circle, thickness);
				break;
			case TShape::T_Custom_shape:
				break;
			}
		}
	}

}


/**
 * Method: TControlComponent::setThickness
 * Purpose: Allows the thickness to be updated after creation
 * Parameters: float f - the thickness to set to
 * Returns: void
 */
void TControlComponent::setThickness(float f)
{
	TObjectLocker threadLock(&thread);
	thickness = f;

}

/**
 * Method: TControlComponent::setOpaquency
 * Purpose: Allows the Opaquency to be updated after creation
 * Parameters: float f - the opaquency to set to (no upadte occurs if value is out of range)
 * Returns: void
 */
void TControlComponent::setOpaquency(float f)
{
	if (f > 1.0000000 || f < 0.0f)
		return;
	TObjectLocker threadLock(&thread);
	if (brush.Get())
	{

	}

}

/**
 * Method: TControlComponent::getThickness
 * Purpose: Allows the current thickness to be seen
 * Parameters: void
 * Returns: float - the current thickness
 */
float TControlComponent::getThickness()
{
	TObjectLocker threadLock(&thread);
	float ret = thickness;

	return ret;
}

/**
 * Method: TControlComponent::getLocation
 * Purpose: Retrieves the Location of the current border
 * Parameters: void
 * Returns: D2D1_RECT_F - the current location
 */
D2D1_RECT_F TControlComponent::getLocation()
{
	TObjectLocker threadLock(&thread);

	return loci;
}

/**
 * Method: TControlComponent::ShiftHorizontal
 * Purpose: Moves the Border left-or right, expected to be used by TLayouts
 * Parameters: int degrees - how much to move the Border and which direction
 * Returns: void
 */
void TControlComponent::ShiftHorizontal(int degrees)
{
	TObjectLocker threadLock(&thread);
	loci.left += degrees;
	loci.right += degrees;
}

/**
 * Method: TControlComponent::ShiftVertical
 * Purpose:Moves the Border up or down, expected to be used by TLayouts
 * Parameters: int degrees - how much to move the border and which direction
 * Returns: void
 */
void TControlComponent::ShiftVertical(int degrees)
{
	TObjectLocker threadLock(&thread);
	loci.top += degrees;
	loci.bottom += degrees;
}


TrecPointer<TBrush> TControlComponent::GetBrush()
{
	TObjectLocker threadLock(&thread);

	return brush;
}

TrecPointer<TControlComponent> TControlComponent::GetControlBorder(TrecPointer<DrawingBoard> board, TrecPointer<TPage> parent)
{
	if (!board.Get() || !parent.Get())
		return TrecPointer<TControlComponent>();
	return TrecPointerKey::GetNewTrecPointer<TControlComponent>(board, parent);
}

TrecPointer<TControlComponent> TControlComponent::GetControlContent(TrecPointer<DrawingBoard> board, TrecPointer<TPage> parent)
{
	TrecPointer<TControlComponent> ret = GetControlBorder(board, parent);
	if (ret.Get())
		ret->isContent = true;
	return ret;
}

/**
 * Method: TControlComponent::ResetBrush
 * Purpose: Allows the border to reset the brush when one of it's attributes changes
 * Parameters: void
 * Returns: void
 */
void TControlComponent::ResetBrush()
{
	TObjectLocker threadLock(&thread);
	if (!drawingBoard.Get())
	{

		return;
	}
	if (stopCollection.GetGradientCount() > 1)
	{

		if (useRadial)
		{
			brush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(loci.left, loci.top),
				D2D1::Point2F(loci.right, loci.bottom),
				loci.right - loci.left, loci.bottom - loci.top);
		}
		else
		{
			brush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(loci.left, loci.top),
				D2D1::Point2F(loci.right, loci.bottom));
		}

	}
	else if (stopCollection.GetGradientCount() == 1)
	{
		brush = drawingBoard->GetBrush(TColor(stopCollection.GetColorAt(0)));
	}
	else
	{
		brush = drawingBoard->GetBrush(TColor());
	}

}


/**
 * Method: TControl::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TControl::GetType()
{
	return TString(L"TControl;") + TPage::GetType();
}

/**
 * Method: TControl::TControl
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
 * Return: New TControl Object
 */
TControl::TControl(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles) : TPage(drawingBoard)
{
	isActive = true;
	mState = messageState::normal;
	overrideParent = true;
	arrayID = -1;
	controlTransform = D2D1::Matrix3x2F::Identity();
	shape = TShape::T_Rect;
	location = margin = bounds = { 0,0,0,0 };
}

/**
 * Method: TControl::setActive
 * Purpose: Enables controls to be either active (default) or inactive (they don't draw or respond to events)
 * Parameters: bool act - whether control should be active or not
 * Returns: void
 */
void TControl::setActive(bool act)
{
	TObjectLocker threadLock(&thread);
	isActive = act;

}

/**
 * Method: TControl::GetActive
 * Purpose: Reports whether the control is 'active' or not
 * Parameters: void
 * Returns: bool - whether the control is active or not
 */
bool TControl::GetActive()
{
	TObjectLocker threadLock(&thread);
	bool ret = isActive;

	return ret;
}

bool TControl::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
	return false;
}

/**
 * Method:  TControl::updateArrayID
 * Purpose: Stores an index to an external array, to be determined by the software using the control
 * Parameters: int aid - the array index to store, default -1
 * Returns: void
 */
void TControl::updateArrayID(int aid)
{
	TObjectLocker threadLock(&thread);
	arrayID = aid;

}

/**
 * Method: TControl::getMargin
 * Purpose: Retrieves the space between the control's location and the area it was given
 * Parameters: void
 * Returns: D2D1_RECT_F - the margin
 */
D2D1_RECT_F TControl::getMargin()
{
	TObjectLocker threadLock(&thread);
	auto ret = margin;

	return ret;
}

bool TControl::SetMargin(const D2D1_RECT_F& newMargin)
{
	return false;
}


/*
* Method: TControl::ShiftHorizontal
* Purpose: Moves the Control left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the control and which direction
* Returns: void
*/
void TControl::ShiftHorizontal(int degrees)
{
	TObjectLocker threadLock(&thread);
	location.left += degrees;
	location.right += degrees;

	if (border.Get())
		border->ShiftHorizontal(degrees);
	if (content.Get())
		content->ShiftHorizontal(degrees);
}

/*
* Method: TControl::ShiftVertical
* Purpose:Moves the Control up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the control and which direction
* Returns: void
*/
void TControl::ShiftVertical(int degrees)
{
	TObjectLocker threadLock(&thread);
	location.top += degrees;
	location.bottom += degrees;

	if (border.Get())
		border->ShiftVertical(degrees);
	if (content.Get())
		content->ShiftVertical(degrees);

}


bool TControl::SetDimension(UINT dimensionValue, dimension_spec spec)
{
	TObjectLocker threadLock(&thread);
	if (!dimensions.Get())
		dimensions = TrecPointerKey::GetNewTrecPointer<Dimensions>();

	switch (spec)
	{
	case dimension_spec::ds_height:
		if ((dimensions->minHeight && dimensionValue < dimensions->minHeight) ||
			(dimensions->maxHeight && dimensionValue > dimensions->maxHeight))
			return false;
		dimensions->height = dimensionValue;
		break;
	case dimension_spec::ds_height_max:
		if ((dimensions->height && dimensionValue < dimensions->height) ||
			(dimensions->minHeight && dimensionValue < dimensions->minHeight))
			return false;
		dimensions->maxHeight = dimensionValue;
		break;
	case dimension_spec::ds_height_min:
		if ((dimensions->height && dimensionValue > dimensions->height) ||
			(dimensions->maxHeight && dimensionValue > dimensions->maxHeight))
			return false;
		dimensions->minHeight = dimensionValue;
		break;


	case dimension_spec::ds_width:
		if ((dimensions->minWidth && dimensionValue < dimensions->minWidth) ||
			(dimensions->maxWidth && dimensionValue > dimensions->maxWidth))
			return false;
		dimensions->width = dimensionValue;
		break;
	case dimension_spec::ds_width_max:
		if ((dimensions->width && dimensionValue < dimensions->width) ||
			(dimensions->minWidth && dimensionValue < dimensions->minWidth))
			return false;
		dimensions->maxWidth = dimensionValue;
		break;
	case dimension_spec::ds_width_min:
		if ((dimensions->width && dimensionValue > dimensions->width) ||
			(dimensions->maxWidth && dimensionValue > dimensions->maxWidth))
			return false;
		dimensions->minWidth = dimensionValue;
	}

	return false;
}

UINT TControl::GetDimension(dimension_spec spec)
{
	if(!dimensions.Get())
		return 0;

	switch (spec)
	{
	case dimension_spec::ds_height:
		return dimensions->height;
	case dimension_spec::ds_height_max:
		return dimensions->maxHeight;
	case dimension_spec::ds_height_min:
		return dimensions->minHeight;


	case dimension_spec::ds_width:
		return dimensions->width;
	case dimension_spec::ds_width_max:
		return dimensions->maxWidth;
	case dimension_spec::ds_width_min:
		return dimensions->minWidth;
	}
}

/**
 * Method: TControl::AddClass
 * Purpose: Labels to serve as id's to draw styles from, useful in determining what HTML/TML element was used in creating the control
 * Parameters: TString& t - Class Name to add
 * Returns: void
 */
void TControl::AddClass(const TString& t)
{
	TObjectLocker threadLock(&thread);
	TString trimmedT = t.GetTrim();
	if (className.GetSize())
	{

		className += L";";
		className += trimmedT;
	}
	else
		className = trimmedT;

}

/**
 * Method: TControl::GetID
 * Purpose: Retrieves the ID for the Control
 * Parameters: void
 * Returns: TString - the ID of the Control (Careful, String could be blank)
 */
TString TControl::GetID()
{
	TObjectLocker threadLock(&thread);
	TString ret(ID);

	return ret;
}

bool TControl::HandlesEvents()
{
	return false;
}

void TControl::Draw(TrecPointer<TVariable> object)
{
}

void TControl::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TControl::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TControl::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TControl::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TControl::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&)
{
}

void TControl::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
}

bool TControl::OnDestroy()
{
	return true;
}

bool TControl::OnScroll(const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
	return false;
}
