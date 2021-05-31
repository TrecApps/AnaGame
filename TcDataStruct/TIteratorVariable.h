#pragma once
#include <TVariable.h>
#include "TContainerVariable.h"
#include <TcInterpretor.h>
#include "TPrimitiveVariable.h"

/**
 * Class: TIteratorVariable
 * Purpose: Supplies functionality related to iterators
 * 
 * SuperClass: TVariable - enables 
 */
class TC_DATA_STRUCT TIteratorVariable :
    public TVariable
{
public:
    /**
     * Method: TIteratorVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: abstract
     */
    virtual var_type GetVarType() override;


    /**
     * Method: TIteratorVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
     *
     * Attributes: abstract
     */
    virtual TrecObjectPointer GetObject() override;


    /**
     * Method: TIteratorVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
     *
     * Attributes: abstract
     */
    virtual TString GetString() override;

    /**
     * Method: TIteratorVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
     *
     * Attributes: abstract
     */
    virtual UINT Get4Value() override;



    /**
     * Method: TIteratorVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: abstract
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TIteratorVariable::GetSize
     * Purpose: Returns the estimated size of the value held, for iterators, this would be the number of elements in their collection (or 1, if a function)
     * Parameters: void
     * Returns: UINT - The estimated size in the collection (or 1 for function)
     *
     * Attributes: abstract
     */
    virtual UINT GetSize() override;


    /**
     * Method: TIteratorVariable::GetVType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: abstract
     */
    virtual UINT GetVType() override;

    /**
     * Method: TIteratorVariable::TIteratorVariable
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TContainerVariable> coll - the colection to traverse
     * Returns: New Iterator
     */
    TIteratorVariable(TrecSubPointer<TVariable, TContainerVariable> coll);


    /**
     * Method: TIteratorVariable::TIteratorVariable
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TContainerVariable> coll - the colection to traverse
     * Returns: New Iterator
     */
    TIteratorVariable(TrecSubPointer<TVariable, TcInterpretor> coll);

    /**
     * Method: TIteratorVariable::Traverse
     * Purpose: Moves the Iterator, and returns the resulting value
     * Parameters: int moves - how much to traverse by (typically 1 for next and -1 for prev)
     *              UINT attributes - markers for the attributes to retrieve
     * Returns: TrecPointer<TVariable> - the value at the new Location
     * 
     * Note: If the iterator is attached to a function (i.e. a TcInterpretor, then the "moves" parameter is ignored)
     * 
     * Note: if attributes is 0, then the return value will just be the actial value in the collection. Otherwise, a small collection is
     *  returned, based off of the bits active. 
     * 0x0001 - the actual "value" is included
     * 0x0002 - the "index" is included
     * oxooo4 - a "done" attribute is included
     * 0x0008 - [Reserved]
     * 
     * 0x000F - if running a function, whether to catch any excpetion (if set, then yes! Otherwise, Exception Intormation will be passed to the caller)
     * 
     */
    TrecPointer<TVariable> Traverse(int moves, UINT attributes, ReturnObject& ret);

protected:

    /**
     * Container Variable to Point to
     */
    TrecSubPointer<TVariable, TContainerVariable> collection;

    /**
     * Function to call upon (traverse)
     */
    TrecSubPointer<TVariable, TcInterpretor> function;

    /**
     * Whether the function is "done"
     */
    bool fDone;

    /**
     * Iterators current location on the Collection
     */
    int index;
};

