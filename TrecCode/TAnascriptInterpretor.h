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
     * Returns: ReportObject - objct indicating the success of the program or failure information
     */
    ReportObject ProcessLet(TString& let, UINT line);


    /**
     * Method: TAnascriptInterpretor::ProcessLoop
     * Purpose: Processes the loop block
     * Parameters: TString& let - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or failure information
     */
    ReportObject ProcessLoop(TString& let, UINT line);

    /**
     * Method: TAnascriptInterpretor::ProcessIf
     * Purpose: Processes the if block
     * Parameters: TString& _if - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or failure information
     */
    ReportObject ProcessIf(TString& _if, UINT line);


    /**
     * Method: TAnascriptInterpretor::ProcessWhile
     * Purpose: Processes the while block
     * Parameters: TString& let - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or failure information
     */
    ReportObject ProcessWhile(TString& let, UINT line);


    /**
     * Method: TAnascriptInterpretor::ProcessFunction
     * Purpose: Processes the creation of an Anascript function
     * Parameters: TString& fun - the start of the function declaration to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or failure information
     */
    ReportObject ProcessFunction(TString& fun, UINT line);

    /**
     * Method: TAnascriptInterpretor::ProcessExpression
     * Purpose: Processes the let command
     * Parameters: TString& exp - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     *              ReportObject& ro - objct indicating the success of the program or failure information
     * Returns: void
     */
    void ProcessExpression(TString& exp, UINT line, ReportObject& ro);


    /**
     * Method: TAnascriptInterpretor::ProcessArrayExpression
     * Purpose: Processes the Array expression
     * Parameters: TString& exp - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     *              ReportObject& ro - objct indicating the success of the program or failure information
     * Returns: void
     */
    void ProcessArrayExpression(TString& exp, UINT line, ReportObject& ro);


    /**
     * Method: TAnascriptInterpretor::ProcessProcedureCall
     * Purpose: Processes the Call to a function or method
     * Parameters: TString& exp - the expression to analyze statement to inspect and process
     *              UINT line - the line number being called upon,
     *              ReportObject& ro - objct indicating the success of the program or falure information
     * Returns: void
     */
    void ProcessProcedureCall(TString& exp, UINT line, ReportObject& ro);


    /**
     * Method: TAnascriptInterpretor::ProcessProcedureCall
     * Purpose: Processes the Call to a function or method
     * Parameters: TrecPointer<TVariable> object - the object to call the method on
     *              TrecPointer<TVariable> call - the procedure to call
     *              TString& exp - list of parameters that need to be formatted
     *              UINT line - the line number being called upon,
     *              ReportObject& ro - objct indicating the success of the program or falure information
     * Returns: void
     * Note: if "object" is initialized, it becomes the first parameter passed into "call" and we more or less have a method. Otherwise
     *  it is a regular function
     */
    void ProcessProcedureCall(TrecPointer<TVariable> object, TrecPointer<TVariable> call, TString& exp, UINT line, ReportObject& ro);

    /**
     * Method: TAnascriptInterpretor::IsTruthful
     * Purpose: Determines whether an expression is truthful or not
     * Parameters: TrecPointer<TVariable> var - the variable to analyze. 
     * Returns: bool
     */
    bool IsTruthful(TrecPointer<TVariable> var);
};

