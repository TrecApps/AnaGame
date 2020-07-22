#pragma once
#include "TArenaEngine.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "InlineMatrixOperators.h"
#include <TFile.h>
typedef union ShaderCard
{
	int id;
	DefaultShader dID;
}ShaderCard;

typedef struct ShaderKey
{
	bool _default;
	ShaderCard card;
}ShaderKey;

class _VIDEO_GRAPHICS TextureResources
{
public:
	TextureResources();
	TextureResources(const TextureResources& copy);
	void operator=( TextureResources& copy);
	TrecComPointer<ID3D11ShaderResourceView> colorMap;
	TrecComPointer<ID3D11SamplerState> sampleState;
};


class _VIDEO_GRAPHICS ArenaModel : public TObject
{
public:
	ArenaModel(TrecPointer<TArenaEngine> engine);
	ArenaModel(ArenaModel& am);
	~ArenaModel();

	TString toString() override;
	TString getVariableValueStr(const TString& varName) override;

	void setName(const TString& newName);
	TString getName();

	void SetNewEngine(TrecPointer<TArenaEngine> e);

	bool ValidateConstruction();

	HRESULT LoadModel(TFile& ar);
	int SetVertexData(TDataArray<float>& data, int shaderID, D3D11_PRIMITIVE_TOPOLOGY prim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	int SetVertexData(TDataArray<float>& data, DefaultShader ds, D3D11_PRIMITIVE_TOPOLOGY prim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	int SetVertexData(TDataArray<float>& data);
	bool SetIndices(TDataArray<UINT>& indices);
	void ProjectionGPU(bool gpu);


	void Render(DirectX::XMMATRIX& proj, DirectX::XMMATRIX& cam);  // Use if Shader expects CPU to perform Calculation



	// Transformations
	int Rotate(float radian, DirectX::XMFLOAT3 direction);
	int Translate(float degree, DirectX::XMFLOAT3 direction);
	int Enlarge(float ratio);

	DirectX::XMFLOAT3 GetLocation();
	DirectX::XMFLOAT3 GetDirection();

	bool setColorBuffer(float r, float g, float b, float a);
	bool setPipeColorBuffer(float r, float g, float b, float a);
	bool setSingleColorBuffer(bool solidColor);

	// Texture Methods
	int AddTexture(TString& fileName);
	int UpdateTexture(TString& fileName, int location);

	virtual UCHAR* GetAnaGameType() override;

	void SetSelf(TrecPointer<ArenaModel>);

protected:
	TrecPointerSoft<ArenaModel> self;

private:
	TString name;

	D3D11_PRIMITIVE_TOPOLOGY primitive;

	TextureResources GetTexture(TString& fileName);

	TDataArray<float> vertexData;
	TDataArray<unsigned char> DataDescriptor;
	TDataArray<UINT> index;
	TDataArray<UINT> sqIndex;
	DirectX::XMFLOAT3 location, direction;
	float size;
	TrecPointer<TArenaEngine> engine;
	ShaderKey shader;

	// Color resources
	TDataArray<TextureResources> textures;
	DirectX::XMFLOAT4 singleColor, pipeColor;

	TrecComPointer<ID3D11DeviceContext> context;
	TrecComPointer<IDXGISwapChain> swapChain;
	TrecComPointer<ID3D11RenderTargetView> renderTarget;
	TrecComPointer<ID3D11Device> device;

	// Other information
	TrecComPointer<ID3D11Buffer> indexBuffer;
	TrecComPointer<ID3D11Buffer> vertexBuffer;
	TrecComPointer<ID3D11Buffer> projectionBuffer;

	TrecComPointer<ID3D11Buffer> singleColorBuffer;
	TrecComPointer<ID3D11Buffer> singlePipeColorBuffer;

	TDataArray<unsigned char> constantBuffer;
	bool shaderMatrix;
	int bufferSize;
	bool constructionWorked;
	bool hasPipeColor;
};

