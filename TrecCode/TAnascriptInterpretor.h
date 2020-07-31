#pragma once
#include <TInterpretor.h>
class TAnascriptInterpretor :
    public TInterpretor
{
public:

    /**
     * Method: TAnascriptInterpretor::TAnascriptInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TAnascriptInterpretor Object
     */
    TAnascriptInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor);


    /**
     * Method: TAnascriptInterpretor::SetCode
     * Purpose: Sets the initial file of the code
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     */
    virtual UINT SetCode(TFile&) override;


    /**
     * Method: TAnascriptInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     */
    virtual ReportObject Run() override;


    /**
     * Method: TAnascriptInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     */
    virtual ReportObject Run(TDataArray<TrecPointer<TVariable>>& params) override;


    // Methods specific to the Anascript Interpreter

    /**
     * Method: TAnascriptInterpretor::ProcessLet
     * Purpose: Processes the let command
     * Parameters: TString& let - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or falure information
     */
    ReportObject ProcessLet(TString& let, UINT line);


    /**
     * Method: TAnascriptInterpretor::ProcessLoop
     * Purpose: Processes the loop block
     * Parameters: TString& let - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or falure information
     */
    ReportObject ProcessLoop(TString& let, UINT line);



    /**
     * Method: TAnascriptInterpretor::ProcessExpression
     * Purpose: Processes the let command
     * Parameters: TString& exp - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     *              ReportObject ro - objct indicating the success of the program or falure information
     * Returns: void
     */
    void ProcessExpression(TString& exp, UINT line, ReportObject& ro);
};

