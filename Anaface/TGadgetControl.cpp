
#include "TGadgetControl.h"

/*
* Method: TGadgetControl::TGadgetControl 
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> rt - the Render Target to use
*				TrecPointer<TArray<styleTable>> ta - List of Styles for the TControl
*				bool isGadgetBase - the gadget base
* Return: new Gadget Control object
*/
TGadgetControl::TGadgetControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, bool isGadgetBase):TControl(rt, ta, false)
{
	isGadBase = isGadgetBase;
	if (isGadBase)
		isTextControl = false;
	bSize = 30;
	checker = RECT{ 0,0,0,0 };
	thickness = 1.0;
}

/*
* Method: TGadgetControl::~TGadgetControl
* Purpose: Destructor
* Parameters: void
* Return: void
*/
TGadgetControl::~TGadgetControl()
{
	//TControl::~TControl();
	brush.Delete();
	
}

void TGadgetControl::storeInTML(TFile* ar, int childLevel, bool ov)
{
	TString appendable;
	resetAttributeString(&appendable, childLevel + 1);

	appendable.Append(L"|BoxSize:");
	appendable.AppendFormat(L"%d", bSize);
	_WRITE_THE_STRING;

	TControl::storeInTML(ar, childLevel, ov);
}



/*
* Method: TGadgetControl::onCreate
* Purpose: To set up the Gadget Control
* Parameters: D2D1_RECT_F r - the location control will be
* Return: bool - false, ignore for now
*/
bool TGadgetControl::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	marginPriority = false;

	TControl::onCreate(r,d3d);

	bSize = location.bottom-location.top;
	int height = bSize;

	if (bSize > location.right - location.left)
		bSize = location.right - location.left;

	if (bSize > 30)
		bSize = 30;

	if (!content1.Get())
	{
		content1 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, this);
		content1->stopCollection.AddGradient(TGradientStop(TColor(D2D1::ColorF(D2D1::ColorF::White)), 0.0f));
		content1->onCreate(location);                         // this this isn't covered by the TControl
															// as it didn't exist yet
	}


	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|BoxSize"));
	if(valpoint.Get())
	{
		int tSize = bSize;
		if (!valpoint->ConvertToInt(bSize))
		{
			bSize = tSize;
			
		}


	}
	int offset = (height - bSize) / 2;
	checker.top = location.top + offset;
	checker.bottom = location.bottom - offset;
	checker.left = location.left;
	checker.right = checker.left + bSize;

	DxLocation.bottom = checker.bottom;
	DxLocation.left = checker.left;
	DxLocation.right = checker.right;
	DxLocation.top = checker.top;

	if (drawingBoard.Get())
	{
		
		brush = drawingBoard->GetBrush(TColor(color));
	}

	return false;
}

/*
* Method: TGadgetControl::GetAnaGameType
* Purpose: Returns the AnaGame format of the classes type
* Parameters: void
* Return: UCHAR* reference to the AnaGame type
*/
UCHAR * TGadgetControl::GetAnaGameType()
{
	return nullptr;
}


/*
 * Method: TGadgetControl::Resize
 * Purpose: Resizes the control upon the window being resized, applies to the box inside the control
 * Parameters: D2D1_RECT_F& r - the new location for the control
 * Returns: void
 */
void TGadgetControl::Resize(D2D1_RECT_F& r)
{
	TControl::Resize(r);
	int height = location.bottom - location.top;
	int offset = (height - bSize) / 2;
	checker.top = location.top + offset;
	checker.bottom = location.bottom - offset;
	checker.left = location.left;
	checker.right = checker.left + bSize;

	DxLocation.bottom = checker.bottom;
	DxLocation.left = checker.left;
	DxLocation.right = checker.right;
	DxLocation.top = checker.top;
}
