#include "JsNumber.h"
#include "TContainerVariable.h"
#include "TPrimitiveVariable.h"
#include "TSpecialVariable.h"

 TrecPointer<TVariable> JsNumber::GetJsNumberObject(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
{
     TrecSubPointer<TVariable, TContainerVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

     ret->SetValue(L"MAX_VALUE", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(DBL_MAX));
     ret->SetValue(L"MIN_VALUE", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(DBL_MIN));
     ret->SetValue(L"NEGATIVE_INFINITY", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>((-DBL_MAX) * 2));
     ret->SetValue(L"NaN",TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan));
     ret->SetValue(L"INFINITY", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(INFINITY));

     ret->SetValue(L"isFinite", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::isFinite, parent, env));
     ret->SetValue(L"isInteger", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::isInteger, parent, env));
     ret->SetValue(L"isNaN", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::isNaN, parent, env));
     ret->SetValue(L"isSafeInteger", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::isSafeInteger, parent, env));
     ret->SetValue(L"toExponential", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::toExponential, parent, env));
     ret->SetValue(L"toFixed", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::toFixed, parent, env));
     ret->SetValue(L"toLocaleString", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::toLocaleString, parent, env));
     ret->SetValue(L"toPrecision", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::toPrecision, parent, env));
     ret->SetValue(L"toString", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::toString, parent, env));
     ret->SetValue(L"valueOf", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsNumber::valueOf, parent, env));

     return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}



void JsNumber::isFinite(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::isInteger(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::isNaN(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::isSafeInteger(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::toExponential(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }

void JsNumber::toFixed(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::toLocaleString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::toPrecision(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::toString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::valueOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
