#include "JsArray.h"
#include <TContainerVariable.h>

TrecPointer<TVariable> JsArray::GetJsArrayMehtods(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
{

	TrecSubPointer<TVariable, TContainerVariable> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

	// Static
	ret->SetValue(L"from", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayFrom, parent, env));
	ret->SetValue(L"isArray", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayIsArray, parent, env));
	ret->SetValue(L"of", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayOf, parent, env));

	// Instance
	ret->SetValue(L"at", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayAt, parent, env));
	ret->SetValue(L"concat", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayConcat, parent, env));
	ret->SetValue(L"copyWithin", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayCopyWithin, parent, env));
	ret->SetValue(L"entries", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayEntries, parent, env));
	ret->SetValue(L"every", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayEvery, parent, env));
	ret->SetValue(L"fill", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayFill, parent, env));
	ret->SetValue(L"filter", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayFilter, parent, env));
	ret->SetValue(L"find", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayFind, parent, env));
	ret->SetValue(L"findIndex", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayFindIndex, parent, env));
	ret->SetValue(L"forEach", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayForEach, parent, env));
	ret->SetValue(L"includes", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayIncludes, parent, env));
	ret->SetValue(L"indexOf", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayIndexOf, parent, env));
	ret->SetValue(L"join", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayJoin, parent, env));
	ret->SetValue(L"keys", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayKeys, parent, env));
	ret->SetValue(L"lastIndexOf", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayLastIndexOf, parent, env));

	ret->SetValue(L"map", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayMap, parent, env));
	ret->SetValue(L"pop", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayPop, parent, env));
	ret->SetValue(L"push", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayPush, parent, env));
	ret->SetValue(L"reduce", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayReduce, parent, env));
	ret->SetValue(L"reduceRight", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayReduceRight, parent, env));
	ret->SetValue(L"reverse", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayReverse, parent, env));
	ret->SetValue(L"shift", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayShift, parent, env));
	ret->SetValue(L"slice", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArraySlice, parent, env));
	ret->SetValue(L"some", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArraySome, parent, env));
	ret->SetValue(L"sort", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArraySort, parent, env));
	ret->SetValue(L"splice", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArraySplice, parent, env));
	ret->SetValue(L"toLocaleString", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayToLocaleString, parent, env));
	ret->SetValue(L"toString", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayToString, parent, env));
	ret->SetValue(L"unshift", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayUnshift, parent, env));
	ret->SetValue(L"values", TrecPointerKey::GetNewSelfTrecPointerAlt < TVariable, TcNativeInterpretor>(JsArray::JsArrayValues, parent, env));


	return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}

void JsArray::JsArrayFrom(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayIsArray(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayConcat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayCopyWithin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayEntries(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayEvery(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayFill(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{

}

void JsArray::JsArrayFilter(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayFind(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayFindIndex(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayForEach(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayIncludes(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsArray::JsArrayIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayJoin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayKeys(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayLastIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayMap(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayPop(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayPush(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayReduce(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayReduceRight(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayReverse(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayShift(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArraySlice(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArraySome(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArraySort(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArraySplice(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayToLocaleString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayToString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayUnshift(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }
void JsArray::JsArrayValues(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) { }