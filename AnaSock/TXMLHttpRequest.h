#pragma once
#include "THttpClientSocket.h"
#include <TObject.h>
#include <TcInterpretor.h>


class _ANA_SOCK_DLL TXMLHttpRequest : public TObject
{
public:
	TXMLHttpRequest();

	void Abort();
	TString GetResponseHeaders(bool& pres);
	TString GetResponseHeader(const TString& header);

	void Open(TDataArray<TrecPointer<TVariable>>& variables, ReturnObject& ret);

	void Send(TrecPointer<TVariable>);

	void SetRequestHeader(const TString& header, const TString& value);

	void SetProperty(const TString& prop, TrecPointer<TVariable> value);

	TrecPointer<TVariable> GetProperty(const TString& prop);

private:
	THttpRequest request;
	THttpResponse response;

	TrecPointer<THttpClientSocket> clientSocket;
	TrecPointer<THttpClientSocket::TAsyncHttpResponse> asyncResponse;

	USHORT state;

	TrecSubPointer<TVariable, TcInterpretor> abort, error, 
		load, loadend, loadstart, progress, timeout;
};

TrecPointer<TVariable> _ANA_SOCK_DLL GetXmlHttpRequestMethods();