#include "TComputer.h"
#include <DirectoryInterface.h>
#include <d3dcompiler.h>

TComputer::TComputer(TrecComPointer<ID3D11DeviceContext> devContext)
{
	this->devContext = devContext;

	TString computeShaderDir(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\Compute\\");

	TString nv12torgba8Path(computeShaderDir + L"nv12torgba8.hlsl");

	ID3D11Device* device = nullptr;
	devContext->GetDevice(&device);

	ID3DBlob* buffer = nullptr, * errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	HRESULT res = D3DCompileFromFile(nv12torgba8Path.GetConstantBuffer().getBuffer(), 0, 0, "csmain", "vs_5_0", flags, 0, &buffer, &errorBuffer);

	assert(SUCCEEDED(res));

	TrecComPointer<ID3D11ComputeShader>::TrecComHolder css;
	res = device->CreateComputeShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, css.GetPointerAddress());
	this->nv12toRgba8 = css.Extract();
	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;




	device->Release();
}

TString TComputer::ConvertNV12ToRGBA8(BYTE* nv12, UINT byteLength, BYTE** rgba8, UINT& newByteLength)
{




	return TString();
}
