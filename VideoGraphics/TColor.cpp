#include "TColor.h"
#include <TString.h>

const UINT COLOR_MAP_SIZE = 148;

typedef struct ColorMap
{
	const WCHAR* string;
	UINT value;
}ColorMap;

// Colors Provided come from
// https://www.w3schools.com/cssref/css_colors.asp

ColorMap const colorMap[COLOR_MAP_SIZE] = {
	{L"aliceblue", 0x00f8ff},{L"antiquewhite", 0x00faebd7},{L"aqua", 0x0000ffff},
	{L"aquamarine", 0x007fffd4},{L"azure", 0x00f0ffff},{L"beige", 0x00f5f5dc},
	{L"bisque", 0x00ffe4c4},{L"black", 0x00000000},{L"blanchedalmond", 0x00ffebcd},
	{L"blue", 0x000000ff},{L"blueviolet", 0x008a2be2},{L"brown", 0x00a52a2a},
	{L"burlywood", 0x00deb887},{L"caetblue", 0x005f9ea0},{L"chartreuse", 0x007fff00},
	{L"chocolate", 0x00d2691e},{L"coral", 0x00ff7f50},{L"cornflowerblue", 0x006495ed},
	{L"cornsilk", 0x00fff8dc},{L"crimson", 0x00dc143c},{L"cyan", 0x0000ffff},
	{L"darkblue", 0x0000008b},{L"carkcyan", 0x00008b8b},{L"darkgoldenrod", 0x00b8860b},
	{L"darkgray", 0x00a9a9a9},{L"darkgrey", 0x00a9a9a9},{L"darkgreen", 0x00006400},
	{L"darkkhaki", 0x00bdb76b},{L"darkmagenta", 0x008b008b},{L"darkolivegreen", 0x00556b2f},
	{L"darkorange", 0x00ff8c00},{L"darkorchid", 0x009932cc},{L"darkred", 0x008b0000},
	{L"darksalmon", 0x0039967a},{L"darkseagreen", 0x008fbc8f},{L"darkslateblue", 0x00483d8b},
	{L"darkslategray", 0x002f4f4f},{L"darkslategrey", 0x002f4f4f},{L"darkturquoise", 0x0000ced1},
	{L"darkviolet", 0x009400d3},{L"deeppink", 0x00ff1493},{L"deepskyblue", 0x0000bfff},
	{L"dimgray", 0x00696969},{L"dimgrey", 0x00696969},{L"dodgerblue", 0x001e90ff},
	{L"firebrick", 0x00b22222},{L"floralwhite", 0x00fffaf0},{L"forestgreen", 0x00228b22},
	{L"fuchsia", 0x00ff00ff},{L"gainsboro", 0x00dcdcdc},{L"ghostwhite", 0x00f8f8ff},
	{L"gold", 0x00},{L"goldenrod", 0x00daa520},{L"gray", 0x00808080},
	{L"grey", 0x00808080},{L"green", 0x00008000},{L"greenyellow", 0x00adff2f},
	{L"honeydew", 0x00f0fff0},{L"hotpink", 0x00ff69b4},{L"indianred", 0x00cd5c5c},
	{L"indigo", 0x004b0082},{L"ivory", 0x00fffff0},{L"khaki", 0x00f0e68c},
	{L"lavender", 0x00e6e6fa},{L"lavenderblush", 0x00fff0f5},{L"lawngreen", 0x007cfc00},
	{L"lemonchiffon", 0x00fffacd},{L"lightblue", 0x00add8e6},{L"lightcoral", 0x00f08080},
	{L"lightcyan", 0x00e0ffff},{L"lightgoldenrodyellow", 0x00fafad2},{L"lightgray", 0x00d3d3d3},
	{L"lightgrey", 0x00d3d3d3},{L"lightgreen", 0x0090ee90},{L"lightpink", 0x00ffb6c1},
	{L"lightsalmon", 0x00ffa07a},{L"lightseagreen", 0x0020b2aa},{L"lightskyblue", 0x0087cefa},
	{L"lightslategray", 0x00778899},{L"lightslategrey", 0x00778899},{L"lightsteelblue", 0x00b0c4de},
	{L"lightyellow", 0x00ffffe0},{L"lime", 0x0000ff00},{L"limegreen", 0x0032cd32},
	{L"linen", 0x00faf0e6},{L"magenta", 0x00ff00ff},{L"maroon", 0x00800000},
	{L"mediumaquamarine", 0x0066cdaa},{L"mediumblue", 0x000000cd},{L"mediumorchid", 0x00ba55d3},
	{L"mediumpurple", 0x009370db},{L"mediumseagreen", 0x003cb371},{L"mediumslateblue", 0x007b68ee},
	{L"mediumspringgreen", 0x0000fa9a},{L"mediumturquoise", 0x0048d1cc},{L"mediumvioletred", 0x00},
	{L"midnightblue", 0x00191970},{L"mintcream", 0x00f5fffa},{L"mistyrose", 0x00ffe4e1},
	{L"moccasin", 0x00ffe4b5},{L"navajowhite", 0x00ffdead},{L"navy", 0x00000080},
	{L"oldlace", 0x00fdf5e6},{L"olive", 0x00808000},{L"olivedrab", 0x006b8e23},
	{L"orange", 0x00ffa500},{L"orangered", 0x00ff4500},{L"orchid", 0x00da70d6},
	{L"palegoldenrod", 0x00eee8aa},{L"palegreen", 0x0098fb98},{L"paleturquoise", 0x00afeeee},
	{L"palevioletred", 0x00db7093},{L"papayawhip", 0x00ffefd5},{L"peachpuff", 0x00ffdab9},
	{L"peru", 0x00cd853f},{L"pink", 0x00ffc0cb},{L"plum", 0x00dda0dd},
	{L"powderblue", 0x00b0e0e6},{L"purple", 0x00800080},{L"rebeccapurple", 0x00663399},
	{L"red", 0x00ff0000},{L"rosybrown", 0x00bc8f8f},{L"royalblue", 0x004169e1},
	{L"saddlebrown", 0x008b4513},{L"salmon", 0x00fa8072},{L"sandybrown", 0x00f4a460},
	{L"seagreen", 0x002e8b57},{L"seashell", 0x00fff5ee},{L"sienna", 0x00a0522d},
	{L"silver", 0x00c0c0c0},{L"skyblue", 0x0087ceeb},{L"slateblue", 0x006a5acd},
	{L"slategray", 0x00708090},{L"slategrey", 0x00708090},{L"snow", 0x00fffafa},
	{L"springgreen", 0x0000ff7f},{L"steelblue", 0x004682b4},{L"tan", 0x00d2b48c},
	{L"teal", 0x00008080},{L"thistle", 0x00d8bfd8},{L"tomato", 0x00ff6347},
	{L"turquoise", 0x0040e0d0},{L"violet", 0x00ee82ee},{L"wheat", 0x00f5deb3},
	{L"white", 0x00ffffff},{L"whitesmoke", 0x00f5f5f5},{L"yellow", 0x00ffff00},
	{L"yellowgreen", 0x009acd32}
};

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
 * Method: TColor::TColor
 * Purpose: String Constructor
 * Parameters: const TString& colorStr - the Color in String form
 * Returns: new TColor Object
 */
TColor::TColor(const TString& colorStr)
{
	SetColor(colorStr);
}

/**
 * Method: TColor::SetColor
 * Purpose: Sets the color based off of the Enumeration for the color
 * Parameters: t_color::Enum - the Direct2D provided enum for the color
 * Returns: void
 */
void TColor::SetColor(t_color::Enum en)
{
	color = t_color(en);
}


/**
 * Method: TColor::SetColor
 * Purpose: Sets the color based off of the Enumeration for the color
 * Parameters: const TString& colorStr - the Color in String form
 * Returns: void
 */
void TColor::SetColor(const TString& colorStr)
{
	// To-Do - parse the String
	color = t_color(t_color::Black);
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
	color = t_color(r, g, b);
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
	color = t_color(r, g, b, a);
}

/**
 * Method: TColor::SetColor
 * Purpose: Sets this Color to the values held by the Parameter Color
 * Parameters: const TColor& co - the color Object to borow from
 * Returns: void
 */
void TColor::SetColor(const TColor& co)
{
	color = co.GetColor();
}

/**
 * Method: TColor::GetRed
 * Purpose: Retrieves the red value of the color
 * Parameters: void
 * Returns: float - the red value of the color
 */
float TColor::GetRed()const
{
	return color.r;
}

/**
 * Method: TColor::GetBlue
 * Purpose: Retrieves the blue value of the color
 * Parameters: void
 * Returns: float - the blue value of the color
 */
float TColor::GetBlue()const
{
	return color.b;
}

/**
 * Method: TColor::GetGreen
 * Purpose: Retrieves the green value of the color
 * Parameters: void
 * Returns: float - the green value of the color
 */
float TColor::GetGreen()const
{
	return color.g;
}

/**
 * Method: TColor::GetOpacity
 * Purpose: Retrieves the Opacity value of the color
 * Parameters: void
 * Returns: float - the opacity of the color
 */
float TColor::GetOpacity()const
{
	return color.a;
}

/**
 * Method: TColor::GetColor
 * Purpose: Returns the color held by the object
 * Parameters: void
 * Returns: color_struct - the underlying color structure
 */
color_struct TColor::GetColor()const
{
	return color;
}

TColor TColor::GetColorFromString(const TString& color, bool& worked)
{
	TString tempColor(color);
	tempColor.Replace(L" ", L"");
	tempColor.SetLower();

	worked = false;

	for (UINT Rust = 0; Rust < COLOR_MAP_SIZE; Rust++)
	{
		if (!tempColor.Compare(colorMap[Rust].string))
		{
			UINT val = colorMap[Rust].value;
			worked = true;
			float r = static_cast<float>((val & 0x00FF0000) >> 16);
			float g = static_cast<float>((val & 0x0000FF00) >> 8);
			float b = static_cast<float>(val & 0x000000FF);
			return TColor(r / 255.0f, g / 255.0f, b / 255.0f);
		}
	}

	if (tempColor.StartsWith(L"#"))
	{
		UINT hexValue = 0;
		bool w = TString::ConvertStringToUint(tempColor.SubString(1), hexValue, number_base::nb_hexadecimal);
		if (w)
		{
			worked = true;
			byte vals[4];
			vals[0] = (hexValue & 0xff000000) >> 24;
			vals[1] = (hexValue & 0x00ff0000) >> 16;
			vals[2] = (hexValue & 0x0000ff00) >> 8;
			vals[3] = hexValue & 0x000000ff;

			float f0 = static_cast<float>(vals[0]) / 255.0f;
			float f1 = static_cast<float>(vals[1]) / 255.0f;
			float f2 = static_cast<float>(vals[2]) / 255.0f;
			float f3 = static_cast<float>(vals[3]) / 255.0f;
			
			if (vals[0])
			{
				return TColor(f0, f1, f2, f3);
			}
			else
				return TColor(f1, f2, f3);
		}
	}

	if (tempColor.StartsWith(L"rgb(") && tempColor.EndsWith(L")"))
	{
		tempColor.Set(tempColor.SubString(4, tempColor.GetSize() - 1));
		auto colors = tempColor.split(L',');

		if (colors->Size() == 3)
		{
			float rgbf[] = {
				0.0f,0.0f,0.0f
			};
			bool w = true;
			for (UINT Rust = 0; Rust < 3 && w; Rust++)
			{
				colors->at(Rust).Trim();
				if(colors->at(Rust).ConvertToFloat(rgbf[Rust]))
					w = false;
			}
			if (w)
			{
				return TColor(rgbf[0] / 255.0f, rgbf[1] / 255.0f, rgbf[2] / 255.0f);
			}
		}
	}


	return TColor();
}
