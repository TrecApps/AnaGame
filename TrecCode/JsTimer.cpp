#include "JsTimer.h"
#include <TThread.h>
#include <TPrimitiveFormatVariable.h>

class InterpretorParam
{
public:
    bool persist;
    TrecSubPointer<TVariable, TcInterpretor> function;
    UINT id;
    UINT sleep;
    InterpretorParam();
    InterpretorParam(const InterpretorParam& copy);
    void operator=(const InterpretorParam& copy);
};

TDataArray<DWORD> threadIds;


DWORD __stdcall RunTimerFunction(LPVOID param)
{
    InterpretorParam* paramParam = (InterpretorParam*)param;
    TrecSubPointer<TVariable, TcInterpretor> intObj = paramParam->function;
    bool persist = paramParam->persist;
    UINT indexId = paramParam->id;
    UINT sleep = paramParam->sleep;
    delete paramParam;
    ReturnObject ret;
    do
    {
        Sleep(sleep);
        if(threadIds[indexId])
        ret = intObj->Run();


    } while (persist && threadIds[indexId]);

    threadIds[indexId] = 0;
    return 0;
}



void JsTimer::SetTimer(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
    if (params.Size() < 2)
    {
        ret.returnCode = ret.ERR_TOO_FEW_PARMS;
        ret.errorMessage.Set(L"Too Few Params! Expected a function followed by a Number");
        return;
    }

    TrecSubPointer<TVariable, TcInterpretor> function = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[0]);
    TrecSubPointer<TVariable, TPrimitiveVariable> time = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(params[1]);

    if (!function.Get() || !time.Get())
    {
        ret.returnCode = ret.ERR_IMPROPER_TYPE;
        ret.errorMessage.Set(L"Type Exception! Expected a function followed by a Number");
        return;
    }
    TDataArray<TrecPointer<TVariable>> funcParams;
    for (UINT Rust = 2; Rust < params.Size(); Rust++)
    {
        funcParams.push_back(params[Rust]);
    }

    function->SetIntialVariables(funcParams);
    InterpretorParam* iParam = new InterpretorParam();
    iParam->function = function;
    iParam->sleep = time->Get4Value();

    DWORD threadId = TThread::CreateTThread(RunTimerFunction, iParam);

    bool slotFound = false;
    for (UINT Rust = 0; Rust < threadIds.Size(); Rust++)
    {
        if (!threadIds[Rust])
        {
            slotFound = true;
            threadIds[Rust] = threadId;
            iParam->id = Rust;
            break;
        }
    }
    if (!slotFound)
    {
        iParam->id = threadIds.push_back(threadId);
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(iParam->id);
    TThread::Resume(threadId);
}

void JsTimer::SetInterval(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
    if (params.Size() < 2)
    {
        ret.returnCode = ret.ERR_TOO_FEW_PARMS;
        ret.errorMessage.Set(L"Too Few Params! Expected a function followed by a Number");
        return;
    }

    TrecSubPointer<TVariable, TcInterpretor> function = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[0]);
    TrecSubPointer<TVariable, TPrimitiveVariable> time = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(params[1]);

    if (!function.Get() || !time.Get())
    {
        ret.returnCode = ret.ERR_IMPROPER_TYPE;
        ret.errorMessage.Set(L"Type Exception! Expected a function followed by a Number");
        return;
    }
    TDataArray<TrecPointer<TVariable>> funcParams;
    for (UINT Rust = 2; Rust < params.Size(); Rust++)
    {
        funcParams.push_back(params[Rust]);
    }

    function->SetIntialVariables(funcParams);
    InterpretorParam* iParam = new InterpretorParam();
    iParam->function = function;
    iParam->sleep = time->Get4Value();
    iParam->persist = true;

    DWORD threadId = TThread::CreateTThread(RunTimerFunction, iParam);

    bool slotFound = false;
    for (UINT Rust = 0; Rust < threadIds.Size(); Rust++)
    {
        if (!threadIds[Rust])
        {
            slotFound = true;
            threadIds[Rust] = threadId;
            iParam->id = Rust;
            break;
        }
    }
    if (!slotFound)
    {
        iParam->id = threadIds.push_back(threadId);
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(iParam->id);
    TThread::Resume(threadId);
}

void JsTimer::ClearInterval(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
    if (!params.Size())
    {
        ret.returnCode = ret.ERR_TOO_FEW_PARMS;
        ret.errorMessage.Set(L"Too Few Params! Expected a number");
        return;
    }
    TrecSubPointer<TVariable, TPrimitiveVariable> id = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(params[0]);
    if (!id.Get())
    {
        ret.returnCode = ret.ERR_IMPROPER_TYPE;
        ret.errorMessage.Set(L"Type Exception! Expected a Number");
        return;
    }

    UINT index = id->Get4Value();
    if (index < threadIds.Size())
        threadIds[index] = 0;
}

void JsTimer::GetFunctions(TDataMap<TcVariableHolder>& varHolds, TrecSubPointer<TVariable, TcInterpretor> par, TrecPointer<TEnvironment> env)
{
    TrecSubPointer<TVariable, TcNativeInterpretor> st = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcNativeInterpretor>(SetTimer, par, env);
    TrecSubPointer<TVariable, TcNativeInterpretor> si = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcNativeInterpretor>(SetInterval, par, env);
    TrecSubPointer<TVariable, TcNativeInterpretor> ci = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcNativeInterpretor>(ClearInterval, par, env);

    varHolds.setEntry(L"setTimeout", TcVariableHolder(false, L"", TrecPointerKey::GetTrecPointerFromSub<>(st)));
    varHolds.setEntry(L"setInterval", TcVariableHolder(false, L"", TrecPointerKey::GetTrecPointerFromSub<>(si)));
    varHolds.setEntry(L"clearTimeout", TcVariableHolder(false, L"", TrecPointerKey::GetTrecPointerFromSub<>(ci)));
    varHolds.setEntry(L"clearInterval", TcVariableHolder(false, L"", TrecPointerKey::GetTrecPointerFromSub<>(ci)));
}

InterpretorParam::InterpretorParam()
{
    persist = false;
    id = sleep = 0;
}

InterpretorParam::InterpretorParam(const InterpretorParam& copy)
{
    this->function = copy.function;
    this->persist = copy.persist;
    this->id = copy.id;
    this->sleep = copy.sleep;
}

void InterpretorParam::operator=(const InterpretorParam& copy)
{
    this->function = copy.function;
    this->persist = copy.persist;
    this->id = copy.id;
    this->sleep = copy.sleep;
}
