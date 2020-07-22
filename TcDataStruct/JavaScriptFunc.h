#pragma once
#include "TNativeInterpretor.h"


TMap<TNativeInterpretor> GetJavaScriptFunctions();


namespace JavaScriptFunc{
	/**
	 * Function: isFinite
	 * Purpose: Reports whether a given variable is "finite"
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReportObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "isFinite" function in JavaScript
	 */
	void isFinite(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret);

	/**
	 * Function: isFinite
	 * Purpose: Reports whether a given variable is a number
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReportObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "isNan" function in JavaScript
	 */
	void isNan(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret);

	/**
	 * Function: parseFloat
	 * Purpose: attwmpts to parse out a floating point
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReportObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "parseFloat" function in JavaScript. 
	 */
	void parseFloat(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret);

	/**
	 * Function: parseInt
	 * Purpose: attempts to parse out an Integer.
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReportObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "parseInt" function in JavaScript
	 */
	void parseInt(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret);





	// Helper Functions
	bool IsInfinity(TString& str);
}