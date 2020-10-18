#pragma once
#include <TInterpretor.h>
#include "TrecCode.h"

/**
 * Class: TAnascriptInterpretor
 * Purpose: Interprets the Anagame specific language Anascript
 * 
 * SuperClass: TInterpretor
 */
class _TREC_CODE_DLL TAnascriptInterpretor :
    public TInterpretor
{
public:

    /**
     * Method: TAnascriptInterpretor::TAnascriptInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TAnascriptInterpretor Object
     */
    TAnascriptInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);


    /**
     * Method: TAnascriptInterpretor::SetCode
     * Purpose: Sets the initial file of the code
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     * 
     * Attributes: override
     */
    virtual UINT SetCode(TFile&) override;


    /**
     * Method: TAnascriptInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     * 
     * Attributes: override
     */
    virtual ReportObject Run() override;


    /**
     * Method: TAnascriptInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     * 
     * Attributes: override
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
    ReportObject ProcessLet(TString& let, UINT line, bool expectLet);


    /**
     * Method: TAnascriptInterpretor::ProcessLoop
     * Purpose: Processes the loop block
     * Parameters: TString& let - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or failure information
     */
    ReportObject ProcessLoop(TString& let, UINT line);

    /**
     * Method: TAnascriptInterpretor::ProcessElse
     * Purpose: Processes the if block
     * Parameters: TString& _if - the 'let' statement to inspect and process
     *              UINT line - the line number being called upon,
     * Returns: ReportObject - objct indicating the success of the program or failure information
     */
    ReportObject ProcessElse(TString& _else, UINT line);

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

    /**
     * Method: TAnascriptInterpretor::InspectNumber
     * Purpose: Retrieves a number if valid input is provided
     * Parameters: TString& exp - the expression to analyze statement to inspect and process
     *              UINT line - the line number being called upon,
     *              ReportObject& ro - objct indicating the success of the program or falure information
     * Returns: void
     */
    void InspectNumber(TString& exp, UINT line, ReportObject& ro);

    /**
     * Method: TAnascriptInterpretor::InspectVariable
     * Purpose: Retrieves a variable if valid
     * Parameters: TString& exp - the expression to analyze statement to inspect and process
     *              UINT line - the line number being called upon,
     *              ReportObject& ro - objct indicating the success of the program or falure information
     * Returns: void
     */
    void InspectVariable(TString& exp, UINT line, ReportObject& ro);




    /// Handling Operators
    
    /**
     * Method: TAnascriptInterpretor::HandleMultDiv
     * Purpose: Checks list of expressions and operators and sees if multiplication, division, or mod division needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleMultDiv(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

    /**
     * Method: TAnascriptInterpretor::HandleAddSub
     * Purpose: Checks list of expressions and operators and sees if addition or subtraction needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleAddSub(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

    /**
     * Method: TAnascriptInterpretor::HandleCompare
     * Purpose: Checks list of expressions and operators and sees if >/>=/</<= needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleCompare(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

    /**
     * Method: TAnascriptInterpretor::HandleEquality
     * Purpose: Checks list of expressions and operators and sees if equality needs to be analyzed
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleEquality(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);


    /**
     * Method: TAnascriptInterpretor::HandleBitAnd
     * Purpose: Checks list of expressions and operators and sees if bitwise AND needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleBitAnd(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

    /**
     * Method: TAnascriptInterpretor::HandleBitXorNxor
     * Purpose: Checks list of expressions and operators and sees if Bitwise XOR, NXOR needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleBitXorNxor(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

    /**
     * Method: TAnascriptInterpretor::HandleBitOrNor
     * Purpose: Checks list of expressions and operators and sees if bitwise OR or NOR needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleBitOrNor(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);
    
    /**
     * Method: TAnascriptInterpretor::HandleAnd
     * Purpose: Checks list of expressions and operators and sees if Logical AND needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleAnd(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

    /**
     * Method: TAnascriptInterpretor::HandleOr
     * Purpose: Checks list of expressions and operators and sees if bitwise OR or NOR needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleOr(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

    /**
     * Method: TAnascriptInterpretor::HandleAssign
     * Purpose: Checks list of expressions and operators and sees if an assignment needs to be done
     * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
     *              TDataArray<TString>& ops - list of operators
     *              ReportObject& errorMessage - lets caller know if there was an error detected
     * Returns: void
     */
    void HandleAssign(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage);

};

