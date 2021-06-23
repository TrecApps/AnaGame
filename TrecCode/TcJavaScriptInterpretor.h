#pragma once
#include <TcTypeInterpretor.h>
#include <CodeStatement.h>
#include <DefaultObjectOperator.h>
#include "TrecCode.h"
#include <TContainerVariable.h>
#include <TAsyncVariable.h>
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
class _TREC_CODE_DLL TcJavaScriptInterpretor : public TcTypeInterpretor
{
    friend class TcJavaScriptClassInterpretor;
public:

    virtual TrecPointer<TVariable> Clone() override;

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
     * Method: TcJavaScriptInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: void
     * Returns: ReturnObject - info about issues detected
     */
    ReturnObject PreProcess();

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


    static bool IsTruthful(TrecPointer<TVariable> var);
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
     ReturnObject Run(TDataArray<TrecPointer<CodeStatement>>& statements, UINT start = 0);

     /**
      * Method: TcJavaScriptInterpretor::Run
      * Purpose: Allows the Interpretor to run the Code As Is
      * Parameters: void
      * Returns: ReturnObject - information about the Run
      */
     ReturnObject Run(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, TrecPointer<CodeStatement> statement);

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
     * Method: TcJavaScriptInterpretor::HandleSemiColon
     * Purpose: Initiates the process of semi-colon insertion amongst the collected statements
     * Parameters: void
     * Returns: void
     */
    void HandleSemiColon();


    /**
     * Method: TcJavaScriptInterpretor::HandleSemiColon
     * Purpose: Performs the process of semi-colon insertion amongst the collected statements
     * Parameters: TDataArray<TrecPointer<CodeStatement>>& statements - the statements to update
     * Returns: void
     */
    void HandleSemiColon(TDataArray<TrecPointer<CodeStatement>>& statements);

    /**
     * Method: TcJavaScriptInterpretor::HandleSemiColon
     * Purpose: Performs the process of semi-colon insertion amongst the collected statements
     * Parameters: TrecPointer<CodeStatement> statement - the statement to update
     *              TDataArray<TrecPointer<CodeStatement>>& statements - the statement list to add statements to
     *              bool isNext - whether the provided statement is a 'next' statement
     * Returns: void
     */
    void HandleSemiColon(TrecPointer<CodeStatement> statement, TDataArray<TrecPointer<CodeStatement>>& statements, bool isNext = false);
    /**
     * List of statements generated
     */
    TDataArray<TrecPointer<CodeStatement>> statements;

    /**
     * Whether the statements are preprocessed and ready to run
     */
    bool readyToRun;

    // Methods designed to address Pacific Statement Types

    UINT ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret,TrecSubPointer<TVariable, TcJavaScriptInterpretor> in
        = TrecSubPointer<TVariable, TcJavaScriptInterpretor>());                                                         // Call when you need an expression analyzed
    void ProcessAssignmentStatement(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT stringStart = 0);      // Call for const, let, and var statements
    void ProcessBasicFlow(TrecPointer<CodeStatement> statement, ReturnObject& ret);                                     // Call for if, else, and else if statements
    void ProcessWhile(TrecPointer<CodeStatement> statement, UINT& index, ReturnObject& ret);                                         // Call For while and do...while
    void ProcessFor(TDataArray<TrecPointer<CodeStatement>>& statement, UINT& index, ReturnObject& ret);                  // Call for for statements
    void ProcessTryCatchFinally(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, 
        TrecPointer<CodeStatement> statement, ReturnObject& ret);                                                       // Call for the Try/catch/finally blocks
    void ProcessSwitch(TrecPointer<CodeStatement> statement, ReturnObject& ret);                                        // Call for Switch Statements
    bool ProcessCase(TrecPointer<CodeStatement> caseStatement, TrecPointer<CodeStatement> switchStatement,
        TrecPointer<TVariable> var, bool& hadDefault, ReturnObject& ret);                                                                  // Call for case statements
    void ProcessClass(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);

    void ProcessPrototypeAssign(TrecPointer<CodeStatement> state, ReturnObject& ret);

    // Helper Methods for the Process-Methods




    /**
     * Method: TcInterpretor::GetVariable
     * Purpose: Retrieves the variable specified
     * Parameters: TString& varName - the name of the variable requested
     *				bool& present - whether the variable was present or not (used to distinguish between 'null' and 'undefined')
     *              bool currentScope - whether to only check the current scope (don't check parent interpretors), default of false
     * Returns: TrecPointer<TVariable> - the variable requested (null if not found)
     */
    virtual TrecPointer<TVariable> GetVariable(TString& varName, bool& present, bool currentScope = false) override;

    /// Process Expression Methods. They follow a certain pattern as they may or may not encompass the whole expression provided
    /// Parameters:
    ///     UINT& parenth - How many Parenthesis have already been processed
    ///     UINT& square - How many squarebrackets have been processed
    ///     UINT& index - Index in the statement string to start at
    ///     TrecPointer<CodeStatement> statement - the statement to actually process 
    ///     ReturnObject& ret - results of the operation
    /// Returns: UINT - how many times the operation moved into the 'next'  statement
    UINT ProcessExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    UINT ProcessArrayExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    UINT ProcessJsonPiece(TrecPointer<CodeStatement> statement, const TString& piece, TrecSubPointer<TVariable, TContainerVariable> obj, UINT stateIndex, ReturnObject& ret);
    UINT ProcessJsonExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    UINT ProcessVariableExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    UINT ProcessFunctionExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    UINT ProcessNumberExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    UINT ProcessStringExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    UINT ProcessPotentalArrowNotation(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);

    UINT ProcessProcedureCall(UINT& parenth, UINT& square, UINT& index, TrecPointer<TVariable> object,
        TrecPointer<TVariable> proc, TrecPointer<CodeStatement> statement, ReturnObject& ret, bool doAwait = false);

    void ProcessFunctionExpression(TrecPointer<CodeStatement> statement, ReturnObject& obj);

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

    // Sets up a console Object
    TrecPointer<TVariable> GetJsConsole();

    // Support for JS Prototyping
    void AddAssignStatement(const TString& type, const TString& att, TrecPointer<TVariable> value, ReturnObject& ret);
    TrecSubPointer<TVariable, TContainerVariable> ConstructObject(const TString& type);

    // Yield Support
    UINT yieldIndex;
    TrecPointer<CodeStatement> yieldStatement;

};

