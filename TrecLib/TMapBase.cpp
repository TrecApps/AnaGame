
#include "TMapBase.h"
UCHAR TMapType[] = { 1, 0b00010000 };

TMapBase::TMapBase()
{
}


TMapBase::~TMapBase()
{
}

UCHAR * TMapBase::GetAnaGameType()
{
	return TMapType;
}
