#include "TSpecialVariable.h"

TrecPointer<TVariable> spec_var_un, spec_var_nan;

/**
 * Method: TSpecialVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 *
 * Attributes: override
 */
var_type TSpecialVariable::GetVarType()
{
    return var_type::special_value;
}

/**
 * Method: TSpecialVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
 *
 * Attributes: override
 */
TrecObjectPointer TSpecialVariable::GetObject()
{
    return TrecObjectPointer();
}

/**
 * Method: TSpecialVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
 *
 * Attributes: override
 */
TString TSpecialVariable::GetString()
{
    TString ret;
    switch (specialVar)
    {
    case SpecialVar::sp_nan:
        ret.Set(L"NaN");
        break;
    case SpecialVar::sp_undefined:
        ret.Set(L"undefined");
    }
    return ret;
}

/**
 * Method: TSpecialVariable::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
 *
 * Attributes: override
 */
UINT TSpecialVariable::Get4Value()
{
    return 0;
}

/**
 * Method: TSpecialVariable::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 *
 * Attributes: override
 */
ULONG64 TSpecialVariable::Get8Value()
{
    return ULONG64();
}

/**
 * Method: TSpecialVariable::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 *
 * Attributes: override
 */
UINT TSpecialVariable::GetSize()
{
    return 0;
}

/**
 * Method: TSpecialVariable::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 *
 * Attributes: override
 */
UINT TSpecialVariable::GetVType()
{
    ThreadLock();
    UINT ret = static_cast<UINT>(specialVar);
    ThreadRelease();
    return ret;
}

/**
 * Method: TSpecialVariable::GetSpecialVariable
 * Purpose: Retrieves a special variable with the provided value type
 * Parameters: SpecialVar - the Variable present
 * Returns: TrecPointer<TVariable> - the variable representing the specified special variable
 *
 * Attributes: static
 */
TrecPointer<TVariable> TSpecialVariable::GetSpecialVariable(SpecialVar sv)
{
    switch (sv)
    {
    case SpecialVar::sp_nan:
        if (!spec_var_nan.Get())
            spec_var_nan = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TSpecialVariable>(sv);
        return spec_var_nan;
    case SpecialVar::sp_undefined:
        if (!spec_var_un.Get())
            spec_var_un = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TSpecialVariable>(sv);
        return spec_var_un;
    default:
        return TrecPointer<TVariable>();
    }
    
}

/**
 * Method: TSpecialVariable::GetSpecialValue
 * Purpose: Retrieves the acual value held by this variable
 * Parameters: void
 * Returns: SpecialVar - the value held
 */
SpecialVar TSpecialVariable::GetSpecialValue()
{
    return specialVar;
}

/**
 * Method: TSpecialVariable::TSpecialVariable
 * Purpose: Constructor
 * Parameters: SpecialVar - the variable to represent
 * Returns: New Pecial Variable Object
 */
TSpecialVariable::TSpecialVariable(SpecialVar sp)
{
    specialVar = sp;
}
