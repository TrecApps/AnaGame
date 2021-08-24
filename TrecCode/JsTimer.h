#pragma once
#include <TcNativeInterpretor.h>

namespace JsTimer
{
	
	void SetTimer(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void SetInterval(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void ClearInterval(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void GetFunctions(TDataMap<TcVariableHolder>& varHolds, TrecSubPointer<TVariable, TcInterpretor>, TrecPointer<TEnvironment> env);
}