#pragma once
#include "TTextElement.h"

/**
 * Class: TInputTextElement
 * Purpose: Extends the Text Element to provide support for editing
 */
class _VIDEO_GRAPHICS TInputTextElement : public TTextElement
{
protected:
	/**
	 * Carot Location
	 */
	UINT carotLoc;

	/**
	 * Should Show Carot
	 */
	bool carotActive;
	/**
	 * Whether the user has grabed Highlighted text
	 */
	bool grabbedRange;

	/**
	 * The Window to manage carot
	 */
	HWND window;

	/**
	 * Method: TInputTextElement::UpdateCarotByPosition
	 * Purpose: Sets the carot position by text index
	 * Parameters: UINT loc - the location to move the carot to
	 * Returns: void
	 */
	void UpdateCarotPoisition(UINT loc);
public:
	/**
	 * Method: TInputTextElement::TInputTextElement
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> drawingBoard - the drawing board to generate a Layout from
	 * Returns: new TTextElement object
	 */
	TInputTextElement(TrecPointer<DrawingBoard> drawingBoard, HWND window);

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
	 * Method: TTextElement::OnLoseFocus
	 * Purpose: Whether a mouse click was detected outside of it
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void OnLoseFocus() override;

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
	 * Method:TTextElement::OnInputKey
	 * Purpose: handles keyboard input that does not qualify as characters
	 * Parameters: UINT key - the key pressed
	 *				UINT count - the number of times to process it
	 * Returns: bool - whether the object did anything with it
	 *
	 * Attributes: override
	 */
	virtual bool OnInputKey(UINT key, UINT count);

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

