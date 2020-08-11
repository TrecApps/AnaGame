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

union doubleLong
{
    LONG64 s;
    ULONG64 u;
    double d;
};

enum class double_long
{
    dl_sign,
    dl_unsign,
    dl_double
};

struct DoubleLong
{
    doubleLong value;
    double_long type;
};

/**
 * Class ReportObject
 * Purpose: allows the Interpretors to report the results of a run 
 */
class TC_DATA_STRUCT ReportObject
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
    const static UINT incomplete_statement = 4;
    const static UINT invalid_name = 5;
    const static UINT existing_var = 6;
    const static UINT incomplete_block = 7;
    const static UINT improper_type = 8;
};

/**
 * Class: TInterpretor
 * Purpose: Interface Class that represents a runnable program, most likely some source code
 */
class TC_DATA_STRUCT TInterpretor : public TVariable
{
public:

    /**
     * Method: TInterpretor::TInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor);


    /**
     * Method: TInterpretor::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     */
    virtual var_type GetVarType() override;

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
    virtual ReportObject Run(TDataArray<TrecPointer<TVariable>>& params) = 0;


    /**
     * Method: TInterpretor::GetVariable
     * Purpose: Retrieves the variable specified
     * Parameters: TString& varName - the name of the variable requested
     * Returns: TrecPointer<TVariable> - the variable requested (null if not found)
     */
    TrecPointer<TVariable> GetVariable(TString& varName);

    /**
     * Method: TInterretor::SetParamNames
     * Purpose: Sets the names of parameters
     * Parameters: TDataArray<TString>& names - list of names for the parameters
     * Returns: void
     */
    void SetParamNames(TDataArray<TString>& names);
    

    /**
     * Method: TInterpretor::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
     */
    virtual TrecObjectPointer GetObject() override;


    /**
     * Method: TInterpretor::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
     */
    virtual TString GetString() override;

    /**
     * Method: TInterpretor::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
     */
    virtual UINT Get4Value() override;



    /**
     * Method: TInterpretor::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TInterpretor::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     */
    virtual UINT GetSize() override;


    /**
     * Method: TInterpretor::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     */
    virtual UINT GetType() override;


    /**
     * Method: TVariable::SetSelf
     * Purpose: Allows the Variable to have a reference to itself
     * Parameters: TrecPointer<TVariable> - reference to assign
     * Returns: void
     */
    virtual void SetSelf(TrecPointer<TVariable>) override;

    /**
     * Method: TInterpretor::CheckVarName
     * Purpose: Method Interpreters can use to inspect the variable name to make sure it is valid
     * Parameters: TString& varname - the variable name to check
     *              ReportObject& ro - the Object to modify based off of the findings
     * Returns: void
     */
    void CheckVarName(TString& varname, ReportObject& ro, UINT line);

protected:
    /**
     * The Interpretor that created this interpretor 
     */
    TrecSubPointer<TVariable, TInterpretor> parent;

    /**
     * Self Reference
     */
    TrecSubPointerSoft<TVariable, TInterpretor> self;

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


    // Methods for easy arithmetic implementation

    virtual ReportObject ProcessAddition(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);

    virtual ReportObject ProcessSubtraction(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    virtual ReportObject ProcessMultiplication(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    virtual ReportObject ProcessDivision(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    virtual ReportObject ProcessModDivision(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    virtual ReportObject ProcessExponent(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);

    DoubleLong Add(const DoubleLong& v1, const DoubleLong& v2);
    DoubleLong Subtract(const DoubleLong& v1, const DoubleLong& v2);
    DoubleLong Multiply(const DoubleLong& v1, const DoubleLong& v2);
    DoubleLong Divide(const DoubleLong& v1, const DoubleLong& v2);
    DoubleLong ModDivide(const DoubleLong& v1, const DoubleLong& v2);
    DoubleLong Exponent(const DoubleLong& v1, const DoubleLong& v2);

    DoubleLong GetValueFromPrimitive(TrecPointer<TVariable> var);
    TString GetStringFromPrimitive(TrecPointer<TVariable> var);

};

