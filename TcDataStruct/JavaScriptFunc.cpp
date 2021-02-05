#include "pch.h"
#include "JavaScriptFunc.h"
#include "TPrimitiveVariable.h"
#include "TContainerVariable.h"
#include "TObjectVariable.h"
#include "TSpecialVariable.h"
#include "TStringVariable.h"

/**
 * Characters excempt from encoding by encodeURL
 */
const TString regularEscapeExempt(L";,/?:@&=+$-_.!~*'()#");
/**
 * Characters excempt from encoding by encodeURLComponent
 */
const TString componentEscapeExempt(L"-_.!~*'()");

/**
 * Characters excempt from encoding by escape 
 */
const TString escapeEscapeExempt(L"@*_+-./");

TMap<TNativeInterpretor> GetJavaScriptFunctions()
{
    TMap<TNativeInterpretor> jsFunctions;

    jsFunctions.addEntry(L"isFinite", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::isFinite, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"isNaN", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::isNaN, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"parseFloat", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::parseFloat, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"parseInt", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::parseInt, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"encodeURI", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::EncodeURI, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"decodeURI", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::DecodeURI, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"encodeURIComponent", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::EncodeURIComponent, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"decodeURIComponent", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::DecodeURIComponent, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));

    jsFunctions.addEntry(L"escape", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::JsEscape, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    jsFunctions.addEntry(L"unescape", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::JsUnEscape, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    // jsFunctions.addEntry(L"parseFloat", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::parseFloat, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));
    // jsFunctions.addEntry(L"parseInt", TrecPointerKey::GetNewTrecPointer<TNativeInterpretor>(JavaScriptFunc::parseInt, TrecSubPointer<TVariable, TInterpretor>(), TrecPointer<TEnvironment>()));

    return jsFunctions;
}


namespace JSObject
{
    /**
     * Function: JSObject::JsObjectAssign
     * Purpose: Serves as Anagame's implementation of the Object.Assign method
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - objects to work with
     *              TrecPointer<TEnvironment> env - environment function is being called in
     *              ReportObject& ret - return information
     * Returns: void
     * 
     * Note: Function uses the Native Function interface used by TNative Interpretors
     */
    void JsObjectAssign(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
    {
        if (params.Size() < 2)
        {
            ret.returnCode = ReportObject::too_few_params;
            ret.errorMessage.Set("Insufficient Params");
            return;
        }

        if (!params[1].Get() || params[1]->GetVarType() != var_type::collection)
        {
            ret.returnCode = ReportObject::improper_type;
            ret.errorMessage.Set("Target Object must be a JS Object type!");
            return;
        }

        if (params.Size() > 2 && params[2].Get() && params[2]->GetVarType() == var_type::collection)
        {
            auto source = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[2]);
            auto target = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[1]);
            TString key;
            TrecPointer<TVariable> value;
            for (UINT Rust = 0; source->GetValueAt(Rust, key, value); Rust++)
            {
                target->SetValue(key, value);
            }
        }

        ret.errorObject = params[1];
    }

    /**
     * Function: JSObject::JsObjectCreate
     * Purpose: Serves as Anagame's implementation of the Object.Create method
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - objects to work with
     *              TrecPointer<TEnvironment> env - environment function is being called in
     *              ReportObject& ret - return information
     * Returns: void
     *
     * Note: Function uses the Native Function interface used by TNative Interpretors
     */
    void JsObjectCreate(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
    {
        if (params.Size() < 2)
        {
            ret.returnCode = ReportObject::too_few_params;
            ret.errorMessage.Set("Insufficient Params");
            return;
        }

        JsObjectAssign(params, env, ret);
    }
}


void JavaScriptFunc::isFinite(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (!params.Size())
    {
        ret.returnCode = ReportObject::too_few_params;
        ret.errorMessage.Set(L"'parseFloat' expected 1 parameter!");
        return;
    }

    auto var = params[0];
    if (var->GetVarType() == var_type::string)
    {
        parseFloat(params, env, ret);
    }
    if (ret.returnCode)
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(true);
    }
    else if (ret.returnCode == 0)
    {
        ULONG64 rawValue;

        auto prim = dynamic_cast<TPrimitiveVariable*>(ret.errorObject.Get());

        if (!(prim->GetType() & TPrimitiveVariable::type_float))
        {
            // Only a floa can hold infinite
            ret.returnCode = 0;
            ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(true);
        }
        rawValue = prim->Get8Value();
        bool foundInf = false;
        switch (prim->GetSize())
        {
        case 8:
            foundInf = rawValue == 0xFFFFFFFFFFFFFFFF;
            break;
        case 4:
            foundInf = rawValue == INFINITY;
            break;
        }

        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(!foundInf);
    }
}

void JavaScriptFunc::isNaN(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    ReportObject r1, r2;

    parseFloat(params, env, r1);
    
    if (r1.returnCode == ReportObject::not_number  || r1.returnCode == ReportObject::too_few_params)
    {
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(true);
    }
    else
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(false);
}

void JavaScriptFunc::parseFloat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (!params.Size())
    {
        ret.returnCode = ReportObject::too_few_params;
        ret.errorMessage.Set(L"'parseFloat' expected 1 parameter!");
        return;
    }

    auto var = params[0];

    TString strNum(var.Get() ? var->GetString().GetTrim() : L"null");

    float f = 0.0f;
    if (!strNum.ConvertToFloat(f))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(f);
        return;
    }

    double d = 0.0;
    if (!strNum.ConvertToDouble(d))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(d);
        return;
    }

    if (IsInfinity(strNum))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(INFINITY);
        return;
    }

    ret.returnCode = ReportObject::not_number;
    ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);
}

void JavaScriptFunc::parseInt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    if (!params.Size())
    {
        ret.returnCode = ReportObject::too_few_params;
        ret.errorMessage.Set(L"'parseFloat' expected 1 parameter!");
        return;
    }

    auto var = params[0];

    TString strNum(var.Get() ? var->GetString().GetTrim() : L"null");

    float f = 0.0f;
    if (!strNum.ConvertToFloat(f))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(f);
        return;
    }

    double d = 0.0;
    if (!strNum.ConvertToDouble(d))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(d);
        return;
    }
    int i = 0;
    if (!strNum.ConvertToInt(i))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(i);
        return;
    }
    long long l = 0l;
    if (!strNum.ConvertToLong(l))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(l);
        return;
    }
    if (IsInfinity(strNum))
    {
        ret.returnCode = 0;
        ret.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(INFINITY);
        return;
    }

    ret.returnCode = ReportObject::not_number;
    ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_nan);

}

void JavaScriptFunc::EncodeURI(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    TString val;
    if (!ConvertParamOneToString(val, params))
    {
        ret.returnCode = ReportObject::too_few_params;

        return;
    }
    TString val2;
    for (UINT Rust = 0; Rust < val.GetSize(); Rust++)
    {
        WCHAR ch = val[Rust];
        if (IsURLEscaped(ch, 2))
        {
            val2.AppendFormat(L"\%%X", (UINT)ch);
        }
        else
            val2.AppendChar(ch);
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(val2);
}

void JavaScriptFunc::EncodeURIComponent(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    TString val;
    if (!ConvertParamOneToString(val, params))
    {
        ret.returnCode = ReportObject::too_few_params;

        return;
    }
    TString val2;
    for (UINT Rust = 0; Rust < val.GetSize(); Rust++)
    {
        WCHAR ch = val[Rust];
        if (IsURLEscaped(ch, 1))
        {
            val2.AppendFormat(L"\%%X", (UINT)ch);
        }
        else
            val2.AppendChar(ch);
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(val2);
}

void JavaScriptFunc::DecodeURI(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    TString val;
    if (!ConvertParamOneToString(val, params))
    {
        ret.returnCode = ReportObject::too_few_params;

        return;
    }
    TString val2;
    long int charVal = 0;

    for (int Rust = 0; Rust < val.GetSize(); Rust++)
    {
        switch (val2.GetSize())
        {
        case 1:
        case 2:
            val2.AppendChar(val[Rust]);
            break;
        case 3:
            charVal = wcstol(val2.GetConstantBuffer(), nullptr, 16);
            if (!charVal)
            {
                ret.returnCode = ReportObject::invalid_name;

                return;
            }
            if (IsURLEscaped(static_cast<WCHAR>(charVal), 2))
            {
                val.Replace(val2, TString(static_cast<WCHAR>(charVal)));
                Rust = -1;
                continue;
            }
            val2.Empty();
            break;
        case 0:
            if(val[Rust] == L'%')
                val2.AppendChar(val[Rust]);
        }
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(val);
}

void JavaScriptFunc::DecodeURIComponent(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    TString val;
    if (!ConvertParamOneToString(val, params))
    {
        ret.returnCode = ReportObject::too_few_params;

        return;
    }
    TString val2;
    long int charVal = 0;

    for (int Rust = 0; Rust < val.GetSize(); Rust++)
    {
        switch (val2.GetSize())
        {
        case 1:
        case 2:
            val2.AppendChar(val[Rust]);
            break;
        case 3:
            charVal = wcstol(val2.GetConstantBuffer(), nullptr, 16);
            if (!charVal)
            {
                ret.returnCode = ReportObject::invalid_name;

                return;
            }
            if (IsURLEscaped(static_cast<WCHAR>(charVal), 1))
            {
                val.Replace(val2, TString(static_cast<WCHAR>(charVal)));
                Rust = -1;
                continue;
            }
            val2.Empty();
            break;
        case 0:
            if (val[Rust] == L'%')
                val2.AppendChar(val[Rust]);
        }
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(val);
}

void JavaScriptFunc::JsEscape(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
     TString val;
     if (!ConvertParamOneToString(val, params))
     {
         ret.returnCode = ReportObject::too_few_params;

         return;
     }
     TString val2;
     for (UINT Rust = 0; Rust < val.GetSize(); Rust++)
     {
         WCHAR ch = val[Rust];
         if (IsURLEscaped(ch, 0))
         {
             val2.AppendFormat(L"\%%X", (UINT)ch);
         }
         else
             val2.AppendChar(ch);
     }
     ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(val2);
}

TC_DATA_STRUCT void JavaScriptFunc::JsUnEscape(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    TString val;
    if (!ConvertParamOneToString(val, params))
    {
        ret.returnCode = ReportObject::too_few_params;

        return;
    }
    TString val2;
    long int charVal = 0;

    for (int Rust = 0; Rust < val.GetSize(); Rust++)
    {
        switch (val2.GetSize())
        {
        case 1:
        case 2:
            val2.AppendChar(val[Rust]);
            break;
        case 3:
            charVal = wcstol(val2.GetConstantBuffer(), nullptr, 16);
            if (!charVal)
            {
                ret.returnCode = ReportObject::invalid_name;

                return;
            }
            if (IsURLEscaped(static_cast<WCHAR>(charVal), 0))
            {
                val.Replace(val2, TString(static_cast<WCHAR>(charVal)));
                Rust = -1;
                continue;
            }
            val2.Empty();
            break;
        case 0:
            if (val[Rust] == L'%')
                val2.AppendChar(val[Rust]);
        }
    }
    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(val);
}

TrecPointer<TVariable> JavaScriptFunc::GetJSObectVariable(TrecSubPointer<TVariable, TInterpretor> parent, TrecPointer<TEnvironment> env)
{
    auto ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    ret->SetValue(L"assign", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JSObject::JsObjectAssign, parent, env));
    ret->SetValue(L"create", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JSObject::JsObjectCreate, parent, env));


    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}

bool JavaScriptFunc::IsInfinity(TString& str)
{
    return !str.CompareNoCase(L"Infinity");
}

bool JavaScriptFunc::IsURLEscaped(WCHAR c, UCHAR regular)
{
    bool ret = ((c >= L'a' && c <= L'z') ||
        (c >= L'A' && c <=L'Z') ||
        (c >= L'0' && c <=L'9'));

    if (ret)
        return false;
    switch (regular)
    {
    case 0:
        return escapeEscapeExempt.Find(c) == -1;
    case 1:
        return componentEscapeExempt.Find(c) == -1;
    default:
        return regularEscapeExempt.Find(c) == -1;
    }
}

bool JavaScriptFunc::ConvertParamOneToString(TString& value, TDataArray<TrecPointer<TVariable>>& params)
{
    if(!params.Size() || !params[0].Get())
        return false;

    value.Set(params[0]->GetString());
    return true;
}
