#pragma once

#include <TObject.h>
#include <TrecReference.h>
#include <TString.h>
/**
 * Class TVariable
 * Purpose: Represents a given variable in the system, abstract to support both Objects and raw data types
 */
class TVariable
{
public:
    /**
     * Method: TVariable::IsObject
     * Purpose: Reports whether the variable holds an object or not
     * Parameters: void
     * Returns: bool - whether the variable is an object or not
     */
    virtual bool IsObject() = 0;


    /**
     * Method: TVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TrecPointer<TObject> - The Object referered by the variable (or null if not an object)
     *
     * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
     */
    virtual TrecPointer<TObject> GetObject() = 0;

    /**
     * Method: TVariable::IsString
     * Purpose: Reports whether the variable holds a string or not
     * Parameters: void
     * Returns: bool - whether the variable is a string or not
     */
    virtual bool IsString() = 0;


    /**
     * Method: TVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
     */
    virtual TString GetString() = 0;

    /**
     * Method: TVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type
     */
    virtual UINT Get4Value() = 0;


    /**
     * Method: TVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type
     */
    virtual UINT Get4Value() = 0;


    /**
     * Method: TVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     */
    virtual ULONG64 Get8Value() = 0;


    /**
     * Method: TVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     */
    virtual UINT GetSize() = 0;


    /**
     * Method: TVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     */
    virtual UINT GetType() = 0;
};

namespace VarFunction
{
    /**
     * Function: VarFunction::IsTrue
     * Purpose: Reports to the interpretor whether the provided value is "true"
     * Parameters: TrecPointer<TVariable> var - the variable to analyze
     *              bool& result - whether the variable amounts to "true" or not (reliable if the return value is true)
     *              UCHAR def = 0 - token indicating what types to refer to as true or not (0 means only booleans are checked)
     * Returns: bool - whether or not the variable could be analyzed in a boolean fashion based off of the token provided
     * 
     * Note: if this function returns false, the interpretor should consider the line to be an error and handle it accordingly.
     */
    bool IsTrue(TrecPointer<TVariable> var, bool& result, UCHAR def = 0);

}