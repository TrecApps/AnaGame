#include "ColorAnimation.h"
#include <TString.h>
#include <atltrace.h>
#include <TBrush.h>

TString colorAnimationString(L"Simple-Color");

/**
 * Method: ColorAnimation::Animation (Constructor)
 * Purpose: Creates the ColorAnimation with the specified animation-phase setting
 * Parameters: animation_phase ap - the animation_phase this Animation is set to
 * Returns: ColorAnimation
 */
ColorAnimation::ColorAnimation(animation_phase ap): Animation(ap)
{
	color1 = color2 = D2D1::ColorF(D2D1::ColorF::Azure);
	color1Set = color2Set = false;
}

/**
 * Method: ColorAnimation::Update
 * Purpose: Informs the animation regarding what progress a given animation should be along
 * Parameters: float progress - the progression a between the two colors provided
 * Returns: bool - true if the resources are in order, false if a resource is missing
 */
bool ColorAnimation::Update(float progress)
{
	ATLTRACE(L"COLOR ANIMATION UPDATE called!\n");
	if (!brush.Get())
		return false;
	if(millisecondLength && millisecondStoryLength)
		progress = progress * (static_cast<float>(this->millisecondLength) / static_cast<float>(this->millisecondStoryLength));

	D2D1_COLOR_F curColor{
		GetFloatBetween(color1.r, color2.r, progress),
		GetFloatBetween(color1.g, color2.g, progress),
		GetFloatBetween(color1.b, color2.b, progress),
		GetFloatBetween(color1.a, color2.a, progress)
	};
	TString print;
	print.Format(L"COLOR ANIMATION R: %f G: %f, B: %f, A: %f\n", curColor.r, curColor.g, curColor.b, curColor.a);
	ATLTRACE(print.GetConstantBuffer());

	try
	{
		brush.Get()->SetColor(curColor);
	}
	catch (...)
	{
		ATLTRACE(L"Exception Caught Attempting to cast Brush. Was it not a SolidColor Brush?");
	}
	return true;
}

void ColorAnimation::SetAnimationValue(float value, animation_value_type type)
{
	if (value > 1.0f)
	{
		UINT iValue = static_cast<UINT>(value);

		value /= static_cast<float>(iValue + 1);
	}

	switch (type)
	{
	case animation_value_type::animation_value_type_alpha:
	case animation_value_type::animation_value_type_blue:
	case animation_value_type::animation_value_type_red:
	case animation_value_type::animation_value_type_green:
		color1Set = true;
		break;
	case animation_value_type::animation_value_type_alpha2:
	case animation_value_type::animation_value_type_blue2:
	case animation_value_type::animation_value_type_red2:
	case animation_value_type::animation_value_type_green2:
		color2Set = true;
		break;
	}


	switch (type)
	{
	case animation_value_type::animation_value_type_alpha:
		color1.a = value;
		break;
	case animation_value_type::animation_value_type_blue:
		color1.b = value;
		break;
	case animation_value_type::animation_value_type_green:
		color1.g = value;
		break;
	case animation_value_type::animation_value_type_red:
		color1.r = value;
		break;
	case animation_value_type::animation_value_type_alpha2:
		color2.a = value;
		break;
	case animation_value_type::animation_value_type_blue2:
		color2.b = value;
		break;
	case animation_value_type::animation_value_type_green2:
		color2.g = value;
		break;
	case animation_value_type::animation_value_type_red2:
		color2.r = value;
		break;
	}
}

/**
 * Method: ColorAnimation::SetComponent
 * Purpose: Applies a TBrush to the animation to act upon
 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
 * Returns: void
 */
void ColorAnimation::SetComponent(TrecPointer<TBrush> comp)
{
	brush = comp;
	if (!brush.Get())
		return;
	if (color1Set && !color2Set)
		color2 = brush->GetColor().GetColor();
	else if (!color1Set && color2Set)
		color1 = brush->GetColor().GetColor();
}

/**
 * Method: ColorAnimation::SetControl
 * Purpose: Allows animations to get the control they need to act upon
 * Parameters: TrecPointer<TControl> con -  the control to operate upon
 * Returns: void
 */
void ColorAnimation::SetControl(TrecPointer<TControl> con)
{
}

/**
 * Method: ColorAnimation::Prepare
 * Purpose: Sets an animation to its starting position before an animation commences
 * Parameters: void
 * Returns: void
 */
void ColorAnimation::Prepare()
{
	if (done && reverse)
	{
		D2D1_COLOR_F tempColor = color1;
		color1 = color2;
		color2 = tempColor;
	}
	done = false;
}

/**
 * Method: ColorAnimation::GetAnimationType
 * Purpose: Reports the String version of the animation type
 * Parameters: void
 * Returns: TString - representing the type this animation object is
 *
 * Note: In this case, the String is L"Simple-Color"
 */
TString ColorAnimation::GetAnimationType()
{
	return colorAnimationString;
}


