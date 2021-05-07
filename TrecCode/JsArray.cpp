#include "JsArray.h"
#include <TContainerVariable.h>
#include <TStringVariable.h>
#include <TPrimitiveVariable.h>
#include <DefaultObjectOperator.h>
#include <TSpecialVariable.h>
#include "TcJavaScriptInterpretor.h"

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


// STATIC ARRAY METHODS
void JsArray::JsArrayFrom(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!params.Size())
	{
		ret.errorMessage.Set(L"Uncaught TypeError: undefined is not iterable (cannot read property Symbol(Symbol.iterator))");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	TrecSubPointer<TVariable, TContainerVariable> con = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	TrecSubPointer<TVariable, TStringVariable> str = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(params[0]);

	if (con.Get())
	{
		TString tempStr; TrecPointer<TVariable> tempVar;
		for (UINT Rust = 0; con->GetValueAt(Rust, tempStr, tempVar); Rust++)
		{
			rreturnArray->AppendValue(tempVar);
		}
	}
	else if (str.Get())
	{
		TString v(str->GetString());
		for (UINT Rust = 0; Rust < v.GetSize(); Rust++)
		{
			rreturnArray->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(v.GetAt(Rust)));
		}
	}

	if (params.Size() > 1)
	{
		TrecSubPointer<TVariable, TcInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[1]);

		if (!func.Get())
		{
			ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer() : L"null");
			ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
			return;
		}
		TString tempStr; TrecPointer<TVariable> tempVar;
		for (UINT Rust = 0; rreturnArray->GetValueAt(Rust, tempStr, tempVar); Rust++)
		{
			
			TDataArray<TrecPointer<TVariable>> paramSub;
			paramSub.push_back(tempVar);
			paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
			paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray));
			func->SetIntialVariables(paramSub);

			ret = func->Run();
			if (ret.returnCode)
				return;
			rreturnArray->SetValue(Rust, ret.errorObject);
		}
	}

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray);
}

void JsArray::JsArrayIsArray(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	bool isA = params.Size() && dynamic_cast<TContainerVariable*>(params[0].Get()) &&
		dynamic_cast<TContainerVariable*>(params[0].Get())->GetContainerType() == ContainerType::ct_array;
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(isA);
}

void JsArray::JsArrayOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	for (UINT Rust = 0; Rust < params.Size(); Rust++)
	{
		rreturnArray->AppendValue(params[0]);
	}
	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray);
}


// Helper function
bool CheckMethodObject(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
{
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray;
	if (params.Size())
		rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (!rreturnArray.Get() || rreturnArray->GetContainerType() != ContainerType::ct_array)
	{
		ret.errorMessage.Set(L"INTERNAL ERROR! Expected 'this' to be an array object!");
		ret.returnCode = ReturnObject::ERR_INTERNAL;
		return false;
	}
	return true;
}



// INSTANCE ARRAY METHODS
void JsArray::JsArrayAt(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray= TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (params.Size() > 1)
	{
		DoubleLong dl = DoubleLong::GetValueFromPrimitive(params[1]);
		LONG64 index = 0;
		switch (dl.type)
		{
		case double_long::dl_double:
			index = static_cast<LONG64>(dl.value.d);
			break;
		case double_long::dl_sign:
			index = dl.value.s;
			break;
		case double_long::dl_unsign:
			index = static_cast<LONG64>(dl.value.u);
			break;
		default:
			ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
			return;
		}

		if (index < 0)
		{
			index = rreturnArray->GetSize() + index;
			TString tempName;
			if(!rreturnArray->GetValueAt(index, tempName, ret.errorObject))
				ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
		}
	}
	else
	{
		ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
		return;
	}
}

void JsArray::JsArrayConcat(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	for (UINT Rust = 0; Rust < params.Size(); Rust++)
	{
		if (params[Rust].Get())
		{
			TrecSubPointer<TVariable, TContainerVariable> pContain = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[Rust]);
			if (pContain.Get())
			{
				TString tName;
				TrecPointer<TVariable> tVar;
				for (UINT C = 0; pContain->GetValueAt(C, tName, tVar); C++)
					rreturnArray->AppendValue(tVar);
			}
			else
				rreturnArray->AppendValue(params[Rust]->Clone());
		}
		else {
			rreturnArray->AppendValue(params[Rust]);
		}
	}

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray);
}

// To-Do
void JsArray::JsArrayCopyWithin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}

// To-Do
void JsArray::JsArrayEntries(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}

void JsArray::JsArrayEvery(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	bool affirm = true;
	TString tempStr; TrecPointer<TVariable> tempVar;
	auto func = dynamic_cast<TcInterpretor*>(params[1].Get());
	for (UINT Rust = 0; rreturnArray->GetValueAt(Rust, tempStr, tempVar) && affirm; Rust++)
	{

		TDataArray<TrecPointer<TVariable>> paramSub;
		paramSub.push_back(tempVar);
		paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
		paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray));
		func->SetIntialVariables(paramSub);

		ret = func->Run();
		if (ret.returnCode)
			return;
		affirm = TcJavaScriptInterpretor::IsTruthful(ret.errorObject);
	}

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(affirm);
}

// To-Do
void JsArray::JsArrayFill(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

}

void JsArray::JsArrayFilter(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	bool affirm = true;
	TString tempStr; TrecPointer<TVariable> tempVar;
	auto func = dynamic_cast<TcInterpretor*>(params[1].Get());
	for (UINT Rust = 0; methodArray->GetValueAt(Rust, tempStr, tempVar) && affirm; Rust++)
	{

		TDataArray<TrecPointer<TVariable>> paramSub;
		paramSub.push_back(tempVar);
		paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
		paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(methodArray));
		func->SetIntialVariables(paramSub);

		ret = func->Run();
		if (ret.returnCode)
			return;
		if(TcJavaScriptInterpretor::IsTruthful(ret.errorObject))
			rreturnArray->AppendValue(tempVar);
	}

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray);
}

void JsArray::JsArrayFind(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	bool affirm = true;
	TString tempStr; TrecPointer<TVariable> tempVar;
	auto func = dynamic_cast<TcInterpretor*>(params[1].Get());
	for (UINT Rust = 0; methodArray->GetValueAt(Rust, tempStr, tempVar) && affirm; Rust++)
	{

		TDataArray<TrecPointer<TVariable>> paramSub;
		paramSub.push_back(tempVar);
		paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
		paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(methodArray));
		func->SetIntialVariables(paramSub);

		ret = func->Run();
		if (ret.returnCode)
			return;
		if (TcJavaScriptInterpretor::IsTruthful(ret.errorObject))
		{
			ret.errorObject = tempVar;
			return;
		}
	}

	ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
}

void JsArray::JsArrayFindIndex(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	bool affirm = true;
	TString tempStr; TrecPointer<TVariable> tempVar;
	auto func = dynamic_cast<TcInterpretor*>(params[1].Get());
	for (UINT Rust = 0; methodArray->GetValueAt(Rust, tempStr, tempVar) && affirm; Rust++)
	{

		TDataArray<TrecPointer<TVariable>> paramSub;
		paramSub.push_back(tempVar);
		paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
		paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(methodArray));
		func->SetIntialVariables(paramSub);

		ret = func->Run();
		if (ret.returnCode)
			return;
		if (TcJavaScriptInterpretor::IsTruthful(ret.errorObject))
		{
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust);
			return;
		}
	}

	ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
}

void JsArray::JsArrayForEach(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	bool affirm = true;
	TString tempStr; TrecPointer<TVariable> tempVar;
	auto func = dynamic_cast<TcInterpretor*>(params[1].Get());
	for (UINT Rust = 0; methodArray->GetValueAt(Rust, tempStr, tempVar) && affirm; Rust++)
	{

		TDataArray<TrecPointer<TVariable>> paramSub;
		paramSub.push_back(tempVar);
		paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
		paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(methodArray));
		func->SetIntialVariables(paramSub);

		ret = func->Run();
		if (ret.returnCode)
			return;
	}

	ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
}

void JsArray::JsArrayIncludes(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}

void JsArray::JsArrayIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayJoin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayKeys(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayLastIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayMap(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayPop(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayPush(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayReduce(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayReduceRight(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayReverse(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayShift(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArraySlice(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArraySome(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArraySort(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArraySplice(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayToLocaleString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayToString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayUnshift(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}
void JsArray::JsArrayValues(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
}