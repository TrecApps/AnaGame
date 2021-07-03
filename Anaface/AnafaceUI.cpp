
#include "AnafaceUI.h"


/**
 * Method: AnafaceUI::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString AnafaceUI::GetType()
{
	return TString(L"AnafaceUI;") + TControl::GetType();
}

/*
* Method: AnaFaceUI::AnafaceUI
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> rt - the render target to use
*				TrecPointer <TArray<styleTable>> ta - the styles for Anaface
*				HWND win - the window handle to use
* Returns void
*/
AnafaceUI::AnafaceUI(TrecPointer<DrawingBoard>rt, TrecPointer <TArray<styleTable>> ta, HWND win):TControl(rt,ta), bar(rt, ta)
{
	winHandle = win;
	tabHeight = unknownTab = 0;
	tabShow = 1;
	tabHeight = 30;
}

/*
* Method: AnafaceUI::~AnafaceUI
* Purpose: Destructor 
* Parameters: void 
* Returns: void
*/
AnafaceUI::~AnafaceUI()
{

}

/**
 * Method: AnafaceUI::TookTab
 * Purpose: Allows TControls with Tab Bars to take in a tab
 * Parameters: TrecPointer<Tab> tab - the tab to take
 * Returns: bool - whether the tab was taken in
 */
bool AnafaceUI::TookTab(TrecPointer<Tab> tab)
{
	ThreadLock();
	if (bar.AddTab(tab))
		return true;

	for (UINT Rust = 0; Rust < bar.GetContentSize(); Rust++)
	{
		auto curTab = bar.GetCurrentTab();
		if (curTab.Get() && curTab->GetContent().Get())
		{
			if (curTab->GetContent()->TookTab(tab))
			{
				ThreadRelease();
				return true;
			}
		}
	}
	ThreadRelease();
	return false;
}

/*
* Method: AnafaceUI::switchView
* Purpose: Switches the view towards the specified Content
* Parameters: UINT x - the index of the control to use
* Returns: bool - whether the operation is successful
*/
bool AnafaceUI::switchView(UINT x)
{
	ThreadLock();
	if (x < bar.GetContentSize())
	{
		try
		{
			auto tab = bar.GetTabAt(x);

			currentContent = tab->GetContent();
			ThreadRelease();
			return true;
		}
		catch (...)
		{
			currentContent.Nullify();
		}
	}
	ThreadRelease();
	return false;
}



/**
 * Method: AnafaceUI::SwitchChildControl
 * Purpose: Allows a child control to insert another control between it and THIS control (usually a scroller control)
 * Parameters: TrecPointerSoft<TControl> curControl - the Control making the call (used to help parent control identify which child to replace)
 *				 TrecPointer<TControl> newControl - the Control to replace it with
 * Returns: void
 */
void AnafaceUI::SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl)
{
	ThreadLock();
	for (UINT Rust = 0; Rust < bar.GetContentSize(); Rust++)
	{
		auto tab = bar.GetTabAt(Rust);
		if (!tab.Get())
			continue;
		auto tabCont = tab->GetContent();
		if (!tabCont.Get() || (tabCont->GetContentType() != TabContentType::tct_control))
			continue;

		TrecPointer<TControl> c = dynamic_cast<TabControlContent*>(tabCont.Get())->GetControl();

		if (curControl.Get() == c.Get())
		{
			dynamic_cast<TabControlContent*>(tabCont.Get())->SetControl(newControl);
			break;
		}
	}
	ThreadRelease();
}




/*
 * Method: AnafaceUI::OnRButtonUp
 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 */
void AnafaceUI::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	if (currentContent.Get())
		currentContent->OnRButtonUp(nFlags, point, mOut, eventAr);
	ThreadRelease();
}

/*
 * Method: AnafaceUI::OnLButtonDown
 * Purpose: Allows the AnafaceUI to catch when a tab is 
 * Parameters: UINT nFlags - flags provided by MFC
 *			CPoint point - the point of the click
 *			messageOutput * mOut - the result of the click (was it in the control?)
 *			TDataArray<EventID_Cred>& eventAr - events and their handlers documented by the control
 *			TDataArray<TControl*>& clickedControl - allows the control to report that this is a contestant for clicks
 * Returns: void
 */
void AnafaceUI::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	ThreadLock();
	if (isContained(point, location))
	{
		clickedControl.push_back(this);
		bar.OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
		if (currentContent.Get())
			currentContent->OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
	}
	ThreadRelease();
}

/*
* Method: AnafaceUI::OnRButtonDown
* Purpose: Allows Control to catch the RightmessageState::mouse button down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*/
void AnafaceUI::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	ThreadLock();
	if (isContained(point, location))
	{
		if (currentContent.Get())
		{
			currentContent->OnRButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
		}
		clickedButtons.push_back(this);
	}
	ThreadRelease();
}

/*
* Method: AnafaceUI::OnMouseMove
* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*/
void AnafaceUI::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	ThreadLock();
	if (isContained(point, location))
	{
		hoverControls.push_back(this);

		bar.OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
		if (currentContent.Get())
		{
			currentContent->OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
		}
	}
	ThreadRelease();
}

/*
* Method: AnafaceUI::OnLButtonDblClk
* Purpose: Allows control to catch the DOuble Click event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
void AnafaceUI::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	if (isContained(point, location))
	{
		bar.OnLButtonDblClk(nFlags, point, mOut, eventAr);
		if (currentContent.Get())
			currentContent->OnLButtonDblClk(nFlags, point, mOut, eventAr);
	}
	ThreadRelease();
}

/*
* Method: AnafaceUI::OnLButtonUp
* Purpose: Allows control to catch the Left Button Up event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
void AnafaceUI::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	if (isContained(point, location))
	{
		bar.OnLButtonUp(nFlags, point, mOut, eventAr);
		auto curTab = bar.GetCurrentTab();
		if (curTab.Get())
			currentContent = curTab->GetContent();
		if (currentContent.Get())
			currentContent->OnLButtonUp(nFlags, point, mOut, eventAr);
	}
	ThreadRelease();
}

/*
* Method: AnafaceUI::OnChar
* Purpose: Allows Controls to repond to character input
* Parameters: bool fromChar - can be called either from on Key Down or OnChar
*				UINT nChar - The ID of the character that was pressed
*				UINT nRepCnt - how many times the character was processed for this event
*				UINT nFlags - flags provided by MFC's Message system, not used
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
bool AnafaceUI::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	ThreadLock();
	bool ret = currentContent.Get() ? currentContent->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr) : false;
	ThreadRelease();
	return ret;
}


/*
* Method: AnafaceUI::onCreate
* Purpose: Sets up the switching control with AnafaceUI specific attributes
* Parameters: RECT container - the location the control will be stored
* Returns: bool
*/
bool AnafaceUI::onCreate(D2D1_RECT_F container, TrecPointer<TWindowEngine> d3d)
{
	ThreadLock();
	windowEngine = d3d;
	TControl::onCreate(container,d3d);
	D2D1_RECT_F r;

	TrecPointer<TString> valpoint = attributes.retrieveEntry(L"|TabsPresent");

	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"False"))
			this->tabShow = 0;
		else if (!valpoint->Compare(L"Bottom"))
			this->tabShow = 2;
	}

	valpoint = attributes.retrieveEntry(TString(L"|TabHeight"));
	if (valpoint.Get())
	{
		int v = 0;
		if (!valpoint->ConvertToInt(v))
			this->tabHeight = v;

	}// End of valpoint if statement for "TabHeight" and code for creating tabs

	D2D1_RECT_F tabsLoc = container;

	switch (tabShow)
	{
	case 1:
		container.top += tabHeight;
		tabsLoc.bottom = container.top;
		bar.onCreate(tabsLoc, d3d);
		break;
	case 2:
		container.bottom -= tabHeight;
		tabsLoc.top = container.bottom;
		bar.onCreate(tabsLoc, d3d);
	}
	

	for (int c = 0; c < children.Count(); c++)
	{
		auto tc = children.ElementAt(c);
		if (!tc.Get())
			continue;
		TrecSubPointer<TabContent, TabControlContent> controlCont = TrecPointerKey::GetNewTrecSubPointer<TabContent, TabControlContent>();
		controlCont->SetControl(tc);
		tc->onCreate(container, d3d);

		TString tabText = tc->GetID();
		if (!tabText.GetSize())
			tabText.Format(L"Unknown %d", unknownTab++);

		TrecPointer<Tab> tab = bar.AddTab(tabText);
		TrecPointer<TabContent> cContent = TrecPointerKey::GetTrecPointerFromSub<TabContent, TabControlContent>(controlCont);
		tab->SetContent(cContent);

		if (!c)
			currentContent = cContent;
	}
	ThreadRelease();
	return false;
}


/*
 * Method: AnafaceUI::addControl
 * Purpose: Adds a new control to the list of child controls
 * Parameters: TrecPointer<TControl> control - the control to add
 *				TString tabName - the name to give the tab
 * Returns: int - the index of the control (-1 if failed)
 */
int AnafaceUI::addControl(TrecPointer<TabContent> control, TString tabName)
{
	ThreadLock();
	if (control.Get() && control->HasContent())
	{
		if (control->GetContentType() == TabContentType::tct_control)
		{
			auto newControl = dynamic_cast<TabControlContent*>(control.Get())->GetControl();
			for (UINT c = 0; c < children.Count(); c++)
			{
				if (newControl.Get() == children.ElementAt(c).Get())
				{
					currentContent = control;
					ThreadRelease();
					return c;
				}
			}
			children.Add(newControl);
			newControl->setParent(GetParentReference());
			
		}
		control->Resize(CalculateClientSpace());
		currentContent = control;
			
		auto tab = bar.AddTab(tabName);
		tab->SetContent(control);
		int ret =  bar.GetContentSize();;
		ThreadRelease();
		return ret;
	}
	ThreadRelease();
	return -1;
}




/*
* Method: AnafaceUI::onDraw
* Purpose: Draws the control and any tabs present
* Parameters: void
* Returns: void
*/
void AnafaceUI::onDraw(TObject* obj)
{
	ThreadLock();
	//TControl::onDraw();
	if (!isActive)
	{
		ThreadRelease();
		return;
	}
	if (mState == messageState::mouseLClick)
	{
		if (content3.Get())
			content3->onDraw(location);
		else if (content1.Get())
			content1->onDraw(location);
		if (border3.Get())
			border3->onDraw(location);
		else if (border1.Get())
			border1->onDraw(location);
		if (text3.Get())
			text3->onDraw(location, obj);
		else if (text1.Get())
			text1->onDraw(location, obj);
	}
	else if (mState == messageState::mouseHover)
	{
		if (content2.Get())
			content2->onDraw(location);
		else if (content1.Get())
			content1->onDraw(location);
		if (border2.Get())
			border2->onDraw(location);
		else if (border1.Get())
			border1->onDraw(location);
		if (text2.Get())
			text2->onDraw(location, obj);
		else if (text1.Get())
			text1->onDraw(location, obj);
	}
	else
	{
		if (content1.Get())
			content1->onDraw(location);
		if (border1.Get())
			border1->onDraw(location);
		if (text1.Get())
			text1->onDraw(location, obj);
	}
	/*
	if (vScroll)
		vScroll->updateDraw();
	if (hScroll)
		hScroll->updateDraw();
	*/
	if (tabShow)
		bar.onDraw(obj);
	if (currentContent.Get())
		currentContent->Draw(obj);
	ThreadRelease();
}

/*
* Method: AnafaceUI::GetChildAt
* Purpose: Retrieves the child control at the given location
* Parameters: UINT c - the index of the control to look for
* Returns: TrecPointer<TControl> - the control at the given index
*/
TrecPointer<TControl> AnafaceUI::GetChildAt(UINT c)
{
	ThreadLock();
	auto ret = (c < children.Count()) ? children.ElementAt(c) : TrecPointer<TControl>();
	ThreadRelease();
	return ret;
}

/*
* Method: AnafaceUI::GetCurrentChild
* Purpose: Retrieves the currently active body control
* Parameters: void
* Returns: TrecPointer<TControl> - the current control active
*/
TrecPointer<TabContent> AnafaceUI::GetCurrentChild()
{
	ThreadLock();
	auto ret = currentContent;
	ThreadRelease();
	return ret;
}

/*
* Method: AnafaceUI::GetAnaGameType
* Purpose: Enables system to report whether the object is in fact an AnafaceUI
* Parameters: void
* Returns: UCHAR* - the AnaGame representation of the AnafaceUI
*/
UCHAR * AnafaceUI::GetAnaGameType()
{
	return nullptr;
}


/*
* Method: AnafaceUI::GetControlArea
* Purpose: Retrieves the location of the main body control (tabs might reduce the actual size)
* Parameters: void
* Returns: RECT - the location of the main body area
*/
D2D1_RECT_F AnafaceUI::GetControlArea()
{
	D2D1_RECT_F returnable = location;
	returnable.top += tabHeight;
	return returnable;
}

/**
 * Method: AnafaceUI::Resize
 * Purpose: Resizes the control upon the window being resized
 * Parameters: D2D1_RECT_F& r - the new location for the control
 * Returns: void
 */
void AnafaceUI::Resize(D2D1_RECT_F& r)
{
	TControl::Resize(r);
	auto clientRect = CalculateClientSpace();

	for (UINT Rust = 0; Rust < bar.GetContentSize(); Rust++)
	{
		auto tab = bar.GetTabAt(Rust);
		if (tab.Get() && tab->GetContent().Get())
			tab->GetContent()->Resize(clientRect);
	}

	updateComponentLocation();
}

D2D1_RECT_F AnafaceUI::CalculateClientSpace()
{
	auto ret = location;
	switch (tabShow)
	{
	case 1:
		ret.top += tabHeight;
		break;
	case 2:
		ret.bottom -= tabHeight;
	}
	return ret;
}


/**
 * Method: TabControlContent::TabControlContent
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: new TabContent object
 */
TabControlContent::TabControlContent()
{

}

/**
 * Method: TabControlContent::~TabControlContent
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TabControlContent::~TabControlContent()
{
}

/**
 * Method: TabControlContent::Resize
 * Purpose: Calls Resize on the underlying content object
 * Parameters: const D2D1_RECT_F& loc - the new location the content is expected to occupy
 * Returns: void
 *
 * Attributes: override
 */
void TabControlContent::Resize(const D2D1_RECT_F& loc)
{
	auto tLoc = loc;
	if (control.Get())
		control->Resize(tLoc);
}

/**
 * Method: TabControlContent::GetContentType
 * Purpose: Returns the Content Type, allowing users of this object to cast it into the right sub-type and take it from there
 * Parameters: void
 * Returns: TabContentType - the type of content this is expected to yield
 *
 * Attributes: override
 */
TabContentType TabControlContent::GetContentType()
{
	return TabContentType::tct_control;
}

bool TabControlContent::TookTab(TrecPointer<Tab> tab)
{
	if(!tab.Get() || !this->control.Get())	
		return false;
	return control->TookTab(tab);
}

/*
 * Method: TabControlContent::OnRButtonUp
 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 *
 * Attributes: override; message
 */
void TabControlContent::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (control.Get())
		control->OnRButtonUp(nFlags, point, mOut, eventAr);
}

/*
* Method: TabControlContent::OnLButtonDown
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
void TabControlContent::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	if (control.Get())
		control->OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
}

/*
* Method: TabControlContent::OnRButtonDown
* Purpose: Allows Control to catch the RightmessageState::mouse button down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*
* Attributes: override; message
*/
void TabControlContent::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls)
{
	if (control.Get())
		control->OnRButtonDown(nFlags, point, mOut, eventAr, clickedControls);
}

/*
* Method: TabControlContent::OnMouseMove
* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
* Returns: void
*
* Attributes: override; message
*/
void TabControlContent::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	if (control.Get())
		control->OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
}

/**
 * Method: TabControlContent::OnMouseLeave
 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
 * Returns: bool - whether the leave occured
 *
 * Attributes: override; message
 */
bool TabControlContent::OnMouseLeave(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (control.Get())
		return control->OnMouseLeave(nFlags, point, mOut, eventAr);
	return false;
}

/**
 * Method: TabControlContent::OnLButtonDblClk
 * Purpose: Allows control to catch the DOuble Click event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 *
 * Attributes: override; message
 */
void TabControlContent::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (control.Get())
		control->OnLButtonDblClk(nFlags, point, mOut, eventAr);
}

/*
* Method: TabControlContent::OnLButtonUp
* Purpose: Allows control to catch the Left Button Up event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*
* Attributes: override; message
*/
void TabControlContent::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (control.Get())
		control->OnLButtonUp(nFlags, point, mOut, eventAr);
}

/*
* Method: TabControlContent::OnChar
* Purpose: Allows Controls to repond to character input
* Parameters: bool fromChar - can be called either from on Key Down or OnChar
*				UINT nChar - The ID of the character that was pressed
*				UINT nRepCnt - how many times the character was processed for this event
*				UINT nFlags - flags provided by MFC's Message system, not used
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*
* Attributes: override; message
*/
bool TabControlContent::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (control.Get())
		control->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);
	return false;
}

/**
 * Method: TabControlContent::SetControl
 * Purpose: Sets up the Control to hold
 * Parameters: TrecPointer<TControl> control - the control to hold
 * Returns: void
 */
void TabControlContent::SetControl(TrecPointer<TControl> control)
{
	this->control = control;
}

/**
 * Method: TabControlContent:GetControl
 * Purpose: Retrieves the underlying Control
 * Parameters: void
 * Returns: TrecPointer<TControl> - the control this object holds
 */
TrecPointer<TControl> TabControlContent::GetControl()
{
	return control;
}

bool TabControlContent::HasContent()
{
	return control.Get() != nullptr;
}

void TabControlContent::Draw(TObject* obj)
{
	if (control.Get())
		control->onDraw(obj);
}
