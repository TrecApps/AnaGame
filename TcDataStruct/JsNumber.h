#pragma once

#include "TcNativeInterpretor.h"
#include "TInterpretor.h"
namespace JsNumber
{
	/**
	 * Function: GetJsNumberObject
	 * Purpose: Collects all methods into a Math Object for Global Use
	 * Parameters: void
	 * Returns: TrecPointer<TVariable> - the Math Object
	 */
	TC_DATA_STRUCT TrecPointer<TVariable> GetJsNumberObject(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);
	TC_DATA_STRUCT TrecPointer<TVariable> GetJsNumberObject(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TC_DATA_STRUCT void isFinite(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void isInteger(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void isNaN(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void isSafeInteger(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void toExponential(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	TC_DATA_STRUCT void toFixed(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void toLocaleString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void toPrecision(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void toString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void valueOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

}