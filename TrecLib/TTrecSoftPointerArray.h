#pragma once
#include "TDataArrayBase.h"
#include "TrecReference.h"
/*
* Class TArray
* Holds references to objects (Not meant to store straight foward data in a row)
*/
template <typename T>class TTrecPointerSoftArray : public TDataArrayBase
{
private:
	TrecPointerSoft<T>* array;


public:
	/*
	* Method: (TTrecPointerArray) (Constructor)
	* Purpose: Sets up a TDataArray based off an existing one
	* Parameters: TDataArray<T>& newArray - the Data Array to copy
	* Returns: void
	*/
	TTrecPointerSoftArray(const TTrecPointerArray<T>& newArray) : TDataArrayBase(newArray.Size(), newArray.Capacity())
	{
		array = new TrecPointerSoft<T>[newArray.Capacity()];

		for (UINT Rust = 0; Rust < newArray.Size(); Rust++)
		{
			if (Rust < newArray.Capacity())
				array[Rust] = newArray.array[Rust];
		}
	}

	/*
	* Method: TTrecPointerArray - operator[]
	* Purpose: Gives the TDataArray array like functionality
	* Parameters: size_t c - index to target
	* Returns: T& - element at index
	*/
	TrecPointer<T> operator[](size_t c)
	{
		if (c >= size)
			throw new TString(L"IndexOutOfBounds");
		return TrecPointerKey::GetTrecPointerFromSoft<T>(array[c]);
	}

	/*
	* Method: TTrecPointerArray - operator=
	* Purpose: Copies the contents of an exisitng data array to the TDataArray
	* Parameters: TDataArray<T>& newArray - the Data Array to copy
	* Returns: void
	*/
	void operator=(const TTrecPointerSoftArray<T>& newArray)
	{
		if (array)
			delete[] array;
		capacity = newArray.Capacity();
		array = new TrecPointerSoft<T>[capacity];
		size = newArray.Size();

		for (UINT c = 0; c < size && c < capacity; c++)
		{
			if (c < capacity)
				array[c] = newArray.array[c];
		}//memcpy_s(array, capacity, newArray.GetArray(), capacity);
	}

	/*
	* Method: TTrecPointerArray - at
	* Purpose: returns element at a given index
	* Parameters: UINT c - the index to target
	* Returns: T& - element at index
	*/
	TrecPointer<T> at(UINT c)
	{
		if (c >= size)
			throw new TString(L"IndexOutOfBounds");
		return TrecPointerKey::GetTrecPointerFromSoft<T>(array[c]);
	}

	/*
	* Method: (TTrecPointerArray) (Constructor)
	* Purpose: Sets up a TDataArray
	* Parameters: void
	* Returns: void
	*/
	TTrecPointerSoftArray() : TDataArrayBase(0, 5)
	{
		array = new TrecPointerSoft<T>[5];
	}

	/*
* Method: (TTrecPointerArray) (Destructor)
* Purpose: Cleans up the Data Array
* Parameters: void
* Returns: void
*/
	~TTrecPointerSoftArray()
	{
		delete[] array;
	}



	/*
	* Method: TTrecPointerArray - Capacity
	* Purpose: Reports the size of the underlying array
	* Parameters: void
	* Returns: UINT - the size of the unerlying array being used
	*/
	UINT Capacity() const
	{
		return capacity;
	}

	/*
	* Method: TTrecPointerArray - push_back
	* Purpose: Appends an element to the DataArray
	* Parameters: T element - the data to append
	* Returns: UINT - the new size of the DataArray
	*/
	UINT push_back(TrecPointer<T> element)
	{
		if (size >= capacity)
		{
			TrecPointerSoft<T>* newArray = new TrecPointerSoft<T>[capacity + 5];
			for (UINT c = 0; c < capacity; c++)
				newArray[c] = array[c];
			//memcpy_s(newArray, capacity, array, capacity);
			delete[] array;
			array = newArray;
			capacity += 5;
		}
		array[size++] = TrecPointerKey::GetSoftPointerFromTrec<T>(element);
		return size - 1;
	}

	/*
* Method: TTrecPointerArray - RemoveAt
* Purpose: Removes an element at a certain location
* Parameters: UINT c - the location to remove
* Returns: T - the data stored at that location
*/
	TrecPointer<T> RemoveAt(UINT c)
	{
		if (c >= size)
			throw new TString(L"IndexOutOfBounds");
		TrecPointerSoft<T> returnable = array[c];

		for (; c < size - 1; c++)
			array[c] = array[c + 1];
		size--;
		return TrecPointerKey::GetTrecPointerFromSoft<T>(returnable);
	}

	/*
	* Method: TTrecPointerArray - RemoveAll
	* Purpose: Removes all emements and sets count to 0
	* Parameters: void
	* Returns: void
	*/
	void RemoveAll()
	{
		if (array)
			delete[] array;
		array = new TrecPointerSoft<T>[capacity = 5];
		size = 0;
	}

	TObject* GetObjectAt(UINT loc)
	{
		try
		{
			return dynamic_cast<TObject*>(at(loc).Get());
		}
		catch (...)
		{
			return nullptr;
		}
	}

};