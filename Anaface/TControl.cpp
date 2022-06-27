#include "..\Anaface2\TControl.h"
//#include "stdafx.h"
#include "AnafaceUI.h"
#include <Logger.h>
#include <wincodec.h>
#include "TScrollerControl.h"
#include <TTextRenderer.h>

	TTextRenderer* textRenderer;
/*
* Method:
* Purpose:
* Parameters:
* Returns:
*/


/**
 * Method: TControl::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TControl::GetType()
{
	return TString(L"TControl;") + TObject::GetType();
}

/*
* Method: TControl::TControl
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
*				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
* Return: New TControl Object
*/
TControl::TControl(TrecPointer<DrawingBoard> db,TrecPointer<TArray<styleTable>> styTab)
{
	drawBackground = true;
	arrayID = -1;
	eventHandler = NULL;
	isActive = true;
	treeLevel = 0;
	drawingBoard = db;
	styles = styTab;
	location.bottom = 0;
	location.top = 0;
	location.left = 0;
	location.right = 0;
	
	boundsPreset = false;
	location = D2D1_RECT_F{ 0.0f,0.0f,0.0f,0.0f };
	mState = messageState::normal;
	overrideParent = true;
	marginPriority = true;
	dimensions = NULL;
	margin = D2D1_RECT_F{ 0.0f,0.0f,0.0f,0.0f };
	marginSet = false;
	//PointerCase = TrecPointer<TControl>(this);


	shape =TShape::T_Rect;
	fixHeight = fixWidth = false;
	rightBorder = leftBorder = topBorder = bottomBorder = onFocus = onClickFocus = false;

	controlTransform = D2D1::IdentityMatrix();

	isLClick = isRClick = false;
	

	TString logMessage;
	logMessage.Format(L"CREATE %p TControl(TrecComPointer<ID2D1RenderTarget>,TrecPointer<TArray<styleTable>>, bool)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TControl::TControl
* Purpose: Copy Constructor
* Parameters: TControl &rCont - the Constructor to draw from
* Returns: Copy of TControl
*/
TControl::TControl(TControl & rCont)
{
	drawBackground = rCont.drawBackground;
	isLClick = isRClick = false;

	arrayID = rCont.arrayID;
	eventHandler = rCont.eventHandler;
	isActive = rCont.isActive;

	boundsPreset = rCont.boundsPreset;
	className = rCont.className;
	ID = rCont.ID;
	location = rCont.location;
	margin = rCont.margin;
	vScroll = rCont.vScroll;
	hScroll = rCont.hScroll;
	drawingBoard = rCont.drawingBoard;
	parent = rCont.parent;
	//PointerCase = TrecPointer<TControl>(PointerCase);
	treeLevel = rCont.treeLevel;

	// Quickly null the contents, borders, and text
	//text1 = text2 = text3 = NULL;
	//content1 = content2 = content3 = NULL;
	//border1 = border2 = border3 = NULL;

	if(rCont.border1.Get())
	border1 = TrecPointerKey::GetNewTrecPointer<TBorder>((rCont.border1),(this));
	if (rCont.border2.Get())
		border2 = TrecPointerKey::GetNewTrecPointer<TBorder>((rCont.border2), (this));
	if (rCont.border3.Get())
		border3 = TrecPointerKey::GetNewTrecPointer<TBorder>((rCont.border3), (this));

	if (rCont.content1.Get())
		content1 = TrecPointerKey::GetNewTrecPointer<TContent>((rCont.content1), (this));
	if(rCont.content2.Get())
		content2 = TrecPointerKey::GetNewTrecPointer<TContent>((rCont.content2), (this));
	if(rCont.content3.Get())
		content3 = TrecPointerKey::GetNewTrecPointer<TContent>((rCont.content3), (this));

	ellipse = rCont.ellipse;
	roundedRect = rCont.roundedRect;
	shape = rCont.shape;


	if (rCont.dimensions)
	{
		dimensions = new sizeControl;
		dimensions->height = rCont.dimensions->height;
		dimensions->maxHeight = rCont.dimensions->maxHeight;
		dimensions->maxWidth = rCont.dimensions->maxWidth;
		dimensions->minHeight = rCont.dimensions->minHeight;
		dimensions->minWidth = rCont.dimensions->minWidth;
		dimensions->width = rCont.dimensions->width;
	}
	else
		dimensions = NULL;


	marginPriority = rCont.marginPriority;
	marginSet = rCont.marginSet;
	fixHeight = rCont.fixHeight;
	fixWidth = rCont.fixWidth;

	attributes = rCont.attributes;
		
	styles = rCont.styles;
	children = rCont.children;
	holder = rCont.holder;

	// To-Do Later
	overrideParent; 
	messageState mState; // mechanism as to which version of the TControl should be drawn based off of user interaction
	TFlyout* flyout;     // Generic flyout, which can appear if attached
	TContextMenu* contextMenu; // specific flyout primed for dealing with right clicks, as is traditional

							   // and Builder Messages
	rightBorder= rCont.rightBorder; 
	leftBorder=rCont.leftBorder;
	topBorder=rCont.topBorder;
	bottomBorder=rCont.bottomBorder;
	onFocus=rCont.onFocus;
	onClickFocus=rCont.onClickFocus;

	eventList = rCont.eventList;

	controlTransform = D2D1::IdentityMatrix();

	if (rCont.text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TTextElement>(drawingBoard);
		TString tDetails;

		rCont.text1->GetSetFont(tDetails);
		text1->GetSetFont(tDetails, false);

		rCont.text1->GetText(tDetails);
		text1->SetText(tDetails);

		rCont.text1->GetSetLocale(tDetails);
		text1->GetSetLocale(tDetails, false);

		text1->SetHorizontallignment(rCont.text1->GetHorizontalAlignment());
		text1->SetVerticalAlignment(rCont.text1->GetVerticalAligment());

		TextFormattingDetails details;
		rCont.text1->GetBasicFormatting(details);
		text1->SetBasicFormatting(details);
	}

	TString logMessage;
	logMessage.Format(L"CREATE %p TControl(TControl&)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TControl::TControl) 
* Purpose: Default contructor
* Parameters: void
* Returns: Blank TControl
*/
TControl::TControl()
{
	isLClick = isRClick = false;
	arrayID = -1;

	eventHandler = NULL;
	isActive = drawBackground = true;

	treeLevel = 0;

	location.bottom = 0;
	location.top = 0;
	location.left = 0;
	location.right = 0;
	//parent = NULL;
	boundsPreset = false;
	location = D2D1_RECT_F{ 0.0f,0.0f,0.0f,0.0f };
	mState = messageState::normal;
	overrideParent = true;
	marginPriority = true;
	dimensions = NULL;
	margin = D2D1_RECT_F{ 0.0f,0.0f,0.0f,0.0f };
	marginSet = false;

	//PointerCase =TrecPointer<TControl>(this);
	shape =TShape::T_Rect;
	fixHeight = fixWidth = false;
	rightBorder = leftBorder = topBorder = bottomBorder = onFocus = onClickFocus = false;

	controlTransform = D2D1::IdentityMatrix();

	//eventList = new TDataArray<EventTypeID>();
	TString logMessage;
	logMessage.Format(L"CREATE %p TControl()", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TControl::~Tcontrol
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TControl::~TControl()
{
	text1.Delete();
	border1.Delete();
	border2.Delete();
	border3.Delete();
	content1.Delete();
	content2.Delete();
	content3.Delete();

	for (int c = 0; c < children.Count(); c++)
	{
		TrecPointer<TControl> tc;
		tc = children.ElementAt(c);
		tc.Delete();
	}
	children.Clear();
	TString logMessage;
	logMessage.Format(L"DELETE %p TControl", this);

	Log(LogType::lt_memory, logMessage);
}

/**
 * Method: TControl::SetSelf
 * Purpose: Allows TrecPointerKey to provide the Control with a reference to itself
 * Parameters: TrecPointer<TControl> self - the TrecPointer of this Control that the TrecPointerKey generated
 * Returns: void
 */
void TControl::SetSelf(TrecPointer<TControl> self)
{
	if (this != self.Get())
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	tThis = TrecPointerKey::GetSoftPointerFromTrec<TControl>(self);
}




extern TDataArray<TTextField*> TextList;

/*
* Method: TControl::loadFromHTML
* Purpose: Enables Control to load itself from an HTML File
* Parameters: CArchive* ar - archive to read the HTML file
* Returns: int - error
* Note: DEPRECIATED - Functionality should be handled by an HTML parser that's Anaface Compatible
*/
int TControl::loadFromHTML(TFile * ar)
{
	return 0;
}

/*
* Method: TControl::storeInTML
* Purpose: Allows a Control to save itself in a TML File
* Parameters: CArchive* ar - the file to save to
*				int childLevel - the generation if the TControl
*				bool overrideChildren - whether to ignore the children to save
* Returns: void
*/
void TControl::storeInTML(TFile * ar, int childLevel, bool overrideChildren)
{
	TObjectLocker threadLock(&thread);
	TString appendable;
	resetAttributeString(&appendable, childLevel + 1);

	const char* type = typeid(this).name();

	// Only write Object type if this object is a pure TControl
	if (!strcmp( type, "class TControl *"))
	{
		appendable.Append(L">TControl");
		_WRITE_THE_STRING
	}

	if (className.GetSize())
	{
		appendable.Append(L"|Class:");
		appendable.Append(className);
		ar->WriteString(appendable);
		ar->WriteString(L"\n");
		resetAttributeString(&appendable, childLevel + 1);
	}
	if (ID.GetSize())
	{
		appendable.Append(L"|id:");
		appendable.Append(ID);
		ar->WriteString(appendable);
		ar->WriteString(L"\n");
		resetAttributeString(&appendable, childLevel + 1);
	}
	appendable.Append(L"|VerticalScroll:"); 
	appendable.Append(boolToString(vScroll.Get()));
	ar->WriteString(appendable);
	ar->WriteString(L"\n");
	resetAttributeString(&appendable, childLevel + 1);
	appendable.Append(L"|HorizontalScroll:");
	appendable.Append(boolToString(hScroll.Get()));
	ar->WriteString(appendable);
	ar->WriteString(L"\n");
	resetAttributeString(&appendable, childLevel + 1);

	appendable.Append(L"|FixedHeight:");
	appendable.Append(boolToString(fixHeight));
	_WRITE_THE_STRING;

	appendable.Append(L"|FixedWidth:");
	appendable.Append(boolToString(fixWidth));
	_WRITE_THE_STRING;
	
	if (marginSet)
	{
		appendable.Append(L"|Margin:");
		appendable.Append(convertD2D1RectToString(margin));
		ar->WriteString(appendable);
		ar->WriteString(L"\n");
		resetAttributeString(&appendable, childLevel + 1);
	}

	if (dimensions)
	{
		if (dimensions->height)
		{
			appendable.Append(L"|Height:");
			appendable.AppendFormat(L"%d", dimensions->height);
			_WRITE_THE_STRING
		}
		if (dimensions->width)
		{
			appendable.Append(L"|Width:");
			appendable.AppendFormat(L"%d", dimensions->width);
			_WRITE_THE_STRING
		}
		if (dimensions->maxHeight)
		{
			appendable.Append(L"|MaxHeight:");
			appendable.AppendFormat(L"%d", dimensions->maxHeight);
			_WRITE_THE_STRING
		}
		if (dimensions->maxWidth)
		{
			appendable.Append(L"|MaxWidth:");
			appendable.AppendFormat(L"%d", dimensions->maxWidth);
			_WRITE_THE_STRING
		}
		if (dimensions->minHeight)
		{
			appendable.Append(L"|MinHeight:");
			appendable.AppendFormat(L"%d", dimensions->minHeight);
			_WRITE_THE_STRING
		}
		if (dimensions->minWidth)
		{
			appendable.Append(L"|MinWidth:");
			appendable.AppendFormat(L"%d", dimensions->minWidth);
			_WRITE_THE_STRING
		}
	}

	resetAttributeString(&appendable, childLevel + 1);

	if (shape ==TShape::T_Ellipse)
	{
		appendable.Append(L"|Shape:Ellipse");
		_WRITE_THE_STRING;
	}
	else if (shape ==TShape::T_Rounded_Rect)
	{
		appendable.Append(L"|Shape:RoundedRectangle");
		_WRITE_THE_STRING;
		appendable.Append(L"|RoundedRectX:");
		appendable.AppendFormat(L"%f", ellipse.radiusX);
		_WRITE_THE_STRING;
		appendable.Append(L"|RoundedRectY:");
		appendable.AppendFormat(L"%f", ellipse.radiusY);
		_WRITE_THE_STRING;
	}

	if (border1.Get())
	{
		border1->storeInTML(ar, childLevel, messageState::normal);
	}
	if (border2.Get())
	{
		border2->storeInTML(ar, childLevel, messageState::mouseHover);
	}
	if (border3.Get())
		border3->storeInTML(ar, childLevel, messageState::mouseLClick);

	// To-Do: Handle Text Element

	if (content1.Get())
		content1->storeInTML(ar, childLevel, messageState::normal);
	if (content2.Get())
		content2->storeInTML(ar, childLevel, messageState::mouseHover);
	if (content3.Get())
		content3->storeInTML(ar, childLevel, messageState::mouseLClick);


	if (!overrideChildren)
	{
		for (int c = 0; c < children.Count(); c++)
		{
			children.ElementAt(c)->storeInTML(ar, childLevel + 1);
		}
	}

	resetAttributeString(&appendable, childLevel + 1);
	ar->WriteString(appendable);
	ar->WriteString(L"/\n");
	
}

/*
* Method: TControl::storeInHTML
* Purpose: Enables a Control to save itself in an HTML file
* Parameters: TFile* ar - the file to save to
* Returns: void
*/
void TControl::storeInHTML(TFile * ar)
{

	
}

bool TControl::TookTab(TrecPointer<Tab> tab)
{
	if(!tab.Get())
		return false;
	TObjectLocker threadLock(&thread);
	for (UINT Rust = 0; Rust < children.Count(); Rust++)
	{
		auto ch = children.ElementAt(Rust);
		if (ch.Get() && ch->TookTab(tab))
		{
			
			return true;
		}
	}
	
	return false;
}

/*
* Method: TControl::onCreate
* Purpose: Allows the Control To contstruct itself based off of the location it has and the
*		screen space it is given
* Parameters: RECT contain - the area it can use
* Returns: bool - success
* Note: You Must call this on the Root Control before any control can be drawn on sreen
*/
bool TControl::onCreate(D2D1_RECT_F contain, TrecPointer<TWindowEngine> d3d)
{
	TObjectLocker threadLock(&thread);
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|FixedHeight"));
	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"true"))
			fixHeight = true;
	}
	valpoint = attributes.retrieveEntry(TString(L"|FixedWidth"));
	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"true"))
			fixWidth = true;
	}

	valpoint = attributes.retrieveEntry(TString(L"|DrawingBoardContentFallback"));
	if (valpoint.Get())
	{
		if (!valpoint->CompareNoCase(L"true"))
			drawBackground = true;
		else if (!valpoint->CompareNoCase(L"false"))
			drawBackground = false;
	}


	valpoint = attributes.retrieveEntry(TString(L"|ArrayID"));
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(arrayID);
	}

	valpoint = attributes.retrieveEntry(TString(L"|Shape"));
	if (valpoint.Get())
	{
		if (valpoint->Compare(L"Ellipse"))
		{
			shape =TShape::T_Ellipse;
			ellipse.point = D2D1::Point2F((location.right + location.left) / 2, (location.top + location.bottom) / 2);
			ellipse.radiusX = (location.right - location.left) / 2;
			ellipse.radiusY = (location.bottom - location.top) / 2;
		}
		else if (valpoint->Compare(L"RoundedRectangle"))
		{
			shape =TShape::T_Rounded_Rect;
			float xRound = (location.left - location.right) / 10;
			float yRound = (location.bottom - location.top) / 10;
			valpoint = attributes.retrieveEntry(TString(L"|RoundedRectX"));
			if (valpoint.Get() )
			{
				valpoint->ConvertToFloat(xRound);
			}
			valpoint = attributes.retrieveEntry(TString(L"|RoundedRectY"));
			if (valpoint.Get())
			{
				valpoint->ConvertToFloat(yRound);
			}
			roundedRect.rect = location;
			roundedRect.radiusX = xRound;
			roundedRect.radiusY = yRound;
		}
		else if (valpoint->Compare(L"Custom"))
		{

		}
	}


	int occ1 = 0;
	valpoint = attributes.retrieveEntry(TString(L"|Class"), occ1++);
	while (valpoint.Get())
	{
		className += *valpoint.Get();
		className.Trim();
		className += L";";
		valpoint = attributes.retrieveEntry(TString(L"|Class"), occ1++);
	}
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
							onCreate(&classy->names, contain);
						//break;
					}
				}
			}
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|id"));
	if (valpoint.Get())
	{
		ID = *valpoint.Get();
	}
	else
	{
		valpoint = attributes.retrieveEntry(TString(L"|ID"));
		if (valpoint.Get())
		{
			ID = *valpoint.Get();
		}
	}

	onCreate(&attributes,contain); // then derive appearence based off of the individual control's spec. If there are any conflicts between
	                       // a style attribute from a style spec and that of the controls' spec, the latter is used 
	onCreate2(&attributes, contain);
	onCreate3(&attributes, contain);

	SetUpTextElement();

	if (border1.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			border1->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			border1->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			border1->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|BorderAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = border1->GetBrush();

			animateData.push_back(animate);
		}
	}

	bool callOnGif = false;
		
	if (content1.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			content1->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			content1->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			content1->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|ContentAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = content1->GetBrush();

			animateData.push_back(animate);
		}

		if (content1->GetImageCount() > 1)
			callOnGif = true;
	}
	if (text1.Get())
	{
		TDataArray<TString> animators = GetMultiData(TString(L"|TextAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = text1->GetBrush();

			animateData.push_back(animate);
		}
	}
	if (border2.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			border2->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			border2->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			border2->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|HoverBorderAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = border2->GetBrush();

			animateData.push_back(animate);
		}
	}
	if (content2.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			content2->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			content2->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			content2->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:

			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|HoverContentAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = content2->GetBrush();

			animateData.push_back(animate);
		}
	}

	Resize(contain);


	onCreateClassAndId(contain);

	onCreate(&attributes,contain); // then derive appearence based off of the individual control's spec. If there are any conflicts between
	                       // a style attribute from a style spec and that of the controls' spec, the latter is used 
	onCreate2(&attributes, contain);
	onCreate3(&attributes, contain);

	int occ = 0;
	valpoint = attributes.retrieveEntry(TString(L"|ContainerLoc"), occ);

	while (valpoint.Get())
	{
		if (occ < children.Count())
		{
			TrecPointer<TControl> contt = children.ElementAt(occ);
			if (!contt.Get())
			{
				// return error code;
			}

			D2D1_RECT_F chLoc = convertStringToD2D1Rect(valpoint.Get());

			
			chLoc.left += location.left;
			chLoc.top += location.top;
			chLoc.bottom += location.top;
			chLoc.right += location.left;
			contt->onCreate(chLoc, d3d);
		}
		else
		{
			// return error code;
		}
		valpoint = attributes.retrieveEntry(TString(L"|ContainerLoc"), ++occ);

	}

	updateComponentLocation();

	// Now see if any child element extends beyond 
	CheckScroll();


	// Now handle Animations that this Control might have
	TDataArray<TString> animators = GetMultiData(TString(L"Animation"));
	for (UINT Rust = 0; Rust < animators.Size(); Rust++)
	{
		TString name = animators[Rust];
		auto names = name.split(TString(L";"));
		if (!names->Size())
			continue;
		TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
		animate->control = tThis;
		animate->name = names->at(0);
		if (names->Size() > 1)
			animate->storyName = names->at(1);

		animateData.push_back(animate);
	}
	
	return true;
}

/*
 * Method: TControl::Resize
 * Purpose: Resizes the control upon the window being resized
 * Parameters: D2D1_RECT_F& r - the new location for the control
 * Returns: void
 */
void TControl::Resize(D2D1_RECT_F& rr)
{
	// Updated Code
	TObjectLocker threadLock(&thread);

	if (SetScrollControlOnMinSize(rr))
	{
		float difHeight = location.top - rr.top;
		location.bottom -= difHeight;
		location.top -= difHeight;
		float difWidth = location.left - rr.left;
		location.left -= difWidth;
		location.right -= difWidth;
		
		return;
	}
	if (isSnipZero(location) || ((location.bottom - location.top) != (rr.bottom - rr.top))
		|| (location.right - location.left != rr.right - rr.left))
	{
		checkMargin(rr);
		checkHeightWidth(rr);

		if (!marginPriority && dimensions && !vScroll.Get() && !hScroll.Get())
		{
			int heightOffset = 0;
			int widthOffset = 0;
			if (dimensions->height == 0 && location.bottom == 0)
				heightOffset = 0;
			else
				heightOffset = ((rr.bottom - rr.top) - dimensions->height) / 2;

			if (dimensions->width == 0 && location.right == 0)
				widthOffset = 0;
			else
				widthOffset = ((rr.right - rr.left) - dimensions->width) / 2;
			location.top = rr.top + heightOffset;
			location.bottom = rr.bottom - heightOffset;
			location.left = rr.left + widthOffset;
			location.right = rr.right - widthOffset;
		}
		else
		{
			if (!dimensions || !dimensions->height) // Use Margin regardless, nothing to override it
			{
				location.bottom = rr.bottom - margin.bottom;
				location.top = rr.top + margin.top;
			}
			if (!dimensions || !dimensions->width)
			{
				location.left = rr.left + margin.left;
				location.right = rr.right - margin.right;
			}
		}
		if (vScroll.Get())
		{
			if (dimensions && dimensions->height)
			{
				location.top = rr.top + margin.top;
				location.bottom = location.top + dimensions->height;
				if ((rr.bottom - rr.top) > (location.bottom - location.top))
					location.bottom = rr.bottom - margin.bottom;
			}
			else
				location = rr; // if conlfict with hScroll, next code will resolve
		}
		else if (marginPriority)
		{
			location.top = rr.top + margin.top;
			location.bottom = rr.bottom - margin.bottom;
			if (dimensions && dimensions->height)
				dimensions->height = location.bottom - location.top;
		}
		if (hScroll.Get())
		{
			if (dimensions && dimensions->width)
			{
				location.left = rr.left + margin.left;
				location.right = location.left + dimensions->width;
				if ((rr.right - rr.left) > (location.right - location.left))
					location.right = rr.right - margin.right;
			}
			else
			{
				location.left = rr.left + margin.left;
				location.right = rr.right - margin.right;
			}
		}
		else if (marginPriority)
		{
			location.left = rr.left + margin.left;
			location.right = rr.right - margin.right;
			if (dimensions && dimensions->width)
				dimensions->width = location.right - location.left;
		}
	}
	else
	{
		// Already initialized and Size is not changing, we're just shifting the controls
		float x = rr.left - location.left;
		float y = rr.top - location.top;

		onScroll(x, y);
	}
	
}

/*
* Method: TControl::setEventHandler
* Purpose: Sets up an event handler
* Parameters: EventTarget& eh - the Event Class
* Returns: bool - just returns true
* Note: Possible redundence, expect to be DEPRECIATED
*/
bool TControl::setEventHandler(EventTarget & eh)
{
	TObjectLocker threadLock(&thread);
	for (int c = 0; c < children.Count(); c++)
	{
		TControl* tc = children.ElementAt(c).Get();
		if (tc)
			tc->setEventHandler(eh);
	}
	eventHandler = &eh;
	
	return true;
}

/*
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

/* Purpose: to allow the TControl to shift it's contents according to how it is scrolled.
 * Caller: Member TScrollBars, either vScroll or hScroll
 * Parameter: int (x) how to shift itself horizontally
 * Parameter: int (y) how to shift itself vertically
 * return: bool, just because
 * Details: Changes the contents location on the board, reflected in the changes to
 *		TControl's location, but not where it draws, hence why it's own snip stays the same
 *		However, the contents might include child locations so their snips might have to be 
 *		updated
 */
bool TControl::onScroll(float x, float y)
{
	TObjectLocker threadLock(&thread);
	location.left += x;
	location.right += x;
	location.bottom += y;
	location.top += y;
	for (int c = 0; c < children.Count(); c++)
	{
		auto cont = children.ElementAt(c);
		if (!cont.Get())    // don't work with a null pointer
			continue;
		cont->onBeingScrolled(x, y);
	}
	updateComponentLocation();
	

	// CheckScroll();
	return false;
}

/*
* Method: TControl::onBeingScrolled
* Purpose: To be called whenever a parent control is scrolling
* Parameters: int x - the degree to which the control is being moved left or right
*				int y - the degrees to which the control is being moved up or down
*				const RECT& p_snip - the area of the parent control
* Returns: bool - success
*/
bool TControl::onBeingScrolled(float x, float y)
{
	TObjectLocker threadLock(&thread);
	// Update real location of TControl
	location.left += x;
	location.right += x;
	location.bottom += y;
	location.top += y;

	for (int c = 0; c < children.Count(); c++)
	{
		auto cont = children.ElementAt(c);
		if (!cont.Get())    // don't work with a null pointer
			continue;
		cont->onBeingScrolled(x, y);
	}

	updateComponentLocation();
	CheckScroll();
	
	return false;
}

/*
* Method: TControl::AddClass
* Purpose: Labels to serve as id's to draw styles from, useful in determining what HTML/TML element was used in creating the control
* Parameters: TString& t - Class Name to add
* Returns: void
*/
void TControl::AddClass(const TString & t)
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
/*
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

/*
* Method: TControl::SetNormalMouseState
* Purpose: Emergency method to set the Control state to normal
* Parameters: void
* Returns: void
*/
void TControl::SetNormalMouseState()
{
	TObjectLocker threadLock(&thread);
	mState = messageState::normal;
	
}

TrecPointer<TControl> TControl::QueryVideoControl()
{
	TrecPointer<TControl> ret;
	TObjectLocker threadLock(&thread);
	for (UINT Rust = 0; Rust < children.Count(); Rust++)
	{
		auto ch = children.ElementAt(Rust);
		if (!ch.Get())continue;

		ret = ch->QueryVideoControl();
		if (ret.Get())
			break;
	}
	
	return ret;
}

void TControl::QueryMediaControl(TDataArray<TrecPointer<TControl>>& mediaControls)
{
	TObjectLocker threadLock(&thread);
	for (UINT Rust = 0; Rust < children.Count(); Rust++)
	{
		if (children.ElementAt(Rust).Get())
			children.ElementAt(Rust)->QueryMediaControl(mediaControls);
	}
	
}

void TControl::SetUpTextElement()
{
	TextFormattingDetails details;

	TString locale(L"en-us"), font(L"ariel");

	TString text;

	DWRITE_TEXT_ALIGNMENT hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
	DWRITE_PARAGRAPH_ALIGNMENT vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|Caption"));

	TColor fontColor;
	if (valpoint.Get())
	{
		text.Set(*valpoint.Get());
	}

	valpoint = attributes.retrieveEntry(TString(L"|CaptionLocale"));
	if (valpoint.Get())
	{
		locale.Set(valpoint.Get());
	}

	valpoint = attributes.retrieveEntry(TString(L"|Font"));
	if (valpoint.Get())
	{
		font.Set(valpoint.Get());
	}
	valpoint = attributes.retrieveEntry(TString(L"|FontSize"));
	if (valpoint.Get())
	{
		valpoint->ConvertToFloat(details.fontSize);
	}
	valpoint = attributes.retrieveEntry(TString(L"|HorizontalAlignment"));
	if (valpoint.Get())
	{
		hAlign = convertStringToTextAlignment(valpoint.Get());
	}
	valpoint = attributes.retrieveEntry(TString(L"|VerticalAlignment"));
	if (valpoint.Get())
	{
		vAlign = convertStringToParagraphAlignment(valpoint.Get());
	}

	valpoint = attributes.retrieveEntry(TString(L"|FontColor"));
	if (valpoint.Get())
	{
		fontColor.SetColor(convertStringToD2DColor(valpoint.Get()));
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
	if (text.GetSize())
	{
		details.color = drawingBoard->GetBrush(fontColor);

		text1 = TrecPointerKey::GetNewSelfTrecPointer<TTextElement>(drawingBoard);

		text1->SetLocation(location);
		text1->SetBasicFormatting(details);
		text1->SetHorizontallignment(hAlign);
		text1->SetVerticalAlignment(vAlign);
		text1->SetText(text);
	}
}

/**
 * Method: TControl::GetMultiData
 * Purpose: Retireves the Attribute[s] of the Control by the key
 * Parameters: const TString& key - the key to the attribute requested
 * Returns: TDataArray<TString> - the list of values associated with the key
 */
TDataArray<TString> TControl::GetMultiData(const TString& key)
{
	TDataArray<TString> ret;

	TObjectLocker threadLock(&thread);
	TrecPointer<TString> value = attributes.retrieveEntry(key);
	UINT occ = 1;
	while (value.Get())
	{
		ret.push_back(*value.Get());
		value = attributes.retrieveEntry(key, occ++);
	}
	
	return ret;
}

/**
 * Method: TControl::SetScrollControlOnMinSize
 * Purpose: Allows the TControl to set a Scroller Control above it if the bounds becomes too tight 
 * Parameters: D2D1_RECT_F l - the new bounds of the TControl
 * Returns: Whether the ScrollerControl was set or not
 */
bool TControl::SetScrollControlOnMinSize(D2D1_RECT_F l)
{
	TObjectLocker threadLock(&thread);
	bool ret = false;
	if (dimensions)
	{
		bool h = dimensions->minHeight > static_cast<int>(l.bottom - l.top);
		bool w = dimensions->minWidth > static_cast<int>(l.right - l.left);
		if (h || w)
		{
			if (parent.Get() && !parent->IsScroller())
			{
				TrecPointer<TControl> scrollControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TScrollerControl>(drawingBoard, styles);
				scrollControl->onCreate(l, TrecPointer<TWindowEngine>());
				auto oldParent = parent;
				dynamic_cast<TScrollerControl*>(scrollControl.Get())->SetChildControl(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));
				oldParent->SwitchChildControl(tThis, scrollControl);
				ret = true;
			}
		}
	}
	
	return ret;
}

/**
 * Method: TControl::SwitchChildControl
 * Purpose: Allows a child control to insert another control between it and THIS control (usually a scroller control)
 * Parameters: TrecPointerSoft<TControl> curControl - the Control making the call (used to help parent control identify which child to replace)
 *				 TrecPointer<TControl> newControl - the Control to replace it with
 * Returns: void
 */
void TControl::SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl)
{
	TObjectLocker threadLock(&thread);
	for (UINT Rust = 0; Rust < children.Count(); Rust++)
	{
		if (children.ElementAt(Rust).Get() == curControl.Get())
		{
			children.setAt(Rust, newControl);
			newControl->setParent(GetParentReference());
			break;
		}
	}
	
}

/**
 * Method: TControl::GetParentReference
 * Purpose: Sets up a Parent Holder for this Control (used so that no redundant holder object is created)
 * Parameters: void
 * Returns: TrecPointer<TParentHolder> - the holder object referencing this control as a parent control
 */
TrecPointer<TParentHolder> TControl::GetParentReference()
{
	TObjectLocker threadLock(&thread);
	if (!thisParent.Get())
	{
		thisParent = TrecPointerKey::GetNewTrecPointerAlt<TParentHolder, TControlParentHolder>(tThis);
	}
	auto ret = thisParent;
	
	return ret;
}

/*
* Method: TControl::onDraw
* Purpose: Draws the control
* Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
* Returns: void
*/
void TControl::onDraw(TObject* obj)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
	{
		
		return;
	}
	if ((location.bottom - location.top) < 1 || (location.right - location.left) < 1)
	{
		
		return;
	}
	ID2D1Layer* layer = nullptr;

	if (vScroll.Get() || hScroll.Get())
	{
		drawingBoard->AddLayer(location);
	}

	D2D1_MATRIX_3X2_F curTransform;
	drawingBoard->GetTransform(curTransform);

	if (rotation)
	{
		D2D1_POINT_2F point{
			(location.left + location.right) / 2.0f,
			(location.bottom + location.top) / 2.0f
		};

		drawingBoard->SetTransform(curTransform * D2D1::Matrix3x2F::Rotation(rotation, point));
	}


	if (mState == messageState::mouseLClick)
	{
		if (content3.Get())
			content3->onDraw(location);
		else if (content1.Get())
			content1->onDraw(location);
		/*else if (!children.Count() && drawBackground)
			drawingBoard->FillControlBackground(location);*/
		if (border3.Get())
			border3->onDraw(location);
		else if (border1.Get())
			border1->onDraw(location);
		if (text1.Get())
			text1->OnDraw(obj);
	}
	else if (mState == messageState::mouseHover)
	{
		if (content2.Get())
			content2->onDraw(location);
		else if (content1.Get())
			content1->onDraw(location);
		/*else if (!children.Count() && drawBackground)
			drawingBoard->FillControlBackground(location);*/
		if (border2.Get())
			border2->onDraw(location);
		else if (border1.Get())
			border1->onDraw(location);
		if (text1.Get())
			text1->OnDraw(obj);
	}
	else
	{
		if (content1.Get())
			content1->onDraw(location);
		/*else if (!children.Count() && drawBackground)
			drawingBoard->FillControlBackground(location);*/
		if (border1.Get())
			border1->onDraw(location);
		if (text1.Get())
			text1->OnDraw(obj);
	}

	if (vScroll.Get())
		vScroll->onDraw(drawingBoard->GetRenderer().Get());
	if (hScroll.Get())
		hScroll->onDraw(drawingBoard->GetRenderer().Get());

	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->onDraw(obj);
	}

	if (rotation)
	{
		drawingBoard->SetTransform(curTransform);
	}

	if (vScroll.Get() || hScroll.Get())
	{
		drawingBoard->PopLayer();
	}
	layer = nullptr;
	
}

/*
* Method: TControl::getLocation
* Purpose: Retrieves the current physical location on the RenderTarget of the control
* Parameters: void
* Returns: D2D1_RECT_F - the location
*/
D2D1_RECT_F TControl::getLocation()
{
	TObjectLocker threadLock(&thread);
	auto ret = location;
	
	return ret;
}

/*
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

/*
* Method: TControl::getRenderTarget
* Purpose: Retrieves the RenderTarget the Control is currently set to draw to
* Parameters: void
* Returns: TrecComPointer<ID2D1RenderTarget> - Smart Pointer holding the Render Target 
*/
TrecPointer<DrawingBoard> TControl::getDrawingBoard()
{
	TObjectLocker threadLock(&thread);
	auto ret = drawingBoard;
	
	return ret;;
}

/*
* Method: TControl::getParent
* Purpose: Retrieves the parent control of the current control
* Parameters: void
* Returns: TrecPointer<TControl> - Smart Pointer to the Parent Control
*/
TrecPointer<TControl> TControl::getParent()
{
	TObjectLocker threadLock(&thread);
	auto ret = parent.Get() ? parent->GetParent() : TrecPointer<TControl>();
	
	return ret;
}

/*
* Method: TControl::offsetLocation
* Purpose: Moves the Control to the specified point
* Parameters: TPoint cp - the point to move the control to 
* Returns: void
*/
void TControl::offsetLocation(TPoint cp)
{
	TObjectLocker threadLock(&thread);
	int height = location.bottom - location.top;
	int width = location.right - location.left;

	location.top = cp.y;
	location.left = cp.x;
	location.bottom = location.top + height;
	location.right = location.left + width;

	updateComponentLocation();
	
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

	if (text1.Get())
	{
		text1->SetLocation(location);
	}

	if (content1.Get())
		content1->ShiftHorizontal(degrees);
	if (content2.Get())
		content2->ShiftHorizontal(degrees);
	if (content3.Get())
		content3->ShiftHorizontal(degrees);

	if (border1.Get())
		border1->ShiftHorizontal(degrees);
	if (border2.Get())
		border2->ShiftHorizontal(degrees);
	if (border3.Get())
		border3->ShiftHorizontal(degrees);
	
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

	if (text1.Get())
	{
		text1->SetLocation(location);
	}

	if (content1.Get())
		content1->ShiftVertical(degrees);
	if (content2.Get())
		content2->ShiftVertical(degrees);
	if (content3.Get())
		content3->ShiftVertical(degrees);

	if (border1.Get())
		border1->ShiftVertical(degrees);
	if (border2.Get())
		border2->ShiftVertical(degrees);
	if (border3.Get())
		border3->ShiftVertical(degrees);
	
}

/*
* Method: TControl::setMinHeight
* Purpose: Sets the Minimum height this control can be
* Parameters: int h - the minimum height
* Returns: void
*/
void TControl::setMinHeight(int h)
{
	TObjectLocker threadLock(&thread);
	if (!dimensions)
		generateSizeControl();
	dimensions->minHeight = h;
	
}

/*
* Method: TControl::setMaxHeight
* Purpose: Sets the Maximum height this control can be
* Parameters: int h - the maximum height this control can be
* Returns: void
*/
void TControl::setMaxHeight(int h)
{
	TObjectLocker threadLock(&thread);
	if (!dimensions)
		generateSizeControl();
	dimensions->maxHeight = h;
	
}

/*
* Method: TControl::setMinWidth
* Purpose: Sets the minimum width the control can be
* Parameters: int w - the minimum width
* Returns: void
*/
void TControl::setMinWidth(int w)
{
	TObjectLocker threadLock(&thread);
	if (!dimensions)
		generateSizeControl();
	dimensions->minWidth = w;
	
}

/*
* Method: TControl::setMaxWidth
* Purpose: Sets the maximum width the control can be
* Parameters: int w - the max width
* Returns: void
*/
void TControl::setMaxWidth(int w)
{
	TObjectLocker threadLock(&thread);
	if (!dimensions)
		generateSizeControl();
	dimensions->maxWidth = w;
	
}

/*
* Method: TControl::setWidth
* Purpose: Sets the current witdh of the control
* Parameters: int w - the new width
* Returns: void
*/
void TControl::setWidth(int w)
{
	TObjectLocker threadLock(&thread);
	if (!dimensions)
		generateSizeControl();
	dimensions->width = w;
	
}

/*
* Method: TControl::setHeight
* Purpose: Sets the current height of the control
* Parameters: int h - the height to set the control to
* Returns: void
*/
void TControl::setHeight(int h)
{
	TObjectLocker threadLock(&thread);
	if (!dimensions)
		generateSizeControl();
	dimensions->height = h;
	
}

/*
* Method: TControl::addAttribute
* Purpose: Adds a new attribute to use during creation
* Parameters: TString& attr - the attribute name to add
*				TrecPointer<TString> val - the value of that attribute
* Returns: bool - success or error
* Note: Call this method BEFORE calling onCreate. Otherwise, it will have no effect on the Control's
*	appearence or functionality
*/
bool TControl::addAttribute(const TString& attr, TrecPointer<TString> val)
{
	if(!attr.GetSize() || !val.Get())
		return false;
	TObjectLocker threadLock(&thread);
	attributes.addEntry(attr, val);
	
	return true;
}

/*
* Method: TControl::addChild
* Purpose: Adds a child control to the Current Control to hold
* Parameters: TrecPointer<TContainer> tcon - the Child to add
* Returns: bool - success or error
*/
bool TControl::addChild(TrecPointer<TControl> tcon)
{
	if(!tcon.Get())
		return false;
	TObjectLocker threadLock(&thread);
	children.Add(tcon);
	
	tcon->setParent(GetParentReference());
	
	return true;
}

/*
* Method: TControl::setLocation
* Purpose: Sets the Control in a new location
* Parameters: RECT R - the new location
* Returns: void
*/
void TControl::setLocation(D2D1_RECT_F r)
{
	TObjectLocker threadLock(&thread);
	location = r;
	
}

/*
* Method: TControl::setMBottom
* Purpose: Sets the margin on the bottom, likely used by the Builder when visually designing a control
* Parameters: int b - the new bottom margin
* Returns: void
*/
void TControl::setMBottom(int b)
{
	TObjectLocker threadLock(&thread);
	margin.bottom = b;
	marginSet = true;
	
}

/*
* Method: TControl::setMBottom
* Purpose: Sets the margin on the top, likely used by the Builder when visually designing a control
* Parameters: int t - the new top margin
* Returns: void
*/
void TControl::setMTop(int t)
{
	TObjectLocker threadLock(&thread);
	margin.top = t;
	marginSet = true;
	
}

/*
* Method: TControl::setMRight
* Purpose:Sets the margin on the right, likely used by the Builder when visually designing a control
* Parameters: int r - the new right margin
* Returns: void
*/
void TControl::setMRight(int r)
{
	TObjectLocker threadLock(&thread);
	margin.right = r;
	marginSet = true;
	
}

/*
* Method: TControl::setMLeft
* Purpose: Sets the margin on the left, likely used by the Builder when visually designing a control
* Parameters: int l - the new left margin
* Returns: void
*/
void TControl::setMLeft(int l)
{
	TObjectLocker threadLock(&thread);
	margin.left = l;
	marginSet = true;
	
}

/**
 * Method: TControl::RotateDegrees
 * Purpose: Rotates the Control
 * Parameters: float degrees - the angle to rotate the Control by (in degrees)
 * Returns: void
 */
void TControl::RotateDegrees(float degrees)
{
	TObjectLocker threadLock(&thread);
	if (degrees < 0.0f)
		degrees += 360.0f;
	rotation = degrees;
	
}

/**
 * Method: TControl::RotateRadians
 * Purpose: Rotates the Control
 * Parameters: float radians - the angle to rotate the Control by (in degrees)
 * Returns: void
 */
void TControl::RotateRadians(float radians)
{
	TObjectLocker threadLock(&thread);
	RotateDegrees(radians * RADIAN_DEGREE_RATIO);
	
}

/*
* Method: TControl::determineMinHeightNeeded
* Purpose: Determines the minimum height needed, base returns the current height, useful for sub classes
* Parameters: void
* Returns: UINT - the minimum height needed
*/
UINT TControl::determineMinHeightNeeded()
{
	TObjectLocker threadLock(&thread);
	UINT ret = location.bottom - location.top;
	
	return ret;
}

/*
* Method:
* Purpose:
* Parameters:
* Returns:
*/
void TControl::SetNewLocation(const D2D1_RECT_F& r)
{
	TObjectLocker threadLock(&thread);
	location = r;

	if (text1.Get())
	{
		text1->SetLocation(location);
	}
	if (border1.Get())
	{
		border1->loci = r;
	}
	if (border2.Get())
	{
		border2->loci = r;
	}
	if (border3.Get())
	{
		border3->loci = r;
	}
	if (content1.Get())
	{
		content1->location = r;
	}
	if (content2.Get())
	{
		content2->location = r;
	}
	if (content3.Get())
	{
		content3->location = r;
	}
	

}

/*
* Method: TControl::ShrinkHeight
* Purpose: Reduces the height of the control down to what is needed --> just shrinks its children
*		some of whom might find ways to shrink themselves
* Parameters: void
* Returns: void
*/
void TControl::ShrinkHeight()
{
	TObjectLocker threadLock(&thread);
	float bottom = location.top;
	for (UINT c = 0; c < children.Count(); c++)
	{
		if (!children.ElementAt(c).Get())
			continue;
		children.ElementAt(c)->ShrinkHeight();
		if (bottom < children.ElementAt(c)->location.bottom)
			bottom = children.ElementAt(c)->location.bottom;
	}
	
	if (bottom > location.top)
		location.bottom = bottom;
	
}

/*
* Method: TControl::setParent
* Purpose: Sets the controls parent for reference in the tree
* Parameters: TrecPointer<TParentHolder> tcp - the pointer to the parent (whether it is a TControl, a (Tap) Page
* Returns: void
* To-Do: Add code to make sure the parent is not a child or grand-child ( or lower)
*/
void TControl::setParent(TrecPointer<TParentHolder> tcp)
{
	TObjectLocker threadLock(&thread);
	parent = tcp;
	
}

/*
* Method: TControl::getText
* Purpose: Returns the Text Component labeled by the id
* Parameters: int n - the id of the component
* Returns: TrecPointer<TText> - Smart Pointer to the target text component
*/
TrecPointer<TText> TControl::getText(int n)
{
	return TrecPointer<TText>();
	
}

/*
* Method: TControl::getContent
* Purpose: Returns the Content Component labeled by the id
* Parameters: int n - the id of the component
* Returns: const TrecPointer<TContent> - Smart Pointer to the target content component
*/
TrecPointer<TContent> TControl::getContent(int n)
{
	TrecPointer<TContent> ret;
	TObjectLocker threadLock(&thread);
	switch (n)
	{
	case 1:
		ret = content1; break;
	case 2:
		ret = content2; break;
	case 3:
		ret = content3; break;
	}
	
	return ret;
}

/*
* Method: TControl::getBorder
* Purpose: Returns the Border Component labeled by the id
* Parameters: int n - the id of the component
* Returns: const TrecPointer<TBorder> - Smart Pointer to the target Border Component 
*/
TrecPointer<TBorder> TControl::getBorder(int n)
{
	TrecPointer<TBorder> ret;
	TObjectLocker threadLock(&thread);
	switch (n)
	{
	case 1:
		ret = border1; break;
	case 2:
		ret = border2; break;
	case 3:
		ret = border3; break;
	}
	
	return ret;
}

/**
 * Method: TControl::RegisterAnimations
 * Purpose: Allows the Control to report Animations that it has been told to call for
 * Parameters: TDataArray<TrecPointer<AnimationData>>& aData - reference to Animation Data Collection used by the Tap Library to generate Animation for this control
 * Returns: void
 */
void TControl::RegisterAnimations(TDataArray<TrecPointer<AnimationData>>& aData)
{
	TObjectLocker threadLock(&thread);
	for (UINT Rust = 0; Rust < children.Count(); Rust++)
	{
		if (children.ElementAt(Rust).Get())
			children.ElementAt(Rust)->RegisterAnimations(aData);
	}

	for (UINT Rust = 0; Rust < animateData.Size(); Rust++)
	{
		aData.push_back(animateData[Rust]);
	}
	animateData.RemoveAll();
	
}


/*
* Method: TControl::onCreate
* Purpose: Sets up the basic components based off of the available attributes
* Parameters: TMap<TString>* att - list of attributes
*				RECT loc - the location ot use
* Returns: bool - false
*/
bool TControl::onCreate(TMap<TString>* att, D2D1_RECT_F loc)
{
	TrecPointer<TString> valpoint;	// 
	
	valpoint = att->retrieveEntry(TString(L"|BorderThickness"));

	borderThick:
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		valpoint->ConvertToFloat((border1->thickness));         // Logic Bug needs fixing
	}
	/*else
	{
		valpoint = att->retrieveEntry(TString(L""));
		goto borderThick;
	}*/

	// 
	valpoint = att->retrieveEntry(TString(L"|BorderColor"));
	borderColor_:
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		border1->stopCollection.AddGradient(TGradientStop(TColor(convertStringToD2DColor(valpoint.Get())), 0.0f));
	}
	/*else
	{
		valpoint = att->retrieveEntry(TString(L""));
		goto borderColor_;
	}*/

	// 
	valpoint = att->retrieveEntry(TString(L"|ContentThickness"));
	contThick:
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		valpoint->ConvertToFloat((content1->thickness));
	}
	/*else
	{
		valpoint = att->retrieveEntry(TString(L""));
		goto contThick;
	}*/

	// 
	valpoint = att->retrieveEntry(TString(L"|ContentColor"));
	contColor:
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		content1->stopCollection.AddGradient(TGradientStop(TColor(convertStringToD2DColor(valpoint.Get())), 0.0f));
	}



	// 


	// 


	// 






	valpoint = att->retrieveEntry(TString(L"|ContentGrad"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		float entry = 0.0f;
		valpoint->ConvertToFloat(entry);
		//content1->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = content1->stopCollection.GetGradientCount())
			content1->stopCollection.SetPositionAt(entry, gradCount - 1);
	}
	

	valpoint = att->retrieveEntry(TString(L"|BorderGrad"));
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		float entry = 0.0f;
		valpoint->ConvertToFloat(entry);
		//border1->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = border1->stopCollection.GetGradientCount())
			border1->stopCollection.SetPositionAt(entry, gradCount - 1);
	}


	valpoint = att->retrieveEntry(TString(L"|ContentGradMode"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		if (valpoint->Compare(L"Radial"))
			content1->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|BorderGradMode"));
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		if (valpoint->Compare(L"Radial"))
			border1->useRadial = true;
	}

	valpoint = att->retrieveEntry(TString(L"|ImageSource"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		valpoint->Replace(L"/", L"\\");
		int res = generateImage(content1, valpoint, loc);
	}

	if (border1.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			border1->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			border1->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			border1->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|BorderAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = border1->GetBrush();

			animateData.push_back(animate);
		}
	}

	bool callOnGif = false;

	if (content1.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			content1->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			content1->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			content1->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|ContentAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = content1->GetBrush();

			animateData.push_back(animate);
		}

		if (content1->GetImageCount() > 1)
			callOnGif = true;
	}
	if (text1.Get())
	{
		TDataArray<TString> animators = GetMultiData(TString(L"|TextAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = text1->GetBrush();

			animateData.push_back(animate);
		}
	}
	if (callOnGif)
	{
		TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
		animate->control = tThis;
		animate->brush = TrecPointerKey::GetTrecPointerFromSub<TBrush, TBitmapBrush>(content1->image);
		animate->name.Set(L"Gif");
		animate->storyName.Set(L"GifRunner");

		animateData.push_back(animate);
	}
	return false;
}

/*
* Method: TControl::onCreate2
* Purpose: Sets up themessageState::mouse hover components based off of the available attributes
* Parameters: TMap<TString>* att - list of attributes
*				RECT loc - the location ot use
* Returns: bool - false
*/
bool TControl::onCreate2(TMap<TString>* att, D2D1_RECT_F loc)
{
	TrecPointer<TString> valpoint;	// 

	valpoint = att->retrieveEntry(TString(L"|HoverBorderThickness"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		valpoint->ConvertToFloat((border2->thickness));         // Logic Bug needs fixing
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverBorderColor"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		border2->stopCollection.AddGradient(TGradientStop(TColor(convertStringToD2DColor(valpoint.Get())), 0.0f));
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverContentThickness"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		valpoint->ConvertToFloat((content2->thickness));
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverContentColor"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		content2->stopCollection.AddGradient(TGradientStop(TColor(convertStringToD2DColor(valpoint.Get())), 0.0f));
	}



	valpoint = att->retrieveEntry(TString(L"|HoverContentGrad"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(entry);
		//content2->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = content2->stopCollection.GetGradientCount())
			content2->stopCollection.SetPositionAt(entry, gradCount - 1);
	}


	valpoint = att->retrieveEntry(TString(L"|HoverBorderGrad"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(entry);
		//border2->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = border2->stopCollection.GetGradientCount())
			border2->stopCollection.SetPositionAt(entry, gradCount - 1);
	}

	valpoint = att->retrieveEntry(TString(L"|HoverContentGradMode"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		if (valpoint->Compare(L"Radial"))
			content2->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|HoverBorderGradMode"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		if (valpoint->Compare(L"Radial"))
			border2->useRadial = true;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverImageSource"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		generateImage(content2, valpoint, loc);
	}


	if (border2.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			border2->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			border2->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			border2->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|HoverBorderAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = border2->GetBrush();

			animateData.push_back(animate);
		}
	}
	if (content2.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			content2->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			content2->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			content2->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:

			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|HoverContentAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = content2->GetBrush();

			animateData.push_back(animate);
		}

	}

	return false;
}

/*
* Method: TControl::onCreate3
* Purpose: Sets up the click event components based off of the available attributes
* Parameters: TMap<TString>* att - list of attributes
*				RECT loc - the location ot use
* Returns: bool - false
*/
bool TControl::onCreate3(TMap<TString>* att, D2D1_RECT_F loc)
{
	TrecPointer<TString> valpoint;	// 

	valpoint = att->retrieveEntry(TString(L"|ClickBorderThickness"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		valpoint->ConvertToFloat((border3->thickness));         // Logic Bug needs fixing
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickBorderColor"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		border3->stopCollection.AddGradient(TGradientStop(TColor(convertStringToD2DColor(valpoint.Get())), 0.0f));
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickContentThickness"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		valpoint->ConvertToFloat((content3->thickness));
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickContentColor"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		content3->stopCollection.AddGradient(TGradientStop(TColor(convertStringToD2DColor(valpoint.Get())), 0.0f));
	}



	valpoint = att->retrieveEntry(TString(L"|ClickContentGrad"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(entry);
		//content3->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = content3->stopCollection.GetGradientCount())
			content3->stopCollection.SetPositionAt(entry, gradCount - 1);
	}


	valpoint = att->retrieveEntry(TString(L"|ClickBorderGrad"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(entry);
		//border3->secondColor = true;
		UINT gradCount = 0;
		if (gradCount = border3->stopCollection.GetGradientCount())
			border3->stopCollection.SetPositionAt(entry, gradCount - 1);
	}


	valpoint = att->retrieveEntry(TString(L"|ClickContentGradMode"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		if (valpoint->Compare(L"Radial"))
			content3->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|ClickBorderGradMode"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(drawingBoard, (this));
		}
		if (valpoint->Compare(L"Radial"))
			border3->useRadial = true;
	}


	valpoint = att->retrieveEntry(TString(L"|ClickImageSource"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, (this));
		}
		generateImage(content3, valpoint,loc);
	}

	if (border3.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			border3->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			border3->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			border3->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|ClickBorderAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = border3->GetBrush();

			animateData.push_back(animate);
		}
	}
	if (content3.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			content3->onCreate(location);
			break;
		case TShape::T_Rounded_Rect:
			content3->onCreate(roundedRect);
			break;
		case TShape::T_Ellipse:
			content3->onCreate(ellipse);
			break;
		case TShape::T_Custom_shape:
			break;
		}

		TDataArray<TString> animators = GetMultiData(TString(L"|ClickContentAnimation"));
		for (UINT Rust = 0; Rust < animators.Size(); Rust++)
		{
			TString name = animators[Rust];
			auto names = name.split(TString(L";"));
			if (!names->Size())
				continue;
			TrecPointer<AnimationData> animate = TrecPointerKey::GetNewTrecPointer<AnimationData>();
			animate->control = tThis;
			animate->name = names->at(0);
			if (names->Size() > 1)
				animate->storyName = names->at(1);
			animate->brush = content3->GetBrush();

			animateData.push_back(animate);
		}

	}
	return false;
}

bool TControl::onCreateClassAndId(D2D1_RECT_F& contain)
{
	TObjectLocker threadLock(&thread);
	auto valpoint = attributes.retrieveEntry(TString(L"|ArrayID"));
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(arrayID);
	}

	valpoint = attributes.retrieveEntry(TString(L"|Shape"));
	if (valpoint.Get())
	{
		if (valpoint->Compare(L"Ellipse"))
		{
			shape = TShape::T_Ellipse;
			ellipse.point = D2D1::Point2F((location.right + location.left) / 2, (location.top + location.bottom) / 2);
			ellipse.radiusX = (location.right - location.left) / 2;
			ellipse.radiusY = (location.bottom - location.top) / 2;
		}
		else if (valpoint->Compare(L"RoundedRectangle"))
		{
			shape = TShape::T_Rounded_Rect;
			float xRound = (location.left - location.right) / 10;
			float yRound = (location.bottom - location.top) / 10;
			valpoint = attributes.retrieveEntry(TString(L"|RoundedRectX"));
			if (valpoint.Get())
			{
				valpoint->ConvertToFloat(xRound);
			}
			valpoint = attributes.retrieveEntry(TString(L"|RoundedRectY"));
			if (valpoint.Get())
			{
				valpoint->ConvertToFloat(yRound);
			}
			roundedRect.rect = location;
			roundedRect.radiusX = xRound;
			roundedRect.radiusY = yRound;
		}
		else if (valpoint->Compare(L"Custom"))
		{

		}
	}


	int occ1 = 0;
	valpoint = attributes.retrieveEntry(TString(L"|Class"), occ1++);
	while (valpoint.Get())
	{
		className += *valpoint.Get();
		className.Trim();
		className += L";";
		valpoint = attributes.retrieveEntry(TString(L"|Class"), occ1++);
	}
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
							onCreate(&classy->names, contain);
						//break;
					}
				}
			}
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|id"));
	if (valpoint.Get())
	{
		ID = *valpoint.Get();
	}
	else
	{
		valpoint = attributes.retrieveEntry(TString(L"|ID"));
		if (valpoint.Get())
		{
			ID = *valpoint.Get();
		}
	}
	

	return false;
}

/*
* Method: TControl::updateComponentLocation
* Purpose: Update the locations of the control's components - call whenever the control's location is updated
* Parameters: void
* Returns: void
*/
void TControl::updateComponentLocation()
{
	TObjectLocker threadLock(&thread);
	if (border1.Get())
	{
		border1->loci = location;
	}
	if (border2.Get())
	{
		border2->loci = location;
	}
	if (border3.Get())
	{
		border3->loci = location;
	}

	// Content
	if (text1.Get())
	{
		text1->SetLocation(location);
	}

	// Content
	if (content1.Get())
	{
		content1->location = location;
	}
	if (content2.Get())
	{
		content2->location = location;
	}
	if (content3.Get())
	{
		content3->location = location;
	}
	

}

/**
 * Method: TControl::CheckScroll
 * Purpose: Makes sure that the Control doesn't need scrollers or, if it needs them, then the bars are properly configured
 * Parameters: void
 * Returns: void
 */
void TControl::CheckScroll()
{
	TObjectLocker threadLock(&thread);
	D2D1_RECT_F area = location;

	bool needV = false, needH = false;

	for (UINT c = 0; c < children.Count(); c++)
	{
		auto con = children.ElementAt(c).Get();
		if (!con) continue;

		D2D1_RECT_F cLoc = con->getLocation();

		if (cLoc.bottom > area.bottom)
		{
			area.bottom = cLoc.bottom;
			needV = true;
		}
		if (cLoc.top < area.top)
		{
			needV = true;
			area.top = cLoc.top;
		}
		if (cLoc.right > area.right)
		{
			area.right = cLoc.right;
			needH = true;
		}
		if (cLoc.left < area.left)
		{
			area.left = cLoc.left;
			needH = true;
		}
	}

	if (needV)
	{
		if (!vScroll.Get())
			vScroll = TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this,ScrollOrient::so_vertical);
		vScroll->Refresh(location, area);
	}
	else if (vScroll.Get())
	{
		vScroll.Delete();
	}

	if (needH)
	{
		if (!hScroll.Get())
			hScroll = TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this,ScrollOrient::so_horizontal);
		hScroll->Refresh(location, area);
	}
	else if (hScroll.Get())
	{
		hScroll.Delete();
	}
	

}


/*
* Method: TControl::generateImage
* Purpose: Sets up an image for the Control if an attribute calls for it
* Parameters: TrecPointer<TContent> tcon - the Content component to attach the image to
*				TrecPointer<TString> p - the file name of the target image
*				RECT loc - the location where the image should be
* Returns: int - error (0 if successful)
*/
int TControl::generateImage(TrecPointer<TContent> tcon, TrecPointer<TString> p, D2D1_RECT_F& loc)
{
	if (!tcon.Get() || !(p.Get()))
		return 1;

	TrecPointer<TFileShell> file = TFileShell::GetFileInfo(*p.Get());

	if (file.Get())
	{	
		TObjectLocker threadLock(&thread);
		tcon->image = drawingBoard->GetBrush(file, loc);
		
		return tcon->image.Get() ? 0 : 2;
	}

	return 3;
}

/*
* Method: TControl::checkMargin
* Purpose: Sets up the margin attribute if available, called by onCreate
* Parameters: RECT contain - the location given to onCreate
* Returns: void
*/
void TControl::checkMargin(D2D1_RECT_F contain)
{
	TObjectLocker threadLock(&thread);
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|Margin"));
	//margin = RECT{ 0,0,0,0 };
	if (valpoint.Get())
	{
		margin = convertStringToD2D1Rect(valpoint.Get());
		marginSet = true;
	}
	
	if (location.bottom == 0 && location.right == 0)
	{
		location.bottom = contain.bottom - margin.bottom;
		location.left = contain.left + margin.left;
		location.top = contain.top + margin.top;
		location.right = contain.right - margin.right;
	}
	else
	{
		location.bottom = location.bottom - margin.bottom;
		location.left = location.left + margin.left;
		location.right = location.right - margin.right;
		location.top = location.top + margin.top;
	}
	
}

/*
* Method: TControl::checkHeightWidth
* Purpose: checks for the height and width attributes of the control, called by onCreate
* Parameters: RECT r the location given to onCreate
* Returns: void
*/
void TControl::checkHeightWidth(D2D1_RECT_F r)
{
	TObjectLocker threadLock(&thread);
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|Height"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt((dimensions->height)))
			dimensions->height = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|Width"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt((dimensions->width)))
			dimensions->width = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MaxHeight"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt((dimensions->maxHeight)))
			dimensions->maxHeight = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MaxWidth"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt((dimensions->maxWidth)))
			dimensions->maxWidth = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MinHeight"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt((dimensions->minHeight)))
			dimensions->minHeight = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MinWidth"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt((dimensions->minWidth)))
			dimensions->minWidth = 0;
	}
	if (dimensions)
	{
		if (dimensions->height > r.bottom - r.top && !vScroll.Get())
			dimensions->height = r.bottom - r.top;
		if (dimensions->width > r.right - r.left && !hScroll.Get())
			dimensions->width = r.right - r.left;
	}
	
}

/*
* Method: TControl::generateSizeControl
* Purpose: Sets up the heights and widths of the control (stored as pointer so controls that
*		don't have this attribute can save memory this way
* Parameters: void
* Returns: void
*/
void TControl::generateSizeControl()
{
	TObjectLocker threadLock(&thread);
	if (!dimensions)
	{
		dimensions = new sizeControl;

		// only do if dimensions is new, otherwise, we risk overriding previously set values
		dimensions->height = 0;
		dimensions->maxHeight = 0;
		dimensions->minHeight = 0;
		dimensions->width = 0;
		dimensions->maxWidth = 0;
		dimensions->minWidth = 0;
	}
	
}

/*
* Method: TControl::hasEvent
* Purpose: Checks to see whether the control is set to respond to a certain event with a handler
* Parameters: R_Message_Type type - type of message to respond to
* Returns: bool - presense of type handler
*/
bool TControl::hasEvent(R_Message_Type type)
{
	TObjectLocker threadLock(&thread);
	for (int c = 0; c < eventList.Size(); c++)
	{
		if ((eventList)[c].eventType == type)
		{
			
			return true;
		}
	}
	
	return false;
}

/*
* Method: TControl::resetArgs
* Purpose: resets the Arguements handlers have to their default values
* Parameters: void
* Returns: void
*/
void TControl::resetArgs()
{
	TObjectLocker threadLock(&thread);
	args.control = this;
	args.isClick = false;
	args.isLeftClick = false;
	args.methodID = -1;
	args.point.x = 0.0f;
	args.point.y = 0.0f;
	args.positive = false;
	args.text.Empty();
	args.type = L'\0';
	args.object.Nullify();
	
}

/*
* Method: TControl::getEventID
* Purpose: Controls never touch their handlers. Instead, they're given an index to a collection of handlers
* Parameters: R_Message_Type type -  the type of message to respond to
* Returns: int - index of handler to call (-1 if handler not provided)
*/
int TControl::getEventID(R_Message_Type type)
{
	TObjectLocker threadLock(&thread);
	for (int c = 0; c < eventList.Size(); c++)
	{
		if (eventList[c].eventType == type)
		{
			int ret = eventList.at(c).eventID;
			
			return ret;
		}
	}
	
	return -1;
}

/*
* Method: TControl::SetToRenderTarget
* Purpose: Updates each component to the controls current DrawingBoard, call if DrwingBoard is ever switched
* Parameters: void
* Returns: void
*/
void TControl::SetToRenderTarget()
{
	TObjectLocker threadLock(&thread);
	if (border1.Get())
		border1->drawingBoard = drawingBoard;
	if (border2.Get())
		border2->drawingBoard = drawingBoard;
	if (border3.Get())
		border3->drawingBoard = drawingBoard;

	if (content1.Get())
		content1->drawingBoard = drawingBoard;
	if (content2.Get())
		content2->drawingBoard = drawingBoard;
	if (content3.Get())
		content3->drawingBoard = drawingBoard;

	if (text1.Get())
	{
		// Text Component has no means of replacing Drawing Board, create new Text Component
		text1.Nullify();
		SetUpTextElement();
	}
	
}

/*
* Method: TControl::addEventID
* Purpose: Adds the Event Type with a handler index to associate it with
* Parameters: R_Message_Type rmt -  message type
*				int e_id - the idex of the message handler that goes with it
* Returns: void
*/
void TControl::addEventID(R_Message_Type rmt, int e_id)
{
	TObjectLocker threadLock(&thread);
	EventTypeID et;
	et.eventID = e_id;
	et.eventType = rmt;
	eventList.push_back(et);
	
}


// Messages TControls can expect to get from the Central Program (or when they are part of a running TAP

/*
* Method: TControl::OnRButtonUp
* Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
	{
		
		return;
	}
	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->OnRButtonUp(nFlags, point, mOut,eventAr);
	}
	if (!isContained(point, getLocation()))
	{
		mState = messageState::normal;
		isRClick = false;
		
		return;
	}

	if (hasEvent(R_Message_Type::On_R_Button_Up))
	{
		// Set args
		resetArgs();
		args.eventType = R_Message_Type::On_R_Button_Up;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_R_Button_Up);
		args.isClick = true;
		args.isLeftClick = false;
		args.control = this;

		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_R_Button_Up, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
	}

	if (hasEvent(R_Message_Type::On_Right_Click) && isRClick)
	{
		// Set args
		resetArgs();
		args.eventType = R_Message_Type::On_Right_Click;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_Right_Click);
		args.isClick = true;
		args.isLeftClick = false;
		args.control = this;

		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_Right_Click, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
	}

	isRClick = false;

	if (*mOut == messageOutput::positiveContinue)
	{
		if (mState ==messageState::mouseRClick)
		{
			*mOut = messageOutput::positiveContinueUpdate;
			if (hasEvent(R_Message_Type::On_Right_Release))
			{
				// Set args
				resetArgs();
				args.eventType = R_Message_Type::On_Right_Release;
				args.point = point;
				args.methodID = getEventID(R_Message_Type::On_Right_Release);
				args.isClick = true;
				args.isLeftClick = false;
				args.control = this;

				eventAr.push_back(TPage::EventID_Cred( R_Message_Type::On_Right_Release, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
			}
		}
	}
	mState =messageState::mouseHover;
	
}

/*
* Method: TControl::OnLButtonDown
* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*/
afx_msg void TControl::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
	{
		
		return;
	}
	if (vScroll.Get() && vScroll->OnLButtonDown(nFlags, point, mOut))
	{
		*mOut = messageOutput::positiveScroll;
		resetArgs();
		args.eventType = R_Message_Type::On_Select_Scroller;
		args.point = point;
		args.methodID = -1;
		args.isClick = args.isLeftClick = true;
		args.control = nullptr;;
		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_Click, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis), vScroll));
		
		return;
	}


	if (hScroll.Get() && hScroll->OnLButtonDown(nFlags, point, mOut))
	{
		*mOut = messageOutput::positiveScroll;
		resetArgs();
		args.eventType = R_Message_Type::On_Select_Scroller;
		args.point = point;
		args.methodID = -1;
		args.isClick = args.isLeftClick = true;
		args.control = nullptr;

		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_Click, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis), hScroll));
	}

	if (!isContained(point, getLocation()))
	{
		if (mState != messageState::normal)
		{
			mState = messageState::normal;
			*mOut = messageOutput::negativeUpdate;
		}
		else
			*mOut = messageOutput::negative;
		
		return;
	}

	isLClick = true;
	if (text1.Get())
	{
		text1->OnCLickDown(point);
	}

	for (int c = 0; c < children.Count();c++)
	{
		if (children.ElementAt(c).Get())
			children.ElementAt(c)->OnLButtonDown(nFlags, point, mOut,eventAr, clickedControls);
	}

	if (*mOut == messageOutput::positiveOverride || *mOut == messageOutput::positiveOverrideUpdate)
	{
		mState = messageState::normal;
		
		return;
	}

	if (mState !=messageState::mouseLClick)
	{
		if (overrideParent)
			*mOut = messageOutput::positiveOverrideUpdate;
		else
			*mOut = messageOutput::positiveContinueUpdate;
	}
	else if (*mOut != messageOutput::positiveContinueUpdate && *mOut != messageOutput::positiveOverrideUpdate && *mOut != messageOutput::negativeUpdate)
	{
		if (overrideParent)
			*mOut = messageOutput::positiveOverride;
		else
			*mOut = messageOutput::positiveContinue;
	}
	mState =messageState::mouseLClick;
	clickedControls.push_back(this);
	
	if (hasEvent(R_Message_Type::On_L_Button_Down))
	{
		// Set args
		resetArgs();
		args.eventType = R_Message_Type::On_L_Button_Down;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_L_Button_Down);
		args.isClick = true;
		args.isLeftClick = true;
		args.control = this;

		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_L_Button_Down, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
	}

}

/*
* Method: TControl::OnRButtonDown
* Purpose: Allows Control to catch the RightmessageState::mouse button down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*/
afx_msg void TControl::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
		return;


	if (!isContained(point, getLocation()))
	{
		if (mState != messageState::normal)
		{
			mState = messageState::normal;
			*mOut = messageOutput::negativeUpdate;
		}
		else
			*mOut = messageOutput::negative;
		
		return;
	}
	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->OnRButtonDown(nFlags, point, mOut, eventAr, clickedControls);
		if (*mOut == messageOutput::negative)
			continue;
		if (*mOut == messageOutput::positiveOverride || *mOut == messageOutput::positiveOverrideUpdate)
		{
			
			return;
		}
	}
	if (*mOut == messageOutput::positiveContinue || *mOut == messageOutput::positiveContinueUpdate)
	{
		if (mState !=messageState::mouseRClick)
			*mOut = messageOutput::positiveContinueUpdate;
		
	}
	mState =messageState::mouseRClick;

	if (hasEvent(R_Message_Type::On_R_Button_Down))
	{	
		// Set args
		resetArgs();
		args.eventType = R_Message_Type::On_R_Button_Down;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_R_Button_Down);
		args.isClick = true;
		args.isLeftClick = false;
		args.control = this;
		eventAr.push_back(TPage::EventID_Cred( R_Message_Type::On_R_Button_Down, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
	}
	isRClick = true;
	
}

/*
* Method: TControl::OnMouseMove
* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*/
afx_msg void TControl::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
	{
		
		return;
	}
	if (!isContained(point, getLocation()))
	{
		if (mState != messageState::normal)
		{
			*mOut = messageOutput::negativeUpdate;
		}
		else
			*mOut = messageOutput::negative;

		if (isMouseFocus)
		{
			isMouseFocus = false;

			if (hasEvent(R_Message_Type::On_Lose_Focus))
			{
				// Set args
				resetArgs();
				args.eventType = R_Message_Type::On_Lose_Focus;
				args.point = point;
				args.methodID = getEventID(R_Message_Type::On_Lose_Focus);
				args.isClick = false;
				args.isLeftClick = false;
				args.control = this;
				eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_Lose_Focus, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
			}

			for (UINT Rust = 0; Rust < clickedControls.Size(); Rust++)
			{
				if (clickedControls[Rust] == this)
				{
					clickedControls.RemoveAt(Rust);
					break;
				}
			}
		}

		
		return;
	}

	*mOut = messageOutput::positiveContinueUpdate;

	for (int c = 0; c < children.Count();c++)
	{
		if (children.ElementAt(c).Get())
			children.ElementAt(c)->OnMouseMove(nFlags, point, mOut, eventAr, clickedControls);
	}

	if (*mOut == messageOutput::positiveOverride || *mOut == messageOutput::positiveOverrideUpdate)
	{
		clickedControls.push_back(this);
		
		return;
	}

	if (mState !=messageState::mouseHover)
	{
		if (overrideParent)
			*mOut = messageOutput::positiveOverrideUpdate;
		else
			*mOut = messageOutput::positiveContinueUpdate;
	}
	else if (*mOut != messageOutput::positiveContinueUpdate && *mOut != messageOutput::positiveOverrideUpdate && *mOut != messageOutput::negativeUpdate)
	{
		if (overrideParent)
			*mOut = messageOutput::positiveOverride;
		else
			*mOut = messageOutput::positiveContinue;
	}
	if(mState !=messageState::mouseLClick)
		mState =messageState::mouseHover;

	if (!isMouseFocus)
	{
		isMouseFocus = true;

		if (hasEvent(R_Message_Type::On_Hover))
		{
			// Set args
			resetArgs();
			args.eventType = R_Message_Type::On_Hover;
			args.point = point;
			args.methodID = getEventID(R_Message_Type::On_Hover);
			args.control = this;
			eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_Hover, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
		}

		
	}
	clickedControls.push_back(this);
	
}



/**
 * Method: TControl::OnMouseLeave
 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
 * Returns: void
 */
bool TControl::OnMouseLeave(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr)
{
	TObjectLocker threadLock(&thread);
	bool ret = false;
	if (!isContained(point, location))
	{
		mState = messageState::normal;

		resetArgs();
		args.eventType = R_Message_Type::On_Hover_Leave;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_Hover_Leave);
		args.isClick = true;
		args.isLeftClick = false;
		args.control = this;
		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_Hover_Leave, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
		ret = true;
	}
	
	return ret;
}

/*
* Method: TControl::OnLButtonDblClk
* Purpose: Allows control to catch the DOuble Click event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
void TControl::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
	{
		
		return;
	}

	if (!isContained(point, getLocation()))
	{
		if (mState != messageState::normal)
		{
			mState = messageState::normal;
			*mOut = messageOutput::negativeUpdate;
		}
		else
			*mOut = messageOutput::negative;
		
		return;
	}
	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->OnLButtonDblClk(nFlags, point, mOut, eventAr);
		if (*mOut == messageOutput::negative)
			continue;
		if (*mOut == messageOutput::positiveOverride || *mOut == messageOutput::positiveOverrideUpdate)
		{
			
			return;
		}
	}
	if (*mOut == messageOutput::positiveContinue || *mOut == messageOutput::positiveContinueUpdate)
	{
		if (mState != messageState::mouseRClick)
			*mOut = messageOutput::positiveContinueUpdate;

	}
	

	if (hasEvent(R_Message_Type::On_LDoubleClick))
	{
		// Set args
		resetArgs();
		args.eventType = R_Message_Type::On_LDoubleClick;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_LDoubleClick);
		args.isClick = true;
		args.isLeftClick = false;
		args.control = this;
		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_LDoubleClick, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
	}
	
}

/*
* Method: TControl::OnLButtonUp
* Purpose: Allows control to catch the Left Button Up event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
	{
		
		return;
	}

	for (int c = 0; c < children.Count(); c++)
	{
		TControl* tc_temp = children.ElementAt(c).Get();
		if(tc_temp)
		tc_temp->OnLButtonUp(nFlags, point, mOut,eventAr);
		if (*mOut != messageOutput::negative && *mOut != messageOutput::negativeUpdate)
			break;
	}
	if (!isContained(point, getLocation()))
	{
		mState = messageState::normal;
		*mOut = messageOutput::negative;
		isLClick = false;
		
		return;
	}

	*mOut = messageOutput::positiveContinue;
	if (mState ==messageState::mouseLClick)
		*mOut = messageOutput::positiveContinueUpdate;

	mState =messageState::mouseHover;

	if (hasEvent(R_Message_Type::On_L_Button_Up))
	{
		// Set args
		resetArgs();
		args.eventType = R_Message_Type::On_L_Button_Up;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_L_Button_Up);
		args.isClick = true;
		args.isLeftClick = true;
		args.control = this;
		eventAr.push_back(TPage::EventID_Cred(R_Message_Type::On_Click, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
	}

	if (hasEvent(R_Message_Type::On_Click) || isLClick)
	{
		// Set args
		resetArgs();
		args.eventType = R_Message_Type::On_Click;
		args.point = point;
		args.methodID = getEventID(R_Message_Type::On_Click);
		args.isClick = true;
		args.isLeftClick = true;
		args.control = this;

		TPage::EventID_Cred cred(R_Message_Type::On_L_Button_Up, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));

		if (text1.Get())
			cred.textIntercepter = text1->GetTextInterceptor();

		eventAr.push_back(cred);

		
	}

	if (text1.Get())
	{
		text1->OnCLickUp(point);

	}

	isLClick = false;
	
}

/*
* Method: TControl::OnChar
* Purpose: Allows Controls to repond to character input
* Parameters: bool fromChar - can be called either from on Key Down or OnChar
*				UINT nChar - The ID of the character that was pressed
*				UINT nRepCnt - how many times the character was processed for this event
*				UINT nFlags - flags provided by MFC's Message system, not used
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<TPage::EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg bool TControl::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<TPage::EventID_Cred>& eventAr)
{
	TObjectLocker threadLock(&thread);
	if (!isActive)
	{
		
		return false;
	}
	TControl* tcCall = NULL;
	for (int c = 0; c < children.Count(); c++)
	{
		if (children.ElementAt(c).Get())
			tcCall = children.ElementAt(c).Get();

		if (tcCall && tcCall->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr))
		{
			if (hasEvent(R_Message_Type::On_Char))
			{
				resetArgs();
				args.eventType = R_Message_Type::On_Char;
				args.control = this;
				args.methodID = getEventID(R_Message_Type::On_Char);
				args.type = static_cast<WCHAR>(LOWORD(nChar));
				args.control = this;
				eventAr.push_back(TPage::EventID_Cred( R_Message_Type::On_Char, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
			}
			
			return true;
		}
	}
	
	return false;
}

/**
 * Method: TControl::OnScroll
 * Purpose: Allows Controls to handle Scrolling messages from Windows
 * Parameters: const TPoint& point - where the mouse is
 *				const TPoint& direction - indication of which direction it is going in
 * Returns: bool - true of a control actually used the message (signal to stop propagating)
 *
 * Attributes: virtual
 */
bool TControl::OnScroll(const TPoint& point, const TPoint& direction)
{
	if (!isContained(point, location))
		return false;

	bool found = false;
	for (UINT Rust = 0; Rust < children.Count() && !found; Rust++)
	{
		auto ch = children.ElementAt(Rust);
		if (!ch.Get())
		{
			children.RemoveAt(Rust--);
			continue;
		}
		found = ch->OnScroll(point, direction);
	}
	return found;
}

// Messages TControls can get while under construction in the Builder

/*
* Method: TControl::Builder_OnLButtonUp
* Purpose: Allows special calls from the Builder to know when the control is no longer being dragged
* Parameters: UINT flags - Flags provided by MFC, not used
*				TPoint point - the Point where event occured
*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
* Returns: void 
*/
afx_msg void TControl::Builder_OnLButtonUp(UINT flags, TPoint point, TControl** mOut)
{
	TObjectLocker threadLock(&thread);
	for (int c = 0; c < children.Count();c++)
	{
		if (!children.ElementAt(c).Get())
			continue;
		TControl* tc = children.ElementAt(c).Get();
		tc->Builder_OnLButtonUp(flags, point, mOut);
	}
	rightBorder = leftBorder = topBorder = bottomBorder = onClickFocus = false;
	

}

/*
* Method: TControl::Builder_OnLButtonDown
* Purpose: Allows Controls to tell if a use of the Builder is planning to move them around
* Parameters: UINT flags - Flags provided by MFC, not used
*				TPoint point - the Point where event occured
*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
*				messageOutput* o - Allows control to keep track of how other controls respond to the call
* Returns: void
*/
afx_msg void TControl::Builder_OnLButtonDown(UINT flags, TPoint point, TControl** mOut, messageOutput* o)
{
	TObjectLocker threadLock(&thread);
	for (int c = 0; c < children.Count();c++)
	{
		if (!children.ElementAt(c).Get()|| !children.ElementAt(c).Get())
			continue;
		TControl* tc = children.ElementAt(c).Get();
		tc->Builder_OnLButtonDown(flags, point, mOut, o);
		if (*o != messageOutput::negative && *o != messageOutput::negativeUpdate)
		{
			onFocus = onClickFocus = leftBorder = rightBorder = topBorder = bottomBorder = false; // other focus flags rendered false by LButtonUp method
			
			return;
		}
	}
	
	if (isContained(&point, &location))
	{
		*o = messageOutput::positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		*mOut = (this);
	}
	else
	{
		onClickFocus = onFocus = leftBorder = rightBorder = topBorder = bottomBorder = false;
		*o = messageOutput::negativeUpdate;
	}
	if (abs((double)point.x - (double)location.left) < 5)
	{
		*o = messageOutput::positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		leftBorder = true;
		rightBorder = false;
		topBorder = false;
		bottomBorder = false;
		
		return;
	}
	if (abs((double)point.x - (double)location.right) < 5)
	{
		*o = messageOutput::positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		rightBorder = true;
		leftBorder = false;
		topBorder = false;
		bottomBorder = false;
		
		return;
	}
	if (abs((double)point.y - (double)location.top) < 5)
	{
		*o = messageOutput::positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		rightBorder = false;
		leftBorder = false;
		topBorder = true;
		bottomBorder = false;
		
		return;
	}
	if (abs((double)point.y - (double)location.bottom) < 5)
	{
		*o = messageOutput::positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		rightBorder = false;
		leftBorder = false;
		topBorder = false;
		bottomBorder = true;
		
		return;
	}
	
}

/*
* Method: TControl::Builder_OnMouseMove
* Purpose: Allows controls to know when they are being moved
* Parameters: UINT flags - Flags provided by MFC, not used
*				TPoint point - the Point where event occured
*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
*				const RECT& bounds - bounds of the parent
*				messageOutput* o - Allows control to keep track of how other controls respond to the call
* Returns: void
*/
afx_msg void TControl::Builder_OnMouseMove(UINT flags, TPoint cp, TControl** mOut, const RECT& bounds, messageOutput* o)
{
	TObjectLocker threadLock(&thread);
	if (!onClickFocus)
	{
		TControl* point;
		for (int c = 0; c < children.Count(); c++)
		{
			point = children.ElementAt(c).Get();
			if (point)
			{
				//const TControlHolder** hold = &PointerCase;
				//point->Builder_OnMouseMove(flags, cp, mOut, location, o);
			}
			if (*o == messageOutput::negative || *o == messageOutput::negativeUpdate) // then it is positive
			{
				
				return;
			}
		}
	}

	// Okay, this Control Does have focus
	if (isContained(&cp, &bounds))
	{
		if (leftBorder)
		{
			margin.left = abs(bounds.left - cp.x);


			location.left = cp.x;
			//if (location.left > location.right)
				//switchLongs(location.left, location.right);
			*o = messageOutput::positiveOverrideUpdate;


			if (content1.Get())
			{
				content1->location.left = location.left;
			}
			if (content2.Get())
			{
				content2->location.left = location.left;
			}
			if (content3.Get())
			{
				content3->location.left = location.left;
			}
			if (border1.Get())
			{
				border1->loci.left = location.left;
			}
			if (border2.Get())
			{
				border2->loci.left = location.left;
			}
			if (border3.Get())
			{
				border3->loci.left = location.left;
			}
			
			
			return;
		}
		if (rightBorder)
		{

			margin.right = abs(bounds.right - cp.x);

			location.right = cp.x;
			//if (location.left > location.right)
				//switchLongs(location.left, location.right);
			*o = messageOutput::positiveOverrideUpdate;

			if (content1.Get())
			{
				content1->location.right = location.right;
			}
			if (content2.Get())
			{
				content2->location.right = location.right;
			}
			if (content3.Get())
			{
				content3->location.right = location.right;
			}
			if (border1.Get())
			{
				border1->loci.right = location.right;
			}
			if (border2.Get())
			{
				border2->loci.right = location.right;
			}
			if (border3.Get())
			{
				border3->loci.right = location.right;
			}
			

			return;
		}
		if (topBorder)
		{
			margin.top = abs(bounds.top - cp.y);


			location.top = cp.y;
			///if (location.top > location.bottom)
				//switchLongs(location.top, location.bottom);

			*o = messageOutput::positiveOverrideUpdate;

			if (content1.Get())
			{
				content1->location.top = location.top;
			}
			if (content2.Get())
			{
				content2->location.top = location.top;
			}
			if (content3.Get())
			{
				content3->location.top = location.top;
			}
			if (border1.Get())
			{
				border1->loci.top = location.top;
			}
			if (border2.Get())
			{
				border2->loci.top = location.top;
			}
			if (border3.Get())
			{
				border3->loci.top = location.top;
			}
			
			return;
		}
		if (bottomBorder)
		{
			margin.bottom = abs(bounds.bottom - cp.y);

			location.bottom = cp.y;
			//if (location.top > location.bottom)
				//switchLongs(location.top, location.bottom);

			*o = messageOutput::positiveOverrideUpdate;


			if (content1.Get())
			{
				content1->location.bottom = location.bottom;
			}
			if (content2.Get())
			{
				content2->location.bottom = location.bottom;
			}
			if (content3.Get())
			{
				content3->location.bottom = location.bottom;
			}
			if (border1.Get())
			{
				border1->loci.bottom = location.bottom;
			}
			if (border2.Get())
			{
				border2->loci.bottom = location.bottom;
			}
			if (border3.Get())
			{
				border3->loci.bottom = location.bottom;
			}
			
			return;
		}
	} // end if "isContained" if
	
}


/*
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

/*
* Method: TControl::getEventArgs
* Purpose: Allows Action Methods to retrieve Event Arguements before calling the target Event Handler
* Parameters: void
* Returns: EventArgs - structure of relevent arguements to provide an Event Handler
*/
EventArgs TControl::getEventArgs()
{
	TObjectLocker threadLock(&thread);
	auto ret = args;
	
	return ret;
}

/**
 * Method: TBorder::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TBorder::GetType()
{
	return TString(L"TBorder;") + TObject::GetType();
}

/*
* Method:  TBorder::TBorder
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render target to draw against
*				TControl* tc - the TControl to which the TBorder is a party to 
* Returns: New TBorder Object
*/
TBorder::TBorder(TrecPointer<DrawingBoard>rtp, TControl*tc)
{
	drawingBoard = rtp;
	cap = tc;

	thickness = 1.0;

	if (drawingBoard.Get())
	{
		BuilderFocusBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Red));
	}
	else
		BuilderFocusBrush.Nullify();

	TString logMessage;
	logMessage.Format(L"CREATE %p TBorder(TrecComPointer<ID2D1RenderTarget>, TControl*)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TBorder:TBorder
* Purpose: Constructor
* Parameters: TrecPointer<TBorder> & rBord - Reference to the Border to copy
*				TControl* tc_holder - the Control to work for
* Returns: New TBorder Object
*/
TBorder::TBorder(TrecPointer<TBorder> & rBord,TControl* tc_holder)
{	
	if (!rBord.Get())return;

	thickness = rBord->thickness;
	style = rBord->style;
	stopCollection = rBord->stopCollection;
	drawingBoard = rBord->drawingBoard;

	cap = tc_holder;
	shape = rBord->shape;

	useRadial = rBord->useRadial;

	// Alternate shapes
	loci = rBord->loci;
	circle = rBord->circle;
	roundedRect = rBord->roundedRect;

	TString logMessage;
	logMessage.Format(L"CREATE %p TBorder(TrecPointer<TBorder>&,TControl*)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TBorder::TBorder
* Purpose: Blank Constructor
* Parameters: void
* Returns: Blank TBorder Object
*/
TBorder::TBorder()
{
	thickness = 1.0;


	TString logMessage;
	logMessage.Format(L"CREATE %p TBorder()", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TBorder::~TBorder
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TBorder::~TBorder()
{
	brush.Delete();
	
	BuilderFocusBrush.Delete();
	bitBrush.Delete();

	TString logMessage;
	logMessage.Format(L"DELETE %p TBorder", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TBorder::onCreate
* Purpose: Sets up a basic Rectangular border
* Parameters: RECT location - the location the Border should draw on
* Returns: bool - success
*/
bool TBorder::onCreate(D2D1_RECT_F location)
{
	TObjectLocker threadLock(&thread);
	if (!drawingBoard.Get())
	{
		
		return false;
	}
	brush.Nullify();


	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stopCollRaw;
	shape =TShape::T_Rect;
	loci.bottom = location.bottom;
	loci.left = location.left;
	loci.right = location.right;
	loci.top = location.top;

	ResetBrush();

	
	return true;
}

/*
* Method: TBorder::onCreate
* Purpose: Sets up a border optimized for dawing ovals
* Parameters: D2D1_ELLIPSE e - the oval to draw
* Returns: bool - success
*/
bool TBorder::onCreate(D2D1_ELLIPSE e)
{
	TObjectLocker threadLock(&thread);

	shape =TShape::T_Ellipse;
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

/*
* Method: TBorder::onCreate
* Purpose: Sets up a Border that has a rectangle with rounded corners
* Parameters: D2D1_ROUNDED_RECT rr - the border to draw
* Returns: bool - success
*/
bool TBorder::onCreate(D2D1_ROUNDED_RECT rr)
{
	TObjectLocker threadLock(&thread);
	shape =TShape::T_Rounded_Rect;
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


/*
* Method: TBorder::onDraw
* Purpose: Draws the border
* Parameters: void
* Returns: void
*/
void TBorder::onDraw(D2D1_RECT_F& f_loc)
{
	TObjectLocker threadLock(&thread);
	if (drawingBoard.Get() && brush.Get() && cap)
	{
		bool drawRect = true;

		switch(shape)
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


/*
* Method: TBorder::setThickness
* Purpose: Allows the thickness to be updated after creation
* Parameters: float f - the thickness to set to
* Returns: void
*/
void TBorder::setThickness(float f)
{
	TObjectLocker threadLock(&thread);
	thickness = f;
	
}

/*
* Method: TBorder::setOpaquency
* Purpose: Allows the Opaquency to be updated after creation
* Parameters: float f - the opaquency to set to (no upadte occurs if value is out of range)
* Returns: void 
*/
void TBorder::setOpaquency(float f)
{
	if (f > 1.0000000 || f < 0.0f)
		return;
	TObjectLocker threadLock(&thread);
	if (brush.Get())
	{
		
	}
	
}

/*
* Method: TBorder::getThickness
* Purpose: Allows the current thickness to be seen
* Parameters: void
* Returns: float - the current thickness
*/
float TBorder::getThickness()
{
	TObjectLocker threadLock(&thread);
	float ret = thickness;
	
	return ret;
}

/*
* Method: TBorder::getLocation
* Purpose: Retrieves the Location of the current border
* Parameters: void
* Returns: RECT - the current location
*/
RECT TBorder::getLocation()
{
	TObjectLocker threadLock(&thread);
	auto ret = convertD2DRectToRECT(loci);
	
	return ret;
}

/*
* Method: TBorder::ShiftHorizontal
* Purpose: Moves the Border left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the Border and which direction
* Returns: void
*/
void TBorder::ShiftHorizontal(int degrees)
{
	TObjectLocker threadLock(&thread);
	loci.left += degrees;
	loci.right += degrees;
	

}

/*
* Method: TBorder::ShiftVertical
* Purpose:Moves the Border up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the border and which direction
* Returns: void
*/
void TBorder::ShiftVertical(int degrees)
{
	TObjectLocker threadLock(&thread);
	loci.top += degrees;
	loci.bottom += degrees;
	
}

/*
* Method: TBorder::SetColor
* Purpose: Allows the color of the border to change even after creation
* Parameters: D2D1_COLOR_F& cf -  the color to set the border to
* Returns: void
*/
void TBorder::SetColor(const D2D1_COLOR_F& cf)
{
	TObjectLocker threadLock(&thread);
	if (brush.Get())
	{
		ID2D1SolidColorBrush* sb = dynamic_cast<ID2D1SolidColorBrush*>(brush.Get());
		if (sb)
			sb->SetColor(cf);
	}
	

}

D2D1_COLOR_F TBorder::GetColor()
{
	TObjectLocker threadLock(&thread);
	if (brush.Get())
	{
		
	}
	

	return D2D1::ColorF(D2D1::ColorF::Black);
}

void TBorder::SetColor2(const D2D1_COLOR_F& cf)
{
	TObjectLocker threadLock(&thread);
	if (brush.Get())
	{
		ID2D1LinearGradientBrush* lb = nullptr;
		ID2D1RadialGradientBrush* rb = nullptr;
	}
	
}

D2D1_COLOR_F TBorder::GetColor2()
{
	return D2D1::ColorF(D2D1::ColorF::Black);
}

D2D1_RECT_F TBorder::GetLocation()
{
	TObjectLocker threadLock(&thread);
	auto ret = loci;
	
	return ret;
}

void TBorder::SetLocation(const D2D1_RECT_F& loc)
{
	TObjectLocker threadLock(&thread);
	loci = loc;
	if (cap)
		cap->setLocation(loc);
	

}

TrecPointer<TBrush> TBorder::GetBrush()
{
	TObjectLocker threadLock(&thread);
	auto ret = brush;
	
	return ret;
}


/*
 * Method: TBorder::ResetBrush
 * Purpose: Allows the border to reset the brush when one of it's attributes changes
 * Parameters: void
 * Returns: void
 */
void TBorder::ResetBrush()
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
	else if(stopCollection.GetGradientCount() == 1)
	{
		brush = drawingBoard->GetBrush(TColor(stopCollection.GetColorAt(0)));
	}
	else
	{
		brush = drawingBoard->GetBrush(TColor());
	}
	
}

/*
* Method: TBorder::storeInTML
* Purpose: Allows te Border to save itself in the Anaface TML format
* Parameters: CArchive * ar - the File object to save to
*				int childLevel - the child level (in TML this lets the Border know how many dashes to write for each attribute)
*				messageState ms - which Border the current Border is (the Border itself does not know)
* Returns: int
*/
int TBorder::storeInTML(TFile * ar, int childLevel,messageState ms)
{
	TObjectLocker threadLock(&thread);
	TString writable;
	resetAttributeString(&writable, childLevel +1);
	TString val;
	val.Format(L"%f", thickness);
	ar->WriteString(writable);
	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|BorderThickness:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverBorderThickness:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickBorderThickness:");
		break;
	}
	
	ar->WriteString(val);
	ar->WriteString("\n");
	//val = convertD2DColorToString(color);
	//ar->WriteString(writable);

	resetAttributeString(&writable, childLevel + 1);

	switch (ms)
	{
	case messageState::normal:
		writable.Append(L"|BorderColor:");
		break;
	case messageState::mouseHover:
		writable.Append(L"|HoverBorderColor:");
		break;
	case messageState::mouseLClick:
		writable.Append(L"|ClickBorderColor:");
	}
	ar->WriteString(writable);

	ar->WriteString(val);
	ar->WriteString(L"\n");
	
	return 0;
}

/**
 * Method: TText::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TText::GetType()
{
	return TString(L"TText;") + TObject::GetType();
}

/*
* Method: TText::TText
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render Target to draw to
*				TControl* tc - the TControl to work for
* Returns: New TText Object
*/
TText::TText(TrecPointer<DrawingBoard>rtp,TControl* tc)
{
	drawingBoard = rtp;
	cap = tc;
	fontSize = 12.0;
	locale.Set( L"en-us" );
	font.Set(L"Ariel" );
	fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
	fontStyle = DWRITE_FONT_STYLE_NORMAL;
	fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	horizontalAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
	verticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	textLength = 0;

	TString logMessage;
	logMessage.Format(L"CREATE %p TText(TrecComPointer<ID2D1RenderTarget>, TControl*)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TText::TText
* Purpose: Close to a Copy Constructor
* Parameters: TrecPointer<TText> & rText -  the text element to copy
*				TControl* tc_holder - The TControl to work for
* Returns: Copied TText
*/
TText::TText(TrecPointer<TText> & rText, TControl* tc_holder)
{
	if (!rText.Get())
		return;

	drawingBoard = rText->drawingBoard;
	writeFact = rText->writeFact;

	bounds = rText->bounds;
	setCaption(rText->text);

	fontWeight = rText->fontWeight;
	fontStyle = rText->fontStyle;
	fontStretch = rText->fontStretch;

	verticalAlignment = rText->verticalAlignment;
	horizontalAlignment = rText->horizontalAlignment;
	
	fontSize = rText->fontSize;
	setLocale(rText->locale);
	setFont(rText->font);
	cap = tc_holder;

	useRadial = rText->useRadial;
	stopCollection = rText->stopCollection;

	fontLayout = rText->fontLayout;
	format = rText->format;


	TString logMessage;
	logMessage.Format(L"CREATE %p TText(TrecPointer<TText> &, TControl*)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TText::TText
* Purpose: Default Constructor
* Parameters: void
* Returns: Blank TText
*/
TText::TText()
{
	fontSize = 12.0;
	locale.Set( L"en-us" );
	font.Set(L"Ariel" );
	fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
	fontStyle = DWRITE_FONT_STYLE_NORMAL;
	fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	textLength = 0;


	horizontalAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	verticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

	TString logMessage;
	logMessage.Format(L"CREATE %p TText()", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TText::~TText
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TText::~TText()
{

	TString logMessage;
	logMessage.Format(L"DELETE %p TText::%S", this, this->text);

	Log(LogType::lt_memory, logMessage);

	writeFact.Nullify();


	format.Delete();

	fontLayout.Delete();

	penBrush.Delete();
	
}

/*
* Method: TText::onCreate
* Purpose: Sets up the Text element
* Parameters: RECT loc - the screen space to use
* Returns: int success (0 means no error)
*/
int TText::onCreate(D2D1_RECT_F loc)
{
	TObjectLocker threadLock(&thread);
	if (drawingBoard.Get() == NULL)
	{
		
		return 1;
	}

	writeFact.Nullify();
	TrecComPointer<IDWriteFactory>::TrecComHolder rawFact;

	HRESULT results = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(rawFact.GetPointerAddress()));

	if (!SUCCEEDED(results))
	{
		
		return 2;
	}
	writeFact = rawFact.Extract();

	if (fontSize <= 0.0)
		fontSize = 12.0;

	format.Nullify();
	TrecComPointer<IDWriteTextFormat>::TrecComHolder wtf;
	results = writeFact->CreateTextFormat(
		font.GetConstantBuffer().getBuffer(),
		NULL,
		fontWeight,
		fontStyle,
		fontStretch,
		fontSize,
		locale.GetConstantBuffer().getBuffer(),
		wtf.GetPointerAddress());
	format = wtf.Extract();

	if (!SUCCEEDED(results))
	{
		
		return 3;
	}
	results = format->SetTextAlignment(horizontalAlignment);
	if (SUCCEEDED(results))
		results = format->SetParagraphAlignment(verticalAlignment);

	if (!SUCCEEDED(results))
	{
		
		return 4;
	}
	bounds.bottom = loc.bottom;
	bounds.left = loc.left;
	bounds.right = loc.right;
	bounds.top = loc.top;

	ResetBrush();

	if (!SUCCEEDED(results))
	{
		
		return 5;
	}

	TrecComPointer<IDWriteTextLayout>::TrecComHolder wtl;
	results = writeFact->CreateTextLayout(text.GetConstantBuffer().getBuffer(), text.GetSize(), format.Get(), loc.right - loc.left, loc.bottom - loc.top, wtl.GetPointerAddress());
	

	if (SUCCEEDED(results))
	{
		fontLayout = wtl.Extract();
		
		return 0;
	}
	else
	{
		int e = GetLastError();
		
		return 6;
	}
}

/*
* Method: TText::reCreateLayout
* Purpose: Resets the Text Layout upon changes made to the format, text, or location
* Parameters: void 
* Returns: void
* Note: Call this method whenever you make a change to the format of the control, the text, or the location,
*		Otherwise, your changes might not be seen 
*/
void TText::reCreateLayout()
{
	TObjectLocker threadLock(&thread);
	reCreateLayout(text);
	
}

/*
 * Method: TText::reCreateLayout
 * Purpose: Resets the Text Layout upon changes made to the format, text, or location
 * Parameters: TString & str - the Text to change the format object to
 * Returns: void
 * Note: Call this method whenever you make a change to the format of the control, the text, or the location,
 *		Otherwise, your changes might not be seen
 */
void TText::reCreateLayout(TString & str)
{
	TObjectLocker threadLock(&thread);
	fontLayout.Nullify();
	if (format.Get())
	{
		TrecComPointer<IDWriteTextLayout>::TrecComHolder wfl;
		writeFact->CreateTextLayout(str.GetConstantBuffer().getBuffer(), str.GetSize(), format.Get(), bounds.right - bounds.left, bounds.bottom - bounds.top, wfl.GetPointerAddress());
		fontLayout = wfl.Extract();
	}
	

}

/*
* Method: TText::onDraw
* Purpose: Draws the text
* Parameters: RECT r - the area to draw
* Returns: bool success
*/
bool TText::onDraw(D2D1_RECT_F& loc, TObject* obj)
{
	TObjectLocker threadLock(&thread);

	
	if (!penBrush.Get() || !drawingBoard.Get())
	{	
		
		return false;
	}
	if (bounds != loc)
		bounds = loc;
	// TString print;
	if (obj && text.GetSize() > 0 && text.GetAt(0) == L'{' && text.GetAt(text.GetSize() - 1) == L'}')
	{
		TString print = obj->getVariableValueStr(text.SubString(1, text.GetSize() - 1));
		reCreateLayout(print);
	}
	

		bounds = loc;
		if (fontLayout.Get())
		{
			fontLayout->SetMaxHeight(loc.bottom - loc.top);
			fontLayout->SetMaxWidth(loc.right - loc.left);
			ID2D1Brush* b = penBrush->GetUnderlyingBrush().Get();
			if (b)
			{
				TDrawingContext tContext{ drawingBoard->GetRenderer().Get(), b };
				fontLayout->Draw(&tContext, textRenderer, loc.left, loc.top);
				//drawingBoard->GetRenderer()->DrawTextLayout(D2D1::Point2F(), fontLayout.Get(), b);
			}
		}
		
		
	return true;
}


/*
* Method: TText::setNewFont
* Purpose: Sets up the new font for the text
* Parameters: TString& pFont - String holding the new fnt to use
* Returns: bool - true (redundant return value)
*/
bool TText::setNewFont(TString& pFont)
{
	TObjectLocker threadLock(&thread);
	font.Set( pFont);
	onCreate(bounds);
	reCreateLayout();
	
	return true;
}

/*
* Method: TText::setNewFontSize
* Purpose: Updates the size of the font to print
* Parameters: float fs - the new font size
* Returns: void
*/
void TText::setNewFontSize(float fs)
{
	TObjectLocker threadLock(&thread);
	fontSize = abs(fs);
	onCreate(bounds);
	reCreateLayout();
	

}

/*
* Method: TText::setNewHorizontalAlignment
* Purpose: Updates the Horizontal Alignment of the text element
* Parameters: DWRITE_TEXT_ALIGNMENT ha - the new horizontal alignment
* Returns: void
*/
void TText::setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT ha)
{
	TObjectLocker threadLock(&thread);
	horizontalAlignment = ha;
	if (format.Get())
	{
		format->SetTextAlignment(ha);
		reCreateLayout();
	}
	

}

/*
* Method: TText::setNewVerticalAlignment
* Purpose: Updates the Vertical Alignment of the Text element
* Parameters: DWRITE_PARAGRAPH_ALIGNMENT va - the new vertical alignment
* Returns: void
*/
void TText::setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT va)
{
	TObjectLocker threadLock(&thread);
	verticalAlignment = va;
	if (format.Get())
	{
		format->SetParagraphAlignment(va);
		reCreateLayout();
	}
	

}

/*
* Method: TText::setOpaquency
* Purpose: Updates the opaquency of the text
* Parameters: float o - the new opequency
* Returns: bool - true if data is valid, false otherwise
*/
bool TText::setOpaquency(float o)
{
	if(o > 1.0000000000f || o < 0.0f)
	return false;
	//color.a = o;
	return true;
}

/*
* Method: TText::getFontSize
* Purpose: Retrieves the current font size of the text
* Parameters: void
* Returns: float - the current font size
*/
float TText::getFontSize()
{
	TObjectLocker threadLock(&thread);
	auto ret = fontSize;
	
	return ret;
}

/*
* Method: TText::getFont
* Purpose: Retrieves the current font of the text
* Parameters: void
* Returns: TString - the current font
*/
TString TText::getFont()
{
	TObjectLocker threadLock(&thread);
	TString ret(font);
	
	return ret;
}

/*
* Method: TText::getCaption
* Purpose: Retrieves the new caption for the text element
* Parameters: void
* Returns: TString - the current caption
*/
TString TText::getCaption()
{
	TObjectLocker threadLock(&thread);
	TString ret(text);
	
	return ret;
}

/*
 * Method: TText::GetMinWidth
 * Purpose: Retirvees the minimum width needed before DirectWrtie has to add emergency breaks in line
 * Parameters: bool& worked - whether the value returned is truely the reported value
 * Return: float - the min width needed. If inspection fails, this represents the width currently used
 */
float TText::GetMinWidth(bool& worked)
{
	TObjectLocker threadLock(&thread);
	float ret = 0.0f;
	if(!(worked = (fontLayout.Get() && SUCCEEDED(fontLayout->DetermineMinWidth(&ret)))))
		ret = bounds.right - bounds.left;
	
	
	return ret;
}

float TText::GetMinHeight(bool& worked)
{
	float ret = 0.0f;
	DWRITE_TEXT_METRICS mets;
	worked = false;
	if (fontLayout.Get() && SUCCEEDED(fontLayout->GetMetrics(&mets)))
	{
		worked = true;
		ret = mets.height;
	}

	return ret;
}

/*
* Method: TText::setCaption
* Purpose: Updates the text with a new caption
* Parameters: TString& string - the new Caption to use
* Returns: void
*/
void TText::setCaption(const TString& string)
{
	TObjectLocker threadLock(&thread);
	text.Set(string);
	reCreateLayout();
	
}

/*
* Method: TText::setLocale
* Purpose: Sets a new Local for the text
* Parameters: TString& loc - the String holding the new locale
* Returns: void
*/
void TText::setLocale(TString& loc)
{
	TObjectLocker threadLock(&thread);
	locale = loc;
	reCreateLayout();
	
	
}

/*
* Method: TText::setFont
* Purpose: Sets up the new font for the text
* Parameters: TString& fo - String holding the new fnt to use
* Returns: bool - true (redundant return value)
* Note: DEPRECIATED - us the method TText::setNewFont
*/
void TText::setFont(TString& fo)
{
	TObjectLocker threadLock(&thread);
	font = fo;
	reCreateLayout();
	
	
}

/*
* Method: TText::removeCaption
* Purpose: Removes the Text and sets it to zero
* Parameters: void
* Returns: void
*/
void TText::removeCaption()
{

	TObjectLocker threadLock(&thread);
	text.Empty();
	textLength = 0;
	
	
}

/*
* Method: TText::getHorizontalAlignment
* Purpose: Gets the Horizontal alignment of the text
* Parameters: void
* Returns: DWRITE_TEXT_ALIGNMENT - the Horizontal alignment
*/
DWRITE_TEXT_ALIGNMENT TText::getHorizontalAlignment()
{
	TObjectLocker threadLock(&thread);
	auto ret = horizontalAlignment;
	
	return ret;
}

/*
* Method: TText::getVerticalAlignment
* Purpose: Gets the Vertical alignment of the text
* Parameters: void
* Returns: DWRITE_PARAGRAPH_ALIGNMENT - the vetical alignment if the text
*/
DWRITE_PARAGRAPH_ALIGNMENT TText::getVerticalAlignment()
{
	TObjectLocker threadLock(&thread);
	auto ret = verticalAlignment;
	
	return ret;
}

/*
* Method: TText::getLength
* Purpose: Retrieves the current length of the text being held
* Parameters: void
* Returns: int - the length of the text
*/
int TText::getLength()
{
	TObjectLocker threadLock(&thread);
	int ret = textLength;
	
	return ret;
}

/*
* Method: TText::getLocation
* Purpose: Retrieves the Location of the Text Control
* Parameters: void
* Returns: RECT - the location of the text
*/
RECT TText::getLocation()
{
	TObjectLocker threadLock(&thread);
	auto ret = convertD2DRectToRECT(bounds);
	
	return ret;
}

/*
* Method: TText::ShiftHorizontal
* Purpose: Moves the Text element left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the text and which direction
* Returns: void
*/
void TText::ShiftHorizontal(int degrees)
{
	TObjectLocker threadLock(&thread);
	bounds.left += degrees;
	bounds.right += degrees;
	
	//if (!isSnipZero(snip))
	//{
	//	snip->left += degrees;
	//	snip->right += degrees;
	//}
}

/*
* Method: TText::ShiftVertical
* Purpose:Moves the Text Element up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the text and which direction
* Returns: void
*/
void TText::ShiftVertical(int degrees)
{
	TObjectLocker threadLock(&thread);
	bounds.top += degrees;
	bounds.bottom += degrees;
	
}

void TText::SetColor(const D2D1_COLOR_F& cf)
{
	TObjectLocker threadLock(&thread);
	//color = cf;
	if (penBrush.Get())
	{
		penBrush->SetColor(TColor(cf));
	}
	
}

D2D1_COLOR_F TText::GetColor()
{
	TObjectLocker threadLock(&thread);
	auto ret = penBrush.Get() ? penBrush->GetColor().GetColor() : D2D1::ColorF(D2D1::ColorF::Black);
	
	return ret;
}

void TText::SetColor2(const D2D1_COLOR_F& color)
{
	//color2 = color;
}

D2D1_COLOR_F TText::GetColor2()
{
	TObjectLocker threadLock(&thread);
	auto ret = D2D1::ColorF(D2D1::ColorF::Black);
	
	return ret;
}

D2D1_RECT_F TText::GetLocation()
{
	TObjectLocker threadLock(&thread);
	auto ret = bounds;
	
	return ret;
}

void TText::SetLocation(const D2D1_RECT_F& loc)
{
	TObjectLocker threadLock(&thread);
	bounds = loc;
	this->reCreateLayout();
	
}

TrecPointer<TBrush> TText::GetBrush()
{
	TObjectLocker threadLock(&thread);
	auto ret = penBrush;
	
	return ret;
}

/*
 * Method: TText::ResetBrush
 * Purpose: Allows the Text to reset the brush when one of it's attributes changes
 * Parameters: void
 * Returns: void
 */
void TText::ResetBrush()
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
			penBrush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(bounds.left, bounds.top),
				D2D1::Point2F(bounds.right, bounds.bottom),
				bounds.right - bounds.left, bounds.bottom - bounds.top);
		}
		else
		{
			penBrush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(bounds.left, bounds.top),
				D2D1::Point2F(bounds.right, bounds.bottom));
		}

	}
	else if (stopCollection.GetGradientCount() == 1)
	{
		penBrush = drawingBoard->GetBrush(TColor(stopCollection.GetColorAt(0)));
	}
	else
	{
		penBrush = drawingBoard->GetBrush(TColor());
	}
	
}



/*
* Method: TText::storeInTML
* Purpose: Allows the text to store it's attributes to a TML file
* Parameters: CArchive* ar - the file system to write to
*				int childLevel - the level this control is - how many dashes to write
*				messageState ms - the version of the text it is (TText doesn't know if it is normal, hover, or click)
* Returns: int - 0
*/
int TText::storeInTML(TFile * ar, int childLevel,messageState ms)
{
	TObjectLocker threadLock(&thread);
	TString writable;
	resetAttributeString(&writable, childLevel + 1);
	ar->WriteString(writable);
	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|Caption:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverCaption:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickCaption:");
	}

	ar->WriteString(text);
	ar->WriteString(L"\n");

	ar->WriteString(writable);

	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|CaptionLocale:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverCaptionLocale:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickCaptionLocale:");
	}
	ar->WriteString(locale);
	ar->WriteString(L"\n");

	ar->WriteString(writable);
	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|Font:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverFont:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickFont:");
	}

	ar->WriteString(font);
	ar->WriteString(L"\n");

	//TString val = convertD2DColorToString(color);
	ar->WriteString(writable);
	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|FontColor:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverFontColor:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickFontColor:");
	}

	//ar->WriteString(val);
	ar->WriteString(L"\n");
	//val.Format(L"%f", fontSize);
	ar->WriteString(writable);
	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|FontSize:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverFontSize:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickFontSize:");
	}

	//ar->WriteString(val);
	ar->WriteString(L"\n");
	resetAttributeString(&writable, childLevel +1);
	ar->WriteString(writable);
	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|VerticalAlignment:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverVerticalAlignment:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickVerticalAlignment:");
	}
	switch (verticalAlignment)
	{
	case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
		ar->WriteString(L"Bottom\n");
		break;
	case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
		ar->WriteString(L"Top\n");
		break;
	default:
		ar->WriteString(L"Center\n");
	}
	ar->WriteString(writable);
	switch (ms)
	{
	case messageState::normal:
		ar->WriteString(L"|HorizontalAlignment:");
		break;
	case messageState::mouseHover:
		ar->WriteString(L"|HoverHorizontalAlignment:");
		break;
	case messageState::mouseLClick:
		ar->WriteString(L"|ClickHorizontalAlignment:");
	}
	switch (horizontalAlignment)
	{
	case DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
		ar->WriteString(L"Justified\n");
		break;
	case DWRITE_TEXT_ALIGNMENT_LEADING:
		ar->WriteString(L"Right\n");
		break;
	case DWRITE_TEXT_ALIGNMENT_TRAILING:
		ar->WriteString(L"Left\n");
		break;
	default:
		ar->WriteString(L"Center\n");
	}

	
	return 0;
}

/**
 * Method: TContent::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TContent::GetType()
{
	return TString(L"TContent;") + TObject::GetType();
}

/*
* Method: TContent::TContent
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render Target to draw to
*				TControl* tc - the parent Control to work for
* Returns: New TContent
*/
TContent::TContent(TrecPointer<DrawingBoard>rtp, TControl* tc)
{
	drawingBoard = rtp;
	cap = tc;
	
	thickness = 1.0;
	location = D2D1_RECT_F{ 0,0,0,0 };


	TString logMessage;
	logMessage.Format(L"CREATE %p TContent(TrecComPointer<ID2D1RenderTarget>, TControl*)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TContent::TContent
* Purpose: Default Constructor
* Parameters: void 
* Returns: void
*/
TContent::TContent()
{
	thickness = 1.0;
	location = D2D1_RECT_F{ 0,0,0,0 };

	TString logMessage;
	logMessage.Format(L"CREATE %p TContent()", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TContent::TContent
* Purpose: Constructor
* Parameters: TrecPointer<TContent> &rCont - an exiting Content to copy from
*				TControl* tc_holder - the Control to work for
* Returns: New TContent object
*/
TContent::TContent(TrecPointer<TContent> &rCont, TControl* tc_holder)
{
	if (!rCont.Get())return;

	drawingBoard = rCont->drawingBoard;
	
	thickness =rCont->thickness;
	style = rCont->style;
	location = rCont->location;
	
	cap = tc_holder;
	shape = rCont->shape;

	useRadial = rCont->useRadial;
	stopCollection = rCont->stopCollection;

	// Alternate shapes
	circle = rCont->circle;
	roundedRect = rCont->roundedRect;

	TString logMessage;
	logMessage.Format(L"CREATE %p TContent(TrecPointer<TContent> &rCont, TControl*)", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TContent::~TContent
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TContent::~TContent()
{
	image.Delete();
	brush.Delete();

	TString logMessage;
	logMessage.Format(L"DELETE %p TContent", this);

	Log(LogType::lt_memory, logMessage);
}

/*
* Method: TContent::onCreate
* Purpose: Sets up the details of the Content with a Rectangle to draw
* Parameters: RECT l - the Location
*				RECT s - the snip, where the control can appear
* Returns: bool - success (whether the resources are prepared)
*/
bool TContent::onCreate(D2D1_RECT_F l)
{
	TObjectLocker threadLock(&thread);
	location.top = l.top;
	location.left = l.left;
	location.right = l.right;
	location.bottom = l.bottom;

	if (!drawingBoard.Get())
	{
		
		return false;
	}
	brush.Nullify();

	ResetBrush();
	
	return true;
}

/*
* Method: TContent::onCreate
* Purpose: Sets up the details of the Content with an oval to draw
* Parameters: D2D1_ELLIPSE e - the oval to draw
* Returns: bool - success (whether the resources are prepared)
*/
bool TContent::onCreate(D2D1_ELLIPSE e)
{
	TObjectLocker threadLock(&thread);
	if (!drawingBoard.Get())
	{
		
		return false;
	}
	circle = e;
	brush.Nullify();

	location.left = e.point.x - e.radiusX;
	location.right = e.point.x + e.radiusX;
	location.top = e.point.y - e.radiusY;
	location.bottom = e.point.y + e.radiusY;
	ResetBrush();
	
	return true;
}

/*
* Method: TContent::onCreate
* Purpose: Sets up the details of the Content with a rounded Rectangle to draw
* Parameters: D2D1_ROUNDED_RECT rr -  the rounded rectangle to draw
* Returns: bool - success (whether the resources are prepared)
*/
bool TContent::onCreate(D2D1_ROUNDED_RECT rr)
{
	TObjectLocker threadLock(&thread);
	if (!drawingBoard.Get())
	{
		
		return false;
	}
	roundedRect = rr;
	brush.Nullify();

	location.left = rr.rect.left;
	location.right = rr.rect.right;
	location.top = rr.rect.top;
	location.bottom = rr.rect.bottom;

	ResetBrush();
	
	return true;
}

/*
* Method: TContent::ShiftHorizontal
* Purpose: Moves the content left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the content and which direction
* Returns: void
*/
void TContent::ShiftHorizontal(int degrees)
{
	TObjectLocker threadLock(&thread);
	location.left += degrees;
	location.right += degrees;
	
}

/*
* Method: TContent::ShiftVertical
* Purpose:Moves the content up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the content and which direction
* Returns: void
*/
void TContent::ShiftVertical(int degrees)
{
	TObjectLocker threadLock(&thread);
	location.top += degrees;
	location.bottom += degrees;
	
}

/*
* Method: TContent::onDraw
* Purpose: Draws the content, including a picture if one is provided
* Parameters: void
* Returns: void
*/
void TContent::onDraw(D2D1_RECT_F& f_snip)
{
	TObjectLocker threadLock(&thread);
	if (!drawingBoard.Get())
	{
		
		return;
	}
	if (brush.Get())
	{
		switch (shape)
		{
		case TShape::T_Rect:
			brush->FillRectangle(f_snip);
			break;
		case TShape::T_Rounded_Rect:
			roundedRect.rect = f_snip;
			brush->FillRoundedRectangle(roundedRect);

			break;
		case TShape::T_Ellipse:
			circle.point.x = (f_snip.right + f_snip.left) / 2;
			circle.point.y = (f_snip.bottom + f_snip.top) / 2;
			circle.radiusX = f_snip.right - f_snip.left;
			circle.radiusY = f_snip.bottom - f_snip.top;
			brush->FillEllipse(circle);
			break;
		case TShape::T_Custom_shape:
			break;
		}
	}
	if (image.Get() )
	{
		image->FillRectangle(f_snip);
	
		//cropImage->CopyFromBitmap(NULL)
		
		//rt->DrawBitmap(image, thickness, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, location);
		
	}
	
}


/*
* Method: TContent::setOpaquency
* Purpose: Updates the opaquency after creation
* Parameters: float f - the new opacuency to use
* Returns: bool whether the input was valid or not
*/
bool TContent::setOpaquency(float f)
{
	if(f > 1.000000000 || f < 0.0f)
		return false;
	//color.a = f;
	return true;
}



/*
* Method: TContent::getLocation
* Purpose: Retrieves the current location of the content
* Parameters:
* Returns: RECT - the current location of the Control
*/
RECT TContent::getLocation()
{
	TObjectLocker threadLock(&thread);
	auto ret = convertD2DRectToRECT(location);
	
	return ret;
}

void TContent::SetRadialImage(TDataArray<D2D1_COLOR_F>& colors)
{
	TObjectLocker threadLock(&thread);
	auto stopper = getStopCollection(colors);
	if (stopper.Get())
	{
		stopCollection = *stopper.Get();

		brush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(location.left, location.top),
			D2D1::Point2F(location.right, location.bottom),
			location.right - location.left, location.bottom - location.top);
	}
	
}

void TContent::SetLinearImage(TDataArray<D2D1_COLOR_F>& colors)
{
	TObjectLocker threadLock(&thread);
	auto stopper = getStopCollection(colors);
	if (stopper.Get())
	{
		stopCollection = *stopper.Get();

		brush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(location.left, location.top),
			D2D1::Point2F(location.right, location.bottom));
	}
	
}


void TContent::SetColor(const D2D1_COLOR_F& cf)
{
	TObjectLocker threadLock(&thread);
	if (brush.Get())
	{
		brush->SetColor(TColor(cf));
	}
	
}

D2D1_COLOR_F TContent::GetColor()
{
	return TColor().GetColor();
}

void TContent::SetColor2(const D2D1_COLOR_F& color)
{
	//color2 = color;
}

D2D1_COLOR_F TContent::GetColor2()
{
	return TColor().GetColor();
}

D2D1_RECT_F TContent::GetLocation()
{
	TObjectLocker threadLock(&thread);
	auto ret = location;
	
	return ret;
}

void TContent::SetLocation(const D2D1_RECT_F& loc)
{
	TObjectLocker threadLock(&thread);
	location = loc;
	if (cap)
		cap->setLocation(loc);
	
}

TrecPointer<TBrush> TContent::GetBrush()
{
	TObjectLocker threadLock(&thread);
	auto ret = brush;
	
	return ret;
}

UINT TContent::GetImageCount()
{
	TObjectLocker threadLock(&thread);

	UINT ret = image.Get() ? image->GetFrameCount() : 0;
	

	return ret;
}

TrecPointer<TGradientStopCollection> TContent::getStopCollection(TDataArray<D2D1_COLOR_F>& colors)
{
	TObjectLocker threadLock(&thread);
	if (!drawingBoard.Get() || !colors.Size()) {
		
		return TrecPointer<TGradientStopCollection>();
	}
	TrecPointer<TGradientStopCollection> grads = TrecPointerKey::GetNewTrecPointer<TGradientStopCollection>();

	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stop;

	float stopValue = 1.0f / static_cast<float>(colors.Size());

	for (UINT Rust = 0; Rust < colors.Size(); Rust++)
	{

		grads->AddGradient(TGradientStop(TColor(colors[Rust]), static_cast<float>(Rust) * stopValue));
	}
	
	return grads;
}

/*
 * Method: TContent::ResetBrush
 * Purpose: Allows the content to reset the brush when one of it's attributes changes
 * Parameters: void
 * Returns: void
 */
void TContent::ResetBrush()
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
			brush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom),
				location.right - location.left, location.bottom - location.top);
		}
		else
		{
			brush = drawingBoard->GetBrush(stopCollection, D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom));
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

/*
* Method: TContent::storeInTML
* Purpose: Enables the Content to store itself in a TML file
* Parameters: CArchive * ar - the file to write to
*				int childLevel - the generation level content is (how many dashes to write)
*				messageState ms - the state the content is (Object does not know this)
* Returns: int - 0
*/
int TContent::storeInTML(TFile * ar, int childLevel,messageState ms)
{
	//TString writable;
	//resetAttributeString(&writable, childLevel + 1);
	//TString val;
	//val.Format(L"%f", thickness);
	//ar->WriteString(writable);
	//switch (ms)
	//{
	//case normal:
	//	ar->WriteString(L"|ContentThickness:");
	//	break;
	//casemessageState::mouseHover:
	//	ar->WriteString(L"|HoverContentThickness:");
	//	break;
	//casemessageState::mouseLClick:
	//	ar->WriteString(L"|ClickContentThickness:");
	//}

	//ar->WriteString(val);
	//ar->WriteString(L"\n");
	//val = convertD2DColorToString(color);
	//ar->WriteString(writable);
	//switch (ms)
	//{
	//case normal:
	//	ar->WriteString(L"|ContentColor:");
	//	break;
	//casemessageState::mouseHover:
	//	ar->WriteString(L"|HoverContentColor:");
	//	break;
	//casemessageState::mouseLClick:
	//	ar->WriteString(L"|ClickContentColor:");
	//}

	//ar->WriteString(val);
	//ar->WriteString(L"\n");
	return 0;
}


/**
* Function: resetAttributeString
* Purpose: Resets an attribute string for when saving to a TML String attribute
* Parameters: TString* st - The String to Adjust
*				int cou - the generation count
* Returns: void
*/
void resetAttributeString(TString* st, int cou)
{
	st->Empty();
	for (int c = 0; c < cou;c++)
		st->AppendChar(L'-');
}

/*
* Function: convertD2DColorToString
* Purpose: Creates a string that represents a D2D color
* Parameters: D2D1::ColorF - the color to convert
* Returns: CString - the Color in string form
*/
TString convertD2DColorToString(D2D1_COLOR_F color) 
{
	TString val;
	val.Format(L"%f", color.r);
	TString col = val + TString(",");
	val.Format(L"%f", color.g);
	col = col + val + TString(",");
	val.Format(L"%f", color.b);
	col = col + val + TString(",");
	val.Format(L"%f", color.a);
	col = col + val;
	return col;
}


/*
* Function: convertRectToString
* Purpose: Creates a string that represents a rectangle
* Parameters: RECT - the Rectangle to convert
* Returns: CString - the Representation of a rectangle in string form
*/
TString convertRectToString(RECT sty) 
{
	TString val;
	val.Format(L"%d", sty.top);

	TString comma(L",");

	TString col = val + comma;
	val.Format(L"%d", sty.left);
	col = col + val + comma;
	val.Format(L"%d", sty.bottom);
	col = col + val + comma;
	val.Format(L"%d", sty.right);
	col = col + val;
	return col;
}

/*
* Function: convertRectToString
* Purpose: Creates a string that represents a rectangle
* Parameters: D2D1_RECT_F - the Rectangle to convert
* Returns: CString - the Representation of a rectangle in string form
*/
TString _ANAFACE_DLL convertD2D1RectToString(D2D1_RECT_F sty)
{
	TString val;
	val.Format(L"%f", sty.top);

	TString comma(L",");

	TString col(val + comma);
	val.Format(L"%f", sty.left);
	col.Set(col + val + comma);
	val.Format(L"%f", sty.bottom);
	col.Set(col + val + comma);
	val.Format(L"%f", sty.right);
	col.Set(col + val);
	return col;
}

/*
* Function: convertStringToRECT
* Purpose: Determines the Rectangle to use
* Parameters: TString* the String to read
* Returns: RECT - the resulting rectangle (Zero Rect if string is invalid)
*/
RECT convertStringToRECT(TString* str)
{
	RECT returnable = RECT{ 0,0,0,0 };
	if (str == NULL)
	{
		fallBack:
		returnable = RECT{ 0,0,0,0 };
		return returnable;
	}

	TrecPointer<TDataArray<TString>> strSpl = str->split(",");

	
	int res[4] = { 0,0,0,0 };
	if (strSpl.Get() == NULL || strSpl->Size() != 4)
	{
		goto fallBack;
	}
	res[0] = strSpl->at(0).ConvertToInt(returnable.top);
	res[1] = strSpl->at(1).ConvertToInt(returnable.left);
	res[2] = strSpl->at(2).ConvertToInt(returnable.bottom);
	res[3] = strSpl->at(3).ConvertToInt(returnable.right);
	if (res[0] > 0 || res[1] > 0 || res[2] > 0 || res[3] > 0)
		goto fallBack;
	return returnable; LONG i;
}

D2D1_RECT_F convertStringToD2D1Rect(TString* str)
{
	D2D1_RECT_F returnable = D2D1_RECT_F{ 0,0,0,0 };
	if (str == NULL)
	{
	fallBack:
		returnable = D2D1_RECT_F{ 0,0,0,0 };
		return returnable;
	}

	TrecPointer<TDataArray<TString>> strSpl = str->split(",");


	int res[4] = { 0,0,0,0 };
	if (strSpl.Get() == NULL || strSpl->Size() != 4)
	{
		goto fallBack;
	}
	res[0] = strSpl->at(0).ConvertToFloat(returnable.top);
	res[1] = strSpl->at(1).ConvertToFloat(returnable.left);
	res[2] = strSpl->at(2).ConvertToFloat(returnable.bottom);
	res[3] = strSpl->at(3).ConvertToFloat(returnable.right);
	if (res[0] > 0 || res[1] > 0 || res[2] > 0 || res[3] > 0)
		goto fallBack;
	return returnable; LONG i;
}

/*
* Function: convertStringToD2DColor
* Purpose: Converts a String to it's Color Representation (Black if invalid)
* Parameters: TString* - the string to read
* Returns: D2D1::ColorF - the color the string represents (Black if invalid)
*/
D2D1::ColorF convertStringToD2DColor(TString *str)
{
	int res[4] = { 0,0,0,0 };
	float rgba[4] = { 0,0,0,0 };
	if (str == NULL)
	{

		colorFallBack:
		return D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	}

	TrecPointer<TDataArray<TString>> strSpl = str->split(",");

	
	res[0] = strSpl->at(0).ConvertToFloat(rgba[0]);
	res[1] = strSpl->at(1).ConvertToFloat(rgba[1]);
	res[2] = strSpl->at(2).ConvertToFloat(rgba[2]);
	res[3] = strSpl->at(3).ConvertToFloat(rgba[3]);
	if (res[0] > 0 || res[1] > 0 || res[2] > 0 || res[3] > 0)
	{
		goto colorFallBack;
	}

	for (int c = 0; c < 4; c++)
	{
		if (rgba[c] > 1.0f && rgba[c] <= 10.0f)
			rgba[c] /= 10.0f;
		else if (rgba[c] > 10.0f) // In this case, color was given in 255 form, adjust accordingly
			rgba[c] /= 255.0f;

	}

	return D2D1::ColorF(rgba[0], rgba[1], rgba[2], rgba[3]);
}

/*
* Function: convertCRectToD2DRect
* Purpose: Turns an MFC RECT into a Direct2D Rect for compatibility purposes
* Parameters: RECT * - the source rectangle
*				D2D1_RECT_F* - the destination rectangle
* Returns: false if either parameter is null
*/
bool convertCRectToD2DRect(RECT* mfcr, D2D_RECT_F* d2dr)
{
	if (!mfcr || !d2dr)
		return false;
	d2dr->bottom = mfcr->bottom;
	d2dr->left = mfcr->left;
	d2dr->right = mfcr->right;
	d2dr->top = mfcr->top;
	return true;
}

/*
* Function: isContained
* Purpose: Checks of a point is within a given MFC Rectangle
* Parameters: const TPoint* - the point to check
*				const RECT* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool isContained(const TPoint* cp, const RECT* r)
{
	if (!cp || !r)
		return false;
	return cp->x > r->left && cp->x < r->right &&
		cp->y > r->top && cp->y < r->bottom;
}

/*
* Function: isContained
* Purpose: Checks of a point is within a given MFC Rectangle
* Parameters: const TPoint& - the point to check
*				const RECT& - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool isContained(const TPoint& cp, const RECT& r)
{
	return cp.x > r.left&& cp.x < r.right &&
		cp.y > r.top&& cp.y < r.bottom;
}

/*
* Function: isContained
* Purpose: Checks of a point is within a given Direct2D Rectangle
* Parameters: const TPoint* - the point to check
*				const D2D1_RECT_F* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool isContained(const TPoint* cp, const D2D1_RECT_F* r)
{
	if (!cp || !r)
		return false;
	return cp->x >= r->left && cp->x <= r->right &&
		cp->y >= r->top && cp->y <= r->bottom;
}

/*
* Function: isContained
* Purpose: Checks of a point is within a given Direct2D Rectangle
* Parameters: const TPoint& - the point to check
*				const D2D1_RECT_F& - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool isContained(const TPoint& cp, const D2D1_RECT_F& r)
{
	return cp.x >= r.left&& cp.x <= r.right &&
		cp.y >= r.top&& cp.y <= r.bottom;
}

/*
* Function: isContained
* Purpose: Checks if a point is in a certain ellipse
* Parameters: const TPoint* - the point to check
*				const D2D1_ELLIPSE* - the ellipse to check
* Returns: bool - whether the point is within the specified bounds
*/
// To-Do: improve function as more math is learned
bool isContained(const TPoint& cp, const D2D1_ELLIPSE& el) 
{

	return (pow((cp.x - el.point.x),2) / pow(el.radiusX,2)
		+ pow((cp.y - el.point.y),2) / pow(el.radiusY,2)) <= 1;
}

/*
* Function: convertStringToTextAlignment
* Purpose: Reads the String and returns whether the Text should be right, left, center, or justified
* Parameters: TString* t - the string to parse
* Returns: DWRITE_TEXT_ALIGNMENT - the Text position (Center if string is invalid)
*/
DWRITE_TEXT_ALIGNMENT convertStringToTextAlignment(TString* t)
{
	if (!t)
		return DWRITE_TEXT_ALIGNMENT_CENTER;
	if (!t->Compare(L"Left"))
		return DWRITE_TEXT_ALIGNMENT_TRAILING;
	if (!t->Compare(L"Right"))
		return DWRITE_TEXT_ALIGNMENT_LEADING;
	if (!t->Compare(L"Justified"))
		return DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	return DWRITE_TEXT_ALIGNMENT_CENTER;
}

/*
* Function: convertStringToParagraphAlignment
* Purpose: Reads the String and returns whether the Text should be top, bottom, or center
* Parameters: TString* t - the string to parse
* Returns: DWRITE_PARAGRAPH_ALIGNMENT - the text alignment (Center if string is invalid)
*/
DWRITE_PARAGRAPH_ALIGNMENT convertStringToParagraphAlignment(TString * t)
{
	if (!t)
		return DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	if (!t->Compare(L"Top"))
		return DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	if (!t->Compare(L"Bottom"))
		return DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	return DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
}

/*
* Function: ZeroMemory_
* Purpose: "Zero's an Event Args, (Using Microsoft's ZeroMemory macro currupts the TString in the EVent Args)
* Parameters: EventArgs& ea - the Event arguement structure to reset
* Returns: void
*/
void ZeroMemory_(EventArgs & ea)
{
	ea.arrayLabel = -1;
	ea.control = nullptr;
	ea.eventType = static_cast<R_Message_Type>(0);
	ea.isClick = false;
	ea.isLeftClick = false;
	ea.methodID = -1;
	ea.point.x = 0.0f;
	ea.point.y = 0.0f;
	ea.positive = false;
	ea.text.Empty();
}



/*
* Function: switchLongs
* Purpose: Swaps longs
* Parameters: LONG& l1 - the first long to switch
*				LONG& l2 - the second long to switch
* Returns: void
*/
void switchLongs(LONG & l1, LONG & l2)
{
	LONG temp = l1;
	l1 = l2;
	l2 = temp;
}

/*
* Function: isSnipZero
* Purpose: Checks if a Rectangle is a "Zero" rectangle
* Parameters: const RECT& snip - the Rectangle to check
* Returns: bool - whether the RECT is "Zero" or not
*/
bool isSnipZero(const RECT & snip)
{
	return !snip.bottom && !snip.left && !snip.right && !snip.top;
}

bool _ANAFACE_DLL isSnipZero(const D2D1_RECT_F& snip)
{
	return !snip.bottom && !snip.left && !snip.right && !snip.top;
}

/*
* Function: zeroSnip
* Purpose: Sets the Rectangle to 0, used to mark a control as off screen
* Parameters: RECT& snip - the Rectangle to Nullify
* Returns: void
*/
void zeroSnip(RECT & snip)
{
	snip.bottom = 0;
	snip.left = 0;
	snip.right = 0;
	snip.top = 0;
}

bool operator==(D2D1_RECT_F & op1, D2D1_RECT_F & op2)
{
	return op1.bottom == op2.bottom && op1.left == op2.left
		&& op1.right == op2.right && op1.top == op2.top;
}

bool operator!=(D2D1_RECT_F & op1, D2D1_RECT_F & op2)
{
	return op1.bottom != op2.bottom || op1.left != op2.left
		|| op1.right != op2.right || op1.top != op2.top;
}




/*
* Function: convertRECTToD2DRectF
* Purpose: Converts an MFC Rectangle to a D2D Rectangle
* Parameters: RECT - the Rectangle to Convert
* Returns: D2D1_RECT_F - the D2D Rectangle sought
*/
D2D1_RECT_F convertRECTToD2DRectF(RECT r)
{
	D2D1_RECT_F dRect;
	dRect.bottom = r.bottom;
	dRect.left = r.left;
	dRect.right = r.right;
	dRect.top = r.top;
	return dRect;
}

/*
* Function: convertD2DRectToRECT
* Purpose: Returns an MFC Rectangle from a Direct2D rectangle
* Parameters: D2D1_RECT_F f - the D2D Rectangle to convert
* Returns: RECT - the MFC rectangle created
*/
RECT convertD2DRectToRECT(D2D1_RECT_F f)
{
	RECT ret;
	ret.top = static_cast<LONG>(f.top);
	ret.left = static_cast<LONG>(f.left);
	ret.right = static_cast<LONG>(f.right);
	ret.bottom = static_cast<LONG>(f.bottom);
	return ret;
}

TPage::EventID_Cred::TPage::EventID_Cred()
{
	eventType = R_Message_Type::On_Click;
}

TPage::EventID_Cred::TPage::EventID_Cred(const TPage::EventID_Cred& copy)
{
	eventType = copy.eventType;
	control = copy.control;
	scroll = copy.scroll;
	textIntercepter = copy.textIntercepter;
}

TPage::EventID_Cred::TPage::EventID_Cred(R_Message_Type t, TrecPointer<TControl> c)
{
	eventType = t;
	control = c;
}

TPage::EventID_Cred::TPage::EventID_Cred(R_Message_Type t, TrecPointer<TControl> c, TrecPointer<TScrollBar> sb)
{
	eventType = t;
	control = c;
	scroll = sb;
}


TPage::EventID_Cred::TPage::EventID_Cred(R_Message_Type t, TrecPointer<TControl> c, TrecPointer<TTextIntercepter> i)
{
	eventType = t;
	control = c;
	textIntercepter = i;
}

TPage::EventID_Cred::TPage::EventID_Cred(TrecPointer<TFlyout> fly)
{
	eventType = R_Message_Type::On_Flyout;
	if (!fly.Get())
		throw L"Error! Needed initialized Flyout!";
	flyout = fly;
}

/**
 * Method: TControlParentHolder::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TControlParentHolder::GetType()
{
	return TString(L"TControlParentHolder;") + TParentHolder::GetType();
}

/**
 * Method: TControlParentHolder::TControlParentHolder
 * Purpose: Constructor
 * Parameters: TrecPointer<TControl> parent -  the TControl to serve as a Parent to TControls
 * Returns: New TControlParentHolder
 */
TControlParentHolder::TControlParentHolder(TrecPointer<TControl> parent)
{
	this->parent = TrecPointerKey::GetSoftPointerFromTrec<TControl>(parent);
}

/**
 * Method: TControlParentHolder::TControlParentHolder
 * Purpose: Constructor
 * Parameters: TrecPointerSoft<TControl> parent -  the TControl to serve as a Parent to TControls
 * Returns: New TControlParentHolder
 */
TControlParentHolder::TControlParentHolder(TrecPointerSoft<TControl> parent)
{
	this->parent = parent;
}

/**
 * Method: TControlParentHolder::SwitchChildControl
 * Purpose: Has the Control switch one of its child controls with a new one
 * Parameters: TrecPointerSoft<TControl> cur - the control making the call
 *				TrecPointer<TControl> newTControl - the Control to replace it with
 * Returns: void
 */
void TControlParentHolder::SwitchChildControl(TrecPointerSoft<TControl> cur, TrecPointer<TControl> newTControl)
{
	auto tParent = TrecPointerKey::GetTrecPointerFromSoft<TControl>(parent);
	if (tParent.Get())
	{
		tParent->SwitchChildControl(cur, newTControl);
	}
}

/**
 * Method: TControlParentHolder::GetParent
 * Purpose: Allows the Retrieval of the Parent Control (if the holder is holding a control)
 * Parameters: void
 * Returns: TrecPointer<TControl> - the Parent (the default returns null but the TControlParentHolder will return the parent)
 */
TrecPointer<TControl> TControlParentHolder::GetParent()
{
	return TrecPointerKey::GetTrecPointerFromSoft<TControl>(parent);
}

/**
 * Method: TControlParentHolder::IsScroller
 * Purpose: Reports to the Child whether the parent holding it is a Scroller Control
 * Parameters: void
 * Returns: bool - whether or not the parent is a Scroller Control
 */
bool TControlParentHolder::IsScroller()
{
	return dynamic_cast<TScrollerControl*>(parent.Get()) != nullptr;
}

EventTypeID::EventTypeID()
{
	eventType = R_Message_Type::On_Click;
	eventID = -1;
}


EventArgs::EventArgs()
{
	Reset();
	control = nullptr;
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
}

EventArgs::EventArgs(const EventArgs& args)
{
	text.Set(args.text);
	positive = args.positive;
	point = args.point;
	isClick = args.isClick;
	isLeftClick = args.isLeftClick;
	eventType = args.eventType;
	control = args.control;
	methodID = args.methodID;
	arrayLabel = args.arrayLabel;
	type = args.type;
	object = args.object;
}
