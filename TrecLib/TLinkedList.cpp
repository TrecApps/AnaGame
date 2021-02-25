#include "TLinkedList.h"

TLinkedListBase::TLinkedListBase()
{
	size = currentNode = 0;
}

UINT TLinkedListBase::GetSize()
{
	return size;
}

UINT TLinkedListBase::GetCurrentIndex()
{
	return currentNode;
}
