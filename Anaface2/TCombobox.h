#pragma once
#include "TGadget.h"
#include "TDataLayout.h"
#include <TContainerVariable.h>

/**
 * Class: TCombobox
 * Purpose: References a Combobox in Anagame
 */
class _ANAFACE_DLL2 TCombobox :
    public TGadget
{
	friend class TComboBoxUpdater;
public:
	/**
	 * Method: TGadget::TGadget
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - the Render Target to use
	 *				TrecPointer<TArray<styleTable>> ta - List of Styles for the TControl
	 *				bool isGadgetBase - the gadget base
	 * Return: new Gadget Control object
	 */
	TCombobox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);

	/**
	 * Method: TGadget::~TGadget
	 * Purpose: Destructor
	 * Parameters: void
	 * Return: void
	 */
	virtual ~TCombobox();


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
	 * Method: TCombobox::GetVariableList
	 * Purpose: Allows external code to access the Variable List
	 * Parameters: void
	 * Returns: TrecSubPointer<TVariable, TContainerVariable> - the list requested 
	 */
	TrecSubPointer<TVariable, TContainerVariable> GetVariableList();

	/**
	 * Method: TControl::InjectChildTemplate
	 * Purpose: Some TControls can take in a single child page as a 'template', so this method allows this to happen
	 * Parameters: TrecPointer<TPage> page - the page to inject
	 * Returns: bool - whether the control takes in a ChildTemplate Or not
	 */
	virtual bool InjectChildTemplate(TrecPointer<TPage> page);


	/**
	 * Method: TControl::SupportsChildTemplateInjection
	 * Purpose: Reports whether the Control supports TTemplate Injection before the Anaface-Page attempts to perform it
	 * Parameters: void
	 * Returns: bool - whether InjectChildTemplate could work on this control
	 */
	virtual bool SupportsChildTemplateInjection();

protected:

	void SetCurrentContent(TrecPointer<TVariable> v);

    TrecSubPointer<TPage, TDataLayout> dataLayout;

    TrecSubPointer<TVariable, TContainerVariable> vars;

    TrecPointer<TVariable> currentVariable;

	UINT childHeight;

	
};

