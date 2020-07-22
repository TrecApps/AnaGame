
#include "TArrayBase.h"

UCHAR TArrayType[] = { 1, 0b01000000 };


/**
 * Method: TArrayBase::TArrayBase
 * Purpose: Creates the TArray
 * Parameters: void
 * Returns: THe TArrayBase Object
 */
TArrayBase::TArrayBase()
{
}

/**
 * Method: TArrayBase::~TArrayBase
 * Purpose: Destroys the object
 * Parameters: void
 * Returns: void
 */
TArrayBase::~TArrayBase()
{
}

/**
 * Method: TArrayBase::GetAnaGameType
 * Purpose: Reports the anagame type
 * Parameters: void
 * Returns: UCHAR* array expressing the type of this object
 *
 * Note: DEPRICATED - should not be used in Anagame development
 */
UCHAR * TArrayBase::GetAnaGameType()
{
	return TArrayType;
}

/**
 * Method: TArrayBase::Count
 * Purpose: Reports the current count of the array
 * Parameters: void
 * Returns: UINT - the number of elements currently held
 */
UINT TArrayBase::Count()
{
	return count;
}
