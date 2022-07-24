#pragma once

#define SECURITY_MAC

#include <winsock2.h>
#include <schannel.h>
#include <Security.h>
#include <ws2tcpip.h>
#include "TObject.h"
#include "TString.h"


class _TREC_LIB_DLL TSocket :
	public TObject
{
public:
	TSocket();
	virtual ~TSocket();

	void ConfigurePort_(USHORT p);
	virtual void Close() = 0;

private:
	static bool InitializeSockets();

protected:
	// virtual UINT InitializeSocket() = 0;
	UCHAR networkType;
	TString port;
};

