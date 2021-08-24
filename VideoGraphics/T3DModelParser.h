#pragma once
#include <TObject.h>
#include "ArenaModel.h"
#include <TDirectory.h>

class TVertex
{
public:
    TVertex();
    TVertex(const TVertex& orig);

    float x, y, z;
};

class T4Vertex : public TVertex
{
public:
    T4Vertex();
    T4Vertex(const T4Vertex& orig);

    float w;
};

class T2Float
{
public:
    T2Float();
    T2Float(const T2Float& orig);

    float x, y;
};



class T3DModelParser :
    public TObject
{
    friend class ModelCompiler;
public:
    T3DModelParser();

    virtual TrecPointer<ArenaModel> Generate(const ShaderProgram& program, TString& err) = 0;
    virtual void Generate(TrecPointer<ArenaModel>& model, TString& err) = 0;
    virtual TString Save(TrecPointer<ArenaModel> model) = 0;

protected:
    TDataArray<T4Vertex> vertices4;
    TDataArray<TVertex> vertices;
    TDataArray<TVertex> normal;
    TDataArray<T2Float> textures;
    TDataArray<UINT> indices;

    UINT iVertex3, iVertex4, iNormal, iTexture;

    TrecPointer<TArenaEngine> engine;


    TrecPointer<ArenaModel> Compile(const ShaderProgram& program, TString& err);
    void Compile(TrecPointer<ArenaModel>& model, TString& err);
    void Compile(D3D11_INPUT_ELEMENT_DESC* desc, UINT count, TString& err, TDataArray<float>& vertices);
};

class T3DModelParserBuilder :
    public TObject
{
public:
    virtual bool FormatSupport(const TString& extension) = 0;
    virtual TrecPointer<T3DModelParser> GetParser(TrecPointer<TFileShell> filepath, TrecPointer<TArenaEngine> engine) = 0;
    virtual void GetSupportedFormats(TDataArray<TString>& subFormats) = 0;
};


bool Is3DModelFormatSupported(TrecPointer<TFileShell> filepath);
TrecPointer<T3DModelParser> Get3DParser(TrecPointer<TFileShell> filepath, TrecPointer<TArenaEngine> engine);
void Submit3DParserBuilder(TrecPointer<T3DModelParserBuilder> builder);

