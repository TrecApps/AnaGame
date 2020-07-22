
#include "TCheckBox.h"

/*
* Method: TCheckBox::TCheckBox 
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> rt - the render target to draw to
*				TrecPointer<TArray<styleTable>> ta - the style list for Anaface
* Returns: void
*/
TCheckBox::TCheckBox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta):TGadgetControl(rt, ta)
{
	isClicked = onClick = false;
	height = width = 0;
}

/*
* Method: TCheckBox::~TCheckBox
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TCheckBox::~TCheckBox()
{
}

/*
* Method: TCheckBox::onCreate
* Purpose: Sets up the check-boxes attribute
* Parameters: RECT r - the location for the check-box
* Returns: bool - ignore
*/
bool TCheckBox::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TGadgetControl::onCreate(r,d3d);
	if (text1.Get())
	{
		text1->bounds.left = text1->bounds.left + bSize;
	}
	else
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
		text1->text = L"Check-Box";


	}

	return true;
}

/*
* Method: TCheckBox::onDraw
* Purpose: Draws the check-box
* Parameters: void
* Returns: void
*/
void TCheckBox::onDraw(TObject* obj)
{
	if (!isActive)
		return;
	TControl::onDraw(obj);



	brush->DrawRectangle(DxLocation);
	if (isClicked)
	{
		D2D1_POINT_2F upLeft = D2D1::Point2F(DxLocation.left, DxLocation.top);
		D2D1_POINT_2F upRight = D2D1::Point2F(DxLocation.right, DxLocation.top);
		D2D1_POINT_2F downLeft = D2D1::Point2F(DxLocation.left, DxLocation.bottom);
		D2D1_POINT_2F downRight = D2D1::Point2F(DxLocation.right, DxLocation.bottom);
		brush->DrawLine(upLeft, downRight);
		brush->DrawLine(upRight, downLeft);
	}
}

/*
* Method: TCheckBox::IsClicked
* Purpose: Reports whether the Check-box is in a clicked state
* Parameters: void
* Returns: bool - whether the check-box has been "selected" or not
*/
bool TCheckBox::IsClicked()
{
	return isClicked;
}

/*
* Method: TCheckBox::OnLButtonDown
* Purpose: Catches the OnClick event and allows the check box to add a check-box event to the Event List
* Parameters: UINT nFlags - Details provided by Windows 
*				CPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?) 
*				TDataArray<EventID_Cred>& eventAr - the List of Events in a User's actions
* Returns: void
*/
void TCheckBox::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{	
	resetArgs();
	if (isContained(&point, &location))
	{
		//onClick = true;
		*mOut = messageOutput::positiveOverrideUpdate;
		isClicked = !isClicked;
		eventAr.push_back({ R_Message_Type::On_check,TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis) });

		if (hasEvent(R_Message_Type::On_check))
		{

			args.eventType = R_Message_Type::On_check;
			args.positive = isClicked;
			if (text1.Get())
				args.text.Set(text1->text);
			args.methodID = getEventID(R_Message_Type::On_check);
		}

	}
	TControl::OnLButtonDown(nFlags, point, mOut,eventAr, clickedControl);
}

/*
* Method: TCheckBox::OnLButtonUp
* Purpose: Catches the Click Release method
* Parameters: UINT nFlags - Details provided by Windows 
*				CPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?) 
*				TDataArray<EventID_Cred>& eventAr - the List of Events in a User's actions
* Returns: void
*/
void TCheckBox::OnLButtonUp(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr)
{
	resetArgs();
	TControl::OnLButtonUp(nFlags, point, mOut,eventAr);
}

/*
* Method: TCheckBox::GetAnaGameType
* Purpose: Retrieves the Structure that lets AnaGame know that this is a TCheck-Box
* Parameters: void
* Returns: UCHAR* - The AnaGame type structure
*/
UCHAR * TCheckBox::GetAnaGameType()
{
	return nullptr;
}


