#pragma once
#include "TInterpretor.h"


typedef void (*NativeFunction)(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret);


/**
 * Class: TNativeInterpretor
 * Purpose: Allows Code interpretors and high-level source code to call native functions and methods provided by Anagame 
 */
class TNativeInterpretor :
	public TInterpretor
{
public:
    /**
     * Method: TNativeInterpretor::TNativeInterpretor
     * Purpose: Constructor
     * Parameters: NativeFunction function - the function to call
     *              TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TNativeInterpretor(NativeFunction function, TrecPointer<TInterpretor> parent);


    /**
     * Method: TInterpretor::SetCode
     * Purpose: Not Applicable
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: This is unused as this interpretor gets it'c code from a native function (meaning it's already compiled), not a source file
     */
    virtual UINT SetCode(TFile&) override;

    /**
     * Method: TNativeInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     */
    virtual ReportObject Run()override;


    /**
     * Method: TNativeInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     */
    virtual ReportObject Run(TDataArray<TVariable>& params) override;

private:
    /**
     * Pointer to the native function to call
     */
    NativeFunction nativeFunction;
};

