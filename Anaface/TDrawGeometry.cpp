
#include "TDrawGeometry.h"


TDrawGeometry::TDrawGeometry()
{
	thickness = 1.0f;
	fill = false;
}

TDrawGeometry::TDrawGeometry(bool fill)
{
	this->fill = fill;
	thickness = 1.0f;
}


TDrawGeometry::~TDrawGeometry()
{
}

void TDrawGeometry::setPoint(D2D1_POINT_2F, int)
{
}

void TDrawGeometry::setWidth(int)
{
}

void TDrawGeometry::setHeight(int)
{
}

void TDrawGeometry::setClosure(bool)
{
}

D2D1_POINT_2F TDrawGeometry::getPoint(int)
{
	return D2D1_POINT_2F();
}

int TDrawGeometry::getHeight()
{
	return 0;
}

int TDrawGeometry::getWidth()
{
	return 0;
}

void TDrawGeometry::onCreate(D2D1_RECT_F, bool)
{
}

bool TDrawGeometry::getClosure()
{
	return false;
}

void TDrawGeometry::onDraw(ID2D1Brush*, ID2D1RenderTarget*)
{
}

void TDrawGeometry::setFill(bool f)
{
	fill = f;
}

float TDrawGeometry::getThickness()
{
	return thickness;
}

void TDrawGeometry::setThickness(float f)
{
	thickness = f;
}
