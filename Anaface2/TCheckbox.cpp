#include "TCheckbox.h"

TCheckbox::TCheckbox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta): TGadget(rt, ta)
{
	isChecked = false;
}

TCheckbox::~TCheckbox()
{
}

bool TCheckbox::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d, TrecPointer<TFileShell> d)
{
	TGadget::onCreate(loc, d3d, d);

	if (!text.Get())
	{
		text = TrecPointerKey::GetNewTrecPointer<TTextElement>(drawingBoard);
		text->SetText(L"Check-Box");
	}
	auto tempLoc = text->GetLocation();
	tempLoc.left += bSize;
	text->SetLocation(tempLoc);
	return true;
}

void TCheckbox::Draw(TrecPointer<TVariable> object)
{
	TObjectLocker lock(&thread);
	if (!isActive)
	{
		return;
	}
	TControl::Draw(object);

	brush->DrawRectangle(DxLocation);
	if (this->isChecked)
	{
		D2D1_POINT_2F upLeft = D2D1::Point2F(DxLocation.left, DxLocation.top);
		D2D1_POINT_2F upRight = D2D1::Point2F(DxLocation.right, DxLocation.top);
		D2D1_POINT_2F downLeft = D2D1::Point2F(DxLocation.left, DxLocation.bottom);
		D2D1_POINT_2F downRight = D2D1::Point2F(DxLocation.right, DxLocation.bottom);
		brush->DrawLine(upLeft, downRight);
		brush->DrawLine(upRight, downLeft);
	}
}

bool TCheckbox::IsChecked()
{
	TObjectLocker lock(&thread);
	return isChecked;
}

void TCheckbox::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (this->isLeftClicked && IsContained(point, area))
	{
		isChecked = !isChecked;
		EventID_Cred cred(R_Message_Type::On_check, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		TString checkIndex = HasEvent(R_Message_Type::On_check);
		if (checkIndex.GetSize())
		{
			this->args.Reset();
			this->args.point = point;
			this->args.eventType = R_Message_Type::On_check;
			this->args.positive = isChecked;
			if (text.Get())
				text->GetText(this->args.text);
			this->args.methodID = checkIndex;

			cred.args = TrecPointerKey::GetNewTrecPointer<EventArgs>(this->args);
		}
		eventAr.push_back(cred);
	}
}



void TCheckbox::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
	TGadget::OnResize(newLoc, nFlags, eventAr);

	if (text.Get())
	{
		auto tempLoc = text->GetLocation();
		tempLoc.left += bSize;
		text->SetLocation(tempLoc);
	}
}
