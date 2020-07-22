#pragma once
#include <TObject.h>
#include <TString.h>
#include <TDataArray.h>
#include <d3d11.h>
#include <TrecReference.h>
#include "VideoGraphics.h"
#include <DirectXMath.h>

/*
* enum ShaderPhase
* Used to keep track of what shader type is being discussed
*/
typedef enum class ShaderPhase
{
	sp_Vertex,
	sp_Compute,
	sp_Hull,
	sp_Domain,
	sp_Geometry,
	sp_Pixel
}ShaderPhase;

/*
* enum DefaultShader
* Mark for a built-in AnaGame Shader
*/
typedef enum class _VIDEO_GRAPHICS DefaultShader
{
	default_shader_Single_Color = 0,
	default_shader_3D_Color = 1,
	default_Shader_4D_Color = 2,
	default_Shader_Texture = 3,
	default_Shader_Texture2 = 4,
}DefaultShader;

/*
* struct ConstantBufferMark
* Holds a shader's constant buffer as well as a label signaling what that buffer is
*/
typedef struct ConstantBufferMark
{
	unsigned char label;
	TrecComPointer<ID3D11Buffer> buff;
}ConstantBufferMark;

/*
* struct ShaderProgram
* Holds all of the resources that constitute a shader as well as indexes to imprtant
*	resources within the shader
*/
typedef struct ShaderProgram
{
	TrecComPointer<ID3D11VertexShader> vs;
	TrecComPointer<ID3D11ComputeShader> cs;
	TrecComPointer<ID3D11DomainShader> ds;
	TrecComPointer<ID3D11GeometryShader> gs;
	TrecComPointer<ID3D11HullShader> hs;
	TrecComPointer<ID3D11PixelShader> ps;
	D3D11_INPUT_ELEMENT_DESC* elements;
	unsigned char elementCount;
	int bufferSize;
	TrecComPointer<ID3D11InputLayout> layout;
	HRESULT layoutError;
	TDataArray<ConstantBufferMark> constantBuffers;
	signed char cameraLoc;
	signed char colorLoc;
	signed char modelLoc;
	signed char viewLoc;
	signed char TextureCount;
	bool mvp_cpu;
}ShaderProgram;

/*
* struct DefaultShaderProgram
* Holds a Default shader
*/
typedef struct DefaultShaderProgram
{
	ShaderProgram sp;
	DefaultShader type;
}DefaultShaderProgram;

/*
* struct ShaderLayoutDesc
* Acts as a primitive array for the input element descriptors
*/
typedef struct ShaderLayoutDesc
{
	D3D11_INPUT_ELEMENT_DESC* elements;
	unsigned char elementCount;
}ShaderLayoutDesc;

class _VIDEO_GRAPHICS TShaderHost :	public TObject
{
	friend class TShaderParser;
	friend class TArenaEngine;
	friend class ArenaModel;
public:
	TShaderHost();

	int getBufferSize(int shaderID);

	int SetNewBasicShader(TrecComPointer<ID3D11Device> graphicsDevice, UINT index, TString& vs, TString& ps, const CHAR* vf, const CHAR* sf, TDataArray<unsigned short>& bufferDesc);
	int SetNewBasicShader(TString& s, const CHAR* f, TDataArray<unsigned short>& bufferDesc, bool isVertex, UINT index, TrecComPointer<ID3D11Device> graphicsDevice);

	signed char GetModelLocation_(int shaderID);
	signed char GetViewLocation_(int shaderID);
	signed char GetCameraLocation_(int shaderID);

	bool assignShader(UINT shaderIndex, DirectX::XMMATRIX& camera, DirectX::XMMATRIX& currentCameraProj, TrecComPointer<ID3D11Device> graphicsDevice, TrecComPointer<ID3D11DeviceContext> contextDevice, bool& doMvp);
protected:
	UINT InitializeShader();
	void ResetShaderProgram(ShaderProgram& sp);
	TDataArray<D3D11_INPUT_ELEMENT_DESC>* getInputDescription(TDataArray<unsigned short>& bufferDesc, int& buffSize);
	void assignDescriptionFormat(D3D11_INPUT_ELEMENT_DESC& desc, unsigned char valueSize, unsigned short valueCount);
	int GetConstantBuffer(int size, TrecComPointer<ID3D11Buffer>& buff, TrecComPointer<ID3D11Device> graphicsDevice);
	bool SetNewConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, int slot, bool isModelBuffer, ShaderPhase sp, int shaderID, unsigned char purp);
	
	int AddComputeShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& cs, const CHAR* cf);
	int AddDomainShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& ds, const CHAR* df);
	int AddGeometryShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& gs, const CHAR* gf);
	int AddHullShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& hs, const CHAR* hf);

	signed char GetTextureCount(int shaderID);
	void SetTextureCount(int shaderID, unsigned char c);

	void UpdateConstantBuffer(TrecComPointer<ID3D11DeviceContext> contextDevice,TrecComPointer<ID3D11Buffer>& buff, ShaderPhase sp, int slot);
	signed char getColorBufferLocation_(int shaderID);

	int ReplaceConstantBuffer_(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);

	TDataArray<ShaderProgram> shaders;
};

