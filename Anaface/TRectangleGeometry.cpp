
#include "TRectangleGeometry.h"


TRectangleGeometry::TRectangleGeometry()
{
	height = width = 10;
	rectangle.left = 0;
	rectangle.top = 0;
	rectangle.bottom = 10;
	rectangle.right = 10;
}


TRectangleGeometry::~TRectangleGeometry()
{
}

void TRectangleGeometry::setPoint(D2D1_POINT_2F p, int)
{
	rectangle.left = p.x;
	rectangle.top = p.y;
	rectangle.bottom = rectangle.top + height;
	rectangle.right = rectangle.left + width;
}

void TRectangleGeometry::setWidth(int w)
{
	width = w;
	rectangle.right = rectangle.left + width;
}

void TRectangleGeometry::setHeight(int h)
{
	height = h;
	rectangle.bottom = rectangle.top + height;
}

D2D1_POINT_2F TRectangleGeometry::getPoint(int)
{
	return D2D1::Point2F(rectangle.left,rectangle.top);
}

int TRectangleGeometry::getHeight()
{
	return height;
}

int TRectangleGeometry::getWidth()
{
	return width;
}

void TRectangleGeometry::onCreate(RECT cont, bool compress)
{
	if (compress)
	{
		int conWidth = cont.right - cont.left;
		int conHeight = cont.bottom - cont.top;
		float widthPer = width / (float)conWidth;
		float heightPer = height / (float)conHeight;
		rectangle.right *= widthPer;
		rectangle.left *= widthPer;
		rectangle.top *= heightPer;
		rectangle.bottom *= heightPer;
		height = rectangle.right - rectangle.left;
		width = rectangle.bottom - rectangle.top;
	}
	rectangle.top += cont.top;
	rectangle.bottom = +cont.top;
	rectangle.left += cont.left;
	rectangle.right += cont.left;
	
}

void TRectangleGeometry::onDraw(ID2D1Brush * br, ID2D1DCRenderTarget * rt)
{
	if (!br || !rt)
		return;
	if (fill)
		rt->FillRectangle(&rectangle, br);
	else
		rt->DrawRectangle(&rectangle, br,thickness);
}
