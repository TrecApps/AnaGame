#include "TTabBar.h"

TTabBar::TTabBar(TrecPointer<DrawingBoard> db, TrecPointer<TArray<styleTable>> styles): TControl(db, styles)
{
	startTab = 0;
	selectionSize = 30;
	haveAdd = false;
}

TTabBar::~TTabBar()
{
}

bool TTabBar::onCreate(D2D1_RECT_F loc, TrecPointer<TWindowEngine> d3d)
{
	bool ret = TControl::onCreate(loc, d3d);

	auto valpoint = attributes.retrieveEntry(TString(L"|HaveAddTab"));



	if (!content1.Get())
	{
		content1 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, this);
		content1->stopCollection.AddGradient(TGradientStop(TColor(t_color::Aqua), 0.0f));
		content1->onCreate(location);
	}
	if (!content2.Get())
	{
		content2 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, this);
		content2->stopCollection.AddGradient(TGradientStop(TColor(t_color::Azure), 0.0f));
		content2->onCreate(location);
	}
	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
		text1->stopCollection.AddGradient(TGradientStop(TColor(), 0.0f));
		text1->onCreate(location);
	}

	if (valpoint.Get())
	{
		valpoint->Trim();
		if (!valpoint->CompareNoCase(TString(L"true")))
		{
			haveAdd = true;

			AddTab(TString("+"));

			if (tabs.Size() && tabs[0].Get())
				tabs[0]->isAdd = true;

		}
	}

	leftTab.content1 = content1;
	leftTab.content2 = content2;

	leftTab.text = TrecPointerKey::GetNewTrecPointer<TText>(text1, this);

	leftTab.isAdd = false;
	leftTab.draw1 = true;
	leftTab.SetText(TString(L"<"));

	rightTab.content1 = content1;
	rightTab.content2 = content2;

	rightTab.text = TrecPointerKey::GetNewTrecPointer<TText>(text1, this);

	rightTab.isAdd = false;
	rightTab.draw1 = true;
	rightTab.SetText(TString(L">"));


	return ret;
}

void TTabBar::Resize(D2D1_RECT_F& l)
{
	TControl::Resize(l);
	SetTabSizes();
}

void TTabBar::onDraw(TObject* obj)
{
	for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
	{
		if (!tabs[Rust].Get())
			continue;

		if (isSnipZero(tabs[Rust]->location))
			break;

		tabs[Rust]->Draw();
	}

	if (!isSnipZero(leftTab.location))
		leftTab.Draw();

	if (!isSnipZero(rightTab.location))
		rightTab.Draw();
}

void TTabBar::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	if (!isContained(point, location))
		return;

	for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
	{
		if (!tabs[Rust].Get())
			continue;
		
		tabs[Rust]->draw1 = !isContained(point, tabs[Rust]->location);
	}
}

void TTabBar::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	if (isContained(point, location))
		onClick = true;
	TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
}

void TTabBar::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (onClick && isContained(point, location))
	{
		if (isContained(point, leftTab.location) && startTab > 0)
		{
			startTab--;
			SetTabSizes();
			*mOut = messageOutput::positiveOverrideUpdate;
		}
		else if (isContained(point, rightTab.location))
		{
			startTab++;
			SetTabSizes();
			*mOut = messageOutput::positiveOverrideUpdate;
		}
		else
		{
			for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
			{
				if (!tabs[Rust].Get())
					continue;
				if (isContained(point, tabs[Rust]->location))
				{
					if (tabs[Rust]->isAdd)
					{

					}
					else
					{

					}
				}
			}
		}
	}
	onClick = false;
}

void TTabBar::AddTab(const TString& text)
{
	TrecPointer<Tab> newTab = TrecPointerKey::GetNewTrecPointer<Tab>();

	newTab->content1 = content1;
	newTab->content2 = content2;

	newTab->text = TrecPointerKey::GetNewTrecPointer<TText>(text1, this);

	newTab->isAdd = false;
	newTab->draw1 = true;
	newTab->SetText(text);

	tabs.push_back(newTab);

	if (haveAdd && tabs.Size() > 1)
	{
		auto tempTab = tabs[tabs.Size() - 1];
		tabs[tabs.Size() - 1] = tabs[tabs.Size() - 2];
		tabs[tabs.Size() - 2] = tempTab;
	}

	SetTabSizes();
}

void TTabBar::SetTabSizes()
{
	UINT limit = location.right - selectionSize;

	bool showSelection = startTab > 0;

	UINT leftBounds = location.left;

	for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
	{
		if (!tabs[Rust].Get())
			continue;

		auto newLoc = tabs[Rust]->SetLocation(D2D1::RectF(leftBounds, location.top, location.right, location.bottom));


		if (newLoc.right > limit)
		{
			tabs[Rust]->SetLocation(D2D1::RectF());
			showSelection = true;
			break;
		}

		leftBounds = newLoc.right;
	}

	if (showSelection)
	{
		UINT half = (limit + location.right) / 2;
		leftTab.SetLocation(D2D1::RectF(limit, location.top, half, location.bottom));
		rightTab.SetLocation(D2D1::RectF(half, location.top, location.right, location.bottom));
	}
	else
	{
		leftTab.SetLocation(D2D1::RectF());
		rightTab.SetLocation(D2D1::RectF());
	}
}

Tab::Tab()
{
	draw1 = true;
	isAdd = false;
}

void Tab::SetBrush(TrecPointer<TBrush> brush)
{
	this->brush = brush;
}

void Tab::SetText(const TString& text)
{
	if (this->text.Get())
		this->text->setCaption(text);
}

D2D1_RECT_F Tab::SetLocation(const D2D1_RECT_F& newLoc)
{
	location = newLoc;
	if (text.Get())
	{
		text->setNewLocation(convertD2DRectToRECT(newLoc));
		bool w;
		float width = text->GetMinWidth(w);
		location.right = location.left + width;

		text->setNewLocation(convertD2DRectToRECT(location));

	}
	return location;
}

void Tab::Draw()
{
	if (draw1 && content1.Get())
	{
		content1->onDraw(location);
	}
	else if (content2.Get())
		content2->onDraw(location);
	else if (content1.Get())
		content1->onDraw(location);

	if (text.Get())
		text->onDraw(location);
}

void Tab::MovePoint(float x, float y)
{
}
