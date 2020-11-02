#include "TSliceManager.h"

void TSliceManager::SetSelf(TrecPointer<TSliceManager> self)
{
	if (self.Get() != this)
		throw L"Expected self to refer to 'this'";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TSliceManager>(self);
}
