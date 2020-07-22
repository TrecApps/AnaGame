#include "TSliderControl.h"



TSliderControl::TSliderControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> styles) : TControl(rt,styles,false)
{
}


TSliderControl::~TSliderControl()
{
}

UCHAR * TSliderControl::GetAnaGameType()
{
	return nullptr;
}
