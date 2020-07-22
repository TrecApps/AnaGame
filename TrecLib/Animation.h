#pragma once
#include "TObject.h"
#include "TrecReference.h"
#include "TControlComponent.h"

/*
 * enum animation_phase
 * Purpose: Provide information on whether a certain animation should be played at the beginning of the
 *	control's lifecycle, end of the lifecycle, throughout the lifecycle, or somewhere in the middle
 *
 * Note: Might be deprecated, depending on how development of the animation API goses, as how
 *	Tap's StoryBoards are handled has a much greater influence on when Animations are played.
 */
typedef enum class animation_phase
{
	animation_pase_beginning,
	animation_phase_end,
	animation_phase_middle,
	animation_phase_persistant
}animation_phase;

/**
 * enum animation_value_type
 * Purpose: Provide Animations setting themselves up with insight on what values they should strive for
 *
 * Note: Each of these values are relevant to certain type of animations and irrelevant to others. For instance,
 *	an Animation focused on changing the color would find the value "animation_value_type_red" to be relevent but
 *	ignore an "animation_value_type_width" setting.
 */
typedef enum class animation_value_type
{
	animation_value_type_width,
	animation_value_type_hight,
	animation_value_type_top_location,
	animation_value_type_left_location,
	animation_value_type_right_location,
	animation_value_type_bottom_location,
	animation_value_type_red,
	animation_value_type_green,
	animation_value_type_blue,
	animation_value_type_alpha,
	animation_value_type_degree,
	animation_value_type_rotate_start,
	animation_value_type_rotate_end,
	animation_value_type_radian,
	animation_value_type_direction,
	animation_value_type_frame,
	animation_value_type_red2,
	animation_value_type_green2,
	animation_value_type_blue2,
	animation_value_type_alpha2,
	animation_value_type_special
}animation_value_type;


// Declare classes relevant to the functioning of Animations.
//
// Since most (if not all) of these classes are defined in higher up libraries,
// We need to declare their existance here. This is fine as Animation is an abstract class
// and is expected to be implemented in the higher-up librries
class TBrush;
class TControl;

/**
 * class Animation (abstract)
 * Purpose: Provide the bare-bones essential information on what any Animation object would need to 
 *	function properly.
 *
 * The main essentials an animation object needs are time-based variables, such as the desired refresh rate, time to complete
 *	an animation sequence, as well as methods to supply the object with the resources the object is to act upon
 *
 *	It was made abstract in order to better allow for a wide variety of Animations to be implemented
 */
class _TREC_LIB_DLL Animation :
	public TObject
{
public:
	/**
	 * Method: Animation::Animation (Constructor)
	 * Purpose: Creates the Animation with the specified animation-phase setting
	 * Parameters: animation_phase ap - the animation_phase this Animation is set to
	 * Returns: Animation
	 */
	Animation(animation_phase ap);

	/**
	 * Method: Animation::GetAnimationPhase
	 * Purpose: Retrieves the animation_phase value associated with this animation
	 * Parameters: void
	 * Returns: animation_phase - the phase associated with this object
	 */
	animation_phase GetAnimationPhase();

	/**
	 * Method: Animation::GetMillisecondRefresh
	 * Purpose: Retrieves the Refreshrate (which is in milliseconds)
	 * Parameters: void
	 * Returns: the Millisecond refresh rate for the animation
	 */
	UINT GetMillisecondRefresh();

	/**
	 * Method: Animation::SetMillisecondRefresh
	 * Purpose: Sets up the milisecond refresh rate
	 * Parameters: UINT mr - the millisecond refresh rate
	 * Returns: void
	 */
	void SetMillisecondRefresh(UINT mr);

	/**
	 * Method: Animation::GetMillisecondLength
	 * Purpose: Reports the number of milliseconds that this animation is expected to run in one round
	 * Parameters: void
	 * Returns: UINT - number of milliseconds that one round of this nimation is expected to play
	 */
	UINT GetMillisecondLength();

	/**
	 * Method: Animation::SetMillisecondLength
	 * Purpose: Sets the time it takes for this specific millisecond to run its course
	 * Parameters: UINT ml - the time this animation should take to complete
	 * Returns: void
	 */
	void SetMillisecondLength(UINT ml);

	/**
	 * Method: Animation::GetMillisecondStoryLength
	 * Purpose: Reports the time length of the Story Board the animaiton is a part of
	 * Parameters: void
	 * Returns: UINT - the time in milliseconds it will take the Story board to complete
	 */
	UINT GetMillisecondStoryLength();

	/**
	 * Method: Animation::SetMillisecindStoryLength
	 * Purpose: Sets the total length of the story board
	 * Parameters: UINT msl - the length of the Story board that the animation is a part of
	 * Returns: void
	 */
	void SetMillisecindStoryLength(UINT msl);

	/**
	 * Method: Animation::SetReverse
	 * Purpose: Determine whether the animation will play in reverse on a second run or not
	 * Parameters: bool rev - whether to play animations in reverse or not
	 * Returns: void
	 */
	void SetReverse(bool rev);

	/**
	 * Method: Animation::GetReverse
	 * Purpose: Reports whether a second run will cause the animation to go in reverse
	 * Parameters: void
	 * Returns: bool - true if even runs cause the animatio to flow the other way, false if susequent runs flow 
	 *	in the same direction.
	 */
	bool GetReverse();

	/**
	 * Method: Animation::Prepare
	 * Purpose: Sets an animation to its starting position before an animation commences
	 * Parameters: void
	 * Returns: void
	 *
	 * Note: Abstract as each type can decide what its starting position is
	 */
	virtual void Prepare() = 0;

	/**
	 * Method: Animation::Update
	 * Purpose: Informs the animation regarding what progress a given animation should be along
	 * Parameters: float progress - the progression a given animation has undergone
	 * Returns: bool - true if the resources are in order, false if a resource is missing
	 *
	 * Note: Abstract as each animation type has different definitions for how it needs to "progress"
	 */
	virtual bool Update(float progress) = 0;

	/**
	 * Method: Animation::SetAnimationValue
	 * Purpose: Sets a value needed to define an animation beginning and end
	 * Parameters: float value - the value of an animation
	 *				animation_value_type type - the meaning behind the value parameter
	 * Returns: void
	 *
	 * Note: Abstract as each subclass decides what to do with the provided parameter values.
	 */
	virtual void SetAnimationValue(float value, animation_value_type type) = 0;


	/**
	 * Method: Animation::SetComponent
	 * Purpose: Applies a TBrush to the animation to act upon
	 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
	 * Returns: void
	 *
	 * Note: Abstract as not every Animation is going to need a brush to work
	 */
	virtual void SetComponent(TrecPointer<TBrush> comp) = 0;

	/**
	 * Method: Animation::SetControl
	 * Purpose: Allows animations to get the control they need to act upon
	 * Parameters: TrecPointer<TControl> con -  the control to operate upon
	 * Returns: void
	 *
	 * Note: Abstract so that each subclass could decide if it needs the control and what to do with it
	 */
	virtual void SetControl(TrecPointer<TControl> con) = 0;


	/**
	 * Method: Animation::GetAnimationType
	 * Purpose: Reports the String version of the animation type
	 * Parameters: void
	 * Returns: TString - representing the type this animation object is
	 *
	 * Note: Abstract so that each subclass can report it's own string
	 */
	virtual TString GetAnimationType() = 0;

protected:
	/**
	 * the Refresh rate of the Animation in milliseconds
	 */
	UINT millisecondRefresh;

	/**
	 * The millisecond time length this animation needs
	 */
	UINT millisecondLength;

	/**
	 * The time it takes for the storyboard to complete
	 */
	UINT millisecondStoryLength;

	/**
	 * the phase that this animation belongs to
	 */
	animation_phase phase;

	/**
	 * done: reports whether the animation has completed 
	 * reverse: whether to play the animation in reverse every other run
	 */
	bool done, reverse;
};

/*
 * Function: GetFloatBetween
 * Purpose: Helper method to calculate the value that represents a certain progression between 
 *	two sets of points. In otherwords, if you wanted to know the progress between 0.0f and 1.0f and wished to 
 *	translate those values into something else, this function will convert some value between 0.0f and 1.0f into
 *  a comparable value between two other values.
 *	For instance, if you had a progression value of 0.5f (half-way) and wished to see what it would be if 100.0f and
 *	300.0f were the star and endpoints, this function would return 200.0f as that is half-way between 100.0f and 300.0f
 * Parameters: float value1 - the lower bound of the progression
 *				float value2 - the upper bound of the progression
 *				float progress - the progress value on the 0.0-1.0 scale to convert
 * Returns: float - the progress value on the $value1-$value2 scale
 */
float _TREC_LIB_DLL GetFloatBetween(float value1, float value2, float progress);