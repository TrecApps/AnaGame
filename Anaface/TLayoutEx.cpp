
#include "TLayoutEx.h"


/*
 * Method: TLayoutEx::TLayoutEx
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> rt -  the Render target to draw to (TControl handles it)
*				TrecPointer<TArray<styleTable>> ta - the Class Style list (TControl handles this)
 * Returns: New TLayoutEx Object
 */
TLayoutEx::TLayoutEx(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> st):TLayout(rt,st)
{
}


/*
 * Method: TLayoutEx::~TLayoutEx
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TLayoutEx::~TLayoutEx()
{
}

/*
 * Method: TLayoutEx::RemoveChildWithPrejudice
 * Purpose: Performs Delete on a child control
 * Parameters: UINT x - the column of the control
 *				UINT y - the row of the control
 * Returns: int - 0
 */
int TLayoutEx::RemoveChildWithPrejudice(UINT x, UINT y)
{
	for (UINT c = 0; c < lChildren.Count(); c++)
	{
		if (lChildren.ElementAt(c)->x == x && lChildren.ElementAt(c)->y == y)
			lChildren.ElementAt(c)->contain.Delete();
	}
	return 0;
}

/*
 * Method: TLayoutEx::RemoveColumn
 * Purpose: Removes the specified column from the control
 * Parameters: UINT c - index of the column to remove
 * Returns: int - error code (0 if successful)
 */
int TLayoutEx::RemoveColumn(UINT c)
{
	if(c >= columnLines.Size())
		return 1;

	int shift = columnLines[c];

	for (UINT C = c + 1; C < columnLines.Size(); C++)
	{
		columnLines[C - 1] = columnLines[C];
	}
	columnLines.RemoveAt(columnLines.Size() - 1);

	TDataArray<UINT> removeEl;

	for (UINT C = 0; C < lChildren.Count(); C++)
	{
		if (lChildren.ElementAt(C)->x == c)
		{
			lChildren.ElementAt(C)->contain.Delete();
			removeEl.push_back(C);
		}
		else if (lChildren.ElementAt(C)->x > c)
		{
			lChildren.ElementAt(C)->x--;
			lChildren.ElementAt(C)->contain->ShiftHorizontal(-shift);
		}
	}

	for (int C = 0; C < removeEl.Size(); C++)
	{
		lChildren.RemoveAt(C);
	}
	return 0;
}

/*
 * Method: TLayoutEx::RemoveRow
 * Purpose: Removes the specified row from the control
 * Parameters: UINT c - index of the row to remove
 * Returns: int - error code (0 if successful)
 */
int TLayoutEx::RemoveRow(UINT c)
{
	if (c >= rowLines.Size())
		return 1;

	int shift = rowLines[c];

	for (UINT C = c + 1; C < rowLines.Size(); C++)
	{
		rowLines[C - 1] = rowLines[C];
	}
	rowLines.RemoveAt(rowLines.Size() - 1);

	TDataArray<UINT> removeEl;

	for (UINT C = 0; C < lChildren.Count(); C++)
	{
		if (lChildren.ElementAt(C)->y == c)
		{
			lChildren.ElementAt(C)->contain.Delete();
			removeEl.push_back(C);
		}
		else if (lChildren.ElementAt(C)->y > c)
		{
			lChildren.ElementAt(C)->y--;
			lChildren.ElementAt(C)->contain->ShiftVertical(-shift);
		}
	}

	for (int C = 0; C < removeEl.Size(); C++)
	{
		lChildren.RemoveAt(C);
	}
	return 0;
}

/*
 * Method: TLayoutEx::AddRow
 * Purpose: Appends a Row to the Layout
 * Parameters: UINT size - the height of the row to add
 * Returns: UINT - 0 if falied, otherwise the number of rows currently in the control
 */
UINT TLayoutEx::AddRow(UINT size)
{
	// rowLines.push_back(size);
	if (addRow(size,false))
		return rowLines.Size();
	return 0;
}

/*
 * Method: TLayoutEx::AddCol
 * Purpose: Appends a Column to the Layout
 * Parameters: UINT size - the width of the column to add
 * Returns: UINT - 0 if falied, otherwise the number of collumns currently in the control
 */
UINT TLayoutEx::AddCol(UINT size)
{
	if(addColunm(size, false))
		return columnLines.Size();
	return 0;
}

/*
 * Method: TLayoutEx::RemoveChild
 * Purpose: Removes a certain control from the layout
 * Parameters: UINT x - the column of the control
 *				UINT y - the row of the control
 * Returns: TrecPointer<TControl> - the control that has been removed
 */
TrecPointer<TControl> TLayoutEx::RemoveChild(UINT x, UINT y)
{
	TrecPointer<TControl> ret;
	for (UINT c = 0; c < lChildren.Count(); c++)
	{
		if (lChildren.ElementAt(c)->x == x && lChildren.ElementAt(c)->y == y)
		{
			ret = lChildren.ElementAt(c)->contain;
			lChildren.RemoveAt(c);
		}
	}
	return ret;
}

/*
 * Method: TLayoutEx::onCreate
 * Purpose: Sets up the attributes of the TLayout by processing the attributes and playing it out
 * Parameters: RECT margin - the location the layout has to work with
 * Returns: bool - success
 */
bool TLayoutEx::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	return TLayout::onCreate(r,d3d);
}

/*
 * Method: TLayoutEx::onDraw
 * Purpose: Draws out the Layout Generated
 * Parameters: void
 * Returns: void
 */
void TLayoutEx::onDraw(TObject* obj)
{
	TLayout::onDraw(obj);
}

UCHAR * TLayoutEx::GetAnaGameType()
{
	return nullptr;
}
