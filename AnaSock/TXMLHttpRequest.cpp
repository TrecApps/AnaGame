#include "TXMLHttpRequest.h"

TXMLHttpRequest::TXMLHttpRequest(): request(THttpMethod::http_get), response("")
{
}

void TXMLHttpRequest::Abort()
{
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

		ret.Append(header + L': ' + headValue);
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

void TXMLHttpRequest::SetProperty(TString& prop, TrecPointer<TVariable> value)
{

}

TrecPointer<TVariable> GetXmlHttpRequestMethods()
{
	return TrecPointer<TVariable>();
}
