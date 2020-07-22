#include "pch.h"
#include "JavaScriptFunc.h"
#include "TPrimitiveVariable.h"

TMap<TNativeInterpretor> GetJavaScriptFunctions()
{
    TMap<TNativeInterpretor> jsFunctions;

    jsFunctions.addEntry(L"isFinite", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::isFinite));
    jsFunctions.addEntry(L"isNan", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::isNan));
    jsFunctions.addEntry(L"parseFloat", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::parseFloat));
    jsFunctions.addEntry(L"parseInt", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::parseInt));


    return jsFunctions;
}

void JavaScriptFunc::isFinite(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret)
{
    if (!params.Size())
    {
        ret.returnCode = ReportObject::too_few_params;
        ret.errorMessage.Set(L"'parseFloat' expected 1 parameter!");
        return;
    }

    auto var = params[0];
    if (var->IsString())
    {
        parseFloat(params, ret);
    }
    if (ret.returnCode == ReportObject::not_number)
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
    }
    else if (ret.returnCode == 0)
    {

    }
}

void JavaScriptFunc::isNan(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret)
{
    ReportObject r1, r2;

    parseFloat(params, r1);
    
    if (r1.returnCode == ReportObject::not_number  || r1.returnCode == ReportObject::too_few_params)
    {
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(true);
    }
    else
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
}

void JavaScriptFunc::parseFloat(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret)
{
    if (!params.Size())
    {
        ret.returnCode = ReportObject::too_few_params;
        ret.errorMessage.Set(L"'parseFloat' expected 1 parameter!");
        return;
    }

    auto var = params[0];

    if (var->IsObject())
    {
        // To-Do: Attempt to convert to string
        ret.returnCode = ReportObject::not_number;
        return;
    }

    if (var->IsString())
    {
        auto str = var->GetString();
        float f = 0.0f;
        if (!str.ConvertToFloat(&f))
        {
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(f);
            return;
        }

        double d = 0.0;
        if (!str.ConvertToDouble(&d))
        {
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(d);
            return;
        }

        if (IsInfinity(str))
        {
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(INFINITY);
            return;
        }

    }



    ret.returnCode = ReportObject::not_number;
}

void JavaScriptFunc::parseInt(TDataArray<TrecPointer<TVariable>>& params, ReportObject& ret)
{
    if (!params.Size())
    {
        ret.returnCode = ReportObject::too_few_params;
        ret.errorMessage.Set(L"'parseInt' expected 1 parameter!");
        return;
    }

    auto var = params[0];

    if (var->IsObject())
    {
        // To-Do: Attempt to convert to string
        ret.returnCode = ReportObject::not_number;
        return;
    }

    if (var->IsString())
    {
        auto str = var->GetString();
        float f = 0.0f;
        if (!str.ConvertToFloat(&f))
        {
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<int>(f));
            return;
        }

        double d = 0.0;
        if (!str.ConvertToDouble(&d))
        {
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<LONG64>(d));
            return;
        }

        int i = 0;
        if (!str.ConvertToInt(&i))
        {
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(i);
            return;
        }

        long long l = 0l;
        if (!str.ConvertToLong(&l))
        {
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(l);
            return;
        }
    }
    ret.returnCode = ReportObject::not_number;
}

bool JavaScriptFunc::IsInfinity(TString& str)
{
    return !str.CompareNoCase(L"Infinity");
}
