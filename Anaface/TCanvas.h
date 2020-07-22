#pragma once
#include "TControl.h"



class _ANAFACE_DLL TCanvas :	public TControl
{
public:
	TCanvas(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);
	~TCanvas();

	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	void addGeoGroup(TrecPointer<GeoGroup>);
	virtual UCHAR* GetAnaGameType()override;
private:
	TArray<GeoGroup> geometry;
};

