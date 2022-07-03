#pragma once
#include "TcAnascriptInterpretor.h"


class TcPythonInterpretor :
    public TcAnascriptInterpretor
{
public:

    /**
     * Method: TcTypeInterpretor::TcTypeInterpretor
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env
     * Returns: New TcTypeInterpretor object
     */
    TcPythonInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);

    /**
     * Method: TcInterpretor::SetFile
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
     * Method: TcInterpretor::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() override;

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
     * Method: TcInterpretor::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) override;


    /**
     * Method: TcInterpretor::PreProcess
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
     * Attributes: override
     */
    virtual void ProcessIndividualStatement(const TString& statement, ReturnObject& ret) override;

protected:

    void ProcessIf(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, ReturnObject& ret);
    void ProcessWhile(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, ReturnObject& ret);
    void ProcessFor(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, ReturnObject& ret);
    void ProcessElse(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, ReturnObject& ret);


    void ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT index);
    void ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT index, UINT parenth, UINT square);



    bool IsTruthful(TrecPointer<TVariable>);
    void PrepSubInterpretor(TrecPointer<CodeStatement> statement);
    void PythonPreProcess(TrecPointer<CodeStatement>& statement, ReturnObject& ret);
    void PythonPreProcess(TDataArray<TrecPointer<CodeStatement>>& statements, bool indentationExpected, ReturnObject& ret);

    void PrintStatement(UINT indent);

    // Temporary Expression Processing
    void ProcessExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessArrayExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessVariableExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessFunctionExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessNumberExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessStringExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessProcedureCall(UINT& parenth, UINT& square, UINT& index, TrecPointer<TVariable> object,
        TrecPointer<TVariable> proc, TrecPointer<CodeStatement> statement, ReturnObject& ret, bool doAwait = false);


    // Block Support
    UINT yieldIndex;
    TrecPointer<CodeStatement> yieldStatement;
};

