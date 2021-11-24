#include "TRadiobutton.h"

TRadiobutton::TRadioClass::TRadioClass()
{
}

TRadiobutton::TRadioClass::TRadioClass(const TRadioClass& copy)
{
	this->name.Set(copy.name);

	for (UINT Rust = 0; Rust < copy.buttons.Size(); Rust++)
	{
		buttons.push_back(copy.buttons[Rust]);
	}
}

void TRadiobutton::TRadioClass::AddButton(TrecPointer<TPage> radioPage)
{
	for (UINT Rust = 0; Rust < buttons.Size(); Rust++)
	{
		if (radioPage.Get() == buttons[Rust].Get())
			return;
	}

	auto subPage = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TRadiobutton>(radioPage);
	if (subPage.Get())
		buttons.push_back(TrecPointerKey::GetSoftSubPointerFromSub<>(subPage));
}

UINT TRadiobutton::TRadioClass::RemoveButton(TRadiobutton* remove)
{
	for (UINT Rust = 0; Rust < buttons.Size(); Rust++)
	{
		if (buttons[Rust].Get() == remove)
		{
			buttons.RemoveAt(Rust);
			break;
		}
	}
	return buttons.Size();
}

void TRadiobutton::TRadioClass::ClaimClick(TRadiobutton* claiment)
{
	for (UINT Rust = 0; Rust < buttons.Size(); Rust++)
	{
		if (buttons[Rust].Get() != claiment)
			buttons[Rust].Get()->UnCheck();
	}
}

TDataArray<TrecPointer<TRadiobutton::TRadioClass>> radioClasses;

TrecPointer<TRadiobutton::TRadioClass> TRadiobutton::GetRadioClass(const TString& name)
{
	bool nullFound = false;
	UINT nullIndex = 0;
	for (UINT Rust = 0; Rust < radioClasses.Size(); Rust++)
	{
		if (!radioClasses[Rust].Get() && !nullFound)
		{
			nullFound = true;
			nullIndex = Rust;
			continue;
		}
		if (!name.Compare(radioClasses[Rust]->name))
		{
			return radioClasses[Rust];
		}
	}

	TrecPointer<TRadiobutton::TRadioClass> r = TrecPointerKey::GetNewTrecPointer<TRadiobutton::TRadioClass>();
	r->name.Set(name);

	if (nullFound)
		radioClasses[nullIndex] = r;
	else
		radioClasses.push_back(r);

	return r;
}

TRadiobutton::TRadiobutton(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta): TGadget(rt, ta)
{
	isChecked = false;
	this->ellBut.radiusX = ellBut.radiusY = 0.0f;
	ellBut.point = { 0.0f, 0.0f };
}

TRadiobutton::~TRadiobutton()
{
}

bool TRadiobutton::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
	TGadget::onCreate(loc, d3d);


	if (attributes.retrieveEntry(L"RadioClass", radioClass))
	{
		auto group = GetRadioClass(radioClass);
		group->AddButton(TrecPointerKey::GetTrecPointerFromSoft<>(self));
	}

	if (text.Get())
	{
		auto tempLoc = text->GetLocation();
		tempLoc.left += bSize;
		text->SetLocation(tempLoc);
	}
	else
	{
		text = TrecPointerKey::GetNewTrecPointer<TTextElement>(drawingBoard);
		text->SetText(L"Radio-Botton");
	}

	ellBut.point = D2D1::Point2F((DxLocation.right + DxLocation.left) / 2, (DxLocation.top + DxLocation.bottom) / 2);
	ellBut.radiusX = (DxLocation.right - DxLocation.left) / 2;
	ellBut.radiusY = (DxLocation.bottom - DxLocation.top) / 2;

	return false;
}

void TRadiobutton::Draw(TrecPointer<TVariable> object)
{

	TControl::Draw(object);

	if (isActive)
	{
		if (isChecked)
			brush->FillEllipse(ellBut);
		else
			brush->DrawEllipse(ellBut);
	}
}

void TRadiobutton::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
	if (this->isLeftClicked && IsContained(point, area))
	{
		isChecked = true;

		if (radioClass.GetSize())
		{
			auto group = GetRadioClass(radioClass);
			group->ClaimClick(this);
		}

		eventAr.push_back(EventID_Cred(R_Message_Type::On_check, TrecPointerKey::GetTrecPointerFromSoft<>(self)));
		int checkIndex = HasEvent(R_Message_Type::On_check);
		if (checkIndex != -1)
		{
			this->args.Reset();
			this->args.point = point;
			this->args.eventType = R_Message_Type::On_check;
			this->args.positive = isChecked;
			if (text.Get())
				text->GetText(this->args.text);
			this->args.methodID = checkIndex;
		}
	}
}

void TRadiobutton::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	TGadget::OnResize(newLoc, nFlags, eventAr, args);

	if (text.Get())
	{
		auto tempLoc = text->GetLocation();
		tempLoc.left += bSize;
		text->SetLocation(tempLoc);
	}
	ellBut.point = D2D1::Point2F((DxLocation.right + DxLocation.left) / 2, (DxLocation.top + DxLocation.bottom) / 2);
	ellBut.radiusX = (DxLocation.right - DxLocation.left) / 2;
	ellBut.radiusY = (DxLocation.bottom - DxLocation.top) / 2;
}

void TRadiobutton::UnCheck()
{
	TObjectLocker lock(&thread);
	isChecked = false;
}
