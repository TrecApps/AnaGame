#include "JsPromise.h"
#include <TAsyncVariable.h>
#include <TReturnInterpretor.h>
#include <TContainerVariable.h>
#include <TThread.h>
#include <TStringVariable.h>






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

	TrecSubPointer<TVariable, TcInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[0]);

	TDataArray<TrecPointer<TVariable>> newParams;
	newParams.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TReturnInterpretor>(TrecPointer<TVariable>(), env, true, true));

	newParams.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TReturnInterpretor>(TrecPointer<TVariable>(), env, false, true));
	func->SetIntialVariables(newParams);
	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>
		(GetCurrentThreadId(), func);

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
	if (!params.Size())
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Set(L"undefined is not iterable(cannot read property Symbol(Symbol.iterator))");
		return;
	}
	if (!params[0].Get() || params[0]->GetVarType() != var_type::collection)
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Format(L"%ws is not iterable(cannot read property Symbol(Symbol.iterator))", params[0].Get() ? params[0]->GetString().GetConstantBuffer().getBuffer() : L"null");
		return;
	}

	TrecSubPointer<TVariable, TcNativeInterpretor> nativeRunner = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcNativeInterpretor>(JsPromiseAllSub, TrecSubPointer<TVariable, TcInterpretor>(), env);
	TDataArray<TrecPointer<TVariable>> clonedParams;
	for (UINT Rust = 0; Rust < params.Size(); Rust++)
		clonedParams.push_back(params[Rust].Get() ? params[Rust]->Clone() : TrecPointer<TVariable>());
	nativeRunner->SetIntialVariables(clonedParams);
	TrecPointer<TVariable> varRunner = TrecPointerKey::GetTrecPointerFromSub<>(nativeRunner);
	TrecSubPointer<TVariable, TcInterpretor> regRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(varRunner);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAsyncVariable>(GetCurrentThreadId(), regRunner);
}

void JsPromise::JsPromiseAllSettled(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size())
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Set(L"undefined is not iterable(cannot read property Symbol(Symbol.iterator))");
		return;
	}
	if (!params[0].Get() || params[0]->GetVarType() != var_type::collection)
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Format(L"%ws is not iterable(cannot read property Symbol(Symbol.iterator))", params[0].Get() ? params[0]->GetString().GetConstantBuffer().getBuffer() : L"null");
		return;
	}

	TrecSubPointer<TVariable, TcNativeInterpretor> nativeRunner = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcNativeInterpretor>(JsPromiseAllSettledSub, TrecSubPointer<TVariable, TcInterpretor>(), env);
	TDataArray<TrecPointer<TVariable>> clonedParams;
	for (UINT Rust = 0; Rust < params.Size(); Rust++)
		clonedParams.push_back(params[Rust].Get() ? params[Rust]->Clone() : TrecPointer<TVariable>());
	nativeRunner->SetIntialVariables(clonedParams);
	TrecPointer<TVariable> varRunner = TrecPointerKey::GetTrecPointerFromSub<>(nativeRunner);
	TrecSubPointer<TVariable, TcInterpretor> regRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(varRunner);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAsyncVariable>(GetCurrentThreadId(), regRunner);
}

void JsPromise::JsPromiseAny(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size())
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Set(L"undefined is not iterable(cannot read property Symbol(Symbol.iterator))");
		return;
	}
	if (!params[0].Get() || params[0]->GetVarType() != var_type::collection)
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Format(L"%ws is not iterable(cannot read property Symbol(Symbol.iterator))", params[0].Get() ? params[0]->GetString().GetConstantBuffer().getBuffer() : L"null");
		return;
	}

	TrecSubPointer<TVariable, TcNativeInterpretor> nativeRunner = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcNativeInterpretor>(JsPromiseAnySub, TrecSubPointer<TVariable, TcInterpretor>(), env);
	TDataArray<TrecPointer<TVariable>> clonedParams;
	for (UINT Rust = 0; Rust < params.Size(); Rust++)
		clonedParams.push_back(params[Rust].Get() ? params[Rust]->Clone() : TrecPointer<TVariable>());
	nativeRunner->SetIntialVariables(clonedParams);
	TrecPointer<TVariable> varRunner = TrecPointerKey::GetTrecPointerFromSub<>(nativeRunner);
	TrecSubPointer<TVariable, TcInterpretor> regRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(varRunner);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAsyncVariable>(GetCurrentThreadId(), regRunner);
}

void JsPromise::JsPromiseRace(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size())
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Set(L"undefined is not iterable(cannot read property Symbol(Symbol.iterator))");
		return;
	}
	if (!params[0].Get() || params[0]->GetVarType() != var_type::collection)
	{
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Format(L"%ws is not iterable(cannot read property Symbol(Symbol.iterator))", params[0].Get() ? params[0]->GetString().GetConstantBuffer().getBuffer() : L"null");
		return;
	}

	TrecSubPointer<TVariable, TcNativeInterpretor> nativeRunner = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcNativeInterpretor>(JsPromiseRaceSub, TrecSubPointer<TVariable, TcInterpretor>(), env);
	TDataArray<TrecPointer<TVariable>> clonedParams;
	for (UINT Rust = 0; Rust < params.Size(); Rust++)
		clonedParams.push_back(params[Rust].Get() ? params[Rust]->Clone() : TrecPointer<TVariable>());
	nativeRunner->SetIntialVariables(clonedParams);
	TrecPointer<TVariable> varRunner = TrecPointerKey::GetTrecPointerFromSub<>(nativeRunner);
	TrecSubPointer<TVariable, TcInterpretor> regRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(varRunner);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAsyncVariable>(GetCurrentThreadId(), regRunner);
}

void JsPromise::JsPromiseReject(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecPointer<TVariable> retFunc = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TReturnInterpretor>(params.Size() ? params[0] : TrecPointer<TVariable>(), env, false);

	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>
		(GetCurrentThreadId(), TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(retFunc));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseResolve(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecPointer<TVariable> retFunc = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TReturnInterpretor>(params.Size() ? params[0] : TrecPointer<TVariable>(), env, true);

	TrecSubPointer<TVariable, TAsyncVariable> prom = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>
		(GetCurrentThreadId(), TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(retFunc));

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(prom);
}

void JsPromise::JsPromiseAllSub(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TContainerVariable> promises = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	TDataArray<TrecPointer<TVariable>> returnEntries;
	TDataArray<TrecSubPointer<TVariable, TAsyncVariable>> asyncEntries;

	for (UINT Rust = 0; Rust < promises->GetSize(); Rust++)
	{
		TrecPointer<TVariable> var = promises->GetValueAt(Rust);
		returnEntries.push_back(var);

		TrecSubPointer<TVariable, TAsyncVariable> aVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(var);
		if (aVar.Get())
		{
			aVar->UpdateThread(GetCurrentThreadId());
			asyncEntries.push_back(aVar);
		}
	}

	for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
	{
		ProcessTAsyncObject(asyncEntries[Rust]);
	}
	async_mode aMode = async_mode::m_waiting;
	while (asyncEntries.Size())
	{
		Sleep(20);
		for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
		{
			asyncEntries[Rust]->GetSetMode(aMode, false);
			if (aMode == async_mode::m_complete)
			{
				asyncEntries.RemoveAt(Rust--);
			}
			else if (aMode == async_mode::m_error)
			{
				asyncEntries[Rust]->GetResult(ret);
				ret.errorMessage.Set(L"Uncaught (in promise)");
				return;
			}
		}
	}

	TrecSubPointer<TVariable, TContainerVariable> arrayRet = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	for (UINT Rust = 0; Rust < returnEntries.Size(); Rust++)
	{
		TrecPointer<TVariable> var = returnEntries[Rust];
		TrecSubPointer<TVariable, TAsyncVariable> aVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(var);
		
		if (aVar.Get())
		{
			ReturnObject tempRet;
			aVar->GetResult(tempRet);
			arrayRet->AppendValue(tempRet.errorObject);
		}
		else
			arrayRet->AppendValue(var);
	}

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(arrayRet);
}

void JsPromise::JsPromiseAllSettledSub(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TContainerVariable> promises = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	TDataArray<TrecPointer<TVariable>> returnEntries;
	TDataArray<TrecSubPointer<TVariable, TAsyncVariable>> asyncEntries;

	for (UINT Rust = 0; Rust < promises->GetSize(); Rust++)
	{
		TrecPointer<TVariable> var = promises->GetValueAt(Rust);
		returnEntries.push_back(var);

		TrecSubPointer<TVariable, TAsyncVariable> aVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(var);
		if (aVar.Get())
		{
			aVar->UpdateThread(GetCurrentThreadId());
			asyncEntries.push_back(aVar);
		}
	}

	for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
	{
		ProcessTAsyncObject(asyncEntries[Rust]);
	}
	async_mode aMode = async_mode::m_waiting;
	while (asyncEntries.Size())
	{
		Sleep(20);
		for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
		{
			asyncEntries[Rust]->GetSetMode(aMode, false);
			if (aMode == async_mode::m_complete || aMode == async_mode::m_error)
			{
				asyncEntries.RemoveAt(Rust--);
			}
		}
	}

	TrecSubPointer<TVariable, TContainerVariable> arrayRet = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	for (UINT Rust = 0; Rust < returnEntries.Size(); Rust++)
	{
		TrecPointer<TVariable> var = returnEntries[Rust];
		TrecSubPointer<TVariable, TAsyncVariable> aVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(var);

		TrecSubPointer<TVariable, TContainerVariable> returnEntry = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

		if (aVar.Get())
		{
			ReturnObject tempRet;
			aVar->GetResult(tempRet);

			if (tempRet.returnCode)
				returnEntry->SetValue(L"status", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(L"rejected"));
			else
				returnEntry->SetValue(L"status", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(L"fulfilled"));

			returnEntry->SetValue(L"value", tempRet.errorObject);
		}
		else
		{
			returnEntry->SetValue(L"status", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(L"fulfilled"));
			returnEntry->SetValue(L"value", var);
		}
		arrayRet->AppendValue(TrecPointerKey::GetTrecPointerFromSub<>(returnEntry));
	}

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(arrayRet);
}

void JsPromise::JsPromiseAnySub(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TContainerVariable> promises = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	TDataArray<TrecPointer<TVariable>> returnEntries;
	TDataArray<TrecSubPointer<TVariable, TAsyncVariable>> asyncEntries;

	for (UINT Rust = 0; Rust < promises->GetSize(); Rust++)
	{
		TrecPointer<TVariable> var = promises->GetValueAt(Rust);
		returnEntries.push_back(var);

		TrecSubPointer<TVariable, TAsyncVariable> aVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(var);
		if (aVar.Get())
		{
			aVar->UpdateThread(GetCurrentThreadId());
			asyncEntries.push_back(aVar);
		}
		else
		{
			ret.errorObject = var;
			return;
		}
	}

	for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
	{
		ProcessTAsyncObject(asyncEntries[Rust]);
	}
	async_mode aMode = async_mode::m_waiting;
	while (asyncEntries.Size())
	{
		Sleep(20);
		for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
		{
			asyncEntries[Rust]->GetSetMode(aMode, false);
			if (aMode == async_mode::m_error)
			{
				asyncEntries.RemoveAt(Rust--);
			}
			else if (aMode == async_mode::m_complete)
			{
				asyncEntries[Rust]->GetResult(ret);
				return;
			}
		}
	}

	ret.returnCode = ret.ERR_GENERIC_ERROR;
	ret.errorMessage.Set(L"All Promises Rejected!");
}

void JsPromise::JsPromiseRaceSub(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TContainerVariable> promises = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	TDataArray<TrecPointer<TVariable>> returnEntries;
	TDataArray<TrecSubPointer<TVariable, TAsyncVariable>> asyncEntries;

	for (UINT Rust = 0; Rust < promises->GetSize(); Rust++)
	{
		TrecPointer<TVariable> var = promises->GetValueAt(Rust);
		returnEntries.push_back(var);

		TrecSubPointer<TVariable, TAsyncVariable> aVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(var);
		if (aVar.Get())
		{
			aVar->UpdateThread(GetCurrentThreadId());
			asyncEntries.push_back(aVar);
		}
		else
		{
			ret.errorObject = var;
			return;
		}
	}

	for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
	{
		ProcessTAsyncObject(asyncEntries[Rust]);
	}
	async_mode aMode = async_mode::m_waiting;
	while (asyncEntries.Size())
	{
		Sleep(20);
		for (UINT Rust = 0; Rust < asyncEntries.Size(); Rust++)
		{
			asyncEntries[Rust]->GetSetMode(aMode, false);
			if (aMode == async_mode::m_error || aMode == async_mode::m_complete)
			{
				asyncEntries[Rust]->GetResult(ret);
				return;
			}
		}
	}
}
