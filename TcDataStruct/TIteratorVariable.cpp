#include "TIteratorVariable.h"

/**
 * Method: TIteratorVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 *
 * Attributes: abstract
 */
var_type TIteratorVariable::GetVarType()
{
    return var_type::iterator;
}

/**
 * Method: TIteratorVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
 *
 * Attributes: abstract
 */
TrecObjectPointer TIteratorVariable::GetObject()
{
    return TrecObjectPointer();
}

/**
 * Method: TIteratorVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
 *
 * Attributes: abstract
 */
TString TIteratorVariable::GetString()
{
    return TString();
}

/**
 * Method: TIteratorVariable::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
 *
 * Attributes: abstract
 */
UINT TIteratorVariable::Get4Value()
{
    return 0;
}

/**
 * Method: TIteratorVariable::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 *
 * Attributes: abstract
 */
ULONG64 TIteratorVariable::Get8Value()
{
    return ULONG64();
}

/**
 * Method: TIteratorVariable::GetSize
 * Purpose: Returns the estimated size of the value held, for iterators, this would be the number of elements in their collection (or 1, if a function)
 * Parameters: void
 * Returns: UINT - The estimated size in the collection (or 1 for function)
 *
 * Attributes: abstract
 */
UINT TIteratorVariable::GetSize()
{
    if (collection.Get())
        return collection->GetSize();
    return 1;
}

/**
 * Method: TIteratorVariable::GetVType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 *
 * Attributes: abstract
 */
UINT TIteratorVariable::GetVType()
{
    return 0;
}

/**
 * Method: TIteratorVariable::TIteratorVariable
 * Purpose: Constructor
 * Parameters: TrecSubPointer<TVariable, TContainerVariable> coll - the colection to traverse
 * Returns: New Iterator
 */
TIteratorVariable::TIteratorVariable(TrecSubPointer<TVariable, TContainerVariable> coll)
{
    if (!coll.Get())
        throw L"Null Parameter Detected!";
    collection = coll;
    index = 0;
    fDone = false;
}

/**
 * Method: TIteratorVariable::TIteratorVariable
 * Purpose: Constructor
 * Parameters: TrecSubPointer<TVariable, TContainerVariable> coll - the colection to traverse
 * Returns: New Iterator
 */
TIteratorVariable::TIteratorVariable(TrecSubPointer<TVariable, TcInterpretor> coll)
{
    if (!coll.Get())
        throw L"Null Parameter Detected!";
    function = coll;
    index = 0;
    fDone = false;
}

/**
 * Method: TIteratorVariable::Traverse
 * Purpose: Moves the Iterator, and returns the resulting value
 * Parameters: int moves - how much to traverse by (typically 1 for next and -1 for prev)
 *              UINT attributes - markers for the attributes to retrieve
 * Returns: TrecPointer<TVariable> - the value at the new Location
 *
 * Note: If the iterator is attached to a function (i.e. a TcInterpretor, then the "moves" parameter is ignored)
 *
 * Note: if attributes is 0, then the return value will just be the actial value in the collection. Otherwise, a small collection is
 *  returned, based off of the bits active.
 * 0x0001 - the actual "value" is included
 * 0x0002 - the "index" is included
 * oxooo4 - a "done" attribute is included
 * 0x0008 - [Reserved]
 *
 * 0x000F - if running a function, whether to catch any excpetion (if set, then yes! Otherwise, Exception Intormation will be passed to the caller)
 *
 */
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
