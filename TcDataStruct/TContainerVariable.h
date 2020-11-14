#pragma once
#include <TVariable.h>
#include "TcDataStruct.h"
#include <TMap.h>

/**
 * Enum Class: ContainerType
 * Purpose: Determine the type of container this object is, in order to determine the type of operations that can be donte on it
 */
typedef enum class ContainerType
{
    ct_json_obj,    // mapped, index, any type, (DICTIONARY for Python)
    ct_set,         // unindex, anytype
    ct_array,       // indexed, same type
    ct_tuple        // immutable, indexed

} ContainerType;


/**
 * Class: TContainerVariable
 * Purpose: Variable that represents a collection of variables
 * 
 * SuperClass: TVariable
 */
class TC_DATA_STRUCT TContainerVariable :
    public TVariable
{
public:

    /**
     * Method: TContainerVariable
     * Purpose: Constructor
     * Parameters: ContainerType type - specified the restrictions on this container
     * Returns: New Container Variable
     */
    TContainerVariable(ContainerType ct);

    /**
     * Method: TContainerVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     * 
     * Attributes: override
     */
    virtual var_type GetVarType() override;

    /**
     * Method: TContainerVariable::Initialize
     * Purpose: Allows initialization of the container, if the restriction is "immutable", then this is an opportunity to set the values before the immutability restriction kicks in
     * Parameters: TMap<TVariable> - the values to initialize with
     * Returns void
     */
    void Initialize(TMap<TVariable>& vars);

    /**
     * Method: TContainerVariable::GetValue
     * Purpose: Retrieves the value by Index, for containers acting as "indexed"
     * Parameters: UINT index - the index to search for
     *              bool& present - whether the index was found. This can be used by JavaScript to distinguish between NULL and UNDEFINED
     *              bool attemptKey = false - if index is out of bounds, attempt to find the value by Key of the string form of the index
     * Returns: TrecPointer<TVariable> - the value at the provided index, or null if the index was not found or if container is a "set"
     */
    TrecPointer<TVariable> GetValue(UINT index, bool& present, bool attemptKey = false);

    /**
     * Method: TContainerVariable::GetValue
     * Purpose: Retrieves the value by Key
     * Parameters: const TString& key - the key of the value to look for
     *              bool& present - whether the index was found. This can be used by JavaScript to distinguish between NULL and UNDEFINED
     * Returns: TrecPointer<TVariable> - the value at the provided index, or null if the index was not found
     */
    TrecPointer<TVariable> GetValue(const TString& key, bool& present);

    /**
     * Method: TContainerVariable::SetValue
     * Purpose: Sets the value of the given index
     * Parameters: int index - index to set
     *              TrecPointer<TVariable> value - the value to set
     *              bool allowKey = false - whether to use the key as a fall back or not
     * Returns: bool - whether the operation was PERMITTED or not (false if the type is a set (non-index) or a tuple (immutable))
     */
    bool SetValue(int index, TrecPointer<TVariable> value, bool allowKey = false);

    /**
     * Method: TContainerVariable::SetValue
     * Purpose: Sets the value of the given index
     * Parameters: TString& key - key of the value to set
     *              TrecPointer<TVariable> value - the value to set
     * Returns: bool - whether the operation was PERMITTED or not int index - index to set
     *              TrecPointer<TVariable> value - the value to set
     */
    bool SetValue(const TString& key, TrecPointer<TVariable> value);

    /**
     * Method: TContainerVariable::AppendValue
     * Purpose: Adds the value to the end of the container
     * Parameters: TrecPointer<TVariable> value - the value to append
     * Returns: bool - whether the operation was PERMITTED or not
     */
    bool AppendValue(TrecPointer<TVariable> value);

    /**
     * Method: TContainerVariable::RemoveByKey
     * Purpose:
     * Parameters: TString& key - Removes the Key from the location
     * Returns: bool - whether the operation was successful or not
     */
    bool RemoveByKey(TString& key);

    /**
     * Method: TContainerVariable::RemoveByValue
     * Purpose: Removes the value by value
     * Parameters: TrecPointer<TVariable> value
     * Returns: bool - whether the operation was successful or not
     */
    bool RemoveByValue(TrecPointer<TVariable> value);


    /**
     * Method: TContainerVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
     *
     * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
     * 
     * Attributes: override
     */
    virtual TrecObjectPointer GetObject()override;


    /**
     * Method: TContainerVariable::GetObject
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
     * Method: TContainerVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type
     * 
     * Attributes: override
     */
    virtual UINT Get4Value()override;



    /**
     * Method: TContainerVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     * 
     * Attributes: override
     */
    virtual ULONG64 Get8Value()override;


    /**
     * Method: TContainerVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     * 
     * Attributes: override
     */
    virtual UINT GetSize()override;


    /**
     * Method: TContainerVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     * 
     * Attributes: override
     */
    virtual UINT GetType()override;


    /**
     * Method: TContainerVariable::GetValueAt
     * Purpose: Retrieves Variables by index
     * Parameters: UINT index - the index to check
     * Returns: TrecPointer<TVariable> - the variable stored at the index (null if not available)
     */
    TrecPointer<TVariable> GetValueAt(UINT index);

 private:
    /**
     * The map holding the values
     */
     TMap<TVariable> values;

     /**
      * Restructions on the container
      */
     ContainerType type;
};

