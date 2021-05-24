#pragma once
#include "TrecLib.h"
#include "TObject.h"
#include "TrecReference.h"
/**
 * Class: TLinkedListBase
 * Purpose: Serves as the base class for Linked Lists
 * 
 * SuperClass: TObject
 */
class _TREC_LIB_DLL TLinkedListBase : public TObject
{
protected:
	UINT size, currentNode;

public:
	TLinkedListBase();
	UINT GetSize();
	UINT GetCurrentIndex();
};

template<typename T> class _TREC_LIB_DLL ListNode 
{
public:
	TrecPointer<ListNode<T>> next;
	TrecPointerSoft<ListNode<T>> prev;
	T data;
};

/**
 * Class: TLinkedList
 * Purpose: Template for actual linked lists in Anagame
 * 
 * SuperClass: TLinkedListBase
 */
template<typename T> class _TREC_LIB_DLL TLinkedList : public TLinkedListBase
{
protected:


	TrecPointer<ListNode<T>> head, current, tail;

public:

	UINT Push(T& newData)
	{
		TrecPointer<ListNode<T>> node = TrecPointerKey::GetNewTrecPointer<ListNode<T>>();
		node->data = newData;

		if (!size)
		{
			head = tail = current = node;
		}
		else
		{
			tail->next = node;
			node->prev = TrecPointerKey::GetSoftPointerFromTrec<ListNode<T>>(tail);
			tail = node;
		}
		size++;
		return size;
	}

	UINT PushHead(T& newData)
	{
		TrecPointer<ListNode<T>> node = TrecPointerKey::GetNewTrecPointer<ListNode<T>>();
		node->data = newData;

		if (!size)
		{
			head = tail = current = node;
		}
		else
		{
			head->prev = node;
			node->next = head;
			head = node;
		}
	}

	TrecPointer<ListNode<T>> GetCurrentNode()
	{
		return current;
	}

	bool Dequeue(T& data)
	{
		if (head.Get())
		{
			data = head->data;
			DropHead();
			return true;
		}
		else
			return false;
	}

	void Flush() {
		head = current;
		size -= currentNode;
		currentNode = 0;
	}

	void DropHead()
	{
		if (!size)
			return;
		if (head.Get() == tail.Get())
		{
			// Size is one, nullify all pointers and set size to 0
			head.Nullify();
			tail.Nullify();
			current.Nullify();
			size = currentNode = 0;
			return;
		}

		if (head.Get() == current.Get())
			current = current->next;
		else
			currentNode--;
		head = head->next;
		size--;
	}

	bool Next()
	{
		if (!size)
			return false;
		if (current.Get() == tail.Get())
			return false;
		current = current->next;
		currentNode++;
		return true;
	}

	bool Prev()
	{
		if (!size || !currentNode)
			return false;
		current = TrecPointerKey::GetTrecPointerFromSoft<ListNode<T>>(current->prev);
	}
	
	

};
