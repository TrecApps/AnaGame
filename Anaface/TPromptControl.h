#pragma once
#include "TTextField.h"
#include <TEnvironment.h>
#include <TShell.h>
/**
 * Class Enum: prompt_mode
 * Purpose: How to handle interactions
 */
typedef enum class prompt_mode
{
	regular,       // Behave like a regular prompt
	print_only,    // only print material through print methods
	program_input  // Accept input but only print out what a user types or a program provides
}prompt_mode;


/**
 * Class: TPromptHolder
 * Purpose: Implements the TConsoleHolder for the TPromptControl
 * 
 * SuperClass: TConsoleHolder
 */
class TPromptHolder : public TConsoleHolder
{
	friend class TPromptControl;
public:
	/**
	 * Method: TPromptHolder::Warn
	 * Purpose: A warning message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Warn(TrecPointer<TVariable> var) override;

	/**
	 * Method: TPromptHolder::Error
	 * Purpose: An error message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Error(TrecPointer<TVariable> var) override;

	/**
	 * Method: TPromptHolder::Info
	 * Purpose: An information message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Info(TrecPointer<TVariable> var) override;

	/**
	 * Method: TPromptHolder::Log
	 * Purpose: A regular message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Log(TrecPointer<TVariable> var) override;

private:

	/**
	 * Method: TPromptHolder::TPromptHolder
	 * Purpose: Allows the TPromptControl to create an instance of this holder pointing to it
	 * Parameters: TrecPointer<TControl> control - the control to point to
	 * Returns: new TPromptHolder object
	 */
	TPromptHolder(TrecPointer<TControl> control);

	/**
	 * The control to hold on to
	 */
	TrecPointer<TControl> control;
};



/**
 * Class: TPromptControl
 * Purpose: Allows Anagame to support terminals in it's interface
 * 
 * SuperClass: TTextField
 */
class _ANAFACE_DLL TPromptControl :
	public TTextField
{
public:

	/**
	 * Method: TPromptControl::GetConsoleHolder
	 * Purpose: Retrieves a console holder that points to this control
	 * Parameters: void
	 * Returns: TrecPointer<TConsoleHolder> - console holder compatible with this console object
	 */
	TrecPointer<TConsoleHolder> GetConsoleHolder();

	/**
	 * Method: TPromptControl::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

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
	* 
	* Attributes: override
	*/
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d) override;

	/*
	* Method: TPromptControl::onDraw
	* Purpose: Draws the text that it was given
	* Parameters: TObject* obj - object used for databinding (unlikely to be used here)
	* Returns: void
	* 
	* Attributes: override
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
	* 
	* Attributes: override; message
	*/
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)override;
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
	* 
	* Attributes: override; message
	*/
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

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

	/**
	 * For use with scripts and other programs
	 */

	/**
	 * Method: TPromptControl::Print
	 * Purpose: Allows external code to manually add something to print out
	 * Parameters: TString& input - the command to enter
	 * Returns: void
	 */
	void Print(const TString& input);

	/**
	 * Method: TPromptControl::PrintLine
	 * Purpose: Allows external code to manually add something to print out, adding an extra new line at the end
	 * Parameters: TString& input - the command to enter
	 * Returns: void
	 */
	void PrintLine(const TString& input);


	/**
	 * Method: TPromptControl::Clear
	 * Purpose: Allows external code to manually clear the buffer
	 * Parameters: void
	 * Returns: void
	 */
	void Clear();

	

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

	/**
	 * the mode to operate under
	 */
	prompt_mode promptMode;

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
	 * 
	 * Attributes: override
	 */
	virtual void InputChar(wchar_t, int)override;
};

