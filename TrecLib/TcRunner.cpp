#include "TcRunner.h"



/**
 * Method: TcRunner::SetParamNames
 * Purpose: Allows Parameter names to be set
 * Parameters: TDataArray<TString>& paramNames - the Names of Initial parameters
 * Returns: void
 */
void TcRunner::SetParamNames(TDataArray<TString>& paramNames_)
{
	this->paramNames.RemoveAll();
	for (UINT Rust = 0; Rust < paramNames_.Size(); Rust++)
	{
		this->paramNames.push_back(paramNames_[Rust]);
	}
}

/**
 * Method: TcRunner::SetParamTypes
 * Purpose: Allows Parameter types to be set
 * Parameters: TDataArray<TString>& paramTypes - the Types of Initial parameters
 * Returns: void
 */
void TcRunner::SetParamTypes(TDataArray<TString>& paramTypes_)
{
	this->paramTypes.RemoveAll();
	for (UINT Rust = 0; Rust < paramTypes_.Size(); Rust++)
	{
		this->paramTypes.push_back(paramTypes_[Rust]);
	}
}

void TcRunner::SetCurrentStack(TrecPointer<BinaryStack> stack)
{
	
}

ReturnObject TcNativeRunner1::Run()
{
	ReturnObject obj;
	function(variables, obj);
	return obj;
}


void TcNativeRunner1::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) {
	this->variables.clear();
	for (UINT Rust = 0; Rust < params.Size(); Rust++)
	{
		variables.addEntry(Rust, TcVariableHolder(true, L"", params[Rust]));
	}
}


TcNativeRunner1::TcNativeRunner1(NativeTcRunner1 function)
{
	this->function = function;
}





/**
 * Method: TcRunner::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
 *
 * Attributes: override
 */
TString TcRunner::GetString()
{
	return TString();
}

/**
 * Method: TcRunner::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type)
 *
 * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
 *
 * Attributes: override
 */
UINT TcRunner::Get4Value()
{
	return 0;
}

/**
 * Method: TcRunner::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 *
 * Attributes: override
 */
ULONG64 TcRunner::Get8Value()
{
	return ULONG64();
}

/**
 * Method: TcRunner::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 *
 * Attributes: override
 */
UINT TcRunner::GetSize()
{
	return 0;
}

/**
 * Method: TcRunner::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 *
 * Attributes: override
 */
UINT TcRunner::GetVType()
{
	return 0;
}

bool BinaryStack::IsError()
{
	return retObj.returnCode > 0;
}

ReturnObject BinaryStack::GetError(bool reset)
{
	ReturnObject r(retObj);
	if (reset)
	{
		retObj.errorMessage.Empty();
		retObj.errorObject.Nullify();
		retObj.errorObject2.Nullify();
		retObj.mode = return_mode::rm_regular;
		retObj.stackTrace.RemoveAll();
		retObj.returnCode = 0;
	}
	return r;
}

UINT BinaryStack::GetSize()
{
	return binaries.Size();
}

UINT BinaryStack::GetSizeInBytes()
{
	return binaries.Size() * 4;
}

BinaryStack::BinaryStack()
{
}

BinaryStack::BinaryStack(const BinaryStack& copy): retObj(copy.retObj)
{
	this->binaries = copy.binaries;
}

void BinaryStack::Push(UINT data)
{
	binaries.push_back(data);
}

void BinaryStack::Push(ULONG64 data)
{
	UINT highEnd = 0, lowEnd = 0;
	memcpy_s(&highEnd, 4, &data, 4);
	memcpy_s(&lowEnd, 4, (&data) + 1, 4);

	binaries.push_back(highEnd);
	binaries.push_back(lowEnd);
}

UINT BinaryStack::Pop1()
{
	if (!binaries.Size())
	{
		retObj.returnCode = retObj.ERR_INTERNAL;
		retObj.errorMessage.Set(L"Binary Attempt to pop value from an Empty Stack!");
		return 0;
	}
	return binaries.RemoveAt(binaries.Size() - 1);
}

ULONG64 BinaryStack::Pop2()
{
	if (binaries.Size() < 2)
	{
		retObj.returnCode = retObj.ERR_INTERNAL;
		retObj.errorMessage.Set(L"Binary Attempt to pop value from an Empty (or insuficciently sized) Stack!");
		return 0;
	}
	UINT lowEnd = binaries.RemoveAt(binaries.Size() - 1);
	UINT highEnd = binaries.RemoveAt(binaries.Size() - 1);
	return (static_cast<ULONG64>(highEnd) << 4) + static_cast<ULONG64>(lowEnd);
}

bool BinaryStack::GetValueFromTop(UINT loc, UINT& val)
{
	if (++loc > binaries.Size())
		return false;
	val = binaries[binaries.Size() - loc];
	return true;
}

bool ObjectStack::IsError()
{
	return retObj.returnCode > 0;
}

ReturnObject ObjectStack::GetError(bool reset)
{
	ReturnObject r(retObj);
	if (reset)
	{
		retObj.errorMessage.Empty();
		retObj.errorObject.Nullify();
		retObj.errorObject2.Nullify();
		retObj.mode = return_mode::rm_regular;
		retObj.stackTrace.RemoveAll();
		retObj.returnCode = 0;
	}
	return r;
}

UINT ObjectStack::GetSize()
{
	return objects.Size();
}

ObjectStack::ObjectStack()
{
}

ObjectStack::ObjectStack(const ObjectStack& copy)
{
	this->objects = copy.objects;
	this->retObj = copy.retObj;
}

void ObjectStack::Push(TrecPointer<TVariable> var)
{
	objects.push_back(var);
}

TrecPointer<TVariable> ObjectStack::Pop()
{
	if (!objects.Size())
	{
		retObj.returnCode = retObj.ERR_INTERNAL;
		retObj.errorMessage.Set(L"Binary Attempt to pop value from an Empty Stack!");
		return TrecPointer<TVariable>();
	}
	return objects.RemoveAt(objects.Size() - 1);
}

bool ObjectStack::GetValueFromTop(UINT loc, TrecPointer<TVariable>& val)
{
	if (++loc > objects.Size())
		return false;
	val = objects[objects.Size() - loc];
	return true;
}
