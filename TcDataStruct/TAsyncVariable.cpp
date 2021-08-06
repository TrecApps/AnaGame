#include "TAsyncVariable.h"
#include "TContainerVariable.h"
#include <TThread.h>

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
    TrecSubPointer<TVariable, TAsyncVariable> var = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>(this->requesterId, this->mainFunction);
    var->progress = this->progress;
    var->mode = this->mode;
    var->mainErrorResolve = this->mainErrorResolve;
    var->subErrorResolve = this->subErrorResolve;
    var->successResolve = this->successResolve;
    
    return TrecPointerKey::GetTrecPointerFromSub < >(var);
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
    if (!func.Get())
        return;

    successResolve.push_back(TrecPointerKey::GetTrecPointerFromSub<>(func));
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

    if (ret.returnCode)
    {
        if (asyncVar->mainErrorResolve.Get())
        {
            TrecSubPointer<TVariable, TcInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(asyncVar->mainErrorResolve);

            TDataArray<TrecPointer<TVariable>> initVars;
            initVars.push_back(ret.errorObject);

            func->SetIntialVariables(initVars);

            ret = func->Run();
            asyncVar->ret = ret;
        }
        aMode = async_mode::m_error;
        asyncVar->GetSetMode(aMode, false);
    }
    else
    {
        for (UINT Rust = 0; Rust < asyncVar->successResolve.Size(); Rust++)
        {
            TrecSubPointer<TVariable, TcInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(asyncVar->successResolve[Rust]);

            TDataArray<TrecPointer<TVariable>> initVars;
            initVars.push_back(ret.errorObject);

            func->SetIntialVariables(initVars);

            ret = func->Run();

            asyncVar->ret = ret;
            if (ret.returnCode)
            {
                // Try to get an error
                TString val;
                val.Format(L"%d", Rust);
                TrecPointer<TVariable > var;
                if (asyncVar->subErrorResolve.Get())
                {
                    bool pres;
                    var = dynamic_cast<TContainerVariable*>(asyncVar->subErrorResolve.Get())->GetValue(val, pres);
                }
                func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(var);
                if (func.Get())
                {
                    initVars.RemoveAll();
                    initVars.push_back(ret.errorObject);
                    func->SetIntialVariables(initVars);
                    ret = func->Run();
                    asyncVar->ret = ret;
                    break;
                }
                else if (asyncVar->mainErrorResolve.Get())
                {
                    func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(asyncVar->mainErrorResolve);

                    initVars.RemoveAll();
                    initVars.push_back(ret.errorObject);

                    func->SetIntialVariables(initVars);

                    ret = func->Run();
                    asyncVar->ret = ret;
                }

                // Catch Methods can reset the error status to 0, meaning we can go on. If this doesn't happen
                // Then the error remains and we break the loop
                if (ret.returnCode)
                {
                    aMode = async_mode::m_error;
                    asyncVar->GetSetMode(aMode, false);
                    break;
                }
                asyncVar->progress++;
            }
            else
                asyncVar->progress++;
        }
        asyncVar->GetSetMode(aMode);
        if (aMode != async_mode::m_error)
        {
            aMode = async_mode::m_complete;
            asyncVar->GetSetMode(aMode, false);
        }


    }

    if (asyncVar->finallyFunction.Get())
    {
        ReturnObject newRet = asyncVar->finallyFunction->Run();

        if (!asyncVar->ret.returnCode)
            asyncVar->ret.returnCode = newRet.returnCode;

    }
    asyncVar->ThreadRelease();
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
    ret.returnCode = success ? 0 : ReturnObject::ERR_GENERIC_ERROR;
    ThreadRelease();
}

void TC_DATA_STRUCT ProcessTAsyncObject(TrecSubPointer<TVariable, TAsyncVariable> asyncVar)
{
    if (asyncVar.Get())
    {
        TrecSubPointer<TVariable, TAsyncVariable>* lpParam = new TrecSubPointer<TVariable, TAsyncVariable>(asyncVar);

        TThread::Resume(TThread::CreateTThread(RunAsyncObject, lpParam));

    }
}
