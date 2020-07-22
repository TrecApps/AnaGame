#include "pch.h"
#include "TStringVariable.h"


/**
 * Method: TStringVariable::TStringVariable
 * Purpose: Constructor
 * Parameters: TString& string - the String tp hold
 * Returns New String Variable
 */
TStringVariable::TStringVariable(TString& string)
{
    this->string.Set(string);
}


/**
 * Method: TStringVariable::IsObject
 * Purpose: Reports whether the variable holds an object or not
 * Parameters: void
 * Returns: bool - whether the variable is an object or not
 */
bool TStringVariable::IsObject()
{
    return false;
}

/**
 * Method: TStringVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecPointer<TObject> - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TrecPointer<TObject> TStringVariable::GetObject()
{
    return TrecPointer<TObject>();
}

/**
 * Method: TStringVariable::IsString
 * Purpose: Reports whether the variable holds a string or not
 * Parameters: void
 * Returns: bool - whether the variable is a string or not
 */
bool TStringVariable::IsString()
{
    return true;
}

/**
 * Method: TStringVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TString TStringVariable::GetString()
{
    return string;
}

/**
 * Method: TStringVariable::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type
 */
UINT TStringVariable::Get4Value()
{
    return 0;
}

/**
 * Method: TStringVariable::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 */
ULONG64 TStringVariable::Get8Value()
{
    return ULONG64();
}

/**
 * Method: TStringVariable::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 */
UINT TStringVariable::GetSize()
{
    return string.GetSize();
}

/**
 * Method: TStringVariable::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TStringVariable::GetType()
{
    return 0;
}
