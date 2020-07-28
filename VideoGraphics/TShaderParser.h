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

class TShaderParser :
	public Parser_
{
public:
	TShaderParser(TShaderHost* ae, TString directory, TrecComPointer<ID3D11Device> dev);
	~TShaderParser();

	// for the initial object type
	virtual bool Obj(TString& v) override;
	// for the attribute name
	virtual bool Attribute(TrecPointer<TString> v, TString& e) override;
	// for the attribute value (here it may be good that TStrings are used)

	virtual bool submitType(TString v);
	virtual bool submitEdition(TString v);

	virtual bool goChild();
	virtual void goParent();

	virtual UCHAR* GetAnaGameType()override;

	bool AddShaderToProgram(TString& str);

	bool SetBasicShader();

	bool SetBufferPurpose(TString& t);

	bool SetInputSlot(TString& v);

	bool SetDataWidth(TString& v);

	bool SetDataCount(TString& v);

	bool SetTextureCount(TString& v);

protected:
	TShaderHost* shaderHost;
	TString directory;
	UINT shaderIndex;

	TrecComPointer<ID3D11Device> device;
	BasicShaderDetails bsd;
	ConstantBufferDescription cbd;
	ShaderPhase phase;

	unsigned short desc;
	TDataArray<unsigned short> bufferDesc;
	TrecPointer<TString> shaderFile;

	bool shaderIDd;
};

