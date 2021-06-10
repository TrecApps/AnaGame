#include "JsPromise.h"

TClassStruct JsPromise::GetPromiseClass(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
{
	TClassStruct ret;

	TClassAttribute att;
	att.name.Set(L"constructor");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseConstructor, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"then");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseThen, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"catch");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseCatch, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"finally");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseFinally, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"all");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseAll, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"allSettled");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseAllSettled, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"any");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseAny, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"race");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseRace, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"reject");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseReject, parent, env);
	ret.AddAttribute(att);

	att.name.Set(L"resolve");
	att.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsPromiseResolve, parent, env);
	ret.AddAttribute(att);

	return ret;
}

void JsPromise::JsPromiseConstructor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseCatch(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseThen(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseFinally(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseAll(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseAllSettled(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseAny(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseRace(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseReject(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}

void JsPromise::JsPromiseResolve(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
}
