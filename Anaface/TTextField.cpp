
#include "TTextField.h"
#include <atltrace.h>

// Allows Anaface to keep track of where the caret is
static TDataArray<TTextField*> TextList;

/*
 * Method:
 * Purpose: 
 * Parameters:
 * Returns:
 */

/*
* Method: TTextField::TTextField
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> rt - The Render Target to draw to
*				TrecPointer<TArray<styleTable>> ta - The Style Table to draw from
*				HWND winHand - the handle to the window so Windows Caret Support is possible
* Returns: New TTextField 
*/
TTextField::TTextField(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> st, HWND winHand) :
	TGadgetControl(rt,st,false),
	highlighter(rt)
{
	isTextControl = drawNumBoxes = true;
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
	if (!onFocus) // hopefully this is true, but just in case
		return;

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
		case VK_DELETE:
			if (text.GetSize() > 0)
			{

				text.Delete(caretLoc, 1);

			}
			break;
		default:
		def:
			text.Insert(caretLoc++, cha);

		}
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
	if (cp.x > location.left && cp.x < location.right && cp.y > location.top && cp.y < location.bottom)
	{
		// TO-DO: figure out how to set caret

		return true;
	}
	return false;
}

/*
 * Method: TTextField::LockText
 * Purpose: Prevents the User from editing the control
 * Parameters: void
 * Returns: void
 */
void TTextField::LockText()
{
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
	//_Unreferenced_parameter_(ov);

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

	TGadgetControl::onCreate(r, d3d);

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

	valpoint = attributes.retrieveEntry(TString(L"|IsNumberControl"));
	if (valpoint.Get() && !valpoint->Compare(L"True"))
	{
		isNumber = true;
	}

	valpoint = attributes.retrieveEntry(TString(L"|DrawNumberBoxes"));
	if (valpoint.Get() && !valpoint->Compare(L"False"))
		drawNumBoxes = false;

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

	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, nullptr);
		int res = text1->onCreate(r);
		res = res;
	}

	if (isPassword || isNumber)
	{
		text1->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		text1->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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
	// in this class, text2 and text3 could actually get in the way, so remove
	// them if normal creation process allocated them.
	//if (text2.Get())
	//	text2 = null<TText>();
	//if (text3.Get())
	//	text3 = null<TText>();

	if (!text1.Get())
		return false;

	IDWriteTextLayout* layout = text1->fontLayout.Get();
	text = text1->text;
	if (!layout)
		return false;
	for (int c = 0; c < details.Size(); c++)
	{
		layout->SetFontStyle(details[c].style, details[c].range);
		layout->SetFontWeight(details[c].weight, details[c].range);
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

	TControl::onDraw(obj);
	if (!isActive)
		return;

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
	if (isPassword && !showPassword && text1.Get() && text1->text.GetSize())
	{
		int len = text1->text.GetSize();
		text1->text.Empty();
		for (int c = 0; c < len;c++)
		{
			text1->text += L'*';
			
		}
		text1->reCreateLayout();
		highlighter.SetLayout(text1->fontLayout);
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
	UINT height = location.bottom - location.top;
	if (!text1.Get())
		return height;
	
	TrecComPointer<IDWriteTextLayout> fl = text1->fontLayout;
	if (!fl.Get())
		return height;

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
	TControl::SetNewLocation(r);

	if (!text1.Get())
		return;

	IDWriteTextLayout* layout = text1->fontLayout.Get();
	if (!layout)
		return ;
	for (int c = 0; c < details.Size(); c++)
	{
		layout->SetFontStyle(details[c].style, details[c].range);
		layout->SetFontWeight(details[c].weight, details[c].range);
	}
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
	resetArgs();
	
	BOOL trailing = false, isInside = false;
	DWRITE_HIT_TEST_METRICS metrics;
	if (!isEditable)
		goto parentCall;

	text1->fontLayout->HitTestPoint(point.x - location.left, point.y - location.top, &trailing, &isInside, &metrics);

	if (isInside && isContained(&point, &location))
	{
		if (onFocus)
			DestroyCaret();

		CreateCaret(windowHandle, NULL, 1, metrics.height);



		if (trailing)
		{
			caretLoc = metrics.textPosition+ 1;
			SetCaretPos(metrics.left + metrics.width + location.left, metrics.top + location.top);
		}
		else
		{
			caretLoc = metrics.textPosition;
			SetCaretPos(metrics.left + location.left, metrics.top + location.top);
		}

		ShowCaret(windowHandle);
		clickedControl.push_back(this);
		if(highlighter.Reset(caretLoc))
			highlighter.SetFirstPosition(caretLoc);
		
		TTextField* feild = nullptr;
		for (int c = 0; c < TextList.Size(); c++)
		{
			feild = TextList[c];
			if (feild)
				feild->onFocus = false;
		}
		onFocus = true;
	}
	else
	{
		if (onFocus)
		{
			DestroyCaret();
			onFocus = false;
		}
		else if (isContained(&point, &location) && !text.GetSize() && text1.Get())
		{
			// Here, he need to have the caret but we don't have a string to place it against
			// Need to make educated guess on where it should go
			TPoint caretPoint;
			switch (text1->getHorizontalAlignment())
			{
			case DWRITE_TEXT_ALIGNMENT_CENTER:
				caretPoint.x = (location.left + location.right) / 2.0f;
				break;
			case DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
			case DWRITE_TEXT_ALIGNMENT_LEADING:
				caretPoint.x = location.left + 1;
				break;
			case DWRITE_TEXT_ALIGNMENT_TRAILING:
				caretPoint.x = location.right - 1;
			default:
				break;
			}

			switch (text1->getVerticalAlignment())
			{
			case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
				caretPoint.y = location.top;
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_CENTER:
				caretPoint.y = (location.top + (location.bottom - metrics.height)) / 2.0f;
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
				caretPoint.y = (location.bottom - metrics.height);
			}

			if (onFocus)
				DestroyCaret();
			CreateCaret(windowHandle, nullptr, 1, metrics.height);
			SetCaretPos(caretPoint.x, caretPoint.y);
			ShowCaret(windowHandle);
			clickedControl.push_back(this);
			// highlighter.SetFirstPosition(caretLoc);
		}
	}

parentCall:
	if (isContained(&point, &location) )
	{
		
		TTextField* feild = nullptr;
		for (int c = 0; c < TextList.Size(); c++)
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
		args.text.Set(text1->getCaption());
	}
}

/*
* Method: TTextField::OnChar
* Purpose: Adds a character to the String
* Parameters: bool fromChar - can be called either from on Key Down or OnChar
*				UINT nChar - The ID of the character that was pressed
*				UINT nRepCnt - how many times the character was processed for this event
*				UINT nFlags - flags provided by MFC's Message system, not used
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
bool TTextField::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	// To-Do: sort out anomalies with characters
	resetArgs();

	if (onFocus)
	{
		if (fromChar)
		{
			InputChar(static_cast<WCHAR>(LOWORD(nChar)), nRepCnt);
			
		}
		else
		{
			POINT caretPoint;
			for (int c = 0; c < nRepCnt;c++)
			{
				switch (nChar)
				{
				case VK_LEFT:
					if (caretLoc > 0)
						caretLoc--;
					if (GetCaretPos(&caretPoint))
					{
						moveCaretLeft(caretPoint);
					}
					break;
				case VK_RIGHT:
					if (caretLoc < text.GetSize())
						caretLoc++;
					if (GetCaretPos(&caretPoint))
					{
						moveCaretRight(caretPoint);
					}
					break;
				case VK_UP:
					if (GetCaretPos(&caretPoint))
					{
						moveCaretUp(caretPoint);
					}
					break;
				case VK_DOWN:
					if (GetCaretPos(&caretPoint))
					{
						moveCaretDown(caretPoint);
					}
					break;
				case VK_ESCAPE:
				case VK_HOME:
				case VK_RETURN:
				case VK_END:
					break;

				}
			}
		}
		*mOut = messageOutput::positiveOverrideUpdate;
		updateTextString();
		resetArgs();
		args.eventType = R_Message_Type::On_Char;
		args.control = this;
		args.methodID = getEventID(R_Message_Type::On_Char);
		args.type = static_cast<WCHAR>(LOWORD(nChar));
		args.control = this;
		eventAr.push_back({ R_Message_Type::On_Text_Change, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis) });

		if (text1.Get() && text1->text.GetSize())
		{
			args.text.Set(text1->text);
			text1->reCreateLayout();
			highlighter.SetLayout(text1->fontLayout);
		}
	}
	return onFocus;
}

/*
* Method: TTextField::OnMouseMove
* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it, used by Text field to oversee the selection of text
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
void TTextField::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ATLTRACE(L"TEXT FILED ONMOVEMOUSECALLED\n");
	TControl::OnMouseMove(nFlags, point, mOut, eventAr);

	if (isContained(&point, &location))
	{
		ATLTRACE(L"CHECKING HILIGHTER STATUS\n");
		if (highlighter.IsActive())
		{
			ATLTRACE(L"HILIGHTER STATUS IS GO GO GO!\n");
			BOOL trailing = false, isInside = false;
			DWRITE_HIT_TEST_METRICS metrics;

			text1->fontLayout->HitTestPoint(point.x - location.left, point.y - location.top, &trailing, &isInside, &metrics);
			HideCaret(windowHandle);

			UINT locCarLoc = 0;
			if (trailing)
			{
				locCarLoc = metrics.textPosition + 1;
				SetCaretPos(metrics.left + metrics.width + location.left, metrics.top + location.top);
			}
			else
			{
				locCarLoc = metrics.textPosition;
				SetCaretPos(metrics.left + location.left, metrics.top + location.top);
			}

			highlighter.SetSecondPosition(caretLoc = locCarLoc);

			ShowCaret(windowHandle);
			if (*mOut == messageOutput::positiveContinue)
				*mOut = messageOutput::positiveContinueUpdate;
			else if (*mOut == messageOutput::positiveOverride)
				*mOut = messageOutput::positiveOverrideUpdate;
		}
	}
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
	if (isContained(&point, &location))
	{
		*mOut = messageOutput::positiveOverrideUpdate;
	}
	showPassword = false;

	BOOL trailing = false, isInside = false;
	DWRITE_HIT_TEST_METRICS metrics;

	text1->fontLayout->HitTestPoint(point.x - location.left, point.y - location.top, &trailing, &isInside, &metrics);
	
	UINT locCarLoc = 0;
	if (trailing)
	{
		locCarLoc = metrics.textPosition + 1;
		
	}
	else
	{
		locCarLoc = metrics.textPosition;
		
	}
	if (isInside)
		highlighter.ResetUp(locCarLoc);
}

/*
* Method: TTextField::AppendBoldText
* Purpose: Adds Bold Text at the end of the text
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendBoldText(const TString & t)
{
	UINT beginFormatting = text.GetSize();
	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	}
	formattingDetails fd;
	fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	fd.style = DWRITE_FONT_STYLE_NORMAL;
	fd.weight = DWRITE_FONT_WEIGHT_BOLD;
	details.push_back(fd);

	text += t;
	updateTextString();
}

/*
* Method: TTextField::AppendItalicText
* Purpose: Adds Italic text at the end of the text
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendItalicText(const TString & t)
{
	UINT beginFormatting = text.GetSize();
	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	}
	formattingDetails fd;
	fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	fd.style = DWRITE_FONT_STYLE_ITALIC;
	fd.weight = DWRITE_FONT_WEIGHT_NORMAL;
	details.push_back(fd);

	text += t;
	updateTextString();
}

/*
* Method: TTextField::AppendBoldItalicText
* Purpose: Adds Italic and Bold Text to the end
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendBoldItalicText(const TString & t)
{
	UINT beginFormatting = text.GetSize();
	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	}
	formattingDetails fd;
	fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	fd.style = DWRITE_FONT_STYLE_ITALIC;
	fd.weight = DWRITE_FONT_WEIGHT_BOLD;
	details.push_back(fd);

	text += t;
	updateTextString();
}

/*
* Method: TTextField::AppendNormalText
* Purpose: Adds Normal Text to the end of the current text
* Parameters: TString& t - the text to add
* Returns: void
*/
void TTextField::AppendNormalText(const TString & t)
{
	UINT beginFormatting = text.GetSize();
	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	}
	formattingDetails fd;
	fd.range = { beginFormatting, static_cast<UINT>(t.GetSize()) };
	fd.style = DWRITE_FONT_STYLE_NORMAL;
	fd.weight = DWRITE_FONT_WEIGHT_NORMAL;
	details.push_back(fd);

	text += t;
	updateTextString();
}

/*
* Method: TTextField::isOnFocus
* Purpose: Reports whether focus is on the current control
* Parameters: void
* Returns: bool - whether focus is on the control
*/
bool TTextField::isOnFocus()
{
	return onFocus;
}

/*
* Method: TTextField::GetText
* Purpose: Retrieves the current text of the control
* Parameters: void
* Returns: TString - the current text held by the control
*/
TString TTextField::GetText()
{
	return text;
}

/*
* Method: TTextField::SetText
* Purpose: Sets the current text of the control
* Parameters: TString t - the string to set the text to
* Returns: void
*/
void TTextField::SetText(TString t )
{
	text = t;
	updateTextString();
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
	if (hasMax && i > maximum)
		return;
	if (hasMin && i < minumum)
		return;
	
	text.Empty();
	text.Format(L"%d", i);

	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
		int res = text1->onCreate(location);
	}
	if (text1.Get())
		text1->setCaption(text);
}

/*
* Method: TTextField::setNumericText
* Purpose: Sets the TextField to a floating point value
* Parameters: float f - the number to set control to
* Returns: void
*/
void TTextField::setNumericText(float f)
{
	if (hasMax && f > maximum)
		return;
	if (hasMin && f < minumum)
		return;

	text.Empty();
	text.Format(L"%f", f);

	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
		int res = text1->onCreate(location);
	}
	if (text1.Get())
		text1->setCaption(text);
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
	if (!text1.Get() || !drawingBoard.Get())
		return;

	IDWriteTextLayout* layout = text1->fontLayout.Get();
	if (!layout)
		return;

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
	for (int c = 0; c < TextList.Size(); c++)
	{
		feild = TextList[c];
		if (feild)
			feild->onFocus = false;
	}
}

/*
* Method: TTextField::updateTextString
* Purpose: Refreshes the Text string formating
* Parameters: void
* Returns: void
*/
void TTextField::updateTextString()
{
	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);

	}
	text1->setCaption(text);
	text1->reCreateLayout();
	IDWriteTextLayout* layout = text1->fontLayout.Get();
	if (!layout)
		return;
	for (int c = 0; c < details.Size(); c++)
	{
		layout->SetFontStyle(details[c].style, details[c].range);
		layout->SetFontWeight(details[c].weight, details[c].range);
	}

	highlighter.SetLayout(text1->fontLayout);
}

/*
* Method: TTextField::moveCaretLeft
* Purpose: Shifts the caret to the left in the text
* Parameters: CPoint point - the point to test
* Returns:void
*/
void TTextField::moveCaretLeft(POINT point)
{
	BOOL isInside = true;
	BOOL isTrailing = FALSE;
	DWRITE_HIT_TEST_METRICS metric;
	text1->fontLayout->HitTestPoint(point.x - 1, point.y + 1, &isTrailing, &isInside, &metric);
	if (isInside)
	{
		HideCaret(windowHandle);
		SetCaretPos(metric.left, point.y);
		ShowCaret(windowHandle);
	}

}

/*
* Method: TTextField::moveCaretRight
* Purpose: Shifts the Caret to the right in the text
* Parameters: CPoint point - the point to test
* Returns: void
*/
void TTextField::moveCaretRight(POINT point)
{
	BOOL isInside = true;
	BOOL isTrailing = FALSE;
	DWRITE_HIT_TEST_METRICS metric;
	text1->fontLayout->HitTestPoint(point.x + 1, point.y + 1, &isTrailing, &isInside, &metric);
	if (isInside)
	{
		HideCaret(windowHandle);
		SetCaretPos(metric.left + metric.width, point.y);
		ShowCaret(windowHandle);
	}
}

/*
* Method: TTextField::moveCaretUp
* Purpose: Shifts the Caret to the up in the text
* Parameters: CPoint point - the point to test
* Returns: void
*/
void TTextField::moveCaretUp(POINT point)
{
	BOOL isInside = true;
	BOOL isTrailing = FALSE;
	DWRITE_HIT_TEST_METRICS metric;

	text1->fontLayout->HitTestPoint(point.x, point.y, &isTrailing, &isInside, &metric);
	float origTop = metric.top;

	for (int c = point.y - 1; c > point.y - 200 && c > 0;c--)
	{
		text1->fontLayout->HitTestPoint(point.x, c, &isTrailing, &isInside, &metric);
		if (isInside && metric.top < origTop)
		{
			HideCaret(windowHandle);
			if (isTrailing)
				point.x = metric.left;
			else
				point.x = metric.left + metric.width;

			SetCaretPos(point.x, metric.top);
			ShowCaret(windowHandle);
			break;
		}
	}
}

/*
* Method: TTextField::moveCaretDown
* Purpose: Shifts the Caret to the down in the text
* Parameters: CPoint point - the point to test
* Returns: void
*/
void TTextField::moveCaretDown(POINT point)
{
	BOOL isInside = true;
	BOOL isTrailing = FALSE;
	DWRITE_HIT_TEST_METRICS metric;

	text1->fontLayout->HitTestPoint(point.x, point.y, &isTrailing, &isInside, &metric);
	float origTop = metric.top;

	for (int c = point.y + metric.height; c < point.y + 200 ;c++)
	{
		text1->fontLayout->HitTestPoint(point.x, c, &isTrailing, &isInside, &metric);
		if (isInside && metric.top > origTop)
		{
			HideCaret(windowHandle);
			if (isTrailing)
				point.x = metric.left;
			else
				point.x = metric.left + metric.width;

			SetCaretPos(point.x, metric.top);
			ShowCaret(windowHandle);
			break;
		}
	}
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

/*
 * Method: TextHighlighter::TextHighlighter
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> - the drawing board to draw to
 * Returns: New Text Highlighter object
 */
TextHighlighter::TextHighlighter(TrecPointer<DrawingBoard> rt)
{
	if (!rt.Get())
		throw L"Error! Need Render Target To Be active!";

	renderer = rt;
	isActive = false;

	
	brush = renderer->GetBrush(TColor(D2D1::ColorF::Aqua));
}

/*
 * Method: TextHighlighter::SetLayout
 * Purpose: Sets the layout of the text to apply the effect to
 * Parameters: TrecComPointer<IDWriteTextLayout> l - the layout to apply effects to
 * Returns: void
 */
void TextHighlighter::SetLayout(TrecComPointer<IDWriteTextLayout> l)
{
	layout = l;
	assert(layout.Get());
}

/*
 * Method: TextHighlighter::SetFirstPosition
 * Purpose: ID's the index of the first point and sets the highlighter to active
 * Parameters: UINT f - the location of the first point in the text
 * Returns: void
 */
void TextHighlighter::SetFirstPosition(UINT f)
{
	beginningPosition = f;
	beginningIsInitial = true;
	isActive = true;
	ATLTRACE(L"HIGHLIGHTER PREPARED\n");
}

/*
 * Method: TextHighlighter::SetSecondPosition
 * Purpose: Identifies the second position the user has specified and applies the highlighing effect to the region betweent he two positions
 * Parameters: UINT s - the location of the second point in the text
 * Returns: void
 */
void TextHighlighter::SetSecondPosition(UINT s)
{
	if (isActive && layout.Get())
	{
		layout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ beginningPosition, endingPosition - beginningPosition });
		isActive = false;
	}

	if (beginningIsInitial)
	{
		if (beginningPosition > s)
		{
			beginningIsInitial = false;
			endingPosition = beginningPosition;
			beginningPosition = s;
		}
		else
			endingPosition = s;
		
	}
	else
	{
		if (endingPosition < s)
		{
			beginningIsInitial = true;
			beginningPosition = endingPosition;
			endingPosition = s;
		}
		else
			beginningPosition = s;
	}
	ATLTRACE(L"HIGHLIGHTER MODIFIED\n");
	layout->SetDrawingEffect(brush->GetUnderlyingBrush().Get(), DWRITE_TEXT_RANGE{ beginningPosition, endingPosition - beginningPosition });
	isActive = true;
}


/*
 * Method: TextHighlighter::Reset
 * Purpose: Resets the highlighter
 * Parameters: UINT location - the location where the mouse is unclicked
 * Returns: bool - whether the highlighter was reset
 */
bool TextHighlighter::Reset(UINT cLocation)
{

	if (cLocation > beginningPosition&& cLocation < endingPosition)
		return false;
	if (layout.Get())
	{
		layout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ beginningPosition, endingPosition - beginningPosition });
		
	}
	isActive = false;
	ATLTRACE(L"HIGHLIGHTER RESET\n");
	return true;
}

/*
 * Method: TextHighlighter::ResetUp
 * Purpose: Resets the highlighter if necessary
 * Parameters: UINT location - the location where the mouse is unclicked
 * Returns: void
 */
void TextHighlighter::ResetUp(UINT cLocation)
{
	if (cLocation == ((beginningIsInitial) ? beginningPosition : endingPosition))
	{
		if (layout.Get())
		{
			layout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ beginningPosition, endingPosition - beginningPosition });
		}
		
	}
	isActive = false;
}

/*
 * Method: TextHighlighter::IsActive
 * Purpose: Reports whether the highlighter is looking for the second position
 * Parameters: void
 * Returns: bool - whether the highlighter is actively looking for the second position
 */
bool TextHighlighter::IsActive()
{
	return isActive;
}
