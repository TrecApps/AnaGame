#include "TClientSocket.h"
//#include <WinSock2.h>
//#include <ws2tcpip.h>

TClientSocket::TClientSocket()
{
}

TClientSocket::~TClientSocket()
{
}

UINT TClientSocket::InitializeSocket(TString& address)
{
	//ADDRINFOW hint4, hint6, * results;

	//hint4.ai_addr = hint6.ai_addr = nullptr;
	//hint4.ai_addrlen = hint6.ai_addrlen = 0;
	//hint4.ai_canonname = hint6.ai_canonname = nullptr;
	//hint4.ai_family = AF_INET;
	//hint6.ai_family = AF_INET6;
	//hint4.ai_flags = hint6.ai_flags = AI_PASSIVE;
	//hint4.ai_next = nullptr;
	//hint6.ai_next = &hint4;


	//if (networkType == 1)
	//{
	//	hint4.ai_protocol = hint6.ai_protocol = IPPROTO_TCP;
	//	hint4.ai_socktype = hint6.ai_socktype = SOCK_STREAM;
	//}
	//else if (networkType == 2)
	//{
	//	hint4.ai_protocol = hint6.ai_protocol = IPPROTO_UDP;
	//	hint4.ai_socktype = hint6.ai_socktype = SOCK_DGRAM;
	//}

	//int intResults = GetAddrInfoW(address.GetConstantBuffer(), port.GetConstantBuffer(), &hint6, &results);

	//if (intResults != 0)
	//	return intResults;



	return 1;
}
