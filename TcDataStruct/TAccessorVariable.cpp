#include "TAccessorVariable.h"

TrecPointer<TVariable> TAccessorVariable::Clone()
{
    TrecSubPointer<TVariable, TAccessorVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAccessorVariable>();
    ret->SetGetter(getter);
    ret->SetSetter(setter);
    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TAccessorVariable>(ret);
}

/**
 * Method: TAccessorVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 *
 * Attributes: override
 */
var_type TAccessorVariable::GetVarType()
{
    return var_type::accessor;
}

/**
 * Method: TAccessorVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
 *
 * Attributes: override
 */
TrecObjectPointer TAccessorVariable::GetObject()
{
    return TrecObjectPointer();
}

/**
 * Method: TAccessorVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
 *
 * Attributes: override
 */
TString TAccessorVariable::GetString()
{
    return TString();
}

/**
 * Method: TAccessorVariable::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
 *
 * Attributes: override
 */
UINT TAccessorVariable::Get4Value()
{
    return 0;
}

/**
 * Method: TAccessorVariable::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 *
 * Attributes: override
 */
ULONG64 TAccessorVariable::Get8Value()
{
    return ULONG64();
}

/**
 * Method: TAccessorVariable::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 *
 * Attributes: override
 */
UINT TAccessorVariable::GetSize()
{
    return 0;
}

/**
 * Method: TAccessorVariable::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 *
 * Attributes: override
 */
UINT TAccessorVariable::GetType()
{
    return 0;
}


/**
 * Method: TAccessorVariable::TAccessorVariable
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Accessor Object
 */
TAccessorVariable::TAccessorVariable()
{

}

/**
 * Method: TAccessorVariable::GetGetter
 * Purpose: Retireve the Getter held by this variable
 * Parameters: void
 * Returns: TrecSubPointer<TVariable, TInterpretor> - the interpretor representing a getter
 */
TrecSubPointer<TVariable, TInterpretor> TAccessorVariable::GetGetter()
{
    return getter;
}

/**
 * Method: TAccessorVariable::GetCGetter
 * Purpose: Retireve the Getter held by this variable
 * Parameters: void
 * Returns: TrecSubPointer<TVariable, TcInterpretor> - the interpretor representing a getter
 */
TrecSubPointer<TVariable, TcInterpretor> TAccessorVariable::GetCGetter()
{
    return cGetter;
}

/**
 * Method: TAccessorVariable::SetGetter
 * Purpose: Retireve the Getter held by this variable
 * Parameters: TrecSubPointer<TVariable, TInterpretor> g - the interpretor representing a getter
 * Returns: void
 */
void TAccessorVariable::SetGetter(TrecSubPointer<TVariable, TInterpretor> g)
{
    getter = g;
}

/**
 * Method: TAccessorVariable::SetGetter
 * Purpose: Retireve the Getter held by this variable
 * Parameters: TrecSubPointer<TVariable, TcInterpretor> g - the interpretor representing a getter
 * Returns: void
 */
void TAccessorVariable::SetGetter(TrecSubPointer<TVariable, TcInterpretor> g)
{
    cGetter = g;
}

/**
 * Method: TAccessorVariable::GetSetter
 * Purpose: Retireve the Setter held by this variable
 * Parameters: void
 * Returns: TrecSubPointer<TVariable, TInterpretor> - the interpretor representing a setter
 */
TrecSubPointer<TVariable, TInterpretor> TAccessorVariable::GetSetter()
{
    return setter;
}


/**
 * Method: TAccessorVariable::GetCSetter
 * Purpose: Retireve the Setter held by this variable
 * Parameters: void
 * Returns: TrecSubPointer<TVariable, TcInterpretor> - the interpretor representing a setter
 */
TrecSubPointer<TVariable, TcInterpretor> TAccessorVariable::GetCSetter()
{
    return cSetter;
}

/**
 * Method: TAccessorVariable::SetSetter
 * Purpose: Retireve the Getter held by this variable
 * Parameters: TrecSubPointer<TVariable, TInterpretor> s - the interpretor representing a setter
 * Returns: void
 */
void TAccessorVariable::SetSetter(TrecSubPointer<TVariable, TInterpretor> s)
{
    setter = s;
}

/**
 * Method: TAccessorVariable::SetSetter
 * Purpose: Retireve the Getter held by this variable
 * Parameters: TrecSubPointer<TVariable, TcInterpretor> s - the interpretor representing a setter
 * Returns: void
 */
void TAccessorVariable::SetSetter(TrecSubPointer<TVariable, TcInterpretor> s)
{
    cSetter = s;
}