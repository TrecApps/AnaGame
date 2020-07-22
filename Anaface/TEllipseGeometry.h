#pragma once
#include "TDrawGeometry.h"
class TEllipseGeometry :public TDrawGeometry
{
public:
	TEllipseGeometry();
	~TEllipseGeometry();
	virtual void setPoint(D2D1_POINT_2F, int);
	virtual void setWidth(int);
	virtual void setHeight(int);

	virtual D2D1_POINT_2F getPoint(int);
	virtual int getHeight();
	virtual int getWidth();
	virtual void onCreate(RECT, bool);

	virtual void onDraw(ID2D1Brush*, ID2D1DCRenderTarget*);
private:
	D2D1_ELLIPSE center;

};

