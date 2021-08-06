#pragma once
#include <TcInterpretor.h>
#include "TcDataStruct.h"
#include "TAsyncVariable.h"

/**
 * Class: TReturnInterpretor
 * Purpose: Very Basic Interpretor whose only goal is to return some Object, and an error
 */
class TC_DATA_STRUCT TReturnInterpretor :
    public TcInterpretor
{
public:

    /**
     * Method: TcInterpreotr::CanCompile
     * Purpose: Whether the Compiler is capable of compiling it's statements into a more compact form
     * Parameters: void
     * Returns: COMPILE_TYPE - marker indicating the Compilation the Interpretor can do
     *
     * Attributes: abstract
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
     * Attributes: abstract
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
     * Attributes: abstract
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
     * Method: TcInterpretor::TcInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TVariable> object - the Object to return upon being run
     *              TrecPointer<TEnvironment> env - the environment to run in
     *              bool success - whether the Function should report a success or not
     *              bool returnByCall - If true, then it returns the first parameter provided by the "SetInitialVariables" call, if false, then it returns the "object" parameter
     *              bool initiateSuperReturn - if true, then the caller should also return
     * Returns: New TInterpretor Object
     * 
     * Note: By default, the resulting function returns the value provided by "object". However, if you set "returnByCall" to true, "object" is ignored and what is returned is the first param provided when it is called
     */
    TReturnInterpretor(TrecPointer<TVariable> object, TrecPointer<TEnvironment> env, bool success, bool returnByCall = false, bool initiateSuperReturn = false);

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
     * Attributes: abstract
     */
    virtual void ProcessIndividualStatement(const TString& statement, ReturnObject& ret) override;

    /**
     * Method: TReturnInterpretor::SetAsyncHold
     * Purpose: An Async Variable to attach (which will then be updated with the provided variable upon being called)
     * Parameters: TrecSubPoiunter<TVariable, TAsyncVariable> sync - the async var to attach
     * Returns: void
     */
    void SetAsyncHold(TrecSubPointer<TVariable, TAsyncVariable> sync);

protected:

    TrecSubPointer<TVariable, TAsyncVariable> asyncVar;

    /**
     * The var to return upon calling
     */
    TrecPointer<TVariable> var;

    /**
     * Whether it is being "returned" (true) or "thrown" (false)
     */
    bool success, returnByCall, superReturn;
};

