#include "TGradientStopCollection.h"
#include <TString.h>

/**
 * Method: TGradientStopCollection::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TGradientStopCollection::GetType()
{
	return TString(L"TGradientStopCollection;") + TObject::GetType();
}

/**
 * Method: TGradientStopCollection::TGradientStopCollection
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: new empty TGradientStopCollection Object
 */
TGradientStopCollection::TGradientStopCollection()
{
}

/**
 * Method: TGradientStopCollection::TGradientStopCollection
 * Purpose: copy constructor
 * Parameters: const TGradientStopCollection& col - the collection to copy
 * Returns: new TGradientStopCollection Object
 */
TGradientStopCollection::TGradientStopCollection(const TGradientStopCollection& col)
{
	AG_THREAD_LOCK
	for (UINT Rust = 0; col.IsValid(Rust); Rust++)
	{
		gradients.push_back(col.GetGradientStopAt(Rust));
	}
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TGradientStopCollection::AddGradient
 * Purpose: Adds a new gradient to the collection
 * Parameters: const TGradientStop& newGradient - the grdient to add
 * Returns: UINT - the new number of gradients currently held
 */
UINT TGradientStopCollection::AddGradient(const TGradientStop& newGradient)
{
	AG_THREAD_LOCK
	gradients.push_back(newGradient);
	UINT ret = gradients.Size() - 1;
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TGradientStopCollection::GetGradientCount
 * Purpose: Reports the current count of the collected gradients
 * Parameters: void
 * Returns: UINT - the number of gradients currently held
 */
UINT TGradientStopCollection::GetGradientCount() const
{
	AG_THREAD_LOCK
		UINT ret = gradients.Size();
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TGradientStopCollection::IsValid
 * Purpose: Reports whether the desired index is within bounds
 * Parameters: UNIT index - the index of the gradient
 * Returns: bool - whether the specified index is within bounds
 */
bool TGradientStopCollection::IsValid(UINT index) const
{
	AG_THREAD_LOCK
		bool ret = index < gradients.Size();
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TGradientStopCollection::GetGradientStopAt
 * Purpose: Retrieves the Gradient at the specified location
 * Parameters: UINT index - the index of the gradient
 * Returns: TGradientStop - the Gradient requested
 */
TGradientStop TGradientStopCollection::GetGradientStopAt(UINT index)const
{
	AG_THREAD_LOCK
	if (!IsValid(index))
		RETURN_THREAD_UNLOCK TGradientStop();
	auto ret =  gradients.data()[index];
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TGradientStopCollection::GetColorAt
 * Purpose: Gets the color at the specified index
 * Parameters: UINT index - the index of the gradient
 * Returns: TColor - the color requested
 *
 * Note: if the index is out of bounds, Black is returned
 */
TColor TGradientStopCollection::GetColorAt(UINT index)
{
	AG_THREAD_LOCK
	if(!IsValid(index))
		RETURN_THREAD_UNLOCK TColor();
	TColor ret = gradients.data()[index].GetColor();
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TGradientStopCollection::SetGradientAt
 * Purpose: Sets the gradient at the specified location
 * Parameters: const TGradientStop& gradient - the gradient to set
 *				UINT index - the index of the gradient to operate on
 * Returns: bool - whether the operation worked (if the index was in bounds)
 */
bool TGradientStopCollection::SetGradientAt(const TGradientStop& gradient, UINT index)
{
	AG_THREAD_LOCK
	if (!IsValid(index))
		RETURN_THREAD_UNLOCK false;
	gradients[index].SetGradient(gradient.GetGradient());
	RETURN_THREAD_UNLOCK true;
}

/**
 * Method: TGradientStopCollection::SetColorAt
 * Purpose: Sets the color of the specified gradient
 * Parameters: const TColor& color - the color to set the gradient to
 *				UINT index - the index of the gradient to operate on
 * Returns:bool  - whether the operation worked (if the index was in bounds)
 */
bool TGradientStopCollection::SetColorAt(const TColor& color, UINT index)
{
	AG_THREAD_LOCK
	if (!IsValid(index))
		RETURN_THREAD_UNLOCK false;
	gradients[index].SetColor(color);
	RETURN_THREAD_UNLOCK true;
}

/**
 * Method: TGradientStopCollection::SetPositionAt
 * Purpose: Sets the position of the specified gradient
 * Parameters: float position - the position to set
 *				UINT index - the index of the gradient to operate on
 * Returns: bool - whether the operation worked (if the index was in bounds)
 */
bool TGradientStopCollection::SetPositionAt(float position, UINT index)
{
	AG_THREAD_LOCK
	if (!IsValid(index))
		RETURN_THREAD_UNLOCK false;
	gradients[index].SetPosition(position);
	RETURN_THREAD_UNLOCK true;
}

/**
 * Method: TGradientStopCollection::Empty
 * Purpose: Empties the collection
 * Parameters: void
 * Returns: void
 */
void TGradientStopCollection::Empty()
{
	AG_THREAD_LOCK
	gradients.RemoveAll();
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TGradientStopCollection::GetRawCollection
 * Purpose: Returns the collection in a form compatible with Direct2D
 * Parameters: void
 * Returns: TDataArray<GRADIENT_STOP_2D> - the raw gradient collection, compatible with Direct2D
 */
TDataArray<GRADIENT_STOP_2D> TGradientStopCollection::GetRawCollection()const
{
	AG_THREAD_LOCK
	TDataArray<GRADIENT_STOP_2D> ret;
	for (UINT Rust = 0; Rust < gradients.Size(); Rust++)
		ret.push_back(gradients.data()[Rust].GetGradient());
	RETURN_THREAD_UNLOCK ret;
}
