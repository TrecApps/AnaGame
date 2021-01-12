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
	}
	content1->stopCollection.AddGradient(TGradientStop(TColor(t_color::Aqua), 0.0f));
	content1->onCreate(location);
	
	if (!content2.Get())
	{
		content2 = TrecPointerKey::GetNewTrecPointer<TContent>(drawingBoard, this);
	}
	content2->stopCollection.AddGradient(TGradientStop(TColor(t_color::Azure), 0.0f));
	content2->onCreate(location);
	
	if (!text1.Get())
	{
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);
	}
	text1->stopCollection.AddGradient(TGradientStop(TColor(), 0.0f));
	text1->onCreate(location);
	

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

	SetTabSizes();

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
	{
		onClick = true;
		for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
		{
			auto tab = tabs[Rust];
			if (!tab.Get())
				continue;
			
			TabClickMode cMode = tab->AttemptClick(point);

			if (cMode == TabClickMode::tcm_not_clicked)
				continue;

			clickMode = cMode;
			currentlyClickedTab = tab;
			break;
		}

		if (currentlyClickedTab.Get() && currentlyClickedTab->isAdd)
			clickMode = TabClickMode::tcm_new_tab;
		else if (clickMode == TabClickMode::tcm_not_clicked)
		{
			if (rightTab.AttemptClick(point) != TabClickMode::tcm_not_clicked)
				clickMode = TabClickMode::tcm_right_tab;
			else if (leftTab.AttemptClick(point) != TabClickMode::tcm_not_clicked)
				clickMode = TabClickMode::tcm_left_tab;
		}
	}
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
						currentlyClickedTab = tabs[Rust];
					}
				}
			}
		}
	}
	onClick = false;
}

TrecPointer<Tab> TTabBar::AddTab(const TString& text)
{
	TrecPointer<Tab> newTab = TrecPointerKey::GetNewTrecPointer<Tab>();

	newTab->content1 = content1;
	newTab->content2 = content2;

	newTab->text = text1.Get() ? TrecPointerKey::GetNewTrecPointer<TText>(text1, this) :
		TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, this);

	newTab->isAdd = false;
	newTab->draw1 = true;
	newTab->SetText(text);

	newTab->text->onCreate(location);

	tabs.push_back(newTab);

	if (haveAdd && tabs.Size() > 1)
	{
		auto tempTab = tabs[tabs.Size() - 1];
		tabs[tabs.Size() - 1] = tabs[tabs.Size() - 2];
		tabs[tabs.Size() - 2] = tempTab;
	}

	SetTabSizes();
	return newTab;
}

/**
 * Method: TTabBar::GetContentSize
 * Purpose: Reports the number of tabs (presumably) with content
 * Parameters: void
 * Returns: UINT - number of content tabs available
 */
UINT TTabBar::GetContentSize()
{
	if (haveAdd && tabs.Size())
		return tabs.Size() - 1;
	return tabs.Size();
}


/**
 * Method: TTabBar::GetTabAt
 * Purpose: Retrieves the Tab at the given index or null if out of bounds
 * Parameters: UINT index - the index to get
 * Returns: TrecPointer<Tab> - the requested tab
 */
TrecPointer<Tab> TTabBar::GetTabAt(UINT index)
{
	if (index < tabs.Size())
		return tabs[index];
	return TrecPointer<Tab>();
}

/**
 * Method: TTabBar::RemoveTabAt
 * Purpose: Removes a tab
 * Parameters: UINT Index - index of the tab to remove
 * Returns: void
 */
void TTabBar::RemoveTabAt(UINT index)
{
	if (index < tabs.Size())
	{
		TrecPointer<Tab> tab = tabs.RemoveAt(index);

		// To-Do: Figure out ho to move the tabs
		SetTabSizes();
	}
}

TrecPointer<Tab> TTabBar::GetCurrentTab()
{
	return this->currentlyClickedTab;
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
	location = { 0.0f,0.0f,0.0f,0.0f };
}

void Tab::SetBrush(TrecPointer<TBrush> brush)
{
	this->brush = brush;
}

void Tab::SetText(const TString& text)
{
	if (this->text.Get())
	{
		this->text->setCaption(text);
		// this->text->onCreate(location);
	}
}

D2D1_RECT_F Tab::SetLocation(const D2D1_RECT_F& newLoc)
{
	location = newLoc;
	if (text.Get())
	{
		text->SetLocation(newLoc);
		bool w;
		float width = text->GetMinWidth(w);
		location.right = location.left + width;

		text->SetLocation(location);

	}

	if (content1.Get())
		content1->onCreate(location);
	if (content2.Get())
		content2->onCreate(location);
	if (text.Get())
		text->onCreate(location);

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

TrecPointer<TabContent> Tab::GetContent()
{
	return fContent;
}

void Tab::SetContent(TrecPointer<TabContent> cont)
{
	fContent = cont;
}

TabClickMode Tab::AttemptClick(const TPoint& point)
{
	return isContained(point, location) ? TabClickMode::tcm_regular_click : TabClickMode::tcm_not_clicked;
}

/**
 * Method: Tab::GetText
 * Purpose: Retireves the Text of the Tab
 * Parameters: void
 * Returns: TString - the text of the tab
 */
TString Tab::GetText()
{
	return text.Get() ? text->getCaption() : L"";
}

TabContent::TabContent()
{
}

TabContent::~TabContent()
{
}
