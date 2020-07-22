#pragma once
#include "TTextField.h"
#include <TShell.h>

/**
 * Class: TPromptControl
 * Purpose: Allows Anagame to support terminals in it's interface
 */
class TPromptControl :
	public TTextField
{
public:
	/*
	 * Method: TPromptControl::TPromptControl
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - The Render Target to draw to
	 *				TrecPointer<TArray<styleTable>> ta - The Style Table to draw from
	 *				HWND winHand - the handle to the window so Windows Caret Support is possible
	 * Returns: New Prompt Control
	 */
	TPromptControl(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta, HWND);
	/*
	 * Method: TPromptControl::~TPromptControl
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TPromptControl();

	/*
	* Method: TPromptControl::onCreate
	* Purpose: Sets up the TPromptControl with Prompt Specific attributes
	* Parameters: RECT r - the location that the control would work in
	* Returns: bool - success (currently arbitrarily)
	*/
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d) override;

	/*
	* Method: TPromptControl::onDraw
	* Purpose: Draws the text that it was given
	* Parameters: TObject* obj - object used for databinding (unlikely to be used here)
	* Returns: void
	*/
	virtual void onDraw(TObject* obj = nullptr) override;

	/*
	* Method: TTextField::OnLButtonDown
	* Purpose: Determines if a mouse click occured and where it should put the caret
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				CPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	/*
	* Method: TPromptControl::OnChar
	* Purpose: Adds a character to the String
	* Parameters: bool fromChar - can be called either from on Key Down or OnChar
	*				UINT nChar - The ID of the character that was pressed
	*				UINT nRepCnt - how many times the character was processed for this event
	*				UINT nFlags - flags provided by MFC's Message system, not used
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	*/
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/*
	 * Method: TPromptControl::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 */
	void SetText(TString);

	/*
	 * Method: TPromptControl::SubmitCommand
	 * Purpose: Allows external code (such as a handler or an environment) to manually enter a command
	 * Parameters: TString& command -  the command to enter
	 * Returns: void
	 */
	void SubmitCommand(TString& command);
protected:
	/**
	 * the Input provided by the user, seperate from the output from the control of the TShell underneath
	 */
	TString input;
	/**
	 * The object that actually processes commands and provides their output
	 */
	TShell shell;
	/**
	 * Whether a process is currently running under this control
	 */
	bool processRunning;

	/*
	 * Method: TPromptControl::isInInput
	 * Purpose: Checks whether a string index is within the input region
	 * Parameters: UINT proposeLoc - the index of the string to check
	 * Returns: bool - whether the index is within the input region or not
	 */
	bool isInInput(UINT proposeLoc);
	/*
	 * Method: TPromptControl::SubmitCommand
	 * Purpose: Submits the command entered by the user for processing
	 * Parameters: void
	 * Returns: void
	 */
	void SubmitCommand();

	/*
	 * Method: TPromptControl::InputChar
	 * Purpose: Adds a character to the input string
	 * Parameters: WCHAR ch - the character to add
	 *				int - the number of times to add that character
	 * Returns: void
	 */
	virtual void InputChar(wchar_t, int)override;
};

