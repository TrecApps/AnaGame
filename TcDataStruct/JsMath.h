#pragma once
#include "TcNativeInterpretor.h"

namespace JsMath
{
	/**
	 * Function: GetJsMathObject
	 * Purpose: Collects all methods into a Math Object for Global Use
	 * Parameters: void
	 * Returns: TrecPointer<TVariable> - the Math Object
	 */
	TC_DATA_STRUCT TrecPointer<TVariable> GetJsMathObject(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

	TC_DATA_STRUCT void abs(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void acos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void acosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void asin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void asinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void atan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void atan2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void atanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void cbrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void ceil(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void clz32(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void cos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void cosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void exp(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void expm1(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	TC_DATA_STRUCT void floor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void fround(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void log10(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void log1p(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void log2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void max_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void min_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void pow(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void random(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void round(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void sign(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void sin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void sinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void sqrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	TC_DATA_STRUCT void tan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void tanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
	TC_DATA_STRUCT void trunc(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);
}

namespace JsMathTc
{
	TC_DATA_STRUCT TrecPointer<TVariable> GetJsMathObject();

	TC_DATA_STRUCT void abs(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void acos(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void acosh(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void asin(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void asinh(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void atan(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void atan2(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void atanh(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void cbrt(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void ceil(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void clz32(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void cos(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void cosh(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void exp(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void expm1(TDataMap<TcVariableHolder> variables, ReturnObject& ret);

	TC_DATA_STRUCT void floor(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void fround(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void log(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void log10(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void log1p(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void log2(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void max_(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void min_(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void pow(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void random(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void round(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void sign(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void sin(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void sinh(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void sqrt(TDataMap<TcVariableHolder> variables, ReturnObject& ret);

	TC_DATA_STRUCT void tan(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void tanh(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void trunc(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
}

namespace JsOperatorsTc
{
	TC_DATA_STRUCT  TrecPointer<TVariable> addDefault(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);

	TC_DATA_STRUCT TrecPointer<TVariable> GetJsOperators();

	TC_DATA_STRUCT void str_cond(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void	bool_op(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void add(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void	sub(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void	mul(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void	div(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void	mod(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
	TC_DATA_STRUCT void	last(TDataMap<TcVariableHolder> variables, ReturnObject& ret);
}