#include "TParentHolder.h"
#include <TString.h>

/**
 * Method: TParentHolder::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TParentHolder::GetType()
{
	return TString(L"TParentHolder;") + TObject::GetType();
}

/**
 * Method: TParentHolder::TParentHolder
 * Purpose: Constructor
 * Parameters: void
 * Returns: New TParentHolder
 */
TParentHolder::TParentHolder()
{
}


/**
 * Method: TParentHolder::GetParent
 * Purpose: Allows the Retrieval of the Parent Control (if the holder is holding a control)
 * Parameters: void
 * Returns: TrecPointer<TControl> - the Parent (the default returns null but the TControlParentHolder will return the parent)
 */
TrecPointer<TControl> TParentHolder::GetParent()
{
	return TrecPointer <TControl>();
}

/**
 * Method: TParentHolder::IsScroller
 * Purpose: Reports to the Child whether the parent holding it is a Scroller Control
 * Parameters: void
 * Returns: bool - whether or not the parent is a Scroller Control (default is false, but the Holder that holds a control can determine otherwise)
 */
bool TParentHolder::IsScroller()
{
	return false;
}
