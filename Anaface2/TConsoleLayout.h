#pragma once
#include "TLayout.h"
#include <TShell.h>

/**
 * Class: TControlLayout
 * Purpose: Provides an Organized way of outputting Console Information
 */
class _ANAFACE_DLL2 TConsoleLayout :
    public TLayout
{
	friend class TConsoleTextInterceptor;
protected:
    TShell shell;

	bool isInput;

	bool doDraw;
	bool restrictDraw;
	bool processRunning;

	TrecPointer<TTextIntercepter> inputIntercepter;

	/**
	 * Method: TConsoleLayout::SubmitInput
	 * Purpose: Attempts to Submit input to the shell
	 * Parameters: void
	 * Returns: bool - whether input was submitted or not
	 */
	bool SubmitInput();


	/**
	 * Method: TConsoleLayout::ProcessShellOutput
	 * Purpose: Transfers output to the Log
	 * Parameters: TString& output - the output from the Command to print out
	 * Returns: void
	 */
	void ProcessShellOutput(TString& output);

public:

	/**
	 * Method: TConsoleLayout::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: const D2D1_RECT_F& loc- the location this control is bound by
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: virtual
	 */
	virtual bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d) override;

	/**
	 * Method: TConsoleLayout::TConsoleLayout
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TConsoleLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);

	virtual ~TConsoleLayout();


	/**
	 * Method: TConsoleLayout::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;

	/**
	 * Method: TConsoleLayout::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&) override;

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
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

	/**
	 * Method: TControl::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>& args) override;

};

