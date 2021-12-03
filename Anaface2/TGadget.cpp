#include "TGadget.h"

TGadget::TGadget(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta): TControl(rt, ta)
{
	dxColor = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	thickness = 1.0f;

	bSize = 30;
	ZeroMemory(&DxLocation, sizeof(DxLocation));
}

TGadget::~TGadget()
{
}

bool TGadget::onCreate(const D2D1_RECT_F& r, TrecPointer<TWindowEngine> d3d)
{
	ThreadLock();

	TControl::onCreate(r, d3d);

	bSize = location.bottom - location.top;
	int height = bSize;

	if (bSize > location.right - location.left)
		bSize = location.right - location.left;

	if (bSize > 30)
		bSize = 30;

	TString valpoint;

	//TrecPointer<TString> valpoint = attributes.retrieveEntry();

	bool setContent = (attributes.retrieveEntry(L"AutoGenContent", valpoint) && !valpoint.CompareNoCase(L"false")) ? false : true;

	if (!this->content.Get() && setContent)
	{
		content = TControlComponent::GetControlContent(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		content->onCreate(location);                         // this this isn't covered by the TControl
															// as it didn't exist yet
	}


	if (attributes.retrieveEntry(L"BoxSize", valpoint))
	{

		valpoint.ConvertToInt(bSize);

	}
	int offset = (height - bSize) / 2;
	DxLocation.top = location.top + offset;
	DxLocation.bottom = location.bottom - offset;
	DxLocation.left = location.left;
	DxLocation.right = DxLocation.left + bSize;


	if (drawingBoard.Get())
	{

		brush = drawingBoard->GetBrush(TColor(dxColor));
	}
	ThreadRelease();


	return false;
}

void TGadget::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	TControl::OnResize(newLoc, nFlags, eventAr);
	int height = location.bottom - location.top;
	int offset = (height - bSize) / 2;
	DxLocation.top = location.top + offset;
	DxLocation.bottom = location.bottom - offset;
	DxLocation.left = location.left;
	DxLocation.right = DxLocation.left + bSize;

	ThreadRelease();
}
