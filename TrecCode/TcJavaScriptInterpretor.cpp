#include "TcJavaScriptInterpretor.h"

COMPILE_TYPE TcJavaScriptInterpretor::CanCompile()
{
    return COMPILE_TYPE(0);
}

void TcJavaScriptInterpretor::Compile(COMPILE_TYPE type, ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
}

ReturnObject TcJavaScriptInterpretor::Run()
{
    return ReturnObject();
}

void TcJavaScriptInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
}

void TcJavaScriptInterpretor::PreProcess(ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
}
