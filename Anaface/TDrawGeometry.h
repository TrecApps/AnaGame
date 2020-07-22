#pragma once
#include <d2d1.h>
#include <TrecReference.h>
#include "Anaface.h"

class _ANAFACE_DLL TDrawGeometry
{
public:
	TDrawGeometry();
	TDrawGeometry(bool fill);
	~TDrawGeometry();

	virtual void setPoint(D2D1_POINT_2F,int);
	virtual void setWidth(int);
	virtual void setHeight(int);
	virtual void setClosure(bool);
	virtual D2D1_POINT_2F getPoint(int);
	virtual int getHeight();
	virtual int getWidth();
	virtual void onCreate(D2D1_RECT_F, bool);
	virtual bool getClosure();
	virtual void onDraw(ID2D1Brush*,ID2D1RenderTarget*);
	virtual void setFill(bool f);
	virtual float getThickness();
	virtual void setThickness(float f);
protected:
	bool fill;
	float thickness;
	TrecComPointer<ID2D1GeometrySink> sinker;
};

