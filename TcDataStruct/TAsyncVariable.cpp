#include "TAsyncVariable.h"
#include <TThread.h>
#include "TStringVariable.h"

DWORD __stdcall RunAsyncObject(LPVOID param)
{
    TrecSubPointer<TVariable, TAsyncVariable> asyncObj;
    TrecSubPointer<TVariable, TAsyncVariable>* asyncParam = (TrecSubPointer<TVariable, TAsyncVariable>*)param;

    asyncObj = *asyncParam;
    delete asyncParam;

    ReturnObject ret;

    TAsyncVariable::RunAsyncObject(asyncObj, ret);

    //if (asyncObj.Get())
    //{
    //    TThread::Resume(asyncObj->GetCallingThread());
    //}
    return ret.returnCode;
}



TrecPointer<TVariable> TAsyncVariable::Clone()
{    
    return TrecPointerKey::GetTrecPointerFromSoft < >(aSelf);
}

var_type TAsyncVariable::GetVarType()
{
    return var_type::async;
}

TrecObjectPointer TAsyncVariable::GetObject()
{
    return TrecObjectPointer();
}

TString TAsyncVariable::GetString()
{
    return TString();
}

UINT TAsyncVariable::Get4Value()
{
    return 0;
}

ULONG64 TAsyncVariable::Get8Value()
{
    return ULONG64();
}

UINT TAsyncVariable::GetSize()
{
    return 0;
}

UINT TAsyncVariable::GetVType()
{
    return 0;
}

TAsyncVariable::TAsyncVariable(DWORD thread, TrecSubPointer<TVariable, TcInterpretor> var)
{
    if (!var.Get() && thread)
        throw L"Null Pointer attached";
    this->mainFunction = var;
    this->mode = async_mode::m_waiting;
    this->requesterId = thread;
    this->progress = 0;
    this->subErrorResolve = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TContainerVariable>(ContainerType::ct_json_obj);
    this->containerResult = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);
}

void TAsyncVariable::GetSetMode(async_mode& m, bool get)
{
    ThreadLock();
    if (get)
        m = this->mode;
    else mode = m;
    ThreadRelease();
}

UINT TAsyncVariable::GetProgress()
{
    return progress;
}


void TAsyncVariable::AppendSuccessResponse(TrecSubPointer<TVariable, TcInterpretor> func, TrecSubPointer<TVariable, TcInterpretor> eFunc)
{
    if (!func.Get())
        return;
    AppendSuccessResponse(func);

    if (eFunc.Get())
    {
        TrecSubPointer<TVariable, TContainerVariable> cont = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(subErrorResolve);

        TString val;
        val.Format(L"%d", successResolve.Size() - 1);

        cont->SetValue(val, TrecPointerKey::GetTrecPointerFromSub<>(eFunc));

        if (!subErrorResolve.Get())
            subErrorResolve = TrecPointerKey::GetTrecPointerFromSub<>(cont);
    }
}

void TAsyncVariable::AppendSuccessResponse(TrecSubPointer<TVariable, TcInterpretor> func)
{
    TObjectLocker objThread(&thread);
    if (!func.Get())
        return;

    successResolve.push_back(TrecPointerKey::GetTrecPointerFromSub<>(func));

    if (mode == async_mode::m_complete)
        HandleSuccessResult();
}

void TAsyncVariable::SetFinally(TrecSubPointer<TVariable, TcInterpretor> func)
{
    finallyFunction = func;
}

void TAsyncVariable::SetErrorResponse(TrecSubPointer<TVariable, TcInterpretor> func)
{
    mainErrorResolve = TrecPointerKey::GetTrecPointerFromSub<>(func);
}

void TAsyncVariable::SetErrorResponse(TrecSubPointer<TVariable, TcInterpretor> func, UINT c)
{   
    if (c < successResolve.Size() && func.Get())
    {
        TrecSubPointer<TVariable, TContainerVariable> cont = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(subErrorResolve);
        TString cStr;
        cStr.Format(L"%d", c);
        cont->SetValue(cStr, TrecPointerKey::GetTrecPointerFromSub<>(func));
    }
}

void TAsyncVariable::RunAsyncObject(TrecSubPointer<TVariable, TAsyncVariable> asyncVar, ReturnObject& ret)
{
    if (!asyncVar.Get())
    {
        ret.returnCode = ReturnObject::ERR_BROKEN_REF;
        ret.errorMessage.Set(L"Error! Null Async Object passed!");
        return;
    }
    asyncVar->ThreadLock();
    async_mode aMode;
    asyncVar->GetSetMode(aMode);
    if (aMode != async_mode::m_waiting)
    {
        asyncVar->ThreadRelease();
        return;
    }
    aMode = async_mode::m_progress;
    asyncVar->GetSetMode(aMode, false);

    if(asyncVar->mainFunction.Get())
    ret = asyncVar->mainFunction->Run();
asyncVar->ThreadRelease();
    return;


    
}

DWORD TAsyncVariable::GetCallingThread()
{
    return requesterId;
}

UINT TAsyncVariable::GetResolveSize()
{
    return successResolve.Size();
}

/**
 * Method: TAsyncVariable::UpdateThread
 * Purpose: Updates the thread that should be alerted once the Async Object is resolved
 * Parameters: DWORD th - the id of the new thread
 * Returns: void
 */
void TAsyncVariable::UpdateThread(DWORD th)
{
    ThreadLock();
    this->requesterId = th;
    ThreadRelease();
}

void TAsyncVariable::GetResult(ReturnObject& ret)
{
    ret = this->ret;
}

DWORD TAsyncVariable::GetThreadCaller()
{
    return requesterId;
}


/**
 * Method: TAsyncVariable::SetParent
 * Purpose: Sets the Parent Interpretor of interpretors held by this object
 * Parameters:TrecSubPointer<TVariable, TcInterpretor> parent - the parent to set
 *              bool replace - whether to set the parent even if interpretor currently has one
 * Returns: void
 */
void TAsyncVariable::SetParent(TrecSubPointer<TVariable, TcInterpretor> parent, bool replace)
{
    ThreadLock();
    if (mainFunction.Get())
        mainFunction->SetParent(parent, replace);

    if (finallyFunction.Get())
        finallyFunction->SetParent(parent, replace);

    auto e = dynamic_cast<TcInterpretor*>(mainErrorResolve.Get());
    if (e)
        e->SetParent(parent, replace);

    for (UINT Rust = 0; Rust < this->successResolve.Size(); Rust++)
    {
        auto s = successResolve[Rust];
        if (!s.Get())
        {
            successResolve.RemoveAt(Rust--);
            continue;
        }
        TcInterpretor* tcInt = dynamic_cast<TcInterpretor*>(s.Get());
        if (tcInt)
            tcInt->SetParent(parent, replace);
    }

    if (subErrorResolve.Get())
    {
        if (subErrorResolve->GetVarType() == var_type::interpretor)
            dynamic_cast<TcInterpretor*>(subErrorResolve.Get())->SetParent(parent, replace);
        else if (subErrorResolve->GetVarType() == var_type::collection)
        {
            auto ec = dynamic_cast<TContainerVariable*>(subErrorResolve.Get());
            UINT Rust = 0;
            TrecPointer<TVariable> iVar = ec->GetValueAt(Rust++);
            while (iVar.Get())
            {
                dynamic_cast<TcInterpretor*>(iVar.Get())->SetParent(parent, replace);
                iVar = ec->GetValueAt(Rust++);
            }
        }
    }

    ThreadRelease();
}


/**
 * Method: TAsyncVariable::SetResult
 * Purpose: Allows Return Interpretors to set the result
 * Parameters: TrecPointer<TVariable> var - the value of the result
 *              bool success - whether it was a successful result or not
 * Returns: void
 */
void TAsyncVariable::SetResult(TrecPointer<TVariable> var, bool success)
{
    ThreadLock();
    ret.errorObject = var;
    containerResult->SetValue(L"value", var);
    ret.returnCode = success ? 0 : ReturnObject::ERR_GENERIC_ERROR;
    mode = success ? async_mode::m_complete : async_mode::m_error;

    if (success)
    {
        containerResult->SetValue(L"value", var);
        containerResult->SetValue(L"status", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TStringVariable>(L"fulfilled"));
        ret.returnCode = 0;
        mode = async_mode::m_initComplete;

        HandleSuccessResult();
    }
    else
    {
        containerResult->SetValue(L"reason", var);
        TString v(L"value");
        containerResult->RemoveByKey(v);
        containerResult->SetValue(L"status", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TStringVariable>(L"rejected"));
        ret.returnCode = ReturnObject::ERR_GENERIC_ERROR;

        HandleErrorResult();
    }
    if (finallyFunction.Get())
    {
        ret = finallyFunction->Run();

    }

    
    ThreadRelease();
}

TrecSubPointer<TVariable, TContainerVariable> TAsyncVariable::GetContainerResult()
{
    return containerResult;
}

void TAsyncVariable::HandleErrorResult()
{

    if (mainErrorResolve.Get())
    {
        TrecSubPointer<TVariable, TcInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(mainErrorResolve);

        TDataArray<TrecPointer<TVariable>> initVars;
        initVars.push_back(ret.errorObject);

        func->SetIntialVariables(initVars);

        ret = func->Run();
    }
    mode = async_mode::m_error;
}

void TAsyncVariable::SetSelf(TrecPointer<TVariable> self)
{
    if (this != self.Get())
        throw L"Pointers do not match!";
    aSelf = TrecPointerKey::GetSoftPointerFromTrec<>(self);
}

void TAsyncVariable::HandleSuccessResult()
{
    for (UINT Rust = progress; Rust < successResolve.Size(); Rust++)
    {
        TrecSubPointer<TVariable, TcInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(successResolve[Rust]);

        TDataArray<TrecPointer<TVariable>> initVars;
        initVars.push_back(ret.errorObject);

        func->SetIntialVariables(initVars);

        ret = func->Run();

        if (ret.returnCode)
        {
            // Try to get an error
            TString val;
            val.Format(L"%d", Rust);
            TrecPointer<TVariable > var;
            if (subErrorResolve.Get())
            {
                bool pres;
                var = dynamic_cast<TContainerVariable*>(subErrorResolve.Get())->GetValue(val, pres);
            }
            func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(var);
            if (func.Get())
            {
                initVars.RemoveAll();
                initVars.push_back(ret.errorObject);
                func->SetIntialVariables(initVars);
                ret = func->Run();
                break;
            }
            else if (mainErrorResolve.Get())
            {
                func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(mainErrorResolve);

                initVars.RemoveAll();
                initVars.push_back(ret.errorObject);

                func->SetIntialVariables(initVars);

                ret = func->Run();
            }

            // Catch Methods can reset the error status to 0, meaning we can go on. If this doesn't happen
            // Then the error remains and we break the loop
            if (ret.returnCode)
            {
                mode = async_mode::m_error;
                break;
            }
            progress++;
        }
        else
            progress++;
    }
    if (mode != async_mode::m_error)
    {
        mode = async_mode::m_complete;
    }
}

void TC_DATA_STRUCT ProcessTAsyncObject(TrecSubPointer<TVariable, TAsyncVariable> asyncVar)
{
    if (asyncVar.Get())
    {
        TrecSubPointer<TVariable, TAsyncVariable>* lpParam = new TrecSubPointer<TVariable, TAsyncVariable>(asyncVar);

        TThread::Resume(TThread::CreateTThread(RunAsyncObject, lpParam));

    }
}
