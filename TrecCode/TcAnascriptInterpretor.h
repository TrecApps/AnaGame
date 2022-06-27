#pragma once
#include <TcInterpretor.h>
#include <TcTypeInterpretor.h>
#include <DefaultObjectOperator.h>
#include "TrecCode.h"



class _TREC_CODE_DLL AnascriptExpression2
{
public:
    AnascriptExpression2();
    AnascriptExpression2(const AnascriptExpression2& orig);
    AnascriptExpression2(const TString& name, TrecPointer<TVariable> value);

    void operator=(const AnascriptExpression2& orig);

    TString varName;
    TrecPointer<TVariable> value;
};

class TcAnascriptInterpretor :
    public TcTypeInterpretor
{
    friend class TcAnascriptInterpretor;
public:


    /**
     * Method: TcAnascriptInterpretor::TcAnascriptInterpretor
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> parentInterpretor - the parent Interpretor
     *              TrecPointer<TEnvironment> env - the environment in which this interpretor runs
     * Returns: New Anascript Interpretor
     */
    TcAnascriptInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);


    /**
     * Method: TcInterpreotr::CanCompile
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
     * Method: TcAnascriptInterpretor::Compile
     * Purpose: Attempts to Compile the Statements into more compact code according to the Specifications provided
     * Parameters: COMPILE_TYPE type - specifications for the code to be compiled
     *              ReturnObject& ret - allows method to report any error information encountered
     * Returns: void
     *
     * Attributes: override
     */
    virtual void Compile(COMPILE_TYPE type, ReturnObject& ret) override;

    /**
     * Method: TcAnascriptInterpretor::SetFile
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
     * Method: TcAnascriptInterpretor::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     *
     * Attributes: override
     */
    virtual ReturnObject Run() override;

    /**
     * Method: TcAnascriptInterpretor::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     *
     * Attributes: override
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) override;

    /**
     * Method: TcAnascriptInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: ReturnObject& ret - info about issues detected
     * Returns: void
     *
     * Attributes: override
     */
    virtual void PreProcess(ReturnObject& ret) override;

    /**
     * Method: TcAnascriptInterpretor::ProcessIndividualStatement
     * Purpose: Allows an Interpretor to process a statement
     * Parameters: const TString& statement - the statemnt to process
     *              ReturnObject& ret - offers return information
     * Returns: void
     *
     * Attributes: override
     */
    virtual void ProcessIndividualStatement(const TString& statement, ReturnObject& ret) override;

    static bool IsTruthful(TrecPointer<TVariable> var);

protected:

    void ProcessLet(TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessFunction(TrecPointer<CodeStatement> statement, ReturnObject& ret);
    bool ProcessIf(TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessFor(TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT startIndex);
    void ProcessDeclare(TrecPointer<CodeStatement> statement, ReturnObject& ret);
    

    void ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT& pStack, UINT& sStack, UINT& startIndex);
    

    /**
     * List of statements generated
     */
    TDataArray<TrecPointer<CodeStatement>> statements;

    /**
     * Whether the pre-processing has been done
     */
    bool preProcessed;

    /**
     * Refreshes the list
     */
    void PreProcess(ReturnObject& ret, TDataArray<TrecPointer<CodeStatement>>& statements, UINT stack);

    // Expression Processing
    void ProcessArrayExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessVariableExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessNumberExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    void ProcessStringExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret);
    // UINT ProcessPotentalArrowNotation(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement>& statement, ReturnObject& ret);

    UINT ProcessProcedureCall(UINT& parenth, UINT& square, UINT& index, TrecPointer<TVariable> object,
        TrecPointer<TVariable> proc, TrecPointer<CodeStatement> statement, ReturnObject& ret, bool doAwait = false);

    void ProcessFunctionExpression(TrecPointer<CodeStatement> statement, ReturnObject& obj);

    // Operator Handler Methods
    void HandlePreExpr(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleExponents(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleMultDiv(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleAddSub(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseShift(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleLogicalComparison(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleEquality(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseAnd(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseXor(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleBitwiseOr(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleLogicalAnd(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleLogicalOr(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleNullish(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleConditional(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    void HandleAssignment(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret);
    TrecSubPointer<TVariable, TContainerVariable> ConstructObject(const TString& type);


};

