#include "TabBar.h"

void TabBar::Tab::SetBrush(TrecPointer<TBrush> brush, bool doIcon)
{
}

TabBar::Tab::Tab(TrecPointer<DrawingBoard> board): TPage(board)
{
}

bool TabBar::Tab::HandlesEvents()
{
    return false;
}

void TabBar::Tab::Draw(TrecPointer<TVariable> object)
{
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
}

bool TabBar::Tab::OnDestroy()
{
    return false;
}

bool TabBar::Tab::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
    return false;
}

void TabBar::SetTabSizes()
{
}

TabBar::TabBar(TrecPointer<DrawingBoard> board): TPage(board), leftTab(board), rightTab(board)
{
    draggableTabs = false;
    startTab = 0;
}

bool TabBar::HandlesEvents()
{
    return false;
}

void TabBar::Draw(TrecPointer<TVariable> object)
{
}

void TabBar::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&)
{
}

void TabBar::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&)
{
}

void TabBar::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
}

bool TabBar::OnDestroy()
{
    return false;
}

bool TabBar::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
    return false;
}
