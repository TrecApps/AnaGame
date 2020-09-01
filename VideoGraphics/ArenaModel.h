#pragma once
#include "TArenaEngine.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "InlineMatrixOperators.h"
#include <TFile.h>

/**
 * Union: ShaderCard
 * Purpose: Holds information on the shader being used by the model
 */
typedef union ShaderCard
{
	int id;             // The id of the shader, if it is external
	DefaultShader dID;  // the id of the shader, if Anagame provided it
}ShaderCard;

/**
 * Struct: ShaderKey
 * Purpose: Hold information on the shader the model uses
 */
typedef struct ShaderKey
{
	bool _default;   // the type of shader being used
	ShaderCard card; // the id of the shader being used
}ShaderKey;

/**
 * Class: TextureResources
 * Purpose: Supports recourses needed for a texture
 */
class _VIDEO_GRAPHICS TextureResources
{
public:
	/**
	 * Method: TextureResources::TextureResources
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new Texture object
	 */
	TextureResources();

	/**
	 * Method: TextureResources::TextureResources
	 * Purpose: Copy Constructor
	 * Parameters: const TextureResources& copy - the Texture to copy
	 * Returns: new Texture object
	 */
	TextureResources(const TextureResources& copy);

	/**
	 * Method: TextureResources::operator=
	 * Purpose: Assignment operator
	 * Parameters: TextureResources& copy - the textrue to copy
	 * Returns: void
	 */
	void operator=( TextureResources& copy);

	/**
	 * The color map of the texture
	 */
	TrecComPointer<ID3D11ShaderResourceView> colorMap;

	/**
	 * The sample state of the texture
	 */
	TrecComPointer<ID3D11SamplerState> sampleState;
};

/**
 * Class: ArenaModel
 * Purpose: Represents a Model in Anagame's 3D engine
 */
class _VIDEO_GRAPHICS ArenaModel : public TObject
{
public:

	/**
	 * Method: ArenaModel::ArenaModel
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TArenaEngine> engine - the engine this model is tied to
	 * Returns: New Model Object
	 */
	ArenaModel(TrecPointer<TArenaEngine> engine);

	/**
	 * Method: ArenaModel::ArenaModel
	 * Purpose: Copy Constructor
	 * Parameters: const ArenaModel& am - the model to copy
	 * Returns: new Model Object
	 */
	ArenaModel(ArenaModel& am);

	/**
	 * Method: ArenaModel::~ArenaModel
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~ArenaModel();


	/**
	 * Method: ArenaModel::toString
	 * Purpose: Retrieves a String reporesentation of the model
	 * Parameters: void
	 * Returns: TSTring - the TString version of this model
	 */
	TString toString() override;

	/**
	 * Method: ArenaModel::getVariableValueStr
	 * Purpose: Retrieves a variable in string form
	 * Parameters: const TString& varName - the name of the variable to retrieve
	 * Returns: TString the String version of the requested parameter, or empty if invalid
	 */
	TString getVariableValueStr(const TString& varName) override;


	/**
	 * Method: ArenaModel::setName
	 * Purpose: Sets the name of this model
	 * Parameters: const TString& varName - the name of the string
	 * Returns: void
	 */
	void setName(const TString& newName);

	/**
	 * Method: ArenaModel::getName
	 * Purpose: Retrieves the name of the model
	 * Parameters: void
	 * Returns: TString - the name of the model
	 */
	TString getName();


	/**
	 * Method: ArenaModel::SetNewEngine
	 * Purpose: Assigned the model to a new Engine to communicate with
	 * Parameters: TrecPointer<TArenaEngine> e - the new engiine to use
	 * Returns: void
	 */
	void SetNewEngine(TrecPointer<TArenaEngine> e);


	/**
	 * Method: ArenaModel::ValidateConstruction
	 * Purpose: Reports whether the model was properly constructed and ready for use
	 * Parameters: void
	 * Returns: bool 
	 */
	bool ValidateConstruction();


	/**
	 * Method: ArenaModel::LoadModel
	 * Purpose: Retrieves information about model from a file
	 * Parameters: TFile& ar - the file to read
	 * Returns: HRESULT - the result of the method call
	 */
	HRESULT LoadModel(TFile& ar);

	/**
	 * Method: ArenaModel::SetVertexData
	 * Purpose: Sets the list of verticies for the model, using an external shader
	 * Parameters: TDataArray<float>& data - the verticies to hold
	 *				int shaderID - the id of the external shader to use
	 *				D3D11_PRIMITIVE_TOPOLOGY prim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST - the topology of the model
	 * Returns: int - error code (0 means success)
	 */
	int SetVertexData(TDataArray<float>& data, int shaderID, D3D11_PRIMITIVE_TOPOLOGY prim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/**
	 * Method: ArenaModel::SetVertexData
	 * Purpose: Sets the list of verticies for the model, using an external shader
	 * Parameters: TDataArray<float>& data - the verticies to hold
	 *				DefaultShader ds - the id of the anagame shader to use
	 *				D3D11_PRIMITIVE_TOPOLOGY prim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST - the topology of the model
	 * Returns: int - error code (0 means success)
	 */
	int SetVertexData(TDataArray<float>& data, DefaultShader ds, D3D11_PRIMITIVE_TOPOLOGY prim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/**
	 * Method: ArenaModel::SetVertexData
	 * Purpose: Sets the list of verticies for the model
	 * Parameters: TDataArray<float>& data - the verticies to hold
	 * Returns: int - error code (0 means success)
	 */
	int SetVertexData(TDataArray<float>& data);
	/**
	 * Method: ArenaModel::SetIndices
	 * Purpose: Sets the list of indices to use
	 * Parameters: TDataArray<UINT>& indices - the indices to use
	 * Returns: bool - success report
	 */
	bool SetIndices(TDataArray<UINT>& indices);
	/**
	 * Method: ArenaModel::ProjectionGPU
	 * Purpose: sets whether the model uses the gpu or not
	 * Parameters:
	 * Returns:
	 */
	void ProjectionGPU(bool gpu);


	/*
	* Method: ArenaModel::Render
	* Purpose: Draws out the Model to the screen
	* Parameters: DirectX::XMMATRIX & proj - the projection matrix
	*				DirectX::XMMATRIX& camera - the camera matrix to use
	* Return: void
	*/
	void Render(DirectX::XMMATRIX& proj, DirectX::XMMATRIX& cam);  // Use if Shader expects CPU to perform Calculation



	// Transformations

	/**
	 * Method: ArenaModel::Rotate
	 * Purpose: Rotates the Model
	 * Parameters: float radian - the degree to rotate the model
	 *				DirectX::XMFLOAT3 direction - the direction to rotate the model
	 * Return: int - 0 it will work
	 */
	int Rotate(float radian, DirectX::XMFLOAT3 direction);


	/**
	 * Method: ArenaModel::Translate
	 * Purpose: Moves the Model across the coordinate plane
	 * Parameters: float radian - the degree to move the model
	 *				DirectX::XMFLOAT3 direction - the direction to move the model
	 * Return: int - 0 it will work
	 */
	int Translate(float degree, DirectX::XMFLOAT3 direction);
	/*
	 * Method: ArenaModel::Enlarge
	 * Purpose: Adjusts the size of the model
	 * Parameters: float ratio - the degree to change the size of the model
	 * Return: int - 0 it will work
	 */
	int Enlarge(float ratio);

	/**
	 * Method: ArenaModel::GetLocation
	 * Purpose: Retrieves the current location of the model
	 * Parameters: void
	 * Return: DirectX::XMFLOAT3 - the location of the model
	 */
	DirectX::XMFLOAT3 GetLocation();
	/**
	 * Method: ArenaModel::GetDirection
	 * Purpose: Retrieves the current direction of the model
	 * Parameters: void
	 * Return: DirectX::XMFLOAT3 - the direction of the model
	 */
	DirectX::XMFLOAT3 GetDirection();

	/**
	 * Method: ArenaModel::setColorBuffer
	 * Purpose: Sets the Single Solid color for the model
	 * Parameters: float r - the red channel
	 *				float g - the green channel
	 *				float b - the blue channel
	 *				float a - the alpha channel
	 * Return: bool - there is a shader used that would have a single color 
	 */
	bool setColorBuffer(float r, float g, float b, float a);
	/**
	 * Method: ArenaModel::setPipeColorBuffer
	 * Purpose: Sets the Single Wireframe color for the model
	 * Parameters: float r - the red channel
	 *				float g - the green channel
	 *				float b - the blue channel
	 *				float a - the alpha channel
	 * Return: bool - there is a shader used that would have a single color
	 */
	bool setPipeColorBuffer(float r, float g, float b, float a);
	/**
	 * Method: ArenaModel::setSingleColorBuffer
	 * Purpose: Updates the Single Color buffer with Direct3D
	 * Parameters: bool solidColor - Whether it is solid or wireframe 
	 * Return: bool - success
	 */
	bool setSingleColorBuffer(bool solidColor);

	// Texture Methods

	/**
	 * Method: ArenaModel::AddTexture
	 * Purpose: Adds a new texture resource to the Model
	 * Parameters: TString & fileName - File path to the image to use
	 * Return: int - 0 or more if successful, negative value if error
	 */
	int AddTexture(TString& fileName);
	/**
	 * Method: ArenaModel::UpdateTexture
	 * Purpose: Updates an existing texture added to the model
	 * Parameters: TString & fileName - the filepath of the image
	 *				int location -  the texture to update
	 * Return: int - 0 if sccessful, error otherwise 
	 */
	int UpdateTexture(TString& fileName, int location);


	virtual UCHAR* GetAnaGameType() override;

	/**
	 * Method: ArenaModel::SetSelf
	 * Purpose: Allows model to hold a reference to itself
	 * Parameters: TrecPointer<ArenaModel> - the reference to the model
	 * Returns: void
	 */
	void SetSelf(TrecPointer<ArenaModel>);

protected:
	TrecPointerSoft<ArenaModel> self;

private:
	TString name;

	D3D11_PRIMITIVE_TOPOLOGY primitive;

	/**
	 * Method: ArenaModel::GetTexture
	 * Purpose: Retrieves the Texture resources of a given image
	 * Parameters: TString & fileName - the filepath of the image
	 * Return: TextureResources - the resources being generated, check if compenents are null for error
	 */
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

