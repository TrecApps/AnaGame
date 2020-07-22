#include "TArenaEngine.h"
#include "ArenaModel.h"

TArenaEngine::TArenaEngine(TrecPointer<TWindowEngine> wEngine)
{
	windowEngine = wEngine;
	ZeroMemory(&rasterizer, sizeof(rasterizer));
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.CullMode = D3D11_CULL_BACK;
}

TArenaEngine::TArenaEngine(TrecPointer<TWindowEngine> wEngine, const TString& name)
{
	windowEngine = wEngine;
	engineName.Set(name);

	ZeroMemory(&rasterizer, sizeof(rasterizer));
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.CullMode = D3D11_CULL_BACK;
}

TArenaEngine::~TArenaEngine()
{
	int e = 0;
}

bool TArenaEngine::SetShader(int id)
{
	return assignShader(id, camera, mvp, windowEngine->getDeviceD(), windowEngine->getDevice(), doMvp);
}

bool TArenaEngine::SetShader(DefaultShader ds)
{
	return windowEngine->assignShader(static_cast<UINT>(ds), camera, mvp, windowEngine->getDeviceD(), windowEngine->getDevice(), doMvp);
}

void TArenaEngine::SetNewWindowEngine(TrecPointer<TWindowEngine> wEngine)
{
	windowEngine = wEngine;

	// To-Do: Create New Shaders to use the new resources
}

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


TrecPointer<ArenaModel> TArenaEngine::GetModel(UINT Rust)
{
	if (Rust >= models.Size())
		return TrecPointer<ArenaModel>();
	return models[Rust];
}

TrecComPointer<ID3D11DeviceContext> TArenaEngine::getDevice()
{
	return windowEngine->getDevice();
}

TrecComPointer<IDXGISwapChain> TArenaEngine::getSwapChain()
{
	return windowEngine->getSwapChain();
}

TrecComPointer<ID3D11RenderTargetView> TArenaEngine::getRederTarget()
{
	return windowEngine->getRederTarget();
}

TrecComPointer<ID3D11Device> TArenaEngine::getDeviceD()
{
	return windowEngine->getDeviceD();
}

TrecComPointer<IDXGIDevice> TArenaEngine::getDeviceD_U()
{
	return windowEngine->getDeviceD_U();
}

TrecComPointer<IDXGISurface1> TArenaEngine::GetSurface()
{
	return windowEngine->GetSurface();
}

int TArenaEngine::getBufferSize(DefaultShader shaderID)
{
	return windowEngine->getBufferSize(static_cast<int>(shaderID));
}

signed char TArenaEngine::getColorBufferLocation(int shaderID)
{
	return getColorBufferLocation_(shaderID);
}

signed char TArenaEngine::getColorBufferLocation(DefaultShader shaderID)
{
	return windowEngine->getColorBufferLocation_(static_cast<int>(shaderID));
}

int TArenaEngine::ReplaceConstantBuffer(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff)
{
	return ReplaceConstantBuffer_(shaderID, slot, buff);
}

int TArenaEngine::ReplaceConstantBuffer(DefaultShader shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff)
{
	return windowEngine->ReplaceConstantBuffer_(static_cast<int>(shaderID), slot, buff);
}

/*
* Method: ArenaEngine - ModelSetFillMode
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

bool TArenaEngine::DoMvp()
{
	return doMvp;
}

signed char TArenaEngine::GetModelLocation(DefaultShader shaderID)
{
	if (!windowEngine.Get()) return -2;
	return windowEngine->GetModelLocation_(static_cast<int>(shaderID));
}

signed char TArenaEngine::GetViewLocation(DefaultShader shaderID)
{
	if (!windowEngine.Get()) return -2;
	return windowEngine->GetViewLocation_(static_cast<int>(shaderID));
}

signed char TArenaEngine::GetCameraLocation(DefaultShader shaderID)
{
	if (!windowEngine.Get()) return -2;
	return windowEngine->GetCameraLocation_(static_cast<int>(shaderID));
}

signed char TArenaEngine::GetModelLocation(int shaderID)
{
	return GetModelLocation_(shaderID);
}

signed char TArenaEngine::GetViewLocation(int shaderID)
{
	return GetViewLocation_(shaderID);
}

signed char TArenaEngine::GetCameraLocation(int shaderID)
{
	return GetCameraLocation_(shaderID);
}

TString TArenaEngine::GetName()
{
	return engineName;
}

TTrecPointerArray<ArenaModel>* TArenaEngine::GetModelList()
{
	return &models;
}
