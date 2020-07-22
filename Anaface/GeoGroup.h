#pragma once
#include <d2d1.h>
#include "Anaface.h"
#include <TMap.h>
#include <TString.h>

#include "TPathGeometry.h"
#include "TLineGeometry.h"
#include "TEllipseGeometry.h"
#include "TRectangleGeometry.h"
#include "TCurveGeometry.h"

struct BrushHolder;

class _ANAFACE_DLL GeoGroup
{
public:
	GeoGroup(ID2D1RenderTarget* rt);
	virtual ~GeoGroup();

	bool onCreate(D2D1_RECT_F);
	void onDraw();
	bool addAttribute(WCHAR* attr, TrecPointer<TString> value);

	void SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget> rt);

	void addGeometry(TrecPointer<TDrawGeometry>);

private:
	TrecComPointer<ID2D1RenderTarget> renderTarget;
	D2D1_COLOR_F color1, color2;

	void ResetBrush();

	TMap<TString> attributes;
	TArray<TDrawGeometry> children;
	TrecComPointer<ID2D1Brush> brush;
	bool secondColor;
	D2D1_GRADIENT_STOP gradients[2];
	bool useRadial;
	TrecComPointer<ID2D1GradientStopCollection> stopColl;
	bool crunch;
	D2D1_RECT_F loc;
};

