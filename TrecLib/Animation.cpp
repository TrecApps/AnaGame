#include "Animation.h"

/**
 * Method: Animation::Animation (Constructor)
 * Purpose: Creates the Animation with the specified animation-phase setting
 * Parameters: animation_phase ap - the animation_phase this Animation is set to
 * Returns: Animation
 */
Animation::Animation(animation_phase ap)
{
	phase = ap;
	millisecondRefresh = millisecondLength = millisecondStoryLength = 0;
	done = false;
	reverse = true;
}

/**
 * Method: Animation::SetReverse
 * Purpose: Determine whether the animation will play in reverse on a second run or not
 * Parameters: bool rev - whether to play animations in reverse or not
 * Returns: void
 */
void Animation::SetReverse(bool rev)
{
	reverse = rev;
}

/**
 * Method: Animation::GetReverse
 * Purpose: Reports whether a second run will cause the animation to go in reverse
 * Parameters: void
 * Returns: bool - true if even runs cause the animatio to flow the other way, false if susequent runs flow
 *	in the same direction.
 */
bool Animation::GetReverse()
{
	return reverse;
}

/**
 * Method: Animation::GetAnimationPhase
 * Purpose: Retrieves the animation_phase value associated with this animation
 * Parameters: void
 * Returns: animation_phase - the phase associated with this object
 */
animation_phase Animation::GetAnimationPhase()
{
	return phase;
}

/**
 * Method: Animation::GetMillisecondRefresh
 * Purpose: Retrieves the Refreshrate (which is in milliseconds)
 * Parameters: void
 * Returns: the Millisecond refresh rate for the animation
 */
UINT Animation::GetMillisecondRefresh()
{
	return millisecondRefresh;
}

/**
 * Method: Animation::SetMillisecondRefresh
 * Purpose: Sets up the milisecond refresh rate
 * Parameters: UINT mr - the millisecond refresh rate
 * Returns: void
 */
void Animation::SetMillisecondRefresh(UINT mr)
{
	millisecondRefresh = mr;
}

/**
 * Method: Animation::GetMillisecondLength
 * Purpose: Reports the number of milliseconds that this animation is expected to run in one round
 * Parameters: void
 * Returns: UINT - number of milliseconds that one round of this nimation is expected to play
 */
UINT Animation::GetMillisecondLength()
{
	return millisecondLength;
}

/**
 * Method: Animation::SetMillisecondLength
 * Purpose: Sets the time it takes for this specific millisecond to run its course
 * Parameters: UINT ml - the time this animation should take to complete
 * Returns: void
 */
void Animation::SetMillisecondLength(UINT ml)
{
	millisecondLength = ml;
}

/**
 * Method: Animation::GetMillisecondStoryLength
 * Purpose: Reports the time length of the Story Board the animaiton is a part of
 * Parameters: void
 * Returns: UINT - the time in milliseconds it will take the Story board to complete
 */
UINT Animation::GetMillisecondStoryLength()
{
	return millisecondStoryLength;
}

/**
 * Method: Animation::SetMillisecindStoryLength
 * Purpose: Sets the total length of the story board
 * Parameters: UINT msl - the length of the Story board that the animation is a part of
 * Returns: void
 */
void Animation::SetMillisecindStoryLength(UINT msl)
{
	millisecondStoryLength = msl;
}

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
float GetFloatBetween(float value1, float value2, float progress)
{
	float diff = value2 - value1;

	return value1 + diff * progress;
}