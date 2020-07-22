#include "TPromptControl.h"

extern TDataArray<TTextField*> TextList;

/*
 * Method: TPromptControl::TPromptControl
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> rt - The Render Target to draw to
 *				TrecPointer<TArray<styleTable>> ta - The Style Table to draw from
 *				HWND winHand - the handle to the window so Windows Caret Support is possible
 * Returns: New Prompt Control
 */
TPromptControl::TPromptControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, HWND w):TTextField(rt,ta,w)
{
	processRunning = false;
}

/*
 * Method: TPromptControl::~TPromptControl
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TPromptControl::~TPromptControl()
{
}

/*
* Method: TPromptControl::onCreate
* Purpose: Sets up the TPromptControl with Prompt Specific attributes
* Parameters: RECT r - the location that the control would work in
* Returns: bool - success (currently arbitrarily)
*/
bool TPromptControl::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TTextField::onCreate(r, d3d);

	isPassword = false;
	isNumber = false;

	return false;
}

/*
* Method: TPromptControl::onDraw
* Purpose: Draws the text that it was given
* Parameters: TObject* obj - object used for databinding (unlikely to be used here)
* Returns: void
*/
void TPromptControl::onDraw(TObject* obj)
{
	TString shellOutput(shell.GetOutput());

	if (shellOutput.GetSize())
	{
		TTextField::SetText(text + shellOutput);

		// Check to see if we are getting a password 
		TString lowerOutput(shellOutput.GetLower());
		if (lowerOutput.FindLast(L'\n') != -1)
			lowerOutput.Set(lowerOutput.SubString(lowerOutput.FindLast(L'\n')));
		if (lowerOutput.Find(L"password") != -1 || 
			lowerOutput.Find(L"passphrase") != -1 ||
			lowerOutput.Find(L"passcode") != -1)
			isPassword = true;

		caretLoc += shellOutput.GetSize();
	}

	bool processCheck = shell.CheckProcess();

	if (processRunning && !processCheck)
	{
		processRunning = false;
		TString add;

		TString output(shell.GetOutput());
		if (output.GetSize())
			output.Set(TString(L"\n") + output + L"\n");

		if (isEditable)
			add.Set(output + shell.GetWorkingDirectory() + L"\n-->" + input);
		else
			add.Set(output);

		TTextField::SetText(text + add);
		caretLoc += add.GetSize();
	}
	else if (isEditable && !shell.CheckProcess() && !text.GetSize())
	{
		TString add(shell.GetOutput() + L"\n" + shell.GetWorkingDirectory() + L"\n-->" + input);
		TTextField::SetText(text + add);
		caretLoc += add.GetSize();
	}

	TControl::onDraw(obj);
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
void TPromptControl::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	resetArgs();

	BOOL trailing = false, isInside = false;
	DWRITE_HIT_TEST_METRICS metrics;
	if (!isEditable)
		goto parentCall;

	text1->fontLayout->HitTestPoint(point.x - location.left, point.y - location.top, &trailing, &isInside, &metrics);

	if (isInside && isContained(&point, &location))
	{
		UINT tempCurLoc = (trailing) ? metrics.textPosition + 1 : metrics.textPosition;

		if (isInInput(tempCurLoc))
		{
			if (onFocus)
				DestroyCaret();

			CreateCaret(windowHandle, NULL, 1, metrics.height);

			caretLoc = tempCurLoc;

			if (trailing)
			{
				SetCaretPos(metrics.left + metrics.width + location.left, metrics.top + location.top);
			}
			else
			{
				SetCaretPos(metrics.left + location.left, metrics.top + location.top);
			}

			ShowCaret(windowHandle);
			
		}
		TTextField::RemoveFocus();
		onFocus = true;
		clickedControl.push_back(this);
		if (highlighter.Reset(caretLoc))
			highlighter.SetFirstPosition(caretLoc);
	}
	else
	{
		if (onFocus)
		{
			DestroyCaret();
			onFocus = false;
		}
		else if (isContained(&point, &location) && !text.GetSize() && text1.Get() && isEditable)
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
	if (isContained(&point, &location))
	{

		TTextField::RemoveFocus();
		onFocus = true;
		if (offerPasswordPeek && isContained(point, passwordPeek_outer))
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
	TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);

	if (onFocus)
	{
		args.text.Set(text1->getCaption());
	}
}

/*
* Method: TPromptControl::OnChar
* Purpose: Adds a character to the String
* Parameters: bool fromChar - can be called either from on Key Down or OnChar
*				UINT nChar - The ID of the character that was pressed
*				UINT nRepCnt - how many times the character was processed for this event
*				UINT nFlags - flags provided by MFC's Message system, not used
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
bool TPromptControl::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
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
			for (int c = 0; c < nRepCnt; c++)
			{
				switch (nChar)
				{
				case VK_LEFT:
					if (caretLoc > 0 && isInInput(caretLoc - 1))
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
				case VK_RETURN:
					SubmitCommand();
					text.AppendChar(L'\n');
					caretLoc++;
					break;
				/*case VK_UP:
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
					break;*/
				case VK_UP:
				case VK_DOWN: // To-Do: Add History feature for these commands
				case VK_ESCAPE:
				case VK_HOME:
				
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
		eventAr.push_back(EventID_Cred( R_Message_Type::On_Text_Change, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));

		if (text1.Get() && text1->text.GetSize())
		{
			args.text.Set(text1->text);
			text1->reCreateLayout();
			highlighter.SetLayout(text1->fontLayout);
		}
	}
	return onFocus;
}


void TPromptControl::SetText(TString)
{
}

/*
 * Method: TPromptControl::SubmitCommand
 * Purpose: Allows external code (such as a handler or an environment) to manually enter a command
 * Parameters: TString& command -  the command to enter
 * Returns: void
 */
void TPromptControl::SubmitCommand(TString& command)
{
	isPassword = false;
	shell.SubmitCommand(command);
	processRunning = true;
}

/*
 * Method: TPromptControl::isInInput
 * Purpose: Checks whether a string index is within the input region
 * Parameters: UINT proposeLoc - the index of the string to check
 * Returns: bool - whether the index is within the input region or not
 */
bool TPromptControl::isInInput(UINT proposeLoc)
{
	auto textSize = text.GetSize();
	return proposeLoc < textSize && proposeLoc >= (textSize - input.GetSize());
}

/*
 * Method: TPromptControl::SubmitCommand
 * Purpose: Submits the command entered by the user for processing
 * Parameters: void
 * Returns: void
 */
void TPromptControl::SubmitCommand()
{
	isPassword = false;
	shell.SubmitCommand(input);
	processRunning = true;
	input.Empty();
}

/*
 * Method: TPromptControl::InputChar
 * Purpose: Adds a character to the input string
 * Parameters: WCHAR ch - the character to add
 *				int - the number of times to add that character
 * Returns: void
 */
void TPromptControl::InputChar(wchar_t cha, int times)
{
	if (!onFocus) // hopefully this is true, but just in case
		return;

	int caretDif = text.GetSize() - input.GetSize();

	for (int c = 0; c < times; c++)
	{
		switch (cha)
		{
		case VK_BACK:
			if (caretLoc > caretDif && text.GetSize() > 0)
			{

				text.Delete(caretLoc - 1, 1);
				input.Delete(caretLoc - 1 - caretDif, 1);
				caretLoc--;

			}

			break;
		case VK_OEM_PERIOD:
			if (isPassword)
				text.Insert(caretLoc, L'*');
			else
				text.Insert(caretLoc, L'.');
			input.Insert(caretLoc++ - caretDif, L'.');
			break;
		case VK_SUBTRACT:
			if (cha == L'm')
				goto def;
			if (isPassword)
				text.Insert(caretLoc, L'*');
			else
				text.Insert(caretLoc, L'-');
			input.Insert(caretLoc++ - caretDif, L'-');
			break;
		case VK_DIVIDE:
			if (cha == L'o')
				goto def;
			if (isPassword)
				text.Insert(caretLoc, L'*');
			else
				text.Insert(caretLoc, L'/');
			input.Insert(caretLoc++ - caretDif, L'/');
			break;
		case VK_DELETE:
			if (text.GetSize() > 0)
			{

				text.Delete(caretLoc, 1);
				input.Delete(caretLoc - caretDif, 1);
			}
			break;
		case L'\r':
			break;
		default:
		def:
			if (isPassword)
				text.Insert(caretLoc, L'*');
			else
				text.Insert(caretLoc, cha);
			input.Insert(caretLoc++ - caretDif, cha);
		}
	}
}


