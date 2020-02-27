#include "DrawingBoard.h"
#include "TGeometry.h"

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
}

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
}

void DrawingBoard::Set3D(TrecPointer<TWindowEngine> engine)
{
	if (is3D && this->engine.Get())
		return;

	if (renderer.Get())
		return;
	HWND win = reinterpret_cast<ID2D1HwndRenderTarget*>(renderer.Get())->GetHwnd();

	ID2D1Factory1* tempFact = nullptr;
	ID2D1Factory* tempBaseFact = nullptr;
	renderer->GetFactory(&tempBaseFact);
	tempFact = reinterpret_cast<ID2D1Factory1*>(tempBaseFact);

	
	TrecComPointer<IDXGISurface1> surf = engine->GetSurface();
	if (!surf.Get())
		throw L"Error! Provided 3D Engine does not have a Surface to create a 2D Render Target with!";

	TrecComPointer<IDXGIDevice> dev = engine->getDeviceD_U();

	TrecComPointer<ID2D1Device>::TrecComHolder d2dDevHolder;

	HRESULT res = tempFact->CreateDevice(dev.Get(), d2dDevHolder.GetPointerAddress());
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





	tempFact->Release();
	is3D = true;
}

void DrawingBoard::Resize()
{
	if (is3D)
	{
		is3D = false;
		Set3D(engine);
	}
}

TrecPointer<TBrush> DrawingBoard::GetBrush(const TColor& col)
{
	return TrecPointerKey::GetNewTrecPointer<TBrush>(col, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self));
}

TrecPointer<TBrush> DrawingBoard::GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2, float x, float y)
{
	return TrecPointerKey::GetNewTrecPointer<TBrush>(coll, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self), p1, p2, x, y);
}

TrecPointer<TBrush> DrawingBoard::GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2)
{
	return TrecPointerKey::GetNewTrecPointer<TBrush>(coll, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self), p1, p2);
}

TrecSubPointer<TBrush, TBitmapBrush> DrawingBoard::GetBrush(TrecPointer<TFileShell> picture, RECT_2D& loc)
{
	return TrecPointerKey::GetNewTrecSubPointer<TBrush, TBitmapBrush>(picture, TrecPointerKey::GetTrecPointerFromSoft<DrawingBoard>(self), loc);
}

TrecComPointer<ID2D1RenderTarget> DrawingBoard::GetRenderer()
{
	return renderer;
}

TrecComPointer<ID2D1GdiInteropRenderTarget> DrawingBoard::GetGdiRenderer()
{
	return gdiRender;
}

void DrawingBoard::SetSelf(TrecPointer<DrawingBoard> self)
{
	if (this != self.Get())
		throw L"ERROR! Self needed to be a real reference to this object";

	this->self = TrecPointerKey::GetSoftPointerFromTrec<DrawingBoard>(self);
}


bool DrawingBoard::SetTransform(const TRANSFORM_2D& matrix)
{
	if (!renderer.Get())
		return false;

	renderer->SetTransform(matrix);
	return true;
}

bool DrawingBoard::GetTransform(TRANSFORM_2D& matrix)
{
	if (!renderer.Get())
		return false;

	renderer->GetTransform(&matrix);
	return true;
}

void DrawingBoard::PopLayer()
{
	if (renderer.Get() && layers.Size() && geometries.Size())
	{
		renderer->PopLayer();
		layers.RemoveAt(layers.Size() - 1);
		geometries.RemoveAt(geometries.Size() - 1);
	}
}

bool DrawingBoard::AddLayer(RECT_2D& ret)
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

bool DrawingBoard::AddLayer(ELLIPSE_2D& ellipse)
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

bool DrawingBoard::AddLayer(ROUNDED_RECT_2D& rRect)
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

bool DrawingBoard::AddLayer(TDataArray<POINT_2D>& points)
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

TrecPointer<TGeometry> DrawingBoard::GetGeometry(RECT_2D& ret)
{
	if(!renderer.Get())
		return TrecPointer<TGeometry>();

	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, ret);

	return geo;
}

TrecPointer<TGeometry> DrawingBoard::GetGeometry(ELLIPSE_2D& ellipse)
{
	if (!renderer.Get())
		return TrecPointer<TGeometry>();


	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, ellipse);

	return geo;
}

TrecPointer<TGeometry> DrawingBoard::GetGeometry(ROUNDED_RECT_2D& rRect)
{
	if (!renderer.Get())
		return TrecPointer<TGeometry>();


	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, rRect);

	return geo;
}

TrecPointer<TGeometry> DrawingBoard::GetGeometry(TDataArray<POINT_2D>& points)
{
	if (!renderer.Get())
		return TrecPointer<TGeometry>();


	TrecPointer<TGeometry> geo = TrecPointerKey::GetNewTrecPointer<TGeometry>(fact, points);

	return geo;
}

UINT DrawingBoard::GetLayerCount()
{
	return layers.Size();
}