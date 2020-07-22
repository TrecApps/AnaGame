#include "pch.h"
#include "THttpRequest.h"

THttpRequest::THttpRequest()
{
}

THttpRequest::~THttpRequest()
{
}

void THttpRequest::AddHeader(TString& key, TString& value)
{
}

TString THttpRequest::RetrieveHeader(TString& key)
{
	return TString();
}

TString THttpRequest::RemoveHeader(TString& key)
{
	return TString();
}

TString THttpRequest::NextHeader()
{
	return TString();
}

void THttpRequest::ResetIterator()
{
}

UINT THttpRequest::CurrentIterator()
{
	return 0;
}

TDataArray<UCHAR> THttpRequest::CompileRequest()
{
	return TDataArray<UCHAR>();
}

UINT THttpRequest::SetBody(TMap<TString>& formData)
{
	return 0;
}

UINT THttpRequest::SetBody(TObject& data, SocketDataFormat sdf)
{
	return 0;
}

UINT THttpRequest::SetBody(TFile&, TString type)
{
	return 0;
}
