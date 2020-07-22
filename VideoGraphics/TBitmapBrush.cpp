#include "TBitmapBrush.h"
#include <wincodec.h>
#include "DrawingBoard.h"
#include "TGeometry.h"

/**
 * Method: TBitmapBrush::NextFrame
 * Purpose: In a multiframe image, sets the brush to draw the next frame in a sequence
 * Parameters:void
 * Returns: void
 */
void TBitmapBrush::NextFrame()
{
	if (!valid)
		return;
	currentFrame++;
	if (currentFrame >= brushes.Size())
		currentFrame = 0;

	brush = brushes[currentFrame];
}

/**
 * Method: TBitmapBrush::PrevFrame
 * Purpose: In a multifram image, sets the brush to draw the previous frame in an image
 * Parameters: void
 * Returns: void
 */
void TBitmapBrush::PrevFrame()
{
	if (!valid)return;
	if (!currentFrame)
		currentFrame = brushes.Size() - 1;
	else
		currentFrame--;
	brush = brushes[currentFrame];
}

/**
 * Method: TBitmapBrush::IsValid
 * Purpose: Reports whether the Brush Construction worked and is ready to draw
 * Parameters: void
 * Returns: bool
 */
bool TBitmapBrush::IsValid()
{
	return valid;
}

/**
 * Method: TBitmapBrush::SetLocation
 * Purpose: Sets the location where the image will be drawn
 * Parameters: RECT_2D& loc - the location on the board to draw the image
 * Returns: void
 */
void TBitmapBrush::SetLocation(RECT_2D& loc)
{
	location = loc;
	RefreshBrush();
}

/**
 * Method: TBitmapBrush::GetCurrentFrame
 * Purpose: Returns the index of the current frame to be drawn
 * Parameters: void
 * Returns: UINT - the index of the current frame
 */
UINT TBitmapBrush::GetCurrentFrame()
{
	return currentFrame;
}

/**
 * Method: TBitmapBrush::GetFrameCount
 * Purpose: Retrieves the total number of frames in the image
 * Parameters: void
 * Returns: UINT - the total frame count
 */
UINT TBitmapBrush::GetFrameCount()
{
	return bitmaps.Size();
}

/**
 * Method: TBitmapBrush::~TBitmapBrush
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TBitmapBrush::~TBitmapBrush()
{
	SAFE_RELEASE(imageFactory)
	SAFE_RELEASE(decoder)
	SAFE_RELEASE(frameDec)
	SAFE_RELEASE(converter)

	brushes.RemoveAll();
	bitmaps.RemoveAll();
}

/**
 * Method: TBitmapBrush::FillRectangle
 * Purpose: Fills the Rectangle with the currently selected image
 * Parameters: const RECT_2D& r - the rectangle to apply the brush to
 * Returns: void
 */
void TBitmapBrush::FillRectangle(const RECT_2D& r)
{
	if (Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get() 
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), r);
		//currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), r);
	}
}

/**
 * Method: TBitmapBrush::FillRoundedRectangle
 * Purpose:  Fills the Rounded Rectangle with the currently selected image
 * Parameters: const ROUNDED_RECT_2D& r - the rounded rectangle to apply the brush to
 * Returns: void
 */
void TBitmapBrush::FillRoundedRectangle(const ROUNDED_RECT_2D& r)
{
	if (Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get()
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		if (!board->AddLayer(r))
			return;
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), r.rect);
		board->PopLayer();
	}
}

/**
 * Method: TBitmapBrush::FillEllipse
 * Purpose: Fills the Ellipse with the currently selected image
 * Parameters: const ELLIPSE_2D& r - the Ellipse to apply the brush to
 * Returns: void
 */
void TBitmapBrush::FillEllipse(const ELLIPSE_2D& r)
{
	if (Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get()
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		if(!board->AddLayer(r))
			return;
		RECT_2D rect = {
			r.point.x - r.radiusX,
			r.point.y - r.radiusY,
			r.point.x + r.radiusX,
			r.point.y + r.radiusY
		};
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), rect);
		board->PopLayer();
	}
}

/**
 * Method: TBitmapBrush::FillGeometry
 * Purpose: Fills the Geometry with the currently selected image
 * Parameters: TrecPointer<TGeometry> geo - the Geometry to apply the brush to
 * Returns:void
 */
void TBitmapBrush::FillGeometry(TrecPointer<TGeometry> geo)
{
	if (geo.Get() && Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get()
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		RECT_2D rect;
		if (!geo->GetBounds(rect))
			return;
		if (!board->AddLayer(geo))
			return;
	
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), rect);
		board->PopLayer();
	}
}


/**
 * Method: TBitmapBrush::TBitmapBrush
 * Purpose: Cretes a TBitmap Brush
 * Parameters: TrecPointer<TFileShell> picture - the location of the image file
 *				TrecPointer<DrawingBoard> rt - the Drawing Board crating the Brush
 *				RECT_2D& loc - the initial location to draw the image
 * Returns: New Bitmap Brush Object
 */
TBitmapBrush::TBitmapBrush(TrecPointer<TFileShell> picture, TrecPointer<DrawingBoard> rt, RECT_2D& loc): TBrush(rt)
{
	valid = false;

	brushType = brush_type::brush_type_bitmap;

	if (!picture.Get())
		return;

	imageFactory = NULL;
	decoder = NULL;
	frameDec = NULL;
	converter = NULL;
	IWICBitmapScaler* scale = nullptr;
	UINT frameCount = 0;
	HRESULT result = CoCreateInstance(CLSID_WICImagingFactory1,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&imageFactory);
	if (!SUCCEEDED(result))
		return;

	result = imageFactory->CreateDecoderFromFilename(picture->GetPath().GetConstantBuffer(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder);


	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		return;
	}

	result = imageFactory->CreateFormatConverter(&converter);
	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		return;
	}

	result = imageFactory->CreateBitmapScaler(&scale);

	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		converter->Release();
		return;
	}


	decoder->GetFrameCount(&frameCount);
	for (UINT Rust = 0; Rust < frameCount; Rust++)
	{
		if (!SUCCEEDED(decoder->GetFrame(Rust, &frameDec)))
			break;
		UINT h = 0, w = 0;

		frameDec->GetSize(&w, &h);

		result = scale->Initialize(frameDec, loc.right - loc.left, loc.bottom - loc.top,
			WICBitmapInterpolationModeFant);

		if (SUCCEEDED(result))
		{
			converter->Initialize(scale, GUID_WICPixelFormat32bppPRGBA,
				WICBitmapDitherTypeNone, NULL, 0.0f,
				WICBitmapPaletteTypeMedianCut);
			TrecComPointer<ID2D1Bitmap>::TrecComHolder map;
			rt->GetRenderer()->CreateBitmapFromWicBitmap(converter, map.GetPointerAddress());
			TrecComPointer<ID2D1Bitmap> bitmap = map.Extract();

			if (bitmap.Get())
			{
				bitmaps.push_back(bitmap);
				TrecComPointer<ID2D1BitmapBrush>::TrecComHolder bitHolder;
				rt->GetRenderer()->CreateBitmapBrush(bitmap.Get(), bitHolder.GetPointerAddress());
				brushes.push_back(TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1BitmapBrush>(bitHolder));
			}
		}
		scale->Release();
		converter->Release();
		imageFactory->CreateFormatConverter(&converter);
		imageFactory->CreateBitmapScaler(&scale);
	}
	converter->Release();
	converter = nullptr;
	scale->Release();
	if (brushes.Size())
	{
		brush = brushes[currentFrame = 0];
		valid = true;
	}
	location = loc;
}

/**
 * Method: TBitmapBrush::RefreshBrush
 * Purpose: Makes sure that the Brush is compatible with the Render Target, as Direct2D demands that a new Brush be created when
 *		a new Render Target is used
 * Parameters: void
 * Returns: void
 */
void TBitmapBrush::RefreshBrush()
{
	brushes.RemoveAll();
	bitmaps.RemoveAll();

	if (!valid)
		return;
	HRESULT result;
	IWICBitmapScaler* scale = nullptr;
	result = imageFactory->CreateBitmapScaler(&scale);
	UINT frameCount = 0;
	decoder->GetFrameCount(&frameCount);

	imageFactory->CreateFormatConverter(&converter);

	for (UINT Rust = 0; Rust < frameCount && SUCCEEDED(decoder->GetFrame(Rust, &frameDec)); Rust++)
	{
		UINT h = 0, w = 0;

		frameDec->GetSize(&w, &h);

		result = scale->Initialize(frameDec, location.right - location.left, location.bottom - location.top,
			WICBitmapInterpolationModeFant);

		if (SUCCEEDED(result))
		{
			converter->Initialize(scale, GUID_WICPixelFormat32bppPRGBA,
				WICBitmapDitherTypeNone, NULL, 0.0f,
				WICBitmapPaletteTypeMedianCut);
			TrecComPointer<ID2D1Bitmap>::TrecComHolder map;
			currentRenderer->CreateBitmapFromWicBitmap(converter, map.GetPointerAddress());
			TrecComPointer<ID2D1Bitmap> bitmap = map.Extract();

			if (bitmap.Get())
			{
				bitmaps.push_back(bitmap);
				TrecComPointer<ID2D1BitmapBrush>::TrecComHolder bitHolder;
				currentRenderer->CreateBitmapBrush(bitmap.Get(), bitHolder.GetPointerAddress());
				brushes.push_back(TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1BitmapBrush>(bitHolder));
			}
		}
		converter->Release();
		scale->Release();
		imageFactory->CreateBitmapScaler(&scale);
		imageFactory->CreateFormatConverter(&converter);
	}
	converter->Release();
	scale->Release();
	if (currentFrame < brushes.Size())
	{
		brush = brushes[currentFrame];
		valid = true;
	}
}
