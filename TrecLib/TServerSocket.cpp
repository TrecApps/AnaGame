#include "TServerSocket.h"


TServerSocket::TServerSocket(USHORT port)
{
	targetPort = port;
	results = nullptr;
	sock = INVALID_SOCKET;
}

UINT TServerSocket::InitializeSocket()
{
	ADDRINFOW hint4, hint6;

	hint4.ai_addr = hint6.ai_addr = nullptr;
	hint4.ai_addrlen = hint6.ai_addrlen = 0;
	hint4.ai_canonname = hint6.ai_canonname = nullptr;
	hint4.ai_family = AF_INET;
	hint6.ai_family = AF_INET6;
	hint4.ai_flags = hint6.ai_flags = AI_PASSIVE;
	hint4.ai_next = nullptr;
	hint6.ai_next = &hint4;

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

	int intResults = GetAddrInfoW(nullptr, port.GetConstantBuffer().getBuffer(), &hint6, &results);

	if (intResults != 0)
		return intResults;

	sock = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

	if (sock == INVALID_SOCKET)
	{
		FreeAddrInfo(results);
		results = nullptr;
		return 1;
	}

	intResults = bind(sock, results->ai_addr, (int)results->ai_addrlen);
	if (intResults == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		FreeAddrInfo(results);
		closesocket(sock);
		sock = INVALID_SOCKET;
		results = nullptr;
		return 2;
	}
	FreeAddrInfo(results);
	results = nullptr;
	return 0;
}

TServerSocket::~TServerSocket()
{
}

UINT TServerSocket::ProcessConnections()
{
	if (sock == INVALID_SOCKET)
		return 1;

	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(sock);
		sock = INVALID_SOCKET;
		return 2;
	}

	while (sock != INVALID_SOCKET) {
		SOCKET clientSock = accept(sock, nullptr, nullptr);
		if (clientSock == INVALID_SOCKET)
		{
			closesocket(sock);
			sock = INVALID_SOCKET;
			return 3;
		}

		char buffer[513];
		std::string data;
		int res;
		do {
			ZeroMemory(buffer, sizeof(char) * 513);
			res = recv(clientSock, buffer, 512, 0);

			if (res > 0)
				data += buffer;

		} while (res > 0);

		if (!res)
		{
			ProcessRequest(clientSock, data);
		}
		else
		{
			closesocket(clientSock);
			closesocket(sock);
			sock = clientSock = INVALID_SOCKET;
			return 4;
		}
	}


	return 0;
}
