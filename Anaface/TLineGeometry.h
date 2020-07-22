#pragma once
#include "TDrawGeometry.h"
class TLineGeometry :public TDrawGeometry
{
public:
	TLineGeometry();
	~TLineGeometry();
	virtual void setPoint(D2D1_POINT_2F, int);

	virtual D2D1_POINT_2F getPoint(int);

	virtual void onCreate(RECT, bool);
	virtual void onDraw(ID2D1Brush*, ID2D1DCRenderTarget*);
private:
	D2D1_POINT_2F point1;
	D2D1_POINT_2F point2;
};

