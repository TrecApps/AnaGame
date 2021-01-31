#pragma once
#include "TInterpretor.h"

/**
 * Class: TAcessorVariable
 * Purpose: Holds Getter and Setter Methods for a variable
 * 
 * SuperClass: TVariable
 */
class TC_DATA_STRUCT TAccessorVariable : public TVariable
{
    virtual TrecPointer<TVariable> Clone();

    /**
     * Method: TAccessorVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: override
     */
    virtual var_type GetVarType() override;


    /**
     * Method: TAccessorVariable::GetObject
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
     * Method: TAccessorVariable::GetObject
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
     * Method: TAccessorVariable::Get4Value
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
     * Method: TAccessorVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TAccessorVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     *
     * Attributes: override
     */
    virtual UINT GetSize() override;


    /**
     * Method: TAccessorVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual UINT GetType() override;

    /**
     * Method: TAccessorVariable::TAccessorVariable
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New Accessor Object
     */
    TAccessorVariable();

    /**
     * Method: TAccessorVariable::GetGetter
     * Purpose: Retireve the Getter held by this variable
     * Parameters: void
     * Returns: TrecSubPointer<TVariable, TInterpretor> - the interpretor representing a getter
     */
    TrecSubPointer<TVariable, TInterpretor> GetGetter();

    /**
     * Method: TAccessorVariable::SetGetter
     * Purpose: Retireve the Getter held by this variable
     * Parameters: TrecSubPointer<TVariable, TInterpretor> g - the interpretor representing a getter
     * Returns: void
     */
    void SetGetter(TrecSubPointer<TVariable, TInterpretor> g);

    /**
     * Method: TAccessorVariable::GetSetter
     * Purpose: Retireve the Setter held by this variable
     * Parameters: void
     * Returns: TrecSubPointer<TVariable, TInterpretor> - the interpretor representing a setter
     */
    TrecSubPointer<TVariable, TInterpretor> GetSetter();

    /**
     * Method: TAccessorVariable::SetSetter
     * Purpose: Retireve the Getter held by this variable
     * Parameters: TrecSubPointer<TVariable, TInterpretor> s - the interpretor representing a setter
     * Returns: void
     */
    void SetSetter(TrecSubPointer<TVariable, TInterpretor> s);

private:
    TrecSubPointer<TVariable, TInterpretor> getter, setter;
};

