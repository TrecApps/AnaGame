#pragma once
#include "TInterpretor.h"


typedef void (*NativeFunction)(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env,ReportObject& ret);


/**
 * Class: TNativeInterpretor
 * Purpose: Allows Code interpretors and high-level source code to call native functions and methods provided by Anagame
 * 
 * SuperClass: TInterpretor
 */
class TC_DATA_STRUCT TNativeInterpretor :
	public TInterpretor
{
public:
    /**
     * Method: TNativeInterpretor::TNativeInterpretor
     * Purpose: Constructor
     * Parameters: NativeFunction function - the function to call
     *              TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     *				TrecPointer<TEnvironment> env - the environment underwhich this function is operating
     * Returns: New TInterpretor Object
     */
    TNativeInterpretor(NativeFunction function, TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env);


    /**
     * Method: TInterpretor::SetCode
     * Purpose: Not Applicable
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: This is unused as this interpretor gets it'c code from a native function (meaning it's already compiled), not a source file
     * 
     * Attributes: override
     */
    virtual UINT SetCode(TFile&) override;

    /**
     * Method: TNativeInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     * 
     * Attributes: override
     */
    virtual ReportObject Run()override;


    /**
     * Method: TNativeInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     * 
     * Attributes: override
     */
    virtual ReportObject Run(TDataArray<TrecPointer<TVariable>>& params, bool clearVars = true) override;

private:
    /**
     * Pointer to the native function to call
     */
    NativeFunction nativeFunction;
};

