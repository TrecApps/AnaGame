#pragma once
#include <TObject.h>
#include <TVariable.h>
#include "TcDataStruct.h"
#include <TDataMap.h>
#include <TFile.h>
#include <TEnvironment.h>


    /**
     * Method: TInterpretor::
     * Purpose:
     * Parameters:
     * Returns: 
     */

/**
 * Union: doubleLong
 * Purpose: Holds either a double, unsigned or signed integer, all 8 bytes
 */
union doubleLong
{
    LONG64 s;
    ULONG64 u;
    double d;
};

/**
 * Enum Class: double_long
 * Purpose: used to keep track of the status of the doubleLong union
 */
enum class double_long
{
    dl_sign,  // Set to a signed integer
    dl_unsign,// Set to an unsigned integer
    dl_double,// Set to double
    dl_invalid // Not properly set
};

/**
 * Class: DoubleLong
 * Purpose: Aids interpretors in basic arithmetic byt hold values and keeping track of their types
 */
class TC_DATA_STRUCT DoubleLong
{
public:
    DoubleLong(ULONG64 val);
    DoubleLong(LONG64 val);
    DoubleLong(double val);
    DoubleLong();
    doubleLong value;
    double_long type;

    bool operator<(const DoubleLong& o);
    bool operator<=(const DoubleLong& o);
    bool operator>=(const DoubleLong& o);
    bool operator>(const DoubleLong& o);
    bool operator==(const DoubleLong& o);

    ULONG64 ToUnsignedLong()const;

    ULONG64 GetBitAnd(const DoubleLong& o);
    ULONG64 GetBitOr(const DoubleLong& o);
    ULONG64 GetBitXor(const DoubleLong& o);
};

typedef enum class report_mode
{
    report_mode_regular,
    report_mode_break,
    report_mode_continue,
    report_mode_return
}report_mode;

/**
 * Class: ReportObject
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

    /**
     * What happens when the Interpreter has to return due to a command such as return, break, or continue
     */
    report_mode mode;

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
    const static UINT mismatched_parehtnesis = 9;
};

/**
 * Class: TInterpretor
 * Purpose: Interface Class that represents a runnable program, most likely some source code
 * 
 * SuperClass: TVariable
 */
class TC_DATA_STRUCT TInterpretor : public TVariable
{
public:

    /**
     * Method: TInterpretor::UpdateVariable
     * Purpose: Updates an existing Variable
     * Parameters: const TString& name - the name to update
     *              TrecPointer<TVariable> value - value to update it with
     * Returns: UINT - error code (0 for no error, 1 for doesn't exist, 2 for value is immutable)
     */
    UINT UpdateVariable(const TString& name, TrecPointer<TVariable> value);

    /**
     * Method: TInterpretor::TInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);


    /**
     * Method: TInterpretor::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     * 
     * Attributes: override
     */
    virtual var_type GetVarType() override;

    /**
     * Method: TInterpretor::SetCode
     * Purpose: Sets the initial file of the code
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     * 
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     * 
     * Attributes: abstract
     */
    virtual UINT SetCode(TFile&) = 0;


    /**
     * Method: TInterpretor::SetCode
     * Purpose:
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     * 
     * Attributes: virtual
     */
    virtual UINT SetCode(TrecPointer<TFile> file, ULONG64 start, ULONG64 end);

    /**
     * Method: TInterpretor::CloseFile
     * Purpose: Closes the file so a second run can be possible
     * Parameters: void
     * Returns: void
     * 
     * Note: This should only be called on the top-level interpretor
     */
    void CloseFile();


    /**
     * Method: TInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     * 
     * Attributes: abstract
     */
    virtual ReportObject Run() = 0;


    /**
     * Method: TInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide 
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     * 
     * Attributes: abstract
     */
    virtual ReportObject Run(TDataArray<TrecPointer<TVariable>>& params) = 0;


    /**
     * Method: TInterpretor::GetVariable
     * Purpose: Retrieves the variable specified
     * Parameters: TString& varName - the name of the variable requested
	 *				bool& present - whether the variable was present or not (used to distinguish between 'null' and 'undefined')
     * Returns: TrecPointer<TVariable> - the variable requested (null if not found)
     */
    TrecPointer<TVariable> GetVariable(TString& varName, bool& present);

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
     * 
     * Attributes: override
     */
    virtual TrecObjectPointer GetObject() override;


    /**
     * Method: TInterpretor::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
     * 
     * Attributes: override
     */
    virtual TString GetString() override;

    /**
     * Method: TInterpretor::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
     * 
     * Attributes: override
     */
    virtual UINT Get4Value() override;



    /**
     * Method: TInterpretor::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     * 
     * Attributes: override
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TInterpretor::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     * 
     * Attributes: override
     */
    virtual UINT GetSize() override;


    /**
     * Method: TInterpretor::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     * 
     * Attributes: override
     */
    virtual UINT GetType() override;


    /**
     * Method: TVariable::SetSelf
     * Purpose: Allows the Variable to have a reference to itself
     * Parameters: TrecPointer<TVariable> - reference to assign
     * Returns: void
     * 
     * Attributes: override
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
    TDataMap<TVariableMarker> variables;

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

    /**
     *
     */
    TrecPointer<TEnvironment> environment;


    // Methods for easy arithmetic implementation


    /**
     * Method: TInterpretor::ProcessAddition
     * Purpose: Filters variables in preparation for performing an addition operation
     * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the first addend
     *              TrecPointer<TVariable> var2 - the second variable, representing the second addend
     * Returns: ReportObject - the result of performing the operation, including error information if it occurs
     * 
     * Attributes: virtual
     */
    virtual ReportObject ProcessAddition(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);

    /**
     * Method: TInterpretor::ProcessSubtraction
     * Purpose: Filters variables in preparation for performing a subtraction operation
     * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the minuend
     *              TrecPointer<TVariable> var2 - the second variable, representing the subtrahend
     * Returns: ReportObject - the result of performing the operation, including error information if it occurs
     * 
     * Attributes: virtual
     */
    virtual ReportObject ProcessSubtraction(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    /**
     * Method: TInterpretor::ProcessMultiplication
     * Purpose: Filters variables in preparation for performing a multiplication operation
     * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the first factor
     *              TrecPointer<TVariable> var2 - the second variable, representing the second factor
     * Returns: ReportObject - the result of performing the operation, including error information if it occurs
     * 
     * Attributes: virtual
     */
    virtual ReportObject ProcessMultiplication(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    /**
     * Method: TInterpretor::ProcessDivision
     * Purpose: Filters variables in preparation for performing a division operation
     * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the dividend
     *              TrecPointer<TVariable> var2 - the second variable, representing the divisor
     * Returns: ReportObject - the result of performing the operation, including error information if it occurs
     * 
     * Attributes: virtual
     */
    virtual ReportObject ProcessDivision(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    /**
     * Method: TInterpretor::ProcessModDivision
     * Purpose: Filters variables in preparation for performing a mod division operation
     * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the dividend
     *              TrecPointer<TVariable> var2 - the second variable, representing the divisor
     * Returns: ReportObject - the result of performing the operation, including error information if it occurs
     * 
     * Attributes: virtual
     */
    virtual ReportObject ProcessModDivision(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);
    /**
     * Method: TInterpretor::ProcessExponent
     * Purpose: Filters variables in preparation for performing an exponent operation
     * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the base
     *              TrecPointer<TVariable> var2 - the second variable, representing the power
     * Returns: ReportObject - the result of performing the operation, including error information if it occurs
     * 
     * Attributes: virtual
     */
    virtual ReportObject ProcessExponent(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2);

    /**
     * Method: TInterpretor::Add
     * Purpose: Helper method for the ProcessAddition method, performing the actual operation
     * Parameters: const DoubleLong& v1 - the first addend for the operation
     *              const DoubleLong& v2 - the second addend for the operation
     * Returns: DoubleLong - the result of the Addition operation
     * 
     * Attributes: virtual
     */
    virtual DoubleLong Add(const DoubleLong& v1, const DoubleLong& v2);
    /**
     * Method: TInterpretor::Subtract
     * Purpose: Helper method for the ProcessSubtraction method, performing the actual operation
     * Parameters: const DoubleLong& v1 - the minuend for the operation
     *              const DoubleLong& v2 - the subtrahend for the operation
     * Returns: DoubleLong - the result of the Subtraction operation
     * 
     * Attributes: virtual
     */
    virtual DoubleLong Subtract(const DoubleLong& v1, const DoubleLong& v2);
    /**
     * Method: TInterpretor::Multiply
     * Purpose: Helper method for the ProcessMultiplication method, performing the actual operation
     * Parameters: const DoubleLong& v1 - the first factor for the operation
     *              const DoubleLong& v2 - the second factor for the operation
     * Returns: DoubleLong - the result of the Multiply operation
     * 
     * Attributes: virtual
     */
    virtual DoubleLong Multiply(const DoubleLong& v1, const DoubleLong& v2);
    /**
     * Method: TInterpretor::Divide
     * Purpose: Helper method for the ProcessDivision method, performing the actual operation
     * Parameters: const DoubleLong& v1 - the dividend for the operation
     *              const DoubleLong& v2 - the divisor for the operation
     * Returns: DoubleLong - the result of the Division operation
     * 
     * Attributes: virtual
     */
    virtual DoubleLong Divide(const DoubleLong& v1, const DoubleLong& v2);
    /**
     * Method: TInterpretor::ModDivide
     * Purpose: Helper method for the ProcessModDivision method, performing the actual operation
     * Parameters: const DoubleLong& v1 - the dividend for the operation
     *              const DoubleLong& v2 - the divisor for the operation
     * Returns: DoubleLong - the result of the Mod Division operation
     * 
     * Attributes: virtual
     */
    virtual DoubleLong ModDivide(const DoubleLong& v1, const DoubleLong& v2);
    /**
     * Method: TInterpretor::Exponent
     * Purpose: Helper method for the ProcessExponent method, performing the actual operation
     * Parameters: const DoubleLong& v1 - the base value for the operation
     *              const DoubleLong& v2 - the power value for the operation
     * Returns: DoubleLong - the result of the Exponent operation
     * 
     * Attributes: virtual
     */
    virtual DoubleLong Exponent(const DoubleLong& v1, const DoubleLong& v2);

    /**
     * Method: TInterpretor::GetValueFromPrimitive
     * Purpose: Helper method for interpreting arithmetic operations by extracting the value from the variable
     * Parameters: TrecPointer<TVariable> var - the variable believed holding the primitive value
     * Returns: DoubleLong - the value held by the variable
     */
    virtual DoubleLong GetValueFromPrimitive(TrecPointer<TVariable> var);
    /**
     * Method: TInterpretor::GetStringFromPrimitive
     * Purpose:
     * Parameters: TrecPointer<TVariable> var - the variable believed to be capable of producing some string value
     * Returns: TString - string representation of the primitive variable
     */
    TString GetStringFromPrimitive(TrecPointer<TVariable> var);

};

