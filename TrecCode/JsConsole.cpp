#include "JsConsole.h"

#include <TContainerVariable.h>
#include <TPrimitiveVariable.h>

namespace JsConsole
{
	void Log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
		// Not an error, but silently fail if we don't have an environment or text to display.
		// Also, first parameter is expected to be the console object, which we can ignore
		if (!env.Get() || params.Size() < 2)
			return;

		if (!params.at(1).Get())
			env->PrintLine(L"undefined");
		else
		{
			auto message = params.at(1);

			TString finalMessage;

			if (message->GetVarType() == var_type::string)
			{
				int replacements = 0;
				TString initialMessage(message->GetString().GetReplace(replacements, L"\\\\", L"\\"));



				finalMessage.Set(initialMessage);
			}
			else
				finalMessage.Set(message->GetString());

			env->PrintLine(finalMessage);
		}
	}

	void Assert(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void Clear(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void Count(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
		if (!env.Get() || params.Size() < 1 || params.at(0).Get())
			return;

		auto console = params.at(0);

		if (console->GetVarType() != var_type::collection)
			return;

		TContainerVariable* container = dynamic_cast<TContainerVariable*>(console.Get());
		bool present;
		TrecSubPointer<TVariable, TPrimitiveVariable> counter = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(container->GetValue(L"counter", present));

		if (counter.Get())
		{
			UINT val = counter->Get4Value() + 1;
			counter->Set(val);
			env->Print(L"Default: ");
			env->PrintLine(counter->GetString());
		}

	}

	void Error(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void Group(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void GroupEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void Info(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void Time(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void TimeEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}

	void Warn(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
	{
	}


	TrecPointer<TVariable> GetLog(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Log, parent, env);
	}


	TrecPointer<TVariable> GetAssert(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Assert, parent, env);
	}


	TrecPointer<TVariable> GetClear(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Clear, parent, env);
	}


	TrecPointer<TVariable> GetCount(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Count, parent, env);
	}


	TrecPointer<TVariable> GetError(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Error, parent, env);
	}


	TrecPointer<TVariable> GetGroup(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Group, parent, env);
	}


	TrecPointer<TVariable> GetGroupEnd(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(GroupEnd, parent, env);
	}


	TrecPointer<TVariable> GetInfo(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Info, parent, env);
	}


	TrecPointer<TVariable> GetTime(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Time, parent, env);
	}


	TrecPointer<TVariable> GetTimeEnd(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(TimeEnd, parent, env);
	}


	TrecPointer<TVariable> GetWarn(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(Warn, parent, env);
	}


}