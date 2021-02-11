#pragma once
#include "TNativeInterpretor.h"

namespace JsMath
{
	/**
	 * Function: GetJsMathObject
	 * Purpose: Collects all methods into a Math Object for Global Use
	 * Parameters: void
	 * Returns: TrecPointer<TVariable> - the Math Object
	 */
	TC_DATA_STRUCT TrecPointer<TVariable> GetJsMathObject(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TC_DATA_STRUCT void abs(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void acos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void acosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void asin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void asinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void atan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void atan2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void atanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void cbrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void ceil(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void clz32(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void cos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void cosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void exp(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void expm1(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	TC_DATA_STRUCT void floor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void fround(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void log10(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void log1p(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void log2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void max_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void min_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void pow(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void random(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void round(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void sign(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void sin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void sinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void sqrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);

	TC_DATA_STRUCT void tan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void tanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
	TC_DATA_STRUCT void trunc(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret);
}