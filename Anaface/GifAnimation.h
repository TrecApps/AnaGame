#pragma once
#include <Animation.h>
#include <TBitmapBrush.h>

/**
 * Class: GifAnimation
 * Purpose: Allows Gifs to be played in Anagame
 * 
 * SuperClass: Animation
 */
class GifAnimation : public Animation
{
public:

	/**
	 * Method: GifAnimation::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: GifAnimation::Animation
	 * Purpose: Creates the Gif Animation with the specified animation-phase setting
	 * Parameters: animation_phase ap - the animation_phase this Animation is set to
	 * Returns: GifAnimation
	 */
	GifAnimation(animation_phase ap);

	/**
	 * Method: GifAnimation::Prepare
	 * Purpose: Sets an animation to its starting position before an animation commences
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Prepare()override;
	/**
	 * Method: GifAnimation::Update
	 * Purpose: Informs the animation regarding what progress a given animation should be along
	 * Parameters: float progress - the progression a given animation has undergone
	 * Returns: bool - true if the resources are in order, false if a resource is missing
	 *
	 * Note: In this case, all the animation does is call upon the bitmap brush to use the next frame in it's sequence
	 * 
	 * Attributes: override
	 */
	virtual bool Update(float progress)override;
	/**
	 * Method: GifAnimation::SetAnimationValue
	 * Purpose: Sets a value needed to define an animation beginning and end
	 * Parameters: float value - the value of an animation
	 *				animation_value_type type - the meaning behind the value parameter
	 * Returns: void
	 *
	 * Note: Currently has no effect
	 * 
	 * Attributes: override
	 */
	virtual void SetAnimationValue(float value, animation_value_type type)override;

	/**
	 * Method: GifAnimation::SetComponent
	 * Purpose: Applies a TBrush to the animation to act upon
	 * Parameters: TrecPointer<TBrush> comp - the Brush to operate upon
	 * Returns: void
	 *
	 * Note: Attemts to convert the brush into a bitmap brush since it is working with images
	 * 
	 * Attributes: override
	 */
	virtual void SetComponent(TrecPointer<TBrush> comp)override;
	/**
	 * Method: GifAnimation::SetControl
	 * Purpose: Allows animations to get the control they need to act upon
	 * Parameters: TrecPointer<TControl> con -  the control to operate upon
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void SetControl(TrecPointer<TControl> con)override;

	/**
	 * Method: GifAnimation::GetAnimationType
	 * Purpose: Reports the String version of the animation type
	 * Parameters: void
	 * Returns: TString - representing the type this animation object is
	 *
	 * Note: In this case, the String is L"Gif"
	 * 
	 * Attributes: override
	 */
	virtual TString GetAnimationType()override;

protected:
	/**
	 * The Bitmap to operate on
	 */
	TrecSubPointer<TBrush, TBitmapBrush> bitBrush;
	/**
	 * Number of frames in the Gif
	 */
	UINT frameCount;
};

