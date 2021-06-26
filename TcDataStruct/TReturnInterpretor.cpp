#include "TReturnInterpretor.h"

COMPILE_TYPE TReturnInterpretor::CanCompile()
{
	return 0;
}

void TReturnInterpretor::Compile(COMPILE_TYPE type, ReturnObject& ret)
{
	ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
}

void TReturnInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
}

ReturnObject TReturnInterpretor::Run()
{
	ReturnObject ret;
	ret.errorObject = var;
	ret.returnCode = success ? ReturnObject::ERR_NO_ERROR : ReturnObject::ERR_GENERIC_ERROR;
	return ret;
}

void TReturnInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
}

TReturnInterpretor::TReturnInterpretor(TrecPointer<TVariable> object, TrecPointer<TEnvironment> env, bool success)
	: TcInterpretor(TrecSubPointer<TVariable, TcInterpretor>(), env)
{
	var = object;
	this->success = success;
}

void TReturnInterpretor::PreProcess(ReturnObject& ret)
{
}

void TReturnInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
	ret.returnCode = ret.ERR_UNSUPPORTED_OP;
	ret.errorMessage.Set(L"This Interpretor simply returns a value!");
}
