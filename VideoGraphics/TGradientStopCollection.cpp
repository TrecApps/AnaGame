#include "TGradientStopCollection.h"


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
	for (UINT Rust = 0; col.IsValid(Rust); Rust++)
	{
		gradients.push_back(col.GetGradientStopAt(Rust));
	}
}

/**
 * Method: TGradientStopCollection::AddGradient
 * Purpose: Adds a new gradient to the collection
 * Parameters: const TGradientStop& newGradient - the grdient to add
 * Returns: UINT - the new number of gradients currently held
 */
UINT TGradientStopCollection::AddGradient(const TGradientStop& newGradient)
{
	gradients.push_back(newGradient);
	return gradients.Size() - 1;
}

/**
 * Method: TGradientStopCollection::GetGradientCount
 * Purpose: Reports the current count of the collected gradients
 * Parameters: void
 * Returns: UINT - the number of gradients currently held
 */
UINT TGradientStopCollection::GetGradientCount() const
{
	return gradients.Size();
}

/**
 * Method: TGradientStopCollection::IsValid
 * Purpose: Reports whether the desired index is within bounds
 * Parameters: UNIT index - the index of the gradient
 * Returns: bool - whether the specified index is within bounds
 */
bool TGradientStopCollection::IsValid(UINT index) const
{
	return index < gradients.Size();
}

/**
 * Method: TGradientStopCollection::GetGradientStopAt
 * Purpose: Retrieves the Gradient at the specified location
 * Parameters: UINT index - the index of the gradient
 * Returns: TGradientStop - the Gradient requested
 */
TGradientStop TGradientStopCollection::GetGradientStopAt(UINT index)const
{
	if (!IsValid(index))
		return TGradientStop();
	return gradients.data()[index];
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
	if(!IsValid(index))
		return TColor();
	return gradients.data()[index].GetColor();
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
	if (!IsValid(index))
		return false;
	gradients[index].SetGradient(gradient.GetGradient());
	return true;
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
	if (!IsValid(index))
		return false;
	gradients[index].SetColor(color);
	return true;
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
	if (!IsValid(index))
		return false;
	gradients[index].SetPosition(position);
	return true;
}

/**
 * Method: TGradientStopCollection::Empty
 * Purpose: Empties the collection
 * Parameters: void
 * Returns: void
 */
void TGradientStopCollection::Empty()
{
	gradients.RemoveAll();
}

/**
 * Method: TGradientStopCollection::GetRawCollection
 * Purpose: Returns the collection in a form compatible with Direct2D
 * Parameters: void
 * Returns: TDataArray<GRADIENT_STOP_2D> - the raw gradient collection, compatible with Direct2D
 */
TDataArray<GRADIENT_STOP_2D> TGradientStopCollection::GetRawCollection()const
{
	TDataArray<GRADIENT_STOP_2D> ret;
	for (UINT Rust = 0; Rust < gradients.Size(); Rust++)
		ret.push_back(gradients.data()[Rust].GetGradient());
	return ret;
}
