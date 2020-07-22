#pragma once
#include "TObject.h"
#include "TString.h"


class TSocket :
	public TObject
{
public:
	TSocket();
	~TSocket();

	void ConfigurePort_(USHORT p);
	

private:
	static bool InitializeSockets();

protected:
	virtual UINT InitializeSocket() = 0;
	UCHAR networkType;
	TString port;
};

