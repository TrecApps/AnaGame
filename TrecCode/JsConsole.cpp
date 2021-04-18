#include "JsConsole.h"

#include <TContainerVariable.h>
#include <TPrimitiveVariable.h>
#include <TcNativeInterpretor.h>

namespace JsConsole
{
	void Log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		// Not an error, but silently fail if we don't have an environment or text to display.
		if (!env.Get() || params.Size() < 1)
			return;

		if (!params.at(0).Get())
			env->PrintLine(L"undefined");
		else
		{
			auto message = params.at(0);

			TString finalMessage;

			if (message->GetVarType() == var_type::string)
			{
				int replacements = 0;
				TString initialMessage(message->GetString());
				initialMessage.Replace(L"\\\\", L"\\");
				initialMessage.Replace(L"\\t", L"\t");
				initialMessage.Replace(L"\\n", L"\n");
				initialMessage.Replace(L"\\r", L"\r");


				finalMessage.Set(initialMessage);
			}
			else
				finalMessage.Set(message->GetString());

			env->PrintLine(finalMessage);
		}
	}

	void Assert(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void Clear(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void Count(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

	void Error(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void Group(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void GroupEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void Info(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void Time(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void TimeEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}

	void Warn(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
	}


	TrecPointer<TVariable> GetLog(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetAssert(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetClear(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetCount(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetError(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetGroup(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetGroupEnd(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetInfo(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetTime(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetTimeEnd(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return  TrecPointer<TVariable>();
	}


	TrecPointer<TVariable> GetWarn(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointer<TVariable>();
	}

	TrecPointer<TVariable> GetLog(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Log, parent, env);
	}


	TrecPointer<TVariable> GetAssert(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Assert, parent, env);
	}


	TrecPointer<TVariable> GetClear(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Clear, parent, env);
	}


	TrecPointer<TVariable> GetCount(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Count, parent, env);
	}


	TrecPointer<TVariable> GetError(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Error, parent, env);
	}


	TrecPointer<TVariable> GetGroup(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Group, parent, env);
	}


	TrecPointer<TVariable> GetGroupEnd(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(GroupEnd, parent, env);
	}


	TrecPointer<TVariable> GetInfo(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Info, parent, env);
	}


	TrecPointer<TVariable> GetTime(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Time, parent, env);
	}


	TrecPointer<TVariable> GetTimeEnd(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(TimeEnd, parent, env);
	}


	TrecPointer<TVariable> GetWarn(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
	{
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(Warn, parent, env);
	}

}