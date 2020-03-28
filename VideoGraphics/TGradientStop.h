#pragma once
#include <TObject.h>
#include "TColor.h"
#include "Structure2D.h"
class _VIDEO_GRAPHICS TGradientStop : public TObject
{
public:

	/**
	 * Mehtod: TGradientStop::TGradientStop
	 * Purpose: Attribute Constructor
	 * Parameters: const TColor& color - the color of the new gradient stop
	 *				float position - position of the grdient stop
	 * Returns: New TGradientStop Object
	 */
	TGradientStop(const TColor& color, float position);

	/**
	 * Mehtod: TGradientStop::TGradientStop
	 * Purpose: Copy Constructor
	 * Parameters: const TGradientStop& sto - the object to copy from
	 * Returns: New TGradientStop Object
	 */
	TGradientStop(const TGradientStop& stop);

	/**
	 * Mehtod: TGradientStop::TGradientStop
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New TGradientStop Object
	 */
	TGradientStop();


	/**
	 * Mehtod: TGradientStop::SetColor
	 * Purpose: Sets the color of the gradient
	 * Parameters: const TColor& color - the color to set the grdient to
	 * Returns: void
	 */
	void SetColor(const TColor& color);

	/**
	 * Mehtod: TGradientStop::SetPosition
	 * Purpose: Sets the position of the gradient
	 * Parameters: float position - the position to set the grdient to
	 * Returns: void
	 */
	void SetPosition(float position);

	/**
	 * Mehtod: TGradientStop::SetGradient
	 * Purpose: Sets the Gradient of this object by structure
	 * Parameters: const GRADIENT_STOP_2D& stop -  the gradient to set to
	 * Returns: void
	 */
	void SetGradient(const GRADIENT_STOP_2D& stop);


	/**
	 * Mehtod: TGradientStop::GetColor
	 * Purpose:  retrieves the color of the gradient
	 * Parameters: void
	 * Returns: TColor - the color of the gradient
	 */
	TColor GetColor()const;

	/**
	 * Mehtod: TGradientStop::GetPosition
	 * Purpose: Retrieves the position of the gradient
	 * Parameters: void
	 * Returns: float - the position of the gradient
	 */
	float GetPosition()const;

	/**
	 * Mehtod: TGradientStop::GetGradient
	 * Purpose: Retrieves the underlying gradient
	 * Parameters: void
	 * Returns: GRADIENT_STOP_2D - the underlying gradient
	 */ 
	GRADIENT_STOP_2D GetGradient()const;
protected:
	/**
	 * the actual gradient
	 */
	GRADIENT_STOP_2D gradient;
};

