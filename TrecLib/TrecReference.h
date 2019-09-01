#pragma once
#include <WinBase.h>

template<class T> class TrecBoxPointer
{
	friend class TrecPointerKey;
private:
	T* rawPointer;
	UINT counter;
public:
	TrecBoxPointer(T* t)
	{
		counter = (t)? 1 : 0;
		rawPointer = t;
	}

	~TrecBoxPointer()
	{
		if (rawPointer)
			delete rawPointer;
	}

	void Increment()
	{
		counter++;
	}

	void Decrement()
	{
		counter--;
		if (!counter)
			delete this;
	}

	T* Get()
	{
		return rawPointer;
	}

	void Delete()
	{
		delete rawPointer;
		rawPointer = nullptr;
	}
};

template<class T> class TrecComBoxPointer
{
private:
	T* rawPointer;
	UINT counter;
public:
	TrecComBoxPointer(T* t)
	{
		counter = (t) ? 1 : 0;
		rawPointer = t;
	}

	~TrecComBoxPointer()
	{
		if (rawPointer)
			rawPointer->Release();
	}

	void Increment()
	{
		counter++;
	}

	void Decrement()
	{
		counter--;
		if (!counter)
			delete this;
	}

	T* Get()
	{
		return rawPointer;
	}

	void Delete()
	{
		rawPointer->Release();
		rawPointer = nullptr;
	}
};


template<class T> class TrecPointerSoft
{
	friend class TrecPointerKey;
private:
	TrecBoxPointer<T>* pointer;

	TrecPointerSoft(TrecBoxPointer<T>* pointer)
	{
		if(!pointer)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		this->pointer = pointer;
	}

public:
	TrecPointerSoft()
	{
		pointer = nullptr;
	}

	T* Get()
	{
		if (pointer)
			return pointer->Get();
		return nullptr;
	}
};

template<class t> class TrecPointer
{
	friend class TrecPointerKey;
protected:
	TrecBoxPointer<t>* pointer;
	TrecPointer(t* raw)
	{
		if (!raw)
			throw L"Error! TrecPointers must be initialized with a pointer, not NULL!";
		pointer = new TrecBoxPointer<t>(raw);
	}
public:
	TrecPointer(const TrecPointer<t>& copy)
	{
		pointer = copy.pointer;
		if(pointer)
		pointer->Increment();
	}

	TrecPointer()
	{
		pointer = nullptr;
	}

	~TrecPointer()
	{
		Nullify();
	}

	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

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

	t* Get()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	t* operator->()
	{
		if (!pointer) return nullptr;
		return pointer->Get();
	}

	void Delete()
	{
		if (!pointer) return;
		pointer->Delete();
	}
};



template<class t> class TrecComPointer
{
	friend class TrecPointerKey;
protected:
	TrecComBoxPointer<t>* pointer;
public:
	class TrecComHolder
	{
		friend class TrecPointerKey;
	protected:
		t* holder;

	public:

		TrecComHolder()
		{
			holder = nullptr;
		}

		t** GetPointerAddress()
		{
			return &holder;
		}

		void OfferValue(TrecComPointer<t>& point)
		{
			holder = point.Get();
		}

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
	TrecComPointer(t* raw)
	{
		if (!raw)
			throw (L"Error! TrecPointers must be initialized with a pointer, not NULL!");
		pointer = new TrecComBoxPointer<t>(raw);
	}

public:

	TrecComPointer()
	{
		pointer = nullptr;
	}

	~TrecComPointer()
	{
		Nullify();
	}

	TrecComPointer(const TrecComPointer<t>& other)
	{
		pointer = other.pointer;

		if (pointer)
			pointer->Increment();
	}

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


	void Nullify()
	{
		if (pointer)
			pointer->Decrement();
		pointer = nullptr;
	}

	template<class u> u* As()
	{
		if (!pointer) return nullptr;

		t* raw = pointer->Get();
		if (!raw)
			return nullptr;

		u* altRaw = dynamic_cast<u*>(raw);

		if (!altRaw)
			throw (L"Error! Attempted to retrieve a pointer of an incompatible type!");
		return altRaw;
	}

	t* Get()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	t* operator->()
	{
		if (!pointer)return nullptr;
		return pointer->Get();
	}

	void Delete()
	{
		if(pointer)
		pointer->Delete();
	}
};

class TrecPointerKey
{
public:

	template <class T> static TrecPointer<T> GetTrecPointerFromSoft(TrecPointerSoft<T>& soft)
	{
		TrecPointer<T> ret;
		ret.pointer = soft.pointer;
		if(ret.pointer)
			ret.pointer->Increment();
		return ret;
	}

	template <class T> static TrecPointerSoft<T> GetSoftPointerFromTrec(TrecPointer<T>& trec)
	{
		return TrecPointerSoft<T>(trec.pointer);
	}

	template <class T, class...types> static TrecPointer<T> GetNewSelfTrecPointer(types&& ... args)
	{
		T* raw = new T(args...);
		TrecPointer<T> ret(raw);
		raw->SetSelf(ret);
		return ret;
	}

	template <class t, class...types> static TrecPointer<t> GetNewTrecPointer(types&& ... args)
	{
		t* raw = new t(args...);
		TrecPointer<t> ret(raw);
		return ret;
	}

	template <class T> static TrecPointer<T> GetNewTrecPointerPlain()
	{
		T* raw = new T();
		TrecPointer<T> ret(raw);
		return ret;
	}

	template <class t, class u, class...types> static TrecPointer<t> GetNewSelfTrecPointerAlt(types&& ... args)
	{
		TrecPointer<t> ret = GetNewTrecPointerAlt<t, u>(args...);
		if (ret.Get())
			ret->SetSelf(ret);
		return ret;
	}

	template <class t, class u, class...types> static TrecPointer<t> GetNewTrecPointerAlt(types&& ... args)
	{
		u* raw = new u(args...);
		t* raw_t = nullptr;


		// Need to make sure that the alternative type 'u' is compatible with this type 't'
		try
		{
			raw_t = dynamic_cast<t*>(raw);
		}
		catch (std::bad_cast& ex)
		{
			raw_t = nullptr;
		}

		if (!raw_t)
		{
			delete raw;
			throw L"Error! Attempted to assign incompatible type!";
		}
		TrecPointer<t> ret(raw_t);
		return ret;
	}

	template <class t, class u> static TrecComPointer<t> GetComPointer(typename TrecComPointer<u>::TrecComHolder& holder)
	{
		if (!holder.holder)
			throw L"Error! Expected valid value";

		t* raw = dynamic_cast<t*>(holder.holder);

		if (!raw)
			throw L"Error! Dealing with incompatible types!";

		holder.holder = nullptr;

		return TrecComPointer<t>(raw);
	}



};