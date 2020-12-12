#pragma once
#include "TJavaScriptInterpretor.h"
#include <TClassStruct.h>

typedef enum class JsClassBlockType {
    jscbt_base, // The interpretor created when the class keyword is detected
    jscbt_const, // created when the "constructor" keyword is detected
    jscbt_method,// created when a method is detected
    jscbt_json   // Class is not declared with class keyword, uses old JS notation
}JsClassBlockType;

class TJavaScriptClassInterpretor :
    public TJavaScriptInterpretor
{public:
    /**
     * Method: TJavaScriptInterpretor::TInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TJavaScriptClassInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env, JsClassBlockType t);

    JsClassBlockType GetClassBlockType();

    void SetClassName(const TString& name);

    TString GetJsClassName();

    virtual void ProcessStatements(ReportObject& ro)override;

    TClassStruct GetClassData();

protected:
    TString className;
    JsClassBlockType blockType;
    TClassStruct classData;

    void ProcessMethod(const TString& methodName, TString& statement, int startParenth, ReportObject& ro, UCHAR att = 0);
};

