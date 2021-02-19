#include "TGradientStop.h"
#include <TString.h>

/**
 * Method: TGradientStop::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TGradientStop::GetType()
{
	return TString(L"TGradientStop;") + TObject::GetType();
}

/**
 * Method: TGradientStop::TGradientStop
 * Purpose: Attribute Constructor
 * Parameters: const TColor& color - the color of the new gradient stop
 *				float position - position of the grdient stop
 * Returns: New TGradientStop Object
 */
TGradientStop::TGradientStop(const TColor& color, float position)
{
	SetColor(color);
	SetPosition(position);
}

/**
 * Method: TGradientStop::TGradientStop
 * Purpose: Copy Constructor
 * Parameters: const TGradientStop& sto - the object to copy from
 * Returns: New TGradientStop Object
 */
TGradientStop::TGradientStop(const TGradientStop& stop)
{
	gradient = stop.gradient;
}

/**
 * Method: TGradientStop::TGradientStop
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New TGradientStop Object
 */
TGradientStop::TGradientStop()
{
	gradient.position = 0.0f;
	gradient.color = D2D1::ColorF(D2D1::ColorF::Black);
}

/**
 * Method: TGradientStop::SetColor
 * Purpose: Sets the color of the gradient
 * Parameters: const TColor& color - the color to set the grdient to
 * Returns: void
 */
void TGradientStop::SetColor(const TColor& color)
{
	AG_THREAD_LOCK
	gradient.color = color.GetColor();
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TGradientStop::SetPosition
 * Purpose: Sets the position of the gradient
 * Parameters: float position - the position to set the grdient to
 * Returns: void
 */
void TGradientStop::SetPosition(float position)
{
	AG_THREAD_LOCK
	gradient.position = position;
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TGradientStop::SetGradient
 * Purpose: Sets the Gradient of this object by structure
 * Parameters: const GRADIENT_STOP_2D& stop -  the gradient to set to
 * Returns: void
 */
void TGradientStop::SetGradient(const GRADIENT_STOP_2D& stop)
{
	AG_THREAD_LOCK
	gradient = stop;
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TGradientStop::GetColor
 * Purpose:  retrieves the color of the gradient
 * Parameters: void
 * Returns: TColor - the color of the gradient
 */
TColor TGradientStop::GetColor()const
{
	AG_THREAD_LOCK

		auto ret =  TColor(gradient.color);
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TGradientStop::GetPosition
 * Purpose: Retrieves the position of the gradient
 * Parameters: void
 * Returns: float - the position of the gradient
 */
float TGradientStop::GetPosition()const
{
	AG_THREAD_LOCK
		auto ret = gradient.position;
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TGradientStop::GetGradient
 * Purpose: Retrieves the underlying gradient
 * Parameters: void
 * Returns: GRADIENT_STOP_2D - the underlying gradient
 */
GRADIENT_STOP_2D TGradientStop::GetGradient() const
{
	AG_THREAD_LOCK
		auto ret = gradient;
	RETURN_THREAD_UNLOCK ret;
}
