#include "TColor.h"
#include <TString.h>


/**
 * Method: TColor::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TColor::GetType()
{
	return TString(L"TColor;") + TObject::GetType();
}

/**
 * Method: TColor::TColor
 * Purpose: Default constructor
 * Parameters: void
 * Returns: New TColor Object, the value set to Black
 *
 * Note: the default color is Black. To get a different color, use a different constructor or one of the set methods
 */
TColor::TColor()
{
	SetColor(t_color::Black);
}

/**
 * Method: TColor::TColor
 * Purpose: Enum constructor
 * Parameters: t_color::Enum - the Direct2D enumeration of the desired color
 * Returns: New TColor Object
 */
TColor::TColor(t_color::Enum en)
{
	SetColor(en);
}

/**
 * Method: TColor::TColor
 * Purpose: rgb constructor
 * Parameters: float r - the red value to set to
 *				float g - the green value to set to
 *				float b - the blue value to set to
 * Returns: New TColor Object
 */
TColor::TColor(float r, float g, float b)
{
	SetColor(r, g, b);
}

/**
 * Method: TColor::TColor
 * Purpose: rgba constructor
 * Parameters: float r - the red value to set to
 *				float g - the green value to set to
 *				float b - the blue value to set to
 *				float a - the alpha value to set to
 * Returns: New TColor Object
 */
TColor::TColor(float r, float g, float b, float a)
{
	SetColor(r, g, b, a);
}

/**
 * Method: TColor::TColor
 * Purpose: Copy Construcotr
 * Parameters: const TColor& c - the TColor to get the color from
 * Returns: New TColor Object
 */
TColor::TColor(const TColor& c)
{
	SetColor(c);
}

/**
 * Method: TColor::TColor
 * Purpose: struct constructor
 * Parameters: const color_struct& color - the structure hoding the desired color
 * Returns: New TColor Object
 */
TColor::TColor(const color_struct& color)
{
	this->color = color;
}

/**
 * Method: TColor::SetColor
 * Purpose: Sets the color based off of the Enumeration for the color
 * Parameters: t_color::Enum - the Direct2D provided enum for the color
 * Returns: void
 */
void TColor::SetColor(t_color::Enum en)
{
	ThreadLock();
	color = t_color(en);
	ThreadRelease();
}

/**
 * Method: TColor::SetColor
 * Purpose: Sets the color based off of specific rgb values
 * Parameters: float r - the red value to set to
 *				float g - the green value to set to
 *				float b - the blue value to set to
 * Returns: void
 */
void TColor::SetColor(float r, float g, float b)
{
	ThreadLock();
	color = t_color(r, g, b);
	ThreadRelease();
}

/**
 * Method: TColor::SetColor
 * Purpose: Sets the color based off of specific rgba values
 * Parameters: float r - the red value to set to
 *				float g - the green value to set to
 *				float b - the blue value to set to
 *				float a - the alpha value to set to
 * Returns: void
 */
void TColor::SetColor(float r, float g, float b, float a)
{
	ThreadLock();
	color = t_color(r, g, b, a);
	ThreadRelease();
}

/**
 * Method: TColor::SetColor
 * Purpose: Sets this Color to the values held by the Parameter Color
 * Parameters: const TColor& co - the color Object to borow from
 * Returns: void
 */
void TColor::SetColor(const TColor& co)
{
	ThreadLock();
	color = co.GetColor();
	ThreadRelease();
}

/**
 * Method: TColor::GetRed
 * Purpose: Retrieves the red value of the color
 * Parameters: void
 * Returns: float - the red value of the color
 */
float TColor::GetRed()const
{
	ThreadLock();
	float ret = color.r;
	ThreadRelease();
	return ret;
}

/**
 * Method: TColor::GetBlue
 * Purpose: Retrieves the blue value of the color
 * Parameters: void
 * Returns: float - the blue value of the color
 */
float TColor::GetBlue()const
{
	ThreadLock();

	float ret = color.b;
	ThreadRelease();
	return ret;
}

/**
 * Method: TColor::GetGreen
 * Purpose: Retrieves the green value of the color
 * Parameters: void
 * Returns: float - the green value of the color
 */
float TColor::GetGreen()const
{
	ThreadLock();

	float ret = color.g;
	ThreadRelease();
	return ret;
}

/**
 * Method: TColor::GetOpacity
 * Purpose: Retrieves the Opacity value of the color
 * Parameters: void
 * Returns: float - the opacity of the color
 */
float TColor::GetOpacity()const
{
	ThreadLock();
	float ret = color.a;
	ThreadRelease();
	return ret;
}

/**
 * Method: TColor::GetColor
 * Purpose: Returns the color held by the object
 * Parameters: void
 * Returns: color_struct - the underlying color structure
 */
color_struct TColor::GetColor()const
{
	ThreadLock();
	auto ret = color;
	ThreadRelease();
	return ret;
}
