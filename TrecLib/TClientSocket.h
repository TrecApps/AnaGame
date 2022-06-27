#pragma once
#include "TSocket.h"



class _TREC_LIB_DLL TClientSocket :
	public TSocket
{
public:
	TClientSocket(UCHAR type);
	~TClientSocket();

	virtual UINT InitializeSocket(TString& address);

	UINT Connect();

	virtual void Close() override;



	TString Send(TDataArray<char>& bytes);
	TString Recieve(TDataArray<char>& bytes);
protected:
	bool initSuccess;
	ADDRINFOW* results;
	SOCKET sock;
};

