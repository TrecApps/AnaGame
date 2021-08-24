
#include "TTextField.h"
#include "TScrollerControl.h"
#include <atltrace.h>
#include <TTextRenderer.h>
#include <TComplexTextElement.h>

// Allows Anaface to keep track of where the caret is
static TDataArray<TTextField*> TextList;

/*
 * Method:
 * Purpose: 
 * Parameters:
 * Returns:
 */


 /**
  * Method: TTextField::GetType
  * Purpose: Returns a String Representation of the object type
  * Parameters: void
  * Returns: TString - representation of the object type
  */
TString TTextField::GetType()
{
	return TString(L"TTextField;") + TGadgetControl::GetType();
}

/*
* Method: TTextField::TTextField
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> rt - The Render Target to draw to
*				TrecPointer<TArray<styleTable>> ta - The Style Table to draw from
*				HWND winHand - the handle to the window so Windows Caret Support is possible
* Returns: New TTextField 
*/
TTextField::TTextField(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> st, HWND winHand) :
	TGadgetControl(rt,st,false)
{
	drawNumBoxes = true;
	bool notFound = true;
	for (UINT c = 0; c < TextList.Size(); c++)
	{
		if (!TextList[c])
		{
			TextList.setAt(this,c);
			notFound = false;
		}
	}

	if(notFound)
		TextList.push_back(this);
	windowHandle = winHand; isPassword = false;
	text.Empty();
	caretLoc = 0;
	onFocus = false;
	showPassword = false;
	circleCenter = TPoint(0, 0);
	radius = 0;
	isNumber = false;
	offerPasswordPeek = false;
	includeReturn = true;
	isComplex = false;
}

/*
* Method: TTextField::~TTextField
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TTextField::~TTextField()
{
	for (UINT c = 0; c < TextList.Size(); c++)
	{
		if (TextList[c] == this)
		{
			TextList.setAt(nullptr, c);
			break;
		}
	}

	for (UINT Rust = 0; Rust < colors.Size(); Rust++)
	{
		if (colors[Rust].colBrush)
			colors[Rust].colBrush->Release();
		colors[Rust].colBrush = nullptr;
	}
	colors.RemoveAll();
}

/*
* Method: TTextField::InputChar
* Purpose: Adds key stroke to the appropriate location in the string
* Parameters: wchar_t cha - the character to input
*				int times - the number of times to insert it
* Returns: void 
*/
void TTextField::InputChar(wchar_t cha, int times)
{
	TObjectLocker threadLock(&thread);
	if (!onFocus) // hopefully this is true, but just in case
	{
		
		return;
	}
	int curCaretLoc = caretLoc;

	for (int c = 0; c < times; c++)
	{
		switch (cha)
		{
		case VK_BACK:
			if (caretLoc > 0 && text.GetSize() > 0)
			{

				text.Delete(caretLoc - 1, 1);
				caretLoc--;
			}
			break;
		case VK_OEM_PERIOD:
			text.Insert(caretLoc++, L'.');
			break;
		case VK_SUBTRACT:
			if (cha == L'm')
				goto def;
			text.Insert(caretLoc++, L'-');
			break;
		case VK_DIVIDE:
			if (cha == L'o')
				goto def;
			text.Insert(caretLoc++, L'/');
			break;
		default:
		def:
			if(cha != VK_RETURN || includeReturn)
				text.Insert(caretLoc++, cha);
		}
	}
	

}

void TTextField::SetUpTextElement()
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

		text1 = isComplex || !isEditable ? TrecPointerKey::GetNewSelfTrecPointerAlt<TTextElement, TComplexTextElement>(drawingBoard, windowHandle, isEditable) :
			TrecPointerKey::GetNewSelfTrecPointerAlt<TTextElement, TInputTextElement>(drawingBoard, windowHandle);

		text1->SetLocation(location);
		text1->SetBasicFormatting(details);
		text1->SetHorizontallignment(hAlign);
		text1->SetVerticalAlignment(vAlign);
		text1->SetText(text);
	}
}

/*
* Method: TTextField::onClick
* Purpose: Checks to see if a Point falls within it's area space
* Parameters: CPoint cp - the point to check
* Returns: bool - whether the point is present or not
* Note: Check to see if this Method is used and whether it should be depreciated
*/
bool TTextField::onClick(TPoint cp)
{
	TObjectLocker threadLock(&thread);
	bool ret = (cp.x > location.left && cp.x < location.right && cp.y > location.top && cp.y < location.bottom);
	
	return ret;
}

/*
 * Method: TTextField::LockText
 * Purpose: Prevents the User from editing the control
 * Parameters: void
 * Returns: void
 */
void TTextField::LockText()
{
	TObjectLocker threadLock(&thread);
	isEditable = false;
	
}

/*
 * Method: TTextField::UnlockText
 * Purpose: Allows the User to edit the control
 * Parameters: void
 * Returns: void
 */
void TTextField::UnlockText()
{
	TObjectLocker threadLock(&thread);
	isEditable = true;
	
}

/*
* Method: TTextField::loadFromHTML
* Purpose: Loads the Text Element from HTML
* Parameters: CArchive * ar - File to read from
* Returns: int
* Note: DEPRECIATED - Functionality should be handled by a compatible Anaface-HTML parser
*/
int TTextField::loadFromHTML(TFile * ar)
{
	return 0;
}

/*
* Method: TTextField::storeInTML
* Purpose: Stores the control in a TML file
* Parameters: CArchive* ar - the file to save to
*				int childLevel - the generation if the TControl
*				bool overrideChildren - whether to ignore the children to save
* Returns: void
*/
void TTextField::storeInTML(TFile * ar, int childLevel, bool ov)
{
	TObjectLocker threadLock(&thread);
	TString appendable;
	resetAttributeString(&appendable, childLevel + 1);
	if (!isPassword)
	{
		appendable.Append(L"|IsPassword:False");
		_WRITE_THE_STRING;
	}
	else
	{
		appendable.Append(L"|IsPassword:True");
		_WRITE_THE_STRING;

		if (offerPasswordPeek)
		{
			appendable.Append(L"|PasswordPeek:True");
			_WRITE_THE_STRING;
		}
		else
		{
			appendable.Append(L"|PasswordPeek:False");
			_WRITE_THE_STRING;
		}
	}

	if (isEditable)
	{
		appendable.Append(L"|CanEdit:True");
		_WRITE_THE_STRING;
	}
	else
	{
		appendable.Append(L"|CanEdit:False");
		_WRITE_THE_STRING;
	}

	if (isNumber)
	{
		appendable.Append(L"|IsNumberControl:True");
		_WRITE_THE_STRING;

		appendable.Append(L"|DefaultIncriment:");
		if (incriment.type == iControlType::t_float)
		{
			appendable.AppendFormat(L"%f", incriment.value.f);
		}
		else if (incriment.type == iControlType::t_int)
			appendable.AppendFormat(L"%d", incriment.value.i);
		else
			appendable.AppendFormat(L"%d", 1);

		_WRITE_THE_STRING;
	}
	else
	{
		appendable.Append(L"|IsNumberControl:False");
		_WRITE_THE_STRING;
	}

	TControl::storeInTML(ar, childLevel);
	
}

/*
* Method: TTextField::storeInHTML
* Purpose: Stores the control in an HTML format
* Parameters: CArchive * ar - the file to write to
* Returns: void
*/
void TTextField::storeInHTML(TFile * ar)
{
}

/*
* Method: TTextField::onCreate
* Purpose: Sets up the TTextFeild with Text Specific attributes
* Parameters: RECT r - the location that the control would work in
* Returns: bool - success (currently arbitrarily)
*/
bool TTextField::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TObjectLocker threadLock(&thread);


	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|IsPassword"));
	isPassword = false;
	offerPasswordPeek = false;
	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"True"))
		{
			isPassword = true;
			valpoint = attributes.retrieveEntry(TString(L"|PasswordPeek"));
			if (valpoint.Get() &&   !valpoint->Compare(L"True"))
				offerPasswordPeek = true;
		}

	}
	isEditable = true;
	valpoint = attributes.retrieveEntry(TString(L"|CanEdit"));
	if (valpoint.Get() && !valpoint.Get()->Compare(L"False"))
		isEditable = false;

	valpoint = attributes.retrieveEntry(L"|BlockReturn");
	if (valpoint.Get() && !valpoint->CompareNoCase(L"true"))
		includeReturn = false;

	valpoint = attributes.retrieveEntry(TString(L"|IsNumberControl"));
	if (valpoint.Get() && !valpoint->Compare(L"True"))
	{
		isNumber = true;
	}

	valpoint = attributes.retrieveEntry(TString(L"|DrawNumberBoxes"));
	if (valpoint.Get() && !valpoint->Compare(L"False"))
		drawNumBoxes = false;

	valpoint = attributes.retrieveEntry(TString(L"|IsComplex"));
	if (valpoint.Get() && !valpoint->CompareNoCase(L"true"))
		isComplex = true;

	valpoint = attributes.retrieveEntry(TString(L"|Minimum"));
	int value = 0;
	float f_value = 0.0f;
	
	if (valpoint.Get() && !valpoint->ConvertToInt(value))
	{
		hasMin = true;
		minumum.type = iControlType::t_int;
		minumum.value.i = value;
	}
	else if (valpoint.Get() && !valpoint->ConvertToFloat(f_value))
	{
		hasMin = true;
		minumum.type = iControlType::t_float;
		minumum.value.f = f_value;
	}
	else
	{
		hasMin = false;
	}

	valpoint = attributes.retrieveEntry(TString(L"|Maximum"));
	
	if (valpoint.Get() && !valpoint->ConvertToInt(value))
	{
		hasMax = true;
		maximum.type = iControlType::t_int;
		maximum.value.i = value;
	}
	else if (valpoint.Get() && !valpoint->ConvertToFloat(f_value))
	{
		hasMax = true;
		maximum.type = iControlType::t_float;
		maximum.value.f = f_value;
	}
	else
	{
		hasMax = false;
	}
	incriment.type = iControlType::t_none;

	if (isNumber)
	{
		valpoint = attributes.retrieveEntry(TString(L"|DefaultIncriment"));
		if (valpoint.Get())
		{
			int i = 0;
			float f = 0.0;
			if (!valpoint->ConvertToFloat(f))
			{
				incriment.type = iControlType::t_float;
				incriment.value.f = f;
			}
			else if (!valpoint->ConvertToInt(i))
			{
				incriment.type = iControlType::t_int;
				incriment.value.i = i;
			}
			else
				goto otherwise;
		}
		else
		{
		otherwise:
			incriment.type = iControlType::t_int;
			incriment.value.i = 1;
		}
	}

	topBut = DxLocation;
	topBut.bottom = topBut.bottom - (topBut.bottom - topBut.top) / 2;
	botBut = DxLocation;
	botBut.top = botBut.top + (botBut.bottom - botBut.top) / 2;


	TGadgetControl::onCreate(r, d3d);

	if (isPassword || isNumber)
	{
		text1->SetHorizontallignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		text1->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	if (offerPasswordPeek)
	{
		passwordPeek_outer.point = D2D1::Point2F(DxLocation.left + ((DxLocation.right - DxLocation.left) / 2), DxLocation.top + ((DxLocation.bottom - DxLocation.top) / 2));
		passwordPeek_inner.point = passwordPeek_outer.point;

		passwordPeek_outer.radiusX = (DxLocation.right - DxLocation.left) / 2;
		passwordPeek_outer.radiusY = (DxLocation.bottom - DxLocation.top) / 2;

		passwordPeek_inner.radiusX = passwordPeek_outer.radiusX / 3;
		passwordPeek_inner.radiusY = passwordPeek_outer.radiusY / 3;

		circleCenter.x = passwordPeek_outer.point.x;
		circleCenter.y = passwordPeek_outer.point.y;
	}
	text1->GetText(text);

	for (UINT c = 0; c < details.Size(); c++)
	{
		text1->SetFormatting(details[c].details, details[c].range.startPosition, details[c].range.startPosition + details[c].range.length, true);
	}
	
	return true;
}

/*
* Method: TTextField::onDraw
* Purpose: Draws the text that it was given
* Parameters: TObject* obj - object used for databinding (unlikely to be used here)
* Returns: void
*/
void TTextField::onDraw(TObject* obj)
{	
	TObjectLocker threadLock(&thread);
	TControl::onDraw(obj);
	if (!isActive)
	{
		
		return;
	}
	else if (offerPasswordPeek)
	{
		brush->DrawEllipse(passwordPeek_outer);
		brush->FillEllipse(passwordPeek_inner);
	}
	else if (isNumber)
	{
		brush->DrawRectangle(topBut);
		brush->DrawRectangle(botBut);
	}

	// deal with passwords
	if (isPassword && !showPassword && text1.Get())
	{
		TString passText;
		for (UINT Rust = 0; Rust < text.GetSize(); Rust++)
			passText.AppendChar(L'*');
		text1->SetText(passText);
	}
	else if (isPassword && showPassword) 
	{
		updateTextString();
	}
	
}

/*
* Method: TTextField::determineMinHeightNeeded
* Purpose: Determines how high the control HAS to be that it was still functional
* Parameters: void
* Returns: UINT - The minimum height control needed
*/
UINT TTextField::determineMinHeightNeeded()
{
	TObjectLocker threadLock(&thread);
	UINT height = location.bottom - location.top;
	if (!text1.Get())
	{
		
		return height;
	}
	TrecComPointer<IDWriteTextLayout> fl = text1->GetLayout();
	if (!fl.Get())
	{
		
		return height;
	}
	DWRITE_TEXT_METRICS	dtm;
	
	dtm.height = 0;
	dtm.layoutHeight = 0;
	dtm.layoutWidth = 0;
	dtm.left = 0;
	dtm.lineCount = 0;
	dtm.maxBidiReorderingDepth = 0;
	dtm.top = 0;
	dtm.widthIncludingTrailingWhitespace = 0;
	dtm.width = location.right - location.left;
	fl->GetMetrics(&dtm);
	

	return dtm.height;
}

/*
* Method: TTextField::SetNewLocation
* Purpose: Sets a new location for the Control
* Parameters: RECT& r - the new location
* Returns: void
*/
void TTextField::SetNewLocation(const D2D1_RECT_F& r)
{
	TObjectLocker threadLock(&thread);
	TControl::SetNewLocation(r);
	
}

/**
 * Method: TTextField::getLocation
 * Purpose: Reports how much space this object will actually need to draw
 * Parameters: void
 * Returns: D2D1_RECT_F -  the Rectangle of the content that would be drawn (even if it was officially allocated less space)
 */
D2D1_RECT_F TTextField::getLocation()
{
	TObjectLocker threadLock(&thread);
	D2D1_RECT_F ret;
	
	if (!text1.Get())
		ret = location;
	else
	{
		DWRITE_TEXT_METRICS mets;
		text1->GetLayout()->GetMetrics(&mets);

		ret.left = location.left;
		ret.top = location.top;

		ret.bottom = ret.top + (max(mets.height, mets.layoutHeight));
		ret.right = ret.left + (max(mets.widthIncludingTrailingWhitespace, mets.layoutWidth));
	}
	
	return ret;
}

/*
* Method: TTextField::OnLButtonDown
* Purpose: Determines if a mouse click occured and where it should put the caret
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				CPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TTextField::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	TObjectLocker threadLock(&thread);
	resetArgs();

	if (!isEditable)
		goto parentCall;

	if (!text1->OnCLickDown(point))
		return;

parentCall:
	if (isContained(&point, &location) )
	{
		
		TTextField* feild = nullptr;
		for (UINT c = 0; c < TextList.Size(); c++)
		{
			feild = TextList[c];
			if (feild)
				feild->onFocus = false;
		}
		onFocus = true;
		if (offerPasswordPeek && isContained(point,passwordPeek_outer))
		{
			showPassword = true;
			
		}
		float f = 0.0;
		int i = 0;

//		if(!(text && text))

		if (isNumber && isContained(&point, &topBut))
		{
			updateNumber(true);
			*mOut = messageOutput::positiveContinueUpdate;
			if (hasEvent(R_Message_Type::On_Text_Change))
			{
				// Set args
				resetArgs();
				args.eventType = R_Message_Type::On_Text_Change;
				args.point = point;
				args.methodID = getEventID(R_Message_Type::On_Text_Change);
				args.isClick = true;
				args.isLeftClick = false;
				args.control = this;

				eventAr.push_back(EventID_Cred( R_Message_Type::On_Text_Change, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
			}
		}
		else if (isNumber && isContained(&point, &botBut))
		{
			updateNumber(false);
			*mOut = messageOutput::positiveContinueUpdate;
			if (hasEvent(R_Message_Type::On_Text_Change))
			{
				// Set args
				resetArgs();
				args.eventType = R_Message_Type::On_Text_Change;
				args.point = point;
				args.methodID = getEventID(R_Message_Type::On_Text_Change);
				args.isClick = true;
				args.isLeftClick = false;
				args.control = this;

				eventAr.push_back(EventID_Cred( R_Message_Type::On_Text_Change, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
			}
		}

		*mOut = messageOutput::positiveContinueUpdate;
	}
	TControl::OnLButtonDown(nFlags, point, mOut,eventAr, clickedControl);

	if (onFocus)
	{
		(text1->GetText(args.text));
	}
	

}


/*
* Method: TTextField::OnMouseMove
* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it, used by Text field to oversee the selection of text
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*/
void TTextField::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	TObjectLocker threadLock(&thread);
	if(text1->OnMouseMove(point))
		TControl::OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
	
}

/*
* Method: TTextField::OnLButtonUp
* Purpose: Responds to mouse clicks ending
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				CPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
void TTextField::OnLButtonUp(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr)
{
	TObjectLocker threadLock(&thread);
	if (isContained(&point, &location))
	{
		*mOut = messageOutput::positiveOverrideUpdate;

		//auto cThis = TrecPointerKey::GetTrecPointerFromSoft<>(tThis);

		//EventID_Cred cred(R_Message_Type::On_Focus, cThis, TrecPointerKey::GetNewTrecPointerAlt<TTextIntercepter,
		//	TTextFieldIntercepter>(TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(cThis)));
		//eventAr.push_back(cred);
	}
	showPassword = false;

	text1->OnCLickUp(point);
	TControl::OnLButtonUp(nFlags, point, mOut, eventAr);
	
}

/*
* Method: TTextField::AppendBoldText
* Purpose: Adds Bold Text at the end of the text
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendBoldText(const TString & t)
{
	//TObjectLocker threadLock(&thread);
	//UINT beginFormatting = text.GetSize();
	//if (!text1.Get())
	//{
	//	text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	//}
	//FormattingDetails fd;
	//fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	//fd.style = DWRITE_FONT_STYLE_NORMAL;
	//fd.weight = DWRITE_FONT_WEIGHT_BOLD;
	//details.push_back(fd);

	//text += t;
	//updateTextString();
	
}

/*
* Method: TTextField::AppendItalicText
* Purpose: Adds Italic text at the end of the text
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendItalicText(const TString & t)
{
	//TObjectLocker threadLock(&thread);
	//UINT beginFormatting = text.GetSize();
	//if (!text1.Get())
	//{
	//	text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	//}
	//FormattingDetails fd;
	//fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	//fd.style = DWRITE_FONT_STYLE_ITALIC;
	//fd.weight = DWRITE_FONT_WEIGHT_NORMAL;
	//details.push_back(fd);

	//text += t;
	//updateTextString();
	
}

/*
* Method: TTextField::AppendBoldItalicText
* Purpose: Adds Italic and Bold Text to the end
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendBoldItalicText(const TString & t)
{
	//TObjectLocker threadLock(&thread);
	//UINT beginFormatting = text.GetSize();
	//if (!text1.Get())
	//{
	//	text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	//}
	//FormattingDetails fd;
	//fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	//fd.style = DWRITE_FONT_STYLE_ITALIC;
	//fd.weight = DWRITE_FONT_WEIGHT_BOLD;
	//details.push_back(fd);

	//text += t;
	//updateTextString();
	
}

/*
* Method: TTextField::AppendNormalText
* Purpose: Adds Normal Text to the end of the current text
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendNormalText(const TString & t)
{
	//TObjectLocker threadLock(&thread);
	//UINT beginFormatting = text.GetSize();
	//if (!text1.Get())
	//{
	//	text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	//}
	//FormattingDetails fd;
	//fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	//fd.style = DWRITE_FONT_STYLE_NORMAL;
	//fd.weight = DWRITE_FONT_WEIGHT_NORMAL;
	//details.push_back(fd);

	//text += t;
	//updateTextString();
	
}

bool TTextField::ApplyFormatting(FormattingDetails ds)
{
	TObjectLocker threadLock(&thread);
	if ((ds.range.startPosition + ds.range.length) > text.GetSize())
	{
		
		return false;
	}
	this->details.push_back(ds);
	updateTextString();
	
	return true;
}



/*
* Method: TTextField::GetText
* Purpose: Retrieves the current text of the control
* Parameters: void
* Returns: TString - the current text held by the control
*/
TString TTextField::GetText()
{
	TObjectLocker threadLock(&thread);
	TString ret(text);
	
	return ret;
}

/*
* Method: TTextField::SetText
* Purpose: Sets the current text of the control
* Parameters: TString t - the string to set the text to
* Returns: void
*/
void TTextField::SetText(TString t )
{
	TObjectLocker threadLock(&thread);
	text.Set(t);
	updateTextString();
	Resize(location);
	

}

UCHAR * TTextField::GetAnaGameType()
{
	return nullptr;
}

/*
 * Method: TTextField::Resize
 * Purpose: resets the Size of the TextField
 * Parameters: D2D1_RECT_F& r - the location to set the text field to
 * Returns: void
 */
void TTextField::Resize(D2D1_RECT_F& r)
{
	TObjectLocker threadLock(&thread);
	TGadgetControl::Resize(r);
	topBut = DxLocation;
	topBut.bottom = topBut.bottom - (topBut.bottom - topBut.top) / 2;
	botBut = DxLocation;
	botBut.top = botBut.top + (botBut.bottom - botBut.top) / 2;

	if (offerPasswordPeek)
	{
		passwordPeek_outer.point = D2D1::Point2F(DxLocation.left + ((DxLocation.right - DxLocation.left) / 2), DxLocation.top + ((DxLocation.bottom - DxLocation.top) / 2));
		passwordPeek_inner.point = passwordPeek_outer.point;

		passwordPeek_outer.radiusX = (DxLocation.right - DxLocation.left) / 2;
		passwordPeek_outer.radiusY = (DxLocation.bottom - DxLocation.top) / 2;

		passwordPeek_inner.radiusX = passwordPeek_outer.radiusX / 3;
		passwordPeek_inner.radiusY = passwordPeek_outer.radiusY / 3;

		circleCenter.x = passwordPeek_outer.point.x;
		circleCenter.y = passwordPeek_outer.point.y;
	}
	
	if (text1.Get() && parent.Get() && !parent->IsScroller())
	{
		text1->SetLocation(location);
		D2D1_RECT_F textLoc = this->getLocation();
		if ((textLoc.right - textLoc.left > location.right - location.left + 2.0f)
			|| (textLoc.bottom - textLoc.top > location.bottom - location.top + 2.0f))
		{
			TrecPointer<TControl> scrollControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TScrollerControl>(drawingBoard, styles);
			scrollControl->onCreate(r, TrecPointer<TWindowEngine>());

			auto oldParent = parent;

			dynamic_cast<TScrollerControl*>(scrollControl.Get())->SetChildControl(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));
			oldParent->SwitchChildControl(tThis, scrollControl);
		}
	}
	

}

/*
* Method: TTextField::updateNumber
* Purpose: Updates the Number string by a predefined incriment 
* Parameters: bool pos - whether moving the number up or down
* Returns: void
*/
void TTextField::updateNumber(bool pos)
{
	int i = 0;
	float f = 0.0;
	short floatResult =0 , intResult = 0;
	TObjectLocker threadLock(&thread);
	if (text.ConvertToFloat(f) && text.ConvertToInt(i))
		setToZero();
	
	intResult = text.ConvertToInt(i);
	floatResult = text.ConvertToFloat(f);
	
	if (pos)
	{
		// moving the number up
		if (incriment.type == iControlType::t_int)
		{
			if (!floatResult)
			{
				f = f + static_cast<float>(incriment.value.i);
				setNumericText(f);
			}
			else if (!intResult)
			{
				i = i + incriment.value.i;
				setNumericText(i);
			}
		}
		else if(incriment.type == iControlType::t_float)
		{
			if (!floatResult)
			{
				f = f + incriment.value.f;
				setNumericText(f);
			}
			else if (!intResult)
			{
				f = static_cast<float>(i) + incriment.value.f;
				setNumericText(f);
			}
		}
	}
	else
	{
		// moving the number up
		if (incriment.type == iControlType::t_int)
		{
			if (!floatResult)
			{
				f = f - static_cast<float>(incriment.value.i);
				setNumericText(f);
			}
			else if (!intResult)
			{
				i = i - incriment.value.i;
				setNumericText(i);
			}
		}
		else if (incriment.type == iControlType::t_float)
		{
			if (!floatResult)
			{
				f = f - incriment.value.f;
				setNumericText(f);
			}
			else if (!intResult)
			{
				f = static_cast<float>(i) - incriment.value.f;
				setNumericText(f);
			}
		}
	}
	updateTextString();
	
}

/*
* Method: TTextField::setToZero
* Purpose: Sets a number oriented TextField to 0
* Parameters: void
* Returns: void
*/
void TTextField::setToZero()
{
	TObjectLocker threadLock(&thread);
	text.Empty();
	text.AppendChar(L'0');
	
}

/*
* Method: TTextField::setNumericText
* Purpose: Sets the TextField to an integer value
* Parameters: int i - the number to set control to
* Returns: void
*/
void TTextField::setNumericText(int i)
{
	TObjectLocker threadLock(&thread);
	if ((hasMax && i > maximum) || (hasMin && i < minumum))
	{
		
		return;
	}
	
	text.Empty();
	text.Format(L"%d", i);

	if (text1.Get())
		text1->SetText(text);
	
}

/*
* Method: TTextField::setNumericText
* Purpose: Sets the TextField to a floating point value
* Parameters: float f - the number to set control to
* Returns: void
*/
void TTextField::setNumericText(float f)
{
	TObjectLocker threadLock(&thread);
	if ((hasMax && f > maximum) || (hasMin && f < minumum))
	{
		
		return;
	}

	text.Empty();
	text.Format(L"%f", f);

	if (text1.Get())
		text1->SetText(text);
	
}


/*
 * Method: TTextField::AddColorEffect
 * Purpose: Adds a color effect to the text presented
 * Parameters: D2D1_COLOR_F col - the color to set the text
 *				UINT start - the starting index to apply the color
 *				UINT length - the number of characters to apply the color to
 * Returns: void
 */
void TTextField::AddColorEffect(D2D1_COLOR_F col, UINT start, UINT length)
{
	TObjectLocker threadLock(&thread);
	if (!text1.Get() || !drawingBoard.Get())
	{
		
		return;
	}
	IDWriteTextLayout* layout = text1->GetLayout().Get();
	if (!layout)
	{
		
		return;
	}
	for (UINT C = 0; C < colors.Size(); C++)
	{
		if (col == colors[C].color)
		{
			layout->SetDrawingEffect(colors[C].colBrush, DWRITE_TEXT_RANGE{ start, length });
			
			return;
		}
	}

	ColorEffect ce{ nullptr, col };

	HRESULT h = drawingBoard->GetRenderer()->CreateSolidColorBrush(col, &ce.colBrush);
	if (SUCCEEDED(h))
	{
		colors.push_back(ce);
		layout->SetDrawingEffect(ce.colBrush, DWRITE_TEXT_RANGE{ start, length });
	}
	
}


/*
 * Method: TTextField::RemoveFocus
 * Purpose: Removes the User focus from this control
 * Parameters: void
 * Returns: void
 */
void TTextField::RemoveFocus()
{
	TTextField* feild = nullptr;
	for (UINT c = 0; c < TextList.Size(); c++)
	{
		feild = TextList[c];
		if (feild)
		{
			feild->ThreadLock();
			feild->onFocus = false;
			feild->ThreadRelease();
		}
	}
}

TrecPointer<LineMetrics> TTextField::GetLineMetrics()
{
	TObjectLocker threadLock(&thread);
	if (text1.Get() && text1->GetLayout().Get())
	{
		TrecPointer<LineMetrics> ret = TrecPointerKey::GetNewTrecPointer<LineMetrics>(5);
		HRESULT res = text1->GetLayout()->GetLineMetrics(ret->metrics.data(), ret->metrics.Size(), &ret->sizeNeeded);
		if (res == E_NOT_SUFFICIENT_BUFFER)
		{
			ret->SetSize(ret->sizeNeeded);
			text1->GetLayout()->GetLineMetrics(ret->metrics.data(), ret->metrics.Size(), &ret->sizeNeeded);
			
			return ret;
		}
	}
	
	return TrecPointer<LineMetrics>();
}

void TTextField::ShrinkHeight()
{
	if (text1.Get())
	{
		bool worked;
		float height = text1->GetMinHeight(worked);
		if (worked) {
			location.bottom = location.top + height;
			text1->SetLocation(location);
			updateTextString();
		}
	}
}

float TTextField::GetMinWidth()
{
	bool w;
	if (text1.Get())
		return text1->GetMinWidth(w);
	return 0.0f;
}

/*
* Method: TTextField::updateTextString
* Purpose: Refreshes the Text string formating
* Parameters: void
* Returns: void
*/
void TTextField::updateTextString()
{
	TObjectLocker threadLock(&thread);

	
}

/*
* Method: TTextField::moveCaretLeft
* Purpose: Shifts the caret to the left in the text
* Parameters: CPoint point - the point to test
* Returns:void
*/
void TTextField::moveCaretLeft(POINT point)
{
	//BOOL isInside = true;
	//BOOL isTrailing = FALSE;
	//DWRITE_HIT_TEST_METRICS metric;
	//TObjectLocker threadLock(&thread);
	//text1->fontLayout->HitTestPoint(point.x - 1, point.y + 1, &isTrailing, &isInside, &metric);
	//if (isInside)
	//{
	//	HideCaret(windowHandle);
	//	SetCaretPos(metric.left, point.y);
	//	ShowCaret(windowHandle);
	//}
	
}

/*
* Method: TTextField::moveCaretRight
* Purpose: Shifts the Caret to the right in the text
* Parameters: CPoint point - the point to test
* Returns: void
*/
void TTextField::moveCaretRight(POINT point)
{
	//BOOL isInside = true;
	//BOOL isTrailing = FALSE;
	//DWRITE_HIT_TEST_METRICS metric;	
	//TObjectLocker threadLock(&thread);
	//text1->fontLayout->HitTestPoint(point.x + 1, point.y + 1, &isTrailing, &isInside, &metric);
	//if (isInside)
	//{
	//	HideCaret(windowHandle);
	//	SetCaretPos(metric.left + metric.width, point.y);
	//	ShowCaret(windowHandle);
	//}
	
}

/*
* Method: TTextField::moveCaretUp
* Purpose: Shifts the Caret to the up in the text
* Parameters: CPoint point - the point to test
* Returns: void
*/
void TTextField::moveCaretUp(POINT point)
{
	//BOOL isInside = true;
	//BOOL isTrailing = FALSE;
	//DWRITE_HIT_TEST_METRICS metric;
	//TObjectLocker threadLock(&thread);
	//text1->fontLayout->HitTestPoint(point.x, point.y, &isTrailing, &isInside, &metric);
	//float origTop = metric.top;

	//for (int c = point.y - 1; c > point.y - 200 && c > 0;c--)
	//{
	//	text1->fontLayout->HitTestPoint(point.x, c, &isTrailing, &isInside, &metric);
	//	if (isInside && metric.top < origTop)
	//	{
	//		HideCaret(windowHandle);
	//		if (isTrailing)
	//			point.x = metric.left;
	//		else
	//			point.x = metric.left + metric.width;

	//		SetCaretPos(point.x, metric.top);
	//		ShowCaret(windowHandle);
	//		break;
	//	}
	//}
	
}

/*
* Method: TTextField::moveCaretDown
* Purpose: Shifts the Caret to the down in the text
* Parameters: CPoint point - the point to test
* Returns: void
*/
void TTextField::moveCaretDown(POINT point)
{
	//BOOL isInside = true;
	//BOOL isTrailing = FALSE;
	//DWRITE_HIT_TEST_METRICS metric;
	//TObjectLocker threadLock(&thread);
	//text1->fontLayout->HitTestPoint(point.x, point.y, &isTrailing, &isInside, &metric);
	//float origTop = metric.top;

	//for (int c = point.y + metric.height; c < point.y + 200 ;c++)
	//{
	//	text1->fontLayout->HitTestPoint(point.x, c, &isTrailing, &isInside, &metric);
	//	if (isInside && metric.top > origTop)
	//	{
	//		HideCaret(windowHandle);
	//		if (isTrailing)
	//			point.x = metric.left;
	//		else
	//			point.x = metric.left + metric.width;

	//		SetCaretPos(point.x, metric.top);
	//		ShowCaret(windowHandle);
	//		break;
	//	}
	//}
	
}





///////////////////// Overloaded operators for IncrimentControl

/*
* Function: operator>
* Purpose:  Compares two incrimentControls for >
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator>(incrimentControl& ic1, incrimentControl& ic2)
{
	if (ic1.type == iControlType::t_int)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.i > ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.i > ic2.value.f;
		else return false;
	}
	else if(ic1.type == iControlType::t_float)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.f > ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.f > ic2.value.f;
		else return false;
	}

	return false;
}

/*
* Function: operator>
* Purpose: Compares an incrimentControl to an int for >
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator>(incrimentControl& ic, int i)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i > i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f > i;
	else return false;
}

/*
* Function: operator>
* Purpose: Compares an incrimentControl to an int for >
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator>(incrimentControl& ic, float f)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i > f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f > f;
	else return false;
}

/*
* Function: operator>
* Purpose: Compares an incrimentControl to an int for >
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>(int i, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i < i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f < i;
	else return false;
}

/*
* Function: operator>
* Purpose: Compares an incrimentControl to a float for >
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>(float f, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i < f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f < f;
	else return false;
}


/*
* Function: operator>
* Purpose:  Compares two incrimentControls for <
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator<(incrimentControl& ic1, incrimentControl& ic2)
{
	if (ic1.type == iControlType::t_int)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.i < ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.i < ic2.value.f;
		else return false;
	}
	else if (ic1.type == iControlType::t_float)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.f < ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.f < ic2.value.f;
		else return false;
	}

	return false;
}

/*
* Function: operator<
* Purpose: Compares an incrimentControl to an int for <
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator<(incrimentControl& ic, int i)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i < i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f < i;
	else return false;
}

/*
* Function: operator<
* Purpose:
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator<(incrimentControl& ic, float f)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i < f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f < f;
	else return false;
}

/*
* Function: operator<
* Purpose: Compares an incrimentControl to an int for <
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<(int i, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i > i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f > i;
	else return false;
}

/*
* Function: operator<
* Purpose:
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<(float f, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i > f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f > f;
	else return false;
}


/*
* Function: operator>=
* Purpose:  Compares two incrimentControls for >=
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator>=(incrimentControl& ic1, incrimentControl& ic2)
{
	if (ic1.type == iControlType::t_int)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.i >= ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.i >= ic2.value.f;
		else return false;
	}
	else if (ic1.type == iControlType::t_float)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.f >= ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.f >= ic2.value.f;
		else return false;
	}

	return false;
}

/*
* Function:operator>=
* Purpose: Compares an incrimentControl to an int for >=
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(incrimentControl& ic, int i)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i >= i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f >= i;
	else return false;
}

/*
* Function: operator>=
* Purpose:
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(incrimentControl& ic, float f)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i >= f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f >= f;
	else return false;
}

/*
* Function: operator>=
* Purpose: Compares an incrimentControl to an int for >=
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(int i, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i <= i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f <= i;
	else return false;
}

/*
* Function: operator>=
* Purpose:
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(float f, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i <= f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f <= f;
	else return false;
}


/*
* Function: operator<=
* Purpose: Compares two incrimentControls for <=
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator<=(incrimentControl& ic1, incrimentControl& ic2)
{
	if (ic1.type == iControlType::t_int)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.i <= ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.i <= ic2.value.f;
		else return false;
	}
	else if (ic1.type == iControlType::t_float)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.f <= ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.f <= ic2.value.f;
		else return false;
	}

	return false;
}

/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(incrimentControl& ic, int i)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i <= i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f <= i;
	else return false;
}

/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(incrimentControl& ic, float f)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i <= f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f <= f;
	else return false;
}

/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(int i, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i >= i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f >= i;
	else return false;
}

/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(float f, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i >= f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f >= f;
	else return false;
}


/*
* Function: operator==
* Purpose: Compares two incrimentControls for equality
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - whether the values are equal
*/
bool operator==(incrimentControl& ic1, incrimentControl& ic2)
{
	if (ic1.type == iControlType::t_int)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.i == ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.i == ic2.value.f;
		else return false;
	}
	else if (ic1.type == iControlType::t_float)
	{
		if (ic2.type == iControlType::t_int)
			return ic1.value.f == ic2.value.i;
		else if (ic2.type == iControlType::t_float)
			return ic1.value.f == ic2.value.f;
		else return false;
	}

	return false;
}

/*
* Function: operator==
* Purpose: Compares an incrimentControl to an int for equality
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - whether the values are equal
*/
bool operator==(incrimentControl& ic, int i)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i == i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f == i;
	else return false;
}

/*
* Function: operator==
* Purpose: Compares an incrimentControl to a float for equality
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - whether the values are equal
*/
bool operator==(incrimentControl& ic, float f)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i == f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f == f;
	else return false;
}

/*
* Function: operator==
* Purpose: Compares an incrimentControl to an int for equality
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - whether the values are equal
*/
bool operator==(int i, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i == i;
	else if (ic.type == iControlType::t_float)
		return ic.value.f == i;
	else return false;
}

/*
* Function: operator==
* Purpose: Compares an incrimentControl to a float
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - whether the values are equal
*/
bool operator==(float f, incrimentControl& ic)
{
	if (ic.type == iControlType::t_int)
		return ic.value.i == f;
	else if (ic.type == iControlType::t_float)
		return ic.value.f == f;
	else return false;
}

/*
* Function: operator==
* Purpose: Compares two colors
* Parameters: D2D1_COLOR_F & c1 - the first color to compare
*				D2D1_COLOR_F & c2 - the second color to compare
* Returns: bool - whether the values are equal
*/
bool operator==(D2D1_COLOR_F & c1, D2D1_COLOR_F & c2)
{
	return c1.r == c2.r && c1.g == c2.b 
		&& c1.b == c2.b && c1.a == c2.a;
}

/*
* Function: incrimentControl::operator=
* Purpose: Assigns an incrimentControl to this incrimentControl
* Parameters: incrimentControl & ic - the incrimentControl value to assign to this incrimentControl
* Returns: void
*/
void incrimentControl::operator=(incrimentControl & ic)
{
	type = ic.type;
	if (type == iControlType::t_int)
		value.i = ic.value.i;
	else if (type == iControlType::t_float)
		value.f = ic.value.f;
}

/*
* Function: incrimentControl::operator=
* Purpose: Assigns an int to this incrimentControl
* Parameters: int i - the int value to assign to this incrimentControl
* Returns: void
*/
void incrimentControl::operator=(int i)
{
	type = iControlType::t_int;
	value.i = i;
}

/*
* Function: incrimentControl::operator=
* Purpose: Assigns a float to this incrimentControl
* Parameters: float f - the float value to assign to this incrimentControl
* Returns: void
*/
void incrimentControl::operator=(float f)
{
	type = iControlType::t_float;
	value.f = f;
}

FormattingDetails::FormattingDetails()
{
	this->range = { 0,0 };
}

FormattingDetails::FormattingDetails(const FormattingDetails& copy)
{
	this->details = copy.details;
	this->range = copy.range;
}


LineMetrics::LineMetrics()
{
	sizeNeeded = 0;
}

LineMetrics::LineMetrics(const LineMetrics& orig)
{
	sizeNeeded = orig.sizeNeeded ? orig.sizeNeeded : orig.metrics.Size();
	for (UINT Rust = 0; Rust < sizeNeeded; Rust++)
	{
		auto met = orig.metrics.data()[Rust];
		metrics.push_back(met);
	}
}

LineMetrics::LineMetrics(USHORT i)
{
	SetSize(true);
}

void LineMetrics::SetSize(UINT i, bool fromConstructor)
{
	DWRITE_LINE_METRICS met;
	sizeNeeded = fromConstructor ? 0 : i;
	ZeroMemory(&met, sizeof(met));
	metrics.RemoveAll();
	for (UINT Rust = 0; Rust < i; Rust++)
		metrics.push_back(met);
}

TTextFieldIntercepter::TTextFieldIntercepter(TrecSubPointer<TControl, TTextField> control)
{
	this->textControl = control;
}

void TTextFieldIntercepter::OnChar(UINT ch, UINT count, UINT flags)
{
	if (textControl.Get())
		textControl->InputChar(ch, count);
}

void TTextFieldIntercepter::OnLoseFocus()
{
}

void TTextFieldIntercepter::OnCopy()
{
}

void TTextFieldIntercepter::OnCut()
{
}
