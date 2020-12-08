#pragma once
#include "TSocket.h"
class TClientSocket :
	public TSocket
{
public:
	TClientSocket(UCHAR type);
	~TClientSocket();

	virtual UINT InitializeSocket(TString& address);

	UINT Connect();

	virtual void Close() override;

protected:

	TString Send(TDataArray<char>& bytes);
	TString Recieve(TDataArray<char>& bytes);

	bool initSuccess;
	ADDRINFOW* results;
	SOCKET sock;
};

