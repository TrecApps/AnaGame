#pragma once
#include "TObject.h"

/**
 * class TArrayBase
 * Purpose: holds the bare essentials for all TArray's
 */
class _TREC_LIB_DLL TArrayBase :
	public TObject
{
public:

	/**
	 * Method: TArrayBase::TArrayBase
	 * Purpose: Creates the TArray
	 * Parameters: void
	 * Returns: THe TArrayBase Object
	 */
	TArrayBase();

	/**
     * Method: TArrayBase::~TArrayBase
     * Purpose: Destroys the object
     * Parameters: void
     * Returns: void
     */
	~TArrayBase();

	/**
     * Method: TArrayBase::GetAnaGameType
     * Purpose: Reports the anagame type
     * Parameters: void
     * Returns: UCHAR* array expressing the type of this object
     *
     * Note: DEPRICATED - should not be used in Anagame development
     */
	virtual UCHAR* GetAnaGameType()override;

	/**
     * Method: TArrayBase::Count
     * Purpose: Reports the current count of the array
     * Parameters: void
     * Returns: UINT - the number of elements currently held
     */
	UINT Count();

	/**
     * Method: TArrayBase::GetObjectAt
     * Purpose: Returns the TObject in TObject form (if it is a TObject
     * Parameters: UINT loc - the target element
     * Returns: The TObject pointer, if the type held is a TObject and the index is within bounds
     *
     * Note: Abstract in case the type is not a TObject
     */
	virtual TObject* GetObjectAt(UINT loc) = 0;

protected:
    /**
     * elements currently held
     */
	UINT count = 0;

    /**
     * the level of the TArray (since they are partially linked lists
     */
	BYTE level = 0;
};

