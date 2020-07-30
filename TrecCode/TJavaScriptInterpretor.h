#pragma once
#include <TInterpretor.h>

class TJavaScriptInterpretor :
    public TInterpretor
{

    /**
     * Method: TJavaScriptInterpretor::TInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TJavaScriptInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor);

    /**
     * Method: TJavaScriptInterpretor::SetCode
     * Purpose: Sets the initial file of the code
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     */
    virtual UINT SetCode(TFile&)override;

    /**
     * Method: TJavaScriptInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     */
    virtual ReportObject Run()override;


    /**
     * Method: TJavaScriptInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     */
    virtual ReportObject Run(TDataArray<TrecPointer<TVariable>>& params)override;

private:
    /**
     * Method: TJavaScriptInterpretor::GetNextStatement
     * Purpose: retrieves the tokens 
     */
};
