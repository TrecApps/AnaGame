#include "TWindowEngine.h"
#include <DirectoryInterface.h>
#include "TShaderParser.h"
#include <TML_Reader_.h>

TWindowEngine::TWindowEngine(HWND win, HINSTANCE ins)
{
	isInit = false;
	window = win;
	instance = ins;

	driver = D3D_DRIVER_TYPE_NULL;    // System will pick best configuration
	version = D3D_FEATURE_LEVEL_11_0; // Target Windows 7 and above

	defaultShadersSet = false;
	Location = RECT{ 0,0,0,0 };
	cullMode = 0;
	isFill = false;
	ZeroMemory(&rasterizer, sizeof(rasterizer));
}

TWindowEngine::~TWindowEngine()
{
	int e = 0;

	for (UINT Rust = 0; Rust < shaders.Size(); Rust++)
	{
		shaders[Rust].cs.Delete();
		shaders[Rust].ds.Delete();
		if(shaders[Rust].elements)
			delete[] shaders[Rust].elements;
		shaders[Rust].gs.Delete();
		shaders[Rust].hs.Delete();
		shaders[Rust].layout.Delete();
		shaders[Rust].ps.Delete();
		shaders[Rust].vs.Delete();
	}
	swapper.Delete();
	renderTarget.Delete();
	contextDevice.Delete();
	surface.Delete();
	dxDev.Delete();
	graphicsDevice.Delete();
	
	int f = 0;

}

int TWindowEngine::Initialize()
{
	if (isInit)
		return 0;
	 
	GetClientRect(window, &Location);

	unsigned int width = Location.right - Location.left,
		height = Location.bottom - Location.top;

	D3D_DRIVER_TYPE dTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE//, D3D_DRIVER_TYPE_WARP
	};
	int tTypes = ARRAYSIZE(dTypes);
	D3D_FEATURE_LEVEL dLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
	};
	int tLevels = ARRAYSIZE(dLevels);


	DXGI_SWAP_CHAIN_DESC swapChainDescription;



	// Initialize the swap cahin
	swapChainDescription.BufferCount = 2;
	swapChainDescription.BufferDesc.Width = Location.right - Location.left;
	swapChainDescription.BufferDesc.Height = Location.bottom - Location.top;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 30;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow = window;
	swapChainDescription.Windowed = true;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;// DXGI_SWAP_EFFECT_DISCARD;

	unsigned int flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	HRESULT results;

	TrecComPointer<ID3D11Device>::TrecComHolder d;
	TrecComPointer<ID3D11DeviceContext>::TrecComHolder cd;
	results = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags,
		0, 0, D3D11_SDK_VERSION, d.GetPointerAddress(), &version, cd.GetPointerAddress());
	graphicsDevice = d.Extract();
	contextDevice = cd.Extract();


	if (FAILED(results))
	{
		return NO_DEVICE;
	}

	dxDev.Nullify();
	TrecComPointer<IDXGIDevice>::TrecComHolder raw_dxDev;
	IDXGIAdapter* dxAdpt = nullptr;
	IDXGIFactory* dxFact = nullptr;
	results = (graphicsDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)raw_dxDev.GetPointerAddress()));
	if (FAILED(results))
	{

		graphicsDevice.Nullify();
		contextDevice.Nullify();
		return NO_DEVICE;
	}
	dxDev = raw_dxDev.Extract();
	results = dxDev->GetParent(__uuidof(IDXGIAdapter), (void**)& dxAdpt);
	if (FAILED(results))
	{

		graphicsDevice.Nullify();
		contextDevice.Nullify();
		dxDev.Nullify();
		return NO_DEVICE;
	}
	results = dxAdpt->GetParent(__uuidof(IDXGIFactory), (void**)& dxFact);


	if (FAILED(results))
	{


		dxAdpt->Release();
		graphicsDevice.Nullify();
		contextDevice.Nullify();
		dxDev.Nullify();
		return NO_DEVICE;
	}


	TrecComPointer<IDXGISwapChain>::TrecComHolder sc;
	results = dxFact->CreateSwapChain(graphicsDevice.Get(), &swapChainDescription, sc.GetPointerAddress());
	

	if (FAILED(results))
	{
		dxAdpt->Release();
		dxFact->Release();
		dxDev.Nullify();
		graphicsDevice.Nullify();
		contextDevice.Nullify();
		return NO_DEVICE;


	}
	swapper = sc.Extract();

	ID3D11Texture2D* texture;

	results = swapper->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& texture);
	if (FAILED(results))
	{
		return NO_SWAP_CHAIN;
	}

	D3D11_RENDER_TARGET_VIEW_DESC rendDesc;
	ZeroMemory(&rendDesc, sizeof(rendDesc));
	//rendDesc.
	rendDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rendDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rendDesc.Texture2D.MipSlice = 0;

	TrecComPointer<ID3D11RenderTargetView>::TrecComHolder rt;
	results = graphicsDevice->CreateRenderTargetView(texture, &rendDesc, rt.GetPointerAddress());
	renderTarget = rt.Extract();



	if (texture)
		texture->Release();

	if (FAILED(results))
	{
		return NO_RENDER_TARGET;
	}

	// Create a DXGISurface so that Interoperability with Direct2D is simpler
	TrecComPointer<IDXGISurface1>::TrecComHolder surfaceHolder;
	results = swapper->GetBuffer(0, __uuidof(IDXGISurface1), (void**)surfaceHolder.GetPointerAddress());
	surface = surfaceHolder.Extract();

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	stencilDesc.Texture2D.MipSlice = 0;
	ID3D11DepthStencilView* depthStencil = nullptr;
	results = graphicsDevice->CreateDepthStencilView(texture, &stencilDesc, &depthStencil);

	ID3D11RenderTargetView* const renderer = renderTarget.Get();
	if (SUCCEEDED(results))
		contextDevice->OMSetRenderTargets(1, &renderer, depthStencil);
	else
		contextDevice->OMSetRenderTargets(1, &renderer, 0);


	//D3D11_BUFFER_DESC conBuf;
	//ZeroMemory(&conBuf, sizeof(conBuf));
	//conBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//conBuf.ByteWidth = sizeof(DirectX::XMMATRIX);
	//conBuf.Usage = D3D11_USAGE_DEFAULT;


	ID3D11RasterizerState* rs = nullptr;


	SetDefaultRasterization();
	InitializeDefaultShaders();
	return NO_ERROR;
}

void TWindowEngine::Resize()
{
	if (!swapper.Get() || !surface.Get() || !contextDevice.Get())
		return;
	RECT oldRect = Location;
	if (!GetClientRect(window, &Location))
		return;

	UINT buffCount = 2;
	UINT width = Location.right - Location.left;
	UINT height = Location.bottom - Location.top;

	DXGI_FORMAT NewFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	UINT flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

	renderTarget.Delete();
	surface.Delete();

	contextDevice->ClearState();

	//ReportLiveObjects();

	HRESULT res = swapper->ResizeBuffers(buffCount, width, height, NewFormat, flags);
	if (FAILED(res))
		int e = 0;

	ID3D11Texture2D* texture;

	res = swapper->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture);
	if (FAILED(res))
	{
		return ;
	}


	D3D11_RENDER_TARGET_VIEW_DESC rendDesc;
	ZeroMemory(&rendDesc, sizeof(rendDesc));
	//rendDesc.
	rendDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rendDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rendDesc.Texture2D.MipSlice = 0;

	TrecComPointer<ID3D11RenderTargetView>::TrecComHolder rt;
	res = graphicsDevice->CreateRenderTargetView(texture, &rendDesc, rt.GetPointerAddress());
	renderTarget = rt.Extract();



	if (texture)
		texture->Release();

	if (FAILED(res))
	{
		return;
	}

	// Create a DXGISurface so that Interoperability with Direct2D is simpler
	TrecComPointer<IDXGISurface1>::TrecComHolder surfaceHolder;
	res = swapper->GetBuffer(0, __uuidof(IDXGISurface1), (void**)surfaceHolder.GetPointerAddress());
	surface = surfaceHolder.Extract();
}

/*
* Method: TWindowEngine - getDevice
* Purpose: Retrieves the Context Device used for drawing
* Parameters: void
* Return: TrecComPointer<ID3D11DeviceContext> - Pointer to the Device Context
*/
TrecComPointer<ID3D11DeviceContext> TWindowEngine::getDevice()
{
	return contextDevice;
}

/*
* Method: TWindowEngine - getSwapChain
* Purpose: Retrieves the Swap Chain used by the Engine
* Parameters: void
* Return: TrecComPointer<IDXGISwapChain> - Pointer to the Swap Chain
*/
TrecComPointer<IDXGISwapChain> TWindowEngine::getSwapChain()
{
	return swapper;
}

/*
* Method: TWindowEngine - getRederTarget
* Purpose: Retrieves the 3D Render Target for the engine
* Parameters: void
* Return: TrecComPointer<ID3D11RenderTargetView> - Pointer to the 3D Render Target
*/
TrecComPointer<ID3D11RenderTargetView> TWindowEngine::getRederTarget()
{
	return renderTarget;
}

/*
* Method: TWindowEngine - getDeviceD
* Purpose: Retrieves the Direct 3D device used by the engine
* Parameters: void
* Return: TrecComPointer<ID3D11Device> - Pointer to the engine's Direct3D Device
*/
TrecComPointer<ID3D11Device> TWindowEngine::getDeviceD()
{
	return graphicsDevice;
}

/*
* Method: TWindowEngine - getDeviceD_U
* Purpose: Retrieves the DirectX (mixer) device the engine uses
* Parameters: void
* Return: TrecComPointer<IDXGIDevice> - Pointer to the engine's DirectX Device
*/
TrecComPointer<IDXGIDevice> TWindowEngine::getDeviceD_U()
{
	return dxDev;
}

TrecComPointer<IDXGISurface1> TWindowEngine::GetSurface()
{
	return surface;
}

/*
* Method: TWindowEngine - PrepareScene
* Purpose: Sets up a Round of Drawing
* Parameters: D2D1::ColorF color - The color to clear the render target to
* Return: void
*/
void TWindowEngine::PrepareScene(D2D1::ColorF color)
{
	if (!contextDevice.Get() || !renderTarget.Get())
		return;
	float clearColor[4] = { color.r, color.g ,color.b,color.a };

	contextDevice->ClearRenderTargetView(renderTarget.Get(), clearColor);
	ID3D11RenderTargetView* const renderer = renderTarget.Get();
	contextDevice->OMSetRenderTargets(1, &renderer, nullptr);
}

/*
* Method: TWindowEngine - FinalizeScene
* Purpose: Completes a round of Rendering by presenting it to the Swap Chain
* Parameters: void
* Return: void
*/
void TWindowEngine::FinalizeScene()
{
	if (swapper.Get())
		swapper->Present(0, 0);
}

void TWindowEngine::ReportLiveObjects()
{
	if (!graphicsDevice.Get())return;
	ID3D11Debug* debugger = nullptr;
	graphicsDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&debugger);
	if (debugger)
		debugger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}



int TWindowEngine::InitializeDefaultShaders()
{
	if (defaultShadersSet)
		return 0;


	TString tDirectory = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

	tDirectory.Append(L"\\Resources\\DefaultShaders");

	TrecPointer<TFile> file;

	TML_Reader_* read = nullptr;

	TString tempDir;


	// Prepare to Read the Single Color Shader

	tempDir.Set(tDirectory);
	tempDir.Append(L"\\SingleColor");
	TrecPointer<Parser_> parse = TrecPointerKey::GetNewTrecPointerAlt<Parser_, TShaderParser>(this, tempDir, graphicsDevice);
	tempDir.Append(L"\\SingleColor.tml");


	try {
		file = TrecPointerKey::GetNewTrecPointer<TFile>(tempDir, TFile::t_file_read);

		TML_Reader_* read = new TML_Reader_(file, parse);
		int error = 0;
		if (!read->read(&error))
		{

			file->Close();
			delete read;
			return -2;
		}
	}
	catch (...)
	{

		return 2;
	}


	file->Close();

	delete read;

	file.Nullify();
	read = nullptr;
	parse.Nullify();

	// Prepare to read the 3D Color Vertex Shader


	tempDir.Set(tDirectory);
	tempDir.Append(L"\\3DColor");
	parse = TrecPointerKey::GetNewTrecPointerAlt<Parser_, TShaderParser>(this, tempDir, graphicsDevice);
	tempDir.Append(L"\\3DColor.tml");


	try {
		file = TrecPointerKey::GetNewTrecPointer<TFile>(tempDir, TFile::t_file_read);
		TML_Reader_* read = new TML_Reader_(file, parse);
		int error = 0;
		if (!read->read(&error))
		{
			file->Close();
			delete read;
			return -3;
		}
	}
	catch (...)
	{

		return 3;
	}
	file->Close();
	delete read;
	file.Nullify();
	read = nullptr;
	parse.Nullify();

	// Prepare to Read the 4D Color Shader


	tempDir.Set(tDirectory);
	tempDir.Append(L"\\4DColor");
	parse = TrecPointerKey::GetNewTrecPointerAlt<Parser_, TShaderParser>(this, tempDir, graphicsDevice);
	tempDir.Append(L"\\4DColor.tml");


	try {
		file = TrecPointerKey::GetNewTrecPointer<TFile>(tempDir, TFile::t_file_read);
		TML_Reader_* read = new TML_Reader_(file, parse);
		int error = 0;
		if (!read->read(&error))
		{
			file->Close();
			delete read;
			return -4;
		}
	}
	catch (...)
	{

		return 4;
	}
	file->Close();
	delete read;
	file.Nullify();
	read = nullptr;
	parse.Nullify();

	// Prepare to read the one Texture Shader

	tempDir.Set(tDirectory);
	tempDir.Append(L"\\Texture1");
	parse = TrecPointerKey::GetNewTrecPointerAlt<Parser_, TShaderParser>(this, tempDir, graphicsDevice);
	tempDir.Append(L"\\Texture1.tml");


	try {
		file = TrecPointerKey::GetNewTrecPointer<TFile>(tempDir, TFile::t_file_read);
		TML_Reader_* read = new TML_Reader_(file, parse);
		int error = 0;
		if (!read->read(&error))
		{
			file->Close();
			delete read;
			return -5;
		}
	}
	catch (...)
	{

		return 5;
	}
	file->Close();
	delete read;
	file.Nullify();
	read = nullptr;
	parse.Nullify();

	// Prepare the Double Texture Shader
	tempDir.Set(tDirectory);
	tempDir.Append(L"\\Texture2");
	parse = TrecPointerKey::GetNewTrecPointerAlt<Parser_, TShaderParser>(this, tempDir, graphicsDevice);
	tempDir.Append(L"\\Texture2.tml");


	try {
		file = TrecPointerKey::GetNewTrecPointer<TFile>(tempDir, TFile::t_file_read);
		TML_Reader_* read = new TML_Reader_(file, parse);
		int error = 0;
		if (!read->read(&error))
		{
			file->Close();
			delete read;
			return -6;
		}
	}
	catch (...)
	{

		return 6;
	}
	file->Close();
	delete read;
	file.Nullify();
	read = nullptr;
	parse.Nullify();


	defaultShadersSet = true;
	return 0;
}

/*
* Method: TWindowEngine - SetDefaultRasterization
* Purpose: Sets the Rasterization mode to the default settings
* Parameters: void
* Return: void
*/
void TWindowEngine::SetDefaultRasterization()
{
	if (contextDevice.Get())
	{
		rasterizer.CullMode = D3D11_CULL_BACK;
		rasterizer.FillMode = D3D11_FILL_SOLID;
		TrecComPointer<ID3D11RasterizerState>::TrecComHolder state;
		graphicsDevice->CreateRasterizerState(&rasterizer, state.GetPointerAddress());
		rasterizerState = state.Extract();
		contextDevice->RSSetState(rasterizerState.Get());
		isFill = true;
		cullMode = CULL_BACK + 1;
	}
}
