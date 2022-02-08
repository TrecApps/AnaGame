#pragma once
#include "TCombobox.h"
#include "TTextInput.h"

/**
 * Class: TTextCombobox
 * Purpose: Extends the Combo-Box by adding Text Input Support
 */
class _ANAFACE_DLL2 TTextCombobox :
    public TCombobox
{
	friend class TTextComboElement;
public:

	/**
	 * Method: TTextCombobox::TTextCombobox
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - the Render Target to use
	 *				TrecPointer<TArray<styleTable>> ta - List of Styles for the TControl
	 *				bool isGadgetBase - the gadget base
	 * Return: new Gadget Control object
	 */
	TTextCombobox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, HWND win);

	/**
	 * Method: TTextCombobox::~TTextCombobox
	 * Purpose: Destructor
	 * Parameters: void
	 * Return: void
	 */
	virtual ~TTextCombobox();


	/**
	 * Method: TTextCombobox::onCreate
	 * Purpose: To set up the Gadget Control
	 * Parameters: D2D1_RECT_F r - the location control will be
	 * Return: bool - false, ignore for now
	 *
	 * Attributes: override
	 */
	bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d, TrecPointer<TFileShell> d)override;


	/**
	 * Method: TTextCombobox::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;


	/**
	 * Method: TTextCombobox::OnLButtonUp
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
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


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

protected:

	/**
	 * Method: TTextCombobox::SetUpTextElement
	 * Purpose: Sets up Text Elements in case Some attributes support it
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: virtual - allows sub classes to create more complex Text Elements than what the standard TControl uses
	 */
	virtual void SetUpTextElement() override;

	void UpdateText(const TString& str);

	HWND window;
};

