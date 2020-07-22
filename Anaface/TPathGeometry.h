#pragma once
#include "TDrawGeometry.h"
class TPathGeometry :
	public TDrawGeometry
{
public:
	TPathGeometry();
	~TPathGeometry();
	virtual void setPoint(D2D1_POINT_2F, int);

	virtual void setClosure(bool);
	virtual D2D1_POINT_2F getPoint(int);

	virtual void onCreate(RECT, bool);
	virtual bool getClosure();
	virtual void onDraw(ID2D1Brush*, ID2D1DCRenderTarget*);
private:
	
};

