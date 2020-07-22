#include "TImage.h"

static D2D1_MATRIX_3X2_F identityMatrix = D2D1::Matrix3x2F::Identity();

TImage::TImage(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> styles): TControl(rt, styles, false)
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
	/*TString fileName(file.GetFilePath());
	file.Close();
	return addImage(fileName);*/
}

//UINT TImage::addImage(TString& fileName)
//{
//	IWICImagingFactory* imageFactory = NULL;
//	IWICBitmapDecoder* decoder = NULL;
//	IWICBitmapFrameDecode* framDec = NULL;
//	IWICFormatConverter* converter = NULL;
//	IWICBitmapScaler* scale = nullptr;
//	UINT frameCount = 0;
//	HRESULT result = CoCreateInstance(CLSID_WICImagingFactory,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_IWICImagingFactory,
//		(LPVOID*)& imageFactory);
//	if (!SUCCEEDED(result))
//		return 2;
//	result = imageFactory->CreateDecoderFromFilename(fileName,
//		NULL,
//		GENERIC_READ,
//		WICDecodeMetadataCacheOnLoad,
//		&decoder);
//	if (!SUCCEEDED(result))
//	{
//		imageFactory->Release();
//		return 3;
//	}
//
//	result = decoder->GetFrame(0, &framDec);
//	if (!SUCCEEDED(result))
//	{
//		imageFactory->Release();
//		decoder->Release();
//		return 4;
//	}
//
//	UINT h = 0, w = 0;
//
//	framDec->GetSize(&w, &h);
//	result = imageFactory->CreateFormatConverter(&converter);
//	if (!SUCCEEDED(result))
//	{
//		imageFactory->Release();
//		decoder->Release();
//		return 5;
//	}
//
//	result = imageFactory->CreateBitmapScaler(&scale);
//
//	if (!SUCCEEDED(result))
//	{
//		imageFactory->Release();
//		decoder->Release();
//		converter->Release();
//		return 6;
//	}
//
//
//	//TO-Do - add mechanism for image being it's own size
//	result = scale->Initialize(framDec, location.right - location.left, location.bottom - location.top,
//		WICBitmapInterpolationModeFant);
//	
//
//	if (!SUCCEEDED(result))
//	{
//		imageFactory->Release();
//		decoder->Release();
//		converter->Release();
//		return 7;
//	}
//
//	result = converter->Initialize(scale, GUID_WICPixelFormat32bppPRGBA,
//		WICBitmapDitherTypeNone, NULL, 0.0f,
//		WICBitmapPaletteTypeMedianCut);
//
//	scale->Release();
//	scale = nullptr;
//
//	if (!SUCCEEDED(result))
//	{
//		imageFactory->Release();
//		decoder->Release();
//		converter->Release();
//		return 8;
//	}
//	if (!renderTarget.get())
//	{
//		imageFactory->Release();
//		decoder->Release();
//		converter->Release();
//		return 9;
//	}
//	TBitmap theMap{ nullptr, nullptr };
//	result = renderTarget->CreateBitmapFromWicBitmap(converter, &theMap.d2dBitbap);
//	
//	HRESULT result2 = imageFactory->CreateBitmapFromSource(scale, WICBitmapCacheOnLoad, &theMap.wicBitmap);
//	
//	//decoder->GetFrameCount(&frameCount);
//	imageFactory->Release();
//	decoder->Release();
//	converter->Release();
//	if (!SUCCEEDED(result) || !SUCCEEDED(result2))
//	{
//		if (theMap.d2dBitbap)
//			theMap.d2dBitbap->Release();
//		if (theMap.wicBitmap)
//			theMap.wicBitmap->Release();
//
//		return 10;
//	}
//	images.push_back(theMap);
//	return 0;
//}


void ConvertD2D1ColorToUIntColor(D2D1_COLOR_F& d2dColor, UINT& uColor)
{
	uColor = 0;
	
	UINT tempColor = static_cast<UINT>(d2dColor.a * 255);
	uColor += (tempColor);

	tempColor = static_cast<UINT>(d2dColor.b * 255);
	uColor += (tempColor << 8);

	tempColor = static_cast<UINT>(d2dColor.g * 255);
	uColor += (tempColor << 16);

	tempColor = static_cast<UINT>(d2dColor.b * 255);
	uColor += (tempColor << 24);
}

bool ColorsMatch(UINT color1, UINT color2, float tolerance)
{
	BYTE bColor1[4], bColor2[4];
	memcpy(bColor1, &color1, sizeof(color1));
	memcpy(bColor2, &color2, sizeof(color2));

	float distance = powf(static_cast<float>(bColor1[0]) - static_cast<float>(bColor2[0]), 2) +
		powf(static_cast<float>(bColor1[1]) - static_cast<float>(bColor2[1]), 2) + 
		powf(static_cast<float>(bColor1[2]) - static_cast<float>(bColor2[2]), 2) + 
		powf(static_cast<float>(bColor1[3]) - static_cast<float>(bColor2[3]), 2);

	distance = sqrtf(distance);


	return ((distance/260100.0f) * 100.0f) <= tolerance;
}
