#pragma once

#include <TObject.h>
#include "DrawingBoard.h"
#include <dwrite.h>
#include <TPoint.h>
#include "TTextRenderer.h"

/**
 * Enum CLass: control_text_mode
 * Purpose: Whether this is a cut, copy, or paste operation
 */
typedef enum class control_text_mode {
	ctm_cut,
	ctm_copy,
	ctm_paste
}control_text_mode;

// Allows Anaface to format the text at different sections, used by Web-Tuors
// To support the <b> and <i> tags in HTML
class _VIDEO_GRAPHICS TextFormattingDetails
{
public:
	TextFormattingDetails();
	TextFormattingDetails(const TextFormattingDetails& copy);

	DWRITE_FONT_WEIGHT weight; // For handling boldness
	DWRITE_FONT_STYLE style;   // For handling italics
	DWRITE_TEXT_RANGE range;   // the Range through which it should be done
	DWRITE_FONT_STRETCH stretch;
	TrecPointer<TBrush> color; // The color of the Text
	TrecPointer<TBrush> bColor;// The Color of the Highlight
	TColor defaultBackgroundColor; // the Color of the highlighter to manage when highlighter is not set
	float fontSize;
};

class _VIDEO_GRAPHICS HighlightRange
{
public:
	HighlightRange();
	HighlightRange(const HighlightRange& range);

	bool beginSet, endSet;
	UINT begin, end;

	void Reset();
	bool GetCarotLocation(UINT& loc);
	bool GetHighlightRange(UINT& begin, UINT& end);

	void SetBegin(UINT begin);
	bool SetEnd(UINT end);
};

/**
 * Class: TTextElement
 * Purpose: Provides support for managing Text
 */
class TTextElement : public TObject
{
	friend class TrecPointerKey;
protected:
	/**
	 * Whether the base format has been changed
	 */
	bool recreateFormat;

	/**
	 * whether click-down is active
	 */
	bool isClickDown;

	/**
	 * The Drawing Board to work with
	 */
	TrecPointer<DrawingBoard> drawingBoard;

	/**
	 * The Layout
	 */
	TrecComPointer<IDWriteTextLayout> mainLayout;
	/**
	 * Direct Write Factory
	 */
	TrecComPointer<IDWriteFactory> writeFact;
	/**
	 * the format object to write text with
	 */
	TrecComPointer<IDWriteTextFormat> format;
	/**
	 * Location of the text
	 */
	D2D1_RECT_F bounds;
	/**
	 * Caption of the text object
	 */
	TString text;

	/**
	 * Other Text details in string form (locale, font type)
	 */
	TString locale, fontType;

	/**
	 * Basic Formatting details
	 */
	TextFormattingDetails basicDetails;

	/**
	 * Vertical alignment of the text (top, bottom, or center)
	 */
	DWRITE_PARAGRAPH_ALIGNMENT verticalAlignment;
	/**
	 * The horizontal alignment of the text (left, right, center or justified)
	 */
	DWRITE_TEXT_ALIGNMENT horizontalAlignment;

	/**
	 * Manages highlighter
	 */
	HighlightRange highlightRange;

	/**
	 * Self Pointer
	 */
	TrecPointerSoft<TTextElement> self;

	/**
	 * Method: TTextElement::SetSelf
	 * Purpose: Enables TrecPointerKey to assign a self reference upon creation
	 * Parameters: TrecPointer<TTextElement> self - the self reference
	 * Returns: void
	 */
	void SetSelf(TrecPointer<TTextElement> self);


	/*
	 * Method: TText::reCreateLayout
	 * Purpose: Resets the Text Layout upon changes made to the format, text, or location
	 * Parameters: void
	 * Returns: void
	 * Note: Call this method whenever you make a change to the format of the control, the text, or the location,
	 *		Otherwise, your changes might not be seen
	 * 
	 * Attributes: virtual
	 */
	virtual void ReCreateLayout();

public:
	/**
	 * Method: TTextElement::TTextElement
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> drawingBoard - the drawing board to generate a Layout from
	 * Returns: new TTextElement object
	 */
	TTextElement(TrecPointer<DrawingBoard> drawingBoard);

	/**
	 * Method: TTextElement::SetText
	 * Purpose: Sets the Text of this element
	 * Parameters: const TString& text - the text to set to
	 * Returns: bool - whether the text was set or not
	 * 
	 * Attributes: virtual
	 * 
	 * Note: For most objects, true should be returned. However, more complex implementations of this object could have complex
	 *	layouts, wich may deem a simple setting of the text non-viable. If this were to return false, check documentation for the subclass
	 *  that you are using.
	 */
	virtual bool SetText(const TString& text);


	/**
	 * Method: TTextElement::GetText
	 * Purpose: Gets the Text of this element
	 * Parameters: TString& text - the text to hold data
	 * Returns: void
	 */
	virtual void GetText(TString& text);

	/**
	 * Method: TTextElement::GetLocation
	 * Purpose: Retrieves the location of the Text Element
	 * Parameter: void
	 * Returns: D2D1_RECT_F - location of text element
	 */
	D2D1_RECT_F GetLocation();


	/**
	 * Method: TTextElement::SetLocation
	 * Purpose: Sets the location of the Text Element
	 * Parameter: const D2D1_RECT_F& - location of text element
	 * Returns: void
	 * 
	 * Note: if the parameters are mis-matched, (i.e. the left field is greater than the right) the fields will be reversed
	 */
	 void SetLocation(const D2D1_RECT_F& loc);

	 /**
	  * Method: TTextElement::SetBasicFormatting
	  * Purpose: Sets the Basic formatting
	  * Parameters: const TextFormattingDetails& details - the details to set
	  * Returns: bool - should be true for most instances but some implementations might see fit to refuse
	  * 
	  * Attributes: virtual
	  */
	 virtual bool SetBasicFormatting(const TextFormattingDetails& details);

	 /**
	  * Method: TTextElement::SetBasicFormatting
	  * Purpose: Sets the Basic formatting
	  * Parameters: const TextFormattingDetails& details - the details to set
	  *				UINT start - the start of the range to apply this formatting to
	  *				UINT end - the end of the range
	  *				bool correctInvalidParams - whether the method should attempt to correct invalid parameters (start > end, start or end being greater than string length, etc.)
	  * Returns: bool - should be false for most instances but some implementations might support this feature
	  *
	  * Attributes: virtual
	  */
	 virtual bool SetFormatting(const TextFormattingDetails& details, UINT start, UINT end, bool correctInvalidParams);

	 /**
	  * Method: TTextElement::GetBasicFormatting
	  * Purpose: Gets the Basic formatting
	  * Parameters: TextFormattingDetails& details - the details to get
	  * Returns: bool - should be true for most instances but some implementations might see fit to refuse
	  *
	  * Attributes: virtual
	  */
	 virtual bool GetBasicFormatting(TextFormattingDetails& details);

	 /**
	  * Method: TTextElement::OnClickDown
	  * Purpose: Handles event that a mouse click occurs over it
	  * Parameters: const TPoint& point
	  * Returns: bool - whether point was within location
	  * 
	  * Attributes: virtual 
	  */
	 virtual bool OnCLickDown(const TPoint& point);


	 /**
	  * Method: TTextElement::OnClickUp
	  * Purpose: Handles event that a mouse click occurs over it
	  * Parameters: const TPoint& point
	  * Returns: bool - whether point was within location
	  *
	  * Attributes: virtual
	  */
	 virtual bool OnCLickUp(const TPoint& point);


	 /**
	  * Method: TTextElement::OnMouseMove
	  * Purpose: Handles event that a mouse hovers over it
	  * Parameters: const TPoint& point
	  * Returns: bool - whether point was within location AND Text is curently tracking mouse movement
	  *
	  * Attributes: virtual
	  */
	 virtual bool OnMouseMove(const TPoint& point);

	 /**
	  * Method: TTextElement::OnLoseFocus
	  * Purpose: Whether a mouse click was detected outside of it
	  * Parameters: void
	  * Returns: void
	  * 
	  * Attributes: virtual
	  */
	 virtual void OnLoseFocus();

	 /**
	  * Method: TTextElement::OnCutCopyPaste
	  * Purpose: Allows Anagae to support Cut-Copy-Paste
	  * Parameters: control_text_mode mode - cut, copy, or paste?
	  * Returns: void
	  * 
	  * Attributes: virtual
	  * 
	  * Note: no other parameters are needed as Text Element should have an internal state 'HighlightRange' or 'carotLoc' to ficure out which text to apply
	  */
	 virtual void OnCutCopyPaste(control_text_mode mode);

	 /**
	  * Method: TTextElement::OnInputChar
	  * Purpose: Inserts Text at the moment
	  * Parameters: WCHAR ch - text to insert
	  *				UINT count - number of times to insert it
	  * Returns: bool - whether text was altered (this base class does not support edit-mode so it will return false, but subclasses might and thus return true)
	  * 
	  * Attributes: virtual
	  */
	 virtual bool OnInputChar(WCHAR ch, UINT count);

	 /**
	  * Method: TTextElement::OnDraw
	  * Purpose: Draws the Text
	  * Parameters: TObject* obj - the object to draw
	  * Returns: void
	  * 
	  * Attributes: virtual
	  */
	 virtual void OnDraw(TObject* obj);

	 /**
	  * Method: TTextElement::GetColor
	  * Purpose: Retrieves the specified color
	  * Parameters: TColor& color - the color to retrieve
	  *				bool foreground (default true) - color of the text (as opposed to the background)
	  * Returns: bool - whether a color was returned or not (should be true, but some implemetations might say false)
	  * 
	  * Attributes: virtual
	  */
	 virtual bool GetColor(TColor& color, bool foreground = true);

	 /**
	  * Method: TTextElement::SetColor
	  * Purpose: Sets the specified color
	  * Parameters: const TColor& color - the color to retrieve
	  *				bool foreground (default true) - color of the text (as opposed to the background)
	  * Returns: bool - whether a color was set or not (should be true, but some implemetations might say false)
	  *
	  * Attributes: virtual
	  */
	 virtual bool SetColor(const TColor& color, bool foreground = true);

	 /**
	  * Method: TTextEntitlement::GetVerticalAlignment
	  * Purpose: Retireves the current Vertical alignment
	  * Parameters: void
	  * Returns: DWRITE_PARAGRAPH_ALIGNMENT - the vertical alignment of the text
	  */
	 DWRITE_PARAGRAPH_ALIGNMENT GetVerticalAligment();

	 /**
	  * Method: TTextEntitlement::SetVerticalAlignment
	  * Purpose: Sets the Alignment
	  * Parameters: DWRITE_PARAGRAPH_ALIGNMENT vAlignment - the alignment to set
	  * Returns: void
	  */
	 void SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT vAlignment);

	 /**
	  * Method: TTextEntitlement::GetHorizontalAlignment
	  * Purpose: Retireves the current Horizontal alignment
	  * Parameters: void
	  * Returns: DWRITE_TEXT_ALIGNMENT - the vertical alignment of the text
	  */
	 DWRITE_TEXT_ALIGNMENT GetHorizontalAlignment();

	 /**
	  * Method: TTextEntitlement::SetHorizontallignment
	  * Purpose: Sets the Alignment
	  * Parameters: DWRITE_TEXT_ALIGNMENT vAlignment - the alignment to set
	  * Returns: void
	  */
	 void SetHorizontallignment(DWRITE_TEXT_ALIGNMENT vAlignment);

	 /**
	  * Method: TTextEntitlement::GetSetLocale
	  * Purpose: Sets or Sets the locale of the text
	  * Parameters: TString& locale - the locale 
	  *				bool doGet (true) - if true, mehtod acts like a getter, and acts like a setter if false
	  * Returns: bool - should be true under get, false if setting failed
	  */
	 bool GetSetLocale(TString& local, bool doGet = true);

	 /**
	  * Method: TTextEntitlement::GetSetFont
	  * Purpose: Sets or Sets the font of the text
	  * Parameters: TString& font - the font
	  *				bool doGet (true) - if true, mehtod acts like a getter, and acts like a setter if false
	  * Returns: bool - should be true under get, false if setting failed
	  */
	 bool GetSetFont(TString& font, bool doGet = true);

	 /**
	  * Method: TTextEntitlement::GetSetFontSize
	  * Purpose: Sets or Sets the Font Size of the text
	  * Parameters: float& fontSize - the locale
	  *				bool doGet (true) - if true, mehtod acts like a getter, and acts like a setter if false
	  * Returns: bool - should be true under get, false if setting failed
	  */
	 bool GetSetFontSize(float& fontSize, bool doGet = true);
};

_VIDEO_GRAPHICS TrecComPointer<TTextRenderer> GetTTextRenderer();

_VIDEO_GRAPHICS bool IsContained(const TPoint& point, const D2D1_RECT_F& loc);