#include "RotateAnimation.h"
#include <TString.h>
#include "TControl.h"

TString rotationAnimationString(L"Simple-Rotation");

/**
 * Method: RotateAnimation::Animation (Constructor)
 * Purpose: Creates the Rotation Animation with the specified animation-phase setting
 * Parameters: animation_phase ap - the animation_phase this Animation is set to
 * Returns: RotateAnimation
 */
RotateAnimation::RotateAnimation(animation_phase ap): Animation(ap)
{
	rotateStart = 0.0f;
	rotateEnd = 360.0f;
	clockwise = true;
	degrees = true;
}

/**
 * Method: RotateAnimation::Update
 * Purpose: Informs the animation regarding what progress a given animation should be along
 * Parameters: float progress - the progression of the rotation
 * Returns: bool - true if the resources are in order, false if a resource is missing
 */
bool RotateAnimation::Update(float progress)
{
	if(!control.Get())
		return false;

	if (millisecondLength && millisecondStoryLength)
		progress = progress * (static_cast<float>(this->millisecondLength) / static_cast<float>(this->millisecondStoryLength));

	if (degrees)
		control->RotateDegrees(GetFloatBetween(rotateStart, rotateEnd, progress));
	else
		control->RotateRadians(GetFloatBetween(rotateStart, rotateEnd, progress));
	return true;
}

/**
 * Method: RotateAnimation::SetAnimationValue
 * Purpose: Sets a value needed to define an animation beginning and end
 * Parameters: float value - the value of an animation
 *				animation_value_type type - the meaning behind the value parameter
 * Returns: void
 */
void RotateAnimation::SetAnimationValue(float value, animation_value_type type)
{
	switch (type)
	{
	case animation_value_type::animation_value_type_radian:
		degrees = false;
		break;
	case animation_value_type::animation_value_type_rotate_start:
		rotateStart = value;
		break;
	case animation_value_type::animation_value_type_rotate_end:
		rotateEnd = value;
		break;
	case animation_value_type::animation_value_type_direction:
		if (value == 0.0f)
		{
			if (rotateEnd > rotateStart)
			{
				float temp = rotateEnd;
				rotateEnd = rotateStart;
				rotateStart = temp;
			}
			clockwise = false;
		}
	}
}

/**
 * Method: RotateAnimation::SetComponent
 * Purpose: Applies a TBrush to the animation to act upon
 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
 * Returns: void
 *
 * Note: Has no affect as Brushes are not directly involved in rotation operations
 */
void RotateAnimation::SetComponent(TrecPointer<TBrush> comp)
{
}

/**
 * Method: RotateAnimation::Prepare
 * Purpose: Sets an animation to its starting position before an animation commences
 * Parameters: void
 * Returns: void
 *
 * Note: Abstract as each type can decide what its starting position is
 */
void RotateAnimation::Prepare()
{
}

/**
 * Method: RotateAnimation::GetAnimationType
 * Purpose: Reports the String version of the animation type
 * Parameters: void
 * Returns: TString - representing the type this animation object is
 *
 * Note: In this case, the String is L"Simple-Rotation"
 */
TString RotateAnimation::GetAnimationType()
{
	return rotationAnimationString;
}

/**
 * Method: RotateAnimation::SetControl
 * Purpose: Allows animations to get the control they need to act upon
 * Parameters: TrecPointer<TControl> con -  the control to operate upon
 * Returns: void
 */
void RotateAnimation::SetControl(TrecPointer<TControl> con)
{
	control = con;
}
