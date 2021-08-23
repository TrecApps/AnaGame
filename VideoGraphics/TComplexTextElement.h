#pragma once
#include "TInputTextElement.h"

/**
 * Class: TComplexTextElement
 * Purpose: Extends the Input Text Element to provide support for complex formats
 */
class TComplexTextElement : public TInputTextElement
{
protected:
	/**
	 * Whether to allow Editing
	 */
	bool editAllowed;

	/**
	 * Formatting Details
	 */
	TDataArray<TextFormattingDetails> formatting;


	/*
	 * Method: TComplexTextElement::ReCreateLayout
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
	 * Method: TTextElement::SetText
	 * Purpose: Sets the Text of this element
	 * Parameters: const TString& text - the text to set to
	 * Returns: bool - whether the text was set or not
	 *
	 * Attributes: virtual
	 *
	 * Note: This implementation will perform the operation (and return true) under the folowwing circumstances
	 *		There are no formatting details or edit mode is off (and formatting details is removed)
	 */
	virtual bool SetText(const TString& text);

	/**
	 * Method: TComplexTextElement::TComplexTextElement
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> drawingBoard - the drawing board to generate a Layout from
	 * Returns: new TTextElement object
	 */
	TComplexTextElement(TrecPointer<DrawingBoard> drawingBoard, HWND window, bool editAllowed = true);


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
	 * Method: TTextElement::OnClickDown
	 * Purpose: Handles event that a mouse click occurs over it
	 * Parameters: const TPoint& point
	 * Returns: bool - whether point was within location
	 *
	 * Attributes: override
	 */
	virtual bool OnCLickDown(const TPoint& point) override;

	/**
	 * Method: TTextElement::OnClickUp
	 * Purpose: Handles event that a mouse click occurs over it
	 * Parameters: const TPoint& point
	 * Returns: bool - whether point was within location
	 *
	 * Attributes: override
	 */
	virtual bool OnCLickUp(const TPoint& point) override;


	/**
	 * Method: TTextElement::OnMouseMove
	 * Purpose: Handles event that a mouse hovers over it
	 * Parameters: const TPoint& point
	 * Returns: bool - whether point was within location AND Text is curently tracking mouse movement
	 *
	 * Attributes: override
	 */
	virtual bool OnMouseMove(const TPoint& point) override;


	/**
	 * Method: TTextElement::OnCutCopyPaste
	 * Purpose: Allows Anagae to support Cut-Copy-Paste
	 * Parameters: control_text_mode mode - cut, copy, or paste?
	 * Returns: void
	 *
	 * Attributes: override
	 *
	 * Note: no other parameters are needed as Text Element should have an internal state 'HighlightRange' or 'carotLoc' to ficure out which text to apply
	 */
	virtual void OnCutCopyPaste(control_text_mode mode) override;

	/**
	 * Method: TTextElement::OnInputChar
	 * Purpose: Inserts Text at the moment
	 * Parameters: WCHAR ch - text to insert
	 *				UINT count - number of times to insert it
	 * Returns: bool - whether text was altered (this base class does not support edit-mode so it will return false, but subclasses might and thus return true)
	 *
	 * Attributes: override
	 */
	virtual bool OnInputChar(WCHAR ch, UINT count) override;

	/**
	 * Method: TInputTextElement::OnTransferText
	 * Purpose: Performs the transfer of text
	 * Parameters: UINT newPos - the new position of the text
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void OnTransferText(UINT newPos);
};

