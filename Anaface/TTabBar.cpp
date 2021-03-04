#include "TTabBar.h"
#include <DirectoryInterface.h>

TTabBar::TTabBar(TrecPointer<DrawingBoard> db, TrecPointer<TArray<styleTable>> styles): TControl(db, styles)
{
	startTab = 0;
	selectionSize = 30;
	haveAdd = false;
	textBuffer = 10;
}

TTabBar::~TTabBar()
{
}

bool TTabBar::onCreate(D2D1_RECT_F loc, TrecPointer<TWindowEngine> d3d)
{
	ThreadLock();
	bool ret = TControl::onCreate(loc, d3d);
	auto valpoint = attributes.retrieveEntry(TString(L"|EnableExit"));

	if (valpoint.Get() && !valpoint->CompareNoCase(L"true") && drawingBoard.Get())
	{
		TString direct = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

		exitReg = drawingBoard->GetBrush(TFileShell::GetFileInfo(direct + L"Resources\\DefaultImages\\RegularX.png"), loc);
		exitHover = drawingBoard->GetBrush(TFileShell::GetFileInfo(direct + L"Resources\\DefaultImages\\HoverX.png"), loc);
	}

	valpoint = attributes.retrieveEntry(L"|TextBuffer");
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(textBuffer);
		if (textBuffer < 0)
			textBuffer = 0;
		if (textBuffer > 60)
			textBuffer = 60;
	}

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
	

	valpoint = attributes.retrieveEntry(TString(L"|HaveAddTab"));

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
	ThreadRelease();

	return ret;
}

void TTabBar::Resize(D2D1_RECT_F& l)
{
	ThreadLock();
	TControl::Resize(l);
	SetTabSizes();
	ThreadRelease();
}

void TTabBar::onDraw(TObject* obj)
{
	ThreadLock();
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
	ThreadRelease();
}

void TTabBar::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	ThreadLock();
	if (isContained(point, location))
	{
		for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
		{
			if (!tabs[Rust].Get())
				continue;

			tabs[Rust]->draw1 = !isContained(point, tabs[Rust]->location);
			tabs[Rust]->xReg = !isContained(point, tabs[Rust]->xLocation);
		}
	}
	ThreadRelease();
}
/**
 * Method: TTabBar::OnLButtonDown
 * Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
 * Returns: void
 *
 * Attributes: override; message
 */
void TTabBar::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	ThreadLock();
	if (isContained(point, location))
	{
		*mOut = messageOutput::positiveContinue;
		onClick = true;
		UINT Rust = 0;
		clickMode = TabClickMode::tcm_not_clicked;
		for (; Rust < tabs.Size(); Rust++)
		{
			auto tab = tabs[Rust];
			if (!tab.Get())
				continue;
			
			TabClickMode cMode = tab->AttemptClick(point);

			if (cMode == TabClickMode::tcm_not_clicked)
				continue;

			clickMode = cMode;
			clickDownTab = tab;
			break;
		}

		if (clickDownTab.Get() && clickDownTab->isAdd)
			clickMode = TabClickMode::tcm_new_tab;
		else if (clickMode == TabClickMode::tcm_not_clicked)
		{
			if (rightTab.AttemptClick(point) != TabClickMode::tcm_not_clicked)
				clickMode = TabClickMode::tcm_right_tab;
			else if (leftTab.AttemptClick(point) != TabClickMode::tcm_not_clicked)
				clickMode = TabClickMode::tcm_left_tab;
		}
	}
	ThreadRelease();
}

/**
 * Method: TTabBar::OnLButtonUp
 * Purpose: Allows control to catch the Left Button Up event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 *
 * Attributes: override; message
 */
void TTabBar::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	if (onClick && isContained(point, location))
	{
		if (isContained(point, leftTab.location) && startTab > 0 && clickMode == TabClickMode::tcm_left_tab)
		{
			startTab--;
			SetTabSizes();
			*mOut = messageOutput::positiveOverrideUpdate;
		}
		else if (isContained(point, rightTab.location) && clickMode == TabClickMode::tcm_right_tab)
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
					if (!tabs[Rust]->isAdd)
					{
						if (tabs[Rust].Get() == clickDownTab.Get())
						{
							TabClickMode tcm = tabs[Rust]->AttemptClick(point);
							if (tcm != clickMode)
								clickMode = TabClickMode::tcm_not_clicked;
							currentlyClickedTab = clickDownTab;
							
						}
					}
					if (tabs[Rust].Get() != clickDownTab.Get())
					{
						clickMode = TabClickMode::tcm_not_clicked;
					}
					*mOut = messageOutput::positiveOverrideUpdate;
					break;
				}
			}
		}
	}
	onClick = false;
	clickDownTab.Nullify();
	ThreadRelease();
}


/**
 * Method: TTabBar::AddTab
 * Purpose: Adds a new Tab with the given text
 * Parameters: const TString& text - The text to add to the tab
 * Returns: void
 */
TrecPointer<Tab> TTabBar::AddTab(const TString& text)
{
	ThreadLock();
	TrecPointer<Tab> newTab = TrecPointerKey::GetNewTrecPointer<Tab>();

	newTab->exitReg = exitReg;
	newTab->exitHover = exitHover;

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
	ThreadRelease();

	return newTab;
}

bool TTabBar::AddTab(TrecPointer<Tab> tab)
{
	if(!tab.Get())
		return false;
	ThreadLock();
	if (tab.Get() == removedTab.Get())
	{
		tab->MovePoint(removedTabPoint.x, removedTabPoint.y);
		removedTab.Nullify();
	}
	TPoint tabPoint = tab->GetCurrentPoint();
	if (!isContained(tabPoint, location))
	{
		ThreadRelease();
		return false;
	}
	bool inserted = false;
	for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
	{
		TrecPointer<Tab> curTab = tabs[Rust];
		if (!curTab.Get())
		{
			tabs.RemoveAt(Rust--);
			continue;
		}
		if (curTab.Get() == tab.Get())
			return true;

		if (isContained(tabPoint, curTab->location))
		{
			tabs.push_back(tabs[tabs.Size() - 1]);

			for (UINT C = tabs.Size() - 2; C > Rust; C--)
			{
				tabs[C] = tabs[C - 1];
			}
			tabs[Rust] = tab;
			inserted = true;
			break;
		}
	}

	if (!inserted)
		tabs.push_back(tab);
	tab->ResetTabMovementVars(false);

	tab->exitHover = exitHover;
	tab->exitReg = exitReg;

	SetTabSizes();
	ThreadRelease();

	return true;
}

/**
 * Method: TTabBar::GetContentSize
 * Purpose: Reports the number of tabs (presumably) with content
 * Parameters: void
 * Returns: UINT - number of content tabs available
 */
UINT TTabBar::GetContentSize()
{
	ThreadLock();
	UINT ret = (haveAdd && tabs.Size()) ? tabs.Size() - 1 : tabs.Size();
	ThreadRelease();
	return ret;
}

/**
 * Method: TTabBar::GetTabAt
 * Purpose: Retrieves the Tab at the given index or null if out of bounds
 * Parameters: UINT index - the index to get
 * Returns: TrecPointer<Tab> - the requested tab
 */
TrecPointer<Tab> TTabBar::GetTabAt(UINT index)
{
	ThreadLock();
	TrecPointer<Tab> ret;
	if (index < tabs.Size())
		ret = tabs[index];
	ThreadRelease();

	return ret;
}

/**
 * Method: TTabBar::RemoveTabAt
 * Purpose: Removes a tab
 * Parameters: UINT Index - index of the tab to remove
 * Returns: void
 */
void TTabBar::RemoveTabAt(UINT index)
{
	ThreadLock();
	if (index < tabs.Size())
	{
		removedTab = tabs.RemoveAt(index);

		if (removedTab.Get())
		{
			float width = removedTab->location.right - removedTab->location.left;
			float height = removedTab->location.bottom - removedTab->location.top;

			removedTabPoint.x = removedTab->location.left + (width / 2);
			removedTabPoint.y = removedTab->location.top + (height / 2);

			if (!isContained(removedTabPoint, location))
			{
				width = location.right - location.left;
				height = location.bottom - location.top;
				removedTabPoint.x = location.left + (width / 2);
				removedTabPoint.y = location.top + (height / 2);
			}
		}

		// To-Do: Figure out ho to move the tabs
		SetTabSizes();
	}
	ThreadRelease();
}

void TTabBar::RemoveTab(TrecPointer<Tab> tab)
{
	ThreadLock();
	for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
	{
		if (tabs[Rust].Get() == tab.Get())
		{
			tabs.RemoveAt(Rust);
			if (Rust < tabs.Size())
				currentlyClickedTab = tabs[Rust];
			else if (tabs.Size())
				currentlyClickedTab = tabs[Rust - 1];
			else currentlyClickedTab.Nullify();
			break;
		}
	}
	ThreadRelease();
}

TrecPointer<Tab> TTabBar::GetCurrentTab()
{
	ThreadLock();
	auto ret =  this->currentlyClickedTab;
	ThreadRelease();
	return ret;
}

/**
 * Method: TTabBar::GetCurrentDownTab
 * Purpose: Retireves the Tab currently clicked on
 * Parameters: void
 * Returns: TrecPointer<Tab> - the current tab selected
 */
TrecPointer<Tab> TTabBar::GetCurrentDownTab()
{
	ThreadLock();
	auto ret = this->clickDownTab;
	ThreadRelease();
	return ret;
}

bool TTabBar::SetCurrentTab(TrecPointer<Tab> tab)
{
	ThreadLock();
	if (tab.Get())
	{
		for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
		{
			if (tab.Get() == tabs[Rust].Get())
			{
				currentlyClickedTab = tab;
				ThreadRelease();
				return true;
			}
		}
	}
	ThreadRelease();

	return false;
}

/**
 * Method: TTabBar::GetClickMode
 * Purpose: Retrieves the Click Mode of the Tab
 * Parameters: void
 * Returns: TabClickMode - the click mode of the tab
 */
TabClickMode TTabBar::GetClickMode()
{
	ThreadLock();
	auto ret = clickMode;
	ThreadRelease();
	return ret;
}

void TTabBar::SetTabSizes()
{
	ThreadLock();

	UINT limit = location.right - selectionSize;
	bool showSelection = startTab > 0;
	UINT leftBounds = location.left;

	for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
	{
		if (!tabs[Rust].Get())
			continue;

		auto newLoc = tabs[Rust]->SetLocation(D2D1::RectF(leftBounds, location.top, location.right, location.bottom), textBuffer);
		if (newLoc.right > limit)
		{
			tabs[Rust]->SetLocation(D2D1::RectF(), textBuffer);
			showSelection = true;
			break;
		}
		leftBounds = newLoc.right;
	}

	if (showSelection)
	{
		UINT half = (limit + location.right) / 2;
		leftTab.SetLocation(D2D1::RectF(limit, location.top, half, location.bottom), textBuffer);
		rightTab.SetLocation(D2D1::RectF(half, location.top, location.right, location.bottom), textBuffer);
	}
	else
	{
		leftTab.SetLocation(D2D1::RectF(), textBuffer);
		rightTab.SetLocation(D2D1::RectF(), textBuffer);
	}
	ThreadRelease();
}

Tab::Tab()
{
	draw1 = xReg =  true;
	isAdd = movedOutside = false;
	location = xLocation = { 0.0f,0.0f,0.0f,0.0f };
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

D2D1_RECT_F Tab::SetLocation(const D2D1_RECT_F& newLoc, UINT buffer)
{
	location = newLoc;
	if (text.Get())
	{
		location.left += buffer;
		text->SetLocation(newLoc);
		bool w;
		float width = text->GetMinWidth(w);
		location.right = location.left + width;

		text->SetLocation(location);

	}
	UINT height = 0;
	if (exitHover.Get() || exitReg.Get())
	{
		height = location.bottom - location.top;
		location.right += height;

		xLocation.right = location.right - (height / 4);
		xLocation.left = xLocation.right - (height / 2);
		xLocation.bottom = location.bottom - (height / 4);
		xLocation.top = location.top + (height / 4);
	}

	if (content1.Get())
		content1->onCreate(location);
	if (content2.Get())
		content2->onCreate(location);
	location.right -= height;
	if (text.Get())
		text->onCreate(location);

	location.right += height;

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

	if (exitHover.Get() || exitReg.Get())
	{
		auto exitBursh = xReg ? (exitReg.Get() ? exitReg : exitHover) :
			(exitHover.Get() ? exitHover : exitReg);

		exitBursh->FillRectangle(xLocation);

		location.right -= (location.bottom - location.top);
	}
	if (text.Get())
		text->onDraw(location);

	if(exitHover.Get() || exitReg.Get())
		location.right += (location.bottom - location.top);
}

void Tab::MovePoint(float x, float y)
{
	if (!initial.x && !initial.y)
	{
		initial.x = x;
		initial.y = y;
	}

	auto tempCurrent = current;

	current.x = x;
	current.y = y;

	if (movedOutside = MovedOutside())
	{
		if (!tempCurrent.x && !tempCurrent.y)
			tempCurrent = initial;
		float deltaX = x - tempCurrent.x;
		float deltaY = y - tempCurrent.y;

		location.bottom += deltaY;
		location.top += deltaY;
		location.right += deltaX;
		location.left += deltaX;
	}
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
	if (isContained(point, xLocation))
		return TabClickMode::tcm_exit;
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

bool Tab::MovedOutside()
{
	if (movedOutside)
		return true;
	float deltaX = current.x - initial.x;
	float deltaY = current.y - initial.y;

	auto tempLoc = location;
	tempLoc.top += deltaY;
	tempLoc.bottom += deltaY;
	tempLoc.left += deltaX;
	tempLoc.right += deltaX;

	return movedOutside = ((tempLoc.top > location.bottom) || (tempLoc.bottom < location.top) 
		|| (tempLoc.right < location.left) || (tempLoc.left > location.right));
}

void Tab::ResetTabMovementVars(bool resetLoc)
{
	if (movedOutside && resetLoc)
	{
		float deltaY = initial.y - current.y;
		float deltaX = initial.x - current.x;

		location.bottom += deltaY;
		location.top += deltaY;
		location.right += deltaX;
		location.left += deltaX;
	}

	movedOutside = false;
	initial = current = TPoint();
}

TPoint Tab::GetCurrentPoint()
{
	return current;
}

TabContent::TabContent()
{
}

TabContent::~TabContent()
{
}
