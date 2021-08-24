#pragma once

#include <TNativeInterpretor.h>
#include <TcInterpretor.h>

// Native Functions that emulate Console Methods

namespace JsConsole
{

	void Log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Assert(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Clear(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Count(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Error(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Group(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void GroupEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Info(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Time(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void TimeEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	void Warn(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);


	TrecPointer<TVariable> GetLog(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetAssert(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetClear(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetCount(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetError(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetGroup(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetGroupEnd(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetInfo(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetTime(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetTimeEnd(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetWarn(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetLog(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetAssert(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetClear(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetCount(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetError(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetGroup(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetGroupEnd(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetInfo(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetTime(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetTimeEnd(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TrecPointer<TVariable> GetWarn(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);
}