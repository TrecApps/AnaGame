#pragma once
#include "TWindowEngine.h"
#include "TShaderHost.h"
#include <TTrecPointerArray.h>
class ArenaModel;

/*
 * class TArenaEngine
 * Purpose: Holds a list of models as well as 3rd party shaders.  
 */
class _VIDEO_GRAPHICS TArenaEngine :
	public TShaderHost
{
public:
	TArenaEngine(TrecPointer<TWindowEngine> wEngine);
	TArenaEngine(TrecPointer<TWindowEngine> wEngine, const TString& name);
	~TArenaEngine();
	bool SetShader(int id);
	bool SetShader(DefaultShader ds);

	void SetNewWindowEngine(TrecPointer<TWindowEngine> wEngine);

	void RenderScene(DirectX::XMMATRIX& proj, DirectX::XMMATRIX& cam, D3D11_VIEWPORT& viewPort);
	void AddModel(TrecPointer<ArenaModel> ae);
	void RemoveModel(TrecPointer<ArenaModel> ae);
	TrecPointer<ArenaModel> GetModel(UINT Rust);

	// Direct3D Get Methods
	TrecComPointer<ID3D11DeviceContext> getDevice();
	TrecComPointer<IDXGISwapChain> getSwapChain();
	TrecComPointer<ID3D11RenderTargetView> getRederTarget();
	TrecComPointer<ID3D11Device> getDeviceD();
	TrecComPointer<IDXGIDevice> getDeviceD_U();
	TrecComPointer<IDXGISurface1> GetSurface();

	int getBufferSize(DefaultShader shaderID);

	signed char getColorBufferLocation(int shaderID);
	signed char getColorBufferLocation(DefaultShader shaderID);

	int ReplaceConstantBuffer(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);
	int ReplaceConstantBuffer(DefaultShader shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);
	void ModelSetFillMode(D3D11_FILL_MODE fill);
	bool DoMvp();

	signed char GetModelLocation(DefaultShader shaderID);
	signed char GetViewLocation(DefaultShader shaderID);
	signed char GetCameraLocation(DefaultShader shaderID);

	signed char GetModelLocation(int shaderID);
	signed char GetViewLocation(int shaderID);
	signed char GetCameraLocation(int shaderID);

	TString GetName();

	TTrecPointerArray<ArenaModel>* GetModelList();
protected:
	TrecPointer<TWindowEngine> windowEngine;
	TString engineName;
	DirectX::XMMATRIX camera, mvp;

	TTrecPointerArray<ArenaModel> models;
	D3D11_RASTERIZER_DESC rasterizer;
	TrecComPointer<ID3D11RasterizerState> rasterizerState;
	bool doMvp;
};

