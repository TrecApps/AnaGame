#include "TransitionAnimation.h"
#include "TControl.h"

TString transitionAnimationString(L"Simple-Transition");

/**
 * Method: TransitionAnimation::Animation (Constructor)
 * Purpose: Creates the TransitionAnimation with the specified animation-phase setting
 * Parameters: animation_phase ap - the animation_phase this Animation is set to
 * Returns: TransitionAnimation
 */
TransitionAnimation::TransitionAnimation(animation_phase ap): Animation(ap)
{
	ZeroMemory(&loc1, sizeof(loc1));
	ZeroMemory(&loc2, sizeof(loc2));
}

/**
 * Method: TransitionAnimation::Update
 * Purpose: Informs the animation regarding what progress a given animation should be along
 * Parameters: float progress - the progression the control has made between departure and destination
 * Returns: bool - true if the resources are in order, false if a resource is missing
 */
bool TransitionAnimation::Update(float progress)
{
	if (!control.Get())
		return false;
	if (millisecondLength && millisecondStoryLength)
		progress = progress * (static_cast<float>(this->millisecondLength) / static_cast<float>(this->millisecondStoryLength));

	D2D1_RECT_F curLoc{
		GetFloatBetween(loc1.left, loc2.left, progress),
		GetFloatBetween(loc1.top, loc2.top, progress),
		GetFloatBetween(loc1.right, loc2.right, progress),
		GetFloatBetween(loc1.bottom, loc2.bottom, progress)
	};

	control->Resize(curLoc);
	return true;
}

/**
 * Method: TransitionAnimation::SetAnimationValue
 * Purpose: Sets a value needed to define an animation beginning and end
 * Parameters: float value - the value of an animation
 *				animation_value_type type - the meaning behind the value parameter
 * Returns: void
 *
 * Note: This one looks t the location of borders and ignores the other types
 */
void TransitionAnimation::SetAnimationValue(float value, animation_value_type type)
{
	if (animation_phase::animation_phase_end == phase)
	{
		switch (type)
		{
		case animation_value_type::animation_value_type_bottom_location:
			loc2.bottom = value;
			break;
		case animation_value_type::animation_value_type_left_location:
			loc2.left = value;
			break;
		case animation_value_type::animation_value_type_right_location:
			loc2.right = value;
			break;
		case animation_value_type::animation_value_type_top_location:
			loc2.top = value;
			break;
		}
	}
	else
	{
		switch (type)
		{
		case animation_value_type::animation_value_type_bottom_location:
			loc1.bottom = value;
			break;
		case animation_value_type::animation_value_type_left_location:
			loc1.left = value;
			break;
		case animation_value_type::animation_value_type_right_location:
			loc1.right = value;
			break;
		case animation_value_type::animation_value_type_top_location:
			loc1.top = value;
			break;
		}
	}
}

/**
 * Method: TransitionAnimation::SetComponent
 * Purpose: Applies a TBrush to the animation to act upon
 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
 * Returns: void
 *
 * Note: Has no effect as the transtion doesn't directly affect brushes
 */
void TransitionAnimation::SetComponent(TrecPointer<TBrush> comp)
{
}

/**
 * Method: TransitionAnimation::Prepare
 * Purpose: Sets an animation to its starting position before an animation commences
 * Parameters: void
 * Returns: void
 */
void TransitionAnimation::Prepare()
{
	if (done && reverse)
	{
		D2D1_RECT_F temploc = loc1;
		loc1 = loc2;
		loc2 = temploc;
	}
	done = false;
}

/**
 * Method: TransitionAnimation::GetAnimationType
 * Purpose: Reports the String version of the animation type
 * Parameters: void
 * Returns: TString - representing the type this animation object is
 *
 * Note: In this case, the String is L"Simple-Transition"
 */
TString TransitionAnimation::GetAnimationType()
{
	return transitionAnimationString;
}

/**
 * Method: TransitionAnimation::SetControl
 * Purpose: Allows animations to get the control they need to act upon
 * Parameters: TrecPointer<TControl> con -  the control to operate upon
 * Returns: void
 */
void TransitionAnimation::SetControl(TrecPointer<TControl> con)
{
	control = con;
	if (!control.Get())
		return;
	if (phase == animation_phase::animation_phase_end)
	{
		loc1 = control->getLocation();
	}
	else
		loc2 = control->getLocation();
}
