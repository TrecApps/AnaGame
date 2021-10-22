
#include "TControl.h"

EventTypeID::EventTypeID(const EventTypeID& copy)
{
	eventType = copy.eventType;
	eventID = copy.eventID;
}

EventTypeID::EventTypeID()
{
	eventType = R_Message_Type::On_Click;
	eventID = -1;
}

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

void TControl::AddAttribute(const TString& att, const TString& value)
{
	this->attributes.addEntry(att, value);
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
	isRightClicked = isLeftClicked = isMouseIn = false;
	fixedWidth = fixedHeight = false;

	this->styles = styles;
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
	TObjectLocker threadLock(&thread);

	bounds = loc;
	TString valpoint;

	OnCreateSize();

	if (attributes.retrieveEntry(L"|ArrayID", valpoint))
	{
		valpoint.ConvertToInt(arrayID);
	}

	if (attributes.retrieveEntry(L"|Shape", valpoint))
	{
		if (valpoint.Compare(L"Ellipse"))
		{
			shape = TShape::T_Ellipse;
			ellipse.point = D2D1::Point2F((location.right + location.left) / 2, (location.top + location.bottom) / 2);
			ellipse.radiusX = (location.right - location.left) / 2;
			ellipse.radiusY = (location.bottom - location.top) / 2;
		}
		else if (valpoint.Compare(L"RoundedRectangle"))
		{
			shape = TShape::T_Rounded_Rect;
			float xRound = (location.left - location.right) / 10;
			float yRound = (location.bottom - location.top) / 10;
			if (attributes.retrieveEntry(L"|RoundedRectX", valpoint))
			{
				valpoint.ConvertToFloat(xRound);
			}
			if (attributes.retrieveEntry(L"|RoundedRectY", valpoint))
			{
				valpoint.ConvertToFloat(yRound);
			}
			roundedRect.rect = location;
			roundedRect.radiusX = xRound;
			roundedRect.radiusY = yRound;
		}
		else if (valpoint.Compare(L"Custom"))
		{

		}
	}

	if (attributes.retrieveEntry(L"|id", valpoint) || attributes.retrieveEntry(L"|ID", valpoint))
	{
		ID.Set(valpoint);
	}

	if (attributes.retrieveEntry(L"|Class", valpoint))
	{
		TrecPointer<styleTable> classy;

		TrecPointer<TDataArray<TString>> classes = className.split(L";");

		for (int c = 0; c < classes->Size(); c++)
		{
			if (classes->at(c).GetSize())
			{
				if (styles.Get())
				{

					for (int c = 0; c < styles->Count(); c++)
					{
						if (styles->ElementAt(c)->style == classes->at(c))
						{
							classy = styles->ElementAt(c);
							if (classy.Get())
							{
								TDataMap<TString> dataClassy;

								for (UINT Rust = 0; Rust < classy->names.count(); Rust++)
								{
									auto ent = classy->names.GetEntryAt(Rust);
									if(ent.Get())
										dataClassy.addEntry(ent->key, ent->object.Get());
								}

								OnCreateStyle(dataClassy);
							}//break;
						}
					}
				}
			}
		}
	}

	OnCreateStyle(attributes);
	this->SetUpTextElement();

	if (attributes.retrieveEntry(TString(L"|FixedHeight"),valpoint))
	{
		if (!valpoint.Compare(L"true"))
			fixedHeight = true;
	}

	if (attributes.retrieveEntry(TString(L"|FixedWidth"), valpoint))
	{
		if (!valpoint.Compare(L"true"))
			fixedWidth = true;
	}

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
	if((bounds.left + newMargin.left) > (bounds.right - newMargin.right))
		return false;

	if ((bounds.top + newMargin.top) > (bounds.right - newMargin.right))
		return false;

	margin = newMargin;
	return true;
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

	return true;
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
	if (!isActive)
		return;
	if (content.Get())
		content->onDraw(location);
	if (text.Get())
		text->OnDraw(object);
	if (border.Get())
		border->onDraw(location);
}

void TControl::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>& args)
{
	if (!isActive)
		return;
	if (IsContained(point, location))
	{
		mOut = this->overrideParent ? message_output::mo_positive_override : message_output::mo_positive_continue;

		if (isRightClicked)
		{
			int index = HasEvent(R_Message_Type::On_Right_Click);
			if (index != -1)
			{
				this->args.Reset();
				this->args.arrayLabel = this->arrayID;
				this->args.eventType = R_Message_Type::On_Right_Click;
				this->args.isClick = true;
				this->args.isLeftClick = false;
				this->args.methodID = index;
				this->args.point = point;
				this->args.positive = true;
				this->args.type = L'\0';
				args.push_back(this->args);
			}
		}
		else
		{
			int index = HasEvent(R_Message_Type::On_R_Button_Up);
			if (index != -1)
			{
				this->args.Reset();
				this->args.arrayLabel = this->arrayID;
				this->args.eventType = R_Message_Type::On_R_Button_Up;
				this->args.isClick = true;
				this->args.isLeftClick = false;
				this->args.methodID = index;
				this->args.point = point;
				this->args.positive = true;
				this->args.type = L'\0';
				args.push_back(this->args);
			}
		}
	}
	isRightClicked = false;
}

void TControl::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& pages, TDataArray<EventArgs>& args)
{
	if (!isActive)
		return;
	if (IsContained(point, location))
	{
		mOut = this->overrideParent ? message_output::mo_positive_override : message_output::mo_positive_continue;

		int index = HasEvent(R_Message_Type::On_R_Button_Down);
		if (index != -1)
		{
			this->args.Reset();
			this->args.arrayLabel = this->arrayID;
			this->args.eventType = R_Message_Type::On_R_Button_Down;
			this->args.isClick = true;
			this->args.isLeftClick = false;
			this->args.methodID = index;
			this->args.point = point;
			this->args.positive = true;
			this->args.type = L'\0';
			args.push_back(this->args);
		}

		pages.push_back(EventID_Cred(R_Message_Type::On_R_Button_Down, TrecPointerKey::GetTrecPointerFromSoft<>(self)));
		isRightClicked = true;
	}
}

void TControl::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	if (!isActive)
		return;
	if (IsContained(point, location))
	{
		mOut = this->overrideParent ? message_output::mo_positive_override : message_output::mo_positive_continue;

		if (isLeftClicked)
		{
			int index = HasEvent(R_Message_Type::On_Click);
			if (index != -1)
			{
				this->args.Reset();
				this->args.arrayLabel = this->arrayID;
				this->args.eventType = R_Message_Type::On_Click;
				this->args.isClick = true;
				this->args.isLeftClick = true;
				this->args.methodID = index;
				this->args.point = point;
				this->args.positive = true;
				this->args.type = L'\0';
				args.push_back(this->args);
			}
			if(text.Get())
			eventAr.push_back(EventID_Cred(R_Message_Type::On_Click, TrecPointerKey::GetTrecPointerFromSoft<>(self), text->GetTextInterceptor()));

		}
		else
		{
			int index = HasEvent(R_Message_Type::On_L_Button_Up);
			if (index != -1)
			{
				this->args.Reset();
				this->args.arrayLabel = this->arrayID;
				this->args.eventType = R_Message_Type::On_L_Button_Up;
				this->args.isClick = true;
				this->args.isLeftClick = true;
				this->args.methodID = index;
				this->args.point = point;
				this->args.positive = true;
				this->args.type = L'\0';
				args.push_back(this->args);
			}
		}
	}
	isLeftClicked = false;
}

void TControl::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& pages, TDataArray<EventArgs>& args)
{
	if (!isActive)
		return;
	if (IsContained(point, location))
	{
		mOut = this->overrideParent ? message_output::mo_positive_override : message_output::mo_positive_continue;
		if (!isMouseIn)
		{
			int index = HasEvent(R_Message_Type::On_Hover_Enter);
			if (index != -1)
			{
				this->args.Reset();
				this->args.arrayLabel = this->arrayID;
				this->args.eventType = R_Message_Type::On_Hover_Enter;
				this->args.isClick = false;
				this->args.isLeftClick = false;
				this->args.methodID = index;
				this->args.point = point;
				this->args.positive = true;
				this->args.type = L'\0';
				args.push_back(this->args);
			}
		}
		isMouseIn = true;
		int index = HasEvent(R_Message_Type::On_Hover);
		if (index != -1)
		{
			this->args.Reset();
			this->args.arrayLabel = this->arrayID;
			this->args.eventType = R_Message_Type::On_Hover;
			this->args.isClick = false;
			this->args.isLeftClick = false;
			this->args.methodID = index;
			this->args.point = point;
			this->args.positive = true;
			this->args.type = L'\0';
			args.push_back(this->args);
		}
		pages.push_back(EventID_Cred(R_Message_Type::On_Hover, TrecPointerKey::GetTrecPointerFromSoft<>(self)));
	}
	else
	{
		if (isMouseIn)
		{
			int index = HasEvent(R_Message_Type::On_Hover_Leave);
			if (index != -1)
			{
				this->args.Reset();
				this->args.arrayLabel = this->arrayID;
				this->args.eventType = R_Message_Type::On_Hover_Leave;
				this->args.isClick = true;
				this->args.isLeftClick = true;
				this->args.methodID = index;
				this->args.point = point;
				this->args.positive = true;
				this->args.type = L'\0';
				args.push_back(this->args);
			}
		}
		isMouseIn = false;
	}
}

void TControl::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>& args)
{
	if (!isActive)
		return;
	if (IsContained(point, location))
	{
		mOut = this->overrideParent ? message_output::mo_positive_override : message_output::mo_positive_continue;

		int index = HasEvent(R_Message_Type::On_LDoubleClick);
		if (index != -1)
		{
			this->args.Reset();
			this->args.arrayLabel = this->arrayID;
			this->args.eventType = R_Message_Type::On_LDoubleClick;
			this->args.isClick = true;
			this->args.isLeftClick = true;
			this->args.methodID = index;
			this->args.point = point;
			this->args.positive = true;
			this->args.type = L'\0';
			args.push_back(this->args);
		}

	}
}

void TControl::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& pages , TDataArray<EventArgs>& args)
{
	if (!isActive)
		return;
	if (IsContained(point, location))
	{
		mOut = this->overrideParent ? message_output::mo_positive_override : message_output::mo_positive_continue;

		int index = HasEvent(R_Message_Type::On_L_Button_Down);
		if (index != -1)
		{
			this->args.Reset();
			this->args.arrayLabel = this->arrayID;
			this->args.eventType = R_Message_Type::On_L_Button_Down;
			this->args.isClick = true;
			this->args.isLeftClick = true;
			this->args.methodID = index;
			this->args.point = point;
			this->args.positive = true;
			this->args.type = L'\0';
			args.push_back(this->args);
		}

		pages.push_back(EventID_Cred(R_Message_Type::On_R_Button_Down, TrecPointerKey::GetTrecPointerFromSoft<>(self)));
		isLeftClicked = true;
	}
}

void TControl::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	bounds = newLoc;
	auto curLoc = location;
	SetSize();

	int index = HasEvent(R_Message_Type::On_Resized);
	if (index != -1)
	{
		this->args.Reset();
		this->args.arrayLabel = this->arrayID;
		this->args.eventType = R_Message_Type::On_Resized;
		this->args.isClick = false;
		this->args.isLeftClick = false;
		this->args.methodID = index;
		this->args.point.x = 0;
		this->args.point.y = 0;
		this->args.positive = true;
		this->args.type = L'\0';
		this->args.oldSize = curLoc;
		this->args.newSize = location;
		args.push_back(this->args);
	}
}

bool TControl::OnDestroy()
{
	return true;
}

bool TControl::OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>& args)
{
	location.bottom += direction.y;
	location.top += direction.y;
	location.left += direction.x;
	location.right += direction.x;

	if (point.x > 0 && point.y > 0)
	{
		int index = HasEvent(R_Message_Type::On_Scrolled);
		if (index != -1)
		{
			this->args.Reset();
			this->args.arrayLabel = this->arrayID;
			this->args.eventType = R_Message_Type::On_Scrolled;
			this->args.isClick = false;
			this->args.isLeftClick = false;
			this->args.methodID = index;
			this->args.point = point;
			this->args.positive = true;
			this->args.type = L'\0';
			args.push_back(this->args);
		}
		return true;
	}

	return false;
}

int TControl::HasEvent(R_Message_Type mType)
{
	for (UINT Rust = 0; Rust < eventList.Size(); Rust++)
	{
		if (eventList[Rust].eventType == mType)
			return eventList[Rust].eventID;
	}
	return -1;
}

void TControl::OnCreateSize()
{
	location = bounds;

	TString valpoint;
	UINT dime = 0;
	if (attributes.retrieveEntry(L"|Height", valpoint) && TString::ConvertStringToUint(valpoint, dime))
	{
		SetDimension(dime, dimension_spec::ds_height);
	}
	if (attributes.retrieveEntry(L"|MaxHeight", valpoint) && TString::ConvertStringToUint(valpoint, dime))
	{
		SetDimension(dime, dimension_spec::ds_height_max);
	}
	if (attributes.retrieveEntry(L"|MinHeight", valpoint) && TString::ConvertStringToUint(valpoint, dime))
	{
		SetDimension(dime, dimension_spec::ds_height_min);
	}


	if (attributes.retrieveEntry(L"|Width", valpoint) && TString::ConvertStringToUint(valpoint, dime))
	{
		SetDimension(dime, dimension_spec::ds_width);
	}
	if (attributes.retrieveEntry(L"|MaxWidth", valpoint) && TString::ConvertStringToUint(valpoint, dime))
	{
		SetDimension(dime, dimension_spec::ds_width_max);
	}
	if (attributes.retrieveEntry(L"|MinWidth", valpoint) && TString::ConvertStringToUint(valpoint, dime))
	{
		SetDimension(dime, dimension_spec::ds_width_min);
	}


	if (attributes.retrieveEntry(L"|Margin", valpoint))
		SetMargin(ConvertStringToD2D1Rect(valpoint));

	SetSize();
}

void TControl::SetSize()
{
	location.top += margin.top;
	location.left += margin.left;
	location.right -= margin.right;
	location.bottom -= margin.bottom;


	float curWidth = location.right - location.left;
	float curHeight = location.bottom - location.top;

	UINT maxDime = GetDimension(dimension_spec::ds_width_max),
		minDime = GetDimension(dimension_spec::ds_width_min);

	if (maxDime && curWidth > maxDime)
	{
		location.right = location.left + maxDime;
	}
	else if (minDime && curWidth < minDime)
	{
		D2D1_RECT_F newBounds = bounds;
		UINT diff = curWidth - minDime;
		location.right += diff;
		newBounds.right += diff;

		if (parent.Get())
		{
			TrecPointer<TPage> tParent = TrecPointerKey::GetTrecPointerFromSoft<>(parent);
			tParent->InjectScrollerPage(bounds, newBounds, TrecPointerKey::GetTrecPointerFromSoft<>(self));
			bounds = newBounds;
		}
	}


	maxDime = GetDimension(dimension_spec::ds_height_max);
	minDime = GetDimension(dimension_spec::ds_height_min);

	if (maxDime && curHeight > maxDime)
	{
		location.bottom = location.top + maxDime;
	}
	else if (minDime && curHeight < minDime)
	{
		D2D1_RECT_F newBounds = bounds;
		UINT diff = curHeight - minDime;
		location.bottom += diff;
		newBounds.bottom += diff;

		if (parent.Get())
		{
			TrecPointer<TPage> tParent = TrecPointerKey::GetTrecPointerFromSoft<>(parent);
			tParent->InjectScrollerPage(bounds, newBounds, TrecPointerKey::GetTrecPointerFromSoft<>(self));
			bounds = newBounds;
		}
	}
}


void TControl::SetUpTextElement()
{
	TextFormattingDetails details;

	TString locale(L"en-us"), font(L"ariel");

	TString actText;

	DWRITE_TEXT_ALIGNMENT hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
	DWRITE_PARAGRAPH_ALIGNMENT vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

	TString valpoint;

	TColor fontColor;
	if (attributes.retrieveEntry(TString(L"|Caption"), valpoint))
	{
		actText.Set(valpoint);
	}

	if (attributes.retrieveEntry(TString(L"|CaptionLocale"), valpoint))
	{
		locale.Set(valpoint);
	}

	if (attributes.retrieveEntry(TString(L"|Font"), valpoint))
	{
		font.Set(valpoint);
	}
	if (attributes.retrieveEntry(TString(L"|FontSize"), valpoint))
	{
		valpoint.ConvertToFloat(details.fontSize);
	}
	if (attributes.retrieveEntry(TString(L"|HorizontalAlignment"), valpoint))
	{
		hAlign = convertStringToTextAlignment(valpoint);
	}
	if (attributes.retrieveEntry(TString(L"|VerticalAlignment"), valpoint))
	{
		vAlign = convertStringToParagraphAlignment(valpoint);
	}

	if (attributes.retrieveEntry(TString(L"|FontColor"), valpoint))
	{
		bool b = false;
		fontColor.SetColor(TColor::GetColorFromString(valpoint,b));
	}
	else
		fontColor.SetColor(drawingBoard->GetDefaultTextColor());

	// Commented out code (in case gradient colors should be added

	//valpoint = att->retrieveEntry(TString(L"|TextGrad"));
	//if (valpoint.Get())
	//{
	//	if (!text1.Get())
	//		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, (this));
	//	float entry = 0.0f;
	//	valpoint->ConvertToFloat(entry);
	//	//text1->secondColor = true;
	//	UINT gradCount = 0;
	//	if (gradCount = text1->stopCollection.GetGradientCount())
	//		text1->stopCollection.SetPositionAt(entry, gradCount - 1);
	//}


	//valpoint = att->retrieveEntry(TString(L"|TextGradMode"));
	//if (valpoint.Get())
	//{
	//	if (!text1.Get())
	//		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, (this));
	//	if (valpoint->Compare(L"Radial"))
	//		text1->useRadial = true;
	//}

	// See if have text. If we do, then set up text element
	if (actText.GetSize())
	{
		details.color = drawingBoard->GetBrush(fontColor);

		text = TrecPointerKey::GetNewSelfTrecPointer<TTextElement>(drawingBoard);

		text->SetLocation(location);
		text->SetBasicFormatting(details);
		text->SetHorizontallignment(hAlign);
		text->SetVerticalAlignment(vAlign);
		text->SetText(actText);
	}
}

void TControl::OnCreateStyle(TDataMap<TString>& atts)
{
	TString valpoint;

	if (atts.retrieveEntry(L"|BorderThickness", valpoint))
	{
		if (!border.Get())
			border = TControlComponent::GetControlBorder(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));  //TrecPointerKey::GetNewTrecPointer<TControlComponent>(drawingBoard, (this));
		valpoint.ConvertToFloat((border->thickness));         // Logic Bug needs fixing
	}

	bool worked;
	if (atts.retrieveEntry(L"|BorderColor", valpoint))
	{
		if (!border.Get())
			border = TControlComponent::GetControlBorder(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		
		TColor::GetColorFromString(valpoint, worked);
		border->stopCollection.AddGradient(TGradientStop(TColor::GetColorFromString(valpoint, worked), 0.0f));
	}


	if (atts.retrieveEntry(L"|ContentThickness", valpoint))
	{
		if (!content.Get())
			content = TControlComponent::GetControlContent(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		valpoint.ConvertToFloat((content->thickness));
	}

	if (atts.retrieveEntry(L"|ContentColor", valpoint))
	{
		if (!content.Get())
			content = TControlComponent::GetControlContent(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		content->stopCollection.AddGradient(TGradientStop(TColor::GetColorFromString(valpoint, worked), 0.0f));
	}

	if (atts.retrieveEntry(L"|ContentGrad", valpoint))
	{
		if (!content.Get())
			content = TControlComponent::GetControlContent(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		float entry = 0.0f;
		valpoint.ConvertToFloat(entry);
		//content->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = content->stopCollection.GetGradientCount())
			content->stopCollection.SetPositionAt(entry, gradCount - 1);
	}

	if (atts.retrieveEntry(L"|BorderGrad", valpoint))
	{
		if (!border.Get())
			border = TControlComponent::GetControlBorder(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		float entry = 0.0f;
		valpoint.ConvertToFloat(entry);
		//border->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = border->stopCollection.GetGradientCount())
			border->stopCollection.SetPositionAt(entry, gradCount - 1);
	}

	if (atts.retrieveEntry(L"|ContentGradMode", valpoint))
	{
		if (!content.Get())
			content = TControlComponent::GetControlContent(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		if (valpoint.Compare(L"Radial"))
			content->useRadial = true;
	}
	if (atts.retrieveEntry(L"|BorderGradMode", valpoint))
	{
		if (!border.Get())
			border = TControlComponent::GetControlBorder(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		if (valpoint.Compare(L"Radial"))
			border->useRadial = true;
	}

	if (atts.retrieveEntry(L"|ImageSource", valpoint))
	{
		if (!content.Get())
			content = TControlComponent::GetControlContent(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		valpoint.Replace(L"/", L"\\");

		TrecPointer<TFileShell> file = TFileShell::GetFileInfo(valpoint);

		if (file.Get())
		{
			TObjectLocker threadLock(&thread);
			content->image = drawingBoard->GetBrush(file, location);
		}
	}


}

/**
 * Function: convertStringToTextAlignment
 * Purpose: Reads the String and returns whether the Text should be right, left, center, or justified
 * Parameters: TString* t - the string to parse
 * Returns: DWRITE_TEXT_ALIGNMENT - the Text position (Center if string is invalid)
 */
DWRITE_TEXT_ALIGNMENT convertStringToTextAlignment(const TString& t)
{

	if (!t.CompareNoCase(L"left"))
		return DWRITE_TEXT_ALIGNMENT_TRAILING;
	if (!t.CompareNoCase(L"right"))
		return DWRITE_TEXT_ALIGNMENT_LEADING;
	if (!t.CompareNoCase(L"justified"))
		return DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	return DWRITE_TEXT_ALIGNMENT_CENTER;
}


/**
 * Function: convertStringToParagraphAlignment
 * Purpose: Reads the String and returns whether the Text should be top, bottom, or center
 * Parameters: TString* t - the string to parse
 * Returns: DWRITE_PARAGRAPH_ALIGNMENT - the text alignment (Center if string is invalid)
 */
DWRITE_PARAGRAPH_ALIGNMENT convertStringToParagraphAlignment(const TString& t)
{
	if (!t.Compare(L"Top"))
		return DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	if (!t.Compare(L"Bottom"))
		return DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	return DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
}


/*
 * Function: isContained
 * Purpose: Checks of a point is within a given Direct2D Rectangle
 * Parameters: const TPoint& - the point to check
 *				const D2D1_RECT_F& - the rectangle to check
 * Returns: bool - whether the point is withing the bounds
 */
bool IsContained(const TPoint& cp, const D2D1_ELLIPSE& el)
{
	return (pow((cp.x - el.point.x), 2) / pow(el.radiusX, 2)
		+ pow((cp.y - el.point.y), 2) / pow(el.radiusY, 2)) <= 1;
}