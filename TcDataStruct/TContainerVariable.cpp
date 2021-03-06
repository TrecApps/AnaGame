#include "pch.h"
#include "TContainerVariable.h"

TrecPointer<TVariable> TContainerVariable::Clone()
{
    ThreadLock();
    TrecSubPointer<TVariable, TContainerVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(type);

    for (UINT Rust = 0; Rust < values.count(); Rust++)
    {
        auto entry = values.GetEntryAt(Rust);

        if (!entry.Get())
            continue;
        if (entry->key.GetSize())
        {
            TrecPointer<TVariable> var;
            if (entry->object.Get())
            {
                // Don't clone if we have a circular reference, else we'll get "infinite" recursion
                if (entry->object.Get() == this)
                    var = TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
                else
                    var = entry->object->Clone();
            }
            ret->values.addEntry(entry->key, var);
        }
    }
    ThreadRelease();
    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}

/**
 * Method: TContainerVariable::Clear
 * Purpose: Empties the container
 * Parameters: void
 * Returns: void
 */
void TContainerVariable::Clear()
{
    values.clear();
}

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
 * Method: TContainerVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 */
var_type TContainerVariable::GetVarType()
{
    return var_type::collection;
}

/**
 * Method: TContainerVariable::Initialize
 * Purpose: Allows initialization of the container, if the restriction is "immutable", then this is an opportunity to set the values before the immutability restriction kicks in
 * Parameters: TMap<TVariable> - the values to initialize with
 * Returns void
 */
void TContainerVariable::Initialize(TMap<TVariable>& vars)
{
    ThreadLock();
    // If container is immutable and already initialized, return so we don't do anything
    if (type == ContainerType::ct_tuple && values.count())
    {
        ThreadRelease();
        return;
    }
    values.clear();

    for (UINT Rust = 0; Rust < vars.count(); Rust++)
    {
        auto value = vars.GetEntryAt(Rust);
        if(value.Get())
        values.addEntry(value->key, value->object);
    }
    ThreadRelease();
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
    ThreadLock();
    if (type == ContainerType::ct_set)
    {
        ThreadRelease();
        return TrecPointer<TVariable>();
    }
    if (index < values.count())
    {
        auto entry = values.GetEntryAt(index);
        if (entry.Get())
        {
            // First check to see if there is a number key at the given index. If there is and it does not align with the index requested, then assume that index is not actually set
            int testIndex = 0;
            if (!entry->key.ConvertToInt(testIndex) && testIndex != index)
                return TrecPointer<TVariable>();

            // here we got our value so return that
            present = true;
            ThreadRelease();
            return entry->object;
        }
    }

    if (attemptKey)
    {
        TString k;
        k.Format(L"%i", index);
        ThreadRelease();
        return GetValue(k, present);
    }
    ThreadRelease();
    return TrecPointer<TVariable>();
}

/**
 * Method: TContainerVariable::GetValue
 * Purpose: Retrieves the value by Key
 * Parameters: TString& key - the key of the value to look for
 *              bool& present - whether the index was found. This can be used by JavaScript to distinguish between NULL and UNDEFINED
 * Returns: TrecPointer<TVariable> - the value at the provided index, or null if the index was not found
 */
TrecPointer<TVariable> TContainerVariable::GetValue(const TString& key, bool& present)
{
    ThreadLock();
    auto ret = values.retrieveEntry(key);
    present = ret.Get() != nullptr;
    ThreadRelease();
    return ret;
    
}

TrecPointer<TVariable> TContainerVariable::GetValue(const TString& key, bool& present, const TString& super)
{
    ThreadLock();
    auto supers = super.split(L";");
    auto ret = GetValue(key, present);
    if (present)
    {
        ThreadRelease();
        return ret;
    }
    for (UINT Rust = 0; Rust < supers->Size(); Rust++)
    {
        ret = GetValue(supers->at(Rust), present);
        if (ret.Get() && ret->GetVarType() == var_type::collection)
        {
            ret = dynamic_cast<TContainerVariable*>(ret.Get())->GetValue(key, present);
            if (present)
            {
                ThreadRelease();
                return ret;
            }
        }
    }

    present = false;
    ThreadRelease();
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
    ThreadLock();
    switch (type)
    {
    case ContainerType::ct_set:
    case ContainerType::ct_tuple:
        ThreadRelease();
        return false;
    }

    if (index < values.count())
    {
        auto entry = values.GetEntryAt(index);
        if (entry.Get())
        {
            int testIndex = 0;
            if (!entry->key.ConvertToInt(testIndex) && testIndex != index)
            {
                // Add the entry at the end
                values.addEntry(entry->key, entry->object);

                entry->key.Set(L"");
                entry->object = value;

                ThreadRelease();
                return true;
            }
            else
            {
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
        ThreadRelease();
        return true;
    }

    ThreadRelease();
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
bool TContainerVariable::SetValue(const TString& key, TrecPointer<TVariable> value)
{
    ThreadLock();
    if (type == ContainerType::ct_tuple)
    {
        ThreadRelease();
        return false;
    }
    TString sKey(key);

    values.removeEntry(sKey);
    values.addEntry(key, value);
    ThreadRelease();
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
    ThreadLock();
    if (type == ContainerType::ct_tuple)
    {
        ThreadRelease();
        return false;
    }
    
    // Otherwise, set value to the index it would be
    TString strIndex;
    strIndex.Format(L"%d", values.count());
    values.addEntry(strIndex, value);
    ThreadRelease();
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
    ThreadLock();
    if (type == ContainerType::ct_tuple)
    {
        ThreadRelease();
        return false;
    }
    values.removeEntry(key);
    ThreadRelease();
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
 * Method: TContainerVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TrecObjectPointer TContainerVariable::GetObject()
{
    return TrecObjectPointer();
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
    ThreadLock();
    UINT ret = values.count();
    ThreadRelease();
    return ret;
}

/**
 * Method: TContainerVariable::GetVType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TContainerVariable::GetVType()
{
    return 0;
}

/**
 * Method: TContainerVariable::GetValueAt
 * Purpose: Retrieves Variables by index
 * Parameters: UINT index - the index to check
 * Returns: TrecPointer<TVariable> - the variable stored at the index (null if not available)
 */
TrecPointer<TVariable> TContainerVariable::GetValueAt(UINT index)
{
    ThreadLock();
    auto entry = values.GetEntryAt(index);

    auto ret = (!entry.Get()) ? TrecPointer<TVariable>() : entry->object;
    ThreadRelease();
    return ret;
}

bool TContainerVariable::GetValueAt(UINT index, TString& key, TrecPointer<TVariable>& value)
{
    ThreadLock();
    if (index >= values.count())
    {
        ThreadRelease();
        return false;
    }
    TrecPointer<tEntry<TVariable>> entry = values.GetEntryAt(index);
    if (entry.Get())
    {
        key.Set(entry->key);
        value = entry->object->Clone();
        ThreadRelease();
        return true;
    }
    ThreadRelease();
    return false;
}

TString TContainerVariable::GetTClassName()
{
    ThreadLock();
    TString ret(className);
    ThreadRelease();
    return ret;
}

bool TContainerVariable::SetClassName(const TString& name)
{
    ThreadLock();
    if (className.GetSize() || !name.GetSize())
    {
        ThreadRelease();
        return false;
    }
    className.Set(name);
    ThreadRelease();
    return true;
}


/**
 * Method: TContainerVariable::GetConteinerType
 * Purpose: Reports to interpretors the container type
 * Parameters: void
 * Return: Container_type - the type of container we're dealing with
 */
ContainerType TContainerVariable::GetContainerType()
{
    return this->type;
}