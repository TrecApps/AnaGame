#pragma once
#include <TcInterpretor.h>
#include <CodeStatement.h>
#include <DefaultObjectOperator.h>
#include "TrecCode.h"
/**
 * Class: JsObjectOperator
 * Purpose: Provides JavaScript specific functionality to code interpretation.
 */
class JsObjectOperator : public DefaultObjectOperator
{
public:
    /**
     * Method: DefaultObjectOperator::Add
     * Purpose: Provides a means through which Addition can be performed
     * Parameters: TrecPointer<TVariable> op1 - the first operator
     *				TrecPointer<TVariable> op2 - the second operator
     * Returns: TrecPointer<TVariable> - the result of the operation
     *
     * Note: If the result is null, that is a sign that the operation didn't work
     */
    virtual TrecPointer<TVariable> Add(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) override;
};


class _TREC_CODE_DLL JavaScriptExpression2
{
public:
    JavaScriptExpression2();
    JavaScriptExpression2(const JavaScriptExpression2& orig);
    JavaScriptExpression2(const TString& name, TrecPointer<TVariable> value);

    void operator=(const JavaScriptExpression2& orig);

    TString varName;
    TrecPointer<TVariable> value;
};


/**
 * Class: TcJavaScriptInterpretor
 * Purpose: Implements the TcInterpretor for the JavaScript Programming Language
 * 
 * SuperClass: TcInterpretor
 */
class _TREC_CODE_DLL TcJavaScriptInterpretor : public TcInterpretor
{
public:

    /**
     * Method: TcJavaScriptInterpretor::CanCompile
     * Purpose: Whether the Compiler is capable of compiling it's statements into a more compact form
     * Parameters: void
     * Returns: COMPILE_TYPE - marker indicating the Compilation the Interpretor can do
     *
     * Attributes: override
     *
     * Note: If the Returned Value is 0, then the Interpretor will not Attempt to Compile and is expected to run the Statements AS IS
     */
    virtual COMPILE_TYPE CanCompile() override;

    /**
     * Method: TcJavaScriptInterpretor::Compile
     * Purpose: Attempts to Compile the Statements into more compact code according to the Specifications provided
     * Parameters: COMPILE_TYPE type - specifications for the code to be compiled
     *              ReturnObject& ret - allows method to report any error information encountered
     * Returns: void
     *
     * Attributes: override
     */
    virtual void Compile(COMPILE_TYPE type, ReturnObject& ret) override;

    /**
     * Method: TcJavaScriptInterpretor::SetFile
     * Purpose: Sets the File Where Sourcecode is expected to be read
     * Parameters: TrecPointer<TFileShell> codeFile - file holding the intitial source code
     *              ReturnObject& ret - allows method to report any error information encountered
     *              bool isFirst - whether this is the first Interpretor called (true by default)
     * Returns: void
     *
     * Attributes: override
     */
    virtual void SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst = true) override;

    /**
     * Method: TcJavaScriptInterpretor::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() override;

    /**
     * Method: TcJavaScriptInterpretor::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) override;


    /**
     * Method: TcJavaScriptInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: ReturnObject& ret - info about issues detected
     * Returns: void
     */
    virtual void PreProcess(ReturnObject& ret) override;

    /**
     * Method: TcInterpretor::ProcessIndividualStatement
     * Purpose: Allows an Interpretor to process a statement
     * Parameters: const TString& statement - the statemnt to process
     *              ReturnObject& ret - offers return information
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void ProcessIndividualStatement(const TString& statement, ReturnObject& ret) override;

    /**
     * Method: TcJavaScriptInterpretor::TcInterpretor
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     *              TrecPointer<TEnvironment> env - the environemnt this interpretor runs in
     * Returns: New TInterpretor Object
     */
    TcJavaScriptInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);

protected:

    /**
     * Method: TcJavaScriptInterpretor::DetermineParenthStatus
     * Purpose: Reports whether there are more open or closed parenthesis
     * Parameters: const TString& string - string to read
     * Returns: int - negative (more close than open parenth), positive (more open than close) or 0 (equal count)
     */
    int DetermineParenthStatus(const TString& string);

    /**
     * Method: TcJavaScriptInterpretor::SetStatements
     * Purpose: Allows Interpretors to set the statements of child Interpretors
     * Parameters: TDataArray<TrecPointer<CodeStatement>>& statements - the statements to set
     * Returns: void
     */
    void SetStatements(TDataArray<TrecPointer<CodeStatement>>& statements);

    /**
     * Method: TcJavaScriptInterpretor::SetStatementToBlock
     * Purpose: Updates a Statement to use an Interpretor to enable Scoping
     * Parameters: TrecPointer<CodeStatement>& statement - the statement to update
     *              ReturnObject& ret - offers return information
     * Returns: void
     */
    void SetStatementToBlock(TrecPointer<CodeStatement>& statement, ReturnObject& ret);

    /**
     * Method: TcJavaScriptInterpretor::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
     ReturnObject Run(TDataArray<TrecPointer<CodeStatement>>& statements);

     /**
      * Method: TcJavaScriptInterpretor::Run
      * Purpose: Allows the Interpretor to run the Code As Is
      * Parameters: void
      * Returns: ReturnObject - information about the Run
      */
     ReturnObject Run(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, TrecPointer<CodeStatement> statement);

    /**
     * Method: TcJavaScriptInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: ReturnObject& ret - info about issues detected
     *              TDataArray<TrecPointer<CodeStatement>>& statements - statement list to focus on
     * Returns: void
     */
    void PreProcess(ReturnObject& ret, TDataArray<TrecPointer<CodeStatement>>& statements);

    /**
     * Method: TcJavaScriptInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: ReturnObject& ret - info about issues detected
     *              TDataArray<TrecPointer<CodeStatement>>& statements - statement list to focus on
     * Returns: void
     */
    void PreProcess(ReturnObject& ret, TrecPointer<CodeStatement>& statement);

    /**
     * List of statements generated
     */
    TDataArray<TrecPointer<CodeStatement>> statements;

    /**
     * Whether the statements are preprocessed and ready to run
     */
    bool readyToRun;

    // Methods designed to address Pacific Statement Types

    void ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret);                    // Call when you need an expression analyzed
    void ProcessAssignmentStatement(TrecPointer<CodeStatement> statement, ReturnObject& ret);           // Call for const, let, and var statements
    void ProcessBasicFlow(TrecPointer<CodeStatement> statement, ReturnObject& ret);                     // Call for if, else, and else if statements
    void ProcessWhile(TrecPointer<CodeStatement> statement, ReturnObject& ret);                         // Call For while and do...while
    void ProcessFor(TDataArray<TrecPointer<CodeStatement>>& statement, UINT index, ReturnObject& ret);  // Call for for statements
    void ProcessTryCatchFinally(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, 
        TrecPointer<CodeStatement> statement, ReturnObject& ret);                                       // Call for the Try/catch/finally blocks
    void ProcessSwitch(TrecPointer<CodeStatement> statement, ReturnObject& ret);                        // Call for Switch Statements

    // Helper Methods for the Process-Methods
    bool IsTruthful(TrecPointer<TVariable> var);

    /// Process Expression Methods. They follow a certain pattern as they may or may not encompass the whole expression provided
    /// Parameters:
    ///     UINT& parenth - How many Parenthesis have already been processed
    ///     UINT& square - How many squarebrackets have been processed
    ///     UINT& index - Index in the statement string to start at
    ///     TrecPointer<CodeStatement> statement - the statement to actually process 
    ///     ReturnObject& ret - results of the operation
    ///     TDataArray<JavaScriptExpression2>& expressions - list of values derived
    ///     TDataArray<TString>& ops - operators between the evaluated expressions
    /// Returns: UINT - how many times the operation moved into the 'next'  statement
    UINT ProcessExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, 
        ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops);
    UINT ProcessArrayExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement,
        ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops);
    UINT ProcessJsonExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement,
        ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops);
    UINT ProcessVariableExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement,
        ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops);
    UINT ProcessFunctionExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement,
        ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops);
    UINT ProcessNumberExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement,
        ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops);
    UINT ProcessStringExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement,
        ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops);

    UINT ProcessProcedureCall(UINT& parenth, UINT& square, UINT& index, TrecPointer<TVariable> object,
        TrecPointer<TVariable> proc, TrecPointer<CodeStatement> statement, ReturnObject& obj);

    // Operator Handler Methods
    void HandlePreExpr(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleExponents(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleMultDiv(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleAddSub(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseShift(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleLogicalComparison(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleEquality(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseAnd(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseXor(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseOr(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleLogicalAnd(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleLogicalOr(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleNullish(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleConditional(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleAssignment(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleComma(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
};

