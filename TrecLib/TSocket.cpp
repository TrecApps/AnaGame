#include "TSocket.h"
#include "TString.h"


TSocket::TSocket()
{
	if(!InitializeSockets())
		throw L"Error! Could not initialize sockets!";
}

TSocket::~TSocket()
{
}

void TSocket::ConfigurePort_(USHORT p)
{
	port.Empty();
	port.Format(L"%i", p);
}


static int socketResult = -1;

bool TSocket::InitializeSockets()
{
	if (socketResult == 0)
		return true;

	WSADATA winSock;
	winSock.wVersion = MAKEWORD(2, 1);
	winSock.wHighVersion = MAKEWORD(2, 2);
	socketResult = WSAStartup(MAKEWORD(2, 2), &winSock);

	return socketResult == 0;
}
