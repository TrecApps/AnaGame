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
 * Class: TWindowEngine
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

	/**
	 * Method: TWindowEngine::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TWindowEngine::TWindowEngine
	 * Purpose: Constructor
	 * Parameters: HWND win - Handle to the Window to use
	 *				HINSTANCE ins - OS level Instance Engine is operating in
	 * Returns: New 3D Window Engine object
	 */
	TWindowEngine(HWND win, HINSTANCE ins);
	/**
	 * Method: TWindowEngine::~TWindowEngine
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TWindowEngine();
	/**
	 * Method: TWindowEngine::Initialize
	 * Purpose: Sets up all of the Direct3D resources this Engine will need to draw properly
	 * Parameters: void
	 * Returns: int - errorcode
	 */
	int Initialize();

	/**
	 * Method: TWindowEngine::Resize
	 * Purpose: Allows the Engine to know that the Window has been resized and update resources accordingly
	 * Parameters: UINT x - the new width of the window
	 *				UINT y - the ne height of the window
	 * Returns: void
	 */
	void Resize(UINT x, UINT y);

	// Direct3D Get Methods

	/**
	 * Method: TWindowEngine::getDevice
	 * Purpose: Retrieves the Context Device used for drawing
	 * Parameters: void
	 * Return: TrecComPointer<ID3D11DeviceContext> - Pointer to the Device Context
	 */
	TrecComPointer<ID3D11DeviceContext> getDevice();
	/**
	 * Method: TWindowEngine::getSwapChain
	 * Purpose: Retrieves the Swap Chain used by the Engine
	 * Parameters: void
	 * Return: TrecComPointer<IDXGISwapChain> - Pointer to the Swap Chain
	 */
	TrecComPointer<IDXGISwapChain> getSwapChain();
	/**
	 * Method: TWindowEngine::getRederTarget
	 * Purpose: Retrieves the 3D Render Target for the engine
	 * Parameters: void
	 * Return: TrecComPointer<ID3D11RenderTargetView> - Pointer to the 3D Render Target
	 */
	TrecComPointer<ID3D11RenderTargetView> getRederTarget();
	/**
	 * Method: TWindowEngine::getDeviceD
	 * Purpose: Retrieves the Direct 3D device used by the engine
	 * Parameters: void
	 * Return: TrecComPointer<ID3D11Device> - Pointer to the engine's Direct3D Device
	 */
	TrecComPointer<ID3D11Device> getDeviceD();
	/**
	 * Method: TWindowEngine::getDeviceD_U
	 * Purpose: Retrieves the DirectX (mixer) device the engine uses
	 * Parameters: void
	 * Return: TrecComPointer<IDXGIDevice> - Pointer to the engine's DirectX Device
	 */
	TrecComPointer<IDXGIDevice> getDeviceD_U();
	/**
	 * Method: TWindowEngine::GetSurface
	 * Purpose: Retrieves the Surface created to allow the Window to integrate with Direct2D
	 * Parameters: void
	 * Returns: TrecComPointer<IDXGISurface1> - reference to the requested surface
	 */
	TrecComPointer<IDXGISurface1> GetSurface();


	/**
	 * Method: TWindowEngine::PrepareScene
	 * Purpose: Sets up a Round of Drawing
	 * Parameters: D2D1::ColorF color - The color to clear the render target to
	 * Return: void
	 */
	void PrepareScene(D2D1::ColorF color);
	/**
	 * Method: TWindowEngine::FinalizeScene
	 * Purpose: Completes a round of Rendering by presenting it to the Swap Chain
	 * Parameters: void
	 * Return: void
	 */
	void FinalizeScene();

	/**
	 * Method: TWindowEngine::ReportLiveObjects
	 * Purpose: Debugging method for live objects created under Direct3D
	 * Parameters: void
	 * Returns: void
	 */
	void ReportLiveObjects();

	/**
	 * Method: TWindowEngine::GetWindowHandle
	 * Purpose: Retrieves the Window Handle used by the engine
	 * Parameters: void
	 * Returns: HWND
	 */
	HWND GetWindowHandle();

protected:
	
	// Methods that external AnaGame Components don't need to interat with directly

	/**
	 * Method: TWindowEngine::InitializeDefaultShaders
	 * Purpose: Prepares the native Anagame shaders to use
	 * Parameters: void
	 * Return: int - errorcode
	 */
	int InitializeDefaultShaders();


	/**
	 * Method: TWindowEngine::SetDefaultRasterization
	 * Purpose: Sets the Rasterization mode to the default settings
	 * Parameters: void
	 * Return: void
	 */
	void SetDefaultRasterization();


	// Basic Object Resources

	/**
	 * The Window these resources are rendering to
	 */
	HWND window;
	/**
	 * The OS Instance that is overseeing this
	 */
	HINSTANCE instance;
	/**
	 * Has this Object been fully initialized
	 */
	bool isInit;
	/**
	 * Have the default shaders been prepared
	 */
	bool defaultShadersSet;
	/**
	 * Used to track the dimensions of the window
	 */
	RECT Location;
	

	// Window Level Direct3D Resources

	/**
	 * Used to retrieve other resources
	 */
	TrecComPointer<ID3D11Device> graphicsDevice;
	/**
	 * Retrieves the Adapter
	 */
	TrecComPointer<IDXGIDevice> dxDev;
	/**
	 * Used to allow integration with Direct2D
	 */
	TrecComPointer<IDXGISurface1> surface;
	/**
	 * Sets configuration for 3D drawing
	 */
	TrecComPointer<ID3D11DeviceContext> contextDevice;
	/**
	 * Holds buffer and used to present it to the system
	 */
	TrecComPointer<IDXGISwapChain> swapper;
	/**
	 * The render Target
	 */
	TrecComPointer<ID3D11RenderTargetView> renderTarget;
	/**
	 * The driver to use (might not currently be in use
	 */
	D3D_DRIVER_TYPE driver;
	/**
	 * Desired Direct3D version
	 */
	D3D_FEATURE_LEVEL version;

	/**
	 * The Rasterization descriptor
	 */
	D3D11_RASTERIZER_DESC rasterizer;
	/**
	 * the Rasterization object
	 */
	TrecComPointer<ID3D11RasterizerState> rasterizerState;
	/**
	 * Whether the engine is set to fill or not
	 */
	bool isFill; // Solid or wire-frame?
	/**
	 * Whether CULL should be front or back
	 */
	CULL_MODE cullMode;
};

