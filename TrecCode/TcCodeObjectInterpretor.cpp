#include "TcCodeObjectInterpretor.h"

TcCodeObjectInterpretor::TcCodeObjectInterpretor(TrecPointer<TVariable> language, TrecPointer<TEnvironment> env): TcTypeInterpretor(TrecSubPointer<TVariable, TcInterpretor>(), env)
{
}

TcCodeObjectInterpretor::TcCodeObjectInterpretor(TrecSubPointer<TVariable, TcInterpretor> language, TrecPointer<TEnvironment> env) :TcTypeInterpretor(language, env)
{
}


void TcCodeObjectInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
}

ReturnObject TcCodeObjectInterpretor::Run()
{
	return ReturnObject();
}

void TcCodeObjectInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
}

void TcCodeObjectInterpretor::PreProcess(ReturnObject& ret)
{
}

void TcCodeObjectInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
}

TrecPointer<TVariable> TcCodeObjectInterpretor::Clone()
{
	return TrecPointer<TVariable>();
}
