#pragma once
#include "T3DModelParser.h"
class TObjModelParser :
    public T3DModelParser
{
    friend class TObjModelParserBuilder;
public:
    virtual TrecPointer<ArenaModel> Generate(const ShaderProgram& program, TString& err) override;
    virtual void Generate(TrecPointer<ArenaModel>& model, TString& err) override;
    virtual TString Save(TrecPointer<ArenaModel> model) override;

private:

    void ReadData(TString& err);

    TrecPointer<TFileShell> filePointer;
};

class TObjModelParserBuilder : public T3DModelParserBuilder
{
public:

    virtual bool FormatSupport(const TString& extension) override;
    virtual TrecPointer<T3DModelParser> GetParser(TrecPointer<TFileShell> filepath, TrecPointer<TArenaEngine> engine) override;
    virtual void GetSupportedFormats(TDataArray<TString>& subFormats) override;
};

