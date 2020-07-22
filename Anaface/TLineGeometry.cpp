
#include "TLineGeometry.h"


TLineGeometry::TLineGeometry()
{
}


TLineGeometry::~TLineGeometry()
{
}

void TLineGeometry::setPoint(D2D1_POINT_2F p, int pN)
{
	if (pN == 1)
		point1 = p;
	else if (pN == 2)
		point2 = p;
}

D2D1_POINT_2F TLineGeometry::getPoint(int n)
{
	if (n == 1)
		return point1;
	else if (n == 2)
		return point2;
	return D2D1::Point2F();
}

void TLineGeometry::onCreate(RECT cont, bool comp)
{
	if (comp)
	{
		int width = point1.x - point2.x;
		if (0 > width)
			width = -width;
		int height = point1.y - point2.y;
		if (0 > height)
			height = -height;
		int contWidth = cont.right - cont.left;
		int contHeight = cont.bottom - cont.top;
		int perWidth = width / contWidth;
		int perHeight = height / contHeight;
		point1.x *= perWidth;
		point2.x *= perWidth;
		point1.y *= perHeight;
		point2.y *= perHeight;
	}
	point1.x += cont.left;
	point2.x += cont.left;
	point1.y += cont.top;
	point2.y += cont.top;
}

void TLineGeometry::onDraw(ID2D1Brush * br, ID2D1DCRenderTarget * rt)
{
	if (!rt || !br)
		return;
	rt->DrawLine(point1, point2, br);
}
