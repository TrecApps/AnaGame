#pragma once
#include "TShaderHost.h"

// Create Macros for error
#define NO_ERROR         0
#define NO_DEVICE        1
#define NO_SWAP_CHAIN    2
#define NO_RENDER_TARGET 3

// Macros for keeping track of non-boolean rasterization
#define CULL_MODE unsigned char
#define CULL_NONE  0
#define CULL_BACK  2
#define CULL_FRONT 1

// Default Shader Count, since AnaGame will provide it's own shaders
// the Actual number being unknown as of right now
#define ANAGAME_DAFAULT_SHADER_COUNT 5


/* 
 * class TWindowEngine
 * Purpose: Hold 3D Resources that can be shared within a Window such as the Context Device,
 *		renderTargets, SwapChains, and AnaGame native shaders.
 * Note: Ideally, there should be one instance of this class per Window and is intended to be 
		an attribute of the TWindow class of AnaGame's Tap library (or which ever class in a 3rd party Library
		that manages Windows)
 */
class _VIDEO_GRAPHICS TWindowEngine :
	public TShaderHost
{
public:
	TWindowEngine(HWND win, HINSTANCE ins);
	~TWindowEngine();
	int Initialize();

	void Resize(UINT x, UINT y);

	// Direct3D Get Methods
	TrecComPointer<ID3D11DeviceContext> getDevice();
	TrecComPointer<IDXGISwapChain> getSwapChain();
	TrecComPointer<ID3D11RenderTargetView> getRederTarget();
	TrecComPointer<ID3D11Device> getDeviceD();
	TrecComPointer<IDXGIDevice> getDeviceD_U();
	TrecComPointer<IDXGISurface1> GetSurface();

	void PrepareScene(D2D1::ColorF color);
	void FinalizeScene();

	void ReportLiveObjects();

	HWND GetWindowHandle();

protected:
	
	// Methods that external AnaGame Components don't need to interat with directly
	int InitializeDefaultShaders();

	void SetDefaultRasterization();


	// Basic Object Resources
	HWND window;		// The Window these resources are rendering to
	HINSTANCE instance;	// The OS Instance that is overseeing this
	bool isInit;		// Has this Object been fully initialized
	bool defaultShadersSet;
	RECT Location;
	

	// Window Level Direct3D Resources
	TrecComPointer<ID3D11Device> graphicsDevice;
	TrecComPointer<IDXGIDevice> dxDev;
	TrecComPointer<IDXGISurface1> surface;
	TrecComPointer<ID3D11DeviceContext> contextDevice;
	TrecComPointer<IDXGISwapChain> swapper;
	TrecComPointer<ID3D11RenderTargetView> renderTarget;
	D3D_DRIVER_TYPE driver;
	D3D_FEATURE_LEVEL version;

	D3D11_RASTERIZER_DESC rasterizer;
	TrecComPointer<ID3D11RasterizerState> rasterizerState;
	bool isFill; // Solid or wire-frame?
	CULL_MODE cullMode;
};

