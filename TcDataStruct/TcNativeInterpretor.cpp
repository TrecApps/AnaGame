#include "TcNativeInterpretor.h"

TcNativeInterpretor::TcNativeInterpretor(NativeTFunction function, TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env): TcInterpretor(parent, env)
{
	nativeFunction = function;
}

COMPILE_TYPE TcNativeInterpretor::CanCompile()
{
	return NO_COMPILE;
}

void TcNativeInterpretor::Compile(COMPILE_TYPE type, ReturnObject& ret)
{
	ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
	ret.errorMessage.Set(L"Native Method is Already compiled!");
}

void TcNativeInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
}

ReturnObject TcNativeInterpretor::Run()
{
	ReturnObject result;
	if (!nativeFunction)
	{
		result.returnCode = ReturnObject::ERR_BROKEN_REF;
		result.errorMessage.Set(L"Null Reference to Native Function!\n");
		return result;
	}
	nativeFunction(defaultVars, environment, result);
	return result;
}

void TcNativeInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
	this->defaultVars = params;
}

void TcNativeInterpretor::PreProcess(ReturnObject& ret)
{
}

void TcNativeInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
}
