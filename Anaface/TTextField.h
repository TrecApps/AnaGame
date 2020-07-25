#pragma once
#include "TControl.h"
#include "TGadgetControl.h"


// Allows TTextControls to keep track of numbers whether they are int or float
typedef enum class _ANAFACE_DLL iControlType
{
	t_none,  // the type has not been determined
	t_float, // the type is a float
	t_int    // the type is an int
}iControlType;

// Holds either a float or an integer
typedef union _ANAFACE_DLL incrimentControlValue
{
	float f;
	int i;
}incrimentControlValue;

// Holds a value of type int or float and a marker indicating the type
typedef struct _ANAFACE_DLL incrimentControl
{
	iControlType type;
	incrimentControlValue value;
	void operator=(incrimentControl&);
	void operator=(int i);
	void operator=(float f);
}incrimentControl;

// Allows Anaface to format the text at different sections, used by Web-Tuors
// To support the <b> and <i> tags in HTML
typedef struct formattingDetails
{
	DWRITE_FONT_WEIGHT weight; // For handling boldness
	DWRITE_FONT_STYLE style;   // For handling italics
	DWRITE_TEXT_RANGE range;   // the Range through which it should be done
}formattingDetails;


/*
* Function: operator>
* Purpose:  Compares two incrimentControls for >
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator>(incrimentControl& ic1, incrimentControl& ic2);
/*
* Function: operator>
* Purpose: Compares an incrimentControl to an int for >
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator>(incrimentControl& ic, int i);
/*
* Function: operator>
* Purpose: Compares an incrimentControl to an int for >
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator>(incrimentControl& ic, float f);
/*
* Function: operator>
* Purpose: Compares an incrimentControl to an int for >
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>(int i, incrimentControl& ic);
/*
* Function: operator>
* Purpose: Compares an incrimentControl to a float for >
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>(float f, incrimentControl& ic);



/*
* Function: operator>
* Purpose:  Compares two incrimentControls for <
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator<(incrimentControl& ic1, incrimentControl& ic2);
/*
* Function: operator<
* Purpose: Compares an incrimentControl to an int for <
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator<(incrimentControl& ic, int i);
/*
* Function: operator<
* Purpose:
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator<(incrimentControl& ic, float f);
/*
* Function: operator<
* Purpose: Compares an incrimentControl to an int for <
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<(int i, incrimentControl& ic);
/*
* Function: operator<
* Purpose:
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<(float f, incrimentControl& ic);



/*
* Function: operator>=
* Purpose:  Compares two incrimentControls for >=
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator>=(incrimentControl& ic1, incrimentControl& ic2);
/*
* Function:operator>=
* Purpose: Compares an incrimentControl to an int for >=
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(incrimentControl& ic, int i);
/*
* Function: operator>=
* Purpose:
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(incrimentControl& ic, float f);
/*
* Function: operator>=
* Purpose: Compares an incrimentControl to an int for >=
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(int i, incrimentControl& ic);
/*
* Function: operator>=
* Purpose:
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator>=(float f, incrimentControl& ic);


/*
* Function: operator<=
* Purpose: Compares two incrimentControls for <=
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - the results of the comparison
*/
bool operator<=(incrimentControl& ic1, incrimentControl& ic2);
/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(incrimentControl& ic, int i);
/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(incrimentControl& ic, float f);
/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(int i, incrimentControl& ic);
/*
* Function: operator<=
* Purpose: Compares an incrimentControl to an int for <=
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - the results of the comparison
*/
bool operator<=(float f, incrimentControl& ic);



/*
* Function: operator==
* Purpose: Compares two incrimentControls for equality
* Parameters: incrimentControl& ic1 - the first incrimentControl
*				incrimentControl& ic2 - the second incrimentControl
* Returns: bool - whether the values are equal
*/
bool operator==(incrimentControl& ic1, incrimentControl& ic2);
/*
* Function: operator==
* Purpose: Compares an incrimentControl to an int for equality
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				int i - the integer to compare
* Returns: bool - whether the values are equal
*/
bool operator==(incrimentControl& ic, int i);
/*
* Function: operator==
* Purpose: Compares an incrimentControl to a float for equality
* Parameters: incrimentControl& ic - the incrimentControl to compare
*				float f - the float to compare
* Returns: bool - whether the values are equal
*/
bool operator==(incrimentControl& ic, float f);
/*
* Function: operator==
* Purpose: Compares an incrimentControl to an int for equality
* Parameters: int i - the integer to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - whether the values are equal
*/
bool operator==(int i, incrimentControl& ic);
/*
* Function: operator==
* Purpose: Compares an incrimentControl to a float
* Parameters: float f - the float to compare
*				incrimentControl& ic - the incrimentControl to compare
* Returns: bool - whether the values are equal
*/
bool operator==(float f, incrimentControl& ic);

/*
* Function: operator==
* Purpose: Compares two colors
* Parameters: D2D1_COLOR_F & c1 - the first color to compare
*				D2D1_COLOR_F & c2 - the second color to compare
* Returns: bool - whether the values are equal
*/
bool operator==(D2D1_COLOR_F& c1, D2D1_COLOR_F& c2);

typedef struct ColorEffect
{
	ID2D1SolidColorBrush* colBrush;
	D2D1_COLOR_F color;
};

/**
 * Class: TextHighlighter
 * Purpose: Helps the TTextField highlight text when the user clicks and moves
 */
class TextHighlighter
{
public:

	/*
	 * Method: TextHighlighter::TextHighlighter
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> - the drawing board to draw to
	 * Returns: New Text Highlighter object
	 */
	TextHighlighter(TrecPointer<DrawingBoard>);

	/*
	 * Method: TextHighlighter::SetLayout
	 * Purpose: Sets the layout of the text to apply the effect to
	 * Parameters: TrecComPointer<IDWriteTextLayout> l - the layout to apply effects to
	 * Returns: void
	 */
	void SetLayout(TrecComPointer<IDWriteTextLayout> l);
	/*
	 * Method: TextHighlighter::SetFirstPosition
	 * Purpose: ID's the index of the first point and sets the highlighter to active
	 * Parameters: UINT f - the location of the first point in the text
	 * Returns: void
	 */
	void SetFirstPosition(UINT);
	/*
	 * Method: TextHighlighter::SetSecondPosition
	 * Purpose: Identifies the second position the user has specified and applies the highlighing effect to the region betweent he two positions
	 * Parameters: UINT s - the location of the second point in the text
	 * Returns: void
	 */
	void SetSecondPosition(UINT);

	/*
	 * Method: TextHighlighter::Reset
	 * Purpose: Resets the highlighter
	 * Parameters: UINT location - the location where the mouse is unclicked
	 * Returns: bool - whether the highlighter was reset
	 */
	bool Reset(UINT location);
	/*
	 * Method: TextHighlighter::ResetUp
	 * Purpose: Resets the highlighter if necessary
	 * Parameters: UINT location - the location where the mouse is unclicked
	 * Returns: void
	 */
	void ResetUp(UINT location);
	/*
	 * Method: TextHighlighter::IsActive
	 * Purpose: Reports whether the highlighter is looking for the second position
	 * Parameters: void
	 * Returns: bool - whether the highlighter is actively looking for the second position
	 */
	bool IsActive();
private:
	/**
	 * the boundaries between the highlighter in the text
	 */
	UINT beginningPosition, endingPosition;
	/**
	 * Whether the first position is smller than the second position
	 */
	bool beginningIsInitial;

	/**
	 * whether it is ctively looking for a second position
	 */
	bool isActive;

	/**
	 * THe Drawing Board
	 */
	TrecPointer<DrawingBoard> renderer;
	/**
	 * Not sure if this is used
	 */
	TrecPointer<TBrush> brush;
	/**
	 * the layout with the text
	 */
	TrecComPointer<IDWriteTextLayout> layout;
};


/*
* Class: TTextField
* Purpose: Supports Anaface's ability to have more complex text layouts, including
*	Password boxes, number fields, or complex formatted strings.
* SuperClass: TGadgetControl - allows the combobox to have a feture within it
*/
class _ANAFACE_DLL TTextField :	public TGadgetControl
{
	friend class AnafaceParser;
public:
	/*
	* Method: TTextField::TTextField
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> rt - The Render Target to draw to
	*				TrecPointer<TArray<styleTable>> ta - The Style Table to draw from
	*				HWND winHand - the handle to the window so Windows Caret Support is possible
	* Returns: New TTextField
	*/
	TTextField(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta, HWND);
	/*
	* Method: TTextField::~TTextField
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TTextField();


	/*
	* Method: TTextField::onClick
	* Purpose: Checks to see if a Point falls within it's area space
	* Parameters: CPoint cp - the point to check
	* Returns: bool - whether the point is present or not
	* Note: Check to see if this Method is used and whether it should be depreciated
	*/
	bool onClick(TPoint);

	/*
	 * Method: TTextField::LockText
	 * Purpose: Prevents the User from editing the control
	 * Parameters: void
	 * Returns: void
	 */
	void LockText();
	/*
	 * Method: TTextField::UnlockText
	 * Purpose: Allows the User to edit the control
	 * Parameters: void
	 * Returns: void
	 */
	void UnlockText();


	/*
	* Method: TTextField::loadFromHTML
	* Purpose: Loads the Text Element from HTML
	* Parameters: CArchive * ar - File to read from
	* Returns: int
	* Note: DEPRECIATED - Functionality should be handled by a compatible Anaface-HTML parser
	*/
	virtual int loadFromHTML(TFile* ar) override;
	/*
	* Method: TTextField::storeInTML
	* Purpose: Stores the control in a TML file
	* Parameters: CArchive* ar - the file to save to
	*				int childLevel - the generation if the TControl
	*				bool overrideChildren - whether to ignore the children to save
	* Returns: void
	*/
	virtual void storeInTML(TFile* ar, int childLevel, bool ov = false) override;
	/*
	* Method: TTextField::storeInHTML
	* Purpose: Stores the control in an HTML format
	* Parameters: CArchive * ar - the file to write to
	* Returns: void
	*/
	virtual void storeInHTML(TFile* ar) override;

	/*
	* Method: TTextField::onCreate
	* Purpose: Sets up the TTextFeild with Text Specific attributes
	* Parameters: RECT r - the location that the control would work in
	* Returns: bool - success (currently arbitrarily)
	*/
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d) override;

	/*
	* Method: TTextField::onDraw
	* Purpose: Draws the text that it was given
	* Parameters: void
	* Returns: void
	*/
	virtual void onDraw(TObject* obj = nullptr) override;
	/*
	* Method: TTextField::determineMinHeightNeeded
	* Purpose: Determines how high the control HAS to be that it was still functional
	* Parameters: void
	* Returns: UINT - The minimum height control needed
	*/
	UINT determineMinHeightNeeded()override;
	/*
	* Method: TTextField::SetNewLocation
	* Purpose: Sets a new location for the Control
	* Parameters: RECT& r - the new location
	* Returns: void
	*/
	void SetNewLocation(const D2D1_RECT_F& r)override;

	/*
	* Method: TTextField::OnLButtonDown
	* Purpose: Determines if a mouse click occured and where it should put the caret
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				CPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)override;
	/*
	* Method: TTextField::OnChar
	* Purpose: Adds a character to the String
	* Parameters: bool fromChar - can be called either from on Key Down or OnChar
	*				UINT nChar - The ID of the character that was pressed
	*				UINT nRepCnt - how many times the character was processed for this event
	*				UINT nFlags - flags provided by MFC's Message system, not used
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;
	/*
	* Method: TTextField::OnMouseMove
	* Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it, used by Text field to oversee the selection of text
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	* Returns: void
	*/
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)override;
	/*
	* Method: TTextField::OnLButtonUp
	* Purpose: Responds to mouse clicks ending
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				CPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	/*
	* Method: TTextField::AppendBoldText
	* Purpose: Adds Bold Text at the end of the text
	* Parameters: TString& t - the text to add
	* Returns: void
	*/
	void AppendBoldText(const TString& t);
	/*
	* Method: TTextField::AppendItalicText
	* Purpose: Adds Italic text at the end of the text
	* Parameters: TString& t - the text to add
	* Returns: void
	*/
	void AppendItalicText(const TString& t);
	/*
	* Method: TTextField::AppendBoldItalicText
	* Purpose: Adds Italic and Bold Text to the end
	* Parameters: TString& t - the text to add
	* Returns: void
	*/
	void AppendBoldItalicText(const TString& t);
	/*
	* Method: TTextField::AppendNormalText
	* Purpose: Adds Normal Text to the end of the current text
	* Parameters: TString& t - the text to add
	* Returns: void
	*/
	void AppendNormalText(const TString& t);

	/*
	* Method: TTextField::isOnFocus
	* Purpose: Reports whether focus is on the current control
	* Parameters: void
	* Returns: bool - whether focus is on the control
	*/
	bool isOnFocus();
	/*
	* Method: TTextField::GetText
	* Purpose: Retrieves the current text of the control
	* Parameters: void
	* Returns: TString - the current text held by the control
	*/
	TString GetText();
	/*
	* Method: TTextField::SetText
	* Purpose: Sets the current text of the control
	* Parameters: TString t - the string to set the text to
	* Returns: void
	*/
	void SetText(TString);
	virtual UCHAR* GetAnaGameType()override;

	/*
	 * Method: TTextField::Resize
	 * Purpose: resets the Size of the TextField
	 * Parameters: D2D1_RECT_F& r - the location to set the text field to
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r) override;


	/*
	* Method: TTextField::setNumericText
	* Purpose: Sets the TextField to an integer value
	* Parameters: int i - the number to set control to
	* Returns: void
	*/
	void setNumericText(int);

	/*
	* Method: TTextField::setNumericText
	* Purpose: Sets the TextField to a floating point value
	* Parameters: float f - the number to set control to
	* Returns: void
	*/
	void setNumericText(float);

	/*
	 * Method: TTextField::AddColorEffect
	 * Purpose: Adds a color effect to the text presented
	 * Parameters: D2D1_COLOR_F col - the color to set the text
	 *				UINT start - the starting index to apply the color
	 *				UINT length - the number of characters to apply the color to
	 * Returns: void
	 */
	void AddColorEffect(D2D1_COLOR_F col, UINT start, UINT length);
protected:
	TextHighlighter highlighter;

	/*
	 * Method: TTextField::RemoveFocus
	 * Purpose: Removes the User focus from this control
	 * Parameters: void
	 * Returns: void
	 */
	static void RemoveFocus();


	/**
	 * whether the control should hide the text and (if so) offer a peek button
	 */
	bool isPassword, offerPasswordPeek;
	/**
	 * whether the user can edit the text in the field
	 */
	bool isEditable;
	/**
	 * The text being held
	 */
	TString text;
	/**
	 * The location of the caret in the text
	 */
	int caretLoc;
	/**
	 * whether the focus is on this particular Text Field
	 */
	bool onFocus;
	/**
	 * Wheter the text should be a number and if numberboxes should be drawn
	 */
	bool isNumber, drawNumBoxes;
	/**
	 * the Handle to the window (for caret manipulation)
	 */
	HWND windowHandle;
	/**
	 * provides info on how much the number boxes should increase/decrease the value by
	 */
	incrimentControl incriment;

	// extra resources

	/**
	 * ocation to draw the password peek button
	 */
	D2D1_ELLIPSE passwordPeek_outer, passwordPeek_inner;
	/**
	 * whether the password should be shown
	 */
	bool showPassword;
	/**
	 * center of the circle
	 */
	TPoint circleCenter;
	/**
	 * the radius of the pek button
	 */
	int radius;

	/**
	 * location of the bottom button (decrement button)
	 */
	D2D1_RECT_F botBut;
	/**
	 * location of the top button (increment button)
	 */
	D2D1_RECT_F topBut;

	/*
	* Method: TTextField::updateNumber
	* Purpose: Updates the Number string by a predefined incriment
	* Parameters: bool pos - whether moving the number up or down
	* Returns: void
	*/
	void updateNumber(bool);
	/*
	* Method: TTextField::setToZero
	* Purpose: Sets a number oriented TextField to 0
	* Parameters: void
	* Returns: void
	*/
	void setToZero();

	/*
	* Method: TTextField::InputChar
	* Purpose: Adds key stroke to the appropriate location in the string
	* Parameters: wchar_t cha - the character to input
	*				int times - the number of times to insert it
	* Returns: void
	*/
	virtual void InputChar(wchar_t, int);
	/*
	* Method: TTextField::updateTextString
	* Purpose: Refreshes the Text string formating
	* Parameters: void
	* Returns: void
	*/
	void updateTextString();
	/*
	* Method: TTextField::moveCaretLeft
	* Purpose: Shifts the caret to the left in the text
	* Parameters: CPoint point - the point to test
	* Returns:void
	*/
	void moveCaretLeft(POINT point);
	/*
	* Method: TTextField::moveCaretRight
	* Purpose: Shifts the Caret to the right in the text
	* Parameters: CPoint point - the point to test
	* Returns: void
	*/
	void moveCaretRight(POINT point);
	/*
	* Method: TTextField::moveCaretUp
	* Purpose: Shifts the Caret to the up in the text
	* Parameters: CPoint point - the point to test
	* Returns: void
	*/
	void moveCaretUp(POINT point);
	/*
	* Method: TTextField::moveCaretDown
	* Purpose: Shifts the Caret to the down in the text
	* Parameters: CPoint point - the point to test
	* Returns: void
	*/
	void moveCaretDown(POINT point);

	// For Numbers, handle minumum and maximum

	/**
	 * Has a minimum value or a maximum
	 */
	bool hasMin, hasMax;
	/**
	 * Holds the minimum and maximum values that a number-based field can hold
	 */
	incrimentControl minumum, maximum;

	/**
	 * List for details such as weight
	 */
	TDataArray<formattingDetails> details;
	/**
	 * List of colors affecting the text
	 */
	TDataArray<ColorEffect> colors;
};

