#pragma once
#include "TObject.h"

/****/
class _TREC_LIB_DLL TMapBase :
	public TObject
{
public:
	TMapBase();
	~TMapBase();

	virtual UCHAR* GetAnaGameType()override;
};

