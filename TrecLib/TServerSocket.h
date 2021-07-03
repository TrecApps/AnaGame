#pragma once
#include "TSocket.h"
class TServerSocket :
	public TSocket
{
public:
	TServerSocket(USHORT port);
	virtual UINT InitializeSocket();

	

	~TServerSocket();

protected:
	virtual void ProcessRequest(SOCKET clientSocket, std::string data)=0;

	UINT ProcessConnections();

	USHORT targetPort;
	ADDRINFOW* results;
	SOCKET sock;
};

