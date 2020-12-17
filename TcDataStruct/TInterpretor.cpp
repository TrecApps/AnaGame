
#include "TInterpretor.h"
#include "TPrimitiveVariable.h"
#include "TStringVariable.h"

#include <cmath>

/**
 * Method: ReportObject::ReportObject
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Report Object
 */
ReportObject::ReportObject()
{
	returnCode = 0;
	mode = report_mode::report_mode_regular;
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
	mode = obj.mode;
}

/**
 * Method: TInterpretor::GetVariable
 * Purpose: Retrieves the variable specified
 * Parameters: TString& varName - the name of the variable requested
 *				bool& present - whether the variable was present or not (used to distinguish between 'null' and 'undefined')
 * Returns: TrecPointer<TVariable> - the variable requested (null if not found)
 */
TrecPointer<TVariable> TInterpretor::GetVariable(TString& varName, bool& present)
{
	TVariableMarker marker;
	if (variables.retrieveEntry(varName, marker))
	{
		present = true;
		return marker.GetVariable();
	}

	if (parent.Get())
		return parent->GetVariable(varName, present);
	if (environment.Get())
		return environment->GetVariable(varName, present);
	present = false;
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
 * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
 */
TrecObjectPointer TInterpretor::GetObject()
{
	return TrecObjectPointer();
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
 * Method: TVariable::CheckVarName
 * Purpose: Method Interpreters can use to inspect the variable name to make sure it is valid
 * Parameters: TString& varname - the variable name to check
 *              ReportObject& ro - the Object to modify based off of the findings
 * Returns: void
 */
void TInterpretor::CheckVarName(TString& varname, ReportObject& ro, UINT line)
{
	int badChar = varname.FindOneOf(L"!@#$%^&*(){}[]-=+/,;:'\"\\`~");
	if (badChar != -1)
	{
		ro.returnCode = ReportObject::invalid_name;
		TString chara(varname.SubString(badChar, badChar + 1));
		ro.errorMessage.Format(L"Error! Found invalid character '%ws' in variable name!", chara.GetConstantBuffer());

		TString stack;
		stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
		ro.stackTrace.push_back(stack);


		return;
	}

	WCHAR firstChar = varname[0];

	if (!((firstChar >= L'a' && firstChar <= 'z') || (firstChar >= L'A' && firstChar <= 'Z') || firstChar == L'_'))
	{
		ro.returnCode = ReportObject::invalid_name;
		TString chara(firstChar);
		ro.errorMessage.Format(L"Error! Found invalid character '%ws' as First character in Variable name. Need to be 'a-z', 'A-Z', or '_'!", chara.GetConstantBuffer());

		TString stack;
		stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
		ro.stackTrace.push_back(stack);


		return;
	}

	ro.returnCode = 0;
}

bool TInterpretor::SubmitClassType(const TString& className, TClassStruct& classStruct)
{
	TClassStruct s;
	if(classes.retrieveEntry(className,s))
		return false;

	classes.addEntry(className, classStruct);
	return true;
}

void TInterpretor::SetFirstParamName(const TString& iParam)
{
	if (!paramNames.Size())
	{
		paramNames.push_back(iParam);
		return;
	}

	if (!iParam.Compare(paramNames[0]))
		return;

	paramNames.push_back(paramNames[paramNames.Size() - 1]);

	for (UINT Rust = paramNames.Size() - 2; Rust < paramNames.Size(); Rust--)
	{
		paramNames[Rust + 1].Set(paramNames[Rust]);
	}

	paramNames[0].Set(iParam);
}

void TInterpretor::CorrectSplitStringForParenthesis(TrecPointer<TDataArray<TString>> splitString, WCHAR join)
{
	for (UINT Rust = 1; Rust < splitString->Size(); Rust++)
	{
		TString first(splitString->at(Rust - 1));
		TString second(splitString->at(Rust));

		if (first.CountFinds(L'(') > first.CountFinds(L')'))
		{
			first.AppendChar(join);
			first.Append(second);

			splitString->at(Rust - 1).Set(first);
			splitString->RemoveAt(Rust--);
			continue;
		}
		if (first.CountFinds(L'[') > first.CountFinds(L']'))
		{
			first.AppendChar(join);
			first.Append(second);

			splitString->at(Rust - 1).Set(first);
			splitString->RemoveAt(Rust--);
		}
	}
}

TrecPointer<TVariable> TInterpretor::Clone()
{
	return TrecPointerKey::GetTrecPointerFromSub<TVariable,TInterpretor>(TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self));
}

/**
 * Method: TInterpretor::UpdateVariable
 * Purpose: Updates an existing Variable
 * Parameters: const TString& name - the name to update
 *              TrecPointer<TVariable> value - value to update it with
 *              bool addLocally - If true, then tf the variable is not found, go ahead and add it to 'this' interpretor (false by default)
 *              bool makeConst - whether the variable added should be const or not (ignored if 'addLocally' is false) (false by Default)
 * Returns: UINT - error code (0 for no error, 1 for doesn't exist, 2 for value is immutable, 3 for no name Provided)
 */
UINT TInterpretor::UpdateVariable(const TString& name, TrecPointer<TVariable> value, bool addLocally, bool makeConst)
{
	if (!name.GetSize())
		return 3;


	for (UINT Rust = 0; Rust < variables.count(); Rust++)
	{
		TDataEntry<TVariableMarker> varMarker;
		if (variables.GetEntryAt(Rust, varMarker))
		{
			if (!varMarker.key.Compare(name))
			{
				if (varMarker.object.IsMutable())
				{
					varMarker.object.SetVariable(value);
					variables.setEntry(name, varMarker.object);
					return 0;
				}
				else return 2;
			}
		}
	}
	UINT res = parent.Get() ? parent->UpdateVariable(name, value) : 1;
	if (res == 1 && addLocally)
	{
		variables.addEntry(name, TVariableMarker(!makeConst, value));
		return 0;
	}
	return res;
}


/**
 * Method: TInterpretor::TInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TInterpretor Object
 */
TInterpretor::TInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env)
{
	parent = parentInterpretor;
	environment = env;
	this->end = 0;
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

/**
 * Method: TInterpretor::CloseFile
 * Purpose: Closes the file so a second run can be possible
 * Parameters: void
 * Returns: void
 *
 * Note: This should only be called on the top-level interpretor
 */
void TInterpretor::CloseFile()
{
	if (file.Get() && file->IsOpen())
		file->Close();
}


/**
 * Method: TInterpretor::ProcessAddition
 * Purpose: Filters variables in preparation for performing an addition operation
 * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the first addend
 *              TrecPointer<TVariable> var2 - the second variable, representing the second addend
 * Returns: ReportObject - the result of performing the operation, including error information if it occurs
 */
ReportObject TInterpretor::ProcessAddition(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
	if (!var1.Get() || !var2.Get())
	{
		ReportObject ro;
		ro.returnCode = ro.broken_reference;
		ro.errorMessage.Set(L"Addition Requires variables that are not null or undefined!");
		return ro;
	}

	if (var1->GetVarType() == var_type::primitive && var2->GetVarType() == var_type::primitive)
	{
		auto retValue = Add(GetValueFromPrimitive(var1), GetValueFromPrimitive(var2));

		ReportObject ro;
		ro.returnCode = 0;


		switch (retValue.type)
		{
		case double_long::dl_double:
			ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.d);
			break;
		case double_long::dl_unsign:
			ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.u);
			break;
		case double_long::dl_sign:
			ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.s);
		}
		return ro;
	}

	if (var1->GetVarType() == var_type::primitive &&
		(var2->GetVarType() == var_type::string || var2->GetVarType() == var_type::native_object))
	{
		TString strValue(GetStringFromPrimitive(var1) + var2->GetString());
		ReportObject ro;
		ro.returnCode = 0;
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(strValue);
		return ro;
	}

	if ((var1->GetVarType() == var_type::string || var1->GetVarType() == var_type::native_object) &&
		(var2->GetVarType() == var_type::string || var2->GetVarType() == var_type::native_object))
	{

		TString strValue(var1->GetString() + var2->GetString());
		ReportObject ro;
		ro.returnCode = 0;
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(strValue);
		return ro;
	}

	if ((var1->GetVarType() == var_type::string || var1->GetVarType() == var_type::native_object)
		&& var2->GetVarType() == var_type::primitive)
	{
		TString strValue(var1->GetString() + GetStringFromPrimitive(var2));
		ReportObject ro;
		ro.returnCode = 0;
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(strValue);
		return ro;
	}

	ReportObject ro;
	ro.returnCode = ReportObject::improper_type;
	ro.errorMessage.Set(L"Addition operator does not support the types provided!");
	return ro;
}

/**
 * Method: TInterpretor::ProcessSubtraction
 * Purpose: Filters variables in preparation for performing a subtraction operation
 * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the minuend
 *              TrecPointer<TVariable> var2 - the second variable, representing the subtrahend
 * Returns: ReportObject - the result of performing the operation, including error information if it occurs
 */
ReportObject TInterpretor::ProcessSubtraction(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
	if (!var1.Get() || !var2.Get())
	{
		ReportObject ro;
		ro.returnCode = ro.broken_reference;
		ro.errorMessage.Set(L"Subtraction Requires variables that are not null or undefined!");
		return ro;
	}

	if (var1->GetVarType() != var_type::primitive || var2->GetVarType() != var_type::primitive)
	{
		ReportObject ro;
		ro.returnCode = ro.improper_type;
		ro.errorMessage.Set(L"Subtraction Requires variables to be primitive types!");
		return ro;
	}

	ReportObject ro;

	auto retValue = Subtract(GetValueFromPrimitive(var1), GetValueFromPrimitive(var2));
	ro.returnCode = 0;


	switch (retValue.type)
	{
	case double_long::dl_double:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.d);
		break;
	case double_long::dl_unsign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.u);
		break;
	case double_long::dl_sign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.s);
	}
	return ro;
}

/**
 * Method: TInterpretor::ProcessMultiplication
 * Purpose: Filters variables in preparation for performing a multiplication operation
 * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the first factor
 *              TrecPointer<TVariable> var2 - the second variable, representing the second factor
 * Returns: ReportObject - the result of performing the operation, including error information if it occurs
 */
ReportObject TInterpretor::ProcessMultiplication(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
	if (!var1.Get() || !var2.Get())
	{
		ReportObject ro;
		ro.returnCode = ro.broken_reference;
		ro.errorMessage.Set(L"Multiplication Requires variables that are not null or undefined!");
		return ro;
	}

	if (var1->GetVarType() != var_type::primitive || var2->GetVarType() != var_type::primitive)
	{
		ReportObject ro;
		ro.returnCode = ro.improper_type;
		ro.errorMessage.Set(L"Multiplication Requires variables to be primitive types!");
		return ro;
	}

	ReportObject ro;

	auto retValue = Multiply(GetValueFromPrimitive(var1), GetValueFromPrimitive(var2));
	ro.returnCode = 0;


	switch (retValue.type)
	{
	case double_long::dl_double:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.d);
		break;
	case double_long::dl_unsign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.u);
		break;
	case double_long::dl_sign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.s);
	}
	return ro;
}

/**
 * Method: TInterpretor::ProcessDivision
 * Purpose: Filters variables in preparation for performing a division operation
 * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the dividend
 *              TrecPointer<TVariable> var2 - the second variable, representing the divisor
 * Returns: ReportObject - the result of performing the operation, including error information if it occurs
 */
ReportObject TInterpretor::ProcessDivision(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
	if (!var1.Get() || !var2.Get())
	{
		ReportObject ro;
		ro.returnCode = ro.broken_reference;
		ro.errorMessage.Set(L"Division Requires variables that are not null or undefined!");
		return ro;
	}

	if (var1->GetVarType() != var_type::primitive || var2->GetVarType() != var_type::primitive)
	{
		ReportObject ro;
		ro.returnCode = ro.improper_type;
		ro.errorMessage.Set(L"Division Requires variables to be primitive types!");
		return ro;
	}

	ReportObject ro;

	auto retValue = Divide(GetValueFromPrimitive(var1), GetValueFromPrimitive(var2));
	ro.returnCode = 0;


	switch (retValue.type)
	{
	case double_long::dl_double:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.d);
		break;
	case double_long::dl_unsign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.u);
		break;
	case double_long::dl_sign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.s);
	}
	return ro;
}

/**
 * Method: TInterpretor::ProcessModDivision
 * Purpose: Filters variables in preparation for performing a mod division operation
 * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the dividend
 *              TrecPointer<TVariable> var2 - the second variable, representing the divisor
 * Returns: ReportObject - the result of performing the operation, including error information if it occurs
 */
ReportObject TInterpretor::ProcessModDivision(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
	if (!var1.Get() || !var2.Get())
	{
		ReportObject ro;
		ro.returnCode = ro.broken_reference;
		ro.errorMessage.Set(L"Mod Division Requires variables that are not null or undefined!");
		return ro;
	}

	if (var1->GetVarType() != var_type::primitive || var2->GetVarType() != var_type::primitive)
	{
		ReportObject ro;
		ro.returnCode = ro.improper_type;
		ro.errorMessage.Set(L"Mod Division Requires variables to be primitive types!");
		return ro;
	}

	ReportObject ro;

	auto retValue = ModDivide(GetValueFromPrimitive(var1), GetValueFromPrimitive(var2));
	ro.returnCode = 0;


	switch (retValue.type)
	{
	case double_long::dl_double:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.d);
		break;
	case double_long::dl_unsign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.u);
		break;
	case double_long::dl_sign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.s);
	}
	return ro;
}

/**
 * Method: TInterpretor::ProcessExponent
 * Purpose: Filters variables in preparation for performing an exponent operation
 * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the base
 *              TrecPointer<TVariable> var2 - the second variable, representing the power
 * Returns: ReportObject - the result of performing the operation, including error information if it occurs
 */
ReportObject TInterpretor::ProcessExponent(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
	if (!var1.Get() || !var2.Get())
	{
		ReportObject ro;
		ro.returnCode = ro.broken_reference;
		ro.errorMessage.Set(L"Exponent analysis Requires variables that are not null or undefined!");
		return ro;
	}

	if (var1->GetVarType() != var_type::primitive || var2->GetVarType() != var_type::primitive)
	{
		ReportObject ro;
		ro.returnCode = ro.improper_type;
		ro.errorMessage.Set(L"Exponent analysis Requires variables to be primitive types!");
		return ro;
	}

	ReportObject ro;

	auto retValue = Exponent(GetValueFromPrimitive(var1), GetValueFromPrimitive(var2));
	ro.returnCode = 0;


	switch (retValue.type)
	{
	case double_long::dl_double:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.d);
		break;
	case double_long::dl_unsign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.u);
		break;
	case double_long::dl_sign:
		ro.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.s);
	}
	return ro;
}



/**
 * Method: TInterpretor::Add
 * Purpose: Helper method for the ProcessAddition method, performing the actual operation
 * Parameters: const DoubleLong& v1 - the first addend for the operation
 *              const DoubleLong& v2 - the second addend for the operation
 * Returns: DoubleLong - the result of the Addition operation
 */
DoubleLong TInterpretor::Add(const DoubleLong& v1, const DoubleLong& v2)
{
	if (v1.type == double_long::dl_double)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( v1.value.d + v2.value.d );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong(static_cast<double>(v1.value.d + v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<double>(v1.value.d + v2.value.u) );
		}
	}
	else if (v1.type == double_long::dl_sign)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(v1.value.s + v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( (v1.value.s + v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<LONG64>(v1.value.s + v2.value.u));
		}
	}
	else
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(v1.value.u + v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<LONG64>(v1.value.u + v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<ULONG64>(v1.value.u + v2.value.u) );
		}
	}
}

/**
 * Method: TInterpretor::Subtract
 * Purpose: Helper method for the ProcessSubtraction method, performing the actual operation
 * Parameters: const DoubleLong& v1 - the minuend for the operation
 *              const DoubleLong& v2 - the subtrahend for the operation
 * Returns: DoubleLong - the result of the Subtraction operation
 */
DoubleLong TInterpretor::Subtract(const DoubleLong& v1, const DoubleLong& v2)
{
	if (v1.type == double_long::dl_double)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( v1.value.d - v2.value.d );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<double>(v1.value.d - v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<double>(v1.value.d - v2.value.u) );
		}
	}
	else if (v1.type == double_long::dl_sign)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(static_cast<double>(v1.value.s) - v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( (v1.value.s - v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<LONG64>(v1.value.s - v2.value.u));
		}
	}
	else
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(v1.value.u - v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<LONG64>(v1.value.u - v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<ULONG64>(v1.value.u - v2.value.u) );
		}
	}
}

/**
 * Method: TInterpretor::Multiply
 * Purpose: Helper method for the ProcessMultiplication method, performing the actual operation
 * Parameters: const DoubleLong& v1 - the first factor for the operation
 *              const DoubleLong& v2 - the second factor for the operation
 * Returns: DoubleLong - the result of the Multiply operation
 */
DoubleLong TInterpretor::Multiply(const DoubleLong& v1, const DoubleLong& v2)
{
	if (v1.type == double_long::dl_double)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( v1.value.d * v2.value.d );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<double>(v1.value.d * v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<double>(v1.value.d * v2.value.u) );
		}
	}
	else if (v1.type == double_long::dl_sign)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(v1.value.s * v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( (v1.value.s * v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<LONG64>(v1.value.s * v2.value.u));
		}
	}
	else
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(v1.value.u * v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<LONG64>(v1.value.u * v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<ULONG64>(v1.value.u * v2.value.u) );
		}
	}
}

/**
 * Method: TInterpretor::Divide
 * Purpose: Helper method for the ProcessDivision method, performing the actual operation
 * Parameters: const DoubleLong& v1 - the dividend for the operation
 *              const DoubleLong& v2 - the divisor for the operation
 * Returns: DoubleLong - the result of the Division operation
 */
DoubleLong TInterpretor::Divide(const DoubleLong& v1, const DoubleLong& v2)
{
	if (v1.type == double_long::dl_double)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( v1.value.d / v2.value.d );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<double>(v1.value.d / v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<double>(v1.value.d / v2.value.u) );
		}
	}
	else if (v1.type == double_long::dl_sign)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(v1.value.s / v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( (v1.value.s / v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<LONG64>(v1.value.s / v2.value.u));
		}
	}
	else
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( static_cast<double>(v1.value.u / v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<LONG64>(v1.value.u / v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<ULONG64>(v1.value.u / v2.value.u) );
		}
	}
}

/**
 * Method: TInterpretor::ModDivide
 * Purpose: Helper method for the ProcessModDivision method, performing the actual operation
 * Parameters: const DoubleLong& v1 - the dividend for the operation
 *              const DoubleLong& v2 - the divisor for the operation
 * Returns: DoubleLong - the result of the Mod Division operation
 */
DoubleLong TInterpretor::ModDivide(const DoubleLong& v1, const DoubleLong& v2)
{
	if (v1.type == double_long::dl_double)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( fmod( v1.value.d, v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( fmod(v1.value.d, v2.value.s) );
		}
		else
		{
			return DoubleLong( fmod(v1.value.d, v2.value.s) );
		}
	}
	else if (v1.type == double_long::dl_sign)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( fmod(v1.value.d, v2.value.s) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( (v1.value.s % v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<LONG64>(v1.value.s % v2.value.u));
		}
	}
	else
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( fmod(v1.value.d, v2.value.s) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( static_cast<LONG64>(v1.value.u % v2.value.s) );
		}
		else
		{
			return DoubleLong( static_cast<ULONG64>(v1.value.u % v2.value.u) );
		}
	}
}

/**
 * Method: TInterpretor::Exponent
 * Purpose: Helper method for the ProcessExponent method, performing the actual operation
 * Parameters: const DoubleLong& v1 - the base value for the operation
 *              const DoubleLong& v2 - the power value for the operation
 * Returns: DoubleLong - the result of the Exponent operation
 */
DoubleLong TInterpretor::Exponent(const DoubleLong& v1, const DoubleLong& v2)
{
	if (v1.type == double_long::dl_double)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( pow(v1.value.d, v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( pow(v1.value.d, v2.value.s) );
		}
		else
		{
			return DoubleLong( pow(v1.value.d, v2.value.u) );
		}
	}
	else if (v1.type == double_long::dl_sign)
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( pow(v1.value.s, v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( pow(v1.value.s, v2.value.s) );
		}
		else
		{
			return DoubleLong( pow(v1.value.s, v2.value.u));
		}
	}
	else
	{
		if (v2.type == double_long::dl_double)
		{
			return DoubleLong( pow(v1.value.u, v2.value.d) );
		}
		else if (v2.type == double_long::dl_sign)
		{
			return DoubleLong( pow(v1.value.u, v2.value.s) );
		}
		else
		{
			return DoubleLong( pow(v1.value.u, v2.value.u) );
		}
	}
}

/**
 * Method: TInterpretor::GetValueFromPrimitive
 * Purpose: Helper method for interpreting arithmetic operations by extracting the value from the variable
 * Parameters: TrecPointer<TVariable> var - the variable believed holding the primitive value
 * Returns: DoubleLong - the value held by the variable
 */
DoubleLong TInterpretor::GetValueFromPrimitive(TrecPointer<TVariable> var)
{
	if(!var.Get() || var->GetVarType() != var_type::primitive)
		return DoubleLong();

	double f_value = 0.0;
	ULONG64 u_value = 0ULL;
	LONG64 s_value = 0LL;

	switch (var->GetType())
	{
	case (TPrimitiveVariable::type_bool):
		return DoubleLong( 1ULL);

	case (0b00110010):						// Indicates a four-byte float
		u_value = var->Get4Value();
		memcpy_s(&f_value, sizeof(f_value), &u_value, sizeof(u_value));
		return DoubleLong( f_value);

	case (0b01000010):						// Indicates an eight-byte float
		u_value = var->Get8Value();
		memcpy_s(&f_value, sizeof(f_value), &u_value, sizeof(u_value));
		return DoubleLong( f_value);

	case (0b00010000):						// Indicates a 1-byte int
	case (0b00100000):						// Indicates a 2-byte int
	case (0b00110000):						// Indicates a 4-byte int
	case (0b01000000):						// Indicates an 8-byte int
		u_value = var->Get8Value();
		memcpy_s(&s_value, sizeof(s_value), &u_value, sizeof(u_value));
		return DoubleLong( s_value);

	case (0b00011000):						// Indicates a 1-byte uint
	case (0b00101000):						// Indicates a 2-byte uint
	case (0b00111000):						// Indicates a 4-byte uint
	case (0b01001000):						// Indicates an 8-byte uint
		return DoubleLong( static_cast<ULONG64>(u_value));


	}
	return DoubleLong();
}

/**
 * Method: TInterpretor::GetStringFromPrimitive
 * Purpose:
 * Parameters: TrecPointer<TVariable> var - the variable believed to be capable of producing some string value
 * Returns: TString - string representation of the primitive variable
 */
TString TInterpretor::GetStringFromPrimitive(TrecPointer<TVariable> var)
{
	if (!var.Get() || var->GetVarType() != var_type::primitive)
		return TString();
	TString strValue;
	auto v1Type = var->GetType();

	if (v1Type == (TPrimitiveVariable::type_char & TPrimitiveVariable::type_one))
		strValue.Set(static_cast<char>(var->Get4Value()));
	else if (v1Type == (TPrimitiveVariable::type_char & TPrimitiveVariable::type_two))
		strValue.Set(static_cast<WCHAR>(var->Get4Value()));
	else if (v1Type == TPrimitiveVariable::type_bool)
	{
		if (var->Get8Value())
			strValue.Set(L"true");
		else
			strValue.Set(L"false");
	}
	else
	{
		auto value = GetValueFromPrimitive(var);
		switch (value.type)
		{
		case double_long::dl_double:
			strValue.Format(L"%f", value.value.d);
			break;
		case double_long::dl_sign:
			strValue.Format(L"%d", value.value.s);
			break;
		case double_long::dl_unsign:
			strValue.Format(L"%u", value.value.u);
		}
	}
	return strValue;
}

DoubleLong::DoubleLong(ULONG64 val)
{
	value.u = val;
	type = double_long::dl_unsign;
}

DoubleLong::DoubleLong(LONG64 val)
{
	value.s = val;
	type = double_long::dl_sign;
}

DoubleLong::DoubleLong(double val)
{
	value.d = val;
	type = double_long::dl_double;
}

DoubleLong::DoubleLong()
{
	type = double_long::dl_invalid;
	value.u = 0ULL;
}

bool DoubleLong::operator<(const DoubleLong& o)
{
	switch (type)
	{
	case double_long::dl_double:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.d < o.value.d;
		case double_long::dl_sign:
			return value.d < o.value.s;
		case double_long::dl_unsign:
			return value.d < o.value.u;
		}
		break;
	case double_long::dl_sign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.s < o.value.d;
		case double_long::dl_sign:
			return value.s < o.value.s;
		case double_long::dl_unsign:
			return value.s < o.value.u;
		}
		break;
	case double_long::dl_unsign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.u < o.value.d;
		case double_long::dl_sign:
			return value.u < o.value.s;
		case double_long::dl_unsign:
			return value.u < o.value.u;
		}
	}

	return false;
}

bool DoubleLong::operator==(const DoubleLong& o)
{
	switch (type)
	{
	case double_long::dl_double:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.d == o.value.d;
		case double_long::dl_sign:
			return value.d == o.value.s;
		case double_long::dl_unsign:
			return value.d == o.value.u;
		}
		break;
	case double_long::dl_sign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.s == o.value.d;
		case double_long::dl_sign:
			return value.s == o.value.s;
		case double_long::dl_unsign:
			return value.s == o.value.u;
		}
		break;
	case double_long::dl_unsign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.u == o.value.d;
		case double_long::dl_sign:
			return value.u == o.value.s;
		case double_long::dl_unsign:
			return value.u == o.value.u;
		}
	}
	return type == o.type;
}

ULONG64 DoubleLong::ToUnsignedLong()const
{
	ULONG64 ret;

	switch (type)
	{
	case double_long::dl_double:
		memcpy_s(&ret, sizeof(ret), &value.d, sizeof(value.d));
		break;
	case double_long::dl_sign:
		memcpy_s(&ret, sizeof(ret), &value.s, sizeof(value.s));
		break;
	case double_long::dl_unsign:
		ret = value.u;
		break;
	default:
		ret = 0ULL;
	}

	return ret;
}

ULONG64 DoubleLong::GetBitAnd(const DoubleLong& o)
{
	return ToUnsignedLong() & o.ToUnsignedLong();
}

ULONG64 DoubleLong::GetBitOr(const DoubleLong& o)
{
	return ToUnsignedLong() | o.ToUnsignedLong();
}

ULONG64 DoubleLong::GetBitXor(const DoubleLong& o)
{
	return ToUnsignedLong() ^ o.ToUnsignedLong();
}

bool DoubleLong::operator<=(const DoubleLong& o)
{
	switch (type)
	{
	case double_long::dl_double:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.d <= o.value.d;
		case double_long::dl_sign:
			return value.d <= o.value.s;
		case double_long::dl_unsign:
			return value.d <= o.value.u;
		}
		break;
	case double_long::dl_sign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.s <= o.value.d;
		case double_long::dl_sign:
			return value.s <= o.value.s;
		case double_long::dl_unsign:
			return value.s <= o.value.u;
		}
		break;
	case double_long::dl_unsign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.u <= o.value.d;
		case double_long::dl_sign:
			return value.u <= o.value.s;
		case double_long::dl_unsign:
			return value.u <= o.value.u;
		}
	}
	return false;
}

bool DoubleLong::operator>=(const DoubleLong& o)
{
	switch (type)
	{
	case double_long::dl_double:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.d >= o.value.d;
		case double_long::dl_sign:
			return value.d >= o.value.s;
		case double_long::dl_unsign:
			return value.d >= o.value.u;
		}
		break;
	case double_long::dl_sign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.s >= o.value.d;
		case double_long::dl_sign:
			return value.s >= o.value.s;
		case double_long::dl_unsign:
			return value.s >= o.value.u;
		}
		break;
	case double_long::dl_unsign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.u >= o.value.d;
		case double_long::dl_sign:
			return value.u >= o.value.s;
		case double_long::dl_unsign:
			return value.u >= o.value.u;
		}
	}
	return false;
}

bool DoubleLong::operator>(const DoubleLong& o)
{
	switch (type)
	{
	case double_long::dl_double:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.d > o.value.d;
		case double_long::dl_sign:
			return value.d > o.value.s;
		case double_long::dl_unsign:
			return value.d > o.value.u;
		}
		break;
	case double_long::dl_sign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.s > o.value.d;
		case double_long::dl_sign:
			return value.s > o.value.s;
		case double_long::dl_unsign:
			return value.s > o.value.u;
		}
		break;
	case double_long::dl_unsign:
		switch (o.type)
		{
		case double_long::dl_double:
			return value.u > o.value.d;
		case double_long::dl_sign:
			return value.u > o.value.s;
		case double_long::dl_unsign:
			return value.u > o.value.u;
		}
	}
	return false;
}
