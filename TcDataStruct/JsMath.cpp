#define _CRT_RAND_S

#include "JsMath.h"

#include "TContainerVariable.h"
#include "TPrimitiveVariable.h"
#include "TSpecialVariable.h"
#include "TStringVariable.h"

#include <cstdlib>
#include "DefaultObjectOperator.h"

DefaultObjectOperator dObjectOperator;

const double AG_MATH_E = 2.71828182845904523536;


TrecPointer<TVariable> JsMath::GetJsMathObject(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
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
    ret->SetValue(L"abs", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::abs, parent, env));
    ret->SetValue(L"acos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::acos, parent, env));
    ret->SetValue(L"acosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::acosh, parent, env));
    ret->SetValue(L"asin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::asin, parent, env));
    ret->SetValue(L"asinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::asinh, parent, env));
    ret->SetValue(L"atan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::atan, parent, env));
    ret->SetValue(L"atan2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::atan2, parent, env));
    ret->SetValue(L"atanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::atanh, parent, env));
    ret->SetValue(L"cbrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::cbrt, parent, env));
    ret->SetValue(L"ceil", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::ceil, parent, env));
    ret->SetValue(L"clz32", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::clz32, parent, env));

    ret->SetValue(L"cos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::cos, parent, env));
    ret->SetValue(L"cosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::cosh, parent, env));
    ret->SetValue(L"exp", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::exp, parent, env));
    ret->SetValue(L"expm1", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::expm1, parent, env));
    ret->SetValue(L"floor", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::floor, parent, env));
    ret->SetValue(L"fround", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::fround, parent, env));
    ret->SetValue(L"log", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::log, parent, env));
    ret->SetValue(L"log10", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::log10, parent, env));
    ret->SetValue(L"log1p", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::log1p, parent, env));
    ret->SetValue(L"log2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::log2, parent, env));
    ret->SetValue(L"max", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::max_, parent, env));

    ret->SetValue(L"min", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::min_, parent, env));
    ret->SetValue(L"pow", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::pow, parent, env));
    ret->SetValue(L"random", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::random, parent, env));
    ret->SetValue(L"round", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::round, parent, env));
    ret->SetValue(L"sign", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::sign, parent, env));
    ret->SetValue(L"sin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::sin, parent, env));
    ret->SetValue(L"sinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::sinh, parent, env));
    ret->SetValue(L"sqrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::sqrt, parent, env));
    ret->SetValue(L"tan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::tan, parent, env));
    ret->SetValue(L"tanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::tanh, parent, env));
    ret->SetValue(L"trunc", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsMath::trunc, parent, env));

    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}


void JsMath::abs(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::acos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::acosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::asin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::asinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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
void JsMath::atan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::atan2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::atanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::cbrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::ceil(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::clz32(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::cos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::cosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::exp(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::expm1(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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



void JsMath::floor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::fround(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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
void JsMath::log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::log10(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::log1p(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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
void JsMath::log2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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
void JsMath::max_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::min_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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
void JsMath::pow(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::random(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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
void JsMath::round(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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
void JsMath::sign(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::sin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::sinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::sqrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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

void JsMath::tan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

void JsMath::tanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
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
void JsMath::trunc(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
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

TrecPointer<TVariable> JsMathTc::GetJsMathObject()
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
    ret->SetValue(L"abs", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::abs));
    ret->SetValue(L"acos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::acos));
    ret->SetValue(L"acosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::acosh));
    ret->SetValue(L"asin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::asin));
    ret->SetValue(L"asinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::asinh));
    ret->SetValue(L"atan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::atan));
    ret->SetValue(L"atan2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::atan2));
    ret->SetValue(L"atanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::atanh));
    ret->SetValue(L"cbrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::cbrt));
    ret->SetValue(L"ceil", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::ceil));
    ret->SetValue(L"clz32", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::clz32));

    ret->SetValue(L"cos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::cos));
    ret->SetValue(L"cosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::cosh));
    ret->SetValue(L"exp", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::exp));
    ret->SetValue(L"expm1", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::expm1));
    ret->SetValue(L"floor", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::floor));
    ret->SetValue(L"fround", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::fround));
    ret->SetValue(L"log", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::log));
    ret->SetValue(L"log10", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::log10));
    ret->SetValue(L"log1p", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::log1p));
    ret->SetValue(L"log2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::log2));
    ret->SetValue(L"max", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::max_));

    ret->SetValue(L"min", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::min_));
    ret->SetValue(L"pow", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::pow));
    ret->SetValue(L"random", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::random));
    ret->SetValue(L"round", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::round));
    ret->SetValue(L"sign", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::sign));
    ret->SetValue(L"sin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::sin));
    ret->SetValue(L"sinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::sinh));
    ret->SetValue(L"sqrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::sqrt));
    ret->SetValue(L"tan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::tan));
    ret->SetValue(L"tanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::tanh));
    ret->SetValue(L"trunc", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsMathTc::trunc));

    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}

void JsMathTc::abs(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    if (variables.count() > 1)
    {
        TDataEntry<TcVariableHolder> var1;
        if (!variables.GetEntryAt(1, var1) || !var1.object.value.Get())
        {
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
            return;
        }
        DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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
            ret.errorObject = var1.object.value;
            return;

        }
    }
    ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
}

void JsMathTc::acos(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d > 1.0 || num.value.d < -1.0)
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

void JsMathTc::acosh(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d > 1.0 || num.value.d < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acosh(num.value.d));
        return;
    case double_long::dl_sign:
        if (num.value.s > 1.0 || num.value.s < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acosh(num.value.s));
        return;
    case double_long::dl_unsign:
        if (num.value.u > 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::acosh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMathTc::asin(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

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

void JsMathTc::asinh(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

    switch (num.type)
    {
    case double_long::dl_double:
        if (num.value.d > 1.0 || num.value.d < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asinh(num.value.d));
        return;
    case double_long::dl_sign:
        if (num.value.s > 1.0 || num.value.s < -1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asinh(num.value.s));
        return;
    case double_long::dl_unsign:
        if (num.value.u > 1.0)
            ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        else
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(::asinh(num.value.u));
        return;
    default:
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
    }
}

void JsMathTc::atan(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

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

void JsMathTc::atan2(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1,var2;
    if (variables.count() < 3 || 
        !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted) ||
        !variables.GetEntryAt(2, var2) || !var2.object.value.Get() || (var2.object.value->GetVarType() != var_type::primitive && var2.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
    DoubleLong num2 = DoubleLong::GetValueFromPrimitive(var2.object.value);

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

void JsMathTc::atanh(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

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

void JsMathTc::cbrt(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

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

void JsMathTc::ceil(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(::ceil(num.value.d)));
        return;
    case double_long::dl_sign:
    case double_long::dl_unsign:
        ret.errorObject = var1.object.value;
    }
}

void JsMathTc::clz32(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);

    UINT data = dynamic_cast<TPrimitiveVariable*>(var1.object.value.Get())->Get4Value();

    UINT Rust = 0;
    for (UINT tester = 0x80000000; tester; tester = tester >> 1, Rust++)
    {
        if (data & tester)
            break;
    }

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(data);
}

void JsMathTc::cos(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::cosh(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::exp(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::expm1(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::floor(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(::floor(num.value.d)));
        return;
    case double_long::dl_sign:
    case double_long::dl_unsign:
        ret.errorObject = var1.object.value;
    }
}

void JsMathTc::fround(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::log(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::log10(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::log1p(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::log2(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

void JsMathTc::max_(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
    if (num.type == double_long::dl_invalid)
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    for (UINT Rust = 2; variables.GetEntryAt(Rust, var1); Rust++)
    {
        DoubleLong num2 = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

TC_DATA_STRUCT void JsMathTc::min_(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
    if (num.type == double_long::dl_invalid)
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    for (UINT Rust = 2; variables.GetEntryAt(Rust, var1); Rust++)
    {
        DoubleLong num2 = DoubleLong::GetValueFromPrimitive(var1.object.value);
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

TC_DATA_STRUCT void JsMathTc::pow(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1, var2;
    if (variables.count() < 3 ||
        !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted) ||
        !variables.GetEntryAt(2, var2) || !var2.object.value.Get() || (var2.object.value->GetVarType() != var_type::primitive && var2.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
    DoubleLong num2 = DoubleLong::GetValueFromPrimitive(var2.object.value);
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

TC_DATA_STRUCT void JsMathTc::random(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
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

TC_DATA_STRUCT void JsMathTc::round(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
}

TC_DATA_STRUCT void JsMathTc::sign(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
}

TC_DATA_STRUCT void JsMathTc::sin(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
}

TC_DATA_STRUCT void JsMathTc::sinh(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
}

TC_DATA_STRUCT void JsMathTc::sqrt(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
}

TC_DATA_STRUCT void JsMathTc::tan(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
}

TC_DATA_STRUCT void JsMathTc::tanh(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
}

TC_DATA_STRUCT void JsMathTc::trunc(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (variables.count() < 2 || !variables.GetEntryAt(1, var1) || !var1.object.value.Get() || (var1.object.value->GetVarType() != var_type::primitive && var1.object.value->GetVarType() != var_type::primitive_formatted))
    {
        ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
        return;
    }

    DoubleLong num = DoubleLong::GetValueFromPrimitive(var1.object.value);
    switch (num.type)
    {
    case double_long::dl_double:
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(::trunc(num.value.d)));
        return;
    case double_long::dl_sign:
    case double_long::dl_unsign:
        ret.errorObject = var1.object.value;
    }
}

TrecPointer<TVariable> JsOperatorsTc::addDefault(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
    if (!var1.Get() || !var2.Get())
    {
        return TrecPointer<TVariable>();
    }

    if (var1->GetVarType() == var_type::primitive && var2->GetVarType() == var_type::primitive)
    {
        auto retValue = ::Add(DoubleLong::GetValueFromPrimitive(var1), DoubleLong::GetValueFromPrimitive(var2));

        TrecPointer<TVariable> ret;

        switch (retValue.type)
        {
        case double_long::dl_double:
            ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.d);
            break;
        case double_long::dl_unsign:
            ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.u);
            break;
        case double_long::dl_sign:
            ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retValue.value.s);
        }
        return ret;
    }

    if (var1->GetVarType() == var_type::primitive &&
        (var2->GetVarType() == var_type::string || var2->GetVarType() == var_type::native_object))
    {
        TString strValue(GetStringFromPrimitive(var1) + var2->GetString());
        return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(strValue);
    }

    if ((var1->GetVarType() == var_type::string || var1->GetVarType() == var_type::native_object) &&
        (var2->GetVarType() == var_type::string || var2->GetVarType() == var_type::native_object))
    {
        TString strValue(var1->GetString() + var2->GetString());
        return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(strValue);
    }

    if ((var1->GetVarType() == var_type::string || var1->GetVarType() == var_type::native_object)
        && var2->GetVarType() == var_type::primitive)
    {
        TString strValue(var1->GetString() + GetStringFromPrimitive(var2));
        return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(strValue);
    }
    return TrecPointer<TVariable>();
}

TrecPointer<TVariable> JsOperatorsTc::GetJsOperators()
{
    TrecSubPointer<TVariable, TContainerVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::str_cond));
    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::bool_op));
    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::add));
    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::sub));
    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::mul));
    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::div));
    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::mod));
    ret->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeRunner1>(JsOperatorsTc::last));

    return TrecSubToTrec(ret);
}

void JsOperatorsTc::str_cond(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;
    if (!variables.GetEntryAt(0, var1) || !var1.object.value.Get())
    {
        ret.returnCode = ret.ERR_TOO_FEW_PARMS;
        ret.errorMessage.Set(L"JS Internal Error! Exprected String parameters in String Condensation function!");
        return;
    }

    TString base(var1.object.value->GetString());

    int sLoc = base.FindOutOfQuotes(L"${", 0, false), eLoc = base.FindOutOfQuotes(L'}', sLoc, false);
    UINT Rust = 1;
    while (variables.GetEntryAt(Rust++, var1) && sLoc != -1 && eLoc > sLoc)
    {
        TString subRep(base.SubString(sLoc, eLoc + 1));
        TString repWith(var1.object.value.Get() ? var1.object.value->GetString() : L"null");
        
        int diff = static_cast<int>(subRep.GetSize()) - static_cast<int>(repWith.GetSize());

        base.Replace(subRep, repWith);
        sLoc = base.FindOutOfQuotes(L"${", eLoc + diff, false), eLoc = base.FindOutOfQuotes(L'}', sLoc, false);
    }

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(base);
}

void JsOperatorsTc::bool_op(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> var1;

    bool retVal = variables.GetEntryAt(0, var1);
    if (retVal)
    {
        if (!var1.object.value.Get())
            retVal = false;

        switch (var1.object.value->GetVarType())
        {
        case var_type::string:
            // If a String, then only the Empty String is considered Falsy
            retVal = dynamic_cast<TStringVariable*>(var1.object.value.Get())->GetSize() > 0;
            break;
        case var_type::primitive:
            // If a Primitive value, boolean false, 
            retVal = var1.object.value->Get8Value() > 0;
            break;
        default:

        }

    }

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(retVal);
}

void JsOperatorsTc::add(TDataMap<TcVariableHolder> variables, ReturnObject& ret_)
{
    TDataEntry<TcVariableHolder> v1, v2;
    TrecPointer<TVariable> var1, var2;
    variables.GetEntryAt(0, v1);
    variables.GetEntryAt(0, v2);
    var1 = v2.object.value;
    var2 = v1.object.value;

    TrecPointer<TVariable> ret = addDefault(var1, var2);
    if (ret.Get())
    {
        ret_.errorObject = ret;
        return;
    }
    if (!var1.Get() && !var2.Get())
    {
        ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0ULL);
    }

    if (var1.Get() && var1->GetVarType() == var_type::string)
    {
        TString value(var1->GetString());
        value.Append(var2.Get() ? var2->GetString() : L"null");
        ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
    }
    else if (var2.Get() && var2->GetVarType() == var_type::string)
    {
        TString value(var1.Get() ? var1->GetString() : L"null");
        value.Append(var2->GetString());
        ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
    }
    if (ret.Get())
    {
        ret_.errorObject = ret;
        return;
    }

    if (var1.Get() && !var2.Get())
    {
        if (var1->GetVarType() == var_type::collection)
        {
            auto varColl = dynamic_cast<TContainerVariable*>(var1.Get());
            TString value;
            for (UINT Rust = 0; Rust < varColl->GetSize(); Rust++)
            {
                auto val = varColl->GetValueAt(Rust);
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer().getBuffer() : L"null");
            }
            if (value.StartsWith(L","))
                value.Delete(0);
            value.Append(L"null");
            ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
        }
        if (var1->GetVarType() == var_type::primitive)
        {
            ret = var1;
        }
    }

    if (var2.Get() && !var1.Get())
    {
        if (var2->GetVarType() == var_type::collection)
        {
            auto varColl = dynamic_cast<TContainerVariable*>(var2.Get());
            TString value;
            for (UINT Rust = 0; Rust < varColl->GetSize(); Rust++)
            {
                auto val = varColl->GetValueAt(Rust);
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer().getBuffer() : L"null");
            }
            if (value.StartsWith(L","))
                value.Delete(0);
            value.Set(TString(L"null") + value);
            ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
        }
        if (var2->GetVarType() == var_type::primitive)
        {
            ret = var2;
        }
    }
    ret_.errorObject = ret;
}

void JsOperatorsTc::sub(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> v1, v2;
    variables.GetEntryAt(0, v1);
    variables.GetEntryAt(0, v2);

    ret.errorObject = dObjectOperator.Subtract(v2.object.value, v1.object.value);
}

void JsOperatorsTc::mul(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> v1, v2;
    variables.GetEntryAt(0, v1);
    variables.GetEntryAt(0, v2);

    ret.errorObject = dObjectOperator.Multiply(v2.object.value, v1.object.value);
}

void JsOperatorsTc::div(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> v1, v2;
    variables.GetEntryAt(0, v1);
    variables.GetEntryAt(0, v2);

    ret.errorObject = dObjectOperator.Divide(v2.object.value, v1.object.value);
}

void JsOperatorsTc::mod(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    TDataEntry<TcVariableHolder> v1, v2;
    variables.GetEntryAt(0, v1);
    variables.GetEntryAt(0, v2);

    ret.errorObject = dObjectOperator.Mod(v2.object.value, v1.object.value);
}

void JsOperatorsTc::last(TDataMap<TcVariableHolder> variables, ReturnObject& ret)
{
    
}
