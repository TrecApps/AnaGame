#pragma once
#include "TControl.h"
class TSliderControl :	public TControl
{
public:
	TSliderControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> styles);
	~TSliderControl();

	virtual UCHAR* GetAnaGameType()override;

protected:
	bool isVertical;
	float currentSliderLocation;
	D2D1_COLOR_F sliderColor;
	D2D1_COLOR_F lineColor;
	D2D1_COLOR_F hoverLineColor;
	float lineThickness;
	float tickDistance;
};

