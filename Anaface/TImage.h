#pragma once
#include "TControl.h"
#include <TFile.h>

typedef struct TBitmap {
	IWICBitmap* wicBitmap;
	ID2D1Bitmap* d2dBitbap;
}TBitmap;

typedef struct PixelMark {
	UINT pixel;
	BYTE marker;
}PixelMarker;

class TImage : public TControl
{
public:
	TImage(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> styles);
	TImage(TImage&);
	TImage();
	~TImage();

	/*UINT onFill(TPoint& pixel, D2D1_COLOR_F& color, float tolerance);
	UINT setLinearGradient(TDataArray<D2D1_COLOR_F>& colors);
	UINT setLinearGradient(TDataArray<D2D1_GRADIENT_STOP>& colors);

	UINT setRadialGradient(TDataArray<D2D1_COLOR_F>& colors);
	UINT setRadialGradient(TDataArray<D2D1_GRADIENT_STOP>& colors);

	UINT rotate(TPoint& point);
	virtual void onDraw(TObject* obj) override;

	UINT addImage(TFile& file);
	UINT addImage(TString& fileName);

protected:
	
	TrecPointer<TContent> cover;
	TDataArray<TBitmap> images;*/
	UINT currentImage;
	D2D1_MATRIX_3X2_F rotation;
};

void MarkPixels(TPoint& startPoint, UINT idealPixel, float tolerance, TDataArray<TDataArray<PixelMark>>& pixels);
void ConvertD2D1ColorToUIntColor(D2D1_COLOR_F& d2dColor, UINT& uColor);
