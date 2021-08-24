#pragma once
#include "TWindowEngine.h"
#include "TShaderHost.h"
#include <TTrecPointerArray.h>
class ArenaModel;

/*
 * Class: TArenaEngine
 * Purpose: Holds a list of models as well as 3rd party shaders.
 *
 * SuperClass: TShaderHost - allows arena engine to host shaders
 */
class _VIDEO_GRAPHICS TArenaEngine :
	public TShaderHost
{
public:


	/**
	 * Method: TArenaEngine::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TArenaEngine::TArenaEngine
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TWindowEngine> wEngine - the Window Level Engine to work with
	 * Returns: New Arena Engine
	 */
	TArenaEngine(TrecPointer<TWindowEngine> wEngine);
	/**
	 * Method: TArenaEngine::
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TWindowEngine> wEngine - the Window Level Engine to work with
	 *				const TString& name - the name to give this Engine
	 * Returns: New Arena Engine
	 */
	TArenaEngine(TrecPointer<TWindowEngine> wEngine, const TString& name);
	/**
	 * Method: TArenaEngine::~TArenaEngine
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TArenaEngine();
	/**
	 * Method: TArenaEngine::SetShader
	 * Purpose: Sets Direct3D to use an external shader
	 * Parameters: int id - the external shader to begin using
	 * Returns: bool - success or failure
	 */
	bool SetShader(int id);
	/**
	 * Method: TArenaEngine::
	 * Purpose: Sets Direct3D to use an Anagame shader
	 * Parameters: DefaultShader ds - the Anagame shader to begin using
	 * Returns: bool - success or failure
	 */
	bool SetShader(DefaultShader ds);

	/**
	 * Method: TArenaEngine::SetNewWindowEngine
	 * Purpose: Assigns this engine to a new Window
	 * Parameters: TrecPointer<TWindowEngine> wEngine - the engine to start using
	 * Returns: void
	 */
	void SetNewWindowEngine(TrecPointer<TWindowEngine> wEngine);

	/**
	 * Method: TArenaEngine::RenderScene
	 * Purpose: Goes through all of the models and renders them
	 * Parameters: DirectX::XMMATRIX& proj - The projection matrix
	 *				DirectX::XMMATRIX& cam - the Camera matrix
	 *				D3D11_VIEWPORT& viewPort - location on the screen to draw
	 * Returns: void
	 */
	void RenderScene(DirectX::XMMATRIX& proj, DirectX::XMMATRIX& cam, D3D11_VIEWPORT& viewPort);
	/**
	 * Method: TArenaEngine::AddModel
	 * Purpose: Adds a model to draw
	 * Parameters: TrecPointer<ArenaModel> ae - the model to add
	 * Returns: void
	 */
	void AddModel(TrecPointer<ArenaModel> ae);
	/**
	 * Method: TArenaEngine::RemoveModel
	 * Purpose: Removes a model, if it is currently held
	 * Parameters: TrecPointer<ArenaModel> ae - the model to remove
	 * Returns: void
	 */
	void RemoveModel(TrecPointer<ArenaModel> ae);
	/**
	 * Method: TArenaEngine::GetModel
	 * Purpose: Retrieves the model at the specified index
	 * Parameters: UINT Rust - the Index of the model
	 * Returns: TrecPointer<ArenaModel> - the model requested, null if index out of bounds
	 */
	TrecPointer<ArenaModel> GetModel(UINT Rust);

	// Direct3D Get Methods

	/**
	 * Method: TArenaEngine::getDevice
	 * Purpose: Retrieves the Device Context (responsible for managing buffers and constant buffers)
	 * Parameters: void
	 * Returns: TrecComPointer<ID3D11DeviceContext> - the device context
	 */
	TrecComPointer<ID3D11DeviceContext> getDevice();
	/**
	 * Method: TArenaEngine::getSwapChain
	 * Purpose: Retrieves the SwapChain, which manages screen buffers and presents the picture to the system.
	 * Parameters: void
	 * Returns: TrecComPointer<IDXGISwapChain> - the Swap Chain
	 */
	TrecComPointer<IDXGISwapChain> getSwapChain();
	/**
	 * Method: TArenaEngine::getRederTarget
	 * Purpose: Retrieves the 3D Render Target
	 * Parameters: void
	 * Returns: TrecComPointer<ID3D11RenderTargetView> - the render target
	 */
	TrecComPointer<ID3D11RenderTargetView> getRederTarget();
	/**
	 * Method: TArenaEngine::getDeviceD
	 * Purpose: Retrieves the D3D Devicee
	 * Parameters: void
	 * Returns: TrecComPointer<ID3D11Device> - the D3D Device
	 */
	TrecComPointer<ID3D11Device> getDeviceD();
	/**
	 * Method: TArenaEngine::getDeviceD_U
	 * Purpose: Retrieves the DXGI Device
	 * Parameters: void
	 * Returns: TrecComPointer<IDXGIDevice> - the DXGI Device
	 */
	TrecComPointer<IDXGIDevice> getDeviceD_U();
	/**
	 * Method: TArenaEngine::GetSurface
	 * Purpose: Retrieves the DXGI Surface
	 * Parameters: void
	 * Returns: TrecComPointer<IDXGISurface1> - the surface requested
	 */
	TrecComPointer<IDXGISurface1> GetSurface();

	/**
	 * Method: TArenaEngine::getBufferSize
	 * Purpose: Retrieves the byte size of the input buffer
	 * Parameters: DefaultShader shaderID - the shader to get this information of
	 * Returns: int - the size of the buffer
	 */
	int getBufferSize(DefaultShader shaderID);

	/**
	 * Method: TArenaEngine::getColorBufferLocation
	 * Purpose: Retrieves the index of the color buffer in the external shader
	 * Parameters: int shaderID - the index of the external shader
	 * Returns: signed char - index of the color buffer, if present, in the external shader
	 */
	signed char getColorBufferLocation(int shaderID);
	/**
	 * Method: TArenaEngine::getColorBufferLocation
	 * Purpose: Retrieves the index of the color buffer in the Anagame shader
	 * Parameters: DefaultShader shaderID - the Anagame shader to request info from
	 * Returns: signed char - index of the color buffer, if present, in the Anagame shader
	 */
	signed char getColorBufferLocation(DefaultShader shaderID);

	/**
	 * Method: TArenaEngine::ReplaceConstantBuffer
	 * Purpose: Replaces the current buffer
	 * Parameters: int shaderID - the shader to update
	 *				unsigned char slot - the slot of the constant buffer
	 *				TrecComPointer<ID3D11Buffer> buff the buffer with the new data
	 * Returns: int - error code
	 */
	int ReplaceConstantBuffer(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);
	/**
	 * Method: TArenaEngine::ReplaceConstantBuffer
	 * Purpose: Replaces the current buffer
	 * Parameters: DefaultShader shaderID - the shader to update
	 *				unsigned char slot - the slot of the constant buffer
	 *				TrecComPointer<ID3D11Buffer> buff the buffer with the new data
	 * Returns: int - error code
	 */
	int ReplaceConstantBuffer(DefaultShader shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);

	/**
	 * Method: ArenaEngine::ModelSetFillMode
	 * Purpose: Sets the Fill mode of this engine
	 * Parameters: D3D11_FILL_MODE fill - the fill mode to set the engine to
	 * Return: void
	 */
	void ModelSetFillMode(D3D11_FILL_MODE fill);
	/**
	 * Method: TArenaEngine::DoMvp
	 * Purpose: Reports whether the software needs to perform the MVP calculation
	 * Parameters: void
	 * Returns: bool - whether the software need to calculate the MVP
	 */
	bool DoMvp();

	/**
	 * Method: TArenaEngine::GetModelLocation
	 * Purpose: Retrieves the index of the Model buffer in the specified Anagame shader
	 * Parameters: DefaultShader shaderID - the index of the Anagame shader
	 * Returns: signed char -  the index of the Model buffer in the Anagame shader
	 */
	signed char GetModelLocation(DefaultShader shaderID);
	/**
	 * Method: TArenaEngine::GetViewLocation
	 * Purpose: Retrieves the index of the View buffer in the specified Anagame shader
	 * Parameters: DefaultShader shaderID - the index of the Anagame shader
	 * Returns: signed char -  the index of the View buffer in the Anagame shader
	 */
	signed char GetViewLocation(DefaultShader shaderID);
	/**
	 * Method: TArenaEngine::GetCameraLocation
	 * Purpose: Retrieves the index of the Camera buffer in the specified Anagame shader
	 * Parameters: DefaultShader shaderID - the index of the Anagame shader
	 * Returns: signed char -  the index of the Camera buffer in the Anagame shader
	 */
	signed char GetCameraLocation(DefaultShader shaderID);

	/**
	 * Method: TArenaEngine::GetModelLocation
	 * Purpose: Retrieves the index of the Model buffer in the specified external shader
	 * Parameters: int shaderID - the index of the external shader
	 * Returns: signed char -  the index of the Model buffer in the external shader
	 */
	signed char GetModelLocation(int shaderID);
	/**
	 * Method: TArenaEngine::GetViewLocation
	 * Purpose: Retrieves the index of the View buffer in the specified external shader
	 * Parameters:  int shaderID - the index of the external shader
	 * Returns: signed char -  the index of the View buffer in the external shader
	 */
	signed char GetViewLocation(int shaderID);
	/**
	 * Method: TArenaEngine::GetCameraLocation
	 * Purpose: Retrieves the index of the Camera buffer in the specified external shader
	 * Parameters: int shaderID - the index of the external shader
	 * Returns: signed char -  the index of the Camera buffer in the external shader
	 */
	signed char GetCameraLocation(int shaderID);

	/**
	 * Method: TArenaEngine::GetName
	 * Purpose: Retrieves the name of the arena
	 * Parameters: void
	 * Returns: TString - the name of the arena
	 */
	TString GetName();

	/**
	 * Method: TArenaEngine::GetModelList
	 * Purpose: Retrieves a pointer to the model list
	 * Parameters: void
	 * Returns: TTrecPointerArray<ArenaModel>* - pointer to the model list
	 * 
	 * Note: Because this is a raw pointer, this should eventually be replaced
	 */
	TTrecPointerArray<ArenaModel>* GetModelList();

	/**
	 * Method: TArenaEngine::GetInputInfo
	 * Purpose: allows code to access info about the vertex data specs of a given external shader
	 * Parameters: int shaderId - the shader to access
	 *				UINT& count - number of elements in the returned array
	 * Returns: D3D11_INPUT_ELEMENT_DESC* - pointer to the description map (if null, then no such shader was available and 'count' will be set to 0)
	 */
	D3D11_INPUT_ELEMENT_DESC* GetInputInfo(int shaderId, UINT& count);


	/**
	 * Method: TArenaEngine::GetInputInfo
	 * Purpose: allows code to access info about the vertex data specs of a given anagame shader
	 * Parameters: DefaultShader shaderId - the shader to access
	 *				UINT& count - number of elements in the returned array
	 * Returns: D3D11_INPUT_ELEMENT_DESC* - pointer to the description map (if null, then no such shader was available and 'count' will be set to 0)
	 */
	D3D11_INPUT_ELEMENT_DESC* GetInputInfo(DefaultShader shaderId, UINT& count);
protected:
	/**
	 * the Window Level Engine to work with (holds D3D Resources)
	 */
	TrecPointer<TWindowEngine> windowEngine;
	/**
	 * the name of the engine
	 */
	TString engineName;
	/**
	 * the current camera and projection matrix, updated by each TArena
	 */
	DirectX::XMMATRIX camera, mvp;

	/**
	 * list of models held by the engine
	 */
	TTrecPointerArray<ArenaModel> models;
	/**
	 * Details of the rasterizer
	 */
	D3D11_RASTERIZER_DESC rasterizer;
	/**
	 * Current Rasterizer
	 */
	TrecComPointer<ID3D11RasterizerState> rasterizerState;
	/**
	 * Whether the software needs to perform the MVP calculation
	 */
	bool doMvp;
};

