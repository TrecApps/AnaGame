
#include "TEllipseGeometry.h"


TEllipseGeometry::TEllipseGeometry()
{
}


TEllipseGeometry::~TEllipseGeometry()
{
}

void TEllipseGeometry::setPoint(D2D1_POINT_2F p, int)
{
	center.point = p;
}

void TEllipseGeometry::setWidth(int w)
{
	center.radiusX = w / 2;
}

void TEllipseGeometry::setHeight(int h)
{
	center.radiusY = h / 2;
}

D2D1_POINT_2F TEllipseGeometry::getPoint(int)
{
	return center.point;
}

int TEllipseGeometry::getHeight()
{
	return center.radiusY *2;
}

int TEllipseGeometry::getWidth()
{
	return center.radiusX*2;
}

void TEllipseGeometry::onCreate(RECT cont, bool comp)
{
	if (comp)
	{
		int conWidth = cont.right - cont.left;
		int conHeight = cont.bottom - cont.top;
		float widthPer = center.radiusX *2.0 / (float)conWidth;
		float heightPer = center.radiusY*2.0 / (float)conHeight;

		center.radiusX *= widthPer;
		center.radiusY *= heightPer;
	}
	center.point.x += cont.left;
	center.point.y += cont.top;
}

void TEllipseGeometry::onDraw(ID2D1Brush * br, ID2D1DCRenderTarget * rt)
{
	if (!br || !rt)
		return;
	if (fill)
		rt->FillEllipse(center, br);
	else
		rt->DrawEllipse(center, br);
}
