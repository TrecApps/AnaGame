#include "JsNumber.h"
#include "TContainerVariable.h"
#include "TPrimitiveVariable.h"
#include "TSpecialVariable.h"
#include "JavaScriptFunc.h"

const LONG64 MAX_SAFE_INT = 9007199254740991;
const LONG64 MIN_SAFE_INT = -9007199254740991;

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



void JsNumber::isFinite(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2)
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    TDataArray<TrecPointer<TVariable>> sParam;
    sParam.push_back(params[1]);
    JavaScriptFunc::isFinite(sParam, env, ret);
}
void JsNumber::isInteger(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2)
    {
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
        return;
    }

    TDataArray<TrecPointer<TVariable>> sParam;
    sParam.push_back(params[1]);
    JavaScriptFunc::parseInt(sParam, env, ret);

    if (ret.returnCode)
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
    }
    else
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(true);
}
void JsNumber::isNaN(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get())
    {
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
        return;
    }

    if (params[1]->GetVarType() == var_type::special_value && dynamic_cast<TSpecialVariable*>(params[1].Get())->GetSpecialValue() == SpecialVar::sp_nan)
    {
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(true);
        return;
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
}
void JsNumber::isSafeInteger(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.d < MAX_SAFE_INT && num.value.d > MIN_SAFE_INT);
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.s < MAX_SAFE_INT && num.value.s > MIN_SAFE_INT);
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.u < MAX_SAFE_INT && num.value.u > MIN_SAFE_INT);
        return;
    default:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
    }
}
void JsNumber::toExponential(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{

}

void JsNumber::toFixed(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::toLocaleString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::toPrecision(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::toString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsNumber::valueOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
