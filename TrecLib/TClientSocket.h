#pragma once
#include "TSocket.h"



class _TREC_LIB_DLL TClientSocket :
	public TSocket
{
public:
	TClientSocket(UCHAR type);
	~TClientSocket();

	virtual UINT InitializeSocket(TString& address);

	UINT Connect(bool requireAuth = false);

	virtual void Close() override;



	TString Send(TDataArray<char>& bytes);
	TString Recieve(TDataArray<char>& bytes);
protected:

	bool AttemptAuth();
	bool PrepSecurityContext(UCHAR* inBuffer, UCHAR* outBuffer);
	// Security Attributes
	bool isEncrypted;
	SECURITY_STATUS   ss;
	TimeStamp         Lifetime;
	CredHandle* hCred;
	struct _SecHandle* hcText;
	SecPkgContext_NegotiationInfo  SecPkgNegInfo;
	SecPkgContext_Sizes            SecPkgContextSizes;

	bool initSuccess;
	ADDRINFOW* results;
	SOCKET sock;
};

