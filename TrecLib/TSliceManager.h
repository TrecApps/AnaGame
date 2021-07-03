#pragma once
#include <wtypes.h>
#include "TrecReference.h"

class _TREC_LIB_DLL TSliceManager
{
public:
	virtual void Insert(UINT start, UINT count) = 0;
	virtual void Delete(UINT start, UINT count) = 0;

	void SetSelf(TrecPointer<TSliceManager> self);

protected:
	TrecPointerSoft<TSliceManager> self;
};