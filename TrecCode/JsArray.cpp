#include "JsArray.h"
#include <TContainerVariable.h>
#include <TStringVariable.h>
#include <TPrimitiveVariable.h>
#include <DefaultObjectOperator.h>
#include <TSpecialVariable.h>
#include "TcJavaScriptInterpretor.h"
#include <TIteratorVariable.h>

JsObjectOperator jsOps;


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
			ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
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

void JsArray::JsArrayCopyWithin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	DoubleLong p1, p2, p3;
	if (params.Size() < 2)
	{
		ret.returnCode = ret.ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set("Missing Parameter");
		return;
	}

	p1 = DoubleLong::GetValueFromPrimitive(params[1]);
	if (params.Size() >= 3)
		p2 = DoubleLong::GetValueFromPrimitive(params[2]);
	if (params.Size() >= 4)
		p3 = DoubleLong::GetValueFromPrimitive(params[3]);

	int int1 = 0, int2 = 0, int3 = 0;
	switch (p1.type)
	{
	case double_long::dl_double:
		int1 = static_cast<int>(p1.value.d);
		break;
	case double_long::dl_sign:
		int1 = static_cast<int>(p1.value.s);
		break;
	case double_long::dl_unsign:
		int1 = static_cast<int>(p1.value.u);
		break;
	default:
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Set("First Parameter must be a number");
		return;
	}

	switch (p2.type)
	{
	case double_long::dl_double:
		int2 = static_cast<int>(p2.value.d);
		break;
	case double_long::dl_sign:
		int2 = static_cast<int>(p2.value.s);
		break;
	case double_long::dl_unsign:
		int2 = static_cast<int>(p2.value.u);
		break;
	default:
		int2 = 0;
	}

	switch (p3.type)
	{
	case double_long::dl_double:
		int3 = static_cast<int>(p3.value.d);
		break;
	case double_long::dl_sign:
		int3 = static_cast<int>(p3.value.s);
		break;
	case double_long::dl_unsign:
		int3 = static_cast<int>(p3.value.u);
		break;
	default:
		int3 = rreturnArray->GetSize();
	}

	if (int1 >= rreturnArray->GetSize())
		return;

	if (int1 < 0)
		int1 = rreturnArray->GetSize() + int1;
	if (int1 < 0)
		return;

	if (int2 < 0)
		int2 = rreturnArray->GetSize() + int2;
	if (int2 < 0)
		int2 = 0;
	if (int3 < 0)
		int3 = rreturnArray->GetSize() + int3;
	if (int3 < 0)
		int3 = rreturnArray->GetSize();


	TDataArray<TrecPointer<TVariable>> tempCopy;

	for (UINT Rust = int2; Rust < int3; Rust++)
	{
		tempCopy.push_back(rreturnArray->GetValueAt(Rust));
	}

	for (UINT Rust = int1, C = 0; Rust < rreturnArray->GetSize() && C < tempCopy.Size(); Rust++, C++)
	{
		rreturnArray->SetValue(Rust, tempCopy[C], true);
	}
}

void JsArray::JsArrayEntries(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	ret.errorObject = rreturnArray->GetIterator();
}

void JsArray::JsArrayEvery(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
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

void JsArray::JsArrayFill(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	DoubleLong p2, p3;
	if (params.Size() >= 3)
		p2 = DoubleLong::GetValueFromPrimitive(params[2]);
	if (params.Size() >= 4)
		p3 = DoubleLong::GetValueFromPrimitive(params[3]);

	int int2 = 0, int3 = 0;


	switch (p2.type)
	{
	case double_long::dl_double:
		int2 = static_cast<int>(p2.value.d);
		break;
	case double_long::dl_sign:
		int2 = static_cast<int>(p2.value.s);
		break;
	case double_long::dl_unsign:
		int2 = static_cast<int>(p2.value.u);
		break;
	default:
		int2 = 0;
	}

	switch (p3.type)
	{
	case double_long::dl_double:
		int3 = static_cast<int>(p3.value.d);
		break;
	case double_long::dl_sign:
		int3 = static_cast<int>(p3.value.s);
		break;
	case double_long::dl_unsign:
		int3 = static_cast<int>(p3.value.u);
		break;
	default:
		int3 = rreturnArray->GetSize();
	}

	if (int2 < 0)
		int2 = rreturnArray->GetSize() + int2;
	if (int2 < 0)
		int2 = 0;
	if (int3 < 0)
		int3 = rreturnArray->GetSize() + int3;
	if (int3 < 0)
		int3 = rreturnArray->GetSize();

	TrecPointer<TVariable> p1 = params.Size() > 1 ? params[1] : TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);

	for(; int2 < int3; int2++)
		rreturnArray->SetValue(int2, p1.Get() ? p1->Clone() : p1, true);
}

void JsArray::JsArrayFilter(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
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
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
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
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
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

	if (params.Size() < 2 || !dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
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
	TrecSubPointer<TVariable, TContainerVariable> methodObject = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	bool result = false;

	if (params.Size() > 1)
	{
		UINT Rust = (params.Size() > 2 && dynamic_cast<TPrimitiveVariable*>(params[2].Get())) ? dynamic_cast<TPrimitiveVariable*>(params[2].Get())->Get4Value() : 0;
		TString tName;
		TrecPointer<TVariable> tVal;
		for (; methodObject->GetValueAt(Rust, tName, tVal); Rust++)
		{
			auto eqRes = jsOps.Equality(params[1], tVal, equality_op::eo_equals);
			if (TcJavaScriptInterpretor::IsTruthful(eqRes))
			{
				result = true; break;
			}
		}
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(result);
}

void JsArray::JsArrayIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodObject = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	int result = 0;

	if (params.Size() > 1)
	{
		UINT Rust = (params.Size() > 2 && dynamic_cast<TPrimitiveVariable*>(params[2].Get())) ? dynamic_cast<TPrimitiveVariable*>(params[2].Get())->Get4Value() : 0;
		TString tName;
		TrecPointer<TVariable> tVal;
		for (; methodObject->GetValueAt(Rust, tName, tVal); Rust++)
		{
			auto eqRes = jsOps.Equality(params[1], tVal, equality_op::eo_equals);
			if (TcJavaScriptInterpretor::IsTruthful(eqRes))
			{
				result = Rust; break;
			}
		}
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(result);
}


void JsArray::JsArrayJoin(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodObject = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	TString seperator((params.Size() > 1 && params[1].Get())? params[1]->GetString() : L',');

	TString result;

	if (params.Size() > 1)
	{
		TString tName;
		TrecPointer<TVariable> tVal;
		for (UINT Rust = 0; methodObject->GetValueAt(Rust, tName, tVal); Rust++)
		{
			if (Rust)
				result.Append(seperator);
			result.Append(tVal.Get() ? tVal->GetString() : L"null");
		}
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(result);
}

void JsArray::JsArrayKeys(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	JsArrayEntries(params, env, ret);
}

void JsArray::JsArrayLastIndexOf(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodObject = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	int result = 0;

	if (params.Size() > 1)
	{
		UINT Rust = (params.Size() > 2 && dynamic_cast<TPrimitiveVariable*>(params[2].Get())) ? dynamic_cast<TPrimitiveVariable*>(params[2].Get())->Get4Value() : methodObject->GetSize() -1;
		TString tName;
		TrecPointer<TVariable> tVal;
		for (; methodObject->GetValueAt(Rust, tName, tVal); Rust--)
		{
			auto eqRes = jsOps.Equality(params[1], tVal, equality_op::eo_equals);
			if (TcJavaScriptInterpretor::IsTruthful(eqRes))
			{
				result = Rust; break;
			}
		}
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(result);
}

void JsArray::JsArrayMap(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
	if (params.Size() > 1)
	{
		TrecSubPointer<TVariable, TcInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[1]);

		if (!func.Get())
		{
			ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
			ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
			return;
		}
		TString tempStr; TrecPointer<TVariable> tempVar;
		for (UINT Rust = 0; methodArray->GetValueAt(Rust, tempStr, tempVar); Rust++)
		{

			TDataArray<TrecPointer<TVariable>> paramSub;
			paramSub.push_back(tempVar);
			paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
			paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(methodArray));
			func->SetIntialVariables(paramSub);

			ret = func->Run();
			if (ret.returnCode)
				return;
			rreturnArray->AppendValue(ret.errorObject);
		}
	}

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray);
}

void JsArray::JsArrayPop(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) 
{
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	UINT size = rreturnArray->GetSize();
	if (size)
	{
		bool pres;
		ret.errorObject = rreturnArray->GetValue(size - 1,pres);
		TString strIndex;
		strIndex.Format(L"%d", size - 1);
		rreturnArray->RemoveByKey(strIndex);
	}
	else
		ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
}

void JsArray::JsArrayPush(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodObject = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	for (UINT Rust = 1; Rust < params.Size(); Rust++)
	{
		methodObject->AppendValue(params[Rust].Get() ? params[Rust]->Clone() : params[Rust]);
	}

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(methodObject->GetSize());

}


void JsArray::JsArrayReduce(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	auto func = dynamic_cast<TcInterpretor*>(params[1].Get());

	for (UINT Rust = 0; Rust < rreturnArray->GetSize() - 1;)
	{
		TDataArray<TrecPointer<TVariable>> paramSub;
		paramSub.push_back(rreturnArray->GetValueAt(Rust));
		paramSub.push_back(rreturnArray->GetValueAt(Rust + 1));
		paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust));
		paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray));

		func->SetIntialVariables(paramSub);

		ret = func->Run();
		if (ret.returnCode)
			return;

		rreturnArray->SetValue(Rust, ret.errorObject);
		ret.errorObject.Nullify();
		TString rustStr;
		rustStr.Format(L"%d", Rust + 1);
		rreturnArray->RemoveByKey(rustStr);
	}
}

void JsArray::JsArrayReduceRight(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	auto func = dynamic_cast<TcInterpretor*>(params[1].Get());

	for (UINT Rust = rreturnArray->GetSize(); Rust > 0; Rust--)
	{
		TDataArray<TrecPointer<TVariable>> paramSub;
		paramSub.push_back(rreturnArray->GetValueAt(Rust));
		paramSub.push_back(rreturnArray->GetValueAt(Rust - 1));
		paramSub.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(Rust - 1));
		paramSub.push_back(TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray));

		func->SetIntialVariables(paramSub);

		ret = func->Run();
		if (ret.returnCode)
			return;

		rreturnArray->SetValue(Rust -1, ret.errorObject);
		ret.errorObject.Nullify();
		TString rustStr;
		rustStr.Format(L"%d", Rust);
		rreturnArray->RemoveByKey(rustStr);
	}
}

void JsArray::JsArrayReverse(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> methodArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
	TString tName;
	TrecPointer<TVariable> tVal;
	for (UINT Rust = methodArray->GetSize() - 1; methodArray->GetValueAt(Rust, tName, tVal); Rust--)
	{
		rreturnArray->AppendValue(tVal);
	}

	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray);
}
void JsArray::JsArrayShift(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	UINT size = rreturnArray->GetSize();
	if (size)
	{
		bool pres;
		ret.errorObject = rreturnArray->GetValue( 0, pres);
		TString strIndex;
		strIndex.Format(L"%d", 0);
		rreturnArray->RemoveByKey(strIndex);
	}
	else
		ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
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
	if (params.Size() < 2 || dynamic_cast<TcInterpretor*>(params[1].Get()))
	{
		ret.errorMessage.Format(L"Uncaught TypeError: %ws is not a function", params[1].Get() ? params[1]->GetString().GetConstantBuffer().getBuffer() : L"null");
		ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
		return;
	}

	bool affirm = false;
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
		if (TcJavaScriptInterpretor::IsTruthful(ret.errorObject))
		{
			affirm = true;
			break;
		}
	}

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(affirm);
}

void JsArray::JsArraySort(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);
	TrecSubPointer<TVariable, TcInterpretor> sortFunc;
	if (params.Size() > 1)
	{
		sortFunc = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(params[1]);
		if (!sortFunc.Get())
		{
			ret.returnCode = ret.ERR_IMPROPER_TYPE;
			return;
		}
	}

	for (UINT Rust = 0; Rust < rreturnArray->GetSize(); Rust++)
	{
		for (UINT C = 0; C < rreturnArray->GetSize() - 1; C++)
		{
			int res = 0;
			TrecPointer<TVariable> var1 = rreturnArray->GetValueAt(C), var2 = rreturnArray->GetValueAt(C + 1);
			if (sortFunc.Get())
			{
				TDataArray<TrecPointer<TVariable>> paramSub;
				paramSub.push_back(var1);
				paramSub.push_back(var2);
				sortFunc->SetIntialVariables(paramSub);
				ret = sortFunc->Run();
				if (ret.returnCode)
					return;

				DoubleLong dlRes = DoubleLong::GetValueFromPrimitive(ret.errorObject);
				switch (dlRes.type)
				{
				case double_long::dl_double:
					res = static_cast<int>(dlRes.value.d);
					break;
				case double_long::dl_sign:
					res = static_cast<int>(dlRes.value.s);
					break;
				case double_long::dl_unsign:
					res = static_cast<int>(dlRes.value.u);
					break;
				default:
					ret.returnCode = ret.ERR_IMPROPER_TYPE;
					return;
				}


			}
			else
			{
				TString val1(var1.Get() ? var1->GetString() : L"null"), val2(var2.Get() ? var2->GetString() : L"null");

				res = val1.Compare(val2);
			}

			if (res > 0)
			{
				// if result is above 0, do the swap
				rreturnArray->SetValue(C, var2);
				rreturnArray->SetValue(C + 1, var1);
			}
		}
	}
	// Array need to be the return value
	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(rreturnArray);
} 

void JsArray::JsArraySplice(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	DoubleLong p1, p2;
	if (params.Size() < 2)
	{
		ret.returnCode = ret.ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set("Missing Parameter");
		return;
	}

	p1 = DoubleLong::GetValueFromPrimitive(params[1]);
	if (params.Size() >= 3)
		p2 = DoubleLong::GetValueFromPrimitive(params[2]);

	int int1 = 0, int2 = 0;
	switch (p1.type)
	{
	case double_long::dl_double:
		int1 = static_cast<int>(p1.value.d);
		break;
	case double_long::dl_sign:
		int1 = static_cast<int>(p1.value.s);
		break;
	case double_long::dl_unsign:
		int1 = static_cast<int>(p1.value.u);
		break;
	default:
		ret.returnCode = ret.ERR_IMPROPER_TYPE;
		ret.errorMessage.Set("First Parameter must be a number");
		return;
	}

	switch (p2.type)
	{
	case double_long::dl_double:
		int2 = static_cast<int>(p2.value.d);
		break;
	case double_long::dl_sign:
		int2 = static_cast<int>(p2.value.s);
		break;
	case double_long::dl_unsign:
		int2 = static_cast<int>(p2.value.u);
		break;
	default:
		int2 = 0;
	}

	if (int1 < 0)
		int1 = rreturnArray->GetSize() + int1;
	if (int1 < 0)
		int1 = 0;

	if (int2 < 0)
		int2 = rreturnArray->GetSize() + int2;
	if (int2 < 0)
		int2 = 0;

	UINT Rust = 0;
	auto tempCon = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
	auto delCon = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
	for (; Rust <= int1 && Rust < rreturnArray->GetSize(); Rust++)
	{
		tempCon->AppendValue(rreturnArray->GetValueAt(Rust));
	}

	for (UINT C = 3; C < params.Size(); C++)
	{
		tempCon->AppendValue(params[C]);
	}

	for (UINT C = 0; C < int2 && Rust < rreturnArray->GetSize(); C++, Rust++)
	{
		delCon->AppendValue(rreturnArray->GetValueAt(Rust));
	}

	for (; Rust < rreturnArray->GetSize(); Rust++)
	{
		tempCon->AppendValue(rreturnArray->GetValueAt(Rust));
	}
	rreturnArray->Clear();

	for (Rust = 0; tempCon->GetSize(); Rust++)
	{
		rreturnArray->AppendValue(tempCon->GetValueAt(Rust));
	}
	ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(delCon);
}

void JsArray::JsArrayToLocaleString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	/*if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);*/
	JsArrayToString(params, env, ret);

}

void JsArray::JsArrayToString(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	TString returnValue;
	for (UINT Rust = 0; Rust < rreturnArray->GetSize(); Rust++)
	{
		auto var = rreturnArray->GetValueAt(Rust);
		if (returnValue.GetSize())
			returnValue.AppendChar(L',');

		returnValue.Append(var.Get() ? var->GetString() : L"null");
	}

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(returnValue);
}

void JsArray::JsArrayUnshift(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	if (!CheckMethodObject(params, ret))
		return;
	TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	auto tempCon = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

	for (UINT Rust = 1; Rust < params.Size(); Rust++)
		tempCon->AppendValue(params[Rust]);

	for (UINT Rust = 0; Rust < rreturnArray->GetSize(); Rust++)
		tempCon->AppendValue(rreturnArray->GetValueAt(Rust));

	rreturnArray->Clear();
	for (UINT Rust = 0; Rust < tempCon->GetSize(); Rust++)
	{
		rreturnArray->AppendValue(tempCon->GetValueAt(Rust));
	}

	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(rreturnArray->GetSize());
} 

void JsArray::JsArrayValues(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret) {
	//if (!CheckMethodObject(params, ret))
	//	return;
	//TrecSubPointer<TVariable, TContainerVariable> rreturnArray = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(params[0]);

	JsArrayEntries(params, env, ret);
}