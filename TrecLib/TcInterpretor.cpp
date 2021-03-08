#include "TcInterpretor.h"

///
/// TcVariableHolder
/// 

/**
 * Mehtod: TcVariableHolder::TcVariableHolder
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: new Variable Holder
 */
TcVariableHolder::TcVariableHolder()
{
	mut = true;
}

/**
 * Mehtod: TcVariableHolder::TcVariableHolder
 * Purpose: Copy Constructor
 * Parameters: const TcVariableHolder& copy - Variable Holder to copy
 * Returns: new Variable Holder
 */
TcVariableHolder::TcVariableHolder(const TcVariableHolder& copy)
{
	mut = copy.mut;
	type.Set(copy.type);
	value = copy.value;
}

///
/// TcInterpretor 
///


	/**
	 * Method: TcInterpretor::GetVarType
	 * Purpose: Reports the type of varible that this object represents
	 * Parameters: void
	 * Returns: var_type - the type of variable this represents
	 *
	 * Attributes: override
	 */
var_type TcInterpretor::GetVarType()
{
	return var_type::interpretor;
}

/**
 * Method: TcInterpretor::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
 *
 * Attributes: override
 */
TrecObjectPointer TcInterpretor::GetObject()
{
	return TrecObjectPointer();
}

/**
 * Method: TcInterpretor::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
 *
 * Attributes: override
 */
TString TcInterpretor::GetString()
{
	return TString();
}

/**
 * Method: TcInterpretor::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
 *
 * Attributes: override
 */
UINT TcInterpretor::Get4Value()
{
	return 0;
}

/**
 * Method: TcInterpretor::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 *
 * Attributes: override
 */
ULONG64 TcInterpretor::Get8Value()
{
	return ULONG64();
}

/**
 * Method: TcInterpretor::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 *
 * Attributes: override
 */
UINT TcInterpretor::GetSize()
{
	return 0;
}

/**
 * Method: TcInterpretor::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 *
 * Attributes: override
 */
UINT TcInterpretor::GetType()
{
	return 0;
}

/**
 * Method: TInterpretor::TInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TInterpretor Object
 */
TcInterpretor::TcInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env)
{
	auto p = TrecPointerKey::GetTrecPointerFromSub<>(parentInterpretor);
	parent = TrecPointerKey::GetSoftPointerFromTrec<TVariable>(p);
	environment = env;
}

/**
 * Method: TcInterpretor::SetSelf
 * Purpose: Allows the Variable to have a reference to itself
 * Parameters: TrecPointer<TVariable> - reference to assign
 * Returns: void
 *
 * Attributes: override
 */
void TcInterpretor::SetSelf(TrecPointer<TVariable> self)
{
	if (this != self.Get())
		throw L"Error! Not Properly called";
	this->self = TrecPointerKey::GetSoftSubPointerFromSoft<TVariable, TcInterpretor>(TrecPointerKey::GetSoftPointerFromTrec<TVariable>(self));
}

/**
 * Method: TcInterpretor::CheckVarName
 * Purpose: Method Interpreters can use to inspect the variable name to make sure it is valid
 * Parameters: TString& varname - the variable name to check
 *              ReportObject& ro - the Object to modify based off of the findings
 * Returns: void
 */
void TcInterpretor::CheckVarName(TString& varname, ReturnObject& ro)
{
	int badChar = varname.FindOneOf(L"!@#$%^&*(){}[]-=+/,;:'\"\\`~");
	if (badChar != -1)
	{
		ro.returnCode = ReturnObject::ERR_IMPROPER_NAME;
		return;
	}

	WCHAR firstChar = varname[0];

	if (!((firstChar >= L'a' && firstChar <= 'z') || (firstChar >= L'A' && firstChar <= 'Z') || firstChar == L'_'))
	{
		ro.returnCode = ReturnObject::ERR_IMPROPER_NAME;
		return;
	}

	ro.returnCode = 0;
}

TrecPointer<TVariable> TcInterpretor::GetVariable(TString& varName, bool& present, bool currentScope)
{
	TcVariableHolder marker;
	if (variables.retrieveEntry(varName, marker))
	{
		present = true;
		return marker.value;
	}

	if (!currentScope)
	{
		if (parent.Get())
			return dynamic_cast<TcInterpretor*>(parent.Get())->GetVariable(varName, present,false);
		if (environment.Get())
			return environment->GetVariable(varName, present);
	}
	present = false;
	return TrecPointer<TVariable>();
}


/**
 * Method: TCInterpretor::SetParamNames
 * Purpose: Allows Parameter names to be set
 * Parameters: TDataArray<TString>& paramNames - the Names of Initial parameters
 * Returns: void
 */
void TcInterpretor::SetParamNames(TDataArray<TString>& paramNames)
{
	this->paramNames.RemoveAll();
	for (UINT Rust = 0; Rust < paramNames.Size(); Rust++)
	{
		this->paramNames.push_back(paramNames[Rust]);
	}
}

/**
 * Method: TCInterpretor::SetParamTypes
 * Purpose: Allows Parameter types to be set
 * Parameters: TDataArray<TString>& paramTypes - the Types of Initial parameters
 * Returns: void
 */
void TcInterpretor::SetParamTypes(TDataArray<TString>& paramTypes)
{
	this->paramTypes.RemoveAll();
	for (UINT Rust = 0; Rust < paramTypes.Size(); Rust++)
	{
		this->paramTypes.push_back(paramTypes[Rust]);
	}
}

/**
 * Method: TCInterpretor::SetDefaultValues
 * Purpose: Allows Parameter Defaults to be set
 * Parameters: TDataArray<TrecPointer<TVariable>>& paramTypes - the Types of Initial parameters
 * Returns: void
 */
void TcInterpretor::SetParamDefaults(TDataArray<TrecPointer<TVariable>>& paramDefaults)
{
	this->defaultVars.RemoveAll();
	for (UINT Rust = 0; Rust < paramDefaults.Size(); Rust++)
	{
		this->defaultVars.push_back(paramDefaults[Rust]);
	}
}

/**
 * Method: TcInterpretor::SetActiveObject
 * Purpose: Sets the Active Object, enabling this Interpreotr to act as a method in an existing object
 * Parameters: TrecPointer<TVariable> var - the object to act on
 * Returns: void
 */
void TcInterpretor::SetActiveObject(TrecPointer<TVariable> var)
{
	this->methodObject = var;
}


///
/// ReturnObject
/// 

/**
 * Mehtod: ReturnObject::ReturnObject
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Return Object
 */
ReturnObject::ReturnObject()
{
	returnCode = 0;
	nextCount = 0;
	mode = return_mode::rm_regular;
}

/**
 * Mehtod: ReturnObject::ReturnObject
 * Purpose: Copy Constructor
 * Parameters: const ReutnrObject& copy - the object to copy
 * Returns: new Return Object
 */
ReturnObject::ReturnObject(const ReturnObject& copy)
{
	errorMessage.Set(copy.errorMessage);
	errorObject = copy.errorObject;
	returnCode = copy.returnCode;
	mode = copy.mode;

	stackTrace = copy.stackTrace;

	nextCount = copy.nextCount;
}

