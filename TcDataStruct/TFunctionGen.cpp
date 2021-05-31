#include "TFunctionGen.h"

/**
 * Method: TFunctionGen::TFunctionGen
 * Purpose: Constructor
 * Parameters: TrecSubPointer<TVariable, TcInterpretor> func - the function to clone
 * Returns: new Function Generator
 *
 * Note: Will throw if 'func' is null
 */
TFunctionGen::TFunctionGen(TrecSubPointer<TVariable, TcInterpretor> func)
{
	if (!func.Get())
		throw L"Null Pointer";
	this->func = func;
}

/**
 * Method: TFunctionGen::Generate
 * Purpose: Generates a new Function based off of the provided parameters
 * Parameters: TDataArray<TrecPointer<TVariable>>& params - parameters to provide the function
 * Returns: TrecSubPointer<TVariable, TcInterpretor> - a clone of the initial function
 */
TrecSubPointer<TVariable, TcInterpretor> TFunctionGen::Generate(TDataArray<TrecPointer<TVariable>>& params)
{
	auto ret = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(func->Clone());
	ret->SetIntialVariables(params);
	return ret;
}

/**
 * Method: TFunctionGen::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 *
 * Attributes: override
 */
var_type TFunctionGen::GetVarType()
{
	return var_type::interpretor_gen;
}

TrecObjectPointer TFunctionGen::GetObject()
{
	return TrecObjectPointer();
}

TString TFunctionGen::GetString()
{
	return TString();
}

UINT TFunctionGen::Get4Value()
{
	return 0;
}

ULONG64 TFunctionGen::Get8Value()
{
	return ULONG64();
}

UINT TFunctionGen::GetSize()
{
	return 0;
}

UINT TFunctionGen::GetVType()
{
	return 0;
}
