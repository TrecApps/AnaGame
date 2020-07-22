#pragma once
#include <TObject.h>
#include "TVariable.h"
#include <TMap.h>
#include <TFile.h>
    /**
     * Method: TInterpretor::
     * Purpose:
     * Parameters:
     * Returns: 
     */

/**
 * Class ReportObject
 * Purpose: allows the Interpretors to report the results of a run 
 */
class ReportObject
{
public:

    /**
     * Method: ReportObject::ReportObject
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New Report Object
     */
    ReportObject();


    /**
     * Method: ReportObject::ReportObject
     * Purpose: Copy Constructor
     * Parameters: ReportObject& obj - the object to copy
     * Returns: Copied Report Object
     */
    ReportObject(const ReportObject& obj);

    /**
     * Error code (0 means no error
     */
    UINT returnCode;

    /**
     * Message related to the error
     */
    TString errorMessage;

    /**
     * Holds a list of Strings indicating a stack trace
     */
    TDataArray<TString> stackTrace;

    /**
     * Variable that might have been explicitly "thrown" by the coder or some mechanism 
     */
    TrecPointer<TVariable> errorObject;

    // Error codes
    const static UINT no_error = 0;
    const static UINT broken_reference = 1;
    const static UINT not_number = 2;
    const static UINT too_few_params = 3;
};

/**
 * Class: TInterpretor
 * Purpose: Interface Class that represents a runnable program, most likely some source code
 */
class TInterpretor : public TObject
{
public:

    /**
     * Method: TInterpretor::TInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TInterpretor(TrecPointer<TInterpretor> parentInterpretor);

    /**
     * Method: TInterpretor::SetCode
     * Purpose: Sets the initial file of the code
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     * 
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     */
    virtual UINT SetCode(TFile&) = 0;


    /**
     * Method: TInterpretor::SetCode
     * Purpose:
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     */
    virtual UINT SetCode(TrecPointer<TFile> file, ULONG64 start, ULONG64 end);

    /**
     * Method: TInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     */
    virtual ReportObject Run() = 0;


    /**
     * Method: TInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide 
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     */
    virtual ReportObject Run(TDataArray<TVariable>& params) = 0;


    /**
     * Method: TInterpretor::GetVariable
     * Purpose: Retrieves the variable specified
     * Parameters: TString& varName - the name of the variable requested
     * Returns: TrecPointer<TVariable> - the variable requested (null if not found)
     */
    TrecPointer<TVariable> GetVariable(TString& varName);
    

protected:
    /**
     * The Interpretor that created this interpretor 
     */
    TrecPointer<TInterpretor> parent;

    /**
     * Variables declared in this interpretor's scope
     */
    TMap<TVariable> variables;

    /**
     * Parameter names (if this interpretor represents a specific function)
     */
    TDataArray<TString> paramNames;

    /**
     * The file to read from
     */
    TrecPointer<TFile> file;

    /**
     * The Starting and ending points for our file
     */
    ULONG64 start, end;
};

