#include "pch.h"
#include "TInterpretor.h"

/**
 * Method: ReportObject::ReportObject
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Report Object
 */
ReportObject::ReportObject()
{
	returnCode = 0;
}

/**
 * Method: ReportObject::ReportObject
 * Purpose: Copy Constructor
 * Parameters: ReportObject& obj - the object to copy
 * Returns: Copied Report Object
 */
ReportObject::ReportObject(const ReportObject& obj)
{
	returnCode = obj.returnCode;
	errorMessage.Set(obj.errorMessage);
	stackTrace = obj.stackTrace;
	errorObject = obj.errorObject;
}

/**
 * Method: TInterpretor::GetVariable
 * Purpose: Retrieves the variable specified
 * Parameters: TString& varName - the name of the variable requested
 * Returns: TrecPointer<TVariable> - the variable requested (null if not found)
 */
TrecPointer<TVariable> TInterpretor::GetVariable(TString& varName)
{
	for (int C = variables.count() - 1; C >= 0; C--)
	{
		if (variables.GetEntryAt(C).Get() && !variables.GetEntryAt(C)->key.Compare(varName))
			return variables.GetEntryAt(C)->object;
	}
	if (parent.Get())
		return parent->GetVariable(varName);
	return TrecPointer<TVariable>();
}

/**
 * Method: TInterretor::SetParamNames
 * Purpose: Sets the names of parameters
 * Parameters: TDataArray<TString>& names - list of names for the parameters
 * Returns: void
 */
void TInterpretor::SetParamNames(TDataArray<TString>& names)
{
	this->paramNames.RemoveAll();
	for (UINT Rust = 0; Rust < names.Size(); Rust++)
	{
		paramNames.push_back(names[Rust]);
	}
}

/**
 * Method: TInterpretor::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecPointer<TObject> - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
 */
TrecPointer<TObject> TInterpretor::GetObject()
{
	return TrecPointer<TObject>();
}

/**
 * Method: TInterpretor::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
 */
TString TInterpretor::GetString()
{
	return TString();
}

/**
 * Method: TInterpretor::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
 */
UINT TInterpretor::Get4Value()
{
	return 0;
}

/**
 * Method: TInterpretor::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 */
ULONG64 TInterpretor::Get8Value()
{
	return ULONG64();
}

/**
 * Method: TInterpretor::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 */
UINT TInterpretor::GetSize()
{
	return 0;
}

/**
 * Method: TInterpretor::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TInterpretor::GetType()
{
	return 0;
}

void TInterpretor::SetSelf(TrecPointer<TVariable> self)
{
	if (this != self.Get())
		throw L"Error! Not Properly called";
	this->self = TrecPointerKey::GetSoftSubPointerFromSoft<TVariable, TInterpretor>(TrecPointerKey::GetSoftPointerFromTrec<TVariable>(self));
}

/**
 * Method: TInterpretor::TInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TInterpretor Object
 */
TInterpretor::TInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor)
{
	parent = parentInterpretor;
}

/**
 * Method: TInterpretor::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 */
var_type TInterpretor::GetVarType()
{
	return var_type::interpretor;
}

/**
 * Method: TInterpretor::SetCode
 * Purpose: Sets the initial file of the code
 * Parameters: TFile& the file to read
 * Returns: UINT - error code
 *
 * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
 */
UINT TInterpretor::SetCode(TrecPointer<TFile> file, ULONG64 start, ULONG64 end)
{
	if (!file.Get())
		return 1;
	if (!file->IsOpen())
		return 2;
	if (file->GetLength() < end)
		return 3;
	if (start >= end)
		return 4;

	this->file = file;
	this->start = start;
	this->end = end;

	return 0;
}
