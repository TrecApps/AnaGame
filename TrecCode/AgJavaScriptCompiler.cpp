#include "AgJavaScriptCompiler.h"

TString AgJavaScriptCompiler::SupportsFiles()
{
    return L"js";
}

TString AgJavaScriptCompiler::SupportsOutput(TDataArray<ag_lang_output>& output)
{
    output.push_back(ag_lang_output::tc_interpreter);
    return TString();
}

TString AgJavaScriptCompiler::Configure(ag_lang_output output, TString& outputDetails, TrecPointer<TVariable> config)
{
    return TString();
}

bool AgJavaScriptCompiler::Prepare(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details)
{
    return false;
}

int AgJavaScriptCompiler::CompileResources(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details)
{
    return 0;
}

bool AgJavaScriptCompiler::CompleteCompile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details)
{
    return false;
}

TrecSubPointer<TVariable, TcRunner> AgJavaScriptCompiler::Getrunner()
{
    return TrecSubPointer<TVariable, TcRunner>();
}
