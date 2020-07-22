#pragma once
#include "TFlyout.h"
class _ANAFACE_DLL TContextMenu :
	public TFlyout
{
public:
	TContextMenu(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
	~TContextMenu();


	bool addOption(TrecPointer<TString>);

	void onDraw(TPoint);

private:
	TrecPointer<TWindowEngine> winEngine;
	int childHeight;
};

