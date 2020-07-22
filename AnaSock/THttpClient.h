#pragma once

#include <TClientSocket.h>
#include "SocketTypes.h"

class THttpClient :
	public TClientSocket
{
public:
	THttpClient();
	~THttpClient();


};

