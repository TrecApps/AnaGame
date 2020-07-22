#pragma once
#include <Animation.h>

/**
 * Class: ColorAnimation
 * Purpose: Allows TControls to gradually change their color over a period of time without the user causing every change
 *
 * Note: this animation type is ideal for solid colors. A More Advanced Animation should be used for the Radial/Linear Brushes
 */
class ColorAnimation : public Animation
{
public:
	/**
	 * Method: ColorAnimation::Animation (Constructor)
	 * Purpose: Creates the ColorAnimation with the specified animation-phase setting
	 * Parameters: animation_phase ap - the animation_phase this Animation is set to
	 * Returns: ColorAnimation
	 */
	ColorAnimation(animation_phase ap);
	/**
	 * Method: ColorAnimation::Update
	 * Purpose: Informs the animation regarding what progress a given animation should be along
	 * Parameters: float progress - the progression a between the two colors provided
	 * Returns: bool - true if the resources are in order, false if a resource is missing
	 */
	virtual bool Update(float progress);
	/**
	 * Method: ColorAnimation::SetAnimationValue
	 * Purpose: Sets a value needed to define an animation beginning and end
	 * Parameters: float value - the value of an animation
	 *				animation_value_type type - the meaning behind the value parameter
	 * Returns: void
	 *
	 * Note: This version focuses on Color attributes while ignoring those not related to color
	 */
	virtual void SetAnimationValue(float value, animation_value_type type);

	/**
	 * Method: ColorAnimation::SetComponent
	 * Purpose: Applies a TBrush to the animation to act upon
	 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
	 * Returns: void
	 */
	virtual void SetComponent(TrecPointer<TBrush> comp)override;
	/**
	 * Method: ColorAnimation::SetControl
	 * Purpose: Allows animations to get the control they need to act upon
	 * Parameters: TrecPointer<TControl> con -  the control to operate upon
	 * Returns: void
	 */
	virtual void SetControl(TrecPointer<TControl> con)override;
	/**
	 * Method: ColorAnimation::Prepare
	 * Purpose: Sets an animation to its starting position before an animation commences
	 * Parameters: void
	 * Returns: void
	 */
	virtual void Prepare();
	/**
	 * Method: ColorAnimation::GetAnimationType
	 * Purpose: Reports the String version of the animation type
	 * Parameters: void
	 * Returns: TString - representing the type this animation object is
	 *
	 * Note: In this case, the String is L"Simple-Color"
	 */
	virtual TString GetAnimationType();
protected:
	/**
	 * The start and end colors of the animation
	 */
	D2D1_COLOR_F color1, color2;
	/**
	 * whether the first and second colors are set
	 */
	bool color1Set, color2Set;
	/**
	 * The brush to change the color on
	 */
	TrecPointer<TBrush> brush;
};

