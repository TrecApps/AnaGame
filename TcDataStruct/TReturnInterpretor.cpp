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
	if (asyncVar.Get())
		asyncVar->SetResult(var, success);
	if (superReturn)
		ret.mode = return_mode::rm_super_return;
	return ret;
}

void TReturnInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
	if(returnByCall)
	var = params.Size() ? params[0] : TrecPointer<TVariable>();
}

TReturnInterpretor::TReturnInterpretor(TrecPointer<TVariable> object, TrecPointer<TEnvironment> env, bool success, bool returnByCall, bool initiateSuperReturn)
	: TcInterpretor(TrecSubPointer<TVariable, TcInterpretor>(), env)
{
	var = object;
	this->success = success;
	this->returnByCall = returnByCall;
	this->superReturn = initiateSuperReturn;
}

void TReturnInterpretor::PreProcess(ReturnObject& ret)
{
}

void TReturnInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
	ret.returnCode = ret.ERR_UNSUPPORTED_OP;
	ret.errorMessage.Set(L"This Interpretor simply returns a value!");
}


/**
 * Method: TReturnInterpretor::SetAsyncHold
 * Purpose: An Async Variable to attach (which will then be updated with the provided variable upon being called)
 * Parameters: TrecSubPoiunter<TVariable, TAsyncVariable> sync - the async var to attach
 * Returns: void
 */
void TReturnInterpretor::SetAsyncHold(TrecSubPointer<TVariable, TAsyncVariable> sync)
{
	this->asyncVar = sync;
}