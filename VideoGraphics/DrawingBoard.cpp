#include "DrawingBoard.h"
#include "TGeometry.h"


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
	is3D = false;
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";
	this->fact = fact;

	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat();

	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	RECT area;

	GetClientRect(window, &area);

	D2D1_HWND_RENDER_TARGET_PROPERTIES hProps;
	ZeroMemory(&hProps, sizeof(hProps));

	hProps.hwnd = window;
	hProps.pixelSize = D2D1::SizeU(area.right - area.left,
		area.bottom - area.top);



	TrecComPointer<ID2D1HwndRenderTarget>::TrecComHolder renderHw;
	HRESULT res = fact->CreateHwndRenderTarget(props, hProps, renderHw.GetPointerAddress());

	if (FAILED(res))
		throw L"Error! Failed to Create Window Render Target!";

	renderer = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1HwndRenderTarget>(renderHw);
	layersPushed = 0;
	this->window = window;
}

/**
 * Method: DrawingBoard::DrawingBoard
 * Purpose: Constructor that sets the mode to 3D
 * Parameters: TrecComPointer<ID2D1Factory1> fact - the Direct2D Factory associated with the intance
 *				TrecPointer<TWindowEngine> engine - the set of resources that enable integration with Direct3D
 * Returns: New DrawingBoard that works with the TWindowEngine to allow 3D rendering
 */
DrawingBoard::DrawingBoard(TrecComPointer<ID2D1Factory1> fact, TrecPointer<TWindowEngine> engine)
{
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";
	this->fact = fact;
	if (!engine.Get())
		throw L"Error! ArenaEngine Object MUST be initialized for a 3D enabled Page";
	//if (!eh.Get())
	//	throw L"Error! Event Handler MUST be instantiated!";


	TrecComPointer<IDXGISurface1> surf = engine->GetSurface();
	if (!surf.Get())
		throw L"Error! Provided 3D Engine does not have a Surface to create a 2D Render Target with!";

	TrecComPointer<IDXGIDevice> dev = engine->getDeviceD_U();

	TrecComPointer<ID2D1Device>::TrecComHolder d2dDevHolder;

	HRESULT res = fact->CreateDevice(dev.Get(), d2dDevHolder.GetPointerAddress());
	if (FAILED(res))
		throw L"Error! 2D Device Creation failed!";
	TrecComPointer<ID2D1Device> d2dDev = d2dDevHolder.Extract();

	TrecComPointer<ID2D1DeviceContext>::TrecComHolder contextRenderHolder;
	res = d2dDev->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, contextRenderHolder.GetPointerAddress());

	if (FAILED(res))
		throw L"ERROR! Failed to Generate 3D Compatiblie Render Target!";

	D2D1_BITMAP_PROPERTIES1 bmp1;
	bmp1.colorContext = nullptr;
	bmp1.dpiX = bmp1.dpiY = 0;
	bmp1.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
		| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

	renderer = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DeviceContext>(contextRenderHolder);

	TrecComPointer<ID2D1Bitmap1>::TrecComHolder bitHolder;
	res = reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->CreateBitmapFromDxgiSurface(surf.Get(), bmp1, bitHolder.GetPointerAddress());

	if (FAILED(res))
		throw L"ERROR! Could Not retrieve Bitmap from Device Context!";

	bit = bitHolder.Extract();
	reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->SetTarget(bit.Get());
	TrecComPointer< ID2D1GdiInteropRenderTarget>::TrecComHolder gdiRenderHolder;
	res = reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)gdiRenderHolder.GetPointerAddress());
	gdiRender = gdiRenderHolder.Extract();
	is3D = true;
	this->engine = engine;

	layersPushed = 0;
	this->window = engine->GetWindowHandle();
}


/**
 * Method: DrawingBoard::Set3D
 * Purpose: Enables the Drawing Board to switch from only 2D support to 3D support
 * Parameters: TrecPointer<TWindowEngine> engine - the set of resources needed to jump from 2D to 3D
 * Returns: void
 *
 * Note: this method effectively does noting if the DrawingBoard is already set to 3D
 */
void DrawingBoard::Set3D(TrecPointer<TWindowEngine> engine)
{
	// If already at 3D, just return
	if (is3D && this->engine.Get())
		return;

	if (!renderer.Get())
		return;
	

	ID2D1Factory1* tempFact = nullptr;
	ID2D1Factory* tempBaseFact = nullptr;
	renderer->GetFactory(&tempBaseFact);
	tempFact = reinterpret_cast<ID2D1Factory1*>(tempBaseFact);

	
	Set3D(engine, tempFact);



	tempFact->Release();

}


/**
 * Method: DrawingBoard::Resize
 * Purpose: Resets the Underlying Rendertarget should the window size be changed
 * Parameters: HWND window -  the window to get the size from
 * Returns: void
 */
void DrawingBoard::Resize(HWND window)
{
	RECT area;
	GetClientRect(window, &area);
	if (is3D)
	{

		this->bit.Delete();
		this->gdiRender.Delete();

	


		reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->SetTarget(nullptr);
	this->engine->Resize(area.right - area.left,
			area.bottom - area.top);
		
	auto surf = engine->GetSurface();
	TrecComPointer<ID2D1Bitmap1>::TrecComHolder bitHolder;

	D2D1_BITMAP_PROPERTIES1 bmp1;
	bmp1.colorContext = nullptr;
	bmp1.dpiX = bmp1.dpiY = 0;
	bmp1.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
		| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

	auto res = reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->CreateBitmapFromDxgiSurface(surf.Get(), bmp1, bitHolder.GetPointerAddress());

	if (FAILED(res))
		throw L"ERROR! Could Not retrieve Bitmap from Device Context!";

	bit = bitHolder.Extract();
	reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->SetTarget(bit.Get());
	TrecComPointer< ID2D1GdiInteropRenderTarget>::TrecComHolder gdiRenderHolder;
	res = reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)gdiRenderHolder.GetPointerAddress());
	gdiRender = gdiRenderHolder.Extract();


	}
	else if(renderer.Get())
	{
		
		reinterpret_cast<ID2D1HwndRenderTarget*>(renderer.Get())->Resize(D2D1::SizeU(area.right - area.left,
			area.bottom - area.top));
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
TrecComPointer<ID2D1RenderTarget> DrawingBoard::GetRenderer()
{
	return renderer;
}

/**
 * Method: DrawingBoard::GetGdiRenderer
 * Purpose: Retrieves the GDI-RenderTarget
 * Parameters: void
 * Returns: TrecComPointer<ID2D1GdiInteropRenderTarget> the 3D RenderTarget
 *
 * Note: Retrieving this value and performing a null check is a good way of seeing if this DrawingBoard
 *		is 3D enabled. If it is null, then only 2D is supported
 */
TrecComPointer<ID2D1GdiInteropRenderTarget> DrawingBoard::GetGdiRenderer()
{
	return gdiRender;
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

void DrawingBoard::Set3D(TrecPointer<TWindowEngine> engine, ID2D1Factory1* fact)
{
	TrecComPointer<IDXGISurface1> surf = engine->GetSurface();
	if (!surf.Get())
		throw L"Error! Provided 3D Engine does not have a Surface to create a 2D Render Target with!";

	TrecComPointer<IDXGIDevice> dev = engine->getDeviceD_U();

	TrecComPointer<ID2D1Device>::TrecComHolder d2dDevHolder;

	HRESULT res = fact->CreateDevice(dev.Get(), d2dDevHolder.GetPointerAddress());
	if (FAILED(res))
		throw L"Error! 2D Device Creation failed!";
	TrecComPointer<ID2D1Device> d2dDev = d2dDevHolder.Extract();

	TrecComPointer<ID2D1DeviceContext>::TrecComHolder contextRenderHolder;
	res = d2dDev->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, contextRenderHolder.GetPointerAddress());

	if (FAILED(res))
		throw L"ERROR! Failed to Generate 3D Compatiblie Render Target!";

	D2D1_BITMAP_PROPERTIES1 bmp1;
	bmp1.colorContext = nullptr;
	bmp1.dpiX = bmp1.dpiY = 0;
	bmp1.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
		| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	renderer.Delete();
	renderer = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DeviceContext>(contextRenderHolder);

	TrecComPointer<ID2D1Bitmap1>::TrecComHolder bitHolder;
	res = reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->CreateBitmapFromDxgiSurface(surf.Get(), bmp1, bitHolder.GetPointerAddress());

	if (FAILED(res))
		throw L"ERROR! Could Not retrieve Bitmap from Device Context!";

	bit = bitHolder.Extract();
	reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->SetTarget(bit.Get());
	TrecComPointer< ID2D1GdiInteropRenderTarget>::TrecComHolder gdiRenderHolder;
	res = reinterpret_cast<ID2D1DeviceContext*>(renderer.Get())->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)gdiRenderHolder.GetPointerAddress());
	gdiRender = gdiRenderHolder.Extract();
	is3D = true;
	this->engine = engine;

	is3D = true;
}
