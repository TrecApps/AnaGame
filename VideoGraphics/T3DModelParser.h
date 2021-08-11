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
public:

    virtual TrecPointer<ArenaModel> Generate(const ShaderProgram& program) = 0;
    virtual TString Save(TrecPointer<ArenaModel> model) = 0;

protected:
    TDataArray<T4Vertex> vertices4;
    TDataArray<TVertex> vertices;
    TDataArray<T2Float> normal;
    TDataArray<T2Float> textures;
    TDataArray<UINT> indices;
};

class T3DModelParserBuilder :
    public TObject
{
public:
    virtual bool FormatSupport(const TString& extension) = 0;
    virtual TrecPointer<T3DModelParser> GetParser(TrecPointer<TFileShell> filepath) = 0;
    virtual void GetSupportedFormats(TDataArray<TString>& subFormats) = 0;
};


bool Is3DModelFormatSupported(TrecPointer<TFileShell> filepath);
TrecPointer<T3DModelParser> Get3DParser(TrecPointer<TFileShell> filepath);
void Submit3DParserBuilder(TrecPointer<T3DModelParserBuilder> builder);

