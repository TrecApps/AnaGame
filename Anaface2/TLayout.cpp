#include "TLayout.h"

LayoutSpace::LayoutSpace()
{
	isFlex = false;
	space = 0;
}

LayoutSpace::LayoutSpace(const LayoutSpace& copy)
{
	isFlex = copy.isFlex;
	space = copy.space;
}

bool TLayout::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
	return false;
}

TLayout::TLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles) : TRandomLayout(drawingBoard, styles)
{
	primaryStack = true;
}

TLayout::~TLayout()
{

}

void TLayout::Draw(TrecPointer<TVariable> object)
{
	TRandomLayout::Draw(object);
}

void TLayout::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
}

bool TLayout::OnScroll(const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
	return false;
}

bool TLayout::AddPage(TrecPointer<TPage> page, UINT row, UINT col, bool doOverride)
{
	return false;
}

bool TLayout::RemovePage(UINT row, UINT col)
{
	return false;
}

void TLayout::GetControlCapacity(UINT& row, UINT& col)
{
}

TrecPointer<TPage> TLayout::GetPage(UINT row, UINT col)
{
	return TrecPointer<TPage>();
}
