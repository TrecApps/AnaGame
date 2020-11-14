#pragma once

#include <TNativeInterpretor.h>

// Native Functions that emulate Console Methods

namespace JsConsole
{

	void Log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Assert(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Clear(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Count(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Error(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Group(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void GroupEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Info(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Time(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void TimeEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	void Warn(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

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
}