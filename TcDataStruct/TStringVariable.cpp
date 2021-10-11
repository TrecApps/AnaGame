#include "pch.h"
#include "TStringVariable.h"
#include "TPrimitiveVariable.h"

class TStringVariableIterator : public TVariableIterator
{
protected:
    /**
     * The String to Analyse
     */
    TrecSubPointer<TVariable, TStringVariable> targetVar;

    /**
     * Whether it is in reverse or not
     */
    bool doReverse;

    /**
     * UINT current Index
     */
    UINT currentIndex;

public:
    
    TStringVariableIterator(TrecSubPointer<TVariable, TStringVariable> targetVar)
    {
        this->targetVar = targetVar;
        assert(targetVar.Get());
        doReverse = false;
        currentIndex = 0;
    }


    /**
     * Method: TVariableIterator::SetReverse
     * Purpose: enables iterators to go through backwards
     * Parameters: bool doReverse - if true, the iterator will now go in reverse (if supported)
     *              bool reset - if true, will point to the beginning or end of the target variable
     * Returns: bool - whether the operation was supported
     *
     * Attributes: abstract
     */
    virtual bool SetReverse(bool doReverse, bool reset)
    {
        this->doReverse = doReverse;
        if (reset)
        {
            currentIndex = doReverse ? targetVar->GetSize() - 1 : 0;
        }
        return true;
    }


    /**
     * Method: TVariableIterator::Traverse
     * Purpose: Goues through the variable
     * Parameters: UINT& currentIndex - the index of the retrieved variable
     *              TString& currentName - the name of the retrieved variable
     *              TrecPointer<TVariable>& value - the retireved variable
     * Returns: bool - whether the variable was retrieved
     *
     * Attributes: abstract
     */
    virtual bool Traverse(UINT& currentIndex, TString& currentName, TrecPointer<TVariable>& value)
    {
        if (this->currentIndex >= targetVar->GetSize())
            return false;

        currentName.Empty();
        currentIndex = this->currentIndex;
        value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(targetVar->GetString().GetAt(currentIndex));
        if (doReverse)
            currentIndex--;
        else
            currentIndex++;
        return true;
    }
};

TrecPointer<TVariable> TStringVariable::GetIterator()
{
    TrecPointer<TVariable> v = TrecPointerKey::GetTrecPointerFromSoft<TVariable>(vSelf);
    return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariableIterator>(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(v));
}

TrecPointer<TVariable> TStringVariable::Clone()
{
    return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(string);
}

/**
 * Method: TStringVariable::TStringVariable
 * Purpose: Constructor
 * Parameters: TString& string - the String tp hold
 * Returns New String Variable
 */

TStringVariable::TStringVariable(const TString& string)
{
    this->string.Set(string);
}

/**
 * Method: TStringVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents (in this case, the string)
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 */
var_type TStringVariable::GetVarType()
{
    return var_type::string;
}



/**
 * Method: TStringVariable::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TString TStringVariable::GetString()
{
    ThreadLock();
    TString ret(string);
    ThreadRelease();
    return ret;
}

/**
 * Method: TStringVariable::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type
 */
UINT TStringVariable::Get4Value()
{
    return 0;
}

/**
 * Method: TStringVariable::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 */
ULONG64 TStringVariable::Get8Value()
{
    return ULONG64();
}

/**
 * Method: TStringVariable::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 */
UINT TStringVariable::GetSize()
{
    ThreadLock();
    UINT ret = string.GetSize();
    ThreadRelease();
    return ret;
}

/**
 * Method: TStringVariable::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TStringVariable::GetVType()
{
    return 0;
}
