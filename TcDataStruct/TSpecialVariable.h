#pragma once
#include <TVariable.h>
#include "TcDataStruct.h"
/**
 * Enum Class: SpecialVar
 * Purpose: Represents which special value
 */
typedef enum class SpecialVar
{
	sp_undefined,	// Undefined
	sp_nan			// NaN (JavaScript's Not a Number)
}SpecialVar;

/**
 * Class: TSpecialVariable
 * Purpose: Represents a special Variable that doesn't fit in with the other 
 * 
 * SuperClass: TVariable
 */
class TC_DATA_STRUCT TSpecialVariable : public TVariable
{
    friend class TrecPointerKey;
public:
    /**
     * Method: TSpecialVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: override
     */
    virtual var_type GetVarType() override;



    /**
     * Method: TSpecialVariable::GetObject
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
     * Method: TSpecialVariable::Get4Value
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
     * Method: TSpecialVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TSpecialVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     *
     * Attributes: override
     */
    virtual UINT GetSize() override;


    /**
     * Method: TSpecialVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual UINT GetVType() override;

    /**
     * Method: TSpecialVariable::GetSpecialVariable
     * Purpose: Retrieves a special variable with the provided value type
     * Parameters: SpecialVar - the Variable present
     * Returns: TrecPointer<TVariable> - the variable representing the specified special variable
     * 
     * Attributes: static
     */
    static TrecPointer<TVariable> GetSpecialVariable(SpecialVar sv);

    /**
     * Method: TSpecialVariable::GetSpecialValue
     * Purpose: Retrieves the acual value held by this variable
     * Parameters: void
     * Returns: SpecialVar - the value held
     */
    SpecialVar GetSpecialValue();

protected:

    /**
     * Method: TSpecialVariable::TSpecialVariable
     * Purpose: Constructor
     * Parameters: SpecialVar - the variable to represent
     * Returns: New Pecial Variable Object
     */
    TSpecialVariable(SpecialVar sp);

    SpecialVar specialVar;

};

