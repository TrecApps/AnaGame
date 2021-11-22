#include "TabBar.h"
#include <DirectoryInterface.h>


TrecPointer<TPage> TabBar::Tab::GetContent()
{
    return content;
}

void TabBar::Tab::SetBrush(TrecPointer<TBrush> brush, bool doIcon)
{
    if (doIcon)
        this->image = brush;
    else
        this->color = brush;
}

TabBar::Tab::Tab(TrecPointer<DrawingBoard> board): TPage(board)
{
    iArea = tArea = xArea = { 0,0,0,0 };
    isActive = true;
}

bool TabBar::Tab::HandlesEvents()
{
    return false;
}

void TabBar::Tab::Draw(TrecPointer<TVariable> object)
{
    if (color.Get())
        color->FillRectangle(area);
    if (image.Get())
        image->FillRectangle(iArea);
   

    if (text.Get())
        text->OnDraw(object);
    if (exit.Get())
        exit->FillRectangle(xArea);
}

void TabBar::Tab::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::Tab::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::Tab::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::Tab::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::Tab::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::Tab::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&)
{
}

void TabBar::Tab::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
    area = newLoc;
    iArea = tArea = xArea = { 0,0,0,0 };
    if (image.Get())
    {
        iArea = area;
        iArea.right = iArea.left + (iArea.bottom - iArea.top);
    }

    if (text.Get())
    {
        bool w;
        float f = text->GetMinWidth(w);
        assert(w);
        tArea = area;
        if (iArea.right)
            tArea.left = iArea.right;

        tArea.right = f + 10 + tArea.left;

        text->SetLocation(tArea);
    }

    if (exit.Get())
    {
        xArea = area;
        float xSize = min(20, (xArea.bottom- xArea.top));

        float vBuffer = ((xArea.bottom - xArea.top) - xSize) / 2;

        xArea.left = tArea.right ? tArea.right : area.left;
        xArea.right = xArea.left + xSize;
        xArea.bottom -= vBuffer;
        xArea.top += vBuffer;
    }

    if ((area.right < (xArea.right + 10)) || (area.right < tArea.right))
        area = { 0,0,0,0 };
    else
    {
        if (xArea.right)
            area.right = xArea.right + 10;
        else if (tArea.right)
            area.right = tArea.right;
    }
}

bool TabBar::Tab::OnDestroy()
{
    return false;
}

bool TabBar::Tab::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
    return false;
}

TString TabBar::Tab::GetName()
{
    TString ret;
    if(text.Get()) text->GetText(ret);
    return ret;
}

void TabBar::SetTabSizes()
{
    auto tempArea = area;
    // Assume no ovrflow occured

    TDataArray<EventID_Cred> cred;
    TDataArray<EventArgs> args;

    tabOverflow = false;

    for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
    {
        if (dynamic_cast<Tab*>(tabs[Rust].Get())->isActive)
            continue;
        tabs[Rust]->OnResize(tempArea, 0, cred, args);
        tempArea.left = tabs[Rust]->GetArea().right;

        if (!tempArea.left)
            break;
    }

    if (tempArea.left)
    {
        startTab = 0;
        return;
    }
    tabOverflow = true;

    tempArea = area;
    leftTab.OnResize(tempArea, 0, cred, args);
    tempArea.left = leftTab.GetArea().right;
    
    rightTab.OnResize(tempArea, 0, cred, args);
    tempArea.left = rightTab.GetArea().right;


    for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
    {
        tabs[Rust]->OnResize(tempArea, 0, cred, args);
        tempArea.left = tabs[Rust]->GetArea().right;

        if (!tempArea.left)
            break;
    }
}

TabBar::TabBar(TrecPointer<DrawingBoard> board): TPage(board), leftTab(board), rightTab(board)
{
    draggableTabs = tabOverflow = false;
    startTab = 0;

    tabMode = tab_mode::tm_not_set;

    unknownCount = 0;

    SetConfigure();

    TString direct = GetDirectoryWithSlash(CentralDirectories::cd_Executable);
    D2D1_RECT_F loc{0, 0, 20, 20};
    exitReg = drawingBoard->GetBrush(TFileShell::GetFileInfo(direct + L"Resources\\DefaultImages\\RegularX.png"), loc);
    exitHover = drawingBoard->GetBrush(TFileShell::GetFileInfo(direct + L"Resources\\DefaultImages\\HoverX.png"), loc);
}

bool TabBar::HandlesEvents()
{
    return false;
}

void TabBar::InjectTabAt(TrecPointer<TPage> page, UINT index)
{
    assert(dynamic_cast<Tab*>(page.Get()));

    for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
    {
        if (tabs[Rust].Get() == page.Get())
        {
            if (Rust < index)
                index--;
            tabs.RemoveAt(Rust);
            break;
        }
    }
    tabs.InsertAt(page, index);
    if (holder.Get()) holder->SetView(dynamic_cast<Tab*>(page.Get())->content);
    SetTabSizes();
}

void TabBar::ActivateTabs(const TString& targets, const TString& exceptions, bool activate, bool doMatch)
{
    if (!draggableTabs)
        return;

    TrecPointer<TDataArray<TString>> targetList = targets.split(L';'),
        exceptionList = exceptions.split(L';');

    for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
    {
        TString name(dynamic_cast<Tab*>(tabs[Rust].Get())->GetName());
        name.Trim();

        bool doContinue = false;

        // Make sure tab is not in exception list
        for (UINT C = 0; Rust < exceptionList->Size(); C++)
        {
            if (!name.Compare(exceptionList->at(C).GetTrim()))
            {
                doContinue = true;
                break;
            }
        }
        if (doContinue)continue;

        // Now see if tab is in target list
        for (UINT C = 0; Rust < exceptionList->Size(); C++)
        {
            if (!name.Compare(exceptionList->at(C).GetTrim()))
            {
                doContinue = true;
                break;
            }
        }

        if (doMatch == doContinue)
            dynamic_cast<Tab*>(tabs[Rust].Get())->isActive = activate;
    }

    SetTabSizes();
}

void TabBar::SetHolder(TrecPointer<TabBarHolder> holder)
{
    this->holder = holder;
}

UINT TabBar::GetTabCount()
{
    return tabs.Size();
}

TrecPointer<TPage> TabBar::GetTabAt(UINT index, bool content)
{
    if(index < tabs.Size())
        return TrecPointer<TPage>();

    TrecPointer<TPage> ret = tabs[index];

    if (content)
    {
        return dynamic_cast<Tab*>(ret.Get())->content;
    }
    return ret;
}

void TabBar::Draw(TrecPointer<TVariable> object)
{
    if (tabOverflow)
    {
        leftTab.Draw(object);
        rightTab.Draw(object);
    }
    for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
    {
        if (dynamic_cast<Tab*>(tabs[Rust].Get())->isActive)
            continue;
        if (!tabs[Rust]->GetArea().right)
            break;
        tabs[Rust]->Draw(object);
    }
}

bool TabBar::RemoveTab(TrecPointer<TPage> page)
{
    if(!page.Get())
        return false;

    for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
    {
        if (tabs[Rust].Get() == page.Get())
        {
            tabs.RemoveAt(Rust);
            SetTabSizes();
            return true;
        }
    }
    return false;
}

void TabBar::SetConfigure(bool makeDraggable, bool doExit)
{
    this->draggableTabs = makeDraggable;
    this->exitSupport = doExit;
}

TrecPointer<TPage> TabBar::AddNewTab(const TString& name, TrecPointer<TPage> page, bool exit)
{
    assert(page.Get());

    if (dynamic_cast<Tab*>(page.Get()))
    {
        if(!draggableTabs)
            return TrecPointer<TPage>();

        bool found = false;
        for (UINT Rust = 0; !found && Rust < tabs.Size(); Rust++)
        {
            if (tabs[Rust].Get() == page.Get())
            {
                found = true;
            }
        }

        if (!found)
        {
            tabs.push_back(page);
            SetTabSizes();
            return page;
        }
        return TrecPointer<TPage>();
    }

    TrecPointer<TTextElement> textEle = TrecPointerKey::GetNewTrecPointer<TTextElement>(drawingBoard);

    TrecPointer<TPage> newTab = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, Tab>(drawingBoard);
    
    TString newName(name);

    if (!newName.GetSize())
        newName.Format(L"Unknown%d", unknownCount++);

    textEle->SetText(newName);

    dynamic_cast<Tab*>(newTab.Get())->text = textEle;

    if (exit)
    {
        dynamic_cast<Tab*>(newTab.Get())->exit = TrecPointerKey::GetTrecPointerFromSub<>(exitReg);
    }

    tabs.push_back(newTab);
    SetTabSizes();
    return newTab;

}

bool TabBar::InjectTabAt(const TPoint& point, TrecPointer<TPage> page)
{
    if (!draggableTabs)
        return false;
    assert(dynamic_cast<Tab*>(page.Get()));
    if(!IsContained(point, area))
        return false;

    if (tabOverflow && (IsContained(point, leftTab.area) || IsContained(point, rightTab.area)))
        return false;

    for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
    {
        auto cArea = dynamic_cast<Tab*>(tabs[Rust].Get())->area;
        auto halfWidth = ((cArea.right - cArea.left) / 2);
        cArea.right = cArea.left + halfWidth;
        if (IsContained(point, cArea))
        {
            InjectTabAt(page, Rust);
            break;
        }
        cArea.left += halfWidth;
        cArea.right += halfWidth;
        if (IsContained(point, cArea))
        {
            InjectTabAt(page, Rust + 1);
            break;
        }
    }
}

void TabBar::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
    if (tabMode == tab_mode::tm_exit)
    {
        if (currentTab.Get() && IsContained(point, dynamic_cast<Tab*>(currentTab.Get())->xArea))
        {
            for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
            {
                if (dynamic_cast<Tab*>(tabs[Rust].Get())->isActive)
                    continue;
                if (currentTab.Get() == tabs[Rust].Get())
                {
                    tabs.RemoveAt(Rust);
                    if (holder.Get()) holder->RemoveView(currentTab);
                    break;
                }
            }
        }
    }
    else if (tabMode == tab_mode::tm_regular && currentTab.Get())
    {
        if (IsContained(point, dynamic_cast<Tab*>(currentTab.Get())->area))
        {
            if (this->holder.Get())
                holder->SetView(dynamic_cast<Tab*>(currentTab.Get())->content);
        }
        else if (currentTab.Get() && draggableTabs)
        {
            InjectTabAt(point, currentTab);
        }
    }
    else if (tabMode == tab_mode::tm_left)
    {
        if (startTab && IsContained(point, leftTab.area))
            startTab--;
    }
    else if (tabMode == tab_mode::tm_right)
    {
        if (IsContained(point, rightTab.area))
            startTab++;
    }

    currentTab.Nullify();
    tabMode = tab_mode::tm_not_set;
}

void TabBar::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
    tabMode = tab_mode::tm_not_set;

    if (IsContained(point, area))
        return;

    if (tabOverflow)
    {
        if (IsContained(point, leftTab.area))
            tabMode = tab_mode::tm_left;
        else if (IsContained(point, rightTab.area))
            tabMode = tab_mode::tm_right;

        if (tabMode != tab_mode::tm_not_set)
            return;
    }

    for (UINT Rust = startTab; Rust < tabs.Size(); Rust++)
    {
        if (dynamic_cast<Tab*>(tabs[Rust].Get())->isActive)
            continue;
        if (IsContained(point, dynamic_cast<Tab*>(tabs[Rust].Get())->area))
        {
            tabMode = IsContained(point, dynamic_cast<Tab*>(tabs[Rust].Get())->xArea) ? tab_mode::tm_exit : tab_mode::tm_regular;
            currentTab = tabs[Rust];
            break;
        }
    }
}

void TabBar::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>&)
{
    if (currentTab.Get() && !IsContained(point, area) && draggableTabs)
    {
        EventID_Cred credObj(R_Message_Type::On_SubmitDrag, currentTab);
        cred.push_back(credObj);
    }

    for (UINT Rust = 0; Rust < tabs.Size(); Rust++)
    {
        if (dynamic_cast<Tab*>(tabs[Rust].Get())->isActive)
            continue;
        if (dynamic_cast<Tab*>(tabs[Rust].Get())->exit.Get())
            dynamic_cast<Tab*>(tabs[Rust].Get())->exit = IsContained(point, dynamic_cast<Tab*>(tabs[Rust].Get())->xArea) ?
            TrecPointerKey::GetTrecPointerFromSub<>(exitHover) : TrecPointerKey::GetTrecPointerFromSub<>(exitReg);
    }

}

void TabBar::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&)
{
}

void TabBar::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
    this->area = newLoc;
    SetTabSizes();
}

bool TabBar::OnDestroy()
{
    return false;
}

bool TabBar::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
    area.bottom += direction.y;
    area.right += direction.x;
    area.left += direction.x;
    area.top += direction.y;
    
    SetTabSizes();

    return true;
}
