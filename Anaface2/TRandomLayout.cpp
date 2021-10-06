#include "TRandomLayout.h"

#define T_RANDOM_LAYOUT_LIMIT 10

bool TRandomLayout::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
    bool res = TControl::onCreate(loc, d3d);

    for (UINT Rust = 0; Rust < childControls.Size(); Rust++)
    {
        auto page = childControls[Rust].control;
        if (dynamic_cast<TControl*>(page.Get()))
            res &= dynamic_cast<TControl*>(page.Get())->onCreate(loc, d3d);
    }
    return res;
}

TRandomLayout::TRandomLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles): TControl(drawingBoard, styles)
{
}

TRandomLayout::~TRandomLayout()
{
}

void TRandomLayout::Draw(TrecPointer<TVariable> object)
{
    TControl::Draw(object);
    auto var = TrecPointer<TVariable>();
    for (UINT Rust = 0; Rust < childControls.Size(); Rust++)
    {
        auto page = childControls[Rust].control;
        if (page.Get())
            page->Draw(var);
    }
}

void TRandomLayout::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if (isRightClicked)
        TControl::OnRButtonUp(nFlags, point, mOut, cred, args);
    else
    {
        for (UINT Rust = 0; Rust < childControls.Size() && mOut == message_output::mo_negative; Rust++)
        {
            auto page = childControls[Rust].control;
            if (page.Get())
                page->OnRButtonUp(nFlags, point, mOut, cred, args);
        }

        if (mOut != message_output::mo_positive_override)
            TControl::OnRButtonUp(nFlags, point, mOut, cred, args);
    }
}

void TRandomLayout::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    for (UINT Rust = 0; Rust < childControls.Size() && mOut == message_output::mo_negative; Rust++)
    {
        auto page = childControls[Rust].control;
        if (page.Get())
            page->OnRButtonDown(nFlags, point, mOut, cred, args);
    }

    if (mOut != message_output::mo_positive_override)
        TControl::OnRButtonDown(nFlags, point, mOut, cred, args);
}

void TRandomLayout::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if(isLeftClicked)
        TControl::OnLButtonUp(nFlags, point, mOut, cred, args);
    else
    {
        for (UINT Rust = 0; Rust < childControls.Size() && mOut == message_output::mo_negative; Rust++)
        {
            auto page = childControls[Rust].control;
            if (page.Get())
                page->OnLButtonUp(nFlags, point, mOut, cred, args);
        }

        if (mOut != message_output::mo_positive_override)
            TControl::OnLButtonUp(nFlags, point, mOut, cred, args);
    }
}

void TRandomLayout::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    if (isMouseIn)
        TControl::OnMouseMove(nFlags, point, mOut, cred, args);
    else
    {
        for (UINT Rust = 0; Rust < childControls.Size() && mOut == message_output::mo_negative; Rust++)
        {
            auto page = childControls[Rust].control;
            if (page.Get())
                page->OnMouseMove(nFlags, point, mOut, cred, args);
        }

        if (mOut != message_output::mo_positive_override)
            TControl::OnMouseMove(nFlags, point, mOut, cred, args);
    }
}

void TRandomLayout::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>& args)
{
    for (UINT Rust = 0; Rust < childControls.Size() && mOut == message_output::mo_negative; Rust++)
    {
        auto page = childControls[Rust].control;
        if (page.Get())
            page->OnLButtonDblClk(nFlags, point, mOut, args);
    }

    if (mOut != message_output::mo_positive_override)
        TControl::OnLButtonDblClk(nFlags, point, mOut, args);
}

void TRandomLayout::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
    for (UINT Rust = 0; Rust < childControls.Size() && mOut == message_output::mo_negative; Rust++)
    {
        auto page = childControls[Rust].control;
        if (page.Get())
            page->OnLButtonDown(nFlags, point, mOut, cred, args);
    }

    if (mOut != message_output::mo_positive_override)
        TControl::OnLButtonDown(nFlags, point, mOut, cred, args);
}

void TRandomLayout::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
    for (UINT Rust = 0; Rust < childControls.Size(); Rust++)
    {
        auto page = childControls[Rust].control;
        if (page.Get())
            page->OnResize(newLoc, nFlags, eventAr, args);
    }

    TControl::OnResize(newLoc, nFlags, eventAr, args);
}

bool TRandomLayout::OnScroll(const TPoint& point, const TPoint& direction, TDataArray<EventArgs>& args)
{
    if(TControl::OnScroll(point, direction, args))
        return false;

    for (UINT Rust = 0; Rust < childControls.Size(); Rust++)
    {
        auto page = childControls[Rust].control;
        if (page.Get())
            page->OnScroll(point, direction, args);
    }
    return true;
}

bool TRandomLayout::AddPage(TrecPointer<TPage> page, UINT row, UINT col, bool doOverride)
{
    if (!page.Get())
        return false;
    ChildControl cc;
    cc.col = col;
    cc.row = row;
    cc.control = page;

    for (UINT Rust = 0; Rust < childControls.Size(); Rust++)
    {
        auto page = childControls[Rust].control;
        if (!page.Get())
        {
            childControls[Rust] = cc;
            return true;
        }
    }
    if(childControls.Size() >= T_RANDOM_LAYOUT_LIMIT)
        return false;

    childControls.push_back(cc);
    return true;
}

bool TRandomLayout::RemovePage(TrecPointer<TPage> page)
{
    for (UINT Rust = 0; Rust < childControls.Size(); Rust++)
    {
        if (page.Get() == childControls[Rust].control.Get())
        {
            childControls[Rust].control.Nullify();
            return true;
        }
    }
    return false;
}

bool TRandomLayout::RemovePage(UINT row, UINT col)
{
    if (row < childControls.Size())
    {
        childControls[row].control.Nullify();
        return true;
    }
    return false;
}

void TRandomLayout::GetControlCapacity(UINT& row, UINT& col)
{
    row = T_RANDOM_LAYOUT_LIMIT;
    col = 1;
}

TrecPointer<TPage> TRandomLayout::GetPage(UINT row, UINT col)
{
    if (row < childControls.Size())
    {
        return childControls[row].control;
    }
    return TrecPointer<TPage>();
}

ChildControl::ChildControl()
{    row = col = 0; 
}

ChildControl::ChildControl(const ChildControl& copy)
{
    row = copy.row;
    col = copy.col;
    control = copy.control;
}
