#include "pch.h"
#include "TContainerVariable.h"

/**
 * Method: TContainerVariable
 * Purpose: Constructor
 * Parameters: ContainerType type - specified the restrictions on this container
 * Returns: New Container Variable
 */
TContainerVariable::TContainerVariable(ContainerType ct)
{
    type = ct;
}

/**
 * Method: TContainerVariable::Initialize
 * Purpose: Allows initialization of the container, if the restriction is "immutable", then this is an opportunity to set the values before the immutability restriction kicks in
 * Parameters: TMap<TVariable> - the values to initialize with
 * Returns void
 */
void TContainerVariable::Initialize(TMap<TVariable>& vars)
{
    // If container is immutable and already initialized, return so we don't do anything
    if (type == ContainerType::ct_tuple && values.count())
        return;

    values.clear();

    for (UINT Rust = 0; Rust < vars.count(); Rust++)
    {
        auto value = vars.GetEntryAt(Rust);
        if(value.Get())
        values.addEntry(value->key, value->object);
    }
}

/**
 * Method: TContainerVariable::GetValue
 * Purpose: Retrieves the value by Index, for containers acting as "indexed"
 * Parameters: UINT index - the index to search for
 *              bool& present - whether the index was found. This can be used by JavaScript to distinguish between NULL and UNDEFINED
 *              bool attemptKey = false - if index is out of bounds, attempt to find the value by Key of the string form of the index
 * Returns: TrecPointer<TVariable> - the value at the provided index, or null if the index was not found or if container is a "set"
 */
TrecPointer<TVariable> TContainerVariable::GetValue(UINT index, bool& present, bool attemptKey)
{
    present = false;
    if(type == ContainerType::ct_set)
        return TrecPointer<TVariable>();
    
    if (index < values.count())
    {
        auto entry = values.GetEntryAt(index);
        if (entry.Get())
        {
            // First check to see if there is a number key at the given index. If there is and it does not align with the index requested, then assume that index is not actually set
            int testIndex = 0;
            if (!entry->key.ConvertToInt(&testIndex) && testIndex != index)
                return TrecPointer<TVariable>();

            // here we got our value so return that
            present = true;
            return entry->object;
        }
    }

    if (attemptKey)
    {
        TString k;
        k.Format(L"%i", index);
        return GetValue(k, present);
    }
    return TrecPointer<TVariable>();
}

/**
 * Method: TContainerVariable::GetValue
 * Purpose: Retrieves the value by Key
 * Parameters: TString& key - the key of the value to look for
 *              bool& present - whether the index was found. This can be used by JavaScript to distinguish between NULL and UNDEFINED
 * Returns: TrecPointer<TVariable> - the value at the provided index, or null if the index was not found
 */
TrecPointer<TVariable> TContainerVariable::GetValue(TString& key, bool& present)
{
    auto ret = values.retrieveEntry(key);
    present = ret.Get() != nullptr;
    return ret;
    
}

/**
 * Method: TContainerVariable::SetValue
 * Purpose: Sets the value of the given index
 * Parameters: int index - index to set
 *              TrecPointer<TVariable> value - the value to set
 *              bool allowKey = false - whether to use the key as a fall back or not
 * Returns: bool - whether the operation was PERMITTED or not (false if the type is a set (non-index) or a tuple (immutable))
 */
bool TContainerVariable::SetValue(int index, TrecPointer<TVariable> value, bool allowKey)
{
    switch (type)
    {
    case ContainerType::ct_set:
    case ContainerType::ct_tuple:
        return false;
    }

    if (index < values.count())
    {
        auto entry = values.GetEntryAt(index);
        if (entry.Get())
        {
            int testIndex = 0;
            if (!entry->key.ConvertToInt(&testIndex) && testIndex != index)
            {
                // Add the entry at the end
                values.addEntry(entry->key, entry->object);

                entry->key.Set(L"");
                entry->object = value;

                return true;
            }
        }
    }

    if (allowKey)
    {
        TString k;
        k.Format(L"%i", index);
        values.removeEntry(k);
        values.addEntry(k, value);
        return true;
    }

    return false;
}

/**
 * Method: TContainerVariable::SetValue
 * Purpose: Sets the value of the given index
 * Parameters: TString& key - key of the value to set
 *              TrecPointer<TVariable> value - the value to set
 * Returns: bool - whether the operation was PERMITTED or not int index - index to set
 *              TrecPointer<TVariable> value - the value to set
 */
bool TContainerVariable::SetValue(TString& key, TrecPointer<TVariable> value)
{
    if(type == ContainerType::ct_tuple)
        return false;

    values.removeEntry(key);
    values.addEntry(key, value);
    return true;
}

/**
 * Method: TContainerVariable::AppendValue
 * Purpose: Adds the value to the end of the container
 * Parameters: TrecPointer<TVariable> value - the value to append
 * Returns: bool - whether the operation was PERMITTED or not
 */
bool TContainerVariable::AppendValue(TrecPointer<TVariable> value)
{
    if (type == ContainerType::ct_tuple)
        return false;

    values.addEntry(L"", value);

    return true;
}

/**
 * Method: TContainerVariable::RemoveByKey
 * Purpose:
 * Parameters: TString& key - Removes the Key from the location
 * Returns: bool - whether the operation was successful or not
 */
bool TContainerVariable::RemoveByKey(TString& key)
{
    if (type == ContainerType::ct_tuple)
        return false;

    values.removeEntry(key);

    return false;
}

/**
 * Method: TContainerVariable::RemoveByValue
 * Purpose: Removes the value by value
 * Parameters: TrecPointer<TVariable> value
 * Returns: bool - whether the operation was successful or not
 */
bool TContainerVariable::RemoveByValue(TrecPointer<TVariable> value)
{
    return false;
}

/**
 * Method: TContainerVariable::IsObject
 * Purpose: Reports whether the variable holds an object or not
 * Parameters: void
 * Returns: bool - whether the variable is an object or not
 */
bool TContainerVariable::IsObject()
{
    return false;
}

/**
 * Method: TContainerVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecPointer<TObject> - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TrecPointer<TObject> TContainerVariable::GetObject()
{
    return TrecPointer<TObject>();
}

/**
 * Method: TContainerVariable::IsString
 * Purpose: Reports whether the variable holds a string or not
 * Parameters: void
 * Returns: bool - whether the variable is a string or not
 */
bool TContainerVariable::IsString()
{
    return false;
}

/**
 * Method: TContainerVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TString TContainerVariable::GetString()
{
    return TString();
}

/**
 * Method: TContainerVariable::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type
 */
UINT TContainerVariable::Get4Value()
{
    return 0;
}

/**
 * Method: TContainerVariable::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 */
ULONG64 TContainerVariable::Get8Value()
{
    return ULONG64();
}

/**
 * Method: TContainerVariable::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 */
UINT TContainerVariable::GetSize()
{
    return values.count();
}

/**
 * Method: TContainerVariable::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TContainerVariable::GetType()
{
    return 0;
}
