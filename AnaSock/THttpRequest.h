#pragma once
#include <TObject.h>
#include <TDataArray.h>
#include <TMap.h>
#include "SocketTypes.h"
#include <TFile.h>

class THttpRequest :
	public TObject
{
public:
	THttpRequest();
	~THttpRequest();

	void AddHeader(TString& key, TString& value);
	TString RetrieveHeader(TString& key);
	TString RemoveHeader(TString& key);

	TString NextHeader();
	void ResetIterator();
	UINT CurrentIterator();

	TDataArray<UCHAR> CompileRequest();

	UINT SetBody(TMap<TString>& formData);
	UINT SetBody(TObject& data, SocketDataFormat sdf);
	UINT SetBody(TFile&, TString type);
protected:
};

