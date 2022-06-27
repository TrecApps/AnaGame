
#include "TVariable.h"

bool VarFunction::IsTrue(TrecPointer<TVariable> var, bool& result, UCHAR def)
{
	if (!var.Get())
	{
		bool ret = (def & 0b00000001);
		if(ret)
			result = false;
		return ret;
	}

	switch (var->GetVarType())
	{
	case var_type::string:
		if (def & 0b00000100)
		{
			TString v(var->GetString().GetTrim().GetLower());
			result = v.Compare(L"false");
			break;
		}
		else return false;
	case var_type::primitive:
	case var_type::primitive_formatted:

		result = var->Get8Value() > 0;
		return true;
	case var_type::collection:
		if (def & 0b00010000)
		{
			result = var->GetSize() > 0;
			return true;
		}
		else return false;
	default:
		result = true;
	}

	return true;
}


/**
 * Method: TVariable::GetString
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: const TString& detail -
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
 *
 * Attributes: abstract
 */
TString TVariable::GetString(const TString& detail)
{
	UNREFERENCED_PARAMETER(detail);
	return GetString();
}

/**
 * Method: TVariable::SetSelf
 * Purpose: Allows the Variable to have a reference to itself
 * Parameters: TrecPointer<TVariable> - reference to assign
 * Returns: void
 */
void TVariable::SetSelf(TrecPointer<TVariable> pvSelf)
{
	if (pvSelf.Get() != this)
		throw L"Non Matching reference!";
	this->vSelf = TrecPointerKey::GetSoftPointerFromTrec <> (pvSelf);
}

/**
 * Method: TVariableMarker::TVariableMarker
 * Purpose: Constructor
 * Parameters: bool makeMutable - whether this variable should be able to be changed later
 *              TrecPointer<TVariable> var - the new variable
 * Returns: New Variable marker
 */
TVariableMarker::TVariableMarker(bool makeMutable, TrecPointer<TVariable> var)
{
	mutableVar = makeMutable;
	this->var = var;
}

/**
 * Method: TVariableMarker::TVariableMarker
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Variable marker
 */
TVariableMarker::TVariableMarker()
{
	mutableVar = true;
}

/**
 * Method: TVariableMarker::IsMutable
 * Purpose: Reports whether the variable is mutable (can be changed) or not
 * Parameters: void
 * Returns: bool - mutability status
 */
TVariableMarker::TVariableMarker(const TVariableMarker& orig)
{
	mutableVar = orig.IsMutable();
	var = orig.GetVariable();
}

/**
 * Method: TVariableMarker::operator=
 * Purpose: Overloads assignment operator for the class
 * Parameters: const TVariableMarker& orig -  the marker to copy from
 * Returns: void
 */
void TVariableMarker::operator=(const TVariableMarker& orig)
{
	mutableVar = orig.IsMutable();
	var = orig.GetVariable();
}

/**
 * Method: TVariableMarker::SetVariable
 * Purpose: Sets the Variable if allowed
 * Parameters: TrecPointer<TVariable> var - the new variable to assign to
 * Returns: bool - whether the new variable was set
 */
bool TVariableMarker::SetVariable(TrecPointer<TVariable> var_)
{
	if (mutableVar)
		this->var = var_;
	return mutableVar;
}

/**
 * Method: TVariableMarker::GetVariable
 * Purpose: Reports the Variable being held
 * Parameters: void
 * Returns: TrecPointer<TVariable> var - the variable being held
 */
TrecPointer<TVariable> TVariableMarker::GetVariable()const
{
	return var;
}

/**
 * Method: TVariableMarker::IsMutable
 * Purpose: Reports whether the variable is mutable (can be changed) or not
 * Parameters: void
 * Returns: bool - mutability status
 */
bool TVariableMarker::IsMutable()const
{
	return this->mutableVar;
}


TrecPointer<TVariable> TVariable::Clone()
{
	return TrecPointer<TVariable>();
}

TrecPointer<TVariable> TVariable::GetIterator()
{
	return TrecPointer<TVariable>();
}

bool TVObject::HasVariableSupport()
{
	return true;
}

var_type TVariableIterator::GetVarType()
{
	return var_type::iterator;
}

UINT TVariableIterator::GetVType()
{
	return 0;
}

TString TVariableIterator::GetString()
{
	return TString();
}

UINT TVariableIterator::Get4Value()
{
	return 0;
}

ULONG64 TVariableIterator::Get8Value()
{
	return ULONG64();
}

ReturnObject TVariableIterator::GetErrorInfo()
{
	return ReturnObject();
}
