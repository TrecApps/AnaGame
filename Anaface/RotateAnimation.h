#pragma once
#include <Animation.h>

class TControl;

/**
 * Class: RotateAnimation
 * Purpose: allows Controls to spin around
 */
class RotateAnimation :
	public Animation
{
public:
	/**
	 * Method: RotateAnimation::Animation (Constructor)
	 * Purpose: Creates the Rotation Animation with the specified animation-phase setting
	 * Parameters: animation_phase ap - the animation_phase this Animation is set to
	 * Returns: RotateAnimation
	 */
	RotateAnimation(animation_phase);

	/**
	 * Method: RotateAnimation::Update
	 * Purpose: Informs the animation regarding what progress a given animation should be along
	 * Parameters: float progress - the progression of the rotation
	 * Returns: bool - true if the resources are in order, false if a resource is missing
	 */
	virtual bool Update(float progress);
	/**
	 * Method: RotateAnimation::SetAnimationValue
	 * Purpose: Sets a value needed to define an animation beginning and end
	 * Parameters: float value - the value of an animation
	 *				animation_value_type type - the meaning behind the value parameter
	 * Returns: void
	 */
	virtual void SetAnimationValue(float value, animation_value_type type);

	/**
	 * Method: RotateAnimation::SetComponent
	 * Purpose: Applies a TBrush to the animation to act upon
	 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
	 * Returns: void
	 *
	 * Note: Has no affect as Brushes are not directly involved in rotation operations
	 */
	virtual void SetComponent(TrecPointer<TBrush> comp)override;
	/**
	 * Method: RotateAnimation::Prepare
	 * Purpose: Sets an animation to its starting position before an animation commences
	 * Parameters: void
	 * Returns: void
	 *
	 * Note: Abstract as each type can decide what its starting position is
	 */
	virtual void Prepare();
	/**
	 * Method: RotateAnimation::GetAnimationType
	 * Purpose: Reports the String version of the animation type
	 * Parameters: void
	 * Returns: TString - representing the type this animation object is
	 *
	 * Note: In this case, the String is L"Simple-Rotation"
	 */
	virtual TString GetAnimationType();

	/**
	 * Method: RotateAnimation::SetControl
	 * Purpose: Allows animations to get the control they need to act upon
	 * Parameters: TrecPointer<TControl> con -  the control to operate upon
	 * Returns: void
	 */
	virtual void SetControl(TrecPointer<TControl> con)override;
protected:
	/**
	 *cThe control to rotate
	 */
	TrecPointer<TControl> control;
	/**
	 * the angle of the rotation to start
	 */
	float rotateStart, rotateEnd;
	/**
	 * which direction to go
	 */
	bool clockwise;
	/**
	 * whether the values are in degrees (true) or radians (false)
	 */
	bool degrees;

};

