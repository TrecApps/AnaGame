#pragma once
#include <TVariable.h>
#include <TcInterpretor.h>
#include "TIteratorVariable.h"
/**
 * Class: TFunctionGen
 * Purpose: Produces Function Generators
 * 
 * SuperClass: TVariable - so it can be held by interpretors and environments
 */
class TC_DATA_STRUCT TFunctionGen :
    public TVariable
{
public:

    /**
     * Method: TFunctionGen::TFunctionGen
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> func - the function to clone
     * Returns: new Function Generator
     * 
     * Note: Will throw if 'func' is null
     */
    TFunctionGen(TrecSubPointer<TVariable, TcInterpretor> func);

    /**
     * Method: TFunctionGen::Generate
     * Purpose: Generates a new Function based off of the provided parameters
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - parameters to provide the function
     * Returns: TrecSubPointer<TVariable, TcInterpretor> - a clone of the initial function
     */
    TrecSubPointer<TVariable, TcInterpretor> Generate(TDataArray<TrecPointer<TVariable>>& params);

    /**
 * Method: TFunctionGen::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 *
 * Attributes: override
 */
    virtual var_type GetVarType() override;


    /**
     * Method: TFunctionGen::GetObject
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
     * Method: TFunctionGen::GetObject
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
     * Method: TFunctionGen::Get4Value
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
     * Method: TFunctionGen::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TFunctionGen::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     *
     * Attributes: override
     */
    virtual UINT GetSize() override;


    /**
     * Method: TFunctionGen::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual UINT GetVType() override;

protected:
    /**
     * The function template being held
     */
    TrecSubPointer<TVariable, TcInterpretor> func;
};

