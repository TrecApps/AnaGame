#pragma once
#include "TrecReference.h"
#include "TArrayBase.h"

/*
* Class TArray
* Holds references to objects (Not meant to store straight foward data in a row)
*/
template <typename t>class _TREC_LIB_DLL TArray : public TArrayBase
{
public:
	/*
	* Method: TArray::Tarray
	* Purpose: Default Constructor
	* Parameters: void
	* Returns: void
	*/
	TArray() 
	{
		
	}

	/*
	* Method: TArray::~TArray
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~TArray()
	{
		extension.Nullify();
	}

	/*
	* Method: TArray::Add
	* Purpose: Appends a new Object on the TArray
	* Parameters: TrecPointer<t> loc - the object to add
	* Returns: bool - success or failure
	*/
	bool Add(TrecPointer<t> loc)
	{
		if (loc.Get() == NULL)
			return false;
		if (count == 100) // first container is full
		{
			extension = TrecPointerKey::GetNewTrecPointer<TArray<t>>();
			extension->Add(loc);
		}
		else if (count > 100)
		{
			if (!extension.Get())
				return false;
			extension->Add(loc);
		}
		else
			data[count / 10][count % 10] = loc;
		
		count++;
		return true;
	}

	/*
	* Method: TArray::ElementAt
	* Purpose: Retrieves the Element stored at a specified location
	* Parameters: UINT at - the element to look at
	* Returns: TrecPointer<t> loc - the element at the location (call .Get() to make sure it's not NULL)
	*/
	TrecPointer<t> ElementAt(UINT at)
	{
		if (at > count)
			return TrecPointer<t>();
		if (at >= 100)
		{
			if (!(extension.Get()))
				return TrecPointer<t>();
			return extension->ElementAt(at - 100);
		}
		return data[at / 10][at % 10];
	}

	TObject* GetObjectAt(UINT loc)
	{
		try
		{
			return dynamic_cast<TObject*>(ElementAt(loc).Get());
		}
		catch (...)
		{
			return nullptr;
		}
	}

	/*
	* Method: TArray::RemoveAt
	* Purpose: Removes an element at a certain location
	* Parameters: UINT loc - the Index of the element to remove
	* Returns: bool - success result
	*/
	TrecPointer<t> RemoveAt(UINT loc)
	{
		if (loc > count)
			return TrecPointer<t>();
		count--;
		if (loc > 99)
		{
			if (!extension.Get())
				return TrecPointer<t>();
			return extension->RemoveAt(loc - 100);
		}
		else
		{
			TrecPointer<t> point;
			point = data[loc / 10][loc % 10];
			data[loc / 10][loc % 10] = TrecPointer<t>();
			clearNull();
			return point;
		}
	}

	/*
	* Method: TArray::DeleteAt
	* Purpose: Deletes an element at a certain location
	* Parameters: UINT loc - the Index of the element to delete
	* Returns: bool - success result
	*/
	bool DeleteAt(UINT loc)
	{
		if (loc > count)
			return false;
		count--;
		if (loc > 99)
		{
			if (!extension)
				return false;
			return extension->DeleteAt(loc - 100);
		}
		else
		{
			data[loc / 10][loc % 10].Delete();
			data[loc / 10][loc % 10].Nullify();
			clearNull();
			return true;
		}
	}

	/*
	* Method: TArray::Clear
	* Purpose: Sets all elements to null
	* Parameters: void
	* Returns: void
	*/
	void Clear()
	{
		if (extension.Get())
		{
			extension->Clear();
			//delete extension;
			extension.Nullify();
		}
		else
		{
			for (int c = 0; c < 10; c++)
			{
				for (int d = 0; d < 10; d++)
				{
					data[c][d].Nullify();
				}
			}
		}
		count = 0;
	}

	/*
	* Method: TArray::setAt
	* Purpose: Sets an exising element to a new element
	* Parameters: UINT loc - the index to reset
	*			TrecPointer<t> newV - the new value to use
	* Returns:
	*/
	bool setAt(UINT loc, TrecPointer<t> newV)
	{
		if (loc > this->Count() || loc < 0)
			return false;
		if (count > 99)
		{
			extension->setAt(loc-100, newV);
		}
		else
		{
			data[loc / 10][loc % 10] = newV;
		}
		
		return true;
	}

	/*
	* Method: TArray::operator=
	* Purpose: Copies an existing TArray
	* Parameters: TArray<t>& cop - the array to copy
	* Returns: void
	*/
	void operator=(TArray<t>& cop)
	{
		for (UINT c = 0; c < cop.Count();c++)
		{
			this->Add(cop.ElementAt(c));
		}
	}

private:
	/**
	 * Holds the data
	 */
	TrecPointer<t> data[10][10];
	/**
	 * Allows Array to be extended
	 */
	TrecPointer<TArray<t>> extension;

	/*
	* Method: TArray::clearNull
	* Purpose: Removes Null elements in between non-null elements
	* Parameters: bool isExt - whehter or not the TArray is part of the extension of an original array
	* Returns: void
	*/
	void clearNull(bool isExt = false)
	{
		bool foundNull = false;
		if (isExt)
			data[0][0].Nullify();
		for (UINT c = 0; c < count + 1 && c < 100; c++)
		{
			if (ElementAt(c).Get() == nullptr)
				foundNull = true;
			if (foundNull)
				data[c/10][c%10] = data[(c+1)/10][(c+1)%10];
		}
		if (extension.Get())
		{
			data[99/10][99%10] = extension->ElementAt(0);
			extension->clearNull(true);
		}
	}
};

