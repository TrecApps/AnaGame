#pragma once
#include <Animation.h>

class ColorAnimation : public Animation
{
public:
	ColorAnimation(animation_phase ap);
	virtual bool Update(float progress);
	virtual void SetAnimationValue(float value, animation_value_type type);

	virtual void SetComponent(TrecComPointer<ID2D1Brush> comp);
	virtual void Prepare();
	virtual TString GetAnimationType();
protected:
	D2D1_COLOR_F color1, color2;
	TrecComPointer<ID2D1Brush> brush;
};
