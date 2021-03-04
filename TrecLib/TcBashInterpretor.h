#pragma once
#include "TcInterpretor.h"

/**
 * Class: TcBashInterpretor
 * Purpose: Uses the TcInterpretor structure to allow Anagame to process Bash commands and files
 * 
 * SuperClass: TcInterpretor
 */
class TcBashInterpretor :
    public TcInterpretor
{

    /**
     * Method: TcBashInterpretorCanCompile
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
     * Method: TcBashInterpretorCompile
     * Purpose: Attempts to Compile the Statements into more compact code according to the Specifications provided
     * Parameters: COMPILE_TYPE type - specifications for the code to be compiled
     *              ReturnObject& ret - allows method to report any error information encountered
     * Returns: void
     *
     * Attributes: override
     */
    virtual void Compile(COMPILE_TYPE type, ReturnObject& ret) override;

    /**
     * Method: TcBashInterpretorSetFile
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
     * Method: TcBashInterpretorRun
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() override;

    /**
     * Method: TcBashInterpretorSetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) override;


    /**
     * Method: TcBashInterpretorPreProcess
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
     * Method: TcBashInterpretor::TcInterpretor
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     *              TrecPointer<TEnvironment> env - the environemnt this interpretor runs in
     * Returns: New TInterpretor Object
     */
    TcBashInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);


protected:
    /**
     * List of statements generated
     */
    TDataArray<TrecPointer<CodeStatement>> statements;
};

