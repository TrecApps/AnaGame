#include "TSwitchControl.h"


/**
 * Class: TabBar::TabBarHolder
 * Purpose: Provides the Tab Bar with the means to send the contents of the tab to which-ever control is in charge of hosting it
 */
class TabBarSwitchControlHolder: public TabBar::TabBarHolder
{
    friend class TSwitchControl;
public:
    /**
     * Method: TabBarHolder::SetView
     * Purpose: Called by TabBar when a new Tab is selected
     * Parameters: TrecPointer<TPage> page - the content to display
     *              bool onNew - Relevent if first parameter is null, true if caaused by a click on the new Tab-Tab
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void SetView(TrecPointer<TPage> page, bool onNew = false) override
    {
        TrecSubPointer<TPage, TSwitchControl> actControl = TrecPointerKey::GetSubPointerFromSoft<>(switchControl);
        actControl->SetView(page);
    }

    /**
     * Method: TabBarHolder::RemoveView
     * Purpose: Called by TabBar when a tab Requests to be deleted
     * Parameters: TrecPointer<TPage> page - the content to remove
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void RemoveView(TrecPointer<TPage> page) override
    {
        TrecSubPointer<TPage, TSwitchControl> actControl = TrecPointerKey::GetSubPointerFromSoft<>(switchControl);
        actControl->RemovePage(page);
    }

protected:
    /**
     * The Link to the Switch Control 
     */
    TrecSubPointerSoft<TPage, TSwitchControl> switchControl;
};


TSwitchControl::TSwitchControl(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles)
	: TRandomLayout(drawingBoard, styles)
{
    tabBar = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, TabBar>(drawingBoard);
    tabShow = 1;
    tabHeight = 30;
}

bool TSwitchControl::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
    TrecSubPointer<TabBar::TabBarHolder, TabBarSwitchControlHolder> holder =
        TrecPointerKey::GetNewTrecSubPointer<TabBar::TabBarHolder, TabBarSwitchControlHolder>();

    childSpace = loc;

    holder->switchControl = TrecPointerKey::GetSoftSubPointerFromSoft<TPage, TSwitchControl>(self);
    tabBar->SetHolder(TrecPointerKey::GetTrecPointerFromSub<>(holder));

    TString valpoint;
    if (attributes.retrieveEntry(L"TabsPresent", valpoint))
    {
        if (!valpoint.Compare(L"false"))
            this->tabShow = 0;
        else if (!valpoint.Compare(L"bottom"))
            this->tabShow = 2;
    }

    if (attributes.retrieveEntry(L"TabHeight", valpoint))
    {
        int v = 0;
        if (!valpoint.ConvertToInt(v))
            this->tabHeight = v;

    }// End of valpoint if statement for "TabHeight" and code for creating tabs

    if (attributes.retrieveEntry(L"StackLimit", valpoint))
    {
        int v = 0;
        if (!valpoint.ConvertToInt(v))
            this->maxStack = v;

    }

    D2D1_RECT_F tabsLoc = childSpace;

    TDataArray<EventID_Cred> cred;
    TDataArray<EventArgs> args;

    switch (tabShow)
    {
    case 1:
        childSpace.top += tabHeight;
        tabsLoc.bottom = childSpace.top;
        TRandomLayout::onCreate(loc, d3d);
        tabBar->OnResize(tabsLoc, 0, cred, args);
        break;
    case 2:
        childSpace.bottom -= tabHeight;
        tabsLoc.top = childSpace.bottom;
        TRandomLayout::onCreate(loc, d3d);
        tabBar->OnResize(tabsLoc, 0, cred, args);
        break;
    default:
        TRandomLayout::onCreate(loc, d3d);
    }

    TRandomLayout::onCreate(loc, d3d);

    UINT unknownCount = 1;
    for (UINT Rust = 0; childControls.Size(); Rust++)
    {
        auto control = childControls[Rust];
        if (!control.control.Get())
        {
            childControls.RemoveAt(Rust--);
            continue;
        }

        TString tabName;
        tabName.Format(L"Unknown %d", unknownCount);
        bool updated = false;
        if (dynamic_cast<TControl*>(control.control.Get()))
        {
            TString s(dynamic_cast<TControl*>(control.control.Get())->GetID());
            if (s.GetSize())
            {
                tabName.Set(s);
                updated = true;
            }
        }
        if (!updated)
            unknownCount++;

        tabBar->AddNewTab(tabName, control.control, false);
    }

    UINT start = 0;

    if (attributes.retrieveEntry(L"InitialTab", valpoint))
    {
        TString::ConvertStringToUint(valpoint, start);
    }

    auto s = tabBar->GetTabAt(start, true);

    if (s.Get())
        pageStack.push_back(s);

	return false;
}

void TSwitchControl::Draw(TrecPointer<TVariable> object)
{
    TControl::Draw(object);
    UINT pss = pageStack.Size();
    if (pss)
    {
        auto top = pageStack[pss - 1];
        if (top.Get())
            top->Draw(object);
        else
            pageStack.RemoveAt(pss - 1);
    }

    if (tabBar.Get() && tabShow)
    {
        tabBar->Draw(object);
    }
}

void TSwitchControl::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if (tabBar.Get() && tabShow)
    {
        tabBar->OnRButtonUp(nFlags, point, mOut, cred, args);
    }

    if (mOut == message_output::mo_negative)
    {
        UINT pss = pageStack.Size();
        if (pss)
        {
            auto top = pageStack[pss - 1];
            if (top.Get())
                top->OnRButtonUp(nFlags, point, mOut, cred, args);
            else
                pageStack.RemoveAt(pss - 1);
        }
    }
}

void TSwitchControl::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if (tabBar.Get() && tabShow)
    {
        tabBar->OnRButtonDown(nFlags, point, mOut, cred, args);
    }

    if (mOut == message_output::mo_negative)
    {
        UINT pss = pageStack.Size();
        if (pss)
        {
            auto top = pageStack[pss - 1];
            if (top.Get())
                top->OnRButtonDown(nFlags, point, mOut, cred, args);
            else
                pageStack.RemoveAt(pss - 1);
        }
    }
}

void TSwitchControl::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if (tabBar.Get() && tabShow)
    {
        tabBar->OnLButtonUp(nFlags, point, mOut, cred, args);
    }

    if (mOut == message_output::mo_negative)
    {
        UINT pss = pageStack.Size();
        if (pss)
        {
            auto top = pageStack[pss - 1];
            if (top.Get())
                top->OnLButtonUp(nFlags, point, mOut, cred, args);
            else
                pageStack.RemoveAt(pss - 1);
        }
    }
}

void TSwitchControl::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if (tabBar.Get() && tabShow)
    {
        tabBar->OnMouseMove(nFlags, point, mOut, cred, args);
    }

    if (mOut == message_output::mo_negative)
    {
        UINT pss = pageStack.Size();
        if (pss)
        {
            auto top = pageStack[pss - 1];
            if (top.Get())
                top->OnMouseMove(nFlags, point, mOut, cred, args);
            else
                pageStack.RemoveAt(pss - 1);
        }
    }
}

void TSwitchControl::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>& args)
{

    if (tabBar.Get() && tabShow)
    {
        tabBar->OnLButtonDblClk(nFlags, point, mOut, args);
    }

    if (mOut == message_output::mo_negative)
    {
        UINT pss = pageStack.Size();
        if (pss)
        {
            auto top = pageStack[pss - 1];
            if (top.Get())
                top->OnLButtonDblClk(nFlags, point, mOut, args);
            else
                pageStack.RemoveAt(pss - 1);
        }
    }
}

void TSwitchControl::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if (tabBar.Get() && tabShow)
    {
        tabBar->OnLButtonDown(nFlags, point, mOut, cred, args);
    }

    if (mOut == message_output::mo_negative)
    {
        UINT pss = pageStack.Size();
        if (pss)
        {
            auto top = pageStack[pss - 1];
            if (top.Get())
                top->OnLButtonDown(nFlags, point, mOut, cred, args);
            else
                pageStack.RemoveAt(pss - 1);
        }
    }
}

void TSwitchControl::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    D2D1_RECT_F tabsLoc = newLoc;


    switch (tabShow)
    {
    case 1:
        childSpace.top += tabHeight;
        tabsLoc.bottom = childSpace.top;
        tabBar->OnResize(tabsLoc, 0, cred, args);
        break;
    case 2:
        childSpace.bottom -= tabHeight;
        tabsLoc.top = childSpace.bottom;
        tabBar->OnResize(tabsLoc, 0, cred, args);
        break;
    }

    TRandomLayout::OnResize(childSpace, nFlags, cred, args);

    area = newLoc;
}

bool TSwitchControl::OnScroll(bool b, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>& args)
{
    bool ret = true;
    if (tabBar.Get() && tabShow)
    {
        ret &= tabBar->OnScroll(b, point, direction, args);
    }


    UINT pss = pageStack.Size();
    if (pss)
    {
        auto top = pageStack[pss - 1];
        if (top.Get())
            ret &= top->OnScroll(b, point, direction, args);
        else
            pageStack.RemoveAt(pss - 1);
    }
    return ret;
}

void TSwitchControl::SetView(TrecPointer<TPage> page)
{
    while (pageStack.Size() >= maxStack)
    {
        bool removed = false;
        for (UINT Rust = 0; Rust < pageStack.Size(); Rust++)
        {
            if (page.Get() == pageStack[Rust].Get())
            {
                pageStack.RemoveAt(Rust);
                removed = true;
                break;
            }
        }

        if (!removed)
            pageStack.RemoveAt(0);
    }

    pageStack.push_back(page);
}

void TSwitchControl::RemovePage(TrecPointer<TPage> page)
{
    for (UINT Rust = 0; Rust < pageStack.Size(); Rust++)
    {
        if (page.Get() == pageStack[Rust].Get())
        {
            pageStack.RemoveAt(Rust--);
            continue;
        }
    }
}