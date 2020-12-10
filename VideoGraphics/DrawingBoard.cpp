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
	hMap = 0;
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";
	this->fact = fact;

	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_PREMULTIPLIED);

	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	RECT area;
	
	HDC copy = GetDC(window);
	dc = CreateCompatibleDC(copy);

	TrecComPointer<ID2D1DCRenderTarget>::TrecComHolder renderDc;

	HRESULT res = fact->CreateDCRenderTarget(&props, renderDc.GetPointerAddress());
	if (FAILED(res))
		throw L"Error! Failed to Create DC Render Target!";

	renderer = renderDc.Extract();
	layersPushed = 0;
	this->window = window;

	Resize(window);
}

DrawingBoard::~DrawingBoard()
{
	if (dc)
		DeleteDC(dc);
	dc = nullptr;
	if (hMap)
		DeleteObject(hMap);
	hMap = nullptr;
}




void DrawingBoard::Resize(HWND window)
{
	RECT r{ 0,0,0,0 };
	GetClientRect(window, &r);

	hMap = CreateCompatibleBitmap(GetDC(window), r.right - r.left, r.bottom - r.top);
	int err = 0;
	if (!hMap)
		err = GetLastError();
	assert(hMap);
	SelectObject(dc, hMap);

	renderer->BindDC(dc, &r);
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
TrecComPointer<ID2D1DCRenderTarget> DrawingBoard::GetRenderer()
{
	return renderer;
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
}


/**
 * Method: DrawingBoard::SetTransform
 * Purpose: Sets the transform of the underlying RenderTarget. Enables the rotation, translation, skewing, and sizing of various images
 * Parameters: const TRANSFORM_2D& matrix
 * Returns: bool - whether the operation was successful
 */
bool DrawingBoard::SetTransform(const TRANSFORM_2D& matrix)
{
	if (!renderer.Get())
		return false;

	renderer->SetTransform(matrix);
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
	if (renderer.Get() && layers.Size() && geometries.Size())
	{
		renderer->PopLayer();
		layers.RemoveAt(layers.Size() - 1);
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
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();

		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());

		layers.push_back(layer);
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
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();

		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());

		layers.push_back(layer);
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
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();

		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());

		layers.push_back(layer);
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
		renderer->CreateLayer(layerHolder.GetPointerAddress());
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();

		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());

		layers.push_back(layer);
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
		TrecComPointer<ID2D1Layer> layer = layerHolder.Extract();

		renderer->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geo->GetUnderlyingGeometry().Get()), layer.Get());

		layers.push_back(layer);
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

HDC DrawingBoard::GetDc()
{
	return dc;
}

