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
 * Method: TInterpretor::TInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TInterpretor Object
 */
TInterpretor::TInterpretor(TrecPointer<TInterpretor> parentInterpretor)
{
	parent = parentInterpretor;
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
