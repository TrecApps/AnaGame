#pragma once
#include <TObject.h>
#include <d2d1.h>
#include "Structure2D.h"


#define t_color D2D1::ColorF

#define color_struct D2D1_COLOR_F

/**
 * Class: TColor
 * Purpose: Anagames base class for color support
 * 
 * SuperClass: TObject
 */
class _VIDEO_GRAPHICS TColor : public TObject
{
public:

	/**
	 * Method: TColor::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TColor::TColor
	 * Purpose: Default constructor
	 * Parameters: void
	 * Returns: New TColor Object, the value set to Black
	 *
	 * Note: the default color is Black. To get a different color, use a different constructor or one of the set methods
	 */
	TColor();

	/**
	 * Method: TColor::TColor
	 * Purpose: Enum constructor
	 * Parameters: t_color::Enum - the Direct2D enumeration of the desired color
	 * Returns: New TColor Object
	 */
	TColor(t_color::Enum);

	/**
	 * Method: TColor::TColor
	 * Purpose: rgb constructor
	 * Parameters: float r - the red value to set to
	 *				float g - the green value to set to
	 *				float b - the blue value to set to
	 * Returns: New TColor Object
	 */
	TColor(float r, float g, float b);

	/**
	 * Method: TColor::TColor
	 * Purpose: rgba constructor
	 * Parameters: float r - the red value to set to
	 *				float g - the green value to set to
	 *				float b - the blue value to set to
	 *				float a - the alpha value to set to
	 * Returns: New TColor Object
	 */
	TColor(float r, float g, float b, float a);

	/**
	 * Method: TColor::TColor
	 * Purpose: Copy Construcotr
	 * Parameters: const TColor& c - the TColor to get the color from
	 * Returns: New TColor Object
	 */
	TColor(const TColor& c);

	/**
	 * Method: TColor::TColor
	 * Purpose: struct constructor
	 * Parameters: const color_struct& color - the structure hoding the desired color
	 * Returns: New TColor Object
	 */
	TColor(const color_struct& color);

	/**
	 * Method: TColor::TColor
	 * Purpose: String Constructor
	 * Parameters: const TString& colorStr - the Color in String form
	 * Returns: new TColor Object
	 */
	TColor(const TString& colorStr);


	/**
	 * Method: TColor::SetColor
	 * Purpose: Sets the color based off of the Enumeration for the color
	 * Parameters: t_color::Enum - the Direct2D provided enum for the color
	 * Returns: void
	 */
	void SetColor(t_color::Enum);


	/**
	 * Method: TColor::SetColor
	 * Purpose: Sets the color based off of the Enumeration for the color
	 * Parameters: const TString& colorStr - the Color in String form
	 * Returns: void
	 */
	void SetColor(const TString& colorStr);

	/**
	 * Method: TColor::SetColor
	 * Purpose: Sets the color based off of specific rgb values
	 * Parameters: float r - the red value to set to
	 *				float g - the green value to set to
	 *				float b - the blue value to set to
	 * Returns: void
	 */
	void SetColor(float r, float g, float b);

	/**
	 * Method: TColor::SetColor
	 * Purpose: Sets the color based off of specific rgba values
	 * Parameters: float r - the red value to set to
	 *				float g - the green value to set to
	 *				float b - the blue value to set to
	 *				float a - the alpha value to set to
	 * Returns: void
	 */
	void SetColor(float r, float g, float b, float a);

	/**
	 * Method: TColor::SetColor
	 * Purpose: Sets this Color to the values held by the Parameter Color
	 * Parameters: const TColor& co - the color Object to borow from
	 * Returns: void
	 */
	void SetColor(const TColor& co);


	/**
	 * Method: TColor::GetRed
	 * Purpose: Retrieves the red value of the color
	 * Parameters: void
	 * Returns: float - the red value of the color
	 * 
	 * Attributes: const
	 */
	float GetRed()const;

	/**
	 * Method: TColor::GetBlue
	 * Purpose: Retrieves the blue value of the color
	 * Parameters: void
	 * Returns: float - the blue value of the color
	 * 
	 * Attributes: const
	 */
	float GetBlue()const;

	/**
	 * Method: TColor::GetGreen
	 * Purpose: Retrieves the green value of the color
	 * Parameters: void
	 * Returns: float - the green value of the color
	 * 
	 * Attributes: const
	 */
	float GetGreen()const;

	/**
	 * Method: TColor::GetOpacity
	 * Purpose: Retrieves the Opacity value of the color
	 * Parameters: void
	 * Returns: float - the opacity of the color
	 * 
	 * Attributes: const
	 */
	float GetOpacity()const;


	/**
	 * Method: TColor::GetColor
	 * Purpose: Returns the color held by the object
	 * Parameters: void
	 * Returns: color_struct - the underlying color structure
	 * 
	 * Attributes: const
	 */
	color_struct GetColor()const;

	/**
	 * Method: TColor::GetColorFromString
	 * Purpose: Returns a Color based off of the provided String
	 * Parameters: const TString& color - string to derive a color out of
	 *				bool& worked - reports whether a color can be derived from the string
	 * Returns: TColor - the color returned
	 * 
	 * Note: If the string cannot be converted to a color, Black is returned and the 'worked' parameter is set to false
	 *
	 * Attributes: static
	 */
	static TColor GetColorFromString(const TString& color, bool& worked);

	/**
	 * Method: TObject::toString
	 * Purpose: Returns a string representation of this object
	 * Parameters: void
	 * Returns: TString - repreetnation of this object
	 *
	 * Attributes: virtual
	 */
	virtual TString toString() override;
private:
	color_struct color;
};

