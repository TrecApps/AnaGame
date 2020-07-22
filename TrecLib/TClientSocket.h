#pragma once
#include "TSocket.h"
class TClientSocket :
	public TSocket
{
public:
	TClientSocket();
	~TClientSocket();

	virtual UINT InitializeSocket(TString& address);

protected:

};

