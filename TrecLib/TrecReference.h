#pragma once
//#include <WinBase.h>
//#include <Windows.h>
#include "TObject.h"

/**
 * File: TrecReference.h
 * This file provides the tools for managing memory in AnaGame and could theoretically be used in any Windows C++ project.
 *	The classes are either Anagame Smart Pointers or helper classes for the smart pointers
 *
 *		TrecPointer<T> - the standard smart pointer offered By Anagame, where T can be any type.
 *			Each TrecPointer points to a counter which keeps track of the number of TrecPointers referencing it
 *
 *		TrecPointerSoft<T> - a weak pointer that references counters used byt the standard TrecPointer, but does not
 *			increment the counter. Use for Object to have a TrecReference to itself so that they can produce a TrecPointer to itself
 *			This Pointer can be compared to the weak_ptr in the Standard C++ library
 *
 *		TrecSubPointer<T,U> - A smart pointer that casts the reference type T to it's presumed type U. The raw pointer is of type T,
 *			but the TrecSubPointer attempts to cast it to type U. Ideally, U should be a subclass of T.
 *		
 *		TrecSubPointerSoft<T,U> - a weak pointer that operates with the TrecSubPointer<T,U>. It has access to the counter used by
 *			the other TrecPointers but does not increment or decriment the counter
 *
 *		TrecComPointer<T> - A specialized pointer to be used when the type T inherits from IUnknown in a Mocrosoft Environment. This is
 *			because these types are created by a Factory method/function and deleting them requires calling the Release() method rather than
 *			manually deleting them.
 *
 *		TrecObjectPointer - A new Pointer type that holds a counter to any type that can call itself a TObject. Written with Anagame 
 *			Interpretors in mind
 *
 *		The Other class you will use extensively is the TrecPointerKey class. This is a utility class necessary to operate on the TrecPointers
 *			This is because the TrecPointers restrict what you can do with them. 
 */

/**
 * class TrecBoxPointerBase
 * Purpose: Holds the counter and base for the 
 */
class TrecBoxPointerBase
{
	friend class TrecPointerKey;
	friend class TrecObjectPointer;
protected:
	/**
	 * The raw pointer to the actual object being used
	 */
	void* rawPointer;

	/**
	 * tracks the number of TrecPointers referencing the object
	 */
	UINT counter;
	/**
	 * Keep Track of Soft pointers so they don't become dangling pointers
	 */
	USHORT softCounter;
};


/**
 * Class: TrecObjectPointer
 * Purpose: Holds a reference to an Object as if it was just a TObject
 */
class TrecObjectPointer
{
	friend class TrecPointerKey;
protected:
	/**
	 * TrecBoxPointerBase that holds the raw reference
	 */
	TrecBoxPointerBase* basePointer;

	/**
	 * Method: TrecObjectPointer::TrecObjectPointer
	 * Purpose: Constructor
	 * Parameters: TrecBoxPointerBase* - the pointer to the object
	 * Returns: New TObject pointer
	 */
	TrecObjectPointer(TrecBoxPointerBase* base);

	/**
	 * Method: TrecObjectPointer::Copy
	 * Purpose: Allows refactoring of the copy logic
	 * Parameters: TrecObjectPointer& copy - the Pointer to copy
	 * Returns: void
	 */
	void Copy(TrecObjectPointer& copy);

	/**
	 * Method: TrecObjectPointer::DoDecrement
	 * Purpose: Allows refactoring of the Decrement counter logic
	 * Parameters: void
	 * Returns: void
	 */
	void DoDecrement();

public:

	/**
	 * Method: TrecObjectPointer::~TrecObjectPointer
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TrecObjectPointer();

	/**
	 * Method: TrecObjectPointer::Get
	 * Purpose: Useful for a NULL check
	 * Parameters: void
	 * Returns: TObject* - the Object being held
	 */
	TObject* Get();

	/**
	 * Method: TrecObjectPointer::operator->
	 * Purpose: Used for directly calling methods on a TObject
	 * Parameters: void
	 * Returns: TObject* - the Object being held
	 */
	TObject* operator->();

	/**
	 * Method: TrecObjectPointer::operator=
	 * Purpose: Allows Equality operators to be assigned
	 * Parameters: TrecObjectPointer& obj -  the Pointer to copy
	 * Returns: void
	 */
	void operator=(TrecObjectPointer& obj);

	/**
	 * Method: TrecObjectPointer::TrecObjectPointer
	 * Purpose: Copy Constructor
	 * Parameters: TrecObjectPointer& obj - the Pointer to copy
	 * Returns: New TObject Pointer
	 */
	TrecObjectPointer(TrecObjectPointer& copy);

	/**
	 * Method: TrecObjectPointer::TrecObjectPointer
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New (null) TObject Pointer
	 */
	TrecObjectPointer();

	/**
	 * Method: TrecObjectPointer::Nullify
	 * Purpose: Sets the Pointer to Null
	 * Parameters: void
	 * Returns: void
	 */
	void Nullify();
};


/**
 * class TrecBoxPointer
 * Purpose: holds the raw pointer and a counter to the object referenced by the various TrecPointers
 */
template<class T> class TrecBoxPointer: public TrecBoxPointerBase
{
	/**
	 * Need to allow the utility class TrecPointerKey access for it to perform it's job properly
	 */
	friend class TrecPointerKey;
private:

public:

	/**
	 * Method: TrecBoxPointer::TrecBoxPointer
	 * Purpose: Constructor
	 * Parameters: T* t - raw pointer to the T type that this counter will be holding.
	 * Returns: New TrecBoxPointer Object
	 */
	TrecBoxPointer(T* t)
	{
		counter = (t)? 1 : 0;
		rawPointer = (void*)t;
		softCounter = 0;
	}

	/**
	 * Method: TrecBoxPointer::~TrecBoxPointer
	 * Purpose: Removes the rar pointer if still initialized
	 * Parameters: void
	 * Returns: void
	 */
	~TrecBoxPointer()
	{
		if (rawPointer)
			delete reinterpret_cast<T*>(rawPointer);
	}

	/**
	 * Method: TrecBoxPointer::Increment
	 * Purpose: Increases the counter by one, intended to be called by all regular ans Sub TrecPointers when they first reference this object
	 * Parameters: void
	 * Returns: void
	 */
	void Increment()
	{
		counter++;
	}

	/**
	 * Method: TrecBoxPointer::IncrementSoft
	 * Purpose: Increases the counter by one, intended to be called by all Soft TrecPointers so they don't become dangling pointers
	 * Parameters: void
	 * Returns: void
	 */
	void IncrementSoft()
	{
		softCounter++;
	}

	/**
	 * Method: TrecBoxPointer::Decrement
	 * Purpose: Reduces the counter. If the counter reaches zero, go ahead an commence clean up
	 * Parameters: void
	 * Returns: void
	 */
	void Decrement()
	{
		counter--;
		if (!counter && !softCounter) // if counter is zero, then there is no reference to the object and it is time to delete it.
			delete this;
		else if (!counter)
		{							// Main pointers are gone but soft pointers remain. Delete the object but don't yet delete the counter
			delete rawPointer;
			rawPointer = nullptr;
		}
	}

	/**
	 * Method: TrecBoxPointer::DecrementSoft
	 * Purpose: Reduces the counter. If the counter reaches zero, go ahead an commence clean up
	 * Parameters: void
	 * Returns: void
	 */
	void DecrementSoft()
	{
		softCounter--;
		if (!counter && !softCounter) // if counter is zero, then there is no reference to the object and it is time to delete it.
			delete this;
		else if (!counter)
		{							// Main pointers are gone but soft pointers remain. Delete the object but don't yet delete the counter
			delete rawPointer;
			rawPointer = nullptr;
		}
	}

	/**
	 * Method: TrecBoxPointer::Get
	 * Purpose: Returns the raw object held by this object
	 * Parameters: void
	 * Returns: T* - the object held
	 */
	T* Get()
	{
		return reinterpret_cast<T*>(rawPointer);
	}

	/**
	 * Method: TrecBoxPointer::Delete
	 * Purpose: Performs manual deletion of the object, in case the code deems it necessary to Delete the object early
	 * Parameters: void
	 * Returns: void
	 */
	void Delete()
	{
		if(rawPointer)
			delete rawPointer;
		// now that the object has been deleted, set it to null to avoid a dangling pointer
		rawPointer = nullptr;
	}
};

/**
 * class TrecComBoxPointer
 * Purpose: Holds the actual raw pointer to COM types used
 */
template<class T> class TrecComBoxPointer
{
private:
	/**
	 * the Raw pointer to the type
	 */
	T* rawPointer;
	/**
	 * keeps track of how many TrecComPointers are referencing this object
	 */
	UINT counter;
public:

	/**
	 * Method: TrecComBoxPointer::TrecComBoxPointer
	 * Purpose: Constructor
	 * Parameters: T* t - the raw pointer to the COM object
	 * Returns: New TrecComBoxPointer Object
	 */
	TrecComBoxPointer(T* t)
	{
		counter = (t) ? 1 : 0;
		rawPointer = t;
	}

	/**
	 * Method: TrecComBoxPointer::TrecComBoxPointer
	 * Purpose: Cleans up the Object
	 * Parameters: void
	 * Returns: void
	 */
	~TrecComBoxPointer()
	{
		if (rawPointer)
			// Because this is a COM object, we call Release() on it
			rawPointer->Release();
	}

	/**
	 * Method: TrecComBoxPointer::Increment
	 * Purpose: Increments the counter by 1
	 * Parameters: void
	 * Returns: void
	 */
	void Increment()
	{
		counter++;
	}

	/**
	 * Method: TrecComBoxPointer::Decrement
	 * Purpose: Reduces the country by 1, commencing deletion if the counter reaches 0
	 * Parameters: void
	 * Returns: void
	 */
	void Decrement()
	{
		counter--;
		if (!counter)
			delete this;
	}

	/**
	 * Method: TrecComBoxPointer::Get
	 * Purpose: Returns the raw pointer to the higher TrecComPoint
	 * Parameters: void
	 * Returns: T* - the raw object pointer
	 */
	T* Get()
	{
		return rawPointer;
	}

	/**
	 * Method: TrecComBoxPointer::Delete
	 * Purpose: Performs manual deletion of the object
	 * Parameters: void
	 * Returns: void
	 */
	void Delete()
	{
		if(rawPointer)
			rawPointer->Release();
		rawPointer = nullptr;
	}
};


/**
 * class TrecPointerSoft
 * Purpose: Provides a weak pointer type to help prevent circular references that prevent counters from reaching 0
 * 
 * Note: Written to allow certain objects to hold references to themselves and return regular TrecPointer's to themselves
 */
template<class T> class TrecPointerSoft
{
	friend class TrecPointerKey;
private:
	/**
	 * counter that holds the reference and reference count
	 */
	TrecBoxPointer<T>* pointer;


	/**
	 * Method: TrecPointerSoft::TrecPointerSoft
	 * Purpose: Constructor
	 * Parameters:TrecBoxPointer<T>* pointer - pointer to the Object holding the raw pointer
	 * Returns: New TrecPointerSoft Object
	 */
	TrecPointerSoft(TrecBoxPointer<T>* pointer)
	{
		if(!pointer)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		this->pointer = pointer;
		if (this->pointer)
			this->pointer->IncrementSoft();
	}

public:
	/**
	 * Method: TrecPointerSoft::TrecPointerSoft
	 * Purpose: Default constructor
	 * Parameters: void
	 * Returns: New TrecPointerSoft Object
	 */
	TrecPointerSoft()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecPointerSoft::~TrecPointerSoft
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TrecPointerSoft()
	{
		if(pointer)
			pointer->DecrementSoft();
	}

	/**
	 * Method: TrecPointerSoft::Get
	 * Purpose: Retrieves the underlying object, or null if the object is no longer available
	 * Parameters: void
	 * Returns: T* - the object the pointer was referencing
	 */
	T* Get()
	{
		if (pointer)
			return pointer->Get();
		return nullptr;
	}

	/**
	 * Method: TrecPointerSoft::operator=
	 * Purpose: Allows for use of the = operator when assigning this type of object
	 * Parameters: const TrecPointerSoft<T>& other - the Pointer holding the object to assign to this Pointer
	 * Returns: void
	 */
	void operator=(const TrecPointerSoft<T>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		// If we are currently holding on to an existing pointer, decriment it's soft pointer
		if (pointer)
			pointer->DecrementSoft();

		pointer = other.pointer;
		if (pointer)
			pointer->IncrementSoft();
	}
};

/**
 * class TrecSubPointerSoft
 * Purpose: Provides a weak pointer type for the TrecSubPointer (which assumes object is a subclass of type u)
 */
template<class T, class U> class TrecSubPointerSoft
{
	friend class TrecPointerKey;
private:
	/**
	 * counter that holds the reference and reference count
	 */
	TrecBoxPointer<T>* pointer;

	/**
	 * Method: TrecSubPointerSoft::TrecSubPointerSoft
	 * Purpose: Constructor
	 * Parameters: TrecBoxPointer<T>* pointer - the pointer to refer to 
	 * Returns: New TrecSubPointerSoft Object
	 */
	TrecSubPointerSoft(TrecBoxPointer<T>* pointer)
	{
		if (!pointer)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		this->pointer = pointer;

		if (this->pointer)
			this->pointer->IncrementSoft();
	}

public:
	/**
	 * Method: TrecSubPointerSoft::TrecSubPointerSoft
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New TrecSubPointerSoft Object
	 */
	TrecSubPointerSoft()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecSubPointerSoft::~TrecSubPointerSoft
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: New TrecSubPointerSoft Object
	 */
	~TrecSubPointerSoft()
	{
		if (pointer)
			pointer->DecrementSoft();
	}

	/**
	 * Method: TrecSubPointerSoft::Get
	 * Purpose: Retrieves the Sub-type that this Pointer points to
	 * Parameters: void
	 * Returns: U* - the subtype of the object pointed to (or null if the type s not a U)
	 */
	U* Get()
	{
		if (pointer)
			return dynamic_cast<U*>(pointer->Get());
		return nullptr;
	}

	/**
	 * Method: TrecSubPointerSoft::operator=
	 * Purpose: Allows use of the = operator when setting a TrecSubPointerSoft
	 * Parameters:const TrecSubPointerSoft<T, U>& other - reference to the pointer to get the counter from 
	 * Returns: void
	 */
	void operator=(const TrecSubPointerSoft<T, U>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		if (pointer)
			pointer->DecrementSoft();

		pointer = other.pointer;
	}
};

/**
 * class TrecSubPointer
 * Purpose: Provide a means to hold a reference to a base class T and provide easy access to members of sub class U
 */
template<class t, class u> class TrecSubPointer
{
	friend class TrecPointerKey;
	// friend class TrecPointer<t>;
protected:
	/**
	 * counter that holds the reference and reference count
	 */
	TrecBoxPointer<t>* pointer;


	/**
	 * Method: TrecSubPointer::TrecSubPointer
	 * Purpose: Constructor
	 * Parameters: u* raw - pointer to the new object
	 * Returns: New TrecSubPointer Object
	 *
	 * Note: Made protected to ensure that there is only one counter per object. To initialize a new Pointer,
	 *		call "TrecPointerKey::GetNewTrecSubPointer<T,U>(arguments here)" in order to get a new TrecSubPointer
	 */
	TrecSubPointer(u* raw)
	{
		if(!raw)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		t* baseRaw = dynamic_cast<t*>(raw);
		if (!baseRaw)
			throw L"Error! U type must be a subclass of T type!";
		pointer = new TrecBoxPointer<t>(baseRaw);
	}

public:

	/**
	 * Method: TrecSubPointer::TrecSubPointer
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New Blank TrecSubPointer Object
	 */
	TrecSubPointer()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecSubPointer::TrecSubPointer
	 * Purpose: Copy Constructor
	 * Parameters: const TrecSubPointer<t,u>& copy -  the Pointer to copy from
	 * Returns: New Copied TrecSubPointer object
	 */
	TrecSubPointer(const TrecSubPointer<t,u>& copy)
	{
		pointer = copy.pointer;
		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecSubPointer::~TrecSubPointer
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TrecSubPointer()
	{
		Nullify();
	}

	/**
	 * Method: TrecSubPointer::Nullify
	 * Purpose: Sets the Pointer to null and, if not null already, decriments the counter
	 * Parameters: void
	 * Returns: void
	 */
	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

	/**
	 * Method: TrecSubPointer::operator=
	 * Purpose: Allows use of the = operator
	 * Parameters: const TrecSubPointer<t,u>& other - the Pointer to copy from
	 * Returns: void
	 */
	void operator=(const TrecSubPointer<t,u>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		if (pointer)
			pointer->Decrement();

		pointer = other.pointer;

		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecSubPointer::operator->
	 * Purpose: Allows access to the objects public (or accessible) attributes/methods
	 * Parameters: void
	 * Returns: u* - the object to operate on (or a null value that should have been checked for prior)
	 */
	u* operator->()
	{
		if (!pointer) return nullptr;
		return dynamic_cast<u*>(pointer->Get());
	}

	/**
	 * Method: TrecSubPointer::Get
	 * Purpose: Retrieves the underlying raw pointer
	 * Parameters: void
	 * Returns: u* - the sub class of the raw pointer (null if object is not of type u)
	 */
	u* Get()
	{
		if (!pointer) return nullptr;
		return dynamic_cast<u*>(pointer->Get());
	}

	/**
	 * Method: TrecSubPointer::GetBase
	 * Purpose: Retireves the base pointer
	 * Parameters: void
	 * Returns: t* - the raw pointer 
	 */
	t* GetBase()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecSubPointer::Delete
	 * Purpose: Allows for early deletion
	 * Parameters: void
	 * Returns: void
	 */
	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
		pointer->Decrement();
		pointer = nullptr;
	}
};

/**
 * class TrecPointer
 * Purpose: Provide a Smart Pointer for use in Anagame
 */
template<class t> class TrecPointer
{
	friend class TrecPointerKey;
protected:
	/**
	 * counter that holds the reference and reference count
	 */
	TrecBoxPointer<t>* pointer;


	/**
	 * Method: TrecPointer::TrecPointer
	 * Purpose: Constructor
	 * Parameters: t* raw - pointer to the new object
	 * Returns: New TrecPointer Object
	 *
	 * Note: Made protected to ensure that there is only one counter per object. To initialize a new Pointer,
	 *		call "TrecPointerKey::GetNewTrecPointer<T>(arguments here)" in order to get a new TrecPointer
	 */
	TrecPointer(t* raw)
	{
		if (!raw)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		pointer = new TrecBoxPointer<t>(raw);
	}
public:
	/**
	 * Method: TrecPointer::TrecPointer
	 * Purpose: Copy Constructor
	 * Parameters: const TrecPointer<t>& copy - the Pointer to copy from
	 * Returns: New TrecPointer Object
	 */
	TrecPointer(const TrecPointer<t>& copy)
	{
		pointer = copy.pointer;
		if(pointer)
		pointer->Increment();
	}

	/**
	 * Method: TrecPointer::TrecPointer
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New Null TrecPointer Object
	 */
	TrecPointer()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecPointer::~TrecPointer
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TrecPointer()
	{
		// Make sure we decriment the coutner before deletion is complete
		Nullify();
	}

	/**
	 * Method: TrecPointer::Nullify
	 * Purpose: Sets the TrecPointer to Null, making sure to decriment any counter in the process
	 * Parameters: void
	 * Returns: void
	 */
	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

	/**
	 * Method: TrecPointer::operator=
	 * Purpose: Allows the use of the = operator
	 * Parameters: const TrecPointer<t>& other - the Pointer to copy from
	 * Returns: void
	 */
	void operator=(const TrecPointer<t>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		if(pointer)
			pointer->Decrement();

		pointer = other.pointer;

		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecPointer::As
	 * Purpose: Retrieves a raw pointer in a new form
	 * Parameters: void
	 * Returns: u* the raw pointer in the new type
	 *
	 * Note: DEPRECATED - For alterntive type access, there is the TrecSubPointer<T,U> which share counters with the TrecPointer<T>
	 */
	template<class u> u* As()
	{
		if (!pointer)
			return nullptr;
		t* raw = pointer->Get();
		if (!raw)
			return nullptr;

		u* altRaw = dynamic_cast<u*>(raw);

		if (!altRaw)
			throw (L"Error! Attempted to retrieve a pointer of an incompatible type!");
		return altRaw;
	}

	/**
	 * Method: TrecPointer::Get
	 * Purpose: Retrieves the raw pointer of the t object
	 * Parameters: void
	 * Returns: t* - the raw pointer
	 */
	t* Get()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecPointer::operator->
	 * Purpose: Allows access to the objects public (or accessible) attributes/methods
	 * Parameters: void
	 * Returns: t* - the object to operate on (or a null value that should have been checked for prior)
	 */
	t* operator->()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecPointer::Delete
	 * Purpose: Allows for early deletion
	 * Parameters: void
	 * Returns: void
	 */
	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
		pointer->Decrement();
		pointer = nullptr;
	}
};


/**
 * class TrecComPointer
 * Purpose: Provide a smart pointer that can be used on Microsoft COM objects
 *
 * To initialize a new object, the TrecComPointer provides the TrecComHolder class. Here is an example on how to initialize a ID2D1Factory1 object:
 *
 *		TrecComPointer<ID2D1Factory1>::TrecComHolder factoryHolder;			// Declare the holder object
 *		D2D1_FACTORY_OPTIONS d2dDebugLevel = { D2D1_DEBUG_LEVEL_ERROR };v	// prepare the parameters for the factory function
 *		
 *		// Use the holder's GetPointerAddress to get the new object
 *		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, d2dDebugLevel, factoryHolder.GetPointerAddress());
 *		factory = factoryHolder.Extract();									// Call Extract to get the functional TrecComPointer
 *	
 *
 * Suppose you wish to initialize an ID2D1HwndRenderTarget but wish to store it in an ID2D1RenderTarget Pointer. You'll need the
 *	TrecPointerKey for that.
 *
 * 		TrecComPointer<ID2D1HwndRenderTarget>::TrecComHolder renderHw;		// Declare the holder
 *
 *		// assume props and hProps are ready for use
 *		HRESULT res = fact->CreateHwndRenderTarget(props, hProps, renderHw.GetPointerAddress());
 *
 *		// Use the TrecPointerKey to get the TrecComPointer
 *		renderer = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1HwndRenderTarget>(renderHw);
 */
template<class t> class TrecComPointer
{
	friend class TrecPointerKey;
protected:
	/**
	 * counter that holds the reference and reference count
	 */
	TrecComBoxPointer<t>* pointer;
public:
	/**
	 * class TrecComHolder
	 * Purpose: Provide a means through which TrecComPointers can be initialized when the means to create them is 
	 *	through factory methods/functions
	 */
	class TrecComHolder
	{
		friend class TrecPointerKey;
	protected:
		/**
		 * pointer to the COM object to be initialized
		 */
		t* holder;

	public:

		/**
		 * Method: TrecComHolder::TrecComHolder
		 * Purpose: Default Constructor
		 * Parameters: void
		 * Returns: New Blank TrecComHolder Object
		 */
		TrecComHolder()
		{
			holder = nullptr;
		}

		/**
		 * Method: TrecComHolder::TrecComHolder
		 * Purpose: Returns a pointer to the raw pointer for Factory methods and functions to use to initialize the com object
		 * Parameters: void
		 * Returns: t** - pointer to the raw pointer of the type
		 */
		t** GetPointerAddress()
		{
			return &holder;
		}

		void OfferValue(TrecComPointer<t>& point)
		{
			holder = point.Get();
		}

		/**
		 * Method: TrecComHolder::Extract
		 * Purpose: Provides a parameterized means of initializing the TrecComPointer
		 * Parameters: TrecComPointer<t>& point - the Pointer to initialize
		 * Returns: void 
		 */
		void Extract(TrecComPointer<t>& point)
		{
			if (!holder)
				throw L"Error! Cannot Extract Smart Pointer with raw value!";
			if (holder != point.Get())
			{
				point.Nullify();
				point = TrecComPointer<t>(holder);
			}
			holder = nullptr;
		}

		/**
		 * Method: TrecComHolder::Extract
		 * Purpose: Retrieves the value procured from the Factory call
		 * Parameters: void
		 * Returns: TrecComPointer<t> - the New initialized TrecComPointer
		 *
		 * Note 1: Make sure that the Holder is holding a value, otherwise an exception is thrown.
		 * Note 2: To prevent multiple counters to the same object, this method only works once, after that, it nullifies itself
		 * Note 3: If you wish to use a different (compatible type), use the TrecPointerKey's "GetComPointer" function
		 */
		TrecComPointer<t> Extract()
		{
			if (!holder)
				throw L"Error! Cannot Extract Smart Pointer with raw value!";
			TrecComPointer<t> ret(holder);
			holder = nullptr;
			return ret;
		}

	};

protected:

	/**
	 * Method: TrecComPointer::TrecComPointer
	 * Purpose: Constructor
	 * Parameters: t* raw - pointer to the new object
	 * Returns: New TrecComPointer Object
	 *
	 * Note: Made protected to prevent the creation of multiple counters to the same reference. To initialize a new
	 *		COM object, use the TrecComPointer::TrecComHolder class
	 */
	TrecComPointer(t* raw)
	{
		if (!raw)
			throw (L"Error! TrecPointers must be initialized with a pointer, not NULL!");
		pointer = new TrecComBoxPointer<t>(raw);
	}

public:

	/**
	 * Method: TrecComPointer::
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New Null TrecComPointer Object
	 */
	TrecComPointer()
	{
		pointer = nullptr;
	}

	/**
	 * Method: TrecComPointer::~TrecComPointer
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TrecComPointer()
	{
		// Make sure we decriment the coutner before deletion is complete
		Nullify();
	}

	/**
	 * Method: TrecComPointer::TrecComPointer
	 * Purpose: Copy Constructor
	 * Parameters: const TrecComPointer<t>& copy - the Pointer to copy from
	 * Returns: New TrecComPointer Object
	 */
	TrecComPointer(const TrecComPointer<t>& other)
	{
		pointer = other.pointer;

		if (pointer)
			pointer->Increment();
	}

	/**
	 * Method: TrecComPointer::operator=
	 * Purpose: Allows the use of the = operator
	 * Parameters: const TrecComPointer<t>& other - the Pointer to copy from
	 * Returns: void
	 */
	void operator=(const TrecComPointer<t>& other)
	{
		// First, make sure that the other TrecPointer isn't pointing to the same reference.
		// If it is, we don't want to decrement the reference lest it become 0 (and self-destructs)
		if (other.pointer == pointer)
			return;

		if(pointer)
		pointer->Decrement();
		pointer = other.pointer;
		if(pointer)
		pointer->Increment();
	}


	/**
	 * Method: TrecComPointer::
	 * Purpose: Sets the TrecPointer to Null, making sure to decriment any counter in the process
	 * Parameters: void
	 * Returns: void
	 */
	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}


	/**
	 * Method: TrecComPointer::Get
	 * Purpose: Retrieves the raw pointer of the t object
	 * Parameters: void
	 * Returns: t* - the raw pointer
	 */
	t* Get()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose: Allows access to the objects public (or accessible) attributes/methods
	 * Parameters: void
	 * Returns: t* - the object to operate on (or a null value that should have been checked for prior)
	 */
	t* operator->()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	/**
	 * Method: TrecComPointer::
	 * Purpose: Allows for early deletion
	 * Parameters: void
	 * Returns: void
	 */
	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
		pointer->Decrement();
		pointer = nullptr;
	}
};




/**
 * class TrecPointerKey
 * Purpose: Utility class to allow for initializing various TrecPointer types and swapping compatible TrecPointer types
 */
class TrecPointerKey
{
public:

	/**
	 * Method: static TrecPointerKey::GetTrecPointerFromSoft<T>
	 * Purpose: Retrieves a regular TrecPointer from an instance of a TrecPointerSoft
	 * Parameters: TrecPointerSoft<T>& soft - the weak pointer with the counter
	 * Returns: TrecPointer<T> - a regular TrecPointer with the object held by the soft pointer
	 */
	template <class T> static TrecPointer<T> GetTrecPointerFromSoft(TrecPointerSoft<T>& soft)
	{
		TrecPointer<T> ret;
		ret.pointer = soft.pointer;
		if(ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetSoftPointerFromTrec
	 * Purpose: Retrieves a Soft pointer from a regular TrecPointer
	 * Parameters: TrecPointer<T>& trec -  the TrecPointer to get a Soft pointer from
	 * Returns: TrecPointerSoft<T> -  the Soft pointer with the same reference a the parameter
	 */
	template <class T> static TrecPointerSoft<T> GetSoftPointerFromTrec(TrecPointer<T>& trec)
	{
		return TrecPointerSoft<T>(trec.pointer);
	}

	/**
	 * Method: static TrecPointerKey::GetNewSelfTrecPointer
	 * Purpose:  Creates a new object and returns it in a regular TrecPointer
	 * Parameters: types&& ... args - the arguments that get passed to the constructor of the object
	 * Returns: TrecPointer<t> - The regular TrecPointer with a new object
	 *
	 * Note: This method will only compile if the t type provides a "SetSelf" method used to hold a soft pointer to itself
	 */
	template <class T, class...types> static TrecPointer<T> GetNewSelfTrecPointer(types&& ... args)
	{
		T* raw = new T(args...);
		TrecPointer<T> ret(raw);
		raw->SetSelf(ret);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetNewTrecPointer
	 * Purpose: Creates a new object and returns it in a regular TrecPointer
	 * Parameters: types&& ... args - the arguments that get passed to the constructor of the object
	 * Returns: TrecPointer<t> - The regular TrecPointer with a new object
	 */
	template <class t, class...types> static TrecPointer<t> GetNewTrecPointer(types&& ... args)
	{
		t* raw = new t(args...);
		TrecPointer<t> ret(raw);
		return ret;
	}


	/**
	 * Method: static TrecPointerKey::GetNewSelfTrecPointerAlt
	 * Purpose: Creates an object of type u that is then stored in a TrecPointer of type T (with type T having a SetSelf method)
	 * Parameters: types&& ... args - the arguments that get passed to the constructor of the object
	 * Returns:TrecPointer<t> - the T TrecPointer holding the U type
	 * 
	 * Note: This method will only compile if the t type provides a "SetSelf" method used to hold a soft pointer to itself
	 */
	template <class t, class u, class...types> static TrecPointer<t> GetNewSelfTrecPointerAlt(types&& ... args)
	{
		TrecPointer<t> ret = GetNewTrecPointerAlt<t, u>(args...);
		if (ret.Get())
			ret->SetSelf(ret);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetNewTrecPointerAlt
	 * Purpose: Creates an object of type u that is then stored in a TrecPointer of type T
	 * Parameters: types&& ... args - the arguments that get passed to the constructor of the object
	 * Returns: TrecPointer<t> - the T TrecPointer holding the U type
	 */
	template <class t, class u, class...types> static TrecPointer<t> GetNewTrecPointerAlt(types&& ... args)
	{
		u* raw = new u(args...);
		t* raw_t = nullptr;


		// Need to make sure that the alternative type 'u' is compatible with this type 't'

		raw_t = dynamic_cast<t*>(raw);


		if (!raw_t)
		{
			delete raw;
			throw L"Error! Attempted to assign incompatible type!";
		}
		TrecPointer<t> ret(raw_t);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetTrecPointerFromSub
	 * Purpose: Provides a regular TrecPointer from the provided Sub Pointer
	 * Parameters: TrecSubPointer<t, u>& sub - the Sub Pointer to get the regular TrecPointer
	 * Returns: TrecPointer<t> - the Regular TrecPointer 
	 */
	template <class t, class u> static TrecPointer<t> GetTrecPointerFromSub(TrecSubPointer<t, u>& sub)
	{
		TrecPointer<t> ret;
		ret.pointer = sub.pointer;
		if (ret.pointer && ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetTrecSubPointerFromTrec
	 * Purpose:
	 * Parameters: const TrecPointer<t>& tPointer - Retrieves a Sub Pointer from the Regular Trec Pointer
	 * Returns: TrecSubPointer<t, u> - the Pointer that assumes the object is of type u
	 *
	 * Note: if the object held by the TrecPointer is NOT of type u, the returned pointer will effectively act like a null pointer
	 */
	template <class t, class u> static TrecSubPointer<t, u> GetTrecSubPointerFromTrec(const TrecPointer<t>& tPointer)
	{
		TrecSubPointer<t, u> ret;
		ret.pointer = tPointer.pointer;
		if (ret.pointer && ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetNewTrecSubPointer
	 * Purpose: Initializes an object of type u that has a reference to itself and can operate with TrecPointer<t>
	 * Parameters: types&& ... args - the arguments that get passed to the constructor of the object
	 * Returns: TrecSubPointer<t, u> - the SubPointer with a reference to the u object
	 */
	template <class t, class u, class...types> static TrecSubPointer<t, u> GetNewTrecSubPointer(types&& ... args)
	{
		u* raw = new u(args...);
		TrecSubPointer<t, u> ret(raw);
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetNewSelfTrecSubPointer
	 * Purpose: Initializes an object of type u that has a reference to itself and can operate with TrecPointer<t>
	 * Parameters: types&& ... args - the arguments that get passed to the constructor of the object
	 * Returns: TrecSubPointer<t, u> - the SubPointer with a reference to the u object
	 *
	 * Note: This method will only compile if the t type provides a "SetSelf" method used to hold a soft pointer to itself
	 */
	template <class t, class u, class...types> static TrecSubPointer<t, u> GetNewSelfTrecSubPointer(types&& ... args)
	{
		u* raw = new u(args...);
		TrecSubPointer<t, u> ret(raw);
		ret->SetSelf(GetTrecPointerFromSub<t>(ret));
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetComPointer
	 * Purpose:
	 * Parameters:typename TrecComPointer<u>::TrecComHolder& holder - the Holder to extract the value from
	 * Returns: TrecComPointer<t> -  the Pointer with the desired type specified.
	 *
	 * Note: provided to handle cases where you want to create a specific COM object like ID2D1HwndRenderTarget and wish
	 *		to use a more broad type such as ID2D1RenderTarget
	 */
	template <class t, class u> static TrecComPointer<t> GetComPointer(typename TrecComPointer<u>::TrecComHolder& holder)
	{
		if (!holder.holder)
			throw L"Error! Expected valid value";

		t* raw = dynamic_cast<t*>(holder.holder);

		if (!raw)
			throw L"Error! Dealing with incompatible types!";

		// Prevent a second counter from being created later on
		holder.holder = nullptr;

		return TrecComPointer<t>(raw);
	}

	/**
	 * Method: static TrecPointerKey::GetSubPointerFromSoft
	 * Purpose: Retrieves an active Sub Pointer from a Soft version of the pointer
	 * Parameters: TrecSubPointerSoft<T, U> s - The Soft pointer to get the strong one from
	 * Returns:TrecSubPointer<T, U> -  the Sub Pointer sought 
	 */
	template <class T, class U> static TrecSubPointer<T, U> GetSubPointerFromSoft(TrecSubPointerSoft<T, U> s)
	{
		TrecSubPointer<T, U> ret;

		ret.pointer = s.pointer;
		if (ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	/**
	 * Method: static TrecPointerKey::GetSoftSubPointerFromSub<T,U>
	 * Purpose: Retrieves a Soft Sub Pointer from a Sub Trec Pointer
	 * Parameters: TrecSubPointer<T, U> trec -  the Pointer with the object
	 * Returns: TrecSubPointerSoft<T, U> trec - the Soft version of the TrecSubPointer
	 */
	template <class T, class U> static TrecSubPointerSoft<T, U> GetSoftSubPointerFromSub(TrecSubPointer<T, U> trec)
	{
		return TrecSubPointerSoft<T,U>(trec.pointer);
	}

	/**
	 * Method: static TrecPointerKey::GetSoftSubPointerFromSoft<T,U>
	 * Purpose: Retrieves a Soft Sub Pointer from a Soft Trec Pointer
	 * Parameters: TrecPointerSoft<T> trec -  the Pointer with the object
	 * Returns: TrecSubPointerSoft<T, U> trec - the Soft version of the TrecSubPointer
	 */
	template <class T, class U> static TrecSubPointerSoft<T, U> GetSoftSubPointerFromSoft(TrecPointerSoft<T> trec)
	{
		if(trec.pointer)
			return TrecSubPointerSoft<T, U>(trec.pointer);
		return TrecSubPointerSoft<T, U>();
	}

	/**
	 * Method: static TrecPointerKey::GetTrecObjectPointer<T>
	 * Purpose: Retrieves a TrecObjectPointer that just sees a TObject
	 * Parameters: TrecPointer<T> - the TrecPointer to convert
	 * Returns: TrecObjectPointer - Smart Pointer that holds the object as a TObject, without regard for the specifc type held
	 * 
	 * Note: this can be useful for Interpretors that don't need to track multiple Anagame Object types but just care about whether they are a TObject or not
	 */
	template <class T> static TrecObjectPointer GetTrecObjectPointer(TrecPointer<T> obj)
	{
		T* temp = obj.Get();
		if (dynamic_cast<TObject*>(temp))
		{
			return TrecObjectPointer(obj.pointer);
		}
		return TrecObjectPointer();
	}
};