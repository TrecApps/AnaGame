#include "TrecReference.h"

/**
 * Method: TrecObjectPointer::TrecObjectPointer
 * Purpose: Constructor
 * Parameters: TrecBoxPointerBase* - the pointer to the object
 * Returns: New TObject pointer
 */
TrecObjectPointer::TrecObjectPointer(TrecBoxPointerBase* base)
{
	basePointer = base;
	if (basePointer)
		basePointer->counter++;
}

/**
 * Method: TrecObjectPointer::Copy
 * Purpose: Allows refactoring of the copy logic
 * Parameters: TrecObjectPointer& copy - the Pointer to copy
 * Returns: void
 */
void TrecObjectPointer::Copy(TrecObjectPointer& copy)
{
	if (basePointer == copy.basePointer)
		return;

	DoDecrement();

	basePointer = copy.basePointer;

	if (basePointer)
		basePointer->counter++;
}

/**
 * Method: TrecObjectPointer::DoDecrement
 * Purpose: Allows refactoring of the Decrement counter logic
 * Parameters: void
 * Returns: void
 */
void TrecObjectPointer::DoDecrement()
{
	if (basePointer)
	{
		if (basePointer->counter)
		{
			basePointer->counter--;
			if (!basePointer->counter)
			{
				delete reinterpret_cast<TObject*>(basePointer->rawPointer);
				basePointer->rawPointer = nullptr;
			}
		}

		if (!basePointer->counter && !basePointer->softCounter)
		{
			// If we reach this point, then this is the last reference to the Counter Object
			delete basePointer;
		}
		basePointer = nullptr;
	}
}

/**
 * Method: TrecObjectPointer::~TrecObjectPointer
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TrecObjectPointer::~TrecObjectPointer()
{
	DoDecrement();
}

/**
 * Method: TrecObjectPointer::Get
 * Purpose: Useful for a NULL check
 * Parameters: void
 * Returns: TObject* - the Object being held
 */
TObject* TrecObjectPointer::Get()
{
	if(!basePointer)
		return nullptr;
	return reinterpret_cast<TObject*>(basePointer->rawPointer);
}

/**
 * Method: TrecObjectPointer::operator->
 * Purpose: Used for directly calling methods on a TObject
 * Parameters: void
 * Returns: TObject* - the Object being held
 */
TObject* TrecObjectPointer::operator->()
{
	return Get();
}

/**
 * Method: TrecObjectPointer::operator=
 * Purpose: Allows Equality operators to be assigned
 * Parameters: TrecObjectPointer& obj -  the Pointer to copy
 * Returns: void
 */
void TrecObjectPointer::operator=(TrecObjectPointer& obj)
{
	Copy(obj);
}

/**
 * Method: TrecObjectPointer::TrecObjectPointer
 * Purpose: Copy Constructor
 * Parameters: TrecObjectPointer& obj - the Pointer to copy
 * Returns: New TObject Pointer
 */
TrecObjectPointer::TrecObjectPointer(TrecObjectPointer& copy)
{
	// Since this is a constructor, the base Pointer has not been initialized, even if the memory is not null.
	// Set it to Null before performing Copy
	basePointer == nullptr;

	Copy(copy);
}

/**
 * Method: TrecObjectPointer::TrecObjectPointer
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New (null) TObject Pointer
 */
TrecObjectPointer::TrecObjectPointer()
{
	basePointer = nullptr;
}

/**
 * Method: TrecObjectPointer::Nullify
 * Purpose: Sets the Pointer to Null
 * Parameters: void
 * Returns: void
 */
void TrecObjectPointer::Nullify()
{
	DoDecrement();
}
