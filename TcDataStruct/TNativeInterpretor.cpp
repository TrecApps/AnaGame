#include "pch.h"
#include "TNativeInterpretor.h"

/**
 * Method: TNativeInterpretor::TNativeInterpretor
 * Purpose: Constructor
 * Parameters: NativeFunction function - the function to call
 *              TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 *				TrecPointer<TEnvironment> env - the environment underwhich this function is operating
 * Returns: New TInterpretor Object
 */
TNativeInterpretor::TNativeInterpretor(NativeFunction function, TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env): TInterpretor(parent, env)
{
	nativeFunction = function;
}

/**
 * Method: TInterpretor::SetCode
 * Purpose: Not Applicable
 * Parameters: TFile& the file to read
 * Returns: UINT - error code
 *
 * Note: This is unused as this interpretor gets it'c code from a native function (meaning it's already compiled), not a source file
 */
UINT TNativeInterpretor::SetCode(TFile&)
{
	return 0;
}

/**
 * Method: TNativeInterpretor::Run
 * Purpose: Runs the interpretor
 * Parameters: void
 * Returns: ReportObject - objct indicating the success of the program or falure information
 *
 * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
 */
ReportObject TNativeInterpretor::Run()
{
	TDataArray<TrecPointer<TVariable>> blankParams;
	return Run(blankParams);
}

/**
 * Method: TNativeInterpretor::Run
 * Purpose: Runs the interpretor with the following parameters
 * Parameters: TDataArray<TVariable> params - list of parameters to provide
 * Returns: ReportObject - objct indicating the success of the program or falure information
 *
 * Note: this method is intended to be called in interpretors that represent specific methods or functions
 */
ReportObject TNativeInterpretor::Run(TDataArray<TrecPointer<TVariable>>& params, bool clearVars)
{
	ThreadLock();
	ReportObject result;
	if (!nativeFunction)
	{
		result.returnCode = ReportObject::broken_reference;
		result.errorMessage.Set(L"Null Reference to Native Function!\n");
	}
	else
		nativeFunction(params, environment, result);
	ThreadRelease();
	return result;
}
