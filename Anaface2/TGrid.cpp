#include "TGrid.h"

bool TGrid::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
    return false;
}

TGrid::TGrid(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles): TLayout(drawingBoard, styles)
{
}

TGrid::~TGrid()
{
}

void TGrid::Draw(TrecPointer<TVariable> object)
{
}

void TGrid::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
}

bool TGrid::OnScroll(const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
    return false;
}

bool TGrid::AddPage(TrecPointer<TPage> page, UINT row, UINT col, bool doOverride)
{
    return false;
}

bool TGrid::RemovePage(UINT row, UINT col)
{
    return false;
}

void TGrid::GetControlCapacity(UINT& row, UINT& col)
{
}
