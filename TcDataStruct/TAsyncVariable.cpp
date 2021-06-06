#include "TAsyncVariable.h"

TrecPointer<TVariable> TAsyncVariable::Clone()
{
    TrecSubPointer<TVariable, TAsyncVariable> var = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAsyncVariable>(this->requesterId, this->var);
    var->progress = this->progress;
    var->mode = this->mode;
    
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
    return var.Get() ? var->GetSize() : 0;
}

UINT TAsyncVariable::GetVType()
{
    return 0;
}

TAsyncVariable::TAsyncVariable(DWORD thread, TrecPointer<TVariable> var)
{
    this->var = var;
    this->mode = async_mode::m_waiting;
    this->requesterId = thread;
    this->progress = 0;
}

void TAsyncVariable::GetSetMode(async_mode& m, bool get)
{
    if (get)
        m = this->mode;
    else mode = m;
}

UINT TAsyncVariable::GetProgress()
{
    return progress;
}

TrecPointer<TVariable> TAsyncVariable::GetVariable()
{
    return var;
}
