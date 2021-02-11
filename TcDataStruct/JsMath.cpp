#include "JsMath.h"

#include "TContainerVariable.h"
#include "TPrimitiveVariable.cpp"

#define _USE_MATH_DEFINES 

#include <cmath>

TrecPointer<TVariable> JsMath::GetJsMathObject()
{
    TrecSubPointer<TVariable, TContainerVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    // Add Variable Objects, Values from https://www.quantstart.com/articles/Mathematical-Constants-in-C/#:~:text=Mathematical%20Constants%20in%20C++%20A%20little-known%20feature%20of,and%20add%20it%20before%20importing%20the%20cmath%20library:
    ret->SetValue(L"E", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(2.71828182845904523536));
    ret->SetValue(L"LN2", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(0.693147180559945309417));
    ret->SetValue(L"LN10", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(2.30258509299404568402));
    ret->SetValue(L"LOG2E", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(1.44269504088896340736));
    ret->SetValue(L"LOG10E", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(0.434294481903251827651));
    ret->SetValue(L"PI", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(3.14159265358979323846));
    ret->SetValue(L"SQRT1_2", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(0.707106781186547524401));
    ret->SetValue(L"SQRT2", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TPrimitiveVariable>(1.41421356237309504880));

    // Add all the methods now
    ret->SetValue(L"abs", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::abs));
    ret->SetValue(L"acos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::acos));
    ret->SetValue(L"acosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::acosh));
    ret->SetValue(L"asin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::asin));
    ret->SetValue(L"asinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::asinh));
    ret->SetValue(L"atan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::atan));
    ret->SetValue(L"atan2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::atan2));
    ret->SetValue(L"atanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::atanh));
    ret->SetValue(L"cbrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::cbrt));
    ret->SetValue(L"ceil", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::ceil));
    ret->SetValue(L"clz32", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::clz32));

    ret->SetValue(L"cos", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::cos));
    ret->SetValue(L"cosh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::cosh));
    ret->SetValue(L"exp", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::exp));
    ret->SetValue(L"expm1", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::expm1));
    ret->SetValue(L"floor", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::floor));
    ret->SetValue(L"fround", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::fround));
    ret->SetValue(L"log", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log));
    ret->SetValue(L"log10", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log10));
    ret->SetValue(L"log1p", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log1p));
    ret->SetValue(L"log2", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::log2));
    ret->SetValue(L"max", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::max_));

    ret->SetValue(L"min", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::min_));
    ret->SetValue(L"pow", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::pow));
    ret->SetValue(L"random", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::random));
    ret->SetValue(L"round", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::round));
    ret->SetValue(L"sign", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sign));
    ret->SetValue(L"sin", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sin));
    ret->SetValue(L"sinh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sinh));
    ret->SetValue(L"sqrt", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::sqrt));
    ret->SetValue(L"tan", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::tan));
    ret->SetValue(L"tanh", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::tanh));
    ret->SetValue(L"trunc", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsMath::trunc));

    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}

void JsMath::abs(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{

}

void JsMath::abs(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::acos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::acosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::asin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::asinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::atan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::atan2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::atanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::cbrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::ceil(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::clz32(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::cos(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::cosh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::exp(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::expm1(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }

void JsMath::floor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::fround(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::log(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::log10(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::log1p(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::log2(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::max_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::min_(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::pow(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::random(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::round(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::sign(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::sin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::sinh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::sqrt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }

void JsMath::tan(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::tanh(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
void JsMath::trunc(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret) {  }
