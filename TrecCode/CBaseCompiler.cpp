#include "CBaseCompiler.h"


CBaseCompiler::CBaseCompiler(TrecPointer<TFileShell> file)
{
    assert(file.Get());
    this->file = file;
}

TString CBaseCompiler::SupportsFiles()
{
    return L"agcpp;aghpp";
}

TString CBaseCompiler::SupportsOutput(TDataArray<ag_lang_output>& output)
{
    // Clear the output list
    output.RemoveAll();
    output.push_back(ag_lang_output::ag_runner);
    return TString();
}

TString CBaseCompiler::Configure(ag_lang_output output, TString& outputDetails, TrecPointer<TVariable> config)
{
    switch (output) {
    case ag_lang_output::ag_runner:
        break;
    default:
        return L"Unsupported Output";
    }
    //if (!config.Get())
    //    return L"Configuration not provided!";
    return TString();
}

bool CBaseCompiler::Prepare(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details)
{
    TcPreStatement::GenerateStatements(this->preStatements, TrecPointer<TVariable>(), file, messages);
    for (UINT Rust = 0; Rust < messages.Size(); Rust++)
        if (messages[Rust].isError)
            return false;


    return true;
}

int CBaseCompiler::CompileResources(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details)
{
    return 0;
}

bool CBaseCompiler::CompleteCompile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details)
{
    return false;
}

TrecPointer<AgCompiler> CBaseCompiler::Builder::GetCompiler(TrecPointer<TFileShell> file, ag_lang_output output)
{
    if (file.Get())
        return TrecPointer<AgCompiler>();
    TString ext(file->GetName().SubString(file->GetName().FindLast(L".") + 1));
    switch (output)
    {
    case ag_lang_output::ag_runner:
        return (ext.CompareNoCase(L"agcpp") && ext.CompareNoCase(L"aghpp")) ? TrecPointer<AgCompiler>() :
            TrecPointerKey::GetNewTrecPointerAlt<AgCompiler, CBaseCompiler>(file);
    default:
        return TrecPointer<AgCompiler>();
    }
}
