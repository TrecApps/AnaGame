#include "TImage.h"

static D2D1_MATRIX_3X2_F identityMatrix = D2D1::Matrix3x2F::Identity();

TImage::TImage(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> styles): TControl(rt, styles)
{
	rotation = identityMatrix;
	currentImage = 0;
}

TImage::TImage(TImage& im) : TControl(im)
{
	rotation = identityMatrix;
	currentImage = 0;
}

TImage::TImage() :TControl()
{
	rotation = identityMatrix;
	currentImage = 0;
}

TImage::~TImage()
{
	/*for (UINT Rust = 0; Rust < images.Size(); Rust++)
	{
		TBitmap bit = images[Rust];
		if (bit.d2dBitbap) bit.d2dBitbap->Release();
		if (bit.wicBitmap) bit.wicBitmap->Release();
	}*/
}
//
//UINT TImage::onFill(TPoint& pixel, D2D1_COLOR_F& color, float tolerance)
//{
//	if(currentImage >= images.Size())
//		return 1;
//	
//	TBitmap bitmap = images[currentImage];
//	if (!bitmap.d2dBitbap || !bitmap.wicBitmap)
//		return 2;
//
//	// Create a Lock through which we can edit the bitmap
//	UINT height = 0, width = 0;
//	bitmap.wicBitmap->GetSize(&width, &height);
//
//	double resHeight = 0.0, resWidth = 0.0;
//	bitmap.wicBitmap->GetResolution(&resWidth, &resHeight);
//	
//	pixel.x -= location.left;
//	pixel.y -= location.top;
//
//	pixel.x = static_cast<double>(pixel.x) / resWidth;
//	pixel.y = static_cast<double>(pixel.y) / resHeight;
//
//	IWICBitmapLock* pLock = nullptr;
//	WICRect wicRect{ 0,0,width, height };
//
//	if (SUCCEEDED(bitmap.wicBitmap->Lock(&wicRect, WICBitmapLockRead | WICBitmapLockWrite, &pLock))) {
//		UINT stride = 0;
//		pLock->GetStride(&stride);
//		BYTE* imageData = nullptr;
//		UINT imageDataSize = 0;
//
//		UINT iColor = 0;
//		ConvertD2D1ColorToUIntColor(color, iColor);
//
//		if (SUCCEEDED(pLock->GetDataPointer(&imageDataSize, &imageData)))
//		{
//			// Prepare a marked map for marking pixels for change
//			TDataArray<TDataArray<PixelMark>> pixels;
//			
//			for (UINT Rust = 0, pixelCount = 0; pixelCount < imageDataSize; Rust++)
//			{
//				TDataArray<PixelMark> row;
//				for (UINT C = 0; pixelCount < imageDataSize && C < stride; C++, pixelCount++)
//				{
//					PixelMark pixelMark;
//					memcpy(&pixelMark.pixel, &imageData[4 * pixelCount], 4);
//					pixelMark.marker = 0; // This pixel, just created, has not been looked at by the algorithm
//
//					row.push_back(pixelMark);
//				}
//				pixels.push_back(row);
//			}
//
//			// Now determine the pixels that need to be updated
//			MarkPixels(pixel, iColor, tolerance, pixels);
//
//
//			// Now that we know which pixels to update, go through and update the pixels
//			for (UINT Rust = 0; Rust < pixels.Size(); Rust++)
//			{
//				for (UINT C = 0; C < pixels[Rust].Size(); C++)
//				{
//					auto pixelMark = pixels[Rust][C];
//					if (pixelMark.marker == 2)
//					{
//						memcpy(&imageData[4 * (Rust * stride + C)], &pixelMark.pixel, 4);
//					}
//				}
//			}
//
//			ID2D1Bitmap* releaseImage = bitmap.d2dBitbap;
//
//			renderTarget->CreateBitmapFromWicBitmap(bitmap.wicBitmap, &bitmap.d2dBitbap);
//			images[currentImage].d2dBitbap = bitmap.d2dBitbap;
//			releaseImage->Release();
//		}
//		pLock->Release();
//	}
//
//	return 0;
//}
//
//UINT TImage::setLinearGradient(TDataArray<D2D1_COLOR_F>& colors)
//{
//	if (!cover.Get())
//		cover = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, this);
//	cover->SetLinearImage(colors);
//	return 0;
//}
//
//UINT TImage::setRadialGradient(TDataArray<D2D1_COLOR_F>& colors)
//{
//	if (!cover.Get())
//		cover = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, this);
//	cover->SetRadialImage(colors);
//	return 0;
//}
//
//UINT TImage::setLinearGradient(TDataArray<D2D1_GRADIENT_STOP>& colors)
//{
//	if (!cover.Get())
//		cover = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, this);
//	cover->SetLinearImage(colors);
//	return 0;
//}
//
//UINT TImage::setRadialGradient(TDataArray<D2D1_GRADIENT_STOP>& colors)
//{
//	if (!cover.Get())
//		cover =TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, this);
//	cover->SetRadialImage(colors);
//	return 0;
//}
//
//
//UINT TImage::rotate(TPoint& point)
//{
//	TPoint center((location.left + location.right) / 2, (location.bottom + location.top) / 2);
//	double degrees = atan2(-(static_cast<double>(point.y) - center.y),
//		(static_cast<double>(point.x) - center.y));
//	rotation = D2D1::Matrix3x2F::Rotation(degrees,D2D1::Point2F(center.x, center.y));
//	return 0;
//}
//
//void TImage::onDraw(TObject* obj)
//{
//	if (!renderTarget.Get()) return;
//	renderTarget->SetTransform(rotation);
//	TControl::onDraw(obj);
//	renderTarget->SetTransform(identityMatrix);
//}
//
//
void MarkPixels(TPoint& startPoint, UINT idealPixel, float tolerance, TDataArray<TDataArray<PixelMark>>& pixels)
{
}

void ConvertD2D1ColorToUIntColor(D2D1_COLOR_F& d2dColor, UINT& uColor)
{
}
