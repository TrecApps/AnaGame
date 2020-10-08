#pragma once
#include "TString.h"
#include "TMapBase.h"
/*
* Class: TDataEntry
* Purpose: Holds the entry to the TDataMap
*/
template<class t2>  class _TREC_LIB_DLL TDataEntry : TObject
{
public:
	/**
	 * Method:: TDataEntry::TDataEntry
	 * Purpose: Constructor
	 * Parameters: const TString& str -  the key
	 *				const t2& obj - the value
	 * Returns: new TEntry
	 */
	TDataEntry(const TString& str, const t2& obj): object(obj)
	{
		key.Set(str);
	}

	/**
	 * Method:: TDataEntry::TDataEntry
	 * Purpose: Copy Constructor
	 * Parameters: void
	 * Returns: new TEntry
	 */
	TDataEntry(const TDataEntry& obj) : object(obj.object)
	{
		key.Set(obj.key);
	}


	/**
	 * Method:: TDataEntry::TDataEntry
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new TEntry
	 */
	TDataEntry()
	{

	}

	/**
	 * The object to hold
	 */
	t2 object;
	/**
	 * the key to the object
	 */
	TString key;
};

/**
 * Class: TDataMap
 * Purpose: Holds a map to data directly
 */
template<class T> class _TREC_LIB_DLL TDataMap : TMapBase
{
	friend class TDataMap;
private:
	TDataArray<TDataEntry<T>> fields;

public:
	/*
	 * Method: TMap::TMap
	 * Purpose: Default Constructor
	 * Parameters:
	 * Returns:
	 */
	TDataMap() {}

	/*
	 * Method: TMap::~TMap
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TDataMap() { }

	/*
	 * Method: TMap::addEntry
	 * Purpose: Adds an entry to the TMap
	 * Parameters: TString& str - the key
	 *				const T& obj - the object itself
	 * Returns: void
	 */
	void addEntry(const TString& str, const T& obj)
	{
		T newObj(obj);
		TDataEntry<T> entry(str, newObj);
		
		fields.push_back(entry);
	}


	/*
	 * Method: TMap::setEntry
	 * Purpose: Adds an entry to the TMap
	 * Parameters: TString& str - the key
	 *				const T& obj - the object itself
	 * Returns: void
	 */
	void setEntry(const TString& str, const T& obj)
	{
		UINT target;
		bool found = false;
		for (target = 0; target < fields.Size(); target++)
		{
			if (str.Compare(fields[target].key))
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			fields[target].object = obj;
		}
		else
		{
			addEntry(str, obj);
		}
	}

	/*
	 * Method: TMap::GetEntryAt
	 * Purpose: Retrieves an entry at a specific index
	 * Parameters: UINT c - the index to target
	 *				TDataEntry<T> entry - holds the data if found
	 * Returns: bool - whether the entry was valid
	 */
	bool GetEntryAt(UINT c, TDataEntry<T>& entry)
	{
		if (c < fields.Size())
		{
			entry.key.Set(fields[c].key);
			entry.object = fields[c].object;
			return true;
		}
		return false;
	}

	/*
	* Method: TMap::retrieveEntry
	* Purpose: Retrieves an entry based on a given key
	* Parameters: TString& str - the key
	*				T& entry - the object being held
	* Returns: bool - whether the entry was found
	*/
	bool retrieveEntry(const TString& str, T& entry)
	{
		for (int c = 0; c < fields.Size(); c++)
		{
			if (!str.Compare(fields.at(c).key))
			{
				entry = fields[c].object;
				return true;
			}
		}
		return false;
	}

	/*
	* Method: TMap::retrieveEntry
	* Purpose: Retrieves an entry based on a given key
	* Parameters: TString& str - the key
	*				T& entry - the object being held
	*				int occurance - the number of successes to skip
	* Returns: bool - whether the entry was found
	*/
	bool retrieveEntry(const TString& str, T& entry, int occurance)
	{
		int occ = 0;
		for (int c = 0; c < fields.Size(); c++)
		{
			if (!str.Compare(fields.at(c).key))
			{
				if (occ == occurance)
				{
					entry = fields[c].object;
					return true;
				}
				else
					occ++;
			}
		}
		return false;
	}

	/*
	* Method: TMap::removeEntry
	* Purpose: Removes an entry from the map
	* Parameters: TString& str - the key
	*				T& entry - the object being held
	* Returns: TrecPointer<t2> - the entry found
	*/
	bool removeEntry(TString& str, T& entry)
	{
		for (int c = 0; c < fields.Size(); c++)
		{
			if (!str.Compare(fields.at(c).key))
			{
				entry = fields[c].object;
				fields.RemoveAt(c);
				return true;
			}
		}
		return false;
	}

	/*
	* Method: TMap::clear
	* Purpose: clears the map of all entries
	* Parameters: void
	* Returns: void
	*/
	void clear() { fields.RemoveAll(); }

	/*
	* Method: TMap::
	* Purpose: Retrieves the number of entries added to the TMap
	* Parameters: void
	* Returns: int - the count of entries in the TMap
	*/
	int count() { return fields.Size(); }

	/*
	* Method: TMap::operator=
	* Purpose: copies an existing TMap into this one
	* Parameters: TMap<t2>& cop - TMap to copy from
	* Returns: void
	*/
	void operator=(TDataMap<T>& cop)
	{
		for (int c = 0; c < cop.fields.Size(); c++)
		{
			fields.push_back(cop.fields.at(c));
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
};