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


}

void TXMLHttpRequest::Send(TrecPointer<TVariable>)
{
}

void TXMLHttpRequest::SetRequestHeader(const TString& header, const TString& value)
{
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

TrecPointer<TVariable> TXMLHttpRequest::GetProperty(const TString& prop)
{
	if (!prop.Compare(L"readyState"))
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(this->state);
	return TrecPointer<TVariable>();
}

TrecPointer<TVariable> GetXmlHttpRequestMethods()
{
	return TrecPointer<TVariable>();
}
