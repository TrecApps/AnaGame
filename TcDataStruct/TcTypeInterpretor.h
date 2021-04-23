#pragma once
#include <TcInterpretor.h>
#include "TcDataStruct.h"
#include "TClassStruct.h"
class TC_DATA_STRUCT TcTypeInterpretor :
    public TcInterpretor
{
public:

    TcTypeInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);
    bool SubmitClassType(const TString& className, TClassStruct& classStruct, bool updating);
    bool GetClass(const TString& className, TClassStruct& classStruct);
protected:
    /**
 * The list of Types held by the Interpretor
 */
    TDataMap<TClassStruct> classes;
};

