#pragma once
#include <TObject.h>
#include <TcInterpretor.h>
#include "THttpClientSocket.h"

class _ANA_SOCK_DLL TXMLHttpRequest : public TVObject
{
public:
	TXMLHttpRequest();

	void Abort();
	TString GetResponseHeaders(bool& pres);
	TString GetResponseHeader(const TString& header);

	void Open(TDataArray<TrecPointer<TVariable>>& variables, ReturnObject& ret);

	void Send(TrecPointer<TVariable>);

	void SetRequestHeader(const TString& header, const TString& value);

	void SetProperty(TString& prop, TrecPointer<TVariable> value);

	virtual bool SetVariable(const TString& prop, TrecPointer<TVariable> var) override;

	virtual bool GetVariable(const TString& prop, TrecPointer<TVariable>& var) override;

private:
	THttpRequest request;
	THttpResponse response;

	TrecSubPointer<TVariable, TcInterpretor> abort, error, 
		load, loadend, loadstart, progress, timeout;
};

TrecPointer<TVariable> _ANA_SOCK_DLL GetXmlHttpRequestMethods();