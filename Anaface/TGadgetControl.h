#pragma once
#include "TControl.h"


/*
* Class TGadgetControl
* Purpose: To support controls with a button within them
*/
class _ANAFACE_DLL TGadgetControl :
	public TControl
{
public:
	/*
	* Method: TGadgetControl::TGadgetControl
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> rt - the Render Target to use
	*				TrecPointer<TArray<styleTable>> ta - List of Styles for the TControl
	*				bool isGadgetBase - the gadget base
	* Return: new Gadget Control object
	*/
	TGadgetControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, bool isGagetBase = true);

	/*
	* Method: TGadgetControl::~TGadgetControl
	* Purpose: Destructor
	* Parameters: void
	* Return: void
	*/
	virtual ~TGadgetControl();

	virtual void storeInTML(TFile* ar, int childLevel,bool ov = false) override;

	/*
	* Method: TGadgetControl::onCreate
	* Purpose: To set up the Gadget Control
	* Parameters: D2D1_RECT_F r - the location control will be
	* Return: bool - false, ignore for now
	*/
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	virtual UCHAR* GetAnaGameType()override;


	/*
	 * Method: TGadgetControl::Resize
	 * Purpose: Resizes the control upon the window being resized, applies to the box inside the control
	 * Parameters: D2D1_RECT_F& r - the new location for the control
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r) override;
protected:
	/**
	 * Intended size of the gadget
	 */
	int bSize;
	/**
	 * location of the gadget in RECT form
	 */
	RECT checker;

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
	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	/**
	 * Location of the inner box
	 */
	D2D1_RECT_F DxLocation;
	/**
	 * DEPRECATED
	 */
	bool isGadBase;
};

