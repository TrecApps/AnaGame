#pragma once
#include <TcTypeInterpretor.h>
#include <TcExpression.h>
/**
 * Enum Class: 
 */


/**
 * Class: TcCodeObjectInterpretor
 * Purpose: Generic and customized Interpretor meant to provide support for loosly-typed languages where every variable is an Object in some way
 */
class TcCodeObjectInterpretor :
    public TcTypeInterpretor
{
protected:

    TrecPointer<TDataArray<TcOperatorGroup>> operatorGroup;

    TrecPointer<TDataArray<TDataArray<TString>>> operatorMap;

    TDataArray<TrecPointer<CodeStatement>> statements;

    


public:

    /**
     * Method: TcCodeObjectInterpretor::TcCodeObjectInterpretor
     * Purpose: Constructor for top level Interpretor
     * Parameters: TrecPointer<TVariable> language - information about the language
     *              TrecPointer<TEnvironment> - the environment to work with
     * Returns: New Interpretor
     */
    TcCodeObjectInterpretor(TrecPointer<TVariable> language, TrecPointer<TEnvironment> env);


    /**
     * Method: TcCodeObjectInterpretor::TcCodeObjectInterpretor
     * Purpose: Constructor for top level Interpretor
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> language - information about the language
     *              TrecPointer<TEnvironment> - the environment to work with
     * Returns: New Interpretor
     */
    TcCodeObjectInterpretor(TrecSubPointer<TVariable, TcInterpretor> language, TrecPointer<TEnvironment> env);

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
     * Method: TcInterpretor::Compile
     * Purpose: Attempts to Compile the Statements into more compact code according to the Specifications provided
     * Parameters: COMPILE_TYPE type - specifications for the code to be compiled
     *              ReturnObject& ret - allows method to report any error information encountered
     * Returns: void
     *
     * Attributes: override
     */
    virtual void Compile(COMPILE_TYPE type, ReturnObject& ret) override;

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

    virtual TrecPointer<TVariable> Clone() override;
};

