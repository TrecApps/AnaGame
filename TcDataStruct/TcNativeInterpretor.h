#pragma once
#include <TcInterpretor.h>
#include "TcDataStruct.h"


typedef void (*NativeTFunction)(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret);

class TC_DATA_STRUCT TcNativeInterpretor :
    public TcInterpretor
{
public:
    /**
    * Method: TNativeInterpretor::TNativeInterpretor
    * Purpose: Constructor
    * Parameters: NativeFunction function - the function to call
    *              TrecPointer<TcInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
    *				TrecPointer<TEnvironment> env - the environment underwhich this function is operating
    * Returns: New TInterpretor Object
    */
    TcNativeInterpretor(NativeTFunction function, TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);

    /**
     * Method: TcNativeInterpretor::SetFile
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
     * Method: TcNativeInterpretor::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() override;


    /**
     * Method: TcNativeInterpretor::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) override;

    /**
     * Method: TcNativeInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: ReturnObject& ret - info about issues detected
     * Returns: void
     */
    virtual void PreProcess(ReturnObject& ret) override;


    /**
     * Method: TcNativeInterpretor::ProcessIndividualStatement
     * Purpose: Allows an Interpretor to process a statement
     * Parameters: const TString& statement - the statemnt to process
     *              ReturnObject& ret - offers return information
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void ProcessIndividualStatement(const TString& statement, ReturnObject& ret) override;

private:
    /**
     * Pointer to the native function to call
     */
    NativeTFunction nativeFunction;
};

