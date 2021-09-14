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






TXMLHttpRequest::TXMLHttpRequest(): request(THttpMethod::http_get), response("")
{
	state = 0; // UNSENT = 0
}

void TXMLHttpRequest::Abort()
{
	if (asyncResponse.Get())
		asyncResponse->Abort();

	// To-Do: Check if Request is already complete
}

TString TXMLHttpRequest::GetResponseHeaders(bool& pres)
{
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
	TString ret;
	response.GetHeader(header, ret);
	return ret;
}

void TXMLHttpRequest::Open(TDataArray<TrecPointer<TVariable>>& variables, ReturnObject& ret)
{
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
}

void TXMLHttpRequest::Send(TrecPointer<TVariable> pBody)
{
	if (!clientSocket.Get() || clientSocket->Connect())
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
	}
	else
	{
		TString err;
		this->response = clientSocket->Transmit(request, err);
	}
}

void TXMLHttpRequest::SetRequestHeader(const TString& header, const TString& value)
{
	request.AddHeader(header, value);
}

void TXMLHttpRequest::SetProperty(const TString& prop, TrecPointer<TVariable> value)
{

}

bool TXMLHttpRequest::SetVariable(const TString& prop, TrecPointer<TVariable> var)
{
	return false;
}

bool TXMLHttpRequest::GetVariable(const TString& prop, TrecPointer<TVariable>& var)
{
	return false;

}

TString TXMLHttpRequest::GetType()
{
	return TString(L"TXMLHttpRequest;") + TObject::GetType();
}

TrecPointer<TVariable> TXMLHttpRequest::GetProperty(const TString& prop)
{
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
