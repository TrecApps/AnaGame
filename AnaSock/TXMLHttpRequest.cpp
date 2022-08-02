#include "TXMLHttpRequest.h"
#include <TPrimitiveVariable.h>
#include <TcNativeInterpretor.h>
#include <TObjectVariable.h>
#include <TSpecialVariable.h>
#include <TStringVariable.h>

namespace JSXmlHttpRequest
{
	bool IsValidObjectType(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (!params.Size() || !params[0].Get() || params[0]->GetVarType() != var_type::native_object)
		{
			ret.returnCode = ret.ERR_INTERNAL;
			ret.errorMessage.Set(L"Missing Object for Method Call!");
			return false;
		}

		TrecSubPointer<TVariable, TObjectVariable> oVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TObjectVariable>(params[0]);

		TrecObjectPointer oPoint = oVar->GetObjectW();
		if (!dynamic_cast<TXMLHttpRequest*>(oPoint.Get()))
			return false;
		return true;
	}

	void JsConstructor(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(
			TrecPointerKey::GetTrecObjectPointer(TrecPointerKey::GetNewSelfTrecPointer<TXMLHttpRequest>())
			);
	}

	void JsAbort(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		if (!IsValidObjectType(params, ret))
			return;

		dynamic_cast<TXMLHttpRequest*>(dynamic_cast<TObjectVariable*>(params[0].Get())->GetObjectW().Get())->Abort();
		ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
	}

	void JsGetAllResponseHeaders(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		if (!IsValidObjectType(params, ret))
			return;

		bool pres;
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(dynamic_cast<TXMLHttpRequest*>(dynamic_cast<TObjectVariable*>(params[0].Get())->GetObjectW().Get())->GetResponseHeaders(pres));
	}

	void JsGetResponseHeader(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		if (!IsValidObjectType(params, ret))
			return;

		if (params.Size() < 2)
		{
			ret.returnCode = ret.ERR_TOO_FEW_PARMS;
			ret.errorMessage.Set(L"TypeError: XMLHttpRequest.getResponseHeader: At least 1 argument required, but only 0 passed");
			return;
		}

		TString retStr((dynamic_cast<TXMLHttpRequest*>(
			dynamic_cast<TObjectVariable*>(params[0].Get())->GetObjectW().Get())->GetResponseHeader(params[1].Get() ? params[1]->GetString() : L"")));
		if(retStr.GetSize())
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(retStr);
	}

	void JsOpen(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		if (!IsValidObjectType(params, ret))
			return;

		dynamic_cast<TXMLHttpRequest*>(
			dynamic_cast<TObjectVariable*>(params[0].Get())->GetObjectW().Get())->Open(params, ret);
	}


	void JsSend(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		if (!IsValidObjectType(params, ret))
			return;

		try {
			dynamic_cast<TXMLHttpRequest*>(
				dynamic_cast<TObjectVariable*>(params[0].Get())->GetObjectW().Get())->Send(params.Size() > 1 ? params[1] : TrecPointer<TVariable>());
		}
		catch (UINT u)
		{
			if (u == 1)
			{
				ret.returnCode = ret.ERR_UNSUPPORTED_OP;
				ret.errorMessage.Set(L"Request Not properly configured!");
			}
		}
	}


	void JsSetRequestHeader(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
	{
		if (!IsValidObjectType(params, ret))
			return;

		if (params.Size() < 3 || !params[1].Get() || !params[2].Get())
		{
			ret.returnCode = ret.ERR_IMPROPER_TYPE;
			ret.errorMessage.Set(L"SetRequestHeader expected a header and value for parameters!");
			return;
		}

		dynamic_cast<TXMLHttpRequest*>(
			dynamic_cast<TObjectVariable*>(params[0].Get())->GetObjectW().Get())->SetRequestHeader(params[1]->GetString(), params[2]->GetString());
	}
};


DWORD __stdcall RunRequest(LPVOID param)
{
	TrecPointer<TXMLHttpRequest>* pParam = reinterpret_cast<TrecPointer<TXMLHttpRequest>*>(param);
	TrecPointer<TXMLHttpRequest> xmlReq = *pParam;
	delete pParam;
	pParam = nullptr;

	while (!xmlReq->IsAsyncComplete() && !xmlReq->IsAborted())
		Sleep(50);

	if (!xmlReq->IsAborted())
	{
		xmlReq->SetComplete(GetCurrentThreadId());
	}

}

DWORD __stdcall RunAsyncMethod(LPVOID param)
{
	TrecSubPointer<TVariable, TcInterpretor>* pParam = reinterpret_cast<TrecSubPointer<TVariable, TcInterpretor>*>(param);
	TrecSubPointer<TVariable, TcInterpretor> runner = *pParam;
	delete pParam;
	pParam = nullptr;
	if (runner.Get())
		runner->Run();

}







TXMLHttpRequest::TXMLHttpRequest(): request(THttpMethod::http_get), response("")
{
	state = 0; // UNSENT = 0
	threadId = 0;
	isAborted = requireEnc = false;
	useAsync = true;
}

void TXMLHttpRequest::Abort()
{
	TObjectLocker lock(&this->thread);
	if (asyncResponse.Get())
		asyncResponse->Abort();

	isAborted = true;
	DWORD word;
	CreateThread(nullptr, 0, RunAsyncMethod, new TrecSubPointer<TVariable, TcInterpretor>(this->abort), 0, &word);

	// To-Do: Check if Request is already complete
	if (asyncResponse->IsComplete())
	{

	}
	else
	{

	}
	this->UpdateState(0);
}

TString TXMLHttpRequest::GetResponseHeaders(bool& pres)
{
	TObjectLocker lock(&this->thread);
	TString ret;
	TString header, headValue;
	pres = false;
	for (UINT Rust = 0; response.GetHeader(Rust, header, headValue); Rust++)
	{
		if (!header.CompareNoCase(L"Set-Cookie") || !header.CompareNoCase(L"Set-Cookie2"))
			continue;

		// Note: According to Mozilla, header names are returned in lowercase
		ret.Append(header.GetLower() + L': ' + headValue);
		if (pres)
			ret.Append(L"\r\n");
		else
			pres = true;
	}

	return ret;
}

TString TXMLHttpRequest::GetResponseHeader(const TString& header)
{
	TObjectLocker lock(&this->thread);
	TString ret;
	response.GetHeader(header, ret);
	return ret;
}

void TXMLHttpRequest::Open(TDataArray<TrecPointer<TVariable>>& variables, ReturnObject& ret)
{
	TObjectLocker lock(&this->thread);
	TString method, url;
	useAsync = true;
	TString user, password;

	if (variables.Size() < 3)
	{
		ret.returnCode = ret.ERR_TOO_FEW_PARMS;
		ret.errorMessage.Set(L"Too few params, needed (request, url) at minimum!");
		return;
	}

	method.Set(variables[1].Get() ? variables[1]->GetString() : L"null");
	url.Set(variables[2].Get() ? variables[2]->GetString() : L"null");

	if (url.StartsWith(L"https:"))
		requireEnc = true;

	if (variables.Size() >= 4)
		VarFunction::IsTrue(variables[3], useAsync, 0xFF);
	THttpMethod meth = THttpMethod::http_get;
	if (!ConvertHttpMethodForms(method, meth))
	{
		ret.returnCode = ret.ERR_IMPROPER_NAME;
		ret.errorMessage.Set(L"Second parameter needed to be a string of [GET, POST, PUT, DELETE, HEAD, OPTIONS, CONNECT, PATCH]");
		return;
	}

	// To-Do: Handle Username/Password

	clientSocket = TrecPointerKey::GetNewSelfTrecPointer<THttpClientSocket>();
	if (clientSocket->InitializeSocket(url))
	{
		// Assume URL is invalid
		ret.errorMessage.Set(L"DOM Error! URL not valid!");
		ret.returnCode = ret.ERR_GENERIC_ERROR;
		return;
	}

	this->request.UpdateMethod(meth);

	int slash = url.Find(L'/', url.Find(L'.'));
	if (slash > 0)
	{
		request.SetEndpoint(url.SubString(slash));
	}
	this->url.Set(url);

	UpdateState(1); // 1 means Open has been called

	if (this->stateChange.Get() && stateChange->GetVarType() == var_type::interpretor)
	{
		// Now that state has changed, call method responsible for responding to change
		dynamic_cast<TcInterpretor*>(stateChange.Get())->Run();
	}
}

void TXMLHttpRequest::Send(TrecPointer<TVariable> pBody)
{
	TObjectLocker lock(&this->thread);
	if (!clientSocket.Get() || clientSocket->Connect(requireEnc))
	{
		throw (UINT)1;
	}
	if (pBody.Get())
	{
		switch (pBody->GetVarType())
		{
		case var_type::native_object:

			break;
		default:
			request.SetBody(pBody->GetString());
		}
	}

	if (useAsync)
	{
		this->asyncResponse = clientSocket->TransmitAsync(request);
		// To-Do: Set up thread that monitors response and runs handlers as a response
		// In this thread, state can be updated to 2, 3, or 4
		TrecPointer<TXMLHttpRequest>* pReq = new TrecPointer<TXMLHttpRequest>(TrecPointerKey::GetTrecPointerFromSoft<>(self));
		CreateThread(nullptr, 0, RunRequest, pReq, 0, &this->threadId);
	}
	else
	{
		TString err;
		this->response = clientSocket->Transmit(request, err);
		UpdateState(4); // 4 means Operation is complete
		DWORD word;
		if (err.GetSize())
			CreateThread(nullptr, 0, RunAsyncMethod, new TrecSubPointer<TVariable, TcInterpretor>(error), 0, &word);
	}
}

void TXMLHttpRequest::SetRequestHeader(const TString& header, const TString& value)
{
	TObjectLocker lock(&this->thread);
	request.AddHeader(header, value);
}

void TXMLHttpRequest::SetProperty(const TString& prop, TrecPointer<TVariable> value)
{

}

bool TXMLHttpRequest::SetVariable(const TString& prop, TrecPointer<TVariable> var)
{
	TObjectLocker lock(&this->thread);
	if (!prop.Compare(L"onreadystatechange"))
	{
		if (var.Get() && var->GetVarType() != var_type::interpretor)
			return false;
		PrepHandler(stateChange, TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(var));
		
		return true;
	}

	if (!prop.Compare(L"responseType"))
	{

	}
	if (!prop.Compare(L"timeout"))
	{

	}
	if (!prop.Compare(L"withCredentials"))
	{

	}
	return false;
}

bool TXMLHttpRequest::GetVariable(const TString& prop, TrecPointer<TVariable>& var)
{
	TObjectLocker lock(&this->thread);
	var.Nullify();
	if (!prop.Compare(L"onreadystatechange"))
	{
		var = TrecPointerKey::GetTrecPointerFromSub<>(this->stateChange);
		return true;
	}
	if (!prop.Compare(L"readyState")) // ro
	{
		var = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(this->state);
		return true;
	}
	if (!prop.Compare(L"response")) // ro
	{
		// To-Do: distinguish between response types
	}
	if (!prop.Compare(L"responseText")) // ro
	{
		
		short status = response.GetStatusCode();

		if(status >= 200 && status < 300)
			var = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(response.GetBody());
		return true;
	}
	if (!prop.Compare(L"responseType"))
	{

	}
	if (!prop.Compare(L"responseURL")) // ro
	{
		if(url.GetSize())
			var = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(url);
		return true;
	}
	if (!prop.Compare(L"responseXML")) // ro
	{

	}
	if (!prop.Compare(L"status")) // ro
	{
		var = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(response.GetStatusCode());
		return true;
	}
	if (!prop.Compare(L"statusText")) // ro
	{

		var = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(response.GetFullStatus());
		return true;
	}
	if (!prop.Compare(L"timeout"))
	{

	}
	if (!prop.Compare(L"withCredentials"))
	{

	}

	


	return false;

}

bool TXMLHttpRequest::IsAborted()
{
	TObjectLocker lock(&this->thread);
	return this->isAborted;
}

void TXMLHttpRequest::SetSelf(TrecPointer<TXMLHttpRequest> xmlReq)
{
	if (this != xmlReq.Get())
		throw L"Not this pointer";
	self = TrecPointerKey::GetSoftPointerFromTrec<>(xmlReq);
}

bool TXMLHttpRequest::IsAsyncComplete()
{
	TObjectLocker lock(&this->thread);
	return useAsync && asyncResponse.Get() && asyncResponse->IsComplete();;
}

TString TXMLHttpRequest::GetType()
{
	return TString(L"TXMLHttpRequest;") + TObject::GetType();
}

void TXMLHttpRequest::SetComplete(DWORD id)
{
	TObjectLocker lock(&this->thread);
	if (id == threadId)
	{
		UpdateState(4);
		DWORD word; 
		if (asyncResponse.Get() && asyncResponse->IsError())
			CreateThread(nullptr, 0, RunAsyncMethod, new TrecSubPointer<TVariable, TcInterpretor>(error), 0, &word);
	}
}

void TXMLHttpRequest::PrepHandler(TrecSubPointer<TVariable, TcInterpretor>& handler, TrecSubPointer<TVariable, TcInterpretor> code)
{
	handler = code;

	auto obj = TrecPointerKey::GetTrecObjectPointer(TrecPointerKey::GetTrecPointerFromSoft<>(self));

	handler->SetActiveObject(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(obj));
}

void TXMLHttpRequest::UpdateState(USHORT newState)
{
	TObjectLocker lock(&this->thread);
	if (newState > 4)
		return;
	UpdateState(newState);
	DWORD word;
	CreateThread(nullptr, 0, RunAsyncMethod, new TrecSubPointer<TVariable, TcInterpretor>(stateChange), 0, &word);
}

TrecPointer<TVariable> TXMLHttpRequest::GetProperty(const TString& prop)
{
	TObjectLocker lock(&this->thread);
	if (!prop.Compare(L"readyState"))
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(this->state);
	return TrecPointer<TVariable>();
}

void GetXmlHttpRequestMethods(TDataMap<TcVariableHolder>& variables, TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
{
	TcVariableHolder hold;
	if (variables.retrieveEntry(L"TXMLHttpRequest::send", hold))
		return;
	hold.mut = false;


	hold.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JSXmlHttpRequest::JsAbort, parent, env);
	variables.addEntry(L"TXMLHttpRequest::abort", hold);

	hold.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JSXmlHttpRequest::JsGetAllResponseHeaders, parent, env);
	variables.addEntry(L"TXMLHttpRequest::getAllResponseHeaders", hold);

	hold.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JSXmlHttpRequest::JsGetResponseHeader, parent, env);
	variables.addEntry(L"TXMLHttpRequest::getResponseHeader", hold);

	hold.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JSXmlHttpRequest::JsOpen, parent, env);
	variables.addEntry(L"TXMLHttpRequest::open", hold);

	hold.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JSXmlHttpRequest::JsSend, parent, env);
	variables.addEntry(L"TXMLHttpRequest::send", hold);

	hold.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JSXmlHttpRequest::JsSetRequestHeader, parent, env);
	variables.addEntry(L"TXMLHttpRequest::setRequestHeader", hold);

	hold.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JSXmlHttpRequest::JsConstructor, parent, env);
	variables.addEntry(L"XMLHttpRequest", hold);
}
