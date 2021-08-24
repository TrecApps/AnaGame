#pragma once
#include <TObject.h>
#include <TString.h>
#include <TDataArray.h>
#include <d3d11.h>
#include <TrecReference.h>
#include "VideoGraphics.h"
#include <DirectXMath.h>

/*
* Enum Class: ShaderPhase
* Purpose: Used to keep track of what shader type is being discussed
*/
typedef enum class ShaderPhase
{
	sp_Vertex, // Vertex Shader
	sp_Compute,// Compute Shader
	sp_Hull,   // Hull Shader
	sp_Domain, // Domain Shader
	sp_Geometry,// Geometry Shader
	sp_Pixel   // Pixel Shader
}ShaderPhase;

/*
* Enum Class: DefaultShader
* Purpose: Mark for a built-in AnaGame Shader
*/
typedef enum class _VIDEO_GRAPHICS DefaultShader
{
	default_shader_uninitialized = 0,
	default_shader_Single_Color = 1, // Shader has only one color
	default_shader_3D_Color = 2,     // Shader takes in an RGB float color as part of the input data
	default_Shader_4D_Color = 3,     // Shader takes in an RGBA float color as part of the input data
	default_Shader_Texture = 4,      // Shader uses one texture per mode
	default_Shader_Texture2 = 5,     // Shader uses 2 textures per model
}DefaultShader;

/*
* struct ConstantBufferMark
* Holds a shader's constant buffer as well as a label signaling what that buffer is
*/
class _VIDEO_GRAPHICS ConstantBufferMark
{
public:
	ConstantBufferMark();
	unsigned char label;
	TrecComPointer<ID3D11Buffer> buff;
};

/*
* Class: ShaderProgram
* Holds all of the resources that constitute a shader as well as indexes to imprtant
*	resources within the shader
*/
class _VIDEO_GRAPHICS ShaderProgram
{
public:
	ShaderProgram();
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
};

/*
* struct DefaultShaderProgram
* Holds a Default shader
*/
class _VIDEO_GRAPHICS DefaultShaderProgram
{
public:
	DefaultShaderProgram();
	ShaderProgram sp;
	DefaultShader type;
};

/*
* struct ShaderLayoutDesc
* Acts as a primitive array for the input element descriptors
*/
typedef struct ShaderLayoutDesc
{
	D3D11_INPUT_ELEMENT_DESC* elements;
	unsigned char elementCount;
}ShaderLayoutDesc;

/**
 * Class: TShaderHost
 * Purpose: Hosts shaders
 * 
 * SuperClass: TObject
 */
class _VIDEO_GRAPHICS TShaderHost :	public TObject
{
	friend class TShaderParser;
	friend class TArenaEngine;
	friend class ArenaModel;
public:

	/**
	 * Method: TShaderHost::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TShaderHost::TShaderHost
	 * Purpose: Constructor
	 * Parameters: void
	 * Returns: New TShaderHost object
	 */
	TShaderHost();

	/**
	 * Method: TShaderHost::getBufferSize
	 * Purpose: Retrieves the buffer of a single round of Vertex shader input
	 * Parameters: int shaderID - the index of the shader to use
	 * Return: int -  the buffer size
	 */
	int getBufferSize(int shaderID);

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
	int SetNewBasicShader(TrecComPointer<ID3D11Device> graphicsDevice, UINT index, TString& vs, TString& ps, const CHAR* vf, const CHAR* sf, TDataArray<unsigned short>& bufferDesc);
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
	int SetNewBasicShader(TString& s, const CHAR* f, TDataArray<unsigned short>& bufferDesc, bool isVertex, UINT index, TrecComPointer<ID3D11Device> graphicsDevice);

	/**
	 * Method: TShaderHost::GetModelLocation_
	 * Purpose: Retrieves the index of the Model buffer in a shader
	 * Parameters: int shaderID - the index of the shader
	 * Returns: signed char - location of the requested buffer (-1 if not in shader, -2 if invalid index)
	 */
	signed char GetModelLocation_(int shaderID);
	/**
	 * Method: TShaderHost::GetViewLocation_
	 * Purpose: Retrieves the index of the View buffer in a shader
	 * Parameters: int shaderID - the index of the shader
	 * Returns: signed char - location of the requested buffer (-1 if not in shader, -2 if invalid index)
	 */
	signed char GetViewLocation_(int shaderID);
	/**
	 * Method: TShaderHost::GetCameraLocation_
	 * Purpose: Retrieves the index of the Camera buffer in a shader
	 * Parameters: int shaderID - the index of the shader
	 * Returns: signed char - location of the requested buffer (-1 if not in shader, -2 if invalid index)
	 */
	signed char GetCameraLocation_(int shaderID);

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
	bool assignShader(UINT shaderIndex, DirectX::XMMATRIX& camera, DirectX::XMMATRIX& currentCameraProj, TrecComPointer<ID3D11Device> graphicsDevice, TrecComPointer<ID3D11DeviceContext> contextDevice, bool& doMvp);
protected:
	/**
	 * Method: TShaderHost::InitializeShader
	 * Purpose: Adds a blank shader struct to the collection
	 * Parameters: void
	 * Returns: UINT - the index of the new struct
	 */
	UINT InitializeShader();

	/**
	 * Method: TShaderHost::ResetShaderProgram
	 * Purpose: sets a Shader Module to it's Nullified state
	 * Parameters: ShaderProgram & sp - the shader module to reset
	 * Return: void
	 */
	void ResetShaderProgram(ShaderProgram& sp);

	/**
	 * Method: TShaderHost::getInputDescription
	 * Purpose: Converts An AnaGame Input Layout list to a Direct3D Input Layout List
	 * Parameters:TDataArray<unsigned short>& bufferDesc - the Input Laout in AnaGame format
	 *			int& buffSize - the total size of the input buffer
	 * Return: TDataArray<D3D11_INPUT_ELEMENT_DESC>* - the Direct3D Compatible version of the input layout
	 */
	TDataArray<D3D11_INPUT_ELEMENT_DESC>* getInputDescription(TDataArray<unsigned short>& bufferDesc, int& buffSize);
	/**
	 * Method: TShaderHost::assignDescriptionFormat
	 * Purpose: Aids in the Development of a D3D Input Description b deducing what type of input is being added
	 * Parameters: D3D11_INPUT_ELEMENT_DESC & desc - The description to update
	 *				unsigned char valueSize - the size of the data types
	 *				unsigned short valueCount - the count of data per entry
	 * Return: void
	 */
	void assignDescriptionFormat(D3D11_INPUT_ELEMENT_DESC& desc, unsigned char valueSize, unsigned short valueCount);
	/**
	 * Method: TShaderHost::GetConstantBuffer
	 * Purpose: Creates a Constant Buffer
	 * Parameters: int size - the size being requested
	 *				TrecComPointer<ID3D11Buffer>& buff - reference to hold the buffer
	 * Return: int - 0 if successful, error code otherwise
	 */
	int GetConstantBuffer(int size, TrecComPointer<ID3D11Buffer>& buff, TrecComPointer<ID3D11Device> graphicsDevice);
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
	bool SetNewConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, int slot, bool isModelBuffer, ShaderPhase sp, int shaderID, unsigned char purp);

	/**
	 * Method: TShaderHost::AddComputeShader
	 * Purpose: Adds a Compute Shader to the shader struct
	 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
	 *				int shaderID - the ID of the shader struct
	 *				TString& cs - the file to open
	 *				const CHAR* cf - the name of the function
	 * Returns: int - error code
	 */
	int AddComputeShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& cs, const CHAR* cf);
	/**
	 * Method: TShaderHost::AddDomainShader
	 * Purpose: Adds a Domain Shader to the shader struct
	 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
	 *				int shaderID - the ID of the shader struct
	 *				TString& ds - the file to open
	 *				const CHAR* df - the name of the function
	 * Returns: int - error code
	 */
	int AddDomainShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& ds, const CHAR* df);
	/**
	 * Method: TShaderHost::AddGeometryShader
	 * Purpose: Adds a Geometry Shader to the shader struct
	 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
	 *				int shaderID - the ID of the shader struct
	 *				TString& gs - the file to open
	 *				const CHAR* gf - the name of the function
	 * Returns: int - error code
	 */
	int AddGeometryShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& gs, const CHAR* gf);
	/**
	 * Method: TShaderHost::AddHullShader
	 * Purpose: Adds a Hull Shader to the shader struct
	 * Parameters: TrecComPointer<ID3D11Device> graphicsDevice - the object that compiles the shader code
	 *				int shaderID - the ID of the shader struct
	 *				TString& hs - the file to open
	 *				const CHAR* hf - the name of the function
	 * Returns: int - error code
	 */
	int AddHullShader(TrecComPointer<ID3D11Device> graphicsDevice, int shaderID, TString& hs, const CHAR* hf);

	/**
	 * Method: TShaderHost::GetTextureCount
	 * Purpose: Retrieves the number of textures the shader expects
	 * Parameters: int shaderID - the external shader to look at
	 * Return: signed char - the number of texture buffers
	 */
	signed char GetTextureCount(int shaderID);
	/**
	 * Method: TShaderHost::SetTextureCount
	 * Purpose: Sets the Texture coutn of the shader
	 * Parameters: int shaderID the shader to update
	 *				unsigned char c - the number of textures expected by the shader
	 * Returns: void
	 */
	void SetTextureCount(int shaderID, unsigned char c);

	/**
	 * Method: TShaderHost::UpdateConstantBuffer
	 * Purpose: Updates the Constant Buffer for a shader
	 * Parameters: TrecComPointer<ID3D11DeviceContext> contextDevice - object that does the actual update
	 *				TrecComPointer<ID3D11Buffer>& buff - the buffer to update with
	 *				ShaderPhase sp - the shade type for this operation
	 *				int slot - the slot (typically 0)
	 * Returns: void
	 */
	void UpdateConstantBuffer(TrecComPointer<ID3D11DeviceContext> contextDevice,TrecComPointer<ID3D11Buffer>& buff, ShaderPhase sp, int slot);
	/**
	 * Method: TShaderHost::getColorBufferLocation_
	 * Purpose: Retrieves the index of the color buffer of the specified shader
	 * Parameters: int shaderID - the index of the chader to target
	 * Returns: signed char - the location of the buffer (-1 if not in the shader, -2 if index is invalid)
	 */
	signed char getColorBufferLocation_(int shaderID);

	/**
	 * Method: TShaderHost::ReplaceConstantBuffer
	 * Purpose: Replaces a Constant Buffer with new information in an external shader
	 * Parameters: int shaderID - the external shader to update
	 *				unsigned char slot - the constant buffer to update
	 *				TrecComPointer<ID3D11Buffer> - the new buffer to update the Constant buffer with
	 * Return: int - 0 if successful, error otherwise
	 */
	int ReplaceConstantBuffer_(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);

	/**
	 * Container for actual Shaders
	 */
	TDataArray<ShaderProgram> shaders;
};

