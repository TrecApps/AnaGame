
#include "TRadioButton.h"


TDataArray<TRadioButton*> otherButtons;
bool otherRun = false;

/*
* Function: runLoop
* Purpose: Updates each Radio button as one is clicked as necessary
* Parameters: void
* Returns: void
*/
void runLoop()
{
	if (!otherRun)
	{
		TRadioButton* runner = NULL;
		for (int c = 0; c < otherButtons.Size();c++)
		{
			runner = otherButtons[c];
			if (runner)
				runner->onCreateClass();
		}
	}
	otherRun = true;
}

/*
* Method: TRadioButton::TRadioButton
* Purpose: Cunstructor
* Parameters: TrecPointer<DrawingBoard> rt - render target to use
*				TrecPointer<TArray<styleTable>> ta - the list of Anaface styles
* Returns: void
*/
TRadioButton::TRadioButton(TrecPointer<DrawingBoard>rt, TrecPointer<TArray<styleTable>> ta):TGadgetControl(rt,ta)
{
	otherButtonLocation = otherButtons.push_back(this);
	buttonClass = NULL;
	isClicked = false;
	ellBut.point = { 0,0 };
	ellBut.radiusX = ellBut.radiusY = 0;
	onClick = false;
}

/*
* Method: TRadioButton::~TRadioButton
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TRadioButton::~TRadioButton()
{
	//otherButtons.RemoveAt(otherButtonLocation);
	//for (UINT c = otherButtonLocation; c < otherButtons.Size(); c++)
	//{
	//	otherButtons[c]->otherButtonLocation--;
	//}
}

/*
* Method: TRadioButton::onCreate
* Purpose: Sets up the control, including radio button specific aspects
* Parameters: RECT r - the loaction of the button
* Returns: bool - false (ignore)
*/
bool TRadioButton::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TGadgetControl::onCreate(r,d3d);

	runLoop();

	if (buttonClass)
	{
		TRadioButton* radioCheck = NULL;
		for (int c = 0; c < otherButtons.Size();c++)
		{
			radioCheck = otherButtons[c];
			if (radioCheck && radioCheck != this)
			{
				if (radioCheck->buttonClass && buttonClass->Compare(*(radioCheck->buttonClass)) == 0)
				{
					radioCheck->addButton(this);
				}
			}
		}
	}

	if (text1.Get())
	{
		text1->bounds.left = text1->bounds.left + bSize;
	}
	else
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
		text1->text = L"Radio-Button";


	}


	ellBut.point = D2D1::Point2F((DxLocation.right + DxLocation.left) / 2, (DxLocation.top + DxLocation.bottom) / 2);
	ellBut.radiusX = (DxLocation.right - DxLocation.left) / 2;
	ellBut.radiusY = (DxLocation.bottom - DxLocation.top) / 2;
	return false;
}

/*
* Method: TRadioButton::onDraw
* Purpose: Draws Radio Button specific visuals
* Parameters: void
* Returns: void
*/
void TRadioButton::onDraw(TObject* obj)
{
	TControl::onDraw(obj);

	if (!isActive)
		return;

	if (isClicked)
		brush->FillEllipse(ellBut);
	else
		brush->DrawEllipse(ellBut);
}

/*
* Method: TRadioButton::OnLButtonDown
* Purpose: Allows Radio buttons thair own control of the the click event
* Parameters: UINT nFlags - flags involved
*				CPoint point - point on screen affected
*				messageOutput * mOut - results of message checking
*				TDataArray<EventID_Cred>& eventAr - list of events to respond to
* Returns: void
*/
void TRadioButton::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	resetArgs();
	TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);

	if (isContained(&point, &location))
	{
		isClicked = !isClicked;
		*mOut = messageOutput::positiveOverrideUpdate;
		for (int c = 0; c < otherSameButtons.Size(); c++)
		{
			otherSameButtons[c]->isClicked = false;
		}
		eventAr.push_back({ R_Message_Type::On_radio_change, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis) });

		if (hasEvent(R_Message_Type::On_radio_change))
		{
		
			args.eventType = R_Message_Type::On_radio_change;
			args.positive = isClicked;
			if (text1.Get())
				args.text.Set(text1->text);
			args.methodID = getEventID(R_Message_Type::On_radio_change);
		}
	}

}

/*
* Method: TRadioButton::OnLButtonUp
* Purpose: Calls the TControls version (Don't know why this version is here)
* Parameters: UINT nFlags - flags involved
*				CPoint point - point on screen affected
*				messageOutput * mOut - results of message checking
*				TDataArray<EventID_Cred>& eventAr - list of events to respond to
* Returns: void
*/
void TRadioButton::OnLButtonUp(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr)
{
	TControl::OnLButtonUp(nFlags, point, mOut,eventAr);
}

/*
* Method: TRadioButton::GetAnaGameType
* Purpose: Returns the AnaGame type marker for Radio Buttons
* Parameters: void
* Returns: UCHAR* - pointer to AnaGame marker for radio button class
*/
UCHAR * TRadioButton::GetAnaGameType()
{
	return nullptr;
}

/*
 * Method: TRadioButton::Resize
 * Purpose: Resizes the control upon the window being resized, applies to the box inside the control
 * Parameters: D2D1_RECT_F& r - the new location for the control
 * Returns: void
 */
void TRadioButton::Resize(D2D1_RECT_F& r)
{
	TGadgetControl::Resize(r);
	ellBut.point = D2D1::Point2F((DxLocation.right + DxLocation.left) / 2, (DxLocation.top + DxLocation.bottom) / 2);
	ellBut.radiusX = (DxLocation.right - DxLocation.left) / 2;
	ellBut.radiusY = (DxLocation.bottom - DxLocation.top) / 2;
}

/*
* Method: TRadioButton::addButton
* Purpose: Adds a new radio button to the group
* Parameters: TRadioButton* trb - button to add
* Returns: void
*/
void TRadioButton::addButton(TRadioButton* trb)
{
	if (!trb)
		return;
	for (int c = 0; c < otherSameButtons.Size(); c++)
	{
		if (otherSameButtons[c] == trb)
			return;
	}
	otherSameButtons.push_back(trb);
	trb->addButton(this);
}

/*
* Method: TRadioButton::onCreateClass
* Purpose: Creaes a new class of radio button, used to group buttons
* Parameters: void 
* Returns: void
*/
void TRadioButton::onCreateClass()
{
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|RadioClass"));
	if (valpoint.Get())
	{
		buttonClass = new TString(valpoint.Get());
	}
}

/*
* Method: TRadioButton::storInTML
* Purpose: Stores the radio button in a TML file
* Parameters: CArchive * ar - File to use
*				int childLevel - Level of control in the UI tree
*				bool overrideChildren - UNUSED
* Returns: void
*/
void TRadioButton::storeInTML(TFile * ar, int childLevel, bool overrideChildren)
{
	//_Unreferenced_parameter_(overrideChildren);

	if (buttonClass)
	{
		TString appendable;
		resetAttributeString(&appendable, childLevel + 1);

		appendable.Append(L"|RadioClass:");
		appendable.Append(buttonClass->GetConstantBuffer());
		_WRITE_THE_STRING;
	}
	TControl::storeInTML(ar, childLevel);
}
