#include "TComputer.h"
#include <DirectoryInterface.h>
#include <d3dcompiler.h>
#include <mfapi.h>

#define VID_FORMAT_COUNT 6

typedef enum class t_format_converter_support
{
	tfcs_regular,	// Supported on Windows 7 without a shader
	tfcs_shader_7,  // Supported on Windows 8 without a shader but Anagame has a shader for Windows 7
	tfcs_shader_8,  // Supported on Windows 10 without a shader but Anagame has a shader for Windows 7/8
	tfcs_shader,		// No direct support but Anagame has a shader to use
	tfcs_not_supported
};

/**
 * 
 */
class TFormat
{
public:

	class TFormatMap
	{
	public:
		TFormatMap(TFormat* targetMap, t_format_converter_support support)
		{
			format = targetMap;
			this->support = support;
		}
		TFormatMap(const TFormatMap& orig)
		{
			this->format = orig.format;
			this->support = orig.support;
		}
		TFormatMap()
		{
			this->format = nullptr;
			this->support = t_format_converter_support::tfcs_not_supported;
		}

		TFormat* format;
		t_format_converter_support support;
	};


	TFormat(GUID g)
	{
		mediaType = g;
	}
	TFormat(const TFormat& orig)
	{
		mediaType = orig.mediaType;
		outputs = orig.outputs;
	}

	void AddFormatOutput(const TFormatMap& map)
	{
		outputs.push_back(map);
	}

	TDataArray<TFormatMap> GetOutputMap(GUID id)
	{
		TDataArray<TFormatMap> ret;

		if (GetOutputMap(id, ret, 0))
		{
			TDataArray<TFormatMap> ret2;
			for (UINT Rust = ret.Size() - 1; Rust < ret.Size(); Rust--)
			{
				ret2.push_back(ret[Rust]);
			}
			return ret2;
		}
		return ret;
	}

	GUID GetMediaType()
	{
		return mediaType;
	}

protected:

	bool GetOutputMap(GUID target, TDataArray<TFormatMap>& ret, UINT level)
	{
		// Contingency in case we end up looping. Most conversions should not take more than two sub-conversions
		if (level == 3)
			return false;

		for (UINT Rust = 0; Rust < outputs.Size(); Rust++)
		{
			if (outputs[Rust].format->mediaType == target && IsSupported(outputs[Rust].support))
			{
				ret.push_back(outputs[Rust]);
				return true;
			}
		}

		for (UINT Rust = 0; Rust < outputs.Size(); Rust++)
		{
			if (outputs[Rust].format->GetOutputMap(target, ret, level + 1))
			{
				ret.push_back(outputs[Rust]);
				return true;
			}
		}
		return false;
	}

	bool IsSupported(t_format_converter_support target)
	{
		// To-Do - have it based off of what Operating System is running and if shader is needed, is shader available
		// For now, only return whether Windows 8 can run it without a shader
		return target == t_format_converter_support::tfcs_regular || target == t_format_converter_support::tfcs_shader_7;
	}

	GUID mediaType;
	TDataArray<TFormatMap> outputs;
};


TFormat vidFormats[VID_FORMAT_COUNT] = {
	TFormat(MFVideoFormat_ARGB32),
	TFormat(MFVideoFormat_RGB32),
	TFormat(MFVideoFormat_RGB24),
	TFormat(MFVideoFormat_YV12),
	TFormat(MFVideoFormat_NV11),
	TFormat(MFVideoFormat_NV12)
};


void SetVideoFormats()
{
	static bool set = false;
	if (!set)
	{
		vidFormats[3].AddFormatOutput(TFormat::TFormatMap(&vidFormats[0], t_format_converter_support::tfcs_shader_7));
		vidFormats[3].AddFormatOutput(TFormat::TFormatMap(&vidFormats[1], t_format_converter_support::tfcs_shader_7));
		vidFormats[3].AddFormatOutput(TFormat::TFormatMap(&vidFormats[2], t_format_converter_support::tfcs_shader_7));

		vidFormats[4].AddFormatOutput(TFormat::TFormatMap(&vidFormats[3], t_format_converter_support::tfcs_shader_7));
		vidFormats[5].AddFormatOutput(TFormat::TFormatMap(&vidFormats[3], t_format_converter_support::tfcs_shader_7));

		set = true;
	}
}


TDataArray<TFormat::TFormatMap> IsConversionSupported(GUID input, GUID output)
{
	SetVideoFormats();

	for (UINT Rust = 0; Rust < VID_FORMAT_COUNT; Rust++)
	{
		if (vidFormats[Rust].GetMediaType() == input)
			return vidFormats[Rust].GetOutputMap(output);
	}

	return TDataArray<TFormat::TFormatMap>();
}




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

HRESULT TComputer::ConvertMediaFormats(IMFMediaType* inputType, GUID outputType, IMFSample* input, IMFSample** output)
{
	return E_NOTIMPL;
}

bool TComputer::IsConversionSupported(GUID input, GUID output)
{
	return false;
}


