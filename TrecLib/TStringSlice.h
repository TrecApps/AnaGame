#pragma once
#include "TString.h"
#include "TSliceManager.h"

/**
 * Class: TStringSlice
 * Purpose: Holds a reference to the String and focuses on a small section of the string
 * 
 * Note: Managed by a slice manager to modify the slice or invalidate it as any change to the string may call for
 */
class _TREC_LIB_DLL TStringSlice
{
	friend class TStringSliceManager;
public:
	/**
	 * Method: TStringSlice::TStringSlice
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TString> string - the string to manipulate
	 *				TrecPointerSoft<TSliceManager> parent - The manager to update
	 *				UINT start - the index of the first character to focus on
	 *				UINT count - the number of characters in the string to focus on
	 * Returns: new String Slice Object
	 */
	TStringSlice(TrecPointer<TString> string, TrecPointerSoft<TSliceManager> parent, UINT start, UINT count);
	
	/**
	 * Method: TStringSlice::Invalidate
	 * Purpose: deactivates the slice, either because the section of the String has been removed
	 * Parameters: void
	 * Returns: void
	 */
	void Invalidate();

	/**
	 * Method: TStringSlice::GetStringCopy
	 * Purpose: Retrieves the sliced contentes of the underlying string
	 * Parameters: void
	 * Returns: TString - string representing what was in the slice
	 * 
	 * Note: The returned string is a copy of the sliced string. Changes made to the copy have no impact on the source string
	 */
	TString GetStringCopy();
protected:
	/**
	 * the string to manipulate
	 */
	TrecPointer<TString> string;
	/**
	 * The manager to update if any structural changes to the string are made
	 */
	TrecPointerSoft<TSliceManager> sliceManager;
	/**
	 * Tracks the starting character and the number of characters to manage
	 */
	UINT start, count;
	/**
	 * Whether the String is still valid
	 */
	bool valid;
};

/**
 * Class: TStringSliceManager
 * Purpose: Manages changes to the String and Updates all attached slices accordingly
 * 
 * SuperClass: TSliceManager
 */
class _TREC_LIB_DLL TStringSliceManager : public TSliceManager
{
public:
	/**
	 * Method: TStringSliceManager::TStringSliceManager
	 * Purpose: Constructor using an existing TrecPointer
	 * Parameters: TrecPointer<TString> string - string to attach to
	 * Returns: The new String Manager
	 */
	TStringSliceManager(TrecPointer<TString> string);

	/**
	 * Method: TStringSliceManager::TStringSliceManager
	 * Purpose: Constructor that takes in a base string and copies it into a TrecPointer
	 * Parameters: const TString& string - the string to copy into
	 * Returns: The new String Manager
	 */
	TStringSliceManager(const TString& string);

	/**
	 * Method: TStringSliceManager::isValid
	 * Purpose: Whether the string held is valid or not
	 * Parameters: void
	 * Returns: bool - whether the string is active
	 */
	bool isValid();
	/**
	 * Method: TStringSliceManager::operator->
	 * Purpose: Retrieves the raw string
	 * Parameters: void
	 * Returns: TString* - raw pointer to the string
	 */
	TString* operator->();


	/**
	 * Method: TStringSliceManager::Insert
	 * Purpose:  Adjusts the slices in response to an insertion
	 * Parameters: UINT start - the index where the insertion begins
	 *				UINT count - the number of characters are added
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Insert(UINT start, UINT count) override;
	/**
	 * Method: TStringSliceManager::Delete
	 * Purpose: Adjusts the slices in response to a deletion
	 * Parameters: UINT start - the index where the deletion begins
	 *				UINT count - the number of characters removed
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Delete(UINT start, UINT count) override;

	/**
	 * Method: TStringSliceManager::GetSlice
	 * Purpose: Generates a slice to manage
	 * Parameters: UINT start - the index where the slice begins
	 *				UINT count - the number of characters to track
	 * Returns: TrecPointer<TStringSlice> - the slice (null if the slice would go out of bounds)
	 */
	TrecPointer<TStringSlice> GetSlice(UINT start, UINT count);

	/**
	 * Method: TStringSliceManager::GetUnderlyingString
	 * Purpose: allows objects to access the underlying string
	 * Parameters: void
	 * Returns: TrecPointer<TString> - the underlying string
	 */
	TrecPointer<TString> GetUnderlyingString();

protected:
	/**
	 * The string to manage
	 */
	TrecPointer<TString> string;
	/**
	 * number of slices managed by this manager
	 */
	TDataArray<TrecPointer<TStringSlice>> slices;
};





