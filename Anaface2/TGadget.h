#pragma once
#include "TControl.h"


class _ANAFACE_DLL2 TGadget :
    public TControl
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
	TGadget(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);

	/**
	 * Method: TGadget::~TGadget
	 * Purpose: Destructor
	 * Parameters: void
	 * Return: void
	 */
	virtual ~TGadget();


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

protected:

	/**
	 * Intended size of the gadget
	 */
	int bSize;

	/**
	 * Brish to drw the inner gadget with
	 */
	TrecPointer<TBrush> brush;

	/**
	 * Thickness of the inner box lines
	 */
	float thickness;

	/**
	 * Color of the inner box
	 */
	D2D1_COLOR_F dxColor;
	/**
	 * Location of the inner box
	 */
	D2D1_RECT_F DxLocation;
};

