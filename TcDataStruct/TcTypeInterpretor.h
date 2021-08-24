#pragma once
#include <TcInterpretor.h>
#include "TcDataStruct.h"
#include "TClassStruct.h"
#include "TAsyncVariable.h"

/**
 * Class: TcTypeInterpretor
 * Purpose: Adds Data type management support to the TcInterpretor infrastructure
 * 
 * SuperClass: TcInterpretor
 */
class TC_DATA_STRUCT TcTypeInterpretor :
    public TcInterpretor
{
public:
    /**
     * Method: TcTypeInterpretor::TcTypeInterpretor
     * Purpose: Constructor
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env
     * Returns: New TcTypeInterpretor object
     */
    TcTypeInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);
    /**
     * Method: TcTypeInterpretor::SubmitClassType
     * Purpose: Adds a type to be held by the Interpretor
     * Parameters: const TString& className - Name of the class to add
     *              TClassStruct& classStruct - detals on the class
     *              bool updating - whether this is an addition operation (false) or updating an existing class (true)
     * Returns: bool - whether operation was successful. If updating, then the type must already be registered. Otherwise, it must not
     */
    bool SubmitClassType(const TString& className, TClassStruct& classStruct, bool updating);
    /**
     * Method: TcTypeInterpretor::GetClass
     * Purpose: Retrieves an existing class
     * Parameters: const TString& className - the name of the class to seek
     *              TClassStruct& classStruct - holder of the class structure
     * Returns: bool - whether the class was actually found
     */
    bool GetClass(const TString& className, TClassStruct& classStruct);

    /**
     * Method: TcTypeInterpretor::PrepAsyncCall
     * Purpose: Returns an Async Variable that will run a copy of this function
     * Parameters: void
     * Returns: TrecPointer<TVariable> - The Asynce Variable that will run this function
     */
    virtual TrecPointer<TVariable> PrepAsyncCall();

    /**
     * Method: TcTypeInterpretor::PrepAsyncCall
     * Purpose: Whether this interpretor is meant to run asynchronously
     * Parameters: void
     * Returns: bool
     */
    bool IsAsync();
protected:
    /**
     * The list of Types held by the Interpretor
     */
    TDataMap<TClassStruct> classes;


    // Async functionality
    bool isAsync;
    TrecSubPointer<TVariable, TAsyncVariable> asyncVar;
};

