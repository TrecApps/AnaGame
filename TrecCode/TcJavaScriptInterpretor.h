#pragma once
#include <TcInterpretor.h>
#include <CodeStatement.h>

/**
 * Class: TcJavaScriptInterpretor
 * Purpose: Implements the TcInterpretor for the JavaScript Programming Language
 * 
 * SuperClass: TcInterpretor
 */
class TcJavaScriptInterpretor : public TcInterpretor
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
     ReturnObject Run(TDataArray<TrecPointer<CodeStatement>>& statement, UINT index);

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

    void ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret);            // Call when you need an expression analyzed
    void ProcessAssignmentStatement(TrecPointer<CodeStatement> statement, ReturnObject& ret);   // Call for const, let, and var statements
    void ProcessBasicFlow(TrecPointer<CodeStatement> statement, ReturnObject& ret);             // Call for if, else, and else if statements
    void ProcessWhile(TrecPointer<CodeStatement> statement, ReturnObject& ret);                 // Call For while and do...while
    void ProcessFor(TDataArray<TrecPointer<CodeStatement>>& statement, UINT index, ReturnObject& ret);                   // Call for for statements
    void ProcessTryCatchFinally(TrecPointer<CodeStatement> statement, ReturnObject& ret);       // Call for the Try/catch/finally blocks
    void ProcessSwitch(TrecPointer<CodeStatement> statement, ReturnObject& ret);                // Call for Switch Statements

    // Helper Methods for the Process-Methods
    bool IsTruthful(TrecPointer<TVariable> var);
};

