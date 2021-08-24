#include "TShaderHost.h"
#include <d3dcompiler.h>

const CHAR* _sem_V = "POSITION";
const CHAR* _sem_BN = "BINORMAL";
const CHAR* _sem_BW = "BLENDWEIGHT";
const CHAR* _sem_BI = "BLENDINDICES";
const CHAR* _sem_T = "TEXCOORD";
const CHAR* _sem_N = "NORMAL";
const CHAR* _sem_C = "COLOR";
const CHAR* _sem_PT = "POSITIONT";
const CHAR* _sem_TA = "TANGENT";
const CHAR* _sem_F = "FOG";
const CHAR* _sem_TS = "TESSFACTOR";


/**
 * Method: TShaderHost::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TShaderHost::GetType()
{
	return TString(L"TShaderHost;") + TObject::GetType();
}

/**
 * Method: TShaderHost::TShaderHost
 * Purpose: Constructor
 * Parameters: void
 * Returns: New TShaderHost object
 */
TShaderHost::TShaderHost()
{
}

/**
 * Method: TShaderHost::getBufferSize
 * Purpose: Retrieves the buffer of a single round of Vertex shader input
 * Parameters: int shaderID - the index of the shader to use
 * Return: int -  the buffer size
 */
int TShaderHost::getBufferSize(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaders.Size())
		return 0;

	return shaders[shaderID].bufferSize;
}

/**
 * Method: TShaderHost::SetNewBasicShader
 * Purpose: Sets up new Basic shader, sporting both Vertex and Pixel Shaders
 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - object managing the shader creation
 *				UINT index - the target shader to update
 *				TString& vs - file for the Vertex Shader
 *				TString& ps - file for the Pixel Shader
 *				const CHAR* vf - name of the Vertex Shader function
 *				const CHAR* sf - name of the Pixel Shader function
 *				TDataArray<unsigned short>& bufferDesc - Data about the input expected
 * Returns: int - error code
 */
int TShaderHost::SetNewBasicShader(TrecComPointer<ID3D11Device> graphicsDevice, UINT index, TString& vs, TString& ps, const CHAR* vf, const CHAR* pf, TDataArray<unsigned short>& bufferDesc)
{
	if (index >= shaders.Size())
		return -1;

	if (!graphicsDevice.Get())
		return -2;

	// If Pixel Shader file is empty, assume both shaders are in the Vertex File
	if (!ps.GetSize())
		ps.Set(vs);

	if (!vs.GetSize())
		return -3;

	if (!pf || !vf)
		return -4;

	int test = SetNewBasicShader(vs, vf, bufferDesc, true, index, graphicsDevice);

	if (test)
		return test;
	test = SetNewBasicShader(ps, pf, bufferDesc, false, index, graphicsDevice);

	if (test)
	{
		//newShader.vs->Release();
		shaders[index].vs.Nullify();
		return test;
	}

	return 0;
}
/**
 * Method: TShaderHost::SetNewBasicShader
 * Purpose: Sets up new Basic shader, focusing on either the Pixel or Vertex shader
 * Parameters: TString& s - file for the Shader
 *				const CHAR* f - function name for the Shader
 *				TDataArray<unsigned short>& bufferDesc - Data about the input expected by the vertex shader
 *				bool isVertex - true if focusing on the vertex shader, false for pixel shader
 * 				UINT index - the target shader to update
 *				TrecComPointer<ID3D11Device> graphicsDevice - object managing the shader creation
 * Returns: int - error code
 */
int TShaderHost::SetNewBasicShader(TString& s, const CHAR* f, TDataArray<unsigned short>& bufferDesc, bool isVertex, UINT index, TrecComPointer<ID3D11Device> graphicsDevice)
{
	if (index >= shaders.Size())
		return 1;
	if (!graphicsDevice.Get())
		return 2;
	if (!s.GetSize())
		return 3;
	if (!f)
		return 4;

	ID3DBlob* buffer = nullptr, * errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res;
	WCHAR* sBuff = s.GetBufferCopy();
	if (isVertex)
		res = D3DCompileFromFile(sBuff, 0, 0, f, "vs_4_0", flags, 0, &buffer, &errorBuffer);
	else
		res = D3DCompileFromFile(sBuff, 0, 0, f, "ps_4_0", flags, 0, &buffer, &errorBuffer);
	delete[] sBuff;
	sBuff = nullptr;
	if (!SUCCEEDED(res))
	{
		if (errorBuffer)
		{
			OutputDebugStringA(reinterpret_cast<char*>(errorBuffer->GetBufferPointer()));
			errorBuffer->Release();
			errorBuffer = nullptr;
		}
		if (buffer)
		{
			buffer->Release();
			buffer = nullptr;
		}

		return 5;
	}

	if (isVertex)
	{
		TrecComPointer<ID3D11VertexShader>::TrecComHolder vs;
		TrecComPointer<ID3D11InputLayout>::TrecComHolder il;

		res = graphicsDevice->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, vs.GetPointerAddress());
		shaders[index].vs = vs.Extract();

		// Now handle the input Layout
		TDataArray<D3D11_INPUT_ELEMENT_DESC>* desc = getInputDescription(bufferDesc, shaders[index].bufferSize);
		D3D11_INPUT_ELEMENT_DESC* dec = desc->data();
		shaders[index].layoutError = graphicsDevice->CreateInputLayout(dec, desc->Size(), buffer->GetBufferPointer(), buffer->GetBufferSize(), il.GetPointerAddress());
		shaders[index].layout = il.Extract();
		shaders[index].elements = new D3D11_INPUT_ELEMENT_DESC[(sizeof(D3D11_INPUT_ELEMENT_DESC) * desc->Size())];
		shaders[index].elementCount = desc->Size();
		for (UINT c = 0; c < desc->Size(); c++)
		{
			shaders[index].elements[c] = desc->at(c);
		}
		delete desc;
		desc = nullptr;
	}
	else
	{
		TrecComPointer<ID3D11PixelShader>::TrecComHolder ps;
		res = graphicsDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, ps.GetPointerAddress());
		shaders[index].ps = ps.Extract();
	}

	if (buffer)
		buffer->Release();
	buffer = nullptr;
	if (errorBuffer)
		errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return -3;

	return 0;
}

/**
 * Method: TShaderHost::GetModelLocation_
 * Purpose: Retrieves the index of the Model buffer in a shader
 * Parameters: int shaderID - the index of the shader
 * Returns: signed char - location of the requested buffer (-1 if not in shader, -2 if invalid index)
 */
signed char TShaderHost::GetModelLocation_(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaders.Size())
		return -2;
	return shaders[shaderID].modelLoc;
}

/**
 * Method: TShaderHost::GetViewLocation_
 * Purpose: Retrieves the index of the View buffer in a shader
 * Parameters: int shaderID - the index of the shader
 * Returns: signed char - location of the requested buffer (-1 if not in shader, -2 if invalid index)
 */
signed char TShaderHost::GetViewLocation_(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaders.Size())
		return -2;
	return shaders[shaderID].viewLoc;
}

/**
 * Method: TShaderHost::GetCameraLocation_
 * Purpose: Retrieves the index of the Camera buffer in a shader
 * Parameters: int shaderID - the index of the shader
 * Returns: signed char - location of the requested buffer (-1 if not in shader, -2 if invalid index)
 */
signed char TShaderHost::GetCameraLocation_(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaders.Size())
		return -2;
	return shaders[shaderID].colorLoc;
}

/**
 * Method: TShaderHost::assignShader
 * Purpose: Sets engine up to use a specific shader for rendering
 * Parameters: UINT shaderIndex - index of the target shader
 *				DirectX::XMMATRIX& camera - data for the camera location/direction (fall back if shader doesn't already have it
 *				DirectX::XMMATRIX& currentCameraProj - data for the camera Projection (fall back if shader doesn't supply it)
 *				TrecComPointer<ID3D11Device> graphicsDevice - the device manipulating shaders
 *				TrecComPointer<ID3D11DeviceContext> contextDevice - manages constant buffers
 *				bool& doMvp - whether the Software needs to do the MVP calculation or not
 * Returns:
 */
bool TShaderHost::assignShader(UINT shaderIndex, DirectX::XMMATRIX& camera, DirectX::XMMATRIX& currentCameraProj, TrecComPointer<ID3D11Device> graphicsDevice, TrecComPointer<ID3D11DeviceContext> contextDevice, bool& doMvp)
{
	AG_THREAD_LOCK
	if (!contextDevice.Get())
		throw L"Error! Context Device not initialized!";
	if (shaderIndex >= shaders.Size())
		throw L"Error! Needed shader to exist in the engine!";

	if (shaderIndex < 0 || shaderIndex >= shaders.Size())
	{
		RETURN_THREAD_UNLOCK false;
	}


	ShaderProgram* sp = &(shaders[shaderIndex]);

	if (!sp->layout.Get())
	{
		RETURN_THREAD_UNLOCK false;
	}
	contextDevice->IASetInputLayout(sp->layout.Get());

	contextDevice->CSSetShader(sp->cs.Get(), 0, 0);
	contextDevice->DSSetShader(sp->ds.Get(), 0, 0);
	contextDevice->GSSetShader(sp->gs.Get(), 0, 0);
	contextDevice->HSSetShader(sp->hs.Get(), 0, 0);
	contextDevice->PSSetShader(sp->ps.Get(), 0, 0);
	contextDevice->VSSetShader(sp->vs.Get(), 0, 0);

	if (sp->cameraLoc >= 0 && sp->cameraLoc < sp->constantBuffers.Size())
	{
		if (!sp->constantBuffers[sp->cameraLoc].buff.Get())
		{
			GetConstantBuffer(sizeof(DirectX::XMMATRIX), sp->constantBuffers[sp->cameraLoc].buff, graphicsDevice);
		}
		if (sp->constantBuffers[sp->cameraLoc].buff.Get())
			contextDevice->UpdateSubresource(sp->constantBuffers[sp->cameraLoc].buff.Get(), 0, 0, &currentCameraProj, 0, 0);

	}

	if (sp->viewLoc >= 0 && sp->viewLoc < sp->constantBuffers.Size())
	{
		if (!sp->constantBuffers[sp->viewLoc].buff.Get())
		{
			GetConstantBuffer(sizeof(DirectX::XMMATRIX), sp->constantBuffers[sp->viewLoc].buff, graphicsDevice);
		}
		if (sp->constantBuffers[sp->viewLoc].buff.Get())
			contextDevice->UpdateSubresource(sp->constantBuffers[sp->viewLoc].buff.Get(), 0, 0, &camera, 0, 0);
	}

	for (int c = 0; c < sp->constantBuffers.Size(); c++)
	{
		ConstantBufferMark cbm = sp->constantBuffers[c];
		unsigned char slot = cbm.label >> 4;
		unsigned char shaderType = cbm.label & 0b00000111;
		switch (shaderType)
		{
		case 1: // PixelShader
			UpdateConstantBuffer(contextDevice, cbm.buff, ShaderPhase::sp_Pixel, slot);
			break;
		case 2: // GeometryShader
			UpdateConstantBuffer(contextDevice, cbm.buff, ShaderPhase::sp_Geometry, slot);
			break;
		case 3: // HULL Shader
			UpdateConstantBuffer(contextDevice, cbm.buff, ShaderPhase::sp_Hull, slot);
			break;
		case 4: // Compute Shader
			UpdateConstantBuffer(contextDevice, cbm.buff, ShaderPhase::sp_Compute, slot);
			break;
		case 5: // Domain Shader
			UpdateConstantBuffer(contextDevice, cbm.buff, ShaderPhase::sp_Domain, slot);
			break;
		default: // Vertex Shader
			UpdateConstantBuffer(contextDevice, cbm.buff, ShaderPhase::sp_Vertex, slot);
		}
	}

	doMvp = sp->mvp_cpu;

	RETURN_THREAD_UNLOCK true;

}

/**
 * Method: TShaderHost::InitializeShader
 * Purpose: Adds a blank shader struct to the collection
 * Parameters: void
 * Returns: UINT - the index of the new struct
 */
UINT TShaderHost::InitializeShader()
{
	AG_THREAD_LOCK
	ShaderProgram sp;
	ResetShaderProgram(sp);
	shaders.push_back(sp);
	UINT ret = shaders.Size() - 1;
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TShaderHost::ResetShaderProgram
 * Purpose: sets a Shader Module to it's Nullified state
 * Parameters: ShaderProgram & sp - the shader module to reset
 * Return: void
 */
void TShaderHost::ResetShaderProgram(ShaderProgram& sp)
{
	sp.bufferSize = 0;
	sp.cs.Nullify();
	sp.ds.Nullify();
	sp.elementCount = 0;
	sp.elements = 0;
	sp.gs.Nullify();
	sp.hs.Nullify();
	sp.layout.Nullify();
	sp.layoutError = 0;
	sp.mvp_cpu = false;
	sp.ps.Nullify();
	sp.TextureCount = 0;
	sp.vs.Nullify();
	sp.cameraLoc = -1;
	sp.colorLoc = -1;
	sp.modelLoc = -1;
	sp.viewLoc = -1;
}

/**
 * Method: TShaderHost::getInputDescription
 * Purpose: Converts An AnaGame Input Layout list to a Direct3D Input Layout List
 * Parameters:TDataArray<unsigned short>& bufferDesc - the Input Laout in AnaGame format
 *			int& buffSize - the total size of the input buffer
 * Return: TDataArray<D3D11_INPUT_ELEMENT_DESC>* - the Direct3D Compatible version of the input layout
 */
TDataArray<D3D11_INPUT_ELEMENT_DESC>* TShaderHost::getInputDescription(TDataArray<unsigned short>& bufferDesc, int& buffSize)
{
	AG_THREAD_LOCK
	TDataArray<D3D11_INPUT_ELEMENT_DESC>* returnable = new TDataArray<D3D11_INPUT_ELEMENT_DESC>();

	unsigned char colorCount = 0, positionCount = 0, texcoordCount = 0, normalCount = 0,
		binormalCount = 0, blendweightCount = 0, blendindexCount = 0, tPosCount = 0,
		tangentCount = 0, fogCount = 0, tessCount = 0;
	unsigned char offsetCount = 0;

	for (UINT c = 0; c < bufferDesc.Size(); c++)
	{
		D3D11_INPUT_ELEMENT_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// Width of the data
		unsigned char dataWidth = bufferDesc[c] & 0b0000000011110000;
		dataWidth = dataWidth >> 4;

		// Count of the data
		unsigned short dataCount = bufferDesc[c] & 0b1111000000000000;
		dataCount = dataCount >> 12;

		// First, mark offset count so 0 value is included in first element
		desc.AlignedByteOffset = offsetCount;

		// If these values are 0, assume default of 4
		if (!dataCount)
			dataCount = 4;
		if (!dataWidth)
			dataWidth = 4;

		assignDescriptionFormat(desc, dataWidth, dataCount);
		offsetCount += dataWidth * dataCount;

		switch (bufferDesc[c] & 0b00001111)
		{
		case 0: // Color
			desc.SemanticName = _sem_C;
			desc.SemanticIndex = colorCount++;

			break;
		case 1: // normal
			desc.SemanticName = _sem_N;
			desc.SemanticIndex = normalCount++;
			break;
		case 2: // TextCoord
			desc.SemanticName = _sem_T;
			desc.SemanticIndex = texcoordCount++;
			break;
		case 3: // position
			desc.SemanticName = _sem_V;
			desc.SemanticIndex = positionCount++;
			break;
		case 4: // binormal
			desc.SemanticName = _sem_BN;
			desc.SemanticIndex = binormalCount++;
			break;
		case 5: // blendweight
			desc.SemanticName = _sem_BW;
			desc.SemanticIndex = blendweightCount++;
			break;
		case 6: // blendindex
			desc.SemanticName = _sem_BI;
			desc.SemanticIndex = blendindexCount++;
			break;
		case 7: // position T
			desc.SemanticName = _sem_PT;
			desc.SemanticIndex = tPosCount++;
			break;
		case 8: // Tangent
			desc.SemanticName = _sem_T;
			desc.SemanticIndex = tangentCount++;
			break;
		case 9: // Fog
			desc.SemanticName = _sem_F;
			desc.SemanticIndex = fogCount++;
			break;
		case 10: // Tessfactor
			desc.SemanticName = _sem_TS;
			desc.SemanticIndex = tessCount++;
			break;
		}

		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InputSlot = (bufferDesc[c] & 0b0000111100000000) >> 8;

		returnable->push_back(desc);
	}
	buffSize = offsetCount;
	RETURN_THREAD_UNLOCK returnable;
}

/**
 * Method: TShaderHost::assignDescriptionFormat
 * Purpose: Aids in the Development of a D3D Input Description b deducing what type of input is being added
 * Parameters: D3D11_INPUT_ELEMENT_DESC & desc - The description to update
 *				unsigned char valueSize - the size of the data types
 *				unsigned short valueCount - the count of data per entry
 * Return: void
 */
void TShaderHost::assignDescriptionFormat(D3D11_INPUT_ELEMENT_DESC& desc, unsigned char valueSize, unsigned short valueCount)
{
	AG_THREAD_LOCK
	switch (valueSize)
	{
	case 1: // 8 bits
		switch (valueCount)
		{
		case 1:
			desc.Format = DXGI_FORMAT_R8_TYPELESS;
			break;
		case 2:
			desc.Format = DXGI_FORMAT_R8G8_TYPELESS;
			break;
		case 3:
			desc.Format = DXGI_FORMAT_UNKNOWN;      // Not Supported by Direct 3d
			break;
		default:
			desc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
			break;
		}
		RETURN_THREAD_UNLOCK;
	case 2: // 16 bits
		switch (valueCount)
		{
		case 1:
			desc.Format = DXGI_FORMAT_R16_FLOAT;
			break;
		case 2:
			desc.Format = DXGI_FORMAT_R16G16_FLOAT;
			break;
		case 3:
			desc.Format = DXGI_FORMAT_UNKNOWN;
			break;
		default:
			desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;
		}
		RETURN_THREAD_UNLOCK;
	default: // 32 bits
		switch (valueCount)
		{
		case 1:
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case 2:
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case 3:
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		default:
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		RETURN_THREAD_UNLOCK;
	}
}

/**
 * Method: TShaderHost::GetConstantBuffer
 * Purpose: Creates a Constant Buffer
 * Parameters: int size - the size being requested
 *				TrecComPointer<ID3D11Buffer>& buff - reference to hold the buffer
 * Return: int - 0 if successful, error code otherwise
 */
int TShaderHost::GetConstantBuffer(int size, TrecComPointer<ID3D11Buffer>& buff, TrecComPointer<ID3D11Device> graphicsDevice)
{
	if (!size)
		return 1;

	AG_THREAD_LOCK
	D3D11_BUFFER_DESC conBuf;

	conBuf.CPUAccessFlags = 0;
	conBuf.MiscFlags = 0;
	conBuf.StructureByteStride = 0;
	conBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	conBuf.ByteWidth = size;
	conBuf.Usage = D3D11_USAGE_DEFAULT;
	buff.Nullify();
	TrecComPointer<ID3D11Buffer>::TrecComHolder b;
	HRESULT res = graphicsDevice->CreateBuffer(&conBuf, 0, b.GetPointerAddress());
	buff = b.Extract();
	RETURN_THREAD_UNLOCK !SUCCEEDED(res);
}
/**
 * Method: TShaderHost::SetNewConstantBuffer
 * Purpose: Establishes a Constant buffer in a shader
 * Parameters: TrecComPointer<ID3D11Buffer>& buff - the buffer to apply to
 *				int slot - the slot of the GPU (typically 0)
 *				bool isModelBuffer - is the buffer for the model
 *				ShaderPhase sp - The Specific Shader phase the buffer applies to
 *				int shaderID - The index of the shader to target
 *				unsigned char purp - the purpose of the buffer
 * Returns: bool - whether he update has been made
 */
bool TShaderHost::SetNewConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, int slot, bool isModelBuffer, ShaderPhase sp, int shaderID, unsigned char purp)
{
	AG_THREAD_LOCK
	if (!buff.Get() || shaderID < 0 || shaderID >= shaders.Size() || slot < 0 || slot > 15)
	{
		RETURN_THREAD_UNLOCK false;
	}
	ShaderProgram* spr = &(shaders[shaderID]);

	// Cannot have the same slot
	for (UINT c = 0; c < spr->constantBuffers.Size(); c++)
	{
		if (spr->constantBuffers[c].label >> 4 == slot)
			RETURN_THREAD_UNLOCK false;
	}

	ConstantBufferMark cbm;
	cbm.buff.Nullify();
	cbm.label = 0;

	switch (sp)
	{
	case ShaderPhase::sp_Vertex:
		cbm.label = 0;
		break;
	case ShaderPhase::sp_Pixel:
		cbm.label = 1;
		break;
	case ShaderPhase::sp_Geometry:
		cbm.label = 2;
		break;
	case ShaderPhase::sp_Hull:
		cbm.label = 3;
		break;
	case ShaderPhase::sp_Compute:
		cbm.label = 4;
		break;
	case ShaderPhase::sp_Domain:
		cbm.label = 5;
		break;
	}

	if (isModelBuffer)
		cbm.label = cbm.label | 0b00001000;

	cbm.label += slot << 4;
	cbm.buff = buff;
	spr->constantBuffers.push_back(cbm);


	switch (purp)
	{
	case 1: // Color
		spr->colorLoc = spr->constantBuffers.Size() - 1;
		break;
	case 2:// Camera
		spr->cameraLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // If Camera was in the shader memory, it would have to calculate
		break;
	case 3:
		spr->modelLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = true; // if this was the MVP, then the shader expects the CPU to calculate it
		break;
	case 4:
		spr->viewLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // If view was in the shader memory, it would have to calculate
		break;
	case 5:
		spr->modelLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // In this case, the model just has to update its own matrix, the shader will calculate
	}


	RETURN_THREAD_UNLOCK true;
}

/**
 * Method: TShaderHost::AddComputeShader
 * Purpose: Adds a Compute Shader to the shader struct
 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
 *				int shaderID - the ID of the shader struct
 *				TString& cs - the file to open
 *				const CHAR* cf - the name of the function
 * Returns: int - error code
 */
int TShaderHost::AddComputeShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& cs, const CHAR* cf)
{
	if (!graphicsDevice.Get())
		return 5;

	if (!cf)
		return 1;

	AG_THREAD_LOCK
		if (shaderID < 0 || shaderID >= shaders.Size())
		{
			RETURN_THREAD_UNLOCK 2;
		}
	ID3DBlob* buffer = nullptr, * errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	WCHAR* csBuff = cs.GetBufferCopy();
	HRESULT res = D3DCompileFromFile(csBuff, 0, 0, cf, "vs_4_0", flags, 0, &buffer, &errorBuffer);
	delete[] csBuff;
	if (!SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 3;
	}
	TrecComPointer<ID3D11ComputeShader>::TrecComHolder css;
	res = graphicsDevice->CreateComputeShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, css.GetPointerAddress());
	shaders[shaderID].cs = css.Extract();
	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 0;
	}
	else
	{
		RETURN_THREAD_UNLOCK 4;
	}
}
/**
 * Method: TShaderHost::AddDomainShader
 * Purpose: Adds a Domain Shader to the shader struct
 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
 *				int shaderID - the ID of the shader struct
 *				TString& ds - the file to open
 *				const CHAR* df - the name of the function
 * Returns: int - error code
 */
int TShaderHost::AddDomainShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& ds, const CHAR* df)
{
	if (!graphicsDevice.Get())
		return 5;

	if (!df)
		return 1;
	AG_THREAD_LOCK
		if (shaderID < 0 || shaderID >= shaders.Size())
		{
			RETURN_THREAD_UNLOCK 2;
		}
	ID3DBlob* buffer = nullptr, * errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	WCHAR* dsBuff = ds.GetBufferCopy();
	HRESULT res = D3DCompileFromFile(dsBuff, 0, 0, df, "vs_4_0", flags, 0, &buffer, &errorBuffer);
	delete[] dsBuff;
	if (!SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 3;
	}
	TrecComPointer<ID3D11DomainShader>::TrecComHolder d;
	res = graphicsDevice->CreateDomainShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, d.GetPointerAddress());
	shaders[shaderID].ds = d.Extract();
	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 0;
	}
	RETURN_THREAD_UNLOCK 4;
}
/**
 * Method: TShaderHost::AddGeometryShader
 * Purpose: Adds a Geometry Shader to the shader struct
 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
 *				int shaderID - the ID of the shader struct
 *				TString& gs - the file to open
 *				const CHAR* gf - the name of the function
 * Returns: int - error code
 */
int TShaderHost::AddGeometryShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& gs, const CHAR* gf)
{
	if (!graphicsDevice.Get())
		return 5;

	if (!gf)
		return 1;
	AG_THREAD_LOCK
		if (shaderID < 0 || shaderID >= shaders.Size())
		{
			RETURN_THREAD_UNLOCK 2;
		}
	ID3DBlob* buffer = nullptr, * errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	WCHAR* gsBuff = gs.GetBufferCopy();
	HRESULT res = D3DCompileFromFile(gsBuff, 0, 0, gf, "vs_4_0", flags, 0, &buffer, &errorBuffer);
	delete[] gsBuff;
	if (!SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 3;
	}
	TrecComPointer<ID3D11GeometryShader>::TrecComHolder g;
	res = graphicsDevice->CreateGeometryShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, g.GetPointerAddress());
	shaders[shaderID].gs = g.Extract();
	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 0;
	}
	RETURN_THREAD_UNLOCK 4;
}
/**
 * Method: TShaderHost::AddHullShader
 * Purpose: Adds a Hull Shader to the shader struct
 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
 *				int shaderID - the ID of the shader struct
 *				TString& hs - the file to open
 *				const CHAR* hf - the name of the function
 * Returns: int - error code
 */
int TShaderHost::AddHullShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& hs, const CHAR* hf)
{
	if (!graphicsDevice.Get())
		return 5;

	if (!hf)
		return 1;

	AG_THREAD_LOCK
	if (shaderID < 0 || shaderID >= shaders.Size())
		RETURN_THREAD_UNLOCK 2;

	ID3DBlob* buffer = nullptr, * errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	WCHAR* hsBuff = hs.GetBufferCopy();
	HRESULT res = D3DCompileFromFile(hsBuff, 0, 0, hf, "vs_4_0", flags, 0, &buffer, &errorBuffer);
	delete[] hsBuff;
	if (!SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 3;
	}
	TrecComPointer<ID3D11HullShader>::TrecComHolder h;
	res = graphicsDevice->CreateHullShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, h.GetPointerAddress());
	shaders[shaderID].hs = h.Extract();

	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
	{
		RETURN_THREAD_UNLOCK 0;
	}
	RETURN_THREAD_UNLOCK 4;
}

/**
 * Method: TShaderHost::GetTextureCount
 * Purpose: Retrieves the number of textures the shader expects
 * Parameters: int shaderID - the external shader to look at
 * Return: signed char - the number of texture buffers
 */
signed char TShaderHost::GetTextureCount(int shaderID)
{
	AG_THREAD_LOCK
	if (shaderID < 0 || shaderID >= shaders.Size())
		RETURN_THREAD_UNLOCK -1;
	ShaderProgram sp = shaders[shaderID];
	RETURN_THREAD_UNLOCK sp.TextureCount;
}

/**
 * Method: TShaderHost::SetTextureCount
 * Purpose: Sets the Texture coutn of the shader
 * Parameters: int shaderID the shader to update
 *				unsigned char c - the number of textures expected by the shader
 * Returns: void
 */
void TShaderHost::SetTextureCount(int shaderID, unsigned char c)
{
	AG_THREAD_LOCK
	if (shaderID < 0 || shaderID >= shaders.Size())
		RETURN_THREAD_UNLOCK;
	ShaderProgram sp = shaders[shaderID];
	sp.TextureCount = c;
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TShaderHost::UpdateConstantBuffer
 * Purpose: Updates the Constant Buffer for a shader
 * Parameters: TrecComPointer<ID3D11DeviceContext> contextDevice - object that does the actual update
 *				TrecComPointer<ID3D11Buffer>& buff - the buffer to update with
 *				ShaderPhase sp - the shade type for this operation
 *				int slot - the slot (typically 0)
 * Returns: void
 */
void TShaderHost::UpdateConstantBuffer(TrecComPointer<ID3D11DeviceContext> contextDevice, TrecComPointer<ID3D11Buffer>& buff, ShaderPhase sp, int slot)
{
	if (!buff.Get() || !contextDevice.Get())
		return;

	TrecComPointer<ID3D11Buffer>::TrecComHolder buffHolder;
	buffHolder.OfferValue(buff);

	AG_THREAD_LOCK
	switch (sp)
	{
	case ShaderPhase::sp_Compute:
		contextDevice->CSSetConstantBuffers(slot, 1, buffHolder.GetPointerAddress());
		break;
	case ShaderPhase::sp_Domain:
		contextDevice->DSSetConstantBuffers(slot, 1, buffHolder.GetPointerAddress());
		break;
	case ShaderPhase::sp_Geometry:
		contextDevice->GSSetConstantBuffers(slot, 1, buffHolder.GetPointerAddress());
		break;
	case ShaderPhase::sp_Hull:
		contextDevice->HSSetConstantBuffers(slot, 1, buffHolder.GetPointerAddress());
		break;
	case ShaderPhase::sp_Pixel:
		contextDevice->PSSetConstantBuffers(slot, 1, buffHolder.GetPointerAddress());
		break;
	case ShaderPhase::sp_Vertex:
		contextDevice->VSSetConstantBuffers(slot, 1, buffHolder.GetPointerAddress());
		break;
	}

	buffHolder.Extract(buff);
	RETURN_THREAD_UNLOCK;
}
/**
 * Method: TShaderHost::getColorBufferLocation_
 * Purpose: Retrieves the index of the color buffer of the specified shader
 * Parameters: int shaderID - the index of the chader to target
 * Returns: signed char - the location of the buffer (-1 if not in the shader, -2 if index is invalid)
 */
signed char TShaderHost::getColorBufferLocation_(int shaderID)
{
	AG_THREAD_LOCK
	if (shaderID < 0 || shaderID >= shaders.Size())
		RETURN_THREAD_UNLOCK -2;
	auto ret = shaders[shaderID].colorLoc;
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TShaderHost::ReplaceConstantBuffer
 * Purpose: Replaces a Constant Buffer with new information in an external shader
 * Parameters: int shaderID - the external shader to update
 *				unsigned char slot - the constant buffer to update
 *				TrecComPointer<ID3D11Buffer> - the new buffer to update the Constant buffer with
 * Return: int - 0 if successful, error otherwise
 */
int TShaderHost::ReplaceConstantBuffer_(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff)
{
	AG_THREAD_LOCK
	if (shaderID < 0 || shaderID >= shaders.Size())
		RETURN_THREAD_UNLOCK 1;

	if (slot > shaders[shaderID].constantBuffers.Size())
		RETURN_THREAD_UNLOCK 2;

	if (!buff.Get())
		RETURN_THREAD_UNLOCK 3;



	shaders[shaderID].constantBuffers[slot].buff = buff;

	RETURN_THREAD_UNLOCK 0;
}

ShaderProgram::ShaderProgram()
{
	bufferSize = 0;
	cs.Nullify();
	ds.Nullify();
	elementCount = 0;
	elements = 0;
	gs.Nullify();
	hs.Nullify();
	layout.Nullify();
	layoutError = 0;
	mvp_cpu = false;
	ps.Nullify();
	TextureCount = 0;
	vs.Nullify();
	cameraLoc = -1;
	colorLoc = -1;
	modelLoc = -1;
	viewLoc = -1;
}

ConstantBufferMark::ConstantBufferMark()
{
	label = 0;
}

DefaultShaderProgram::DefaultShaderProgram()
{
	type = DefaultShader::default_shader_uninitialized;
}
