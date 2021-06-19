#include "JsPromise.h"
#include <TAsyncVariable.h>
#include <TReturnInterpretor.h>






TClassStruct JsPromise::GetPromiseClass(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
{
	TClassStruct ret;

	TClassAttribute att;
	att.name.Set(L"constructor");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseConstructor, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"then");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseThen, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"catch");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseCatch, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"finally");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseFinally, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"all");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseAll, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"allSettled");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseAllSettled, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"any");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseAny, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"race");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseRace, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"reject");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseReject, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"resolve");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseResolve, parent, env);
	ret.AddAttribute(att);

	return ret;
}

void JsPromise::JsPromiseConstructor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size() || !params[0].Get() || params[0]->GetVarType() != var_type::interpretor)
	{
		ret.returnCode = ReturnObject::ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"Promise Constructor required a function to run!");
		return;
	}

	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>
		(GetCurrentThreadId(), TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[0]));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseCatch(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size() || !params[0].Get() || params[0]->GetVarType() != var_type::async)
	{
		ret.returnCode = ReturnObject::ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"First Param for Promise.Catch requires an Async Object to operate on!");
		return;
	}

	if (params.Size() < 2 || !params[1].Get() || params[1]->GetVarType() != var_type::interpretor)
	{
		ret.returnCode = ReturnObject::ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"Param for Promise.Catch requires a function to call!");
		return;
	}

	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(params[0]);

	UINT resSize = prom->GetResolveSize();

	if (resSize)
		prom->SetErrorResponse(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[1]), resSize - 1);
	else
		prom->SetErrorResponse(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[1]));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseThen(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size() || !params[0].Get() || params[0]->GetVarType() != var_type::async)
	{
		ret.returnCode = ReturnObject::ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"First Param for Promise.Catch requires an Async Object to operate on!");
		return;
	}

	if (params.Size() < 2 || !params[1].Get() || params[1]->GetVarType() != var_type::interpretor)
	{
		ret.returnCode = ReturnObject::ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"Param for Promise.Catch requires a function to call!");
		return;
	}

	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(params[0]);

	prom->AppendSuccessResponse(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[1]));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseFinally(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size() || !params[0].Get() || params[0]->GetVarType() != var_type::async)
	{
		ret.returnCode = ReturnObject::ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"First Param for Promise.Catch requires an Async Object to operate on!");
		return;
	}

	if (params.Size() < 2 || !params[1].Get() || params[1]->GetVarType() != var_type::interpretor)
	{
		ret.returnCode = ReturnObject::ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"Param for Promise.Catch requires a function to call!");
		return;
	}

	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(params[0]);

	prom->SetFinally(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[1]));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseAll(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseAllSettled(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseAny(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseRace(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseReject(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>
		(GetCurrentThreadId(), TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TReturnInterpretor>(params.Size() ? params[0] : TrecPointer<TVariable>(), env, false));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseResolve(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>
		(GetCurrentThreadId(), TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TReturnInterpretor>(params.Size() ? params[0] : TrecPointer<TVariable>(), env, true));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseAllSub(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseAllSettledSub(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}
