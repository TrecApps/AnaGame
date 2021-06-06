#pragma once
#include <TVariable.h>
#include "TcDataStruct.h"

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
    TAsyncVariable(DWORD thread, TrecPointer<TVariable> var);

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
     * Method: TAsyncVariable::GetVariable
     * Purpose: Retrieves the underlying variable
     * Parameters: void
     * Returns: TrecPointer<TVariable> - the variable being held
     */
    TrecPointer<TVariable> GetVariable();


    // Variables
protected:

    /**
     * The underlying variable to manipulate
     */
    TrecPointer<TVariable> var;

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

};

