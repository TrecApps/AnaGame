#pragma once
#include <TVariable.h>
#include "TcDataStruct.h"
#include <TStringEx.h>

/**
 * Class: TStringVariable
 * Purpose: Represents a Variable that holds a String
 * 
 * SuperClass: TVariable
 */
class TC_DATA_STRUCT TStringVariable :
    public TVariable
{
public:

    virtual TrecPointer<TVariable> Clone()override;

    /**
     * Method: TStringVariable::TStringVariable
     * Purpose: Constructor
     * Parameters: TString& string - the String tp hold
     * Returns New String Variable
     */
    TStringVariable(const TString& string);

    /**
     * Method: TStringVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents (in this case, the string)
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     * 
     * Attributes: override
     */
    virtual var_type GetVarType() override;


    /**
     * Method: TVariable::GetIterator
     * Purpose: for type that support it, retrieves an Iterator Variable of this Variable
     * Parameters: void
     * Return: TrecPointer<TVariable> - for some variable types, this would be null, but other variable types woudld support iteration
     *
     * Attributes: virtual
     */
    virtual TrecPointer<TVariable> GetIterator();

    /**
     * Method: TStringVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
     * 
     * Attributes: override
     */
    virtual TString GetString()override;

    /**
     * Method: TStringVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type
     * 
     * Attributes: override
     */
    virtual UINT Get4Value()override;


    /**
     * Method: TStringVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     * 
     * Attributes: override
     */
    virtual ULONG64 Get8Value()override;


    /**
     * Method: TStringVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     * 
     * Attributes: override
     */
    virtual UINT GetSize()override;


    /**
     * Method: TStringVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     * 
     * Attributes: override
     */
    virtual UINT GetVType()override;
private:
    /**
     * The String held by the variable
     */
    TStringEx string;
};

