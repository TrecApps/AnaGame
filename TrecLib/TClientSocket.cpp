#include "TClientSocket.h"

#include <schannel.h>
//#include <WinSock2.h>
//#include <ws2tcpip.h>

#define CLIENT_BUFFER_SIZE 512;

TClientSocket::TClientSocket(UCHAR type)
{
	if (!type || (type > 2))
		throw L"Error, Expected 1 (TCP) or 2 (UDP) for network type";
	networkType = type;
	initSuccess = false;
	results = nullptr;
	sock = INVALID_SOCKET;
}

TClientSocket::~TClientSocket()
{
	if (results)
	{
		FreeAddrInfo(results);
		results = nullptr;
	}
	Close();
}

UINT TClientSocket::InitializeSocket(TString& address)
{
	ADDRINFOW hint4, hint6;

	ZeroMemory(&hint4, sizeof(hint4));
	ZeroMemory(&hint6, sizeof(hint6));

	hint4.ai_addr = hint6.ai_addr = nullptr;
	hint4.ai_addrlen = hint6.ai_addrlen = 0;
	hint4.ai_canonname = hint6.ai_canonname = nullptr;
	hint4.ai_family = AF_INET;
	hint6.ai_family = AF_INET6;
	hint4.ai_flags = hint6.ai_flags = AI_PASSIVE;
	hint4.ai_next = nullptr;
	hint6.ai_next = nullptr;

	results = nullptr;


	if (networkType == 1)
	{
		hint4.ai_protocol = hint6.ai_protocol = IPPROTO_TCP;
		hint4.ai_socktype = hint6.ai_socktype = SOCK_STREAM;
	}
	else if (networkType == 2)
	{
		hint4.ai_protocol = hint6.ai_protocol = IPPROTO_UDP;
		hint4.ai_socktype = hint6.ai_socktype = SOCK_DGRAM;
	}

	int intResults = GetAddrInfoW(address.GetConstantBuffer().getBuffer(), port.GetConstantBuffer().getBuffer(), &hint4, &results);

	if (intResults != 0)
	{
		//int e = WSAGetLastError();
		return intResults;
	}
	sock = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

	if (sock == INVALID_SOCKET)
	{
		FreeAddrInfo(results);
		results = nullptr;
		return 1;
	}


	initSuccess = true;
	return 0;
}

UINT TClientSocket::Connect()
{
	if (!initSuccess)
		return 1;

	int conRes = connect(sock, results->ai_addr, results->ai_addrlen);

	if (conRes == SOCKET_ERROR)
	{
		closesocket(sock);
		initSuccess = false;
		sock = INVALID_SOCKET;
		return 2;
	}
	return 0;
}

void TClientSocket::Close()
{
	if (sock != INVALID_SOCKET)
	{
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		sock = INVALID_SOCKET;
		initSuccess = false;
	}
}

TString TClientSocket::Send(TDataArray<char>& bytes)
{
	if(!initSuccess)
	return TString(L"Socket Not Properly Initialized!");

	if (!bytes.Size())
		return TString(L"No Data to Send!");
	TString ret;
	int sendRes = send(sock, &bytes[0], bytes.Size(), 0);
	if (sendRes == SOCKET_ERROR)
	{
		ret.Format(L"Send Failed: Error code is %d", WSAGetLastError());
	}
	return ret;
}

TString TClientSocket::Recieve(TDataArray<char>& bytes)
{
	char catchBytes[512];
	ZeroMemory(&catchBytes, sizeof(char)* 512);
	bytes.RemoveAll();
	int recRes;
	do
	{
		recRes = recv(sock, catchBytes, 512, 0);
		if (recRes > 0)
		{
			for (int Rust = 0; Rust < recRes; Rust++)
			{
				bytes.push_back(catchBytes[Rust]);
			}
		}
	} while (recRes > 0);
	TString ret;
	if (recRes < 0)
	{
		ret.Format(L"Recieve Failed with WSA Error: %d", WSAGetLastError());
	}

	return ret;
}
