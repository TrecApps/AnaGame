#pragma once
#include "TcJavaScriptInterpretor.h"

#include <TClassStruct.h>
class TcJavaScriptClassInterpretor : public TcJavaScriptInterpretor
{
public:
	TcJavaScriptClassInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);

    void SetClassName(const TString& name);

    bool SetSuperClassName(const TString& name);

    TString GetJsClassName();

    void ProcessStatements(ReturnObject& ro);

    TClassStruct GetClassData();
protected:
    TString className, superName;
    TClassStruct classData, superData;

    void ProcessMethod(const TString& methodName, TrecPointer<CodeStatement> state, ReturnObject& ro, UCHAR att);
};

