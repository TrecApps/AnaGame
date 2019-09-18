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
	TGadgetControl(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta, bool isGagetBase = true);
	virtual ~TGadgetControl();

	virtual void storeInTML(TFile* ar, int childLevel,bool ov = false) override;

	bool onCreate(RECT);
	virtual UCHAR* GetAnaGameType()override;

	void Resize(RECT r) override;
protected:
	int bSize;
	RECT checker;

	TrecComPointer<ID2D1SolidColorBrush> brush;

	float thickness;

	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	D2D1_RECT_F DxLocation;
	bool isGadBase;
};
