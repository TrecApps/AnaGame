#include "TcBashInterpretor.h"

COMPILE_TYPE TcBashInterpretor::CanCompile()
{
    return COMPILE_TYPE(0);
}

void TcBashInterpretor::Compile(COMPILE_TYPE type, ReturnObject& ret)
{
}

void TcBashInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
}

ReturnObject TcBashInterpretor::Run()
{
    return ReturnObject();
}

void TcBashInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
}

void TcBashInterpretor::PreProcess(ReturnObject& ret)
{
}

void TcBashInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
}
