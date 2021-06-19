#pragma once
#include <TVariable.h>
#include "TcDataStruct.h"
#include <TcInterpretor.h>

/**
 * Enum Class: async_mode
 * Purpose: Helps keep track of the Mode
 */
typedef enum class async_mode
{
    m_waiting,      // Waiting for the first response
    m_progress,     // Waiting for final response
    m_complete,     // Operation is complete
    m_error         // Operation completed with an error
}async_mode;


/**
 * Class: TAsyncVariable
 * Purpose: Allows variables to be shared across threads
 * 
 * SuperClass: TVariable - ca be used as a variable itself
 */
class TC_DATA_STRUCT TAsyncVariable :
    public TVariable
{
public:
    virtual TrecPointer<TVariable> Clone();


    /**
     * Method: TAsyncVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: abstract
     */
    virtual var_type GetVarType() override;


    /**
     * Method: TAsyncVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
     *
     * Attributes: abstract
     */
    virtual TrecObjectPointer GetObject() override;


    /**
     * Method: TAsyncVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
     *
     * Attributes: abstract
     */
    virtual TString GetString() override;

    /**
     * Method: TAsyncVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
     *
     * Attributes: abstract
     */
    virtual UINT Get4Value() override;



    /**
     * Method: TAsyncVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: abstract
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TAsyncVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     *
     * Attributes: abstract
     */
    virtual UINT GetSize() override;


    /**
     * Method: TAsyncVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: abstract
     */
    virtual UINT GetVType() override;

    // Async Specific Methods

    /**
     * Method: TAsyncVariable::TAsyncVariable
     * Purpose: Constructor
     * Parameters: DWORD thread - id of thread calling
     *              TrecPointer<TVariable> var - the variable to hold
     * Returns: New Async Object
     */
    TAsyncVariable(DWORD thread, TrecSubPointer<TVariable, TcInterpretor> var);

    /**
     * Method: TAsyncVariable::GetSetMode
     * Purpose: Gets or sets the mode
     * Parameters: async_mode& m - the status of the object
     *              bool get - if true, m is set to the current status, false, the status is set to m
     * Returns: void
     */
    void GetSetMode(async_mode& m, bool get = true);

    /**
     * Method: TAsyncVariable::GetProgress
     * Purpose: retrieves the Progress
     * Parameters: void
     * Returns: UINT - the number of successes
     */
    UINT GetProgress();


    /**
     * Method: TAsyncVriable::AppendSuccessResponse
     * Purpose: Adds a success Response to the Object
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> func - the function to add
     *              TrecSubPointer<TVariable, TcInterpretor> eFunc - the function to call in case func returns an error
     * Returns: void
     */
    void AppendSuccessResponse(TrecSubPointer<TVariable, TcInterpretor> func, TrecSubPointer<TVariable, TcInterpretor> eFunc);


    /**
     * Method: TAsyncVriable::AppendSuccessResponse
     * Purpose: Adds a success Response to the Object
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> func - the function to add
     * Returns: void
     */
    void AppendSuccessResponse(TrecSubPointer<TVariable, TcInterpretor> func);


    /**
     * Method: TAsyncVriable::SetFinally
     * Purpose: Adds a function to run at the end
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> func - the function to add
     * Returns: void
     */
    void SetFinally(TrecSubPointer<TVariable, TcInterpretor> func);

    /**
     * Method: TAsyncVariable::SetErrorResponse
     * Purpose: Adds a main Error Response
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> func - the function to add
     * Returns: void
     * 
     * Note: This method just sets the main response that is called when the main function or any response function is called
     *  To set an error response to a specific successResponse, pass it in as the 'eFunc' parameter to the 'AppendSuccessResponse'
     */
    void SetErrorResponse(TrecSubPointer<TVariable, TcInterpretor> func);


    /**
     * Method: TAsyncVariable::SetErrorResponse
     * Purpose: Adds a main Error Response
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> func - the function to add
     * Returns: void
     */
    void SetErrorResponse(TrecSubPointer<TVariable, TcInterpretor> func, UINT c);

    /**
     * Method: TAsyncVariable::RunAsyncObject
     * Purpose: Runs the object and follows up on the Response
     * Parameters: TrecSubPointer<TVariable, TAsyncVariable> asyncVar - the async object to run
     *              ReturnObject& ret - info about the result of the run
     * Returns: void
     * 
     * Attributes: static
     */
    static void RunAsyncObject(TrecSubPointer<TVariable, TAsyncVariable> asyncVar, ReturnObject& ret);

    /**
     * Method: TAsyncVariable::GetCallingThread
     * Purpose: Retrieves the thread tht created this object
     * Parameters: void
     * Returns: DWORD - the thread that created the Object
     */
    DWORD GetCallingThread();

    /**
     * Method: TAsyncVariable::GetResolveSize
     * Purpose: Reports how many resolve functions are included
     * Parameters: void
     * Returns: UINT - number of resolve functions in the stack
     */
    UINT GetResolveSize();

    // Variables
protected:

    /**
     * The underlying variable to manipulate, and run at the end
     */
    TrecSubPointer<TVariable, TcInterpretor> mainFunction, finallyFunction;

    /**
     * Successful responses
     */
    TDataArray<TrecPointer<TVariable>> successResolve;

    /**
     * Error Response
     */
    TrecPointer<TVariable> mainErrorResolve, subErrorResolve;

    /**
     * Thread id of the requester
     */
    DWORD requesterId;

    /**
     * The corrent Mode of the async object
     */
    async_mode mode;

    /**
     * Number of successes
     */
    UINT progress;

    /**
     * Result Information
     */
    ReturnObject ret;

};


void TC_DATA_STRUCT ProcessTAsyncObject(TrecSubPointer<TVariable, TAsyncVariable> asyncVar);