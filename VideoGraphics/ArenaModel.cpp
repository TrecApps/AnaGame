#include "stdafx.h"
#include "ArenaModel.h"
#include <DirectXTex.h>

/*
* Method: ArenaModel
* Purpose:
* Parameters:
* Return:
*/

#define GUARD_CODE if(!constructionWorked) return

using namespace DirectX;

/*
* Method: (ArenaModel) (Constructor)
* Purpose: Sets up the Model with the resources of an engine
* Parameters: ArenaEngine& ae - the engine to bound the model to
* Return: void
*/
ArenaModel::ArenaModel(ArenaEngine& ae)
{
	engine = &ae;
	size = 1.0f;
	ZeroMemory(&location, sizeof(location));
	ZeroMemory(&direction, sizeof(direction));
	context = engine->getDevice();
	device = engine->getDeviceD();
	renderTarget = engine->getRederTarget();
	swapChain = engine->getSwapChain();
	constructionWorked = context.get() && device.get() && renderTarget.get() && swapChain.get();

	indexBuffer = (ID3D11Buffer*)nullptr;
	singleColorBuffer = (ID3D11Buffer*)nullptr;
	vertexBuffer = (ID3D11Buffer*)nullptr;
	projectionBuffer = (ID3D11Buffer*)nullptr;

	engine->AddModel(*this);

	location.x = 0.0f;
	location.z = 0.0f;
	location.y = 0.0f;

	direction.x = direction.y = direction.z = 1.0f;

	hasPipeColor = false;
}

ArenaModel::ArenaModel()
{
	size = 1.0f;
	ZeroMemory(&location, sizeof(location));
	ZeroMemory(&direction, sizeof(direction));
	context = engine->getDevice();
	device = engine->getDeviceD();
	renderTarget = engine->getRederTarget();
	swapChain = engine->getSwapChain();
	constructionWorked = context.get() && device.get() && renderTarget.get() && swapChain.get();

	indexBuffer = (ID3D11Buffer*)nullptr;
	singleColorBuffer = (ID3D11Buffer*)nullptr;
	vertexBuffer = (ID3D11Buffer*)nullptr;
	projectionBuffer = (ID3D11Buffer*)nullptr;

	//engine->AddModel(*this);

	location.x = 0.0f;
	location.z = 0.0f;
	location.y = 0.0f;

	direction.x = direction.y = direction.z = 1.0f;

	hasPipeColor = false;
}

/*
* Method: (ArenaModel) (Destructor)
* Purpose: Cleans up the model
* Parameters: void
* Return: void
*/
ArenaModel::~ArenaModel()
{
}

TString ArenaModel::toString()
{
	return TString();
}

TString ArenaModel::getVariableValueStr(TString & varName)
{
	if (!varName.Compare(L"Name"))
		return name;
	if (!varName.Compare(L"location"))
	{
		TString ret;
		ret.Format(L"Location: %f, %f, %f", location.x, location.y, location.z);
		return ret;
	}
	if (!varName.Compare(L"direction"))
	{
		TString ret;
		ret.Format(L"Direction: %f, %f, %f", direction.x, direction.y, direction.z);
		return ret;
	}
	return TString();
}

void ArenaModel::setName(TString & newName)
{
	name = newName;
}

TString ArenaModel::getName()
{
	return name;
}

/*
* Method: ArenaModel - SetNewEngine
* Purpose: Attaches the model to a new engine
* Parameters: ArenaEngine& e - the new engine to bound the model to
* Return: void
*/
void ArenaModel::SetNewEngine(ArenaEngine & e)
{
	if (engine)
		engine->RemoveModel(this);

	engine = &e;
	engine->AddModel(*this);
}

/*
* Method: ArenaModel - ValidateConstruction
* Purpose: Reports whether the model is ready for usage
* Parameters: void
* Return: bool - whether the model is ready for use
*/
bool ArenaModel::ValidateConstruction()
{
	return constructionWorked;
}

/*
* Method: ArenaModel - LoadModel
* Purpose:
* Parameters:
* Return:
* Note: NOT IMPLEMENTED - purpose is not clear yet
*/
HRESULT ArenaModel::LoadModel(CArchive & ar)
{
	return E_NOTIMPL;
}

/*
* Method: ArenaModel - SetVertexData
* Purpose: Sets the models Vertex data to send to the shader as well as the external shader to use
* Parameters: TDataArray<float>& data - the Vertex data to give the model
*				int shaderID - the external shader the model should use
* Return: int - 0 if successful, error code otherwise
*/
int ArenaModel::SetVertexData(TDataArray<float>& data, int shaderID, D3D11_PRIMITIVE_TOPOLOGY prim)
{
	//GUARD_CODE -1;
	int bSize = engine->getBufferSize(shaderID);
	if (!bSize)
		return 1;
	bufferSize = bSize;
	if(!SetVertexData(data))
		return 2;
	shader.default = false;
	shader.card.id = shaderID;

	//bufferSize = data.Size();
	primitive = prim;
	return 0;
}

/*
* Method: ArenaModel - SetVertexData
* Purpose: Sets the models Vertex data to send to the shader as well as the AnaGame shader to use
* Parameters: TDataArray<float>& data - the Vertex data to give the model
*				DefaultShader ds - the AnaGame shader the model should use
* Return: int - 0 if successful, error code otherwise
*/
int ArenaModel::SetVertexData(TDataArray<float>& data, DefaultShader ds, D3D11_PRIMITIVE_TOPOLOGY prim)
{
	//GUARD_CODE -1;
	if (!SetVertexData(data))
		return 2;
	shader.default = true;
	shader.card.dID = ds;

	bufferSize = engine->getBufferSize(ds);
	primitive = prim;
	return 0;
}

/*
* Method: ArenaModel - SetVertexData
* Purpose: Sets the models Vertex data to send to the shader, used by the other methods of the same name
* Parameters: TDataArray<float>& data - the Vertex data to give the model
* Return: int - 0 if successful, error code otherwise
*/
int ArenaModel::SetVertexData(TDataArray<float>& data)
{
	//GUARD_CODE -1;

	if (!data.Size())
		return -5;

	vertexData = data;


	vertexBuffer = nullptr;

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = vertexData.data();
	

	D3D11_BUFFER_DESC vDesc;
	ZeroMemory(&vDesc, sizeof(vDesc));
	vDesc.Usage = D3D11_USAGE_DEFAULT;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.ByteWidth = sizeof(vertexData[0]) * vertexData.Size();


	ID3D11Buffer* vb = nullptr;
	HRESULT res = device->CreateBuffer(&vDesc, &rd, &vb);
	vertexBuffer = vb;
	return static_cast<int>(SUCCEEDED(res));
}

/*
* Method: ArenaModel - SetIndices
* Purpose: Sets the model's index data to send to the device context
* Parameters: TDataArray<UINT>& indices - the list of indices to use
* Return: bool - whether the indices are compatible with the current vertex list
*/
bool ArenaModel::SetIndices(TDataArray<UINT>& indices)
{
	//GUARD_CODE false;

	if (!bufferSize)
		return false;

	if (!indices.Size())
		return false;
	UINT max = 0;
	for (UINT c = 0; c < indices.Size(); c++)
	{
		if (indices[c] > max)
			max = indices[c];
	}
	if (max > vertexData.Size() / (bufferSize/sizeof(float)))
		return false;
	D3D11_BUFFER_DESC vDesc;
	ZeroMemory(&vDesc, sizeof(vDesc));
	vDesc.Usage = D3D11_USAGE_DEFAULT;
	vDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vDesc.ByteWidth = sizeof(indices[0]) * indices.Size();

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = indices.data();

	indexBuffer = nullptr;
	ID3D11Buffer* ib = nullptr;
	HRESULT res = device->CreateBuffer(&vDesc, &rd, &ib);
	indexBuffer = ib;

	if(SUCCEEDED(res))
		index = indices;
	else return false;
	return true;
}

/*
* Method: ArenaModel - ProjectionGPU
* Purpose:
* Parameters:
* Return:
*/
void ArenaModel::ProjectionGPU(bool gpu)
{
	shaderMatrix = gpu;
}


/*
* Method: ArenaModel - Render
* Purpose: Draws out the Model to the screen
* Parameters: DirectX::XMMATRIX & proj - the projection matrix
*				DirectX::XMMATRIX& camera - the camera matrix to use
* Return: void
*/
void ArenaModel::Render(DirectX::XMMATRIX & proj, DirectX::XMMATRIX& camera, ArenaEngine* e)
{	//GUARD_CODE;

	if (!e)
		return;

	e->SetResources(context, swapChain, renderTarget, device);
	if (!context.get() || !swapChain.get() ||
		!renderTarget.get() || !device.get())
		return;


	if (!vertexData.Size())
		return;

	if (!projectionBuffer.get())
	{
		D3D11_BUFFER_DESC vDesc;
		ZeroMemory(&vDesc, sizeof(vDesc));
		vDesc.Usage = D3D11_USAGE_DEFAULT;
		vDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		vDesc.ByteWidth = sizeof(XMMATRIX);
		ID3D11Buffer* pb = nullptr;
		if (FAILED(device->CreateBuffer(&vDesc, 0, &pb)))
			return;
		projectionBuffer = pb;
	}
	signed char modelLocation = -1;
	signed char viewLocation = -1;
	if (shader.default)
	{
		engine->setShader(shader.card.dID);
		modelLocation = engine->GetModelLocation(shader.card.dID);
		viewLocation = engine->GetViewLocation(shader.card.dID);
	}
	else
	{
		engine->setShader(shader.card.id);
		modelLocation = engine->GetModelLocation(shader.card.id);
		viewLocation = engine->GetViewLocation(shader.card.id);
	}
	if (modelLocation < 0)
		return;  // That's it! Back to the Chicken Farm

	bool soft_mvp = engine->DoMVP();


	unsigned int offset = 0, buffSize = static_cast<unsigned int>(bufferSize);

	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11Buffer*const  buff = vertexBuffer.get();
	context->IASetVertexBuffers(0, 1, &buff, &buffSize, &offset);
	//context->UpdateSubresource(indexBuffer.get(), 0, 0, index.data(), 0, 0);


	XMMATRIX rotator =  XMMatrixRotationRollPitchYaw(direction.x, direction.y, direction.z);
	XMMATRIX translate = XMMatrixTranslation(location.x, location.y, location.z);
	XMMATRIX sizeMatrix = XMMatrixScaling(size, size, size);
	XMMATRIX modelMat = XMMatrixTranspose(sizeMatrix*rotator * translate);
	
	if (soft_mvp)
	{
		modelMat = modelMat * camera * proj;
	}

	signed char loc = 0;
	if (shader.default)
	{
		loc = engine->getColorBufferLocation(shader.card.dID);
	}
	else
		loc = engine->getColorBufferLocation(shader.card.id);

	if (loc >= 0)
	{
		setSingleColorBuffer(true);
		ID3D11Buffer*const  cBuff = singleColorBuffer.get();
		context->PSSetConstantBuffers(loc,1,&cBuff);
	}

	/*if (viewLocation > -1)
	{
		ID3D11Buffer* viewBuffer = nullptr;
		D3D11_BUFFER_DESC vDesc;
		ZeroMemory(&vDesc, sizeof(vDesc));
		vDesc.Usage = D3D11_USAGE_DEFAULT;
		vDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		vDesc.ByteWidth = sizeof(XMMATRIX);
		
		if (FAILED(device->CreateBuffer(&vDesc, 0, &viewBuffer)))
			return;
		context->UpdateSubresource(viewBuffer, 0, 0, &world, 0, 0);
		context->VSSetConstantBuffers(viewLocation, 1, &viewBuffer);
	}*/

	context->UpdateSubresource(projectionBuffer.get(), 0, 0, &modelMat, 0, 0);
	ID3D11Buffer*const  pBuff = projectionBuffer.get();
	context->VSSetConstantBuffers(modelLocation, 1, &pBuff);
	//engine->ModelSetFillMode(D3D11_FILL_SOLID);

	if(indexBuffer.get())
		context->IASetIndexBuffer(indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(primitive);
	
	switch (primitive)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
		engine->ModelSetFillMode(D3D11_FILL_WIREFRAME);
		context->Draw(vertexData.Size() / (buffSize/sizeof(float)), 0);
		break;
	default:
		engine->ModelSetFillMode(D3D11_FILL_SOLID);
		
		context->DrawIndexed(index.Size(), 0, 0);
		if (hasPipeColor && setSingleColorBuffer(false) && loc >= 0)
		{
			ID3D11Buffer*const  cBuff = singlePipeColorBuffer.get();
			context->PSSetConstantBuffers(loc, 1, &cBuff);
			engine->ModelSetFillMode(D3D11_FILL_WIREFRAME);
			context->DrawIndexed(index.Size(), 0, 0);
		}

	}

}



/*
* Method: ArenaModel - Rotate
* Purpose: Rotates the Model
* Parameters: float radian - the degree to rotate the model
*				DirectX::XMFLOAT3 direction - the direction to rotate the model
* Return: int - 0 it will work
*/
int ArenaModel::Rotate(float radian, DirectX::XMFLOAT3 direction_)
{
	direction_.x *= abs(radian);
	direction_.y *= abs(radian);
	direction_.z *= abs(radian);


	if (radian > 0.0f)
	{
		this->direction.x += direction_.x;
		this->direction.y += direction_.y;
		this->direction.z += direction_.z;
	}
	else
	{
		this->direction.x -= direction_.x;
		this->direction.y -= direction_.y;
		this->direction.z -= direction_.z;
	}
	return 0;
}

/*
* Method: ArenaModel - Translate
* Purpose: Moves the Model across the coordinate plane
* Parameters: float radian - the degree to move the model
*				DirectX::XMFLOAT3 direction - the direction to move the model
* Return: int - 0 it will work
*/
int ArenaModel::Translate(float degree, DirectX::XMFLOAT3 direction)
{
	direction.x *= abs(degree);
	direction.y *= abs(degree);
	direction.z *= abs(degree);

	if (degree > 0.0f)
	{
		this->location.x += direction.x;
		this->location.y += direction.y;
		this->location.z += direction.z;
	}
	else
	{
		this->location.x -= direction.x;
		this->location.y -= direction.y;
		this->location.z -= direction.z;
	}
	return 0;
}

/*
* Method: ArenaModel - Enlarge
* Purpose: Adjusts the size of the model
* Parameters: float ratio - the degree to change the size of the model
* Return: int - 0 it will work 
*/
int ArenaModel::Enlarge(float ratio)
{
	size +=ratio;

	return 0;
}

/*
* Method: ArenaModel - GetLocation
* Purpose: Retrieves the current location of the model
* Parameters: void
* Return: DirectX::XMFLOAT3 - the location of the model
*/
DirectX::XMFLOAT3 ArenaModel::GetLocation()
{
	return location;
}

/*
* Method: ArenaModel - GetDirection
* Purpose: Retrieves the current direction of the model
* Parameters: void
* Return: DirectX::XMFLOAT3 - the direction of the model
*/
DirectX::XMFLOAT3 ArenaModel::GetDirection()
{
	return direction;
}

/*
* Method: ArenaModel - setColorBuffer
* Purpose: Sets the Single Solid color for the model
* Parameters: float r - the red channel
*				float g - the green channel
*				float b - the blue channel
*				float a - the alpha channel
* Return: bool - there is a shader used that would have a single color 
*/
bool ArenaModel::setColorBuffer(float r, float g, float b, float a)
{
	signed char loc = 0;
	if (shader.default)
	{
		loc = engine->getColorBufferLocation(shader.card.dID);
	}
	else
		loc = engine->getColorBufferLocation(shader.card.id);
	if (loc < 0)
		return false;


	int result = engine->GetConstantBuffer(16, singleColorBuffer);

	if(result) // failed
	{
		singleColorBuffer = nullptr;
		return false;
	}

	if (!context.get())
		return false;

	float color[4];
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	context->UpdateSubresource(singleColorBuffer.get(), 0, 0, &color, 0, 0);

	

	//singleColorBuffer->GetPrivateData(typeid(float), );
	//engine->UpdateConstantBuffer(&singleColorBuffer, sp_Pixel, loc);


	return true;
}

/*
* Method: ArenaModel - setPipeColorBuffer
* Purpose: Sets the Single Wireframe color for the model
* Parameters: float r - the red channel
*				float g - the green channel
*				float b - the blue channel
*				float a - the alpha channel
* Return: bool - there is a shader used that would have a single color
*/
bool ArenaModel::setPipeColorBuffer(float r, float g, float b, float a)
{
	pipeColor.x = r;
	pipeColor.y = g;
	pipeColor.z = b;
	pipeColor.w = a;
	hasPipeColor = true;

	signed char loc = 0;
	if (shader.default)
	{
		loc = engine->getColorBufferLocation(shader.card.dID);
	}
	else
		loc = engine->getColorBufferLocation(shader.card.id);
	if (loc < 0)
		return false;


	int result = engine->GetConstantBuffer(16, singlePipeColorBuffer);

	if (result) // failed
	{
		singlePipeColorBuffer = nullptr;
		return false;
	}

	if (!context.get())
		return false;

	float color[4];
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	context->UpdateSubresource(singlePipeColorBuffer.get(), 0, 0, &color, 0, 0);





	return true;
}

/*
* Method: ArenaModel - setSingleColorBuffer
* Purpose: Updates the Single Color buffer with Direct3D
* Parameters: bool solidColor - Whether it is solid or wireframe 
* Return: bool - success
*/
bool ArenaModel::setSingleColorBuffer(bool solidColor)
{
	signed char loc;
	if (shader.default)
	{
		loc = engine->getColorBufferLocation(shader.card.dID);
	}
	else
		loc = engine->getColorBufferLocation(shader.card.id);
	if (loc < 0)
		return false;
	if (solidColor)
	{
		if (!singleColorBuffer.get())
			return false;
		engine->ReplaceConstantBuffer(default_shader_Single_Color, loc, singleColorBuffer.get());
	}
	else
	{
		if (!singlePipeColorBuffer.get())
			return false;
		engine->ReplaceConstantBuffer(default_shader_Single_Color, loc, singlePipeColorBuffer.get());
	}
	return true;
}

/*
* Method: ArenaModel- AddTexture
* Purpose: Adds a new texture resource to the Model
* Parameters: TString & fileName - File path to the image to use
* Return: int - 0 or more if successful, negative value if error
*/
int ArenaModel::AddTexture(TString & fileName)
{
	signed char id = -2;
	if (shader.default)
	{
		id = engine->GetTextureCount(shader.card.dID);
	}
	else
	{
		id = engine->GetTextureCount(shader.card.id);
	}

	if (id < 1)
		return -1;

	if (id <= textures.Size())
		return -2;

	TextureResources tr = GetTexture(fileName);
	if (!tr.colorMap.get())
		return -3;
	if (!tr.sampleState.get())
	{

		tr.colorMap = nullptr;
		return -4;
	}

	textures.push_back(tr);

	return textures.Size() - 1;
}

/*
* Method: ArenaModel - UpdateTexture
* Purpose: Updates an existing texture added to the model
* Parameters: TString & fileName - the filepath of the image
*				int location -  the texture to update
* Return: int - 0 if sccessful, error otherwise 
*/
int ArenaModel::UpdateTexture(TString & fileName, int location)
{
	if (location < 0)
		return 1;
	if (location >= textures.Size())
		return 2;

	TextureResources tr = GetTexture(fileName);
	if (!tr.colorMap.get())
		return -3;
	if (!tr.sampleState.get())
	{
		tr.colorMap = nullptr;
		return -4;
	}


		textures[location].colorMap= nullptr;

		textures[location].sampleState= nullptr;
	textures[location] = tr;

	return 0;
}


UCHAR ArenaModelType[] = { 2, 0b10000000, 6 };

/*
* Method: ArenaModel - GetAnaGameType
* Purpose: Retrieves the AnaGame type identifier for this class
* Parameters: void
* Return: UCHAR* - the AnaGame type identifier for the Arena Model class
*/
UCHAR * ArenaModel::GetAnaGameType()
{
	return ArenaModelType;
}

/*
* Method: ArenaModel - GetTexture
* Purpose: Retrieves the Texture resources of a given image
* Parameters: TString & fileName - the filepath of the image
* Return: TextureResources - the resources being generated, check if compenents are null for error
*/
TextureResources ArenaModel::GetTexture(TString & fileName)
{
	DirectX::TexMetadata imageData;
	DirectX::ScratchImage* scratchy = new DirectX::ScratchImage();

	HRESULT res = LoadFromDDSFile(fileName.GetBuffer(), DirectX::DDS_FLAGS_NONE, &imageData, *scratchy);

	fileName.ReleaseBuffer();

	TextureResources ret;
	ret.colorMap = nullptr;
	ret.sampleState = nullptr;

	if (FAILED(res))
	{
		if (scratchy)
			delete scratchy;
		scratchy = nullptr;
		return ret;
	}

	ID3D11ShaderResourceView* rv = nullptr;
	ID3D11SamplerState* ss = nullptr;
	res = CreateShaderResourceView(device.get(), scratchy->GetImages(), scratchy->GetImageCount(), imageData, &rv);
	if (scratchy)
		delete scratchy;
	scratchy = nullptr;

	D3D11_SAMPLER_DESC colorMapDes;
	ZeroMemory(&colorMapDes, sizeof(D3D11_SAMPLER_DESC));

	colorMapDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDes.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDes.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDes.MaxLOD = D3D11_FLOAT32_MAX;

	res = device->CreateSamplerState(&colorMapDes, &ss);
	ret.colorMap = rv;
	ret.sampleState = ss;

	return ret;
}
