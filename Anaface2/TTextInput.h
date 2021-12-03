#pragma once
#include "TGadget.h"

typedef union number_type_control
{
	int i;
	float f;
};

class TextIncrementControl
{
public:
	TextIncrementControl();

	number_type_control currentVal;
	number_type_control increment;
	bool useFloat;

	void Increment();
	void Decrement();

	TString GetString();

};


class _ANAFACE_DLL2 TTextInput :
    public TGadget
{
public:

	/**
	 * Method: TGadget::TGadget
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - the Render Target to use
	 *				TrecPointer<TArray<styleTable>> ta - List of Styles for the TControl
	 *				bool isGadgetBase - the gadget base
	 * Return: new Gadget Control object
	 */
	TTextInput(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, HWND win);

	/**
	 * Method: TGadget::~TGadget
	 * Purpose: Destructor
	 * Parameters: void
	 * Return: void
	 */
	virtual ~TTextInput();


	/**
	 * Method: TGadget::onCreate
	 * Purpose: To set up the Gadget Control
	 * Parameters: D2D1_RECT_F r - the location control will be
	 * Return: bool - false, ignore for now
	 *
	 * Attributes: override
	 */
	bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)override;


	/**
	 * Method: TControl::Draw
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
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& args) override;


	/**
	 * Method: TControl::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TControl::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;

	bool SetNumericText(float f);

	void SetText(const TString& text);

	TString GetText();

	void LockText(bool doLock);

	void SetPasswordMode(bool b);


protected:


	/**
	 * Method: TControl::SetUpTextElement
	 * Purpose: Sets up Text Elements in case Some attributes support it
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: virtual - allows sub classes to create more complex Text Elements than what the standard TControl uses
	 */
	virtual void SetUpTextElement() override;

	bool useNumber, usePassword, passwordPeek, editEnabled, useNumBoxes, showPassword;

	bool textLock;

	TextIncrementControl numberControl;

	HWND windowHandle;
	/**
	 * ocation to draw the password peek button
	 */
	D2D1_ELLIPSE passwordPeek_outer, passwordPeek_inner;

	/**
	 * location of the bottom button (decrement button)
	 */
	D2D1_RECT_F botBut;
	/**
	 * location of the top button (increment button)
	 */
	D2D1_RECT_F topBut;
};

