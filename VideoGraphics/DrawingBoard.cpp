#include "DrawingBoard.h"
#include "TGeometry.h"


/**
 * Method: DrawingBoard::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString DrawingBoard::GetType()
{
	return TString(L"DrawingBoard;") + TObject::GetType();
}

/**
 * Method: DrawingBoard::DrawingBoard
 * Purpose: Constructor that sets the mode to 2D
 * Parameters: TrecComPointer<ID2D1Factory1> fact - the Direct2D Factory associated with the intance
 *				 HWND window -  the Window handle associated with the window the Drawing board is attaching to
 * Returns: New DrawingBoard Object set for regular 2D presentation
 *
 * Note: The DrawingBoard produces will not work for 3D drawings. To configure it to work with 3D
 *	simply call the "Set3D" method
 */
DrawingBoard::DrawingBoard(TrecComPointer<ID2D1Factory1> fact, HWND window)
{
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";
	this->fact = fact;

	ZeroMemory(&r, sizeof(r));

	RECT area;

	usePrimaryDc = true;
	GetClientRect(window, &area);

	Resize(window, area, TrecComPointer<IDXGISurface1>());

	this->window = window;

	SetDefaultColor(D2D1::ColorF::ColorF(0.15f, 0.0f, 0.0f));
	SetDefaultTextColor(D2D1::ColorF::ColorF(1.0f, 0.9f, 0.9f));
}

DrawingBoard::~DrawingBoard()
{

}




void DrawingBoard::Resize(HWND window, RECT size, TrecComPointer<IDXGISurface1> surface)
{

	r = size;

	surface3D = surface;

	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_PREMULTIPLIED);

	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	TrecComPointer<ID2D1BitmapRenderTarget>::TrecComHolder bitHolder;

	if (surface3D.Get())
	{
		windowTarget.Delete();
		dxgiTarget.Delete();

		TrecComPointer<ID2D1RenderTarget>::TrecComHolder renderTargetHolder;

		fact->CreateDxgiSurfaceRenderTarget(surface3D.Get(), props, renderTargetHolder.GetPointerAddress());

		dxgiTarget = renderTargetHolder.Extract();

		dxgiTarget->CreateCompatibleRenderTarget(bitHolder.GetPointerAddress());

		renderer = bitHolder.Extract();

		dxgiTarget->CreateCompatibleRenderTarget(bitHolder.GetPointerAddress());

		renderer2 = bitHolder.Extract();
	}
	else
	{
		windowTarget.Delete();
		TrecComPointer<ID2D1HwndRenderTarget>::TrecComHolder renderTargetHolder;

		D2D1_HWND_RENDER_TARGET_PROPERTIES hProps;
		hProps.hwnd = window;
		hProps.pixelSize = D2D1::SizeU(size.right - size.left, size.bottom - size.top);
		hProps.presentOptions = D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS;

		fact->CreateHwndRenderTarget(props, hProps, renderTargetHolder.GetPointerAddress());

		windowTarget = renderTargetHolder.Extract();

		windowTarget->CreateCompatibleRenderTarget(bitHolder.GetPointerAddress());

		renderer = bitHolder.Extract();

		windowTarget->CreateCompatibleRenderTarget(bitHolder.GetPointerAddress());

		renderer2 = bitHolder.Extract();
	}

}

/**
 * Method: DrawingBoard::GetBrush
 * Purpose: Produces a Solid Color-enabled TBrush
 * Parameters: const TColor& col - the color to set the brush to
 * Returns: TrecPointer<TBrush> - the TBrush with solid-color enabled
 */
TrecPointer<TBrush> DrawingBoard::GetBrush(const TColor& col)
{
	return TrecPointerKey::GetNewTrecPointer<TBrush>(col, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self));
}


/**
 * Method: DrawingBoard::GetBrush
 * Purpose: Produces a radial-gradient enabled TBrush
 * Parameters: const TGradientStopCollection& coll - the collection of colors to work with
 *				const POINT_2D& p1 -Center of the gradient ellipse
 *				const POINT_2D& p2 - offset of the gradient origin
 *				float x - x-radius of the ellipse center
 *				float y - y-radius of the ellipse center
 * Returns: TrecPointer<TBrush> - the TBrush holding a Radial gradient brush
 */
TrecPointer<TBrush> DrawingBoard::GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2, float x, float y)
{
	return TrecPointerKey::GetNewTrecPointer<TBrush>(coll, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self), p1, p2, x, y);
}

/**
 * Method: DrawingBoard::GetBrush
 * Purpose: Produces a linear-gradient enabled TBrush
 * Parameters: const TGradientStopCollection& coll - the collection of colors to work with
 *				const POINT_2D& p1 - one point the line is expected to pass through
 *				const POINT_2D& p2 - second point line is expected to pass through
 * Returns: TrecPointer<TBrush> - the TBrush holding a Linear gradient brush
 */
TrecPointer<TBrush> DrawingBoard::GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2)
{
	return TrecPointerKey::GetNewTrecPointer<TBrush>(coll, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self), p1, p2);
}

/**
 * Method: DrawingBoard::GetBrush
 * Purpose: Produces a Brush containing an image and capable of drawing an image
 * Parameters: TrecPointer<TFileShell> picture - Pointer to the file to get the picture from
 *				RECT_2D& loc - the location where the picture is expected to be drawn
 * Returns: TrecSubPointer<TBrush, TBitmapBrush> - the Bursh containing the image (or null if failure occurs)
 */
TrecSubPointer<TBrush, TBitmapBrush> DrawingBoard::GetBrush(TrecPointer<TFileShell> picture, RECT_2D& loc)
{
	return TrecPointerKey::GetNewTrecSubPointer<TBrush, TBitmapBrush>(picture, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self), loc);
}

/**
 * Method: DrawingBoard::GetRenderer
 * Purpose: Returns the Raw Render Object in case direct operations need to be performed on it
 * Parameters: void
 * Returns: TrecComPointer<ID2D1RenderTarget> - the underlying Render Target
 */
TrecComPointer<ID2D1BitmapRenderTarget> DrawingBoard::GetRenderer()
{
	return (usePrimaryDc) ? renderer : renderer2;
}


/**
 * Method: DrawingBoard::SetSelf
 * Purpose: Enables a reference to itself so that when Brushes are created, they have a reference to their creator
 * Parameters: TrecPointer<DrawingBoard> self -  the Parameter holding a reference to this object
 * Returns: void
 */
void DrawingBoard::SetSelf(TrecPointer<DrawingBoard> self)
{
	if (this != self.Get())
		throw L"ERROR! Self needed to be a real reference to this object";

	this->self = TrecPointerKey::GetSoftPointerFromTrec<DrawingBoard>(self);
	defaultBrush = TrecPointerKey::GetNewTrecPointer<TBrush>(defaultColor, self);
}


/**
 * Method: DrawingBoard::SetTransform
 * Purpose: Sets the transform of the underlying RenderTarget. Enables the rotation, translation, skewing, and sizing of various images
 * Parameters: const TRANSFORM_2D& matrix
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::SetTransform(const TRANSFORM_2D& matrix)
{
	if (!renderer.Get() || renderer2.Get())
		return false;

	renderer->SetTransform(matrix);
	renderer2->SetTransform(matrix);
	return true;
}

/**
 * Method: DrawingBoard::GetTransform
 * Purpose: Retrieves the current transform matrix of the underlying RenderTarget
 * Parameters: TRANSFORM_2D& matrix
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::GetTransform(TRANSFORM_2D& matrix)
{
	if (!renderer.Get())
		return false;

	renderer->GetTransform(&matrix);
	return true;
}

/**
 * Method: DrawingBoard::PopLayer
 * Purpose: Removes a layer from the Render Target
 * Parameters: void
 * Returns: void
 */
void DrawingBoard::PopLayer()
{
	if (renderer.Get() && renderer2.Get() && layers.Size() && geometries.Size())
	{
		renderer->PopLayer();
		renderer2->PopLayer();
		layers.RemoveAt(layers.Size() - 1);
		layers2.RemoveAt(layers2.Size() - 1);
		geometries.RemoveAt(geometries.Size() - 1);
	}
}

/**
 * Method: DrawingBoard::AddLayer
 * Purpose: Adds a layer so that contents drawn withing the Rectangle are presented
 * Parameters: const RECT_2D& ret
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::AddLayer(const RECT_2D& ret)
{
	if (renderer.Get())
	{
		TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, ret);
		TrecComPointer<ID2D1Layer>::TrecComHolder layerHolder;

		// Apply to Primary Target
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();
		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers.push_back(layer);

		// Apply to Secondary Target
		renderer2->CreateLayer(layerHolder.GetPointerAddress());
		layer = layerHolder.Extract();
		renderer2->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers2.push_back(layer);

		geometries.push_back(geo);

		return true;
	}
	return false;
}

/**
 * Method: DrawingBoard::AddLayer
 * Purpose:Adds a layer so that contents drawn withing the Ellipse are presented
 * Parameters: const ELLIPSE_2D& ellipse
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::AddLayer(const ELLIPSE_2D& ellipse)
{
	if (renderer.Get())
	{
		TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, ellipse);
		TrecComPointer<ID2D1Layer>::TrecComHolder layerHolder;

		// Apply to Primary Target
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();
		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers.push_back(layer);

		// Apply to Secondary Target
		renderer2->CreateLayer(layerHolder.GetPointerAddress());
		layer = layerHolder.Extract();
		renderer2->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers2.push_back(layer);

		geometries.push_back(geo);

		return true;
	}
	return false;
}

/**
 * Method: DrawingBoard::AddLayer
 * Purpose: Adds a layer so that contents drawn withing the Rounded Rectangle are presented
 * Parameters: const ROUNDED_RECT_2D& rRect
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::AddLayer(const ROUNDED_RECT_2D& rRect)
{
	if (renderer.Get())
	{
		TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, rRect);
		TrecComPointer<ID2D1Layer>::TrecComHolder layerHolder;

		// Apply to Primary Target
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();
		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers.push_back(layer);

		// Apply to Secondary Target
		renderer2->CreateLayer(layerHolder.GetPointerAddress());
		layer = layerHolder.Extract();
		renderer2->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers2.push_back(layer);

		geometries.push_back(geo);

		return true;
	}
	return false;
}

/**
 * Method: DrawingBoard::AddLayer
 * Purpose:Adds a layer so that contents drawn withing the geometry are presented
 * Parameters: const TDataArray<POINT_2D>& points
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::AddLayer(const TDataArray<POINT_2D>& points)
{
	if (renderer.Get())
	{
		TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, points);
		TrecComPointer<ID2D1Layer>::TrecComHolder layerHolder;

		// Apply to Primary Target
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();
		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers.push_back(layer);

		// Apply to Secondary Target
		renderer2->CreateLayer(layerHolder.GetPointerAddress());
		layer = layerHolder.Extract();
		renderer2->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers2.push_back(layer);

		geometries.push_back(geo);

		return true;
	}
	return false;
}

/**
 * Method: DrawingBoard::AddLayer
 * Purpose: Adds a layer so that contents drawn withing the geometry are presented
 * Parameters: TrecPointer<TGeometry> geo
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::AddLayer(TrecPointer<TGeometry> geo)
{
	if (renderer.Get())
	{
		TrecComPointer<ID2D1Layer>::TrecComHolder layerHolder;
		renderer->CreateLayer(layerHolder.GetPointerAddress());

		// Apply to Primary Target
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();
		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers.push_back(layer);

		// Apply to Secondary Target
		renderer2->CreateLayer(layerHolder.GetPointerAddress());
		layer = layerHolder.Extract();
		renderer2->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());
		layers2.push_back(layer);

		geometries.push_back(geo);
		return true;
	}
	return false;
}

/**
 * Method: DrawingBoard::GetGeometry
 * Purpose: Creates a TGeometry based off of the provided Rectabgle
 * Parameters: const RECT_2D& ret - the rectangle to get a geometry object from
 * Returns: TrecPointer<TGeometry> - the TGeometry object
 */
TrecPointer<TGeometry> DrawingBoard::GetGeometry(const RECT_2D& ret)
{
	if(!renderer.Get())
		return TrecPointer<TGeometry>();

	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, ret);

	return geo;
}

/**
 * Method: DrawingBoard::GetGeometry
 * Purpose: Creates a TGeometry based off of the provided Ellipse
 * Parameters: const ELLIPSE_2D& ellipse  - the ellipse to get a geometry object from
 * Returns: TrecPointer<TGeometry> - the TGeometry object
 */
TrecPointer<TGeometry> DrawingBoard::GetGeometry(const ELLIPSE_2D& ellipse)
{
	if (!renderer.Get())
		return TrecPointer<TGeometry>();


	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, ellipse);

	return geo;
}

/**
 * Method: DrawingBoard::GetGeometry
 * Purpose:Creates a TGeometry based off of the provided Rounded-Rectabgle
 * Parameters: const ROUNDED_RECT_2D& rRect - the rounded-rectangle to get a geometry object from
 * Returns: TrecPointer<TGeometry> - the TGeometry object
 */
TrecPointer<TGeometry> DrawingBoard::GetGeometry(const ROUNDED_RECT_2D& rRect)
{
	if (!renderer.Get())
		return TrecPointer<TGeometry>();


	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, rRect);

	return geo;
}

/**
 * Method: DrawingBoard::GetGeometry
 * Purpose:Creates a TGeometry based off of the provided set pf points
 * Parameters:const TDataArray<POINT_2D>& points - the points to get a geometry object from
 * Returns: TrecPointer<TGeometry> - the TGeometry object
 */
TrecPointer<TGeometry> DrawingBoard::GetGeometry(const TDataArray<POINT_2D>& points)
{
	if (!renderer.Get())
		return TrecPointer<TGeometry>();


	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, points);

	return geo;
}

/**
 * Method: DrawingBoard::GetLayerCount
 * Purpose: Retrieves the number of layers currently stacked on the render target
 * Parameters: void
 * Returns: UINT
 */
UINT DrawingBoard::GetLayerCount()
{
	return layers.Size();
}


/**
 * Method: DrawingBoard::SetToSecondaryTarget
 * Purpose: Have the Drawing board draw to the secondary Render Target
 * Parameters: void
 * Returns: void
 *
 * Note: May be depricated soon if secondary target is deemed unecessary
 */
void DrawingBoard::SetToSecondaryTarget()
{
	usePrimaryDc = false;
}


/**
 * Method: DrawingBoard::SetToPrimaryTarget
 * Purpose: Have the Drawing board draw to the primary Render Target
 * Parameters: void
 * Returns: void
 *
 * Note: May be depricated soon if secondary target is deemed unecessary
 */
void DrawingBoard::SetToPrimaryTarget()
{
	usePrimaryDc = true;
}

/**
 * Method: DrawingBoard::BeginDraw
 * Purpose: Have the Drawing Board commence the drawing Process
 * Parameters: void
 * Returns: void
 */
void DrawingBoard::BeginDraw()
{


	renderer->BeginDraw();
	renderer->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));
	renderer2->BeginDraw();
	renderer2->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));
}

/**
 * Method: DrawingBoard::EndDraw
 * Purpose: Have the drawing board finalize the drawing procedure, by copying the contents of the Bitmap targets over to the
 *	main one
 * Parameters: void
 * Return: void
 */
void DrawingBoard::EndDraw()
{
	int err = 0;
	if (!renderer.Get() || !renderer2.Get())
		return;
	if(FAILED(renderer->EndDraw()))
		err = GetLastError();
	if (FAILED(renderer2->EndDraw()))
		err = GetLastError();
	int e = 0;

	if (dxgiTarget.Get())
	{
		dxgiTarget->BeginDraw();
		ID2D1Bitmap* map = nullptr;
		if (SUCCEEDED(renderer->GetBitmap(&map)))
			dxgiTarget->DrawBitmap(map);
		dxgiTarget->EndDraw();
	}
	else if (windowTarget.Get())
	{
		windowTarget->BeginDraw();
		windowTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));
		ID2D1Bitmap* map = nullptr;
		if (SUCCEEDED(renderer->GetBitmap(&map)))
			windowTarget->DrawBitmap(map);
		windowTarget->EndDraw();
	}
}

/**
 * Method: DrawingBoard::GetDefaultColor
 * Purpose: Reports the default color for drawing basic content if a control has no content to draw itself
 * Parameters: void
 * Returns: D2D1_COLOR_F - the default content color to draw
 */
D2D1_COLOR_F DrawingBoard::GetDefaultColor()
{
	return defaultColor;
}

/**
 * Method: DrawingBoard::GetDefaultTextColor
 * Purpose: Reports the default color for drawing basic content if a control has no content to draw itself
 * Parameters: void
 * Returns: D2D1_COLOR_F - the default content color to draw
 */
D2D1_COLOR_F DrawingBoard::GetDefaultTextColor()
{
	return defaultTextColor;
}

/**
 * Method: DrawingBoard::SetDefaultColor
 * Purpose: Sets the default color to use
 * Parameters: const D2D1_COLOR_F& color - the color to set the default background color to
 * Returns: void
 */
void DrawingBoard::SetDefaultColor(const D2D1_COLOR_F& color)
{
	defaultColor = color;
	if(self.Get())
		defaultBrush = TrecPointerKey::GetNewTrecPointer<TBrush>(defaultColor, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self));
}

/**
 * Method: DrawingBoard::SetDefaultTextColor
 * Purpose: Sets the default text color to use
 * Parameters: const D2D1_COLOR_F& color - the color to set the default background color to
 * Returns: void
 */
void DrawingBoard::SetDefaultTextColor(const D2D1_COLOR_F& color)
{
	defaultTextColor = color;
}

/**
 * Method: DrawingBoard::FillControlBackground
 * Purpose: Enables controls to easily draw to the drawing board without holding a brush object themselves
 * Parameters: const D2D1_RECT_F& location - where to draw
 *				TShape shape - the default chape to draw
 * Returns: void
 *
 * Note: Initially, the only shapes supported are RECT and ELLIPSE as an elipse can easily be calculated from the rect param.
 * However other shapes need more data to properly articulate
 */
void DrawingBoard::FillControlBackground(const D2D1_RECT_F& location, TShape shape)
{
	assert(defaultBrush.Get());
	ELLIPSE_2D ell;
	switch (shape)
	{
	case TShape::T_Ellipse:
		ell.point.x = location.left / 2 + location.right / 2;
		ell.point.y = location.bottom / 2 + location.top / 2;
		ell.radiusX = location.right - location.left;
		ell.radiusY = location.bottom - location.top;
		defaultBrush->FillEllipse(ell);
		break;
	default:
		defaultBrush->FillRectangle(location);
	}
}


