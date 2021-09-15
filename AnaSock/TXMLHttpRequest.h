#pragma once
#include "THttpClientSocket.h"
#include <TObject.h>
#include <TcInterpretor.h>


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

	void SetProperty(const TString& prop, TrecPointer<TVariable> value);

	TrecPointer<TVariable> GetProperty(const TString& prop);

	virtual bool SetVariable(const TString& prop, TrecPointer<TVariable> var) override;

	virtual bool GetVariable(const TString& prop, TrecPointer<TVariable>& var) override;

	/**
 * Method: TObject::GetType()
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 *
 * Note: This method is provided to allow interpretors to allow scripts to access methods of Anagame Objects.
 *   subclasses should report their type first, then the parent clss type and seperate it with a SemiColon
 *
 * Attributes: virtual
 */
	virtual TString GetType();
private:
	THttpRequest request;
	THttpResponse response;

	TrecPointer<THttpClientSocket> clientSocket;
	TrecPointer<THttpClientSocket::TAsyncHttpResponse> asyncResponse;
	bool useAsync;
	USHORT state;
	TString url;

	TrecSubPointer<TVariable, TcInterpretor> abort, error, 
		load, loadend, loadstart, progress, timeout, stateChange;
};

void _ANA_SOCK_DLL GetXmlHttpRequestMethods(TDataMap<TcVariableHolder>& variables, TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env);