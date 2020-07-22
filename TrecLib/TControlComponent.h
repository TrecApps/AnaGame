#pragma once
#include "TObject.h"
#include <d2d1.h>
class TControlComponent :
	public TObject
{
public:
	virtual void SetColor(const D2D1_COLOR_F& color) = 0;
	virtual D2D1_COLOR_F GetColor() = 0;
	virtual void SetColor2(const D2D1_COLOR_F& color) = 0;
	virtual D2D1_COLOR_F GetColor2() = 0;
	virtual D2D1_RECT_F GetLocation() = 0;
	virtual void SetLocation(const D2D1_RECT_F& loc) = 0;
};

