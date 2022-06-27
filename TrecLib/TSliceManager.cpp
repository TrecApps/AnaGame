#include "TSliceManager.h"

TSliceManager::~TSliceManager()
{
}

void TSliceManager::SetSelf(TrecPointer<TSliceManager> s)
{
	if (s.Get() != this)
		throw L"Expected self to refer to 'this'";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TSliceManager>(s);
}
