
#include "TPathGeometry.h"


TPathGeometry::TPathGeometry()
{
}


TPathGeometry::~TPathGeometry()
{
}

void TPathGeometry::setPoint(D2D1_POINT_2F, int)
{
}

void TPathGeometry::setClosure(bool)
{
}

D2D1_POINT_2F TPathGeometry::getPoint(int)
{
	return D2D1_POINT_2F();
}

void TPathGeometry::onCreate(RECT, bool)
{
}

bool TPathGeometry::getClosure()
{
	return false;
}

void TPathGeometry::onDraw(ID2D1Brush *, ID2D1DCRenderTarget *)
{
}
