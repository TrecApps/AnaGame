#pragma once
#include "TTextLayout.h"
#include <TShell.h>
class _ANAFACE_DLL2 TConsoleText :
    public TTextLayout
{
	friend class TConsoleInputTextInterceptor;
	friend class TConsoleTextHolder;
protected:
    TShell shell;

	TString input;

    bool isInput;
	bool processRunning;
    TrecPointer<TTextIntercepter> inputIntercepter;

	/**
	 * Method: TConsoleText::SubmitInput
	 * Purpose: Attempts to Submit input to the shell
	 * Parameters: void
	 * Returns: bool - whether input was submitted or not
	 */
	bool SubmitInput();


	/**
	 * Method: TConsoleText::ProcessShellOutput
	 * Purpose: Transfers output to the Log
	 * Parameters: TString& output - the output from the Command to print out
	 * Returns: void
	 */
	void ProcessShellOutput(TString& output);

	TrecPointer<TEnvironment> env;

public:

	/**
	 * Method: TConsoleLayout::GetConsoleHolder
	 * Purpose: Allows Control to return a Console Holder allowing input to be sent
	 * Parameters: void
	 * Returns: TrecPointer<TConsoleHolder> - the holder holding this object
	 */
	TrecPointer<TConsoleHolder> GetConsoleHolder();

	/**
	 * Method: TConsoleText::SetDirectory
	 * Purpose: Sets the current Working Directory of the Control
	 * Parameters: TrecPointer<TFileShell> directory - the directory to send
	 * Returns: void
	 *
	 * Note: if 'directory' is null or not a directory, then directory will default to the user's Documents directory
	 */
	void SetDirectory(TrecPointer<TFileShell> directory);

	/**
	 * Method: TConsoleText::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: const D2D1_RECT_F& loc- the location this control is bound by
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: virtual
	 */
	virtual bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d, TrecPointer<TFileShell> d) override;

	/**
	 * Method: TConsoleText::TConsoleText
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TConsoleText(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);

	virtual ~TConsoleText();


	/**
	 * Method: TConsoleText::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;

	/**
	 * Method: TControl::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;

};

