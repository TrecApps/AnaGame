#include "pch.h"
#include "TObjectVariable.h"




/**
 * Method: TObjectVariable::TObjectVariable
 * Purpose: Constructor
 * Parameters: TrecPointer<TObject> obj - the object to hold
 * Returns: New TObject Variable
 */
TObjectVariable::TObjectVariable(TrecObjectPointer obj)
{
	Set(obj);
}
/**
 * Method: TObjectVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 */
var_type TObjectVariable::GetVarType()
{
	return var_type::native_object;
}

/**
 * Method: TObjectVariable::Set
 * Purpose: Sets up the object
 * Parameters: TrecObjectPointer obj - the object to hold
 * Returns: void
 */
void TObjectVariable::Set(TrecObjectPointer obj)
{
	object = obj;
}


/**
 * Method: TObjectVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TrecObjectPointer TObjectVariable::GetObject()
{
	return object;
}


/**
 * Method: TObjectVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TString TObjectVariable::GetString()
{
	if(!object.Get())
		return TString(L"null");
	return object->toString();
}

/**
 * Method: TObjectVariable::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type
 */
UINT TObjectVariable::Get4Value()
{
	return 0;
}

/**
 * Method: TObjectVariable::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 */
ULONG64 TObjectVariable::Get8Value()
{
	return ULONG64();
}

/**
 * Method: TObjectVariable::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 */
UINT TObjectVariable::GetSize()
{
	return 0;
}

/**
 * Method: TObjectVariable::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TObjectVariable::GetType()
{
	return 0;
}
