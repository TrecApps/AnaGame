#include "TClientSocket.h"


//#include <WinSock2.h>
//#include <ws2tcpip.h>

#define CLIENT_BUFFER_SIZE 512;

#define AUTH_CLIENT_BUFFER_SIZE 2048

BOOL SendBytes(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf)
{
	PBYTE pTemp = pBuf;
	int   cbSent;
	int   cbRemaining = cbBuf;

	if (0 == cbBuf)
		return(TRUE);

	while (cbRemaining)
	{
		cbSent = send(
			s,
			(const char*)pTemp,
			cbRemaining,
			0);
		if (SOCKET_ERROR == cbSent)
		{
			fprintf(stderr, "send failed: %u\n", GetLastError());
			return FALSE;
		}

		pTemp += cbSent;
		cbRemaining -= cbSent;
	}

	return TRUE;
}

BOOL ReceiveBytes(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf,
	DWORD* pcbRead)
{
	PBYTE pTemp = pBuf;
	int cbRead, cbRemaining = cbBuf;

	while (cbRemaining)
	{
		cbRead = recv(
			s,
			(char*)pTemp,
			cbRemaining,
			0);
		if (0 == cbRead)
			break;
		if (SOCKET_ERROR == cbRead)
		{
			fprintf(stderr, "recv failed: %u\n", GetLastError());
			return FALSE;
		}

		cbRemaining -= cbRead;
		pTemp += cbRead;
	}

	*pcbRead = cbBuf - cbRemaining;

	return TRUE;
}  // end ReceiveBytes

BOOL SendMsg(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf)
{
	if (0 == cbBuf)
		return(TRUE);

	//----------------------------------------------------------
	//  Send the size of the message.

	if (!SendBytes(s, (PBYTE)&cbBuf, sizeof(cbBuf)))
		return(FALSE);

	//----------------------------------------------------------
	//  Send the body of the message.

	if (!SendBytes(
		s,
		pBuf,
		cbBuf))
	{
		return(FALSE);
	}

	return(TRUE);
}

BOOL ReceiveMsg(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf,
	DWORD* pcbRead)

{
	DWORD cbRead;
	DWORD cbData;

	//----------------------------------------------------------
	//  Receive the number of bytes in the message.

	if (!ReceiveBytes(
		s,
		(PBYTE)&cbData,
		sizeof(cbData),
		&cbRead))
	{
		return(FALSE);
	}

	if (sizeof(cbData) != cbRead)
		return(FALSE);
	//----------------------------------------------------------
	//  Read the full message.

	if (!ReceiveBytes(
		s,
		pBuf,
		cbData,
		&cbRead))
	{
		return(FALSE);
	}

	if (cbRead != cbData)
		return(FALSE);

	*pcbRead = cbRead;
	return(TRUE);
}  // end ReceiveMessage    


TClientSocket::TClientSocket(UCHAR type)
{
	if (!type || (type > 2))
		throw L"Error, Expected 1 (TCP) or 2 (UDP) for network type";
	networkType = type;
	initSuccess = isEncrypted = false;
	results = nullptr;
	sock = INVALID_SOCKET;
	hCred = nullptr;
	hcText = nullptr;
}

TClientSocket::~TClientSocket()
{
	if (results)
	{
		FreeAddrInfo(results);
		results = nullptr;
	}

	if (hcText)
		DeleteSecurityContext(hcText);
	hcText = nullptr;
	if (hCred)
		FreeCredentialHandle(hCred);
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

UINT TClientSocket::Connect(bool requireAuth)
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

bool TClientSocket::AttemptAuth()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	FILETIME fileTime;
	assert(SystemTimeToFileTime(&sysTime, &fileTime));

	TimeStamp cur;
	memcpy_s(&cur, sizeof(cur), &fileTime, sizeof(fileTime));
	if (hcText && Lifetime > cur)
		return true;

	UCHAR* inBuffer = new UCHAR[AUTH_CLIENT_BUFFER_SIZE];
	UCHAR* outBuffer = new UCHAR[AUTH_CLIENT_BUFFER_SIZE];

	static PTCHAR     lpPackageName = (PTCHAR)UNISP_NAME;

	ss = AcquireCredentialsHandle(
		nullptr,
		lpPackageName,
		SECPKG_CRED_OUTBOUND,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		hCred,
		&Lifetime
	);

	if (ss < 0 || !PrepSecurityContext(inBuffer, outBuffer))
	{
		delete[] inBuffer;
		delete[] outBuffer;
		inBuffer = outBuffer = nullptr;
		return false;
	}
	
	delete[] inBuffer;
	delete[] outBuffer;
	inBuffer = outBuffer = nullptr;
	if (!hcText)
		hcText = new SecHandle;

	ss = QueryContextAttributes(hcText, SECPKG_ATTR_NEGOTIATION_INFO, &SecPkgNegInfo);
	if (ss < 1)
		return false;

	ss = QueryContextAttributes(
		hcText,
		SECPKG_ATTR_SIZES,
		&SecPkgContextSizes);
}

bool TClientSocket::PrepSecurityContext(UCHAR* inBuffer, UCHAR* outBuffer)
{
	bool done = false;

	TDataArray<UCHAR> inBuffer(AUTH_CLIENT_BUFFER_SIZE);
	TDataArray<UCHAR> outBuffer(AUTH_CLIENT_BUFFER_SIZE);

	SecBufferDesc     OutBuffDesc;
	SecBuffer         OutSecBuff;
	SecBufferDesc     InBuffDesc;
	SecBuffer         InSecBuff;

	UINT count = 0;
	ULONG contextAtts = 0;

	DWORD inSize = 0;

	while (!done)
	{
		OutBuffDesc.ulVersion = 0;
		OutBuffDesc.cBuffers = 1;
		OutBuffDesc.pBuffers = &OutSecBuff;

		OutSecBuff.cbBuffer = inSize;
		OutSecBuff.BufferType = SECBUFFER_TOKEN;
		OutSecBuff.pvBuffer = outBuffer;

		InBuffDesc.ulVersion = 0;
		InBuffDesc.cBuffers = 1;
		InBuffDesc.pBuffers = &InSecBuff;

		InSecBuff.cbBuffer = inSize;
		InSecBuff.BufferType = SECBUFFER_TOKEN;
		InSecBuff.pvBuffer = inBuffer;

		ss = InitializeSecurityContext(
			hCred,								// PCredHandle    phCredential
			count ? hcText : nullptr,			// PCtxtHandle    phContext
			nullptr,							// SEC_CHAR       *pszTargetName
			ISC_REQ_CONFIDENTIALITY,			// ULONG          fContextReq
			0,									// ULONG          Reserved1
			0,									// ULONG          TargetDataRep
			count ? (&InBuffDesc) : nullptr,	// PSecBufferDesc pInput
			0,									// ULONG          Reserved2
			hcText,
			&OutBuffDesc,
			&contextAtts,
			&Lifetime
		);

		if (ss < -1)
			return false;

		if ((SEC_I_COMPLETE_NEEDED == ss)
			|| (SEC_I_COMPLETE_AND_CONTINUE == ss))
		{
			ss = CompleteAuthToken(hcText, &OutBuffDesc);
			if (ss < -1)
				return false;
			done = true;
		}
		
		if (!SendMsg(sock, outBuffer, AUTH_CLIENT_BUFFER_SIZE) || !ReceiveMsg(sock, inBuffer, AUTH_CLIENT_BUFFER_SIZE, &inSize))
			return false;


	}
	return true;
}
