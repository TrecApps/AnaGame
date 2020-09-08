#include "TArenaEngine.h"
#include "ArenaModel.h"

/**
 * Method: TArenaEngine::TArenaEngine
 * Purpose: Constructor
 * Parameters: TrecPointer<TWindowEngine> wEngine - the Window Level Engine to work with
 * Returns: New Arena Engine
 */
TArenaEngine::TArenaEngine(TrecPointer<TWindowEngine> wEngine)
{
	windowEngine = wEngine;
	ZeroMemory(&rasterizer, sizeof(rasterizer));
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.CullMode = D3D11_CULL_BACK;
}

/**
 * Method: TArenaEngine::
 * Purpose: Constructor
 * Parameters: TrecPointer<TWindowEngine> wEngine - the Window Level Engine to work with
 *				const TString& name - the name to give this Engine
 * Returns: New Arena Engine
 */
TArenaEngine::TArenaEngine(TrecPointer<TWindowEngine> wEngine, const TString& name)
{
	windowEngine = wEngine;
	engineName.Set(name);

	ZeroMemory(&rasterizer, sizeof(rasterizer));
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.CullMode = D3D11_CULL_BACK;
}

/**
 * Method: TArenaEngine::~TArenaEngine
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TArenaEngine::~TArenaEngine()
{
	int e = 0;
}

/**
 * Method: TArenaEngine::SetShader
 * Purpose: Sets Direct3D to use an external shader
 * Parameters: int id - the external shader to begin using
 * Returns: bool - success or failure
 */
bool TArenaEngine::SetShader(int id)
{
	return assignShader(id, camera, mvp, windowEngine->getDeviceD(), windowEngine->getDevice(), doMvp);
}

/**
 * Method: TArenaEngine::
 * Purpose: Sets Direct3D to use an Anagame shader
 * Parameters: DefaultShader ds - the Anagame shader to begin using
 * Returns: bool - success or failure
 */
bool TArenaEngine::SetShader(DefaultShader ds)
{
	return windowEngine->assignShader(static_cast<UINT>(ds), camera, mvp, windowEngine->getDeviceD(), windowEngine->getDevice(), doMvp);
}

/**
 * Method: TArenaEngine::SetNewWindowEngine
 * Purpose: Assigns this engine to a new Window
 * Parameters: TrecPointer<TWindowEngine> wEngine - the engine to start using
 * Returns: void
 */
void TArenaEngine::SetNewWindowEngine(TrecPointer<TWindowEngine> wEngine)
{
	windowEngine = wEngine;

	// To-Do: Create New Shaders to use the new resources
}

/**
 * Method: TArenaEngine::RenderScene
 * Purpose: Goes through all of the models and renders them
 * Parameters: DirectX::XMMATRIX& proj - The projection matrix
 *				DirectX::XMMATRIX& cam - the Camera matrix
 *				D3D11_VIEWPORT& viewPort - location on the screen to draw
 * Returns: void
 */
void TArenaEngine::RenderScene(DirectX::XMMATRIX& proj, DirectX::XMMATRIX& cam, D3D11_VIEWPORT& viewPort)
{
	if (!windowEngine.Get())
		return;
	TrecComPointer<ID3D11DeviceContext> contextDevice = windowEngine->getDevice();
	if (!contextDevice.Get())
		return;

	contextDevice->RSSetViewports(1, &viewPort);

	//contextDevice->ClearDepthStencilView(depthStensil, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//currentCamera = m;
	mvp = proj;
	camera = cam;

	for (UINT c = 0; c < models.Size(); c++)
	{
		if (models[c].Get())
			models[c]->Render(proj, cam);
	}
}

/**
 * Method: TArenaEngine::AddModel
 * Purpose: Adds a model to draw
 * Parameters: TrecPointer<ArenaModel> ae - the model to add
 * Returns: void
 */
void TArenaEngine::AddModel(TrecPointer<ArenaModel> ae)
{
	for (UINT C = 0; C < models.Size(); C++)
	{
		if (models[C].Get() == ae.Get())
		{
			return;
		}
	}
	models.push_back(ae);
}

/**
 * Method: TArenaEngine::RemoveModel
 * Purpose: Removes a model, if it is currently held
 * Parameters: TrecPointer<ArenaModel> ae - the model to remove
 * Returns: void
 */
void TArenaEngine::RemoveModel(TrecPointer<ArenaModel> ae)
{
	for (UINT C = 0; C < models.Size(); C++)
	{
		if (models[C].Get() == ae.Get())
		{
			models.RemoveAt(C);
			return;
		}
	}
}


/**
 * Method: TArenaEngine::GetModel
 * Purpose: Retrieves the model at the specified index
 * Parameters: UINT Rust - the Index of the model
 * Returns: TrecPointer<ArenaModel> - the model requested, null if index out of bounds
 */
TrecPointer<ArenaModel> TArenaEngine::GetModel(UINT Rust)
{
	if (Rust >= models.Size())
		return TrecPointer<ArenaModel>();
	return models[Rust];
}

/**
 * Method: TArenaEngine::getDevice
 * Purpose: Retrieves the Device Context (responsible for managing buffers and constant buffers)
 * Parameters: void
 * Returns: TrecComPointer<ID3D11DeviceContext> - the device context
 */
TrecComPointer<ID3D11DeviceContext> TArenaEngine::getDevice()
{
	return windowEngine->getDevice();
}

/**
 * Method: TArenaEngine::getSwapChain
 * Purpose: Retrieves the SwapChain, which manages screen buffers and presents the picture to the system.
 * Parameters: void
 * Returns: TrecComPointer<IDXGISwapChain> - the Swap Chain
 */
TrecComPointer<IDXGISwapChain> TArenaEngine::getSwapChain()
{
	return windowEngine->getSwapChain();
}

/**
 * Method: TArenaEngine::getRederTarget
 * Purpose: Retrieves the 3D Render Target
 * Parameters: void
 * Returns: TrecComPointer<ID3D11RenderTargetView> - the render target
 */
TrecComPointer<ID3D11RenderTargetView> TArenaEngine::getRederTarget()
{
	return windowEngine->getRederTarget();
}

/**
 * Method: TArenaEngine::getDeviceD
 * Purpose: Retrieves the D3D Devicee
 * Parameters: void
 * Returns: TrecComPointer<ID3D11Device> - the D3D Device
 */
TrecComPointer<ID3D11Device> TArenaEngine::getDeviceD()
{
	return windowEngine->getDeviceD();
}

/**
 * Method: TArenaEngine::getDeviceD_U
 * Purpose: Retrieves the DXGI Device
 * Parameters: void
 * Returns: TrecComPointer<IDXGIDevice> - the DXGI Device
 */
TrecComPointer<IDXGIDevice> TArenaEngine::getDeviceD_U()
{
	return windowEngine->getDeviceD_U();
}

/**
 * Method: TArenaEngine::GetSurface
 * Purpose: Retrieves the DXGI Surface
 * Parameters: void
 * Returns: TrecComPointer<IDXGISurface1> - the surface requested
 */
TrecComPointer<IDXGISurface1> TArenaEngine::GetSurface()
{
	return windowEngine->GetSurface();
}

/**
 * Method: TArenaEngine::getBufferSize
 * Purpose: Retrieves the byte size of the input buffer
 * Parameters: DefaultShader shaderID - the shader to get this information of
 * Returns: int - the size of the buffer
 */
int TArenaEngine::getBufferSize(DefaultShader shaderID)
{
	return windowEngine->getBufferSize(static_cast<int>(shaderID));
}

/**
 * Method: TArenaEngine::getColorBufferLocation
 * Purpose: Retrieves the index of the color buffer in the external shader
 * Parameters: int shaderID - the index of the external shader
 * Returns: signed char - index of the color buffer, if present, in the external shader
 */
signed char TArenaEngine::getColorBufferLocation(int shaderID)
{
	return getColorBufferLocation_(shaderID);
}

/**
 * Method: TArenaEngine::getColorBufferLocation
 * Purpose: Retrieves the index of the color buffer in the Anagame shader
 * Parameters: DefaultShader shaderID - the Anagame shader to request info from
 * Returns: signed char - index of the color buffer, if present, in the Anagame shader
 */
signed char TArenaEngine::getColorBufferLocation(DefaultShader shaderID)
{
	return windowEngine->getColorBufferLocation_(static_cast<int>(shaderID));
}

/**
 * Method: TArenaEngine::ReplaceConstantBuffer
 * Purpose: Replaces the current buffer
 * Parameters: int shaderID - the shader to update
 *				unsigned char slot - the slot of the constant buffer
 *				TrecComPointer<ID3D11Buffer> buff the buffer with the new data
 * Returns: int - error code
 */
int TArenaEngine::ReplaceConstantBuffer(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff)
{
	return ReplaceConstantBuffer_(shaderID, slot, buff);
}

/**
 * Method: TArenaEngine::ReplaceConstantBuffer
 * Purpose: Replaces the current buffer
 * Parameters: DefaultShader shaderID - the shader to update
 *				unsigned char slot - the slot of the constant buffer
 *				TrecComPointer<ID3D11Buffer> buff the buffer with the new data
 * Returns: int - error code
 */
int TArenaEngine::ReplaceConstantBuffer(DefaultShader shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff)
{
	return windowEngine->ReplaceConstantBuffer_(static_cast<int>(shaderID), slot, buff);
}

/**
 * Method: ArenaEngine::ModelSetFillMode
 * Purpose: Sets the Fill mode of this engine
 * Parameters: D3D11_FILL_MODE fill - the fill mode to set the engine to
 * Return: void
 */
void TArenaEngine::ModelSetFillMode(D3D11_FILL_MODE fill)
{
	if (!windowEngine.Get())
		return;
	auto contextDevice = windowEngine->getDevice();
	auto graphicsDevice = windowEngine->getDeviceD();
	if (!contextDevice.Get())
		return;
	rasterizer.FillMode = fill;
	TrecComPointer<ID3D11RasterizerState>::TrecComHolder state;
	graphicsDevice->CreateRasterizerState(&rasterizer, state.GetPointerAddress());
	rasterizerState = state.Extract();
	contextDevice->RSSetState(rasterizerState.Get());
}

/**
 * Method: TArenaEngine::DoMvp
 * Purpose: Reports whether the software needs to perform the MVP calculation
 * Parameters: void
 * Returns: bool - whether the software need to calculate the MVP
 */
bool TArenaEngine::DoMvp()
{
	return doMvp;
}

/**
 * Method: TArenaEngine::GetModelLocation
 * Purpose: Retrieves the index of the Model buffer in the specified Anagame shader
 * Parameters: DefaultShader shaderID - the index of the Anagame shader
 * Returns: signed char -  the index of the Model buffer in the Anagame shader
 */
signed char TArenaEngine::GetModelLocation(DefaultShader shaderID)
{
	if (!windowEngine.Get()) return -2;
	return windowEngine->GetModelLocation_(static_cast<int>(shaderID));
}

/**
 * Method: TArenaEngine::GetViewLocation
 * Purpose: Retrieves the index of the View buffer in the specified Anagame shader
 * Parameters: DefaultShader shaderID - the index of the Anagame shader
 * Returns: signed char -  the index of the View buffer in the Anagame shader
 */
signed char TArenaEngine::GetViewLocation(DefaultShader shaderID)
{
	if (!windowEngine.Get()) return -2;
	return windowEngine->GetViewLocation_(static_cast<int>(shaderID));
}

/**
 * Method: TArenaEngine::GetCameraLocation
 * Purpose: Retrieves the index of the Camera buffer in the specified Anagame shader
 * Parameters: DefaultShader shaderID - the index of the Anagame shader
 * Returns: signed char -  the index of the Camera buffer in the Anagame shader
 */
signed char TArenaEngine::GetCameraLocation(DefaultShader shaderID)
{
	if (!windowEngine.Get()) return -2;
	return windowEngine->GetCameraLocation_(static_cast<int>(shaderID));
}

/**
 * Method: TArenaEngine::GetModelLocation
 * Purpose: Retrieves the index of the Model buffer in the specified external shader
 * Parameters: int shaderID - the index of the external shader
 * Returns: signed char -  the index of the Model buffer in the external shader
 */
signed char TArenaEngine::GetModelLocation(int shaderID)
{
	return GetModelLocation_(shaderID);
}

/**
 * Method: TArenaEngine::GetViewLocation
 * Purpose: Retrieves the index of the View buffer in the specified external shader
 * Parameters:  int shaderID - the index of the external shader
 * Returns: signed char -  the index of the View buffer in the external shader
 */
signed char TArenaEngine::GetViewLocation(int shaderID)
{
	return GetViewLocation_(shaderID);
}

/**
 * Method: TArenaEngine::GetCameraLocation
 * Purpose: Retrieves the index of the Camera buffer in the specified external shader
 * Parameters: int shaderID - the index of the external shader
 * Returns: signed char -  the index of the Camera buffer in the external shader
 */
signed char TArenaEngine::GetCameraLocation(int shaderID)
{
	return GetCameraLocation_(shaderID);
}

/**
 * Method: TArenaEngine::GetName
 * Purpose: Retrieves the name of the arena
 * Parameters: void
 * Returns: TString - the name of the arena
 */
TString TArenaEngine::GetName()
{
	return engineName;
}

/**
 * Method: TArenaEngine::GetModelList
 * Purpose: Retrieves a pointer to the model list
 * Parameters: void
 * Returns: TTrecPointerArray<ArenaModel>* - pointer to the model list
 *
 * Note: Because this is a raw pointer, this should eventually be replaced
 */
TTrecPointerArray<ArenaModel>* TArenaEngine::GetModelList()
{
	return &models;
}
