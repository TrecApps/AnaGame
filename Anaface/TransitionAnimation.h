#pragma once
#include <Animation.h>
#include <d2d1.h>
class TControl;

class TransitionAnimation :
	public Animation
{
public:
	/**
	 * Method: TransitionAnimation::Animation (Constructor)
	 * Purpose: Creates the TransitionAnimation with the specified animation-phase setting
	 * Parameters: animation_phase ap - the animation_phase this Animation is set to
	 * Returns: TransitionAnimation
	 */
	TransitionAnimation(animation_phase);
	/**
	 * Method: TransitionAnimation::Update
	 * Purpose: Informs the animation regarding what progress a given animation should be along
	 * Parameters: float progress - the progression the control has made between departure and destination
	 * Returns: bool - true if the resources are in order, false if a resource is missing
	 */
	virtual bool Update(float progress);
	/**
	 * Method: TransitionAnimation::SetAnimationValue
	 * Purpose: Sets a value needed to define an animation beginning and end
	 * Parameters: float value - the value of an animation
	 *				animation_value_type type - the meaning behind the value parameter
	 * Returns: void
	 *
	 * Note: This one looks t the location of borders and ignores the other types
	 */
	virtual void SetAnimationValue(float value, animation_value_type type);

	/**
	 * Method: TransitionAnimation::SetComponent
	 * Purpose: Applies a TBrush to the animation to act upon
	 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
	 * Returns: void
	 *
	 * Note: Has no effect as the transtion doesn't directly affect brushes
	 */
	virtual void SetComponent(TrecPointer<TBrush> comp)override;
	/**
	 * Method: TransitionAnimation::Prepare
	 * Purpose: Sets an animation to its starting position before an animation commences
	 * Parameters: void
	 * Returns: void
	 */
	virtual void Prepare();
	/**
	 * Method: TransitionAnimation::GetAnimationType
	 * Purpose: Reports the String version of the animation type
	 * Parameters: void
	 * Returns: TString - representing the type this animation object is
	 *
	 * Note: In this case, the String is L"Simple-Transition"
	 */
	virtual TString GetAnimationType();

	/**
	 * Method: TransitionAnimation::SetControl
	 * Purpose: Allows animations to get the control they need to act upon
	 * Parameters: TrecPointer<TControl> con -  the control to operate upon
	 * Returns: void
	 */
	virtual void SetControl(TrecPointer<TControl> con)override;
protected:
	/**
	 * The control to shift around
	 */
	TrecPointer<TControl> control;
	/**
	 * The starting location and ending location
	 */
	D2D1_RECT_F loc1, loc2;
};

