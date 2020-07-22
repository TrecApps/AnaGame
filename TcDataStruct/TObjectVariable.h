#pragma once
#include "TVariable.h"
/**
 * Class TObjectVariable
 * Purpose: Represents an Object variable, refering to an actual Native Anagame Object
 */
class TObjectVariable :
    public TVariable
{
    /**
     * Method: TObjectVariable::TObjectVariable
     * Purpose: Constructor
     * Parameters: TrecPointer<TObject> obj - the object to hold
     * Returns: New TObject Variable
     */
    TObjectVariable(TrecPointer<TObject> obj);

    /**
     * Method: TObjectVariable::Set
     * Purpose: Sets up the object
     * Parameters: TrecPointer<TObject> obj - the object to hold
     * Returns: void
     */
    void Set(TrecPointer<TObject> obj);


    /**
     * Method: TObjectVariable::IsObject
     * Purpose: Reports whether the variable holds an object or not
     * Parameters: void
     * Returns: bool - whether the variable is an object or not
     */
    virtual bool IsObject()override;


    /**
     * Method: TObjectVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TrecPointer<TObject> - The Object referered by the variable (or null if not an object)
     *
     * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
     */
    virtual TrecPointer<TObject> GetObject()override;

    /**
     * Method: TObjectVariable::IsString
     * Purpose: Reports whether the variable holds a string or not
     * Parameters: void
     * Returns: bool - whether the variable is a string or not
     */
    virtual bool IsString()override;


    /**
     * Method: TObjectVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
     */
    virtual TString GetString()override;

    /**
     * Method: TObjectVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type
     */
    virtual UINT Get4Value()override;


    /**
     * Method: TObjectVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     */
    virtual ULONG64 Get8Value()override;


    /**
     * Method: TObjectVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     */
    virtual UINT GetSize()override;


    /**
     * Method: TObjectVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     */
    virtual UINT GetType()override;

private:
    /**
     * The Object held
     */
    TrecPointer<TObject> object;
};

