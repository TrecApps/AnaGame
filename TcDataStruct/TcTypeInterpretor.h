#pragma once
#include <TcInterpretor.h>
#include "TcDataStruct.h"
#include "TClassStruct.h"
#include "TAsyncVariable.h"
class TC_DATA_STRUCT TcTypeInterpretor :
    public TcInterpretor
{
public:

    TcTypeInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);
    bool SubmitClassType(const TString& className, TClassStruct& classStruct, bool updating);
    bool GetClass(const TString& className, TClassStruct& classStruct);

    virtual TrecPointer<TVariable> PrepAsyncCall();

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

