#pragma once
#include "TNativeInterpretor.h"
#include <TcInterpretor.h>
#include "TcNativeInterpretor.h"


TMap<TcNativeInterpretor> GetJavaScriptFunctions();





namespace JavaScriptFunc{
	/**
	 * Function: isFinite
	 * Purpose: Reports whether a given variable is "finite"
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "isFinite" function in JavaScript
	 */
	TC_DATA_STRUCT void isFinite(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: isFinite
	 * Purpose: Reports whether a given variable is a number
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "isNan" function in JavaScript
	 */
	TC_DATA_STRUCT void isNaN(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: parseFloat
	 * Purpose: attwmpts to parse out a floating point
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "parseFloat" function in JavaScript. 
	 */
	TC_DATA_STRUCT void parseFloat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: parseInt
	 * Purpose: attempts to parse out an Integer.
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 *
	 * Note: This Function is intended to fulfill the "parseInt" function in JavaScript
	 */
	TC_DATA_STRUCT void parseInt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: EncodeURI
	 * Purpose: Implements the encodeURI JavaScript Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void EncodeURI(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: EncodeURIComponent
	 * Purpose: Implements the encodeURIComponent JavaScript Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void EncodeURIComponent(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: DecodeURI
	 * Purpose: Implements the encodeURI JavaScript Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void DecodeURI(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: DecodeURIComponent
	 * Purpose: Implements the encodeURIComponent JavaScript Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void DecodeURIComponent(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: JsEscape
	 * Purpose: Implements the escape JavaScript Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void JsEscape(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: JsUnEscape
	 * Purpose: Implements the unescape JavaScript Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void JsUnEscape(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	// Number Related Functions

	/**
	 * Function: Number
	 * Purpose: Implements the Number Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void Number(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

	/**
	 * Function: String
	 * Purpose: Implements the String Function
	 * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters given to the native function
	 *				ReturnObject& ret - object to report the results to
	 * Returns: void
	 */
	TC_DATA_STRUCT void String(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);



	TC_DATA_STRUCT TrecPointer<TVariable> GetJSObectVariable(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);

	TC_DATA_STRUCT TrecPointer<TVariable> GetJSObectVariable(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);
	// Helper Functions
	bool IsInfinity(TString& str);

	bool IsURLEscaped(WCHAR c, UCHAR regular);

	bool ConvertParamOneToString(TString& value, TDataArray<TrecPointer<TVariable>>& params);
}