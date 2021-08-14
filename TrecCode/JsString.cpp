#include "JsString.h"
#include <TContainerVariable.h>
#include <TcNativeInterpretor.h>
#include <TStringVariable.h>
#include <TSpecialVariable.h>
#include <TPrimitiveVariable.h>

TrecPointer<TVariable> JsString::GetJsStringMethods(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
{
	TrecSubPointer<TVariable, TContainerVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

	// Static
	ret->SetValue(L"fromCharCode", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringFromCharCode, parent, env));
	ret->SetValue(L"fromCodePoint", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringFromCodePoint, parent, env));
	ret->SetValue(L"raw", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringRaw, parent, env));

	// Instance
	ret->SetValue(L"at", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringAt, parent, env));
	ret->SetValue(L"charAt", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringCharAt, parent, env));
	ret->SetValue(L"charCodeAt", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringCharCodeAt, parent, env));
	ret->SetValue(L"charCodePoint", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringCodePointAt, parent, env));
	ret->SetValue(L"concat", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringConcat, parent, env));
	ret->SetValue(L"includes", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringIncludes, parent, env));
	ret->SetValue(L"endsWith", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringEndsWith, parent, env));
	ret->SetValue(L"indexOf", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringIndexOf, parent, env));
	ret->SetValue(L"lastIndexOf", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringLastIndexOf, parent, env));
	ret->SetValue(L"localeCompare", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringLocaleCompare, parent, env));
	ret->SetValue(L"match", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringMatch, parent, env));
	ret->SetValue(L"matchAll", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringMatchAll, parent, env));
	ret->SetValue(L"normalize", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringNormalize, parent, env));
	ret->SetValue(L"padEnd", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringPadEnd, parent, env));
	ret->SetValue(L"padStart", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringPadStart, parent, env));

	ret->SetValue(L"repeat", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringRepeat, parent, env));
	ret->SetValue(L"replace", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringReplace, parent, env));
	ret->SetValue(L"replaceAll", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringReplaceAll, parent, env));
	ret->SetValue(L"search", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringSearch, parent, env));
	ret->SetValue(L"slice", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringSlice, parent, env));
	ret->SetValue(L"split", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringSplit, parent, env));
	ret->SetValue(L"startsWith", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringStartsWith, parent, env));
	ret->SetValue(L"substring", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringSubString, parent, env));
	ret->SetValue(L"toLocaleLowerCase", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringToLocaleLowerCase, parent, env));
	ret->SetValue(L"toLocaleUpperCase", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringToLocaleUpperCase, parent, env));
	ret->SetValue(L"toLowerCase", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringToLowerCase, parent, env));
	ret->SetValue(L"toString", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringToString, parent, env));
	ret->SetValue(L"toUpperCase", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringToUpperCase, parent, env));
	ret->SetValue(L"trim", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringTrim, parent, env));
	ret->SetValue(L"trimStart", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringTrimStart, parent, env));

	ret->SetValue(L"trimEnd", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringTrimEnd, parent, env));
	ret->SetValue(L"valueOf", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringValueOf, parent, env));
	ret->SetValue(L"iterator", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsString::JsStringIterator, parent, env));

	return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}

void JsString::JsStringFromCharCode(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TString res;
	for (UINT Rust = 0; Rust < params.Size(); Rust++)
	{
		auto num = params[Rust];
		if (num.Get() && num->GetVarType() == var_type::primitive)
		{
			WCHAR c = static_cast<WCHAR>(num->Get4Value());
			res.AppendChar(c);
		}
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(res);
}

void JsString::JsStringFromCodePoint(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringRaw(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}


bool InspectObjParam(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
{
	if (!params.Size() || !params[0].Get())
	{
		ret.returnCode = ret.ERR_BROKEN_REF;
		return false;
	}
	if (params[0]->GetVarType() != var_type::string) {
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		return false;
	}
	return true;
}

void JsString::JsStringAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	UINT uIndex = params.Size() > 1 && params[1].Get() ? params[1]->Get4Value() : 0;

	int index = 0;
	memcpy_s(&index, sizeof(index), &uIndex, sizeof(uIndex));

	uIndex = static_cast<UINT>(index);

	if (uIndex >= actString.GetSize())
		ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
	else {
		if (index < 0)
			uIndex = actString.GetSize() + index;
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(actString[uIndex]);
	}
}

void JsString::JsStringCharAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	UINT uIndex = params.Size() > 1 && params[1].Get() ? params[1]->Get4Value() : 0;

	int index = 0;
	memcpy_s(&index, sizeof(index), &uIndex, sizeof(uIndex));

	uIndex = static_cast<UINT>(index);

	if (uIndex >= actString.GetSize())
		ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
	else {
		if (index < 0)
			uIndex = actString.GetSize() + index;
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString[uIndex]);
	}
}

void JsString::JsStringCharCodeAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	UINT uIndex = params.Size() > 1 && params[1].Get() ? params[1]->Get4Value() : 0;

	int index = 0;
	memcpy_s(&index, sizeof(index), &uIndex, sizeof(uIndex));

	uIndex = static_cast<UINT>(index);

	if (uIndex >= actString.GetSize())
		ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
	else {
		if (index < 0)
			uIndex = actString.GetSize() + index;
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<int>(actString[uIndex]));
	}
}

void JsString::JsStringCodePointAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{

}

void JsString::JsStringConcat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	for (UINT Rust = 1; Rust < params.Size(); Rust++)
	{
		actString.Append(params[Rust].Get() ? params[Rust]->GetString() : L"null");
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString);
}

void JsString::JsStringIncludes(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	TString finder((params.Size() > 1 && params[1].Get()) ? params[1]->GetString() : L"");

	UINT startIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : 0;

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString.Find(finder, startIndex) != -1);
}

void JsString::JsStringEndsWith(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	TString finder((params.Size() > 1 && params[1].Get()) ? params[1]->GetString() : L"");

	UINT startIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : actString.GetSize();

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(startIndex < finder.GetSize() ? false : actString.EndsWith(finder));
}

void JsString::JsStringIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	TString finder((params.Size() > 1 && params[1].Get()) ? params[1]->GetString() : L"");

	UINT startIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : 0;

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString.Find(finder, startIndex));
}

void JsString::JsStringLastIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	TString finder((params.Size() > 1 && params[1].Get()) ? params[1]->GetString() : L"");

	UINT startIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : 0;

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString.FindLast(finder, startIndex));
}

void JsString::JsStringLocaleCompare(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringMatch(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringMatchAll(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringNormalize(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringPadEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	UINT startIndex = (params.Size() > 1 && params[1].Get()) ? params[1]->Get4Value() : 0;
	TString appender((params.Size() > 2 && params[2].Get()) ? params[2]->GetString() : L"");

	for (int diff = static_cast<int>(startIndex) - static_cast<int>(actString.GetSize()); diff > 0; diff = static_cast<int>(startIndex) - static_cast<int>(actString.GetSize()))
	{
		actString.Append(appender.SubString(0, diff));
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString);
}

void JsString::JsStringPadStart(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;

	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString(stringObj->GetString());

	UINT startIndex = (params.Size() > 1 && params[1].Get()) ? params[1]->Get4Value() : 0;
	TString appender((params.Size() > 2 && params[2].Get()) ? params[2]->GetString() : L"");

	for (int diff = static_cast<int>(startIndex) - static_cast<int>(actString.GetSize()); diff > 0; diff = static_cast<int>(startIndex) - static_cast<int>(actString.GetSize()))
	{
		actString.Set(appender.SubString(0, diff) + actString);
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString);
}

void JsString::JsStringRepeat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString actString;

	if (params.Size() > 1 && params[1].Get() && params[1]->GetVarType() == var_type::primitive)
	{
		UINT param = params[1]->Get4Value();

		actString.Set(stringObj->GetString());
		TString appender(actString);

		UINT max = INFINITE / actString.GetSize();

		if (param >= max)
		{
			ret.returnCode = ret.ERR_GENERIC_ERROR;
			ret.errorMessage.Set("RANGE Error!");
			return;
		}

		for (UINT Rust = 1; Rust < param; Rust++)
		{
			actString.Append(appender);
		}
	}

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(actString);
}

void JsString::JsStringReplace(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringReplaceAll(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringSearch(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringSlice(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	UINT startIndex = (params.Size() > 1 && params[1].Get()) ? params[1]->Get4Value() : 0;
	UINT endIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : 0;

	int iStart = 0, iEnd = 0;

	if (startIndex & 0x80000000)
		iStart = -(startIndex & 0x7FFFFFFF);
	else iStart = startIndex;

	if (endIndex & 0x80000000)
		iEnd = -(endIndex & 0x7FFFFFFF);
	else iEnd = endIndex;

	if (iStart <= 0)
		iStart = static_cast<int>(stringObj->GetString().GetSize()) + iStart;

	if (iEnd <= 0)
		iEnd = static_cast<int>(stringObj->GetString().GetSize()) + iEnd;

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString().SubString(iStart, iEnd));
}

void JsString::JsStringIterator(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringSplit(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	TString token;
	boolean stringPresented = false;
	if (params.Size() > 1 && params[1].Get()) {
		token.Set(params[1]->GetString());
		stringPresented = true;
	}

	int endIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : -1;

	TrecSubPointer<TVariable, TContainerVariable> strArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	if (!stringPresented)
	{
		strArray->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString()));
		ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(strArray);
		return;
	}

	if (endIndex == -1)
	{
		for (UINT Rust = 0; Rust < stringObj->GetString().GetSize(); Rust++)
		{
			strArray->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(TString(stringObj->GetString()[Rust])));
		}
		ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(strArray);
		return;
	}

	if (endIndex == 0)
	{
		ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(strArray);
		return;
	}

	TrecPointer<TDataArray<TString>> splits = stringObj->GetString().splitn(token, endIndex);
	for (UINT Rust = 0; Rust < splits->Size(); Rust++)
	{
		strArray->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(splits->at(Rust)));
	}
	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(strArray);
}

void JsString::JsStringStartsWith(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	TString starter((params.Size() > 1 && params[1].Get()) ? params[1]->GetString() : L"");
	UINT startIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : 0;
	
	TString actString(stringObj->GetString());
	if(startIndex)
	actString.Delete(0, startIndex);

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(actString.StartsWith(starter));
}

void JsString::JsStringSubString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	UINT startIndex = (params.Size() > 1 && params[1].Get()) ? params[1]->Get4Value() : 0;
	UINT endIndex = (params.Size() > 2 && params[2].Get()) ? params[2]->Get4Value() : 0;

	int iStart = 0, iEnd = 0;

	if (startIndex & 0x80000000)
		iStart = -(startIndex & 0x7FFFFFFF);
	else iStart = startIndex;

	if (endIndex & 0x80000000)
		iEnd = -(endIndex & 0x7FFFFFFF);
	else iEnd = endIndex;

	if (iStart <= 0)
		iStart = static_cast<int>(stringObj->GetString().GetSize()) + iStart;

	if (iEnd <= 0)
		iEnd = static_cast<int>(stringObj->GetString().GetSize()) + iEnd;

	if (iEnd < iStart)
	{
		int temp = iEnd;
		iEnd = iStart;
		iStart = temp;
	}

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString().SubString(iStart, iEnd));
}

void JsString::JsStringToLocaleLowerCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringToLocaleUpperCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringToLowerCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString().GetLower());
}

void JsString::JsStringToUpperCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString().GetUpper());
}

void JsString::JsStringToString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString());
}

void JsString::JsStringTrim(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString().GetTrim());
}

void JsString::JsStringTrimStart(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString().GetTrimLeft());
}

void JsString::JsStringTrimEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString().GetTrimRight());
}

void JsString::JsStringValueOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!InspectObjParam(params, ret)) return;
	TrecSubPointer<TVariable, TStringVariable> stringObj = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringObj->GetString());
}
