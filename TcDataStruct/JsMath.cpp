#define _CRT_RAND_S

#include "JsMath.h"

#include "TContainerVariable.h"
#include "TPrimitiveVariable.h"
#include "TSpecialVariable.h"

#include <cstdlib>

const double AG_MATH_E = 2.71828182845904523536;


TrecPointer<TVariable> JsMath::GetJsMathObject(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
{
    TrecSubPointer<TVariable, TContainerVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    // Add Variable Objects, Values from https://www.quantstart.com/articles/Mathematical-Constants-in-C/#:~:text=Mathematical%20Constants%20in%20C++%20A%20little-known%20feature%20of,and%20add%20it%20before%20importing%20the%20cmath%20library:
    ret->SetValue(L"E", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(AG_MATH_E));
    ret->SetValue(L"LN2", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(0.693147180559945309417));
    ret->SetValue(L"LN10", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(2.30258509299404568402));
    ret->SetValue(L"LOG2E", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(1.44269504088896340736));
    ret->SetValue(L"LOG10E", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(0.434294481903251827651));
    ret->SetValue(L"PI", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(3.14159265358979323846));
    ret->SetValue(L"SQRT1_2", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(0.707106781186547524401));
    ret->SetValue(L"SQRT2", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(1.41421356237309504880));

    // Add all the methods now
    ret->SetValue(L"abs", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::abs, parent, env));
    ret->SetValue(L"acos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::acos, parent, env));
    ret->SetValue(L"acosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::acosh, parent, env));
    ret->SetValue(L"asin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::asin, parent, env));
    ret->SetValue(L"asinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::asinh, parent, env));
    ret->SetValue(L"atan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::atan, parent, env));
    ret->SetValue(L"atan2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::atan2, parent, env));
    ret->SetValue(L"atanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::atanh, parent, env));
    ret->SetValue(L"cbrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::cbrt, parent, env));
    ret->SetValue(L"ceil", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::ceil, parent, env));
    ret->SetValue(L"clz32", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::clz32, parent, env));

    ret->SetValue(L"cos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::cos, parent, env));
    ret->SetValue(L"cosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::cosh, parent, env));
    ret->SetValue(L"exp", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::exp, parent, env));
    ret->SetValue(L"expm1", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::expm1, parent, env));
    ret->SetValue(L"floor", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::floor, parent, env));
    ret->SetValue(L"fround", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::fround, parent, env));
    ret->SetValue(L"log", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log, parent, env));
    ret->SetValue(L"log10", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log10, parent, env));
    ret->SetValue(L"log1p", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log1p, parent, env));
    ret->SetValue(L"log2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log2, parent, env));
    ret->SetValue(L"max", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::max_, parent, env));

    ret->SetValue(L"min", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::min_, parent, env));
    ret->SetValue(L"pow", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::pow, parent, env));
    ret->SetValue(L"random", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::random, parent, env));
    ret->SetValue(L"round", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::round, parent, env));
    ret->SetValue(L"sign", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sign, parent, env));
    ret->SetValue(L"sin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sin, parent, env));
    ret->SetValue(L"sinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sinh, parent, env));
    ret->SetValue(L"sqrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sqrt, parent, env));
    ret->SetValue(L"tan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::tan, parent, env));
    ret->SetValue(L"tanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::tanh, parent, env));
    ret->SetValue(L"trunc", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::trunc, parent, env));

    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}


void JsMath::abs(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() > 1)
    {
        if (!params[1].Get())
        {
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
            return;
        }
        DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);
        switch (num.type)
        {
            case double_long::dl_double:
                if (num.value.d < 0.0)
                    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(-num.value.d);
                else
                    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.d);
                return;
            case double_long::dl_sign:
                if (num.value.s < 0.0)
                    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(-num.value.s);
                else
                    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.s);
                return;
            case double_long::dl_unsign:
                ret.errorObject = params[1];
                return;
                
        }
    }
    ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
}

void JsMath::acos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        if(num.value.d > 1.0 || num.value.d < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acos(num.value.d));
        return;
    case double_long::dl_sign:
        if (num.value.s > 1.0 || num.value.s < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acos(num.value.s));
        return;
    case double_long::dl_unsign:
        if (num.value.u > 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acos(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::acosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d < 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acosh(num.value.d));
        return;
    case double_long::dl_sign:
        if (num.value.s < 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acosh(num.value.s));
        return;
    case double_long::dl_unsign:
        if (num.value.u < 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acosh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::asin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d > 1.0 || num.value.d < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asin(num.value.d));
        return;
    case double_long::dl_sign:
        if (num.value.s > 1.0 || num.value.s < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asin(num.value.s));
        return;
    case double_long::dl_unsign:
        if (num.value.u > 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asin(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::asinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d < 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asinh(num.value.d));
        return;
    case double_long::dl_sign:
        if (num.value.s < 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asinh(num.value.s));
        return;
    case double_long::dl_unsign:
        if (num.value.u < 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asinh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}
void JsMath::atan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::atan(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::atan(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::atan(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::atan2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 3 || !params[1].Get() || !params[2].Get() ||
        (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted) ||
        (params[2]->GetVarType() != var_type::primitive && params[2]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);
    DoubleLong num2 = DoubleLong::GetValueFromPrimitive(params[2]);

    if (num.type == double_long::dl_invalid || num2.type == double_long::dl_invalid)
    {

        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }
    double res = 0.0;

    if (num.type == double_long::dl_double && num2.type == double_long::dl_double)
        res = ::atan2(num.value.d, num2.value.d);
    if (num.type == double_long::dl_double && num2.type == double_long::dl_sign)
        res = ::atan2(num.value.d, num2.value.s);
    if (num.type == double_long::dl_double && num2.type == double_long::dl_unsign)
        res = ::atan2(num.value.d, num2.value.u);

    if (num.type == double_long::dl_sign && num2.type == double_long::dl_double)
        res = ::atan2(num.value.s, num2.value.d);
    if (num.type == double_long::dl_sign && num2.type == double_long::dl_sign)
        res = ::atan2(num.value.s, num2.value.s);
    if (num.type == double_long::dl_sign && num2.type == double_long::dl_unsign)
        res = ::atan2(num.value.s, num2.value.u);

    if (num.type == double_long::dl_unsign && num2.type == double_long::dl_double)
        res = ::atan2(num.value.u, num2.value.d);
    if (num.type == double_long::dl_unsign && num2.type == double_long::dl_sign)
        res = ::atan2(num.value.u, num2.value.s);
    if (num.type == double_long::dl_unsign && num2.type == double_long::dl_unsign)
        res = ::atan2(num.value.u, num2.value.u);

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(res);

}

void JsMath::atanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d > 1.0 || num.value.d < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::atanh(num.value.d));
        return;
    case double_long::dl_sign:
        if (num.value.s > 1.0 || num.value.s < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::atanh(num.value.s));
        return;
    case double_long::dl_unsign:
        if (num.value.u > 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::atanh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::cbrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cbrt(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cbrt(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cbrt(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::ceil(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(::ceil(num.value.d)));
        return;
    case double_long::dl_sign:
    case double_long::dl_unsign:
        ret.errorObject = params[1];
    }
}

void JsMath::clz32(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    UINT data = dynamic_cast<TPrimitiveVariable*>(params[1].Get())->Get4Value();

    UINT Rust = 0;
    for (UINT tester = 0x80000000; tester; tester = tester >> 1, Rust++)
    {
        if (data & tester)
            break;
    }

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(data);
}

void JsMath::cos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cos(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cos(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cos(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::cosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cosh(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cosh(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::cosh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::exp(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::exp(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::exp(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::exp(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::expm1(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::expm1(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::expm1(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::expm1(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}



void JsMath::floor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(::floor(num.value.d)));
        return;
    case double_long::dl_sign:
    case double_long::dl_unsign:
        ret.errorObject = params[1];
    }
}

void JsMath::fround(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::roundf(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::roundf(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::roundf(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}
void JsMath::log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::log10(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log10(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log10(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log10(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::log1p(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log1p(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log1p(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log1p(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}
void JsMath::log2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log2(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log2(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::log2(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}
void JsMath::max_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);
    if (num.type == double_long::dl_invalid)
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    for (UINT Rust = 2; Rust < params.Size(); Rust++)
    {
        DoubleLong num2 = DoubleLong::GetValueFromPrimitive(params[Rust]);
        if (num.type == double_long::dl_invalid)
        {
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
            return;
        }

        if (num2 >= num)
            num = num2;
    }
    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.d);
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.s);
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.u);
        return;
    }

}

void JsMath::min_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{

    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);
    if (num.type == double_long::dl_invalid)
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    for (UINT Rust = 2; Rust < params.Size(); Rust++)
    {
        DoubleLong num2 = DoubleLong::GetValueFromPrimitive(params[Rust]);
        if (num.type == double_long::dl_invalid)
        {
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
            return;
        }

        if (num2 <= num)
            num = num2;
    }
    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.d);
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.s);
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(num.value.u);
        return;
    }


}
void JsMath::pow(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 3 || !params[1].Get() || !params[2].Get() ||
        (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted) ||
        (params[2]->GetVarType() != var_type::primitive && params[2]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);
    DoubleLong num2 = DoubleLong::GetValueFromPrimitive(params[2]);

    if (num.type == double_long::dl_invalid || num2.type == double_long::dl_invalid)
    {

        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }
    double res = 0.0;

    if (num.type == double_long::dl_double && num2.type == double_long::dl_double)
        res = ::pow(num.value.d, num2.value.d);
    if (num.type == double_long::dl_double && num2.type == double_long::dl_sign)
        res = ::pow(num.value.d, num2.value.s);
    if (num.type == double_long::dl_double && num2.type == double_long::dl_unsign)
        res = ::pow(num.value.d, num2.value.u);

    if (num.type == double_long::dl_sign && num2.type == double_long::dl_double)
        res = ::pow(num.value.s, num2.value.d);
    if (num.type == double_long::dl_sign && num2.type == double_long::dl_sign)
        res = ::pow(num.value.s, num2.value.s);
    if (num.type == double_long::dl_sign && num2.type == double_long::dl_unsign)
        res = ::pow(num.value.s, num2.value.u);

    if (num.type == double_long::dl_unsign && num2.type == double_long::dl_double)
        res = ::pow(num.value.u, num2.value.d);
    if (num.type == double_long::dl_unsign && num2.type == double_long::dl_sign)
        res = ::pow(num.value.u, num2.value.s);
    if (num.type == double_long::dl_unsign && num2.type == double_long::dl_unsign)
        res = ::pow(num.value.u, num2.value.u);

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(res);
}

void JsMath::random(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    // Here, no parameters to worry about

    UINT random = 0;
    rand_s(&random);
    int i_random = 0;
    memcpy_s(&i_random, sizeof(i_random), &random, sizeof(random));

    double dRandom = static_cast<double>(i_random);
    double divisor = static_cast<double>(MAXINT);

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dRandom / divisor);
    
}
void JsMath::round(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(::round(num.value.d)));
        return;
    case double_long::dl_sign:
    case double_long::dl_unsign:
        ret.errorObject = params[1];
    }
}
void JsMath::sign(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);
    int res = 0;
    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d > 0.0)
            res = 1;
        else if (num.value.d < 0.0)
            res = -1;
        break;
    case double_long::dl_sign:
        if (num.value.s > 0.0)
            res = 1;
        else if (num.value.s < 0.0)
            res = -1;
        break;
    case double_long::dl_unsign:
        if (num.value.u > 0.0)
            res = 1;
        break;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(res);


}

void JsMath::sin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sin(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sin(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sin(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::sinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sinh(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sinh(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sinh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::sqrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d < 0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sqrt(num.value.d));
        return;
    case double_long::dl_sign:
        if(num.value.s < 0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sqrt(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::sqrt(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::tan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::tan(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::tan(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::tan(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMath::tanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) 
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::tanh(num.value.d));
        return;
    case double_long::dl_sign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::tanh(num.value.s));
        return;
    case double_long::dl_unsign:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::tanh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}
void JsMath::trunc(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (params.Size() < 2 || !params[1].Get() || (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(params[1]);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(::trunc(num.value.d)));
        return;
    case double_long::dl_sign:
    case double_long::dl_unsign:
        ret.errorObject = params[1];
    }
}
