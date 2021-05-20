#include "TIteratorVariable.h"

var_type TIteratorVariable::GetVarType()
{
    return var_type::iterator;
}

TrecObjectPointer TIteratorVariable::GetObject()
{
    return TrecObjectPointer();
}

TString TIteratorVariable::GetString()
{
    return TString();
}

UINT TIteratorVariable::Get4Value()
{
    return 0;
}

ULONG64 TIteratorVariable::Get8Value()
{
    return ULONG64();
}

UINT TIteratorVariable::GetSize()
{
    if (collection.Get())
        return collection->GetSize();
    return 1;
}

UINT TIteratorVariable::GetType()
{
    return 0;
}

TIteratorVariable::TIteratorVariable(TrecSubPointer<TVariable, TContainerVariable> coll)
{
    if (!coll.Get())
        throw L"Null Parameter Detected!";
    collection = coll;
    index = 0;
    fDone = false;
}

TIteratorVariable::TIteratorVariable(TrecSubPointer<TVariable, TcInterpretor> coll)
{
    if (!coll.Get())
        throw L"Null Parameter Detected!";
    function = coll;
    index = 0;
    fDone = false;
}

TrecPointer<TVariable> TIteratorVariable::Traverse(int moves, UINT attributes, ReturnObject& ret)
{
    if (collection.Get())
    {
        TrecPointer<TVariable> value;
        int curIndex = index;
        index += moves;
        bool done = true;
        if (index >= collection->GetSize())
            index = collection->GetSize();
        else if (index < 0)
            index = -1;
        else
        {
            value = collection->GetValueAt(static_cast<UINT>(curIndex));
            done = false;
        }

        if (attributes)
        {
            TrecSubPointer<TVariable, TContainerVariable> sValue = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);
            if (attributes & 0x0001)
                sValue->SetValue(L"value", value);
            if (attributes & 0x0002)
                sValue->SetValue(L"index", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(index));
            if (attributes & 0x0004)
                sValue->SetValue(L"done", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(done));
            return TrecPointerKey::GetTrecPointerFromSub<>(sValue);
        }
        return value;
    }
    else if(function.Get())
    {
        if (!fDone)
        {
            ret = function->Run();
        }

        bool doDone = ret.mode != return_mode::rm_yield;
        ret.mode = return_mode::rm_regular;

        if (attributes)
        {
            TrecSubPointer<TVariable, TContainerVariable> sValue = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);
            if (attributes & 0x0001)
                sValue->SetValue(L"value", ret.errorObject);
            if (attributes & 0x0002)
                sValue->SetValue(L"index", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(index));
            if (attributes & 0x0004)
                sValue->SetValue(L"done", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(fDone));
            if (attributes & 0x000F)
                ret.returnCode = 0;
            fDone = doDone;
            return TrecPointerKey::GetTrecPointerFromSub<>(sValue);
        }
        return ret.errorObject;
    }
    return TrecPointer<TVariable>();
}
