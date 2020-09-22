#pragma once
#include <Parser_.h>
#include "TShaderHost.h"

/*
* struct ConstantBufferDescription
* Purpose: Provides information regarding a shaders constant buffers
*/
typedef struct ConstantBufferDescription
{
	ShaderPhase sp;			// Shader phase the Constant buffer belongs to (Vertex, Pixel, etc.)
	int bufferSlot;			// the slot in the GPU this buffer is located (1 buffer for each slot)
	bool ModelBuffer;		// Whether this buffer is for Model data
	int size;				// Size in bytes of the constant buffer
	unsigned char purpose;	// What this buffer is used for (uased by AnaGame)
}ConstantBufferDescription;

/*
* struct BasicShaderDetails
* Purpose: Provides the bare miminum resources needed to produce a shader
*/
typedef struct BasicShaderDetails
{
	TrecPointer<TString> vertexFile;
	bool vertexFileSet;
	TrecPointer<TString> pixelFile;
	bool pixelFileSet;
	TrecPointer<TString> vertexFunction;
	bool vertexFunctionSet;
	TrecPointer<TString> pixelFunction;
	bool pixelFunctionSet;
}BasicShaderDetails;

/**
 * Class: TShaderParser
 * Purpose: Parses information about a shader from a TML file
 * 
 * SuperClass: Parser_ - Allows Shaders to be parsed in Anagames parsing architexture
 */
class TShaderParser :
	public Parser_
{
public:
	/**
	 * Method: TShaderParser::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: ShaderParser::TShaderParser
	 * Purpose: Constructor
	 * Parameters: TShaderHost* ae - the TShader Host to add shaders to
	 *				TString directory - the Directory to search for files in
	 *				TrecComPointer<ID3D11Device> dev - the device responsible for generating actual shaders
	 * Return: new Shader Parser object
	 */
	TShaderParser(TShaderHost* ae, TString directory, TrecComPointer<ID3D11Device> dev);

	/**
	 * Method: ShaderParser::~TShaderParser
	 * Purpose: Destructor
	 * Parameters: void
	 * Return: void
	 */
	~TShaderParser();

	// for the initial object type

	/**
	 * Method: ShaderParser::Obj
	 * Purpose: Generates a buffer for the shader
	 * Parameters: TString& v - the Object to create (in this case, the Buffer to add to the shader
	 * Return: bool 
	 * 
	 * Note: In contrast to other parsers, this method is expected to be called after the relevant attributes are set, not before
	 * 
	 * Attributes: override
	 */
	virtual bool Obj(TString& v) override;
	// for the attribute name

	/**
	 * Method: ShaderParser::Attribute
	 * Purpose: Sets an attribute for a shader buffer to use (or a shader file/function) to use
	 * Parameters: TrecPointer<TString> v - vlue of the attribute
	 *				TString& e - attribute key
	 * Return: bool
	 * 
	 * Attributes: override
	 */
	virtual bool Attribute(TrecPointer<TString> v, TString& e) override;
	// for the attribute value (here it may be good that TStrings are used)


	/**
	 * Method: ShaderParser::submitType
	 * Purpose: Submits the type of TML file being read,
	 * Parameters: TSTring v - the TML file Type
	 * Return: bool - false for now
	 * 
	 * Attributes: override
	 */
	virtual bool submitType(TString v) override;
	/**
	 * Method: ShaderParser::submitEdition
	 * Purpose: Submits the edition for this TML file type
	 * Parameters: TString v - the Version of the file
	 * Return: bool - false for now
	 * 
	 * Attributes: override
	 */
	virtual bool submitEdition(TString v)override;

	/**
	 * Method: ShaderParser::goChild
	 * Purpose: Unused by this type
	 * Parameters: void
	 * Return: bool - false, there are no 'children' in this TML type
	 * 
	 * Attributes: override
	 */
	virtual bool goChild()override;
	/**
	 * Method: ShaderParser::goParent
	 * Purpose: Unused by this type
	 * Parameters: void
	 * Return: void
	 * 
	 * Attributes: override
	 */
	virtual void goParent()override;

	virtual UCHAR* GetAnaGameType()override;

	/**
	 * Method: ShaderParser::AddShaderToProgram
	 * Purpose: Adds an additional shader program to the generated shader (used for Hull, domain, compute, and geometry shaders)
	 * Parameters: TString
	 * Return: bool - success or failure
	 */
	bool AddShaderToProgram(TString& str);

	/**
	 * Method: ShaderParser::SetBasicShader
	 * Purpose: Attempts to Initialize the basic shader being parsed by the parser
	 * Parameters: void
	 * Return: bool - whether there was no issue (or there was)
	 */
	bool SetBasicShader();

	/**
	 * Method: ShaderParser::SetBufferPurpose
	 * Purpose: Sets the semantic purpose of a vertex buffer entry
	 * Parameters: TString & t - semantic buffer purpose in string form
	 * Return: bool - whether the string was a valid purpose
	 */
	bool SetBufferPurpose(TString& t);

	/**
	 * Method: ShaderParser::SetInputSlot
	 * Purpose: Determines the input slot of a constant buffer
	 * Parameters: TString & v - number in string form
	 * Return: bool - success, whether the string was a number
	 */
	bool SetInputSlot(TString& v);

	/**
	 * Method: ShaderParser::SetDataWidth
	 * Purpose: Sets the size of a field that a vertex buffer expects
	 * Parameters: TString & v - number in string form
	 * Return: bool - success, whether the string was a number
	 */
	bool SetDataWidth(TString& v);

	/**
	 * Method: ShaderParser::SetDataCount
	 * Purpose: Sets the number of a field that a vertex buffer expects
	 * Parameters: TString & v - number in string form
	 * Return: bool - success, whether the string was a number
	 */
	bool SetDataCount(TString& v);

	/**
	 * Method: ShaderParser::SetTextureCount
	 * Purpose: Sets a texture count for the number of textures the shader expects
	 * Parameters: TString & v - number in string form
	 * Return: bool - success, whether the string was a number
	 */
	bool SetTextureCount(TString& v);

protected:
	/**
	 * The Shader Host to work with
	 */
	TShaderHost* shaderHost;
	/**
	 * THe Directory that the TML file was in
	 */
	TString directory;
	/**
	 * the Index of the shader in the Provided Host
	 */
	UINT shaderIndex;

	/**
	 * Used to create Shaders
	 */
	TrecComPointer<ID3D11Device> device;
	/**
	 * Tracks the Basic Shader details, pixel and vertex shader information
	 */
	BasicShaderDetails bsd;
	/**
	 * Tracks the attributes of a Constant Buffer
	 */
	ConstantBufferDescription cbd;
	/**
	 * Tracks which Shader type is being initialized
	 */
	ShaderPhase phase;

	/**
	 * Holds information about the current Input Buffer component
	 */
	unsigned short desc;
	/**
	 * Holds information about all Input Buffer components
	 */
	TDataArray<unsigned short> bufferDesc;
	/**
	 * The Shader File to open
	 */
	TrecPointer<TString> shaderFile;

	/**
	 * Whether we had a valid id for the shader in the host
	 */
	bool shaderIDd;
};

