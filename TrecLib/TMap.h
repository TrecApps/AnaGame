#pragma once
#include "TArray.h"
#include "TString.h"
#include "TMapBase.h"
/*
* Class: tEntry
* Purpose: Holds the entry to the TMap
*/
template<class t2>  class _TREC_LIB_DLL tEntry : TObject
{
public:
	/**
	 * Method:: tEntry::tEntry
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new TEntry
	 */
	tEntry()
	{
		
	}
	/**
	 * The object to hold
	 */
	TrecPointer<t2> object;
	/**
	 * the key to the object
	 */
	TString key;
};

/*
* Class: TMap
* Holds data with a key
*/
template<class t2>class _TREC_LIB_DLL TMap : public TMapBase
{
	friend class TMap<t2>;


public:
	/*
	* Method: TMap::TMap
	* Purpose: Default Constructor
	* Parameters:
	* Returns:
	*/
	TMap() {}

	/*
	* Method: TMap::~TMap
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~TMap() { map.Clear(); }

	/*
	* Method: TMap::addEntry
	* Purpose: Adds an entry to the TMap
	* Parameters: TString& str - the key
	*			TrecPointer<t2> obj - the object itself
	* Returns: void
	*/
	void addEntry(const TString& str, TrecPointer<t2> obj)
	{
		TrecPointer<tEntry<t2>> ent = TrecPointerKey::GetNewTrecPointer<tEntry<t2>>();

		ent->key = str;
		ent->object = obj;
		map.Add(ent);
	}

	/*
	* Method: TMap::GetEntryAt
	* Purpose: Retrieves an entry at a specific index
	* Parameters: UINT c - the index to target
	* Returns: TrecPointer<tEntry<t2>> - the entry to return
	*/
	TrecPointer<tEntry<t2>> GetEntryAt(UINT c)
	{
		if (c < map.Count())
			return map.ElementAt(c);
		return TrecPointer<tEntry<t2>>();
	}

	/*
	* Method: TMap::retrieveEntry
	* Purpose: Retrieves an entry based on a given key
	* Parameters: TString& str - the key
	* Returns: TrecPointer<t2> - the entry (call .get() to check it's null status)
	*/
	TrecPointer<t2> retrieveEntry(const TString& str)
	{
		for (int c = 0; c < map.Count(); c++)
		{
			TrecPointer<tEntry<t2>> point = map.ElementAt(c);

			if (!str.Compare(point->key))
				return (point->object);
		}
		return TrecPointer<t2>();
	}

	/*
	* Method: TMap::retrieveEntry
	* Purpose: Retrieves an entry based on a given key
	* Parameters: TString& str - the key
	*			int occurance - the number of successes to skip
	* Returns: TrecPointer<t2> - the entry (call .get() to check it's null status)
	*/
	TrecPointer<t2> retrieveEntry(const TString& str, int occurance)
	{
		int occ = 0;
		for (int c = 0; c < map.Count(); c++)
		{
			TrecPointer<tEntry<t2>> point = map.ElementAt(c);
			
			if (!point->key.Compare(str))
			{
				if (occ == occurance)
					return (point->object);
				else
					occ++;
			}
		}
		return TrecPointer<t2>();
	}

	/*
	* Method: TMap::removeEntry
	* Purpose: Removes an entry from the map
	* Parameters: TString& str - the key
	* Returns: TrecPointer<t2> - the entry found
	*/
	TrecPointer<t2> removeEntry(TString& str)
	{
		for (int c = 0; c < map.Count(); c++)
		{
			TrecPointer<tEntry<t2>> point = map.ElementAt(c);
			if (point->key == str)
			{
				return map.RemoveAt(c);
			}
		}
		return nullptr;
	}
	
	/*
	* Method: TMap::clear
	* Purpose: clears the map of all entries
	* Parameters: void
	* Returns: void
	*/
	void clear() { map.Clear(); }
	
	/*
	* Method: TMap::
	* Purpose: Retrieves the number of entries added to the TMap
	* Parameters: void
	* Returns: int - the count of entries in the TMap
	*/
	int count() { return map.Count(); }

	/*
	* Method: TMap::operator=
	* Purpose: copies an existing TMap into this one
	* Parameters: TMap<t2>& cop - TMap to copy from
	* Returns: void
	*/
	void operator=(TMap<t2>& cop)
	{
		for (int c = 0; c < cop.map.Count(); c++)
		{
			map.Add(cop.map.ElementAt(c));
		}
	}

	/*
	* Method: TMap::GetAnaGameType
	* Purpose: Retrieves the AnaGame representation of the TMap type
	* Parameters: void
	* Returns: UCHAR* - the array representing the TMap type
	*/
	virtual UCHAR* GetAnaGameType()
	{
		return nullptr;
	}

private:
	TArray<tEntry<t2>> map;
};

