#pragma once

#include <d2d1.h>					// Direct 2D support
#include <d2d1_1.h>					// Help with the D2D1Context Device if necessary
#include <dwrite.h>					// Direct Write
#include <TString.h>				// Use AnaGame's TString for simple conversion to numbers
#include <wincodec.h>				
#include "Anaface.h"				
#include "ControlTypeSafety.h"
#include "TScrollBar.h"
#include "EventTarget.h"
#include <TrecReference.h>
#include <TDataArray.h>
#include <TFile.h>
#include <TPoint.h>

#include <DrawingBoard.h>
#include <TBitmapBrush.h>
#include <TGradientStopCollection.h>

#include <TWindowEngine.h>


#include <TControlComponent.h>
#include "TParentHolder.h"

//#define _TREC_LIB_DLL __declspec(dllimport)
#include <TMap.h>
#include <TObjectNode.h>

#define RADIAN_DEGREE_RATIO 57.2957795

#define afx_msg

class TControl;
class TFlyout;
class TContextMenu;
//using namespace ControlTypeSafety;

// Declare existance of classes so that they can be attributes
// Only implementations need class definitions


#define _WRITE_THE_STRING 		ar->WriteString(appendable);      \
	ar->WriteString(L"\n");                  \
	resetAttributeString(&appendable, childLevel + 1);

/**
 * Enum Class: messageState
 * Purpose: Used by the control to determine which to draw
 */
typedef enum class  messageState
{
	normal,
	mouseHover,
	mouseLClick,
	mouseRClick
} messageState;


/**
 * Enum Class: R_Message_Type
 * Purpose: Determines the message type for UI Responsiveness
 */
typedef enum class R_Message_Type
{
	On_L_Button_Down,
	On_L_Button_Up,
	On_R_Button_Down,
	On_R_Button_Up,
	On_Click,
	On_Hold_Click,
	On_Hover,
	On_Hover_Leave,
	On_Right_Click,
	On_Click_Release,
	On_Text_Change,
	On_Right_Release,
	On_sel_change,
	On_check,
	On_radio_change,
	On_Char,
	On_Focus,
	On_Lose_Focus,
	On_Select_Scroller,
	On_Flyout,
	On_LDoubleClick
}R_Message_Type;

/**
 * Struct: EventArgs
 * Purpose: Event Arguements, used to supply Event Handlers with the necessary information to 
 *		do their job properly
 */
typedef struct EventArgs
{
	TString text;
	bool positive;
	TPoint point;
	bool isClick;
	bool isLeftClick;
	R_Message_Type eventType;
	TControl* control;
	int methodID;
	int arrayLabel;
	WCHAR type;
	TrecPointer<TObjectNode> object;
} EventArgs;

/**
 * Class: EventID_Cred
 * Purpose: Used by controls to register themselves in the message queue if they have a message handler
 */
class _ANAFACE_DLL EventID_Cred
{
public:
	EventID_Cred();
	EventID_Cred(const EventID_Cred& copy);
	EventID_Cred(R_Message_Type t, TrecPointer<TControl> c);
	EventID_Cred(R_Message_Type t, TrecPointer<TControl> c, TrecPointer<TScrollBar> sb);
	EventID_Cred(TrecPointer<TFlyout> fly);

	R_Message_Type eventType;
	TrecPointer<TControl> control;
	TrecPointer<TScrollBar> scroll;
	TrecPointer<TFlyout> flyout;
};

/**
 * Struct: EventTypeId
 * Purpose: Stored by Controls for checking whether or not they actually have a message handler
 */
typedef struct EventTypeID
{
	R_Message_Type eventType;
	int eventID;
}EventTypeID;

/**
 * Struct: sizeControl
 * Purpose: Controls the size of a Given TControl
 */
typedef struct sizeControl
{
	int width;
	int height;
	int minHeight;
	int minWidth;
	int maxWidth;
	int maxHeight;
}sizeControl;

/**
 * Enum Class: TShape
 * Purpose: Determines the basic size of the TControl
 * NOTE: Feature is unstable, stick with T_Rect for now
 */
typedef enum class TShape {
	T_Rect,
	T_Rounded_Rect,
	T_Ellipse,
	T_Custom_shape
}TShape;

/**
 * Enum Class: BrushMarker
 * Purpose: Allows TControls to draw with special styles if specified in the ML
*/
typedef enum class BrushMarker
{
	BM_SolidBrush,
	BM_RadientBrush,
	BM_LinearBrush
}BrushMarker;

/**
 * Class: AnimationData
 * Purpose: Basic ID of the Animation/StoryBoard to create and which control and/or brush to apply it to
 */
class AnimationData
{
public:
	/**
	 * Identifies the Animation to build and the StoryBoard to apply the animation to
	 */
	TString name, storyName;
	/**
	 * The Control to apply the Animation to
	 */
	TrecPointerSoft<TControl> control;
	/**
	 * the Brush to apply the Animation to (if applicable)
	 */
	TrecPointer<TBrush> brush;
};


/*
 * Class TBorder
 * Purpose: Draws the Border of a Given TControl
 */
class _ANAFACE_DLL TBorder : public TControlComponent
{
	friend class TControl;
	friend class TBorder;
public:

	/*
	* Method:  TBorder::TBorder
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render target to draw against
	*				TControl* tc - the TControl to which the TBorder is a party to
	* Returns: New TBorder Object
	*/
	TBorder(TrecPointer<DrawingBoard>,TControl*);
	/*
	* Method: TBorder:TBorder
	* Purpose: Constructor
	* Parameters: TrecPointer<TBorder> & rBord - Reference to the Border to copy
	*				TControl* tc_holder - the Control to work for
	* Returns: New TBorder Object
	*/
	TBorder(TrecPointer<TBorder>&,TControl*);
	/*
	* Method: TBorder::TBorder
	* Purpose: Blank Constructor
	* Parameters: void
	* Returns: Blank TBorder Object
	*/
	TBorder();
	/*
	* Method: TBorder::~TBorder
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TBorder();

	// Various On Create Methods depending on the shape of the control

	/*
	* Method: TBorder::onCreate
	* Purpose: Sets up a basic Rectangular border
	* Parameters: RECT location - the location the Border should draw on
	* Returns: bool - success
	*/
	bool onCreate(D2D1_RECT_F);
	/*
	* Method: TBorder::onCreate
	* Purpose: Sets up a border optimized for dawing ovals
	* Parameters: D2D1_ELLIPSE e - the oval to draw
	* Returns: bool - success
	*/
	bool onCreate(D2D1_ELLIPSE);
	/*
	* Method: TBorder::onCreate
	* Purpose: Sets up a Border that has a rectangle with rounded corners
	* Parameters: D2D1_ROUNDED_RECT rr - the border to draw
	* Returns: bool - success
	*/
	bool onCreate(D2D1_ROUNDED_RECT);

	// Drawing Method

	/*
	* Method: TBorder::onDraw
	* Purpose: Draws the border
	* Parameters: void
	* Returns: void
	*/
	void onDraw(D2D1_RECT_F& loc);

	// Handling Colors
	// NOTE: For Compatibility between AnaGame for Windows and an eventual AnaGame for Unix,
	//	Parameters and Return values should be replaced with a cross-platform structure 


	// Methods for handling Opaquency. Note, there is no getOpaquency() method as it is handled by the getColored method 

	/*
	 * Method: TBorder::setThickness
	 * Purpose: Allows the thickness to be updated after creation
	 * Parameters: float f - the thickness to set to
	 * Returns: void
	 */
	void setThickness(float f);
	/*
	* Method: TBorder::setOpaquency
	* Purpose: Allows the Opaquency to be updated after creation
	* Parameters: float f - the opaquency to set to (no upadte occurs if value is out of range)
	* Returns: void
	*/
	void setOpaquency(float f);
	/*
	* Method: TBorder::getThickness
	* Purpose: Allows the current thickness to be seen
	* Parameters: void
	* Returns: float - the current thickness
	*/
	float getThickness();

	// Getting the Location

	/*
	 * Method: TBorder::getLocation
	 * Purpose: Retrieves the Location of the current border
	 * Parameters: void
	 * Returns: RECT - the current location
	 */
	RECT getLocation();
	/*
	* Method: TBorder::ShiftHorizontal
	* Purpose: Moves the Border left-or right, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the Border and which direction
	* Returns: void
	*/
	void ShiftHorizontal(int degrees);
	/*
	* Method: TBorder::ShiftVertical
	* Purpose:Moves the Border up or down, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the border and which direction
	* Returns: void
	*/
	void ShiftVertical(int degrees);

	// Inherited From TControlComponent

	/*
	 * Method: TBorder::SetColor
	 * Purpose: Allows the color of the border to change even after creation
	 * Parameters: D2D1_COLOR_F& cf -  the color to set the border to
	 * Returns: void
	 */
	virtual void SetColor(const D2D1_COLOR_F& color)override;
	virtual D2D1_COLOR_F GetColor() override;
	virtual void SetColor2(const D2D1_COLOR_F& color) override;
	virtual D2D1_COLOR_F GetColor2() override;
	virtual D2D1_RECT_F GetLocation() override;
	virtual void SetLocation(const D2D1_RECT_F& loc) override;

	/*
	 * Method: TBorder::ResetBrush
	 * Purpose: Allows the border to reset the brush when one of it's attributes changes
	 * Parameters: void
	 * Returns: void
	 */
	TrecPointer<TBrush> GetBrush();


private:

	/**
	 * The Brush to Paint with
	 */
	TrecPointer<TBrush> brush;

	void ResetBrush();

	/**
	 * Used by the Builder to Highlight Controls under editing (not sure if it is still useful?)
	 */
	TrecPointer <TBrush> BuilderFocusBrush;
	/**
	 * thickness of the border
	 */
	float thickness = 1.0f;
	/**
	 * Holds a special style, NOTE: Feature not Implemented
	 */
	TString style;

	/**
	 * Pointer to the DrawingBoard to Draw to
	 */
	TrecPointer<DrawingBoard> drawingBoard;											// 
	/**
	 *
	 */
	TrecPointer<TBitmapBrush> bitBrush;										// In the event that an image is part of the Border
											// the Image of the above structure


	/*
	 * Method: TBorder::storeInTML
	 * Purpose: Allows te Border to save itself in the Anaface TML format
	 * Parameters: CArchive * ar - the File object to save to
	 *				int childLevel - the child level (in TML this lets the Border know how many dashes to write for each attribute)
	 *				messageState ms - which Border the current Border is (the Border itself does not know)
	 * Returns: int
	*/
	int storeInTML(TFile* ar, int childLevel,messageState);


	/**
	 * The TControl that owns the Border
	 */
	TControl* cap;

	/**
	 * Shape to Draw
	 */
	TShape shape;

	/**
	 * Holds color, used for creating the brush
	 */
	TGradientStopCollection stopCollection;

	/**
	 * whether we are creating a radial or gradient brush (ignored if only one color is given)
	 */
	bool useRadial;

	// Alternate shapes


	/**
	 * the rect to pass into the Border and content if used
	 */
	D2D1_RECT_F loci;

	/**
	 * the ellipse to pass into the Border and content if used
	 */
	D2D1_ELLIPSE circle;

	/**
	 * the rounded rect to pass into the Border and content if used
	 */
	D2D1_ROUNDED_RECT roundedRect;

	/*
	* Method: TBorder::BreakShared
	* Purpose: Breaks the link between the Border and the Parent Control, allowing deletion
	* Parameters: void
	* Returns: void
	* Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
	*/
	void BreakShared();
};

/*
 * Class TText
 * Purpose: Handles the Text in a TControl. Note, Normally the text would be static, but extensions of the 
 *	TControl Class might add additional dynamic functionality using this structure
 */
class _ANAFACE_DLL TText :public TControlComponent
{
	friend class TText;
	friend class TControl;
	friend class TTextField;
	friend class TPromptControl;
	friend class TCheckBox;
	friend class TRadioButton;
//	friend class TComboBox;
	friend class TContextMenu;
	friend class TTabBar;
public:

	/*
	* Method: TText::TText
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render Target to draw to
	*				TControl* tc - the TControl to work for
	* Returns: New TText Object
	*/
	TText(TrecPointer<DrawingBoard> drawingBoard, TControl*);
	/*
	* Method: TText::TText
	* Purpose: Close to a Copy Constructor
	* Parameters: TrecPointer<TText> & rText -  the text element to copy
	*				TControl* tc_holder - The TControl to work for
	* Returns: Copied TText
	*/
	TText(TrecPointer<TText>&,TControl*);
	/*
	* Method: TText::TText
	* Purpose: Default Constructor
	* Parameters: void
	* Returns: Blank TText
	*/
	TText();
	/*
	* Method: TText::~TText
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TText();

	// Creation Method, only a rect is needed as the text doesn't care for other shapes
	/*
	 * Method: TText::onCreate
	 * Purpose: Sets up the Text element
	 * Parameters: RECT loc - the screen space to use
	 * Returns: int success (0 means no error)
	 */
	int onCreate(D2D1_RECT_F);

	// Resets the layout in the event one of it's underlying attributes changes

	/*
	 * Method: TText::reCreateLayout
	 * Purpose: Resets the Text Layout upon changes made to the format, text, or location
	 * Parameters: void
	 * Returns: void
	 * Note: Call this method whenever you make a change to the format of the control, the text, or the location,
	 *		Otherwise, your changes might not be seen
	 */
	void reCreateLayout();
	/*
	 * Method: TText::reCreateLayout
	 * Purpose: Resets the Text Layout upon changes made to the format, text, or location
	 * Parameters: TString & str - the Text to change the format object to
	 * Returns: void
	 * Note: Call this method whenever you make a change to the format of the control, the text, or the location,
	 *		Otherwise, your changes might not be seen
	 */
	void reCreateLayout(TString& str);

	// Draw the Text
	/*
	 * Method: TText::onDraw
	 * Purpose: Draws the text
	 * Parameters: RECT r - the area to draw
	 * Returns: bool success
	 */
	bool onDraw(D2D1_RECT_F& loc, TObject* obj = nullptr);

	// Handling Colors
	// NOTE: For Compatibility between AnaGame for Windows and an eventual AnaGame for Unix,
	//	Parameters and Return values should be replaced with a cross-platform structure 

	// Setter methods
	/*
	* Method: TText::setNewFont
	* Purpose: Sets up the new font for the text
	* Parameters: TString& pFont - String holding the new fnt to use
	* Returns: bool - true (redundant return value)
	*/
	bool setNewFont(TString&);

	/*
	* Method: TText::setNewFontSize
	* Purpose: Updates the size of the font to print
	* Parameters: float fs - the new font size
	* Returns: void
	*/
	void setNewFontSize(float fs);
	/*
	* Method: TText::setNewHorizontalAlignment
	* Purpose: Updates the Horizontal Alignment of the text element
	* Parameters: DWRITE_TEXT_ALIGNMENT ha - the new horizontal alignment
	* Returns: void
	*/
	void setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT ha);
	/*
	* Method: TText::setNewVerticalAlignment
	* Purpose: Updates the Vertical Alignment of the Text element
	* Parameters: DWRITE_PARAGRAPH_ALIGNMENT va - the new vertical alignment
	* Returns: void
	*/
	void setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT va);
	/*
	* Method: TText::setOpaquency
	* Purpose: Updates the opaquency of the text
	* Parameters: float o - the new opequency
	* Returns: bool - true if data is valid, false otherwise
	*/
	bool setOpaquency(float o);
	void setNewLocation(RECT r);

	// Get Methods

	/*
	 * Method: TText::getFontSize
	 * Purpose: Retrieves the current font size of the text
	 * Parameters: void
	 * Returns: float - the current font size
	 */
	float getFontSize();
	/*
	* Method: TText::getFont
	* Purpose: Retrieves the current font of the text
	* Parameters: void
	* Returns: TString - the current font
	*/
	TString getFont();
	/*
	* Method: TText::getCaption
	* Purpose: Retrieves the new caption for the text element
	* Parameters: void
	* Returns: TString - the current caption
	*/
	TString getCaption();

	/*
	 * Method: TText::GetMinWidth
	 * Purpose: Retirvees the minimum width needed before DirectWrtie has to add emergency breaks in line
	 * Parameters: bool& worked - whether the value returned is truely the reported value
	 * Return: float - the min width needed. If inspection fails, this represents the width currently used
	 */
	float GetMinWidth(bool& worked);

	// More Set Methods

	/*
	 * Method: TText::setCaption
	 * Purpose: Updates the text with a new caption
	 * Parameters: TString& string - the new Caption to use
	 * Returns: void
	 */
	void setCaption(const TString& string);

	/*
	* Method: TText::setLocale
	* Purpose: Sets a new Local for the text
	* Parameters: TString& loc - the String holding the new locale
	* Returns: void
	*/
	void setLocale(TString& loc);

	/*
	* Method: TText::setFont
	* Purpose: Sets up the new font for the text
	* Parameters: TString& fo - String holding the new fnt to use
	* Returns: bool - true (redundant return value)
	* Note: DEPRECIATED - us the method TText::setNewFont
	*/
	void setFont(TString& f);

	// Remove the Text completely

	/*
	 * Method: TText::removeCaption
	 * Purpose: Removes the Text and sets it to zero
	 * Parameters: void
	 * Returns: void
	 */
	void removeCaption();

	/*
	* Method: TText::getHorizontalAlignment
	* Purpose: Gets the Horizontal alignment of the text
	* Parameters: void
	* Returns: DWRITE_TEXT_ALIGNMENT - the Horizontal alignment
	*/
	DWRITE_TEXT_ALIGNMENT getHorizontalAlignment();
	/*
	* Method: TText::getVerticalAlignment
	* Purpose: Gets the Vertical alignment of the text
	* Parameters: void
	* Returns: DWRITE_PARAGRAPH_ALIGNMENT - the vetical alignment if the text
	*/
	DWRITE_PARAGRAPH_ALIGNMENT getVerticalAlignment();
	/*
	* Method: TText::getLength
	* Purpose: Retrieves the current length of the text being held
	* Parameters: void
	* Returns: int - the length of the text
	*/
	int getLength();
	/*
	* Method: TText::getLocation
	* Purpose: Retrieves the Location of the Text Control
	* Parameters: void
	* Returns: RECT - the location of the text
	*/
	RECT getLocation();
	/*
	* Method: TText::ShiftHorizontal
	* Purpose: Moves the Text element left-or right, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the text and which direction
	* Returns: void
	*/
	void ShiftHorizontal(int degrees);
	/*
	* Method: TText::ShiftVertical
	* Purpose:Moves the Text Element up or down, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the text and which direction
	* Returns: void
	*/
	void ShiftVertical(int degrees);
	

	// Inherited From TControlComponent
	virtual void SetColor(const D2D1_COLOR_F& color)override;
	virtual D2D1_COLOR_F GetColor() override;
	virtual void SetColor2(const D2D1_COLOR_F& color) override;
	virtual D2D1_COLOR_F GetColor2() override;
	virtual D2D1_RECT_F GetLocation() override;
	virtual void SetLocation(const D2D1_RECT_F& loc) override;

	TrecPointer<TBrush> GetBrush();

private:

	/**
	 * Drawing Borad to draw with
	 */
	TrecPointer<DrawingBoard> drawingBoard;
	/**
	 * Direct Write Factory
	 */
	TrecComPointer<IDWriteFactory> writeFact;

	/*
	 * Method: TText::ResetBrush
	 * Purpose: Allows the Text to reset the brush when one of it's attributes changes
	 * Parameters: void
	 * Returns: void
	 */
	void ResetBrush();
	

	/**
	 * Location of the text
	 */
	D2D1_RECT_F bounds;
	/**
	 * Caption of the text object
	 */
	TString text;
	/**
	 * length of the string (redundant, as text would also have this value and would be more reliable)
	 */
	UINT32 textLength;
	/**
	 * Brush to write text with
	 */
	TrecPointer<TBrush> penBrush;


	/**
	 * font weight
	 */
	DWRITE_FONT_WEIGHT fontWeight;
	/**
	 * font style
	 */
	DWRITE_FONT_STYLE fontStyle;
	/**
	 * ho the font stretches
	 */
	DWRITE_FONT_STRETCH fontStretch;

	/**
	 * Vertical alignment of the text (top, bottom, or center)
	 */
	DWRITE_PARAGRAPH_ALIGNMENT verticalAlignment;
	/**
	 * The horizontal alignment of the text (left, right, center or justified)
	 */
	DWRITE_TEXT_ALIGNMENT horizontalAlignment;

	/**
	 * size of the font
	 */
	float fontSize = 12;
	/**
	 * the locale of the text
	 */
	TString locale;
	/**
	 * the font of the text to use
	 */
	TString font;

	/*
	* Method: TText::storeInTML
	* Purpose: Allows the text to store it's attributes to a TML file
	* Parameters: CArchive* ar - the file system to write to
	*				int childLevel - the level this control is - how many dashes to write
	*				messageState ms - the version of the text it is (TText doesn't know if it is normal, hover, or click)
	* Returns: int - 0
	*/
	int storeInTML(TFile* ar, int childLevel,messageState);

	/**
	 * The TControl that owns the Border
	 */
	TControl* cap;

	/**
	 * Holds color, used for creating the brush
	 */
	TGradientStopCollection stopCollection;
	/**
	 * whether we are creating a radial or gradient brush (ignored if only one color is given)
	 */
	bool useRadial;

	// Determine how to draw text

	/**
	 * the layout object to write text with
	 */
	TrecComPointer<IDWriteTextLayout> fontLayout;
	/**
	 * the format object to write text with
	 */
	TrecComPointer<IDWriteTextFormat> format;


	/*
	 * Method: TText::BreakShared
	 * Purpose: Breaks the link between the Text Element and the Parent Control, allowing deletion
	 * Parameters: void
	 * Returns: void
	 * Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
	 */
	void BreakShared();
};


class _ANAFACE_DLL TContent :public TControlComponent
{
	friend class TControl;
	friend class TGadgetControl;
	friend class TTabBar;
public:
	/*
	* Method: TContent::TContent
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render Target to draw to
	*				TControl* tc - the parent Control to work for
	* Returns: New TContent
	*/
	TContent(TrecPointer<DrawingBoard>, TControl*);
	/*
	* Method: TContent::TContent
	* Purpose: Default Constructor
	* Parameters: void
	* Returns: void
	*/
	TContent();
	/*
	* Method: TContent::TContent
	* Purpose: Constructor
	* Parameters: TrecPointer<TContent> &rCont - an exiting Content to copy from
	*				TControl* tc_holder - the Control to work for
	* Returns: New TContent object
	*/
	TContent(TrecPointer<TContent>&, TControl*);
	/*
	* Method: TContent::~TContent
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TContent();

	/*
	* Method: TContent::onCreate
	* Purpose: Sets up the details of the Content with a Rectangle to draw
	* Parameters: RECT l - the Location
	*				RECT s - the snip, where the control can appear
	* Returns: bool - success (whether the resources are prepared)
	*/
	bool onCreate(D2D1_RECT_F);
	/*
	* Method: TContent::onCreate
	* Purpose: Sets up the details of the Content with an oval to draw
	* Parameters: D2D1_ELLIPSE e - the oval to draw
	* Returns: bool - success (whether the resources are prepared)
	*/
	bool onCreate(D2D1_ELLIPSE);
	/*
	* Method: TContent::onCreate
	* Purpose: Sets up the details of the Content with a rounded Rectangle to draw
	* Parameters: D2D1_ROUNDED_RECT rr -  the rounded rectangle to draw
	* Returns: bool - success (whether the resources are prepared)
	*/
	bool onCreate(D2D1_ROUNDED_RECT);
	/*
	* Method: TContent::ShiftHorizontal
	* Purpose: Moves the content left-or right, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the content and which direction
	* Returns: void
	*/
	void ShiftHorizontal(int degrees);
	/*
	* Method: TContent::ShiftVertical
	* Purpose:Moves the content up or down, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the content and which direction
	* Returns: void
	*/
	void ShiftVertical(int degrees);
	/*
	* Method: TContent::onDraw
	* Purpose: Draws the content, including a picture if one is provided
	* Parameters: void
	* Returns: void
	*/
	void onDraw(D2D1_RECT_F& loc);

	/*
	* Method: TContent::setOpaquency
	* Purpose: Updates the opaquency after creation
	* Parameters: float f - the new opacuency to use
	* Returns: bool whether the input was valid or not
	*/
	bool setOpaquency(float f);

	/*
	* Method: TContent::getLocation
	* Purpose: Retrieves the current location of the content
	* Parameters:
	* Returns: RECT - the current location of the Control
	*/
	RECT getLocation();

	void SetRadialImage(TDataArray<D2D1_COLOR_F>& colors);
	void SetLinearImage(TDataArray<D2D1_COLOR_F>& colors);


	// Inherited From TControlComponent
	virtual void SetColor(const D2D1_COLOR_F& color)override;
	virtual D2D1_COLOR_F GetColor() override;
	virtual void SetColor2(const D2D1_COLOR_F& color) override;
	virtual D2D1_COLOR_F GetColor2() override;
	virtual D2D1_RECT_F GetLocation() override;
	virtual void SetLocation(const D2D1_RECT_F& loc) override;


	TrecPointer<TBrush> GetBrush();

	UINT GetImageCount();

private:
	TrecPointer<TGradientStopCollection> getStopCollection(TDataArray<D2D1_COLOR_F>& colors);
	void ResetBrush();


	/**
	 * The Drawing Board providing Drawing resources
	 */
	TrecPointer<DrawingBoard> drawingBoard;

	/**
	 * the brush to draw with
	 */
	TrecPointer<TBrush> brush;

	/**
	 * (not sure this is used)
	 */
	float thickness = 1.0f;
	/**
	 * (not used yet)
	 */
	TString style;

	/**
	 * the location of the content on the window
	 */
	D2D1_RECT_F location;
	/**
	 * Holds images to draw if provided
	 */
	TrecSubPointer<TBrush, TBitmapBrush> image;



	/*
	* Method: TContent::storeInTML
	* Purpose: Enables the Content to store itself in a TML file
	* Parameters: CArchive * ar - the file to write to
	*				int childLevel - the generation level content is (how many dashes to write)
	*				messageState ms - the state the content is (Object does not know this)
	* Returns: int - 0
	*/
	int storeInTML(TFile* ar, int childLevel,messageState);

	/**
	 * The TControl that owns the Border
	 */
	TControl* cap;
	/**
	 * which shape to use
	 */
	TShape shape;

	/**
	 * Holds color, used for creating the brush
	 */
	TGradientStopCollection stopCollection;
	/**
	 * whether we are creating a radial or gradient brush (ignored if only one color is given)
	 */
	bool useRadial;

	// Alternate shapes

	/**
	 * ellipse to draw if used
	 */
	D2D1_ELLIPSE circle;
	/**
	 * rounded rect to draw if used
	 */
	D2D1_ROUNDED_RECT roundedRect;

	/*
	* Method: TContent::BreakShared
	* Purpose: Breaks the link between the Content and the Parent Control, allowing deletion
	* Parameters: void
	* Returns: void
	* Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
	*/
	void BreakShared();
};

/**
 * Class: TControlParentHolder
 * Purpose: Implenents the TParentHolder to allow TControls to act as parents to other TControls
 */
class _ANAFACE_DLL TControlParentHolder: public TParentHolder
{
public:
	/**
	 * Method: TControlParentHolder::TControlParentHolder
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TControl> parent -  the TControl to serve as a Parent to TControls
	 * Returns: New TControlParentHolder
	 */
	TControlParentHolder(TrecPointer<TControl> parent);
	/**
	 * Method: TControlParentHolder::TControlParentHolder
	 * Purpose: Constructor
	 * Parameters: TrecPointerSoft<TControl> parent -  the TControl to serve as a Parent to TControls
	 * Returns: New TControlParentHolder
	 */
	TControlParentHolder(TrecPointerSoft<TControl> parent);

	/**
	 * Method: TControlParentHolder::SwitchChildControl
	 * Purpose: Has the Control switch one of its child controls with a new one
	 * Parameters: TrecPointerSoft<TControl> cur - the control making the call
	 *				TrecPointer<TControl> newTControl - the Control to replace it with
	 * Returns: void
	 */
	virtual void SwitchChildControl(TrecPointerSoft<TControl> cur, TrecPointer<TControl> newTControl)override;


	/**
	 * Method: TControlParentHolder::GetParent
	 * Purpose: Allows the Retrieval of the Parent Control (if the holder is holding a control)
	 * Parameters: void
	 * Returns: TrecPointer<TControl> - the Parent (the default returns null but the TControlParentHolder will return the parent)
	 */
	virtual TrecPointer<TControl> GetParent();


	/**
	 * Method: TControlParentHolder::IsScroller
	 * Purpose: Reports to the Child whether the parent holding it is a Scroller Control
	 * Parameters: void
	 * Returns: bool - whether or not the parent is a Scroller Control
	 */
	virtual bool IsScroller() override;
private:
	/**
	 * the Control to act as a parent
	 */
	TrecPointerSoft<TControl> parent;
};

/**
 * Class: TControl
 * Purpose: Base Class for all Controls in Anaface
 */
class _ANAFACE_DLL TControl : public TObject
{
	friend class TComboBox;
	friend class TControl_Edit;    // CDialog version of TControl editor
	friend class TControl_TDialog; // TDialog version of TControl editor
	friend class TBorder;
	friend class TControl;
	friend class TDataBind;
	friend class TTreeDataBind;
	friend class TControlParentHolder;
public:

	/*
	* Method: TControl::TControl
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	*				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	*				bool base - (DEPRECIATED) added to distinguish between base control and sub-classes
	* Return: New TControl Object
	*/
	TControl(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles, bool base = true);

	/*
	* Method: TControl::TControl
	* Purpose: Copy Constructor
	* Parameters: TControl &rCont - the Constructor to draw from
	* Returns: Copy of TControl
	*/
	TControl(TControl&);

	/*
	* Method: TControl::TControl)
	* Purpose: Default contructor
	* Parameters: void
	* Returns: Blank TControl
	*/
	TControl();

	/*
	* Method: TControl::~Tcontrol
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~TControl();

	/**
	 * Method: TControl::SetSelf
	 * Purpose: Allows TrecPointerKey to provide the Control with a reference to itself
	 * Parameters: TrecPointer<TControl> self - the TrecPointer of this Control that the TrecPointerKey generated
	 * Returns: void
	 */
	void SetSelf(TrecPointer<TControl> self);



	/*
	* Method: TControl::loadFromHTML
	* Purpose: Enables Control to load itself from an HTML File
	* Parameters: CArchive* ar - archive to read the HTML file
	* Returns: int - error
	* Note: DEPRECIATED - Functionality should be handled by an HTML parser that's Anaface Compatible
	*/
	virtual int loadFromHTML(TFile* ar);

	/*
	* Method: TControl::storeInTML
	* Purpose: Allows a Control to save itself in a TML File
	* Parameters: CArchive* ar - the file to save to
	*				int childLevel - the generation if the TControl
	*				bool overrideChildren - whether to ignore the children to save
	* Returns: void
	*/
	virtual void storeInTML(TFile* ar, int childLevel, bool overrideChildren = false);

	/*
	* Method: TControl::storeInHTML
	* Purpose: Enables a Control to save itself in an HTML file
	* Parameters: TFile* ar - the file to save to
	* Returns: void
	*/
	virtual void storeInHTML(TFile* ar);

	// Normal running messages


	/*
	 * Method: TControl::OnRButtonUp
	 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/*
	* Method: TControl::OnLButtonDown
	* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*/
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons);

	/*
	* Method: TControl::OnRButtonDown
	* Purpose: Allows Control to catch the RightmessageState::mouse button down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*/
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls);

	/*
	* Method: TControl::OnMouseMove
	* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*/
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls);

	/*
	* Method: TControl::OnLButtonDblClk
	* Purpose: Allows control to catch the DOuble Click event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/*
	* Method: TControl::OnLButtonUp
	* Purpose: Allows control to catch the Left Button Up event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/*
	* Method: TControl::OnChar
	* Purpose: Allows Controls to repond to character input
	* Parameters: bool fromChar - can be called either from on Key Down or OnChar
	*				UINT nChar - The ID of the character that was pressed
	*				UINT nRepCnt - how many times the character was processed for this event
	*				UINT nFlags - flags provided by MFC's Message system, not used
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut, TDataArray<EventID_Cred>& eventAr);

	// Builder Messages

	/*
	 * Method: TControl::Builder_OnLButtonUp
	 * Purpose: Allows special calls from the Builder to know when the control is no longer being dragged
	 * Parameters: UINT flags - Flags provided by MFC, not used
	 *				TPoint point - the Point where event occured
	 *				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
	 * Returns: void
	 */
	afx_msg void Builder_OnLButtonUp(UINT flags, TPoint point, TControl** mOut);

	/*
	* Method: TControl::Builder_OnLButtonDown
	* Purpose: Allows Controls to tell if a use of the Builder is planning to move them around
	* Parameters: UINT flags - Flags provided by MFC, not used
	*				TPoint point - the Point where event occured
	*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
	*				messageOutput* o - Allows control to keep track of how other controls respond to the call
	* Returns: void
	*/
	afx_msg void Builder_OnLButtonDown(UINT flags, TPoint point, TControl** mOut, messageOutput* o);

	/*
	* Method: TControl::Builder_OnMouseMove
	* Purpose: Allows controls to know when they are being moved
	* Parameters: UINT flags - Flags provided by MFC, not used
	*				TPoint point - the Point where event occured
	*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
	*				const RECT& bounds - bounds of the parent
	*				messageOutput* o - Allows control to keep track of how other controls respond to the call
	* Returns: void
	*/
	afx_msg void Builder_OnMouseMove(UINT flags, TPoint, TControl** mOut,const RECT&, messageOutput* o);

	/*
	* Method: TControl::Remove_Builder_Click_Focus
	* Purpose: Removes the burden of being dragged by the Builder
	* Parameters: void
	* Returns: void
	*/
	afx_msg void Remove_Builder_Click_Focus();

	/*
	* Method: TControl::setActive
	* Purpose: Enables controls to be either active (default) or inactive (they don't draw or respond to events)
	* Parameters: bool act - whether control should be active or not
	* Returns: void
	*/
	void setActive(bool act);

	/*
	* Method: TControl::getEventArgs
	* Purpose: Allows Action Methods to retrieve Event Arguements before calling the target Event Handler
	* Parameters: void
	* Returns: EventArgs - structure of relevent arguements to provide an Event Handler
	*/
	EventArgs getEventArgs();


	/*
	* Method: TControl::onCreate
	* Purpose: Allows the Control To contstruct itself based off of the location it has and the
	*		screen space it is given
	* Parameters: RECT contain - the area it can use
	* Returns: bool - success
	* Note: You Must call this on the Root Control before any control can be drawn on sreen
	*/
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d);

	/*
	 * Method: TControl::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: RECT r - the new location for the control
	 * Returns: void
	 */
	virtual void Resize(D2D1_RECT_F&);

	/*
	* Method: TControl::setEventHandler
	* Purpose: Sets up an event handler
	* Parameters: EventTarget& eh - the Event Class
	* Returns: bool - just returns true
	* Note: Possible redundence, expect to be DEPRECIATED
	*/
	bool setEventHandler(EventTarget& eh);

	/*
	* Method:  TControl::updateArrayID
	* Purpose: Stores an index to an external array, to be determined by the software using the control
	* Parameters: int aid - the array index to store, default -1
	* Returns: void
	*/
	void updateArrayID(int aid);

	/*
	* Method: TControl::onDraw
	* Purpose: Draws the control
	* Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
	* Returns: void
	*/
	virtual void onDraw(TObject* obj = nullptr);

	/*
	* Method: TControl::getLocation
	* Purpose: Retrieves the current physical location on the RenderTarget of the control
	* Parameters: void
	* Returns: D2D1_RECT_F - the location
	*/
	virtual D2D1_RECT_F getLocation();

	/*
	* Method: TControl::getMargin
	* Purpose: Retrieves the space between the control's location and the area it was given
	* Parameters: void
	* Returns: D2D1_RECT_F - the margin
	*/
	D2D1_RECT_F getMargin();

	/*
	* Method: TControl::getRenderTarget
	* Purpose: Retrieves the RenderTarget the Control is currently set to draw to
	* Parameters: void
	* Returns: TrecComPointer<ID2D1RenderTarget> - Smart Pointer holding the Render Target
	*/
	TrecPointer<DrawingBoard> getDrawingBoard();

	/*
	* Method: TControl::getParent
	* Purpose: Retrieves the parent control of the current control
	* Parameters: void
	* Returns: TrecPointer<TControl> - Smart Pointer to the Parent Control
	*/
	TrecPointer<TControl> getParent();

	/**
	 * DEPRECATED
	 */
	void setExternalBounds(D2D1_RECT_F);

	/*
	* Method:  TControl::getLayoutStatus
	* Purpose: Returns whether or not the control is a TLayout
	* Parameters: void
	* Returns: bool - whether or not control is a TLayout
	* Note: DEPRECIATED - C++ RTTI functionality can assume this purpose with greater precision
	*/
	bool getLayoutStatus();

	/*
	* Method: TControl::offsetLocation
	* Purpose: Moves the Control to the specified point
	* Parameters: TPoint cp - the point to move the control to
	* Returns: void
	*/
	void offsetLocation(TPoint);


	/*
	* Method: TControl::ShiftHorizontal
	* Purpose: Moves the Control left-or right, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the control and which direction
	* Returns: void
	*/
	void ShiftHorizontal(int degrees);

	/*
	* Method: TControl::ShiftVertical
	* Purpose:Moves the Control up or down, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the control and which direction
	* Returns: void
	*/
	void ShiftVertical(int degrees);


	/*
	* Method: TControl::setMinHeight
	* Purpose: Sets the Minimum height this control can be
	* Parameters: int h - the minimum height
	* Returns: void
	*/
	void setMinHeight(int h);

	/*
	* Method: TControl::setMaxHeight
	* Purpose: Sets the Maximum height this control can be
	* Parameters: int h - the maximum height this control can be
	* Returns: void
	*/
	void setMaxHeight(int h);

	/*
	* Method: TControl::setMinWidth
	* Purpose: Sets the minimum width the control can be
	* Parameters: int w - the minimum width
	* Returns: void
	*/
	void setMinWidth(int w);

	/*
	* Method: TControl::setMaxWidth
	* Purpose: Sets the maximum width the control can be
	* Parameters: int w - the max width
	* Returns: void
	*/
	void setMaxWidth(int w);

	/*
	* Method: TControl::setWidth
	* Purpose: Sets the current witdh of the control
	* Parameters: int w - the new width
	* Returns: void
	*/
	void setWidth(int w);

	/*
	* Method: TControl::setHeight
	* Purpose: Sets the current height of the control
	* Parameters: int h - the height to set the control to
	* Returns: void
	*/
	void setHeight(int h);

	/*
	* Method: TControl::setLocation
	* Purpose: Sets the Control in a new location
	* Parameters: RECT R - the new location
	* Returns: void
	*/
	virtual void setLocation(D2D1_RECT_F r);

	/*
	* Method: TControl::setMBottom
	* Purpose: Sets the margin on the bottom, likely used by the Builder when visually designing a control
	* Parameters: int b - the new bottom margin
	* Returns: void
	*/
	void setMBottom(int b);
	/*
	* Method: TControl::setMBottom
	* Purpose: Sets the margin on the top, likely used by the Builder when visually designing a control
	* Parameters: int t - the new top margin
	* Returns: void
	*/
	void setMTop(int t);
	/*
	* Method: TControl::setMRight
	* Purpose:Sets the margin on the right, likely used by the Builder when visually designing a control
	* Parameters: int r - the new right margin
	* Returns: void
	*/
	void setMRight(int r);
	/*
	* Method: TControl::setMLeft
	* Purpose: Sets the margin on the left, likely used by the Builder when visually designing a control
	* Parameters: int l - the new left margin
	* Returns: void
	*/
	void setMLeft(int l);

	/**
	 * Method: TControl::RotateDegrees
	 * Purpose: Rotates the Control
	 * Parameters: float degrees - the angle to rotate the Control by (in degrees)
	 * Returns: void
	 */
	void RotateDegrees(float degrees);
	/**
	 * Method: TControl::RotateRadians
	 * Purpose: Rotates the Control
	 * Parameters: float radians - the angle to rotate the Control by (in degrees)
	 * Returns: void
	 */
	void RotateRadians(float radians);


	/*
	* Method: TControl::determineMinHeightNeeded
	* Purpose: Determines the minimum height needed, base returns the current height, useful for sub classes
	* Parameters: void
	* Returns: UINT - the minimum height needed
	*/
	virtual UINT determineMinHeightNeeded();

	/**
	 * DEPRECATED in favor of the "Resize" method
	 */
	virtual void SetNewLocation(const D2D1_RECT_F& r);


	/*
	* Method: TControl::ShrinkHeight
	* Purpose: Reduces the height of the control down to what is needed --> just shrinks its children
	*		some of whom might find ways to shrink themselves
	* Parameters: void
	* Returns: void
	*/
	virtual void ShrinkHeight();


	/*
	 * Method: TControl::addAttribute
	 * Purpose: Adds a new attribute to use during creation
	 * Parameters: TString& attr - the attribute name to add
	 *				TrecPointer<TString> val - the value of that attribute
	 * Returns: bool - success or error
	 *
	 * Note: Call this method BEFORE calling onCreate. Otherwise, it will have no effect on the Control's
	 *	appearence or functionality
	 */
	bool addAttribute(const TString&, TrecPointer<TString>);

	/*
	* Method: TControl::addChild
	* Purpose: Adds a child control to the Current Control to hold
	* Parameters: TrecPointer<TContainer> tcon - the Child to add
	* Returns: bool - success or error
	*/
	bool addChild(TrecPointer<TControl>);

	/*
	* Method: TControl::setParent
	* Purpose: Sets the controls parent for reference in the tree
	* Parameters: TrecPointer<TParentHolder> tcp - the pointer to the parent (whether it is a TControl, a (Tap) Page
	* Returns: void
	* To-Do: Add code to make sure the parent is not a child or grand-child ( or lower)
	*/
	void setParent(TrecPointer<TParentHolder> tcp);

	/*
	* Method: TControl::getText
	* Purpose: Returns the Text Component labeled by the id
	* Parameters: int n - the id of the component
	* Returns: TrecPointer<TText> - Smart Pointer to the target text component
	*/
	TrecPointer<TText> getText(int n);

	/*
	* Method: TControl::getContent
	* Purpose: Returns the Content Component labeled by the id
	* Parameters: int n - the id of the component
	* Returns: const TrecPointer<TContent> - Smart Pointer to the target content component
	*/
	TrecPointer<TContent> getContent(int n);

	/*
	* Method: TControl::getBorder
	* Purpose: Returns the Border Component labeled by the id
	* Parameters: int n - the id of the component
	* Returns: const TrecPointer<TBorder> - Smart Pointer to the target Border Component
	*/
	TrecPointer<TBorder> getBorder(int n);

	/*
	* Method: TControl::setNewText
	* Purpose: Prepares a new TText component, likely called by the Builder when designing controls
	* Parameters: int n - the id of the new TText o override
	* Returns: void
	*/
	void setNewText(int n);


	/**
	 * Method: TControl::RegisterAnimations
	 * Purpose: Allows the Control to report Animations that it has been told to call for
	 * Parameters: TDataArray<TrecPointer<AnimationData>>& aData - reference to Animation Data Collection used by the Tap Library to generate Animation for this control
	 * Returns: void
	 */
	void RegisterAnimations(TDataArray<TrecPointer<AnimationData>>& aData);


	// Methods to support Scrolling

	/* Purpose: to allow the TControl to shift it's contents according to how it is scrolled.
	 * Caller: Member TScrollBars, either vScroll or hScroll
	 * Parameter: int (x) how to shift itself horizontally
	 * Parameter: int (y) how to shift itself vertically
	 * return: bool, just because
	 * Details: Changes the contents location on the board, reflected in the changes to
	 *		TControl's location, but not where it draws, hence why it's own snip stays the same
	 *		However, the contents might include child locations so their snips might have to be
	 *		updated
	 */
	virtual bool onScroll(int x, int y);

	/*
	* Method: TControl::onBeingScrolled
	* Purpose: To be called whenever a parent control is scrolling
	* Parameters: int x - the degree to which the control is being moved left or right
	*				int y - the degrees to which the control is being moved up or down
	*				const RECT& p_snip - the area of the parent control
	* Returns: bool - success
	*/
	bool onBeingScrolled(int x, int y);

	/*
	* Method: TControl::scroll
	* Purpose: Use in the event that scrolling needs to be done by a parent so control ends up in view
	* Parameters: RECT& loc - the location of the control that needs to be in view
	* Returns: void
	*/
	void scroll(RECT& loc);

	/*
	* Method: TControl::BreakShared
	* Purpose: Allows Tcontrol to be deleted by removing links to it by its conponents
	* Parameters: void
	* Returns: void
	* Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
	*/
	void BreakShared();

	/*
	* Method: TControl::AddClass
	* Purpose: Labels to serve as id's to draw styles from, useful in determining what HTML/TML element was used in creating the control
	* Parameters: TString& t - Class Name to add
	* Returns: void
	*/
	void AddClass(const TString& t);

	/*
	* Method: TControl::GetID
	* Purpose: Retrieves the ID for the Control
	* Parameters: void
	* Returns: TString - the ID of the Control (Careful, String could be blank)
	*/
	TString GetID();

	/*
	* Method: TControl::resetArgs
	* Purpose: resets the Arguements handlers have to their default values
	* Parameters: void
	* Returns: void
	*/
	void resetArgs();
	virtual UCHAR* GetAnaGameType()override;


	/*
	* Method: TControl::SetNormalMouseState
	* Purpose: Emergency method to set the Control state to normal
	* Parameters: void
	* Returns: void
	*/
	void SetNormalMouseState();

protected:
	//CMap<CString, CString, CString, CString> styles;
	TDataArray<TrecPointer<AnimationData>> animateData;

	/**
	 * Method: TControl::GetMultiData
	 * Purpose: Retireves the Attribute[s] of the Control by the key
	 * Parameters: const TString& key - the key to the attribute requested
	 * Returns: TDataArray<TString> - the list of values associated with the key
	 */
	TDataArray<TString> GetMultiData(const TString& key);

	/**
	 * Method: TControl::SetScrollControlOnMinSize
	 * Purpose: Allows the TControl to set a Scroller Control above it if the bounds becomes too tight
	 * Parameters: D2D1_RECT_F l - the new bounds of the TControl
	 * Returns: Whether the ScrollerControl was set or not
	 */
	bool SetScrollControlOnMinSize(D2D1_RECT_F l);

	/**
	 * Method: TControl::SwitchChildControl
	 * Purpose: Allows a child control to insert another control between it and THIS control (usually a scroller control)
	 * Parameters: TrecPointerSoft<TControl> curControl - the Control making the call (used to help parent control identify which child to replace)
	 *				 TrecPointer<TControl> newControl - the Control to replace it with
	 * Returns: void
	 */
	virtual void SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl);


	/**
	 * ?
	 */
	bool resistFocusRemoval;

	/**
	 * Unsure if this is still useful ?
	 */
	bool boundsPreset;
	/**
	 * List of Style classes
	 */
	TString className;
	/**
	 * The Name of the Control (comparable to HTML ID's)
	 */
	TString ID;
	/**
	 * Where the Control is to reside
	 */
	D2D1_RECT_F location, margin;
	/**
	 * ScrollBars in case content is larger than it's allocated drawing space
	 */
	TrecPointer<TScrollBar> vScroll, hScroll;
	/**
	 * the Board to draw with
	 */
	TrecPointer<DrawingBoard> drawingBoard;

	/**
	 * Method: TControl::GetParentReference
	 * Purpose: Sets up a Parent Holder for this Control (used so that no redundant holder object is created)
	 * Parameters: void
	 * Returns: TrecPointer<TParentHolder> - the holder object referencing this control as a parent control
	 */
	TrecPointer <TParentHolder> GetParentReference();

	/**
	 * Allows child controls to point to THIS control as their parent
	 */
	TrecPointer<TParentHolder> thisParent;
	/**
	 * Points to which ever object is holding this control
	 */
	TrecPointer<TParentHolder> parent;
	/**
	 * Trec reference to this control (so it can provide objects with a safe reference to itself)
	 */
	TrecPointerSoft<TControl> tThis;


	/**
	 * Level Control is on the Tree (not currently used)
	 */
	BYTE treeLevel;

	/**
	 * the border of the control
	 */
	TrecPointer<TBorder> border1, border2, border3;
	/**
	 * the Text of the control
	 */
	TrecPointer<TText> text1 , text2, text3;
	/**
	 * The content of the control
	 */
	TrecPointer<TContent> content1 , content2, content3;

	/**
	 * the ellipse to pass into the Border and content if used
	 */
	D2D1_ELLIPSE ellipse;
	/**
	 * the rounded rect to pass into the Border and content if used
	 */
	D2D1_ROUNDED_RECT roundedRect;
	/**
	 * The shape the control is in
	 */
	TShape shape;

	/**
	 * the Matrix to manage rotations and other transformations with
	 */
	D2D1_MATRIX_3X2_F controlTransform;
	/**
	 * how much to rotate the control by when drawing
	 */
	float rotation;


	/*
	* Method: TControl::onCreate
	* Purpose: Sets up the basic components based off of the available attributes
	* Parameters: TMap<TString>* att - list of attributes
	*				RECT loc - the location ot use
	* Returns: bool - false
	*/
	bool onCreate(TMap<TString>*, D2D1_RECT_F);

	/*
	* Method: TControl::onCreate2
	* Purpose: Sets up themessageState::mouse hover components based off of the available attributes
	* Parameters: TMap<TString>* att - list of attributes
	*				RECT loc - the location ot use
	* Returns: bool - false
	*/
	bool onCreate2(TMap<TString>*, D2D1_RECT_F);

	/*
	* Method: TControl::onCreate3
	* Purpose: Sets up the click event components based off of the available attributes
	* Parameters: TMap<TString>* att - list of attributes
	*				RECT loc - the location ot use
	* Returns: bool - false
	*/
	bool onCreate3(TMap<TString>*, D2D1_RECT_F);


	/*
	* Method: TControl::updateComponentLocation
	* Purpose: Update the locations of the control's components - call whenever the control's location is updated
	* Parameters: void
	* Returns: void
	*/
	void updateComponentLocation();

	/**
	 * Method: TControl::CheckScroll
	 * Purpose: Makes sure that the Control doesn't need scrollers or, if it needs them, then the bars are properly configured
	 * Parameters: void
	 * Returns: void
	 */
	void CheckScroll();

	/*
	* Method: TControl::generateImage
	* Purpose: Sets up an image for the Control if an attribute calls for it
	* Parameters: TrecPointer<TContent> tcon - the Content component to attach the image to
	*				TrecPointer<TString> p - the file name of the target image
	*				RECT loc - the location where the image should be
	* Returns: int - error (0 if successful)
	*/
	int generateImage(TrecPointer<TContent> tcon, TrecPointer<TString> p,D2D1_RECT_F& loc);

	/*
	* Method: TControl::checkMargin
	* Purpose: Sets up the margin attribute if available, called by onCreate
	* Parameters: RECT contain - the location given to onCreate
	* Returns: void
	*/
	void checkMargin(D2D1_RECT_F);

	/*
	* Method: TControl::checkHeightWidth
	* Purpose: checks for the height and width attributes of the control, called by onCreate
	* Parameters: RECT r the location given to onCreate
	* Returns: void
	*/
	void checkHeightWidth(D2D1_RECT_F);

	/*
	* Method: TControl::generateSizeControl
	* Purpose: Sets up the heights and widths of the control (stored as pointer so controls that
	*		don't have this attribute can save memory this way
	* Parameters: void
	* Returns: void
	*/
	void generateSizeControl();


	/**
	 * Holds the dimensions of the Control (if specified)
	 */
	sizeControl* dimensions;

	/**
	 * DEPRECATED
	 */
	bool isLayout, isTextControl;


	/**
	 * If margin and dimension attributes conflict, which take precedence
	 */
	bool marginPriority;
	/**
	 * whether the margin of the Control has been set
	 */
	bool marginSet;
	/**
	 * whether the hieght can be changed or not
	 */
	bool fixHeight;
	/**
	 * whether the width can be changed or not
	 */
	bool fixWidth;

	/**
	 * List of attributes to use for the control to sets it's structure and style
	 */
	TMap<TString> attributes;
	/**
	 * List of Styles to draw from 
	 */
	TrecPointer<TArray<styleTable>> styles;
	/**
	 * the Controls held by THIS control
	 */
	TArray<TControl> children;
	/**
	 * DEPRECATED ?
	 */
	TArray<TArray<TString>> holder;

	// data for dealing with messages

	/**
	 * should response to message stop parent from doing the same?
	 */
	bool overrideParent;

	/**
	 * mechanism as to which version of the TControl should be drawn based off of user interaction
	 */
	messageState mState;

	/**
	 * Information about the arguements set when an event is generated
	 */
	EventArgs args;

	/**
	 * List of Events to report given certain messages
	 */
	TDataArray<EventTypeID> eventList;

	/**
	 * Controls can be disabled, through this bool
	 */
	bool isActive;

	/**
	 * Keeps track of whether the mouse button came down on these controls
	 */
	bool isLClick, isRClick;

	/**
	 * Whether mouse is over the control
	 */
	bool isMouseFocus;


	/*
	 * Method: TControl::hasEvent
	 * Purpose: Checks to see whether the control is set to respond to a certain event with a handler
	 * Parameters: R_Message_Type type - type of message to respond to
	 * Returns: bool - presense of type handler
	 */
	bool hasEvent(R_Message_Type type);

	/*
	* Method: TControl::getEventID
	* Purpose: Controls never touch their handlers. Instead, they're given an index to a collection of handlers
	* Parameters: R_Message_Type type -  the type of message to respond to
	* Returns: int - index of handler to call (-1 if handler not provided)
	*/
	int getEventID(R_Message_Type type);

	/*
	* Method: TControl::SetToRenderTarget
	* Purpose: Updates each component to the controls current DrawingBoard, call if DrwingBoard is ever switched
	* Parameters: void
	* Returns: void
	*/
	void SetToRenderTarget();

	// and Builder Messages
	bool rightBorder, leftBorder, topBorder, bottomBorder, // if the border is being moved
		onFocus,
		onClickFocus;


	/**
	 * DEPRECATED
	 */
	EventTarget* eventHandler;

	/**
	 * Holds index of this TControl if stored in an external container
	 */
	int arrayID;

	public:

	/*
	 * Method: TControl::addEventID
	 * Purpose: Adds the Event Type with a handler index to associate it with
	 * Parameters: R_Message_Type rmt -  message type
	 *				int e_id - the idex of the message handler that goes with it
	 * Returns: void
	 */
	void addEventID(R_Message_Type rmt, int e_id);
	
};

/*
* Function: resetAttributeString
* Purpose: Resets an attribute string for when saving to a TML String attribute
* Parameters: TString* st - The String to Adjust
*				int cou - the generation count
* Returns: void
*/
void _ANAFACE_DLL resetAttributeString(TString* st, int cou);

/*
* Function: convertD2DColorToString
* Purpose: Creates a string that represents a D2D color
* Parameters: D2D1::ColorF - the color to convert
* Returns: CString - the Color in string form
*/
TString _ANAFACE_DLL convertD2DColorToString(D2D1_COLOR_F);

/*
* Function: convertRectToString
* Purpose: Creates a string that represents a rectangle
* Parameters: RECT - the Rectangle to convert
* Returns: CString - the Representation of a rectangle in string form
*/
TString _ANAFACE_DLL convertRectToString(RECT);

/*
* Function: convertRectToString
* Purpose: Creates a string that represents a rectangle
* Parameters: D2D1_RECT_F - the Rectangle to convert
* Returns: CString - the Representation of a rectangle in string form
*/
TString _ANAFACE_DLL convertD2D1RectToString(D2D1_RECT_F);

/*
* Function: convertStringToRECT
* Purpose: Determines the Rectangle to use
* Parameters: TString* the String to read
* Returns: RECT - the resulting rectangle (Zero Rect if string is invalid)
*/
RECT _ANAFACE_DLL convertStringToRECT(TString*);

/*
* Function: convertStringToRect
* Purpose: Determines the Rectangle to use
* Parameters: TString* the String to read
* Returns: D2D1_RECT_F - the resulting rectangle (Zero Rect if string is invalid)
*/
D2D1_RECT_F _ANAFACE_DLL convertStringToD2D1Rect(TString*);

/*
* Function: convertStringToD2DColor
* Purpose: Converts a String to it's Color Representation (Black if invalid)
* Parameters: TString* - the string to read
* Returns: D2D1::ColorF - the color the string represents (Black if invalid)
*/
D2D1::ColorF _ANAFACE_DLL convertStringToD2DColor(TString*);

/*
* Function: convertRECTToD2DRectF
* Purpose: Converts an MFC Rectangle to a D2D Rectangle
* Parameters: RECT - the Rectangle to Convert
* Returns: D2D1_RECT_F - the D2D Rectangle sought
*/
D2D1_RECT_F _ANAFACE_DLL convertRECTToD2DRectF(RECT);

/*
* Function: convertD2DRectToRECT
* Purpose: Returns an MFC Rectangle from a Direct2D rectangle
* Parameters: D2D1_RECT_F f - the D2D Rectangle to convert
* Returns: RECT - the MFC rectangle created
*/
RECT _ANAFACE_DLL convertD2DRectToRECT(D2D1_RECT_F f);

/*
* Function: convertCRectToD2DRect
* Purpose: Turns an MFC RECT into a Direct2D Rect for compatibility purposes
* Parameters: RECT * - the source rectangle
*				D2D1_RECT_F* - the destination rectangle
* Returns: false if either parameter is null
*/
bool _ANAFACE_DLL convertCRectToD2DRect(RECT*, D2D1_RECT_F*);

/*
* Function: isContained
* Purpose: Checks of a point is within a given MFC Rectangle
* Parameters: const TPoint* - the point to check
*				const RECT* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool _ANAFACE_DLL isContained(const TPoint*, const RECT*);

/*
* Function: isContained
* Purpose: Checks of a point is within a given MFC Rectangle
* Parameters: const TPoint& - the point to check
*				const RECT& - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool _ANAFACE_DLL isContained(const TPoint&, const RECT&);

/*
* Function: isContained
* Purpose: Checks of a point is within a given Direct2D Rectangle
* Parameters: const TPoint* - the point to check
*				const D2D1_RECT_F* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool _ANAFACE_DLL isContained(const TPoint*, const D2D1_RECT_F*);

/*
* Function: isContained
* Purpose: Checks of a point is within a given Direct2D Rectangle
* Parameters: const TPoint& - the point to check
*				const D2D1_RECT_F& - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool _ANAFACE_DLL isContained(const TPoint&, const D2D1_RECT_F&);

/*
* Function: isContained
* Purpose: Checks if a point is in a certain ellipse
* Parameters: const TPoint* - the point to check
*				const D2D1_ELLIPSE* - the ellipse to check
* Returns: bool - whether the point is within the specified bounds
* Note: Algorithm for function found on https://math.stackexchange.com/questions/76457/check-if-a-point-is-within-an-ellipse
*/
bool isContained(const TPoint&, const D2D1_ELLIPSE&);

/*
* Function: convertStringToTextAlignment
* Purpose: Reads the String and returns whether the Text should be right, left, center, or justified
* Parameters: TString* t - the string to parse
* Returns: DWRITE_TEXT_ALIGNMENT - the Text position (Center if string is invalid)
*/
DWRITE_TEXT_ALIGNMENT convertStringToTextAlignment(TString* t);

/*
* Function: convertStringToParagraphAlignment
* Purpose: Reads the String and returns whether the Text should be top, bottom, or center
* Parameters: TString* t - the string to parse
* Returns: DWRITE_PARAGRAPH_ALIGNMENT - the text alignment (Center if string is invalid)
*/
DWRITE_PARAGRAPH_ALIGNMENT convertStringToParagraphAlignment(TString* t);

/*
* Function: ZeroMemory_
* Purpose: "Zero's an Event Args, (Using Microsoft's ZeroMemory macro currupts the TString in the EVent Args)
* Parameters: EventArgs& ea - the Event arguement structure to reset
* Returns: void
*/
void _ANAFACE_DLL ZeroMemory_(EventArgs& ea);

/*
* Function: switchLongs
* Purpose: Swaps longs
* Parameters: LONG& l1 - the first long to switch
*				LONG& l2 - the second long to switch
* Returns: void
*/
void switchLongs(LONG& l1, LONG& l2);

/*
* Function: isSnipZero
* Purpose: Checks if a Rectangle is a "Zero" rectangle
* Parameters: const RECT& snip - the Rectangle to check
* Returns: bool - whether the RECT is "Zero" or not
*/
bool _ANAFACE_DLL isSnipZero(const RECT& snip);

/*
* Function: isSnipZero
* Purpose: Checks if a Rectangle is a "Zero" rectangle
* Parameters: const D2D1_RECT_F& snip - the Rectangle to check
* Returns: bool - whether the RECT is "Zero" or not
*/
bool _ANAFACE_DLL isSnipZero(const D2D1_RECT_F& snip);

/*
* Function: zeroSnip
* Purpose: Sets the Rectangle to 0, used to mark a control as off screen 
* Parameters: RECT& snip - the Rectangle to Nullify
* Returns: void
*/
void zeroSnip(RECT& snip);

bool _ANAFACE_DLL operator==(D2D1_RECT_F& op1, D2D1_RECT_F& op2);

bool operator!=(D2D1_RECT_F& op1, D2D1_RECT_F& op2);

//void DrawArc(const D2D1_ELLIPSE& e, const D2D1_RECT_F& loc, const D2D1_RECT_F& snip, const D2D1_COLOR_F color);

//bool _ANAFACE_DLL IsD2D1RectEqual(const D2D1_RECT_F& r1,const  D2D1_RECT_F& r2, float difference_);