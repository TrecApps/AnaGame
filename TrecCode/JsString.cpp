#include "JsString.h"
#include <TContainerVariable.h>
#include <TcNativeInterpretor.h>

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
}

void JsString::JsStringFromCodePoint(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringRaw(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringCharAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringCharCodeAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringCodePointAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringConcat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringIncludes(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringEndsWith(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringLastIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
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
}

void JsString::JsStringPadStart(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringRepeat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
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
}

void JsString::JsStringIterator(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringSplit(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringStartsWith(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringSubString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringToLocaleLowerCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringToLocaleUpperCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringToLowerCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringToUpperCase(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringToString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringTrim(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringTrimStart(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringTrimEnd(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsString::JsStringValueOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}
