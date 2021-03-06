
#include "TComboBox.h"


TDataArray<TString> comboAtt, regAtt;

/**
 * Function: SetUpComboAtt
 * Purpose: Initializes the comboAtt and regAtt lists with combo-box attributes and their regular counter parts
 * Parameters: void
 * Returns: void
 */
void SetUpComboAtt()
{
	if (comboAtt.Size() && regAtt.Size())
		return;

	comboAtt.push_back(L"|ComboContentColor");
	regAtt.push_back(L"|ContentColor");


	comboAtt.push_back(L"|ComboBorderThickness");
	regAtt.push_back(L"|BorderThickness");


	comboAtt.push_back(L"|ComboBorderColor");
	regAtt.push_back(L"|BorderColor");

	comboAtt.push_back(L"|ComboFont");
	regAtt.push_back(L"|Font");

	comboAtt.push_back(L"|ComboFontColor");
	regAtt.push_back(L"|FontColor");

	comboAtt.push_back(L"|ComboFontSize");
	regAtt.push_back(L"|FontSize");

	comboAtt.push_back(L"|ComboHorizontalAlignment");
	regAtt.push_back(L"|HorizontalAlignment");

	comboAtt.push_back(L"|ComboVerticalAlignment");
	regAtt.push_back(L"|VerticalAlignment");

	comboAtt.push_back(L"|ComboHoverContentColor");
	regAtt.push_back(L"|HoverContentColor");

	comboAtt.push_back(L"|ComboHoverBorderThickness");
	regAtt.push_back(L"|HoverBorderThickness");

	comboAtt.push_back(L"|ComboHoverBorderColor");
	regAtt.push_back(L"|HoverBorderColor");

	comboAtt.push_back(L"|ComboHoverFont");
	regAtt.push_back(L"|HoverFont");

	comboAtt.push_back(L"|ComboHoverFontColor");
	regAtt.push_back(L"|HoverFontColor");

	comboAtt.push_back(L"|ComboHoverFontSize");
	regAtt.push_back(L"|HoverFontSize");

	comboAtt.push_back(L"|ComboHoverHorizontalAlignment");
	regAtt.push_back(L"|HoverHorizontalAlignment");

	comboAtt.push_back(L"|ComboHoverVerticalAlignment");
	regAtt.push_back(L"|HoverVerticalAlignment");

}

/*
* Method: (TComboBox) (Constructor)
* Purpose: Set up the ComboBox with default values
* Parameters: TrecPointer<DrawingBoard> rt - The Render Target to draw to
*				TrecPointer<TArray<styleTable>> ta - The Style Table to draw from
* Returns: void
*/
TComboBox::TComboBox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta):TGadgetControl(rt,ta)
{
	vertexPoint = D2D1::Point2F();
	initClick = false;
	SetUpComboAtt();
}

/*
* Method: (TComboBox) (Destructor)
* Purpose: Cleans up TComboBox resources
* Parameters: void
* Returns: void
* NOTE: Code currently commented out to avoid memory corruption
*/
TComboBox::~TComboBox()
{

}

/*
* Method: TComboBox - onCreate
* Purpose: Sets up The TControl attributes and the Combo-box specific attributes
* Parameters: RECT r - the Location the Basic Box would be in
* Returns: bool
*/
bool TComboBox::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	ThreadLock();
	extension = TrecPointerKey::GetNewSelfTrecSubPointer<TControl, TComboBoxExtension>(drawingBoard, TrecPointer<TArray<styleTable>>(), 
		TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TComboBox>(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));

	flyout = TrecPointerKey::GetNewSelfTrecPointer<TFlyout>(TrecPointerKey::GetTrecPointerFromSub<TControl, TComboBoxExtension>(extension));

	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|SubHeight"));

	int childHeight;
	if (valpoint.Get() && !valpoint->ConvertToInt(childHeight))
	{
		extension->childHeight = childHeight;
	}
	else
		extension->childHeight = 30;
	valpoint = attributes.retrieveEntry(TString(L"|DefaultText"));
	if (valpoint.Get())
	{
		if (!text1.Get())
		{
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard,this);
		}
		text1->removeCaption();
		defaultText = valpoint.Get();

		text1->setCaption(defaultText);
	}
	int occ = 0;
	valpoint = attributes.retrieveEntry(TString(L"|BoxEntry"),occ++);

	while (valpoint.Get())
	{
		extension->elements.push_back(valpoint.Get());
		valpoint = attributes.retrieveEntry(TString(L"|BoxEntry"), occ++);
	}
	
	// To-Do later when features are being added:
	// add mechanism to control the style of the sub ojbects

	//TrecPointer<TContainer> cont;
	TrecPointer<TControl> tc;

	TGadgetControl::onCreate(r,d3d);

	leftpoint = D2D1::Point2F(DxLocation.left, DxLocation.top);
	rightPoint = D2D1::Point2F(DxLocation.right, DxLocation.top);

	float midDx = DxLocation.left + ((DxLocation.right - DxLocation.left) / 2);

	vertexPoint = D2D1::Point2F(midDx, DxLocation.bottom);

	// Handle Styling of the Extension box
	bool handledFontSize = false, handledContentColor = false;
	for (UINT Rust = 0; Rust < comboAtt.Size() && regAtt.Size(); Rust++)
	{
		valpoint = attributes.retrieveEntry(comboAtt[Rust]);
		if (valpoint.Get())
		{
			extension->attributes.addEntry(regAtt[Rust], valpoint);
			if (!regAtt[Rust].Compare(L"|FontSize"))
				handledFontSize = true;
			if (!regAtt[Rust].Compare(L"|ContentColor"))
				handledContentColor = true;
		}
	}

	r = location;
	r.top = r.bottom;
	r.bottom = r.bottom + 10;

	if (!handledFontSize)
	{
		extension->attributes.addEntry(L"|FontSize", TrecPointerKey::GetNewTrecPointer<TString>(L"12"));
	}
	if (!handledContentColor)
	{
		extension->attributes.addEntry(L"|ContentColor", TrecPointerKey::GetNewTrecPointer<TString>(L"1.0,1.0,1.0,1.0"));
	}
	extension->onCreate(r, d3d);

	// Remaining attributes
	valpoint = attributes.retrieveEntry(TString(L"|BoxEntry"));

	if (valpoint.Get() && !valpoint->ConvertToInt(childHeight) && childHeight > -1)
	{
		extension->maxHeight = childHeight;
	}
	ThreadRelease();

	return false;
}



/*
* Method: TComboBox - OnLButtonDown
* Purpose: Determins if the Combo-Box was clicked and if so, which element
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				CPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
void TComboBox::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	ThreadLock();
	resetArgs();

	if (isContained(point, location))
	{
		initClick = true;
		clickedControl.push_back(this);
	}
	ThreadRelease();
}

void TComboBox::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	resetArgs();

	if (isContained(point, location) && initClick)
	{
		// To-Do: Return the TFlyout somehow
		if (flyout.Get())
			eventAr.push_back(EventID_Cred(flyout));

		// End to-do
	}
	initClick = false;
	ThreadRelease();
}


void TComboBox::Resize(D2D1_RECT_F& r)
{
	ThreadLock();
	TGadgetControl::Resize(r);
	
	if (extension.Get())
	{
		r.top = r.bottom;
		r.bottom = r.bottom + 10;
		extension->Resize(r);
	}
	ThreadRelease();
}

void TComboBox::UpdateCaption(TString& str, UINT index)
{
	ThreadLock();
	if (text1.Get())
		text1->setCaption(str);

	resetArgs();

	args.arrayLabel = index;
	args.control = this;
	args.eventType = R_Message_Type::On_sel_change;
	args.isClick = true;
	args.isLeftClick = true;
	args.methodID = getEventID(R_Message_Type::On_sel_change);
	args.text = str;
	ThreadRelease();
}

/**
 * Method: TComboBoxExtension::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TComboBoxExtension::GetType()
{
	return TString(L"TComboBoxExtension;") + TControl::GetType();
}

TComboBoxExtension::TComboBoxExtension(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, TrecSubPointer<TControl, TComboBox> combo): TControl(rt, ta)
{
	this->combo = combo;

	this->childHeight = 30;
	this->maxHeight = 0;

	hoverSelection = -1;
	clickSelection = -1;
}


/*
* Method: TComboBoxExtension::addElement
* Purpose: Adds an element to the Drop-Down Menu
* Parameters: TString
* Returns: void
*/
void TComboBoxExtension::addElement(TString& str)
{
	ThreadLock();
	if (!str.IsEmpty())
		elements.push_back(str);
	ThreadRelease();
}

/*
* Method: TComboBoxExtension::removeElement
* Purpose: Removes an Element from the drop down menu
* Parameters: TString
* Returns: bool - whether the string was found and removed
*/
bool TComboBoxExtension::removeElement(TString& str)
{
	ThreadLock();
	bool ret = false;
	for (UINT Rust = 0; Rust < elements.Size(); Rust++)
	{
		if (!elements[Rust].Compare(str))
		{
			ret = true;
			break;;
		}
	}
	ThreadRelease();
	return ret;
}

void TComboBoxExtension::Resize(D2D1_RECT_F& r)
{
	ThreadLock();
	location = r;

	// To-Do: Handle scenario where a TScrollerControl may need to be created
	ThreadRelease();
}

void TComboBoxExtension::onDraw(TObject* obj)
{
	ThreadLock();
	D2D1_RECT_F miniLoc = location;
	miniLoc.bottom = miniLoc.top + childHeight;

	for (UINT Rust = 0; Rust < elements.Size(); Rust++)
	{
		TrecPointer<TText> useText = (Rust == hoverSelection && text2.Get()) ? text2: text1;
		TrecPointer<TContent> useContent = (Rust == hoverSelection && content2.Get()) ? content2 : content1;
		TrecPointer<TBorder> useBorder = (Rust == hoverSelection && border2.Get()) ? border2 : border1;

		if (useContent.Get())
		{
			// useContent->SetLocation(miniLoc);
			useContent->onDraw(miniLoc);
		}

		if (useText.Get())
		{
			useText->setCaption(elements[Rust]);
			useText->onDraw(miniLoc);
		}
		if (useBorder.Get())
		{
			useBorder->onDraw(miniLoc);
		}

		miniLoc.top += childHeight;
		miniLoc.bottom += childHeight;
	}
	ThreadRelease();
}

void TComboBoxExtension::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	ThreadLock();
	auto rect = location;

	rect.bottom = rect.top + childHeight * elements.Size();
	if (isContained(point, rect))
	{
		clickSelection = static_cast<int>(point.y - rect.top) / childHeight;
		clickedControl.push_back(this);

		*mOut = messageOutput::positiveOverrideUpdate;
	}
	ThreadRelease();
}

void TComboBoxExtension::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	auto rect = location;

	rect.bottom = rect.top + childHeight * elements.Size();
	if (isContained(point, rect))
	{
		if ((clickSelection == static_cast<int>(point.y - rect.top) / childHeight) && combo.Get())
		{
			combo->UpdateCaption(elements[clickSelection], clickSelection);
			EventID_Cred cred;
			cred.control = TrecPointerKey::GetTrecPointerFromSub<TControl, TComboBox>(combo);
			cred.eventType = R_Message_Type::On_sel_change;
			eventAr.push_back(cred);
		}
		*mOut = messageOutput::positiveOverrideUpdate;
	}
	clickSelection = -1;
	ThreadRelease();
}

/**
 * Method: TComboBoxExtension::OnMouseMove
 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
 * Returns: void
 */
void TComboBoxExtension::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	ThreadLock();
	auto rect = location;

	rect.bottom = rect.top + childHeight * elements.Size();
	if (isContained(point, rect))
	{
		hoverSelection = static_cast<int>(point.y - rect.top) / childHeight;

		*mOut = messageOutput::positiveOverrideUpdate;
	}
	ThreadRelease();
}
