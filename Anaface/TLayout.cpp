
#include "TLayout.h"

/*
 * Method:
 * Purpose:
 * Parameters:
 * Returns:
 */

/*
* Method: TLayout::TLayout
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> rt -  the Render target to draw to (TControl handles it)
*				TrecPointer<TArray<styleTable>> ta - the Class Style list (TControl handles this)
* Returns: New TLayout
*/
TLayout::TLayout(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta):TControl(rt,ta)
{
	organization = orgLayout::VBuff;
	specialFunction = specialLayout::Basic;
	conflictResolute = conflictRes::default_Margin;
	//showExpanded = false;
	colunms = 0;
	rows = 0;
	updateColumn = true;
	updateRow = true;
	main = NULL;

	internalColor = D2D1::ColorF(D2D1::ColorF::Black);
	thickness = 1.0f;
	internalInit = false;
}

/*
* Method: TLayout::~TLayout
* Purpose: Destructor
* Parameters: void
* Returns: void
* Note: Currently, the destructor is more ceremonial than functional
*/
TLayout::~TLayout()
{
}

/*
* Method: TLayout::setLayout
* Purpose:Sets the layout mode of the TLayout
* Parameters: orgLayout ol - the layout mode to use
* Returns: bool - success, true if no children are present, false if a child has been added
* Note: Call this method before adding any Controls to the Layout, otherwise the method call will fail
*/
bool TLayout::setLayout(orgLayout ol)
{
	ThreadLock();
	bool ret = lChildren.Count() > 0;
	if(ret)
		organization = ol;
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::setConflictResolutionMode
* Purpose: Sets the Layouts mechanism for handling layouting conflict amongst its controls
* Parameters: conflictRes cr - the mode for resolving conflict
* Returns: bool - success, true if no children are present, false if a child has been added
* Note: Call this method before adding any Controls to the Layout, otherwise the method call will fail
*/
bool TLayout::setConflictResolutionMode(conflictRes cr)
{
	ThreadLock();
	bool ret = lChildren.Count() > 0;
	if (ret)
		conflictResolute = cr;
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::setSpecialFunction
* Purpose: Sets the Basic mechanism for what this layout will do
* Parameters: specialLayout sl - sets the special mode that the Layout will perform
* Returns: bool - success, true if no children are present, false if a child has been added
* Note: Call this method before adding any Controls to the Layout, otherwise the method call will fail
*/
bool TLayout::setSpecialFunction(specialLayout sl)
{
	ThreadLock();
	bool ret = (specialFunction != specialLayout::Basic || lChildren.Count() > 0);
	if(ret)
		specialFunction = sl;
	ThreadRelease();
	return ret;
}


/*
* Method: TLayout::addColunm
* Purpose: Adds a column to the Layout, as long as it is not a vertical stack
* Parameters: int x - the size of the new column
*				bool markDetected - whether the size is absolut or relative compared to available space
* Returns: whether or not the layout organization was compatible with the method call
*/
bool TLayout::addColunm(int x, bool markDetected)
{
	ThreadLock();
	D2D1_RECT_F tempRect = D2D1_RECT_F{ 0,0,0,0 };

	bool AcceptMark = true;

	switch (organization)
	{
	case orgLayout::HStack:
		AcceptMark = false;
		break;
	case orgLayout::HBuff:
		AcceptMark = true;
		break;
	case orgLayout::HMix:
	case orgLayout::grid:
		AcceptMark = markDetected;
	}

	switch (organization)
	{
	case orgLayout::VBuff:
	case orgLayout::VMix:
	case orgLayout::VStack:
		return false; // these have rows
	default:
		columnLines.push_back(x);
		AddToColFlex(AcceptMark);
		return true;
	}
	return false;
}

/*
* Method: TLayout::addRow
* Purpose: Adds a new row to the Layout, as long as the layout is not a vertical gallery
* Parameters: int y - the size of the new row
*				bool markDetected - whether or not the new size is relative to available space or absolute
* Returns: whether the method call was compatible with the layout mode
*/
bool TLayout::addRow(int y, bool markDetected)
{	
	D2D1_RECT_F tempRect = D2D1_RECT_F{ 0,0,0,0 };

	bool AcceptMark = true;

	switch (organization)
	{
	case orgLayout::VStack:
		AcceptMark = false;
		break;
	case orgLayout::VBuff:
		AcceptMark = true;
		break;
	case orgLayout::VMix:
	case orgLayout::grid:
		AcceptMark = markDetected;
	}
	bool ret = false;
	switch (organization)
	{
	case orgLayout::HBuff:
	case orgLayout::HMix:
	case orgLayout::HStack:
		
		break;    // These go by colunms

	default:
		rowLines.push_back(y);
		AddToRowFlex(AcceptMark);
		ret = true;
	}
	ThreadRelease();
	return false;
}

void TLayout::CompileLayout()
{
	ThreadLock();
	switch (organization)
	{
	case orgLayout::HBuff:
	case orgLayout::HMix:
	case orgLayout::HStack:
		for (UINT x = 0; x < columnLines.Size(); x++)
		{
			tempContC = TrecPointerKey::GetNewTrecPointer<containerControl>();
			tempContC->x = tempContC->x2 = x;
			tempContC->extend = false;
			tempContC->y = tempContC->y2 = 0;
			tempContC->contain = TrecPointerKey::GetNewSelfTrecPointer<TControl>(drawingBoard, styles);

			tempContC->contain->setLocation(getRawSectionLocation(0, x));

			lChildren.Add(tempContC);
		}
		break;
	case orgLayout::VBuff:
	case orgLayout::VMix:
	case orgLayout::VStack:

		for (UINT y = 0; y < rowLines.Size(); y++)
		{
			tempContC = TrecPointerKey::GetNewTrecPointer<containerControl>();
			tempContC->x = tempContC->x2 = 0;
			tempContC->extend = false;
			tempContC->y = tempContC->y2 = y;
			tempContC->contain = TrecPointerKey::GetNewSelfTrecPointer<TControl>(drawingBoard, styles);

			tempContC->contain->setLocation(getRawSectionLocation(y, 0));

			lChildren.Add(tempContC);
		}
		break;
	case orgLayout::grid:

		for (UINT x = 0; x < columnLines.Size(); x++)
		{
			for (UINT y = 0; y < rowLines.Size(); y++)
			{
				tempContC = TrecPointerKey::GetNewTrecPointer<containerControl>();
				tempContC->x = tempContC->x2 = x;
				tempContC->extend = false;
				tempContC->y = tempContC->y2 = y;
				tempContC->contain = TrecPointerKey::GetNewSelfTrecPointer<TControl>(drawingBoard, styles);

				tempContC->contain->setLocation(getRawSectionLocation(y, x));

				lChildren.Add(tempContC);
			}
		}
	}
	ThreadRelease();
}

/*
* Method: TLayout::addChild
* Purpose: Adds a new Child Control to the layout at the specified location
* Parameters: TrecPointer<TControl> tc - the control to add
*				UINT x - the column to target
*				UINT y -  he row to target
* Returns: int - error (0 for no error)
*/
int TLayout::addChild(TrecPointer<TControl> tc, UINT x, UINT y)
{
	if (!tc.Get())
		return 1;
	ThreadLock();
	for (int c = 0; c < lChildren.Count();c++)
	{
		if (lChildren.ElementAt(c)->x == x && lChildren.ElementAt(c)->y == y)
		{
			if (!lChildren.ElementAt(c)->contain.Get())
			{
				ThreadRelease();
				return 3;
			}
			lChildren.ElementAt(c)->extend = false;
			lChildren.ElementAt(c)->contain = tc;
			lChildren.ElementAt(c)->contain->setParent(GetParentReference());
			ThreadRelease();
			return 0;
		}
	}
	ThreadRelease();
	return 2;
}

/*
* Method: TLayout::addChild
* Purpose: Adds a new Child Control to the layout at the specified location, and allows for more cells to be covered
* Parameters: TrecPointer<TControl> tc - the control to add
*				UINT x - the column to target
*				UINT y -  he row to target
*				UINT x_2 - the Second column to extend to
*				UINT y_2 - the Row to extend to
* Returns: int - error (0 for no error)
*/
int TLayout::addChild(TrecPointer<TControl> tc, UINT x, UINT y, UINT x_2, UINT y_2)
{
	if (!tc.Get())
		return 1;
	ThreadLock();
	for (int c = 0; c < lChildren.Count(); c++)
	{
		if (lChildren.ElementAt(c)->x == x && lChildren.ElementAt(c)->y == y)
		{
			if (!lChildren.ElementAt(c)->contain.Get())
			{
				ThreadRelease();
				return 3;
			}
			if (x_2 >= x && (x_2 < columnLines.Size() || !columnLines.Size())
				&& y_2 >= y && (y_2 < rowLines.Size() || !rowLines.Size()))
			{
				lChildren.ElementAt(c)->extend = true;
				lChildren.ElementAt(c)->x2 = x_2;
				lChildren.ElementAt(c)->y2 = y_2;
			}
			else
				lChildren.ElementAt(c)->extend = false;
			lChildren.ElementAt(c)->contain = tc;
			lChildren.ElementAt(c)->contain->setParent(GetParentReference());
			ThreadRelease();
			return 0;
		}
	}
	ThreadRelease();
	return 2;
}

/*
* Method: TLayout::setGrid
* Purpose: Sets up a predetermined Grid
* Parameters: TArray<int>* col - the column layout of the grid
*				TArray<int>* row - the row layout of the grid
* Returns: bool - success (method fails if a control is already added or if the layout is not a grid)
* Note: the columns and rows provided will be given a relative spacing based off of the layouts real size
*	if you intend to set at least one row and/or column to have an absolute size, don't use this method
*/
bool TLayout::setGrid(TDataArray<int>& col, TDataArray<int>& row)
{
	ThreadLock();
	if (organization != orgLayout::grid)
	{
		ThreadRelease();
		return false;
	}
	if (!col.Size() || !row.Size())
	{
		ThreadRelease();
		return false;
	}
	D2D1_RECT_F tempRect;
	tempRect.top = 0;
	tempRect.left = 0;
	tempRect.right = col[0];
	tempRect.bottom = row[0];

	columnLines.push_back(col[0]);
	rowLines.push_back(row[0]);


	tempContC = TrecPointerKey::GetNewTrecPointer<containerControl>();
	tempContC->x = 0;
	tempContC->y = 0;
	tempContC->contain = TrecPointerKey::GetNewSelfTrecPointer<TControl>(drawingBoard, styles);

	tempContC->contain->setLocation(tempRect);

	lChildren.Add(tempContC);

	rows = 1;
	colunms = 1;
	//updateColumn = false;
	//updateRow = false;
	for (int c = 1; c < col.Size();c++)
	{
		addColunm(col[c], true);
	}
	for (int c = 1; c < row.Size();c++)
	{
		addRow(row[c], true);
	}
	ThreadRelease();
	return true;
}

/*
* Method: TLayout::setStack
* Purpose: Sets either the Column layout or the Row Layout, based off of the Layout mode
* Parameters: TArray<int>* nums - the layout of the row/column
* Returns: bool - success (method fails if layout is a grid or if a control has already been added)
*/
bool TLayout::setStack(TDataArray<int>& nums)
{
	ThreadLock();
	if (organization == orgLayout::grid)
	{
		ThreadRelease();
		return false;
	}
	switch (organization)
	{
	case orgLayout::VStack:
		for (int c = 0; c < nums.Size(); c++)
			addRow(nums[c], false);
		break;
	case orgLayout::VBuff:
	case orgLayout::VMix:
		for (int c = 0; c < nums.Size(); c++)
			addRow(nums[c], true);
		break;
	case orgLayout::HStack:
		for (int c = 0; c < nums.Size(); c++)
			addColunm(nums[c], false);
		break;
	case orgLayout::HBuff:
	case orgLayout::HMix:
		for (int c = 0; c < nums.Size();c++)
			addColunm(nums[c], true);
	}
	ThreadRelease();
	return true;
}

/*
* Method: TLayout::loadFromHTML
* Purpose: Allows the TLayout to Extract itself from an HTML File
* Parameters: CArchive* ar - the file to read from
* Returns: int - 0
* Note: DEPRECIATED - HTML reading functionality is to be handled by an HTML parser 
*/
int TLayout::loadFromHTML(TFile * ar)
{
	return 0;
}

/*
* Method: TLayout::storeInTML
* Purpose: Allows the TLayout to save itself in an Anaface TML file
* Parameters: CArchive * ar - the file to read from
*				int childLevel - the generation the Layout is (how many dashes to write
*				bool ov - not used
* Returns: void
*/
void TLayout::storeInTML(TFile * ar, int childLevel,bool ov)
{
	//_Unreferenced_parameter_(ov);
	ThreadLock();
	TString appendable;
	resetAttributeString(&appendable, childLevel + 1);
	switch (organization)
	{
	case orgLayout::grid:
		for (int c = 0; c < columnLines.Size();c++)
		{
			appendable.Append(L"|ColumnWidth:");
			appendable.AppendFormat(L"%d", columnLines[c]);
			_WRITE_THE_STRING
		}
		for (int c = 0;c < rowLines.Size();c++)
		{
			appendable.Append(L"|RowHeight:");
			appendable.AppendFormat(L"%d", rowLines[c]);
			_WRITE_THE_STRING
		}
		break;
	case orgLayout::VStack:
	case orgLayout::VMix:
	case orgLayout::VBuff:
		for (int c = 0;c < rowLines.Size();c++)
		{
			appendable.Append(L"|RowHeight:");
			appendable.AppendFormat(L"%d", rowLines[c]);
			_WRITE_THE_STRING
		}
		break;
	case orgLayout::HBuff:
	case orgLayout::HMix:
	case orgLayout::HStack:
		for (int c = 0; c < columnLines.Size();c++)
		{
			appendable.Append(L"|ColumnWidth:");
			appendable.AppendFormat(L"%d", columnLines[c]);
			_WRITE_THE_STRING
		}
	}

	TControl::storeInTML(ar, childLevel);
	ThreadRelease();
}

/*
* Method: TLayout::storeInHTML
* Purpose: Allows the layout to save itself as an HTML tag
* Parameters: CArchive * ar - the file to write to
* Returns: void
*/
void TLayout::storeInHTML(TFile * ar)
{
}

/*
* Method: TLayout::onCreate
* Purpose: Sets up the attributes of the TLayout by processing the attributes and playing it out
* Parameters: RECT margin - the location the layout has to work with
* Returns: bool - success
*/
bool TLayout::onCreate(D2D1_RECT_F margin, TrecPointer<TWindowEngine> d3d)
{
	//TContainer* tempCont = NULL;
	ThreadLock();
	Resize(margin);


	children.Clear();

	for (int c = 0; c < lChildren.Count(); c++)
	{

		TrecPointer<TControl> tempCont = lChildren.ElementAt(c)->contain;
		if (!tempCont.Get())
			continue;
		int x = lChildren.ElementAt(c)->x, y = lChildren.ElementAt(c)->y;
		tempCont->setLocation(getRawSectionLocation(y, x));

		//if (isSnipZero(tempCont->getLocation()))
		//{
		//	tempCont->setLocation(location);
		//}

		lChildren.ElementAt(c)->contain->setLocation(tempCont->getLocation());

		children.Add(tempCont);
		/*if (lChildren.ElementAt(c)->contain->child->isLayout)
		{
			childLayout = lChildren.ElementAt(c)->contain->child;
			childLayout->onCreate(lChildren.ElementAt(c)->contain->location);
		}
		else*/

		//tempCont->onCreate(tempCont->getLocation());

		lChildren.ElementAt(c)->contain->onCreate(tempCont->getLocation(), d3d);

	}

	//	for (int c = 0; c < lChildren.Count();c++)
		//{
		//
		//}

	auto valpoint = attributes.retrieveEntry(TString(L"|InternalBorderColor"));
	if (valpoint.Get())
	{
		internalInit = true;
		internalColor = convertStringToD2DColor(valpoint.Get());
	}

	valpoint = attributes.retrieveEntry(TString(L"|InternalBorderThickness"));
	if (valpoint.Get())
	{
		valpoint->ConvertToFloat(thickness);
	}

	// To-Do: Add support for gradients in future build

	if (internalInit && drawingBoard.Get())
	{
		internalBrush = drawingBoard->GetBrush(internalColor);
	}
	bool ret = TControl::onCreate(margin, d3d);
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::onDraw
* Purpose: Draws out the Layout Generated
* Parameters: void
* Returns: void
*/
void TLayout::onDraw(TObject* obj)
{
	ThreadLock();
	if (!isActive)
	{
		ThreadRelease();
		return;
	}
	TControl::onDraw(obj);

	if (internalBrush.Get())
	{
		for (int c = 0; c < rowLines.Size();c++)
		{
			int add = rowLines[c];
			internalBrush->DrawLine(D2D1::Point2F(location.left, location.top + add),
				D2D1::Point2F(location.right, location.top + add), thickness);
		}
		for (int c = 0; c < columnLines.Size();c++)
		{
			int add = columnLines[c];
			internalBrush->DrawLine(D2D1::Point2F(location.left + add, location.top),
				D2D1::Point2F(location.left + add, location.bottom), thickness);
		}
	}
	ThreadRelease();
}

/*
* Method: TLayout::returnMinX
* Purpose: Returns the left-x coordinate of the right most column depending on whether a new column is expected
* Parameters: bool newColumn - whether a new column is expected
* Returns: int - the minimum x-coordinate the right most column available
*/
int TLayout::returnMinX(bool newColumn)
{
	ThreadLock();
	if (colunms == 0)
	{
		ThreadRelease();
		return 0;
	}
	for (int c = 0; c < lChildren.Count(); c++)
	{
		bool found = false;
		int ret = 0;
		if (newColumn && lChildren.ElementAt(c)->x == colunms - 1)
		{
			found = true;
			ret = lChildren.ElementAt(c)->contain->getLocation().right;
		}
		else if (lChildren.ElementAt(c)->x == colunms)
		{
			found = true;
			ret = lChildren.ElementAt(c)->contain->getLocation().right;
		}
		if (found)
		{
			ThreadRelease();
			return ret;
		}
	}
	ThreadRelease();
	return 0;
}

/*
* Method: TLayout::returnMinY
* Purpose: Returns the bottom-y coordinate of the bottom row depending on whether a new row is expected
* Parameters: bool newRow - whether a new row is expected
* Returns: int - the top coordinate of the bottom row whether it is the current bottom or a new row
*/
int TLayout::returnMinY(bool newRow)
{
	ThreadLock();
	if (rows == 0)
	{
		ThreadRelease();
		return 0;
	}
	for (int c = 0; c < lChildren.Count(); c++)
	{
		bool found = false;
		int ret = 0;
		if (newRow && lChildren.ElementAt(c)->y == rows - 1)
		{
			found = true;
			ret = lChildren.ElementAt(c)->contain->getLocation().bottom;
		}
		else if (lChildren.ElementAt(c)->y == rows)
		{
			found = true;
			ret = lChildren.ElementAt(c)->contain->getLocation().bottom;
		}
		if (found)
		{
			ThreadRelease();
			return ret;
		}
	}
	ThreadRelease();
	return 0;
}

/*
* Method: TLayout::returnRectX
* Purpose: Returns the location of the given of the column (and first row)
* Parameters: int x - the column to look at
* Returns: D2D1_RECT_F - the location of that area
*/
D2D1_RECT_F TLayout::returnRectX(int x)
{
	ThreadLock();
	auto ret =  D2D1_RECT_F();
	for (int c = 0; c < lChildren.Count(); c++)
	{
		if (lChildren.ElementAt(c)->x == x)
		{
			ret = lChildren.ElementAt(c)->contain->getLocation();
			break;
		}
	}
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::returnRectY
* Purpose: Returns the location of the given of the row (and first column)
* Parameters: int y - the row to look st
* Returns: D2D1_RECT_F the location of the area
*/
D2D1_RECT_F TLayout::returnRectY(int y)
{
	ThreadLock();
	auto ret = D2D1_RECT_F();
	for (int c = 0; c < lChildren.Count(); c++)
	{
		if (lChildren.ElementAt(c)->y == y)
		{
			ret = lChildren.ElementAt(c)->contain->getLocation();
			break;
		}
	}
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::GetColumnFlexAt
* Purpose: Determines whether a given column is flexible and able to change size
* Parameters: UINT col - the column to check
* Returns: bool - whether the column is flexible
*/
bool TLayout::GetColumnFlexAt(UINT col)
{
	ThreadLock();
	bool ret = (col >= columnLines.Size() || col / 8 >= columnFlex.Size());
	if (ret)
	{
		UCHAR comp = 0b10000000 >> (col % 8);
		ret = columnFlex[col / 8] & comp;
	}
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::GetRowFlexAt
* Purpose: Determines whether a given row is flexible and able to change size
* Parameters: UINT row - the row to check
* Returns: bool whether the row is flexible
*/
bool TLayout::GetRowFlexAt(UINT row)
{
	ThreadLock();
	bool ret = (row >= rowLines.Size() || row / 8 >= rowFlex.Size());
	if (ret)
	{
		UCHAR comp = 0b10000000 >> (row % 8);
		ret = rowFlex[row / 8] & comp;
	}
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::AddToColFlex
* Purpose: Adds a new column to the column flexibility array
* Parameters: bool val - whether the new Column is to be flexible or not
* Returns: void
*/
void TLayout::AddToColFlex(bool val)
{
	ThreadLock();
	UINT el = columnLines.Size();
	if (el / 8 >= columnFlex.Size())
		columnFlex.push_back(static_cast<UCHAR>(0));

	UCHAR setter = 0b10000000 >> ((el-1) % 8);
	if (val)
	{
		columnFlex[el / 8] = columnFlex[el / 8] | setter;
	}
	ThreadRelease();
}

/*
* Method: TLayout::AddToRowFlex
* Purpose: Adds a new row to the row flexibility array
* Parameters: bool var - whether the new row is to be flexible or not
* Returns: void 
*/
void TLayout::AddToRowFlex(bool val)
{
	ThreadLock();
	UINT el = rowLines.Size();
	if (el / 8 >= rowFlex.Size())
		rowFlex.push_back(static_cast<UCHAR>(0));

	UCHAR setter = 0b10000000 >> ((el-1) % 8);
	if (val)
	{
		rowFlex[el / 8] = rowFlex[el / 8] | setter;
	}
	ThreadRelease();
}

/*
* Method: TLayout::GetTotalFlexRow
* Purpose: Retrieves the total height of all rows marked as flexible
* Parameters: void
* Returns: UINT - current height of all flexible rows
*/
UINT TLayout::GetTotalFlexRow()
{
	ThreadLock();
	UINT ret = 0;
	if (!rowFlex.Size())
	{
		ThreadRelease();
		return ret;
	}
	for (UINT c = 0; c < rowLines.Size(); c++)
	{
		if (GetRowFlexAt(c))
			ret += rowLines[c];
	}
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::GetTotalSetRow
* Purpose: Retrieves the total height of all rows marked as static
* Parameters: void
* Returns: UINT - current height of all fixed sized rows
*/
UINT TLayout::GetTotalSetRow()
{
	ThreadLock();
	if (!rowFlex.Size())
	{
		UINT ret = location.bottom - location.top;
		ThreadRelease();
		return ret;
	}
	UINT ret = 0;
	for (UINT c = 0; c < rowLines.Size(); c++)
	{
		if (!GetRowFlexAt(c))
			ret += rowLines[c];
	}
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::GetTotalFlexCol
* Purpose: Retrieves the total width of all columns marked as flexible
* Parameters: void 
* Returns: UINT - current width of all flexible columns
*/
UINT TLayout::GetTotalFlexCol()
{
	UINT ret = 0;
	ThreadLock();
	if (!columnFlex.Size())
	{
		ThreadRelease();
		return ret;
	}
	for (UINT c = 0; c < columnLines.Size(); c++)
	{
		if (GetColumnFlexAt(c))
			ret += columnLines[c];
	}
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::GetTotalSetCol
* Purpose: Retrieves the total width of all columns marked as static
* Parameters: void
* Returns: UINT - current width of all fixed sized columns
*/
UINT TLayout::GetTotalSetCol()
{
	ThreadLock();
	if (!columnFlex.Size())
	{
		UINT ret = location.right - location.left;
		ThreadRelease();
		return ret;
	}
	UINT ret = 0;
	for (UINT c = 0; c < columnLines.Size(); c++)
	{
		if (!GetColumnFlexAt(c))
			ret += columnLines[c];
	}
	ThreadRelease();
	return ret;
}


/*
 * Method: TLayout::Resize
 * Purpose: resets the Size of the TLayout
 * Parameters: D2D1_RECT_F& r - the location to set the TLayout to
 * Returns: void
 */
void TLayout::Resize(D2D1_RECT_F& r)
{
	ThreadLock();
	TControl::Resize(r);

	int rWidth = (r.right - margin.right) - (r.left + margin.left);
	int flexrWidth = rWidth - GetTotalSetCol();
	int rHeight = (r.bottom - margin.bottom) - (r.top + margin.top);
	int flexrHeight = rHeight - GetTotalSetRow();
	int currentWidth = GetTotalFlexCol();
	int currentHeight = GetTotalFlexRow();

	int newValue = 0;
	if (!hScroll.Get() && flexrWidth < 0)
	{
		hScroll = TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this, ScrollOrient::so_horizontal);
	}
	else if (!hScroll.Get() || (organization != orgLayout::HStack)) // If Layout has a horizontal scroll bar, then no need to resize the columns
	{


		for (int c = 0; c < columnLines.Size(); c++)
		{
			if (GetColumnFlexAt(c))
			{
				newValue = static_cast<int>(static_cast<float>(columnLines[c]) * static_cast<float>(static_cast<float>(flexrWidth) / static_cast<float>(currentWidth)));
				columnLines[c] = newValue;
			}
		}
	}

	if (!vScroll.Get() && flexrHeight < 0)
	{
		vScroll = TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this, ScrollOrient::so_vertical);
	}
	if (!vScroll.Get() || (organization != orgLayout::VStack)) // Ditto with a vertical Scroll bar and rows
	{
		for (int c = 0; c < rowLines.Size(); c++)
		{
			if (GetRowFlexAt(c))
			{
				newValue = static_cast<int>(static_cast<float>(rowLines[c]) * static_cast<float>(static_cast<float>(flexrHeight) / static_cast<float>(currentHeight)));
				rowLines[c] = newValue;
			}
		}
	}

	// To-Do, adjust TLayout's handling of location in respect to scroll Bars
	location.bottom = r.bottom - margin.bottom;
	location.left = r.left + margin.left;
	location.right = r.right - margin.right;
	location.top = r.top + margin.top;

	if (vScroll.Get())
	{
		if (rowLines.Size())
		{
			location.bottom = location.top;
			for (int c = 0; c < rowLines.Size(); c++)
				location.bottom += rowLines[c];
		}
	}

	if (hScroll.Get())
	{
		if (columnLines.Size())
		{
			location.right = location.left;
			for (int c = 0; c < columnLines.Size(); c++)
				location.right += columnLines[c];
		}
	}



	for (UINT Rust = 0; Rust < lChildren.Count(); Rust++)
	{
		auto lChild = lChildren.ElementAt(Rust);
		if (!lChild.Get())
			continue;

		auto child = lChild->contain;
		if (!child.Get())
			continue;
		auto loc = getRawSectionLocation(lChild->y, lChild->x);
		child->Resize(loc);
	}
	ThreadRelease();
}


/*
* Method: TLayout::returnColumnsWidth
* Purpose: Retrieves the Total Current width of all columns up to a certain column
* Parameters: int x - the column to stop at
* Returns: int - The total witdh (-1 if Layout uses only rows)
*/
int TLayout::returnColumnsWidth(int x)
{
	ThreadLock();
	if (x > columnLines.Size() || x < 0)
	{
		ThreadRelease();
		return -1;
	}
	int returnable = 0;
	for (int c = 1; c < x+1; c++)
	{
		returnable = returnable + columnLines[c-1];
	}
	ThreadRelease();
	return returnable;
}

/*
* Method: TLayout::returnRowsHeight
* Purpose: Retrieves the total current height of all rows down to a certain row
* Parameters: int y - the row to stop at
* Returns: int - the total height (-1 if Layout uses only columns)
*/
int TLayout::returnRowsHeight(int y)
{
	ThreadLock();
	if (y > rowLines.Size() || y < 0)
	{
		ThreadRelease();
		return -1;
	}
	int returnable = 0;
	for (int c = 1; c < y + 1; c++)
	{
		returnable = returnable + rowLines[c-1];
	}
	ThreadRelease();
	return returnable;
}

/*
* Method: TLayout::getColunmWidth
* Purpose: Retrieves the Width of a specific column
* Parameters: int x - the column to inspect
* Returns: int - the Size of the column (Zero if column does not exist)
*/
int TLayout::getColunmWidth(int x)
{
	ThreadLock();
	int ret = (x >= columnLines.Size() || x < 0) ? 0 : columnLines[x];
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::getRowHeight
* Purpose: Retrieves the height of a specific row
* Parameters: int y - the row to inspect
* Returns: int - the Size of the Row (0 if row does not exist)
*/
int TLayout::getRowHeight(int y)
{
	ThreadLock();
	int ret = (y >= rowLines.Size() || y < 0) ? 0 : rowLines[y];
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::determineMinHeightNeeded
* Purpose: Reports the minimum height needed by the Layout and the child controls
* Parameters: void
* Returns: UINT - Returns the minimum height needed by the Layout, not the current height
*/
UINT TLayout::determineMinHeightNeeded()
{	
	UINT newHeight2 = 0;
	UINT newHeight = 0;
	UINT maxNeeded = 0;
	ThreadLock();
	switch (organization)
	{
	case orgLayout::VStack:
	case orgLayout::VBuff:
	case orgLayout::VMix:
		for (UINT c = 0; c < lChildren.Count() && lChildren.ElementAt(c)->y < rowLines.Size(); c++)
		{
			if (lChildren.ElementAt(c)->contain.Get())
			{

				UINT minSize = lChildren.ElementAt(c)->contain->determineMinHeightNeeded();
				if (rowLines[lChildren.ElementAt(c)->y] > minSize)
					rowLines[lChildren.ElementAt(c)->y] = minSize;
			}
		}
		for (UINT c = 0; c < rowLines.Size(); c++)
			newHeight += rowLines[c];
		location.bottom = location.top + newHeight;
		break;
	case orgLayout::HStack:
	case orgLayout::HBuff:
	case orgLayout::HMix:
		
		for (UINT c = 0; c < lChildren.Count() && lChildren.ElementAt(c)->x < columnLines.Size(); c++)
		{
			if (lChildren.ElementAt(c)->contain.Get())
			{
				UINT minSize = lChildren.ElementAt(c)->contain->determineMinHeightNeeded();
				if (minSize > maxNeeded)
					maxNeeded = minSize;
			}
		}
		location.bottom = location.top + maxNeeded;
		break;
	case orgLayout::grid: // To-Do
		TDataArray<UINT> rowHieghts;
		for (UINT c = 0; c < rowLines.Size(); c++)
			rowHieghts.push_back(0);
		
		for (UINT c = 0; c < lChildren.Count(); c++)
		{
			if (!lChildren.ElementAt(c).Get())
				continue;
			if (!lChildren.ElementAt(c)->contain.Get())
				continue;
			UINT height = lChildren.ElementAt(c)->contain->determineMinHeightNeeded();
			if (height > rowHieghts[lChildren.ElementAt(c)->y])
				rowHieghts[lChildren.ElementAt(c)->y] = height;
		}
		for (UINT c = 0; c < rowLines.Size() && c < rowHieghts.Size(); c++)
			rowLines[c] = rowHieghts[c];
		
		for (UINT c = 0; c < rowLines.Size(); c++)
			newHeight2 += rowLines[c];
		location.bottom = location.top + newHeight2;
	}
	UINT ret = location.bottom - location.top;
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::SetNewLocation
* Purpose: Sets the new location of the Layout and adjusts the child controls accordingly
* Parameters: RECT& r - the new location to occupy
* Returns: void
*/
void TLayout::SetNewLocation(const D2D1_RECT_F& r)
{
	ThreadLock();
	TControl::SetNewLocation(r);

	for (UINT c = 0; c < lChildren.Count(); c++)
	{
		containerControl* cc = lChildren.ElementAt(c).Get();
		if (!cc || !cc->contain.Get())
			continue;
		//cc->contain->location = r;
		cc->contain->SetNewLocation(getRawSectionLocation(cc->y, cc->x));
	}
	ThreadRelease();
}

/*
* Method: TLayout::ShrinkHeight
* Purpose: Shrinks the Layout to a minimum (Used by Web-tours to make web-page look closer to real web-pages)
* Parameters: void
* Returns: void
*/
void TLayout::ShrinkHeight()
{
	ThreadLock();
	determineMinHeightNeeded();
	SetNewLocation(location);
	ThreadRelease();
}

/*
* Method: TLayout::setNewColunmSize
* Purpose: Sets the size of a given column
* Parameters: int xLoc - the Column to modify
*				int x - the size to set that column to
* Returns: void
*/
void TLayout::setNewColunmSize(int xLoc, int x)
{
	ThreadLock();
	if (xLoc >= columnLines.Size() || xLoc < 0)
	{
		ThreadRelease();
		return;
	}
	int currentSize = columnLines[xLoc];
	columnLines[xLoc] = x;

	if (dimensions && dimensions->width)
	{
		int difference = abs(x - currentSize);
		if (currentSize > x)
			dimensions->width -= difference;
		else
			dimensions->width += difference;
	}
	ThreadRelease();
}

/*
* Method: TLayout::setNewRowSize
* Purpose: Sets the size of a given row
* Parameters: int yLoc - the row to modify
*				int y - the size to set that column to
* Returns: void
*/
void TLayout::setNewRowSize(int yLoc, int y)
{
	ThreadLock();
	if (yLoc >= rowLines.Size() || yLoc < 0)
	{
		ThreadRelease();
		return;
	}
	int currentSize = rowLines[yLoc];
	rowLines[yLoc] = y;

	if (dimensions && dimensions->height)
	{
		int difference = abs(y - currentSize);
		if (currentSize > y)
			dimensions->height -= difference;
		else
			dimensions->height += difference;
	}
	ThreadRelease();
}

/*
* Method: TLayout::getRowNumber
* Purpose: Retireves the number of rows present
* Parameters: void
* Returns: int - the number of rows
*/
int TLayout::getRowNumber()
{
	ThreadLock();
	int ret = rowLines.Size();
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::getColumnNumber
* Purpose: Retrieves the number of Columns present
* Parameters: void
* Returns: int - the number of columns
*/
int TLayout::getColumnNumber()
{
	ThreadLock();
	int ret = columnLines.Size();
	ThreadRelease();
	return ret;
}

/*
* Method: TLayout::getRawSectionLocation
* Purpose: Gets the Location of the section specified by the coordinates and permitted to any control occupied there
* Parameters: int r - the row to look at
*				int c - the column to look at
* Returns: D2D1_RECT_F - the location allocated to the specified location
*/
D2D1_RECT_F TLayout::getRawSectionLocation(int r, int c)
{
	D2D1_RECT_F returnable{ 0,0,0,0 };
	ThreadLock();
	if (r > rowLines.Size() || c > columnLines.Size())
	{
		ThreadRelease();
		return returnable;
	}
	returnable.top = location.top;
	returnable.left = location.left;
	returnable.bottom = returnable.top;
	returnable.right = returnable.left;

	TrecPointer<containerControl> contCont;
	for (UINT C = 0; C < lChildren.Count(); C++)
	{
		TrecPointer<containerControl> contContTemp = lChildren.ElementAt(C);
		if (!contContTemp.Get())
			continue;
		if (contContTemp->x == c && contContTemp->y == r)
		{
			contCont = contContTemp;
			break;
		}
	}

	if (rowLines.Size() == 0)
	{
		returnable.bottom = location.bottom;
	}
	else
	{
		for (int C = 0; C < r; C++)
		{

			returnable.top += rowLines[C];
		}

		if (rowLines.Size() == r)
			returnable.bottom = location.bottom;
		else
		{
			returnable.bottom = returnable.top + rowLines[r];
			if (contCont.Get() && contCont->extend)
			{
				for(UINT C = contCont->y + 1; C <= contCont->y2 && C < rowLines.Size();C++)
					returnable.bottom += rowLines[C];
			}
		}
	}

	if (columnLines.Size() == 0)
		returnable.right = location.right;
	else
	{
		for (int C = 0; C < c; C++)
		{
			returnable.left += columnLines[C];
		}
		if (columnLines.Size() == c)
			returnable.right = location.right;
		else
		{
			returnable.right = returnable.left + columnLines[c];
			if (contCont.Get() && contCont->extend)
			{
				for (UINT C = c + 1; C <= contCont->x2 && C < columnLines.Size(); C++)
					returnable.right += columnLines[C];
			}
		}
	}
	ThreadRelease();
	return returnable;
}

/*
* Method: TLayout::GetLayoutChild
* Purpose: Retrieves the Child located at the specified coordinates
* Parameters: int x - the x-coordinate of the child control
*				int y - the y-coordinate of the child control
* Returns: TrecPointer<TControl> - the child control held at the coordinates (Null if child is absent or if a parameter is out of bounds)
*/
TrecPointer<TControl> TLayout::GetLayoutChild(int x, int y)
{
	containerControl cc;
	ThreadLock();
	for (int c = 0; c < lChildren.Count(); c++)
	{
		if (!lChildren.ElementAt(c).Get())
			continue;
		cc = *(lChildren.ElementAt(c).Get());
		if (cc.x == x && cc.y == y)
		{
			ThreadRelease();
			return (cc.contain);
		}
	}
	ThreadRelease();
	return  TrecPointer<TControl>();
}

/*
* Method: TLayout::GetOrganization
* Purpose: Retrieves the organization mode of the layout
* Parameters: void
* Returns: orgLayout - the organization the layout is configured to
*/
orgLayout TLayout::GetOrganization()
{
	ThreadLock();
	auto ret = organization;
	ThreadRelease();
	return ret;
}

UCHAR * TLayout::GetAnaGameType()
{
	return nullptr;
}

/*
 * Method: TLayout::SwitchChildControl
 * Purpose: Allows a Child Control to be swapped
 * Parameters: TrecPointerSoft<TControl> curControl - the control making the call
 *				TrecPointer<TControl> newTControl - the Control to replace it with
 * Returns: void
 */
void TLayout::SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl)
{
	ThreadLock();
	for (UINT Rust = 0; Rust < lChildren.Count(); Rust++)
	{
		if (lChildren.ElementAt(Rust).Get() && lChildren.ElementAt(Rust)->contain.Get() == curControl.Get())
		{
			auto lElement = lChildren.ElementAt(Rust);
			lElement->contain = newControl;

			auto rect = getRawSectionLocation(lElement->y, lElement->x);

			//newControl->onCreate(rect, TrecPointer<TWindowEngine>());

			break;
		}
	}
	TControl::SwitchChildControl(curControl, newControl);
	ThreadRelease();
}

/**
 * Method: TLayout::GetParentReference
 * Purpose: Sets up a Parent Holder for this Control (used so that no redundant holder object is created)
 * Parameters: void
 * Returns: TrecPointer<TParentHolder> - the holder object referencing this control as a parent control
 */
TrecPointer<TParentHolder> TLayout::GetParentReference()
{
	ThreadLock();
	if (!thisParent.Get())
	{
		thisParent = TrecPointerKey::GetNewTrecPointerAlt<TParentHolder, TLayoutParentHolder>(TrecPointerKey::GetSoftSubPointerFromSoft<TControl, TLayout>(tThis));
	}
	auto ret = thisParent;
	ThreadRelease();
	return ret;
}

containerControl::containerControl()
{
	x = x2 = y = y2 = 0;
	extend = false;
}

/**
 * Method: containerControl::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString containerControl::GetType()
{
	return TString(L"containerControl;") + TObject::GetType();
}

/**
 * Method: TLayoutParentHolder::TLayoutParentHolder
 * Purpose: Constructor
 * Parameters: TrecSubPointer<TControl, TLayout> parent -  the TControl to serve as a Parent to TControls
 * Returns: New TLayoutParentHolder
 */
TLayoutParentHolder::TLayoutParentHolder(TrecSubPointer<TControl, TLayout> parent)
{
	this->parent = TrecPointerKey::GetSoftSubPointerFromSub<TControl, TLayout>(parent);
}

/**
 * Method: TLayoutParentHolder::TLayoutParentHolder
 * Purpose: Constructor
 * Parameters: TrecSubPointerSoft<TControl, TLayout> parent -  the TControl to serve as a Parent to TControls
 * Returns: New TLayoutParentHolder
 */
TLayoutParentHolder::TLayoutParentHolder(TrecSubPointerSoft<TControl, TLayout> parent)
{
	this->parent = parent;
}

/**
 * Method: TLayoutParentHolder::SwitchChildControl
 * Purpose: Has the Control switch one of its child controls with a new one
 * Parameters: TrecPointerSoft<TControl> cur - the control making the call
 *				TrecPointer<TControl> newTControl - the Control to replace it with
 * Returns: void
 */
void TLayoutParentHolder::SwitchChildControl(TrecPointerSoft<TControl> cur, TrecPointer<TControl> newTControl)
{
	auto pointerParent = TrecPointerKey::GetSubPointerFromSoft<TControl, TLayout>(parent);

	if (pointerParent.Get())
		pointerParent->SwitchChildControl(cur, newTControl);
}

/**
 * Method: TLayoutParentHolder::GetParent
 * Purpose: Allows the Retrieval of the Parent Control (if the holder is holding a control)
 * Parameters: void
 * Returns: TrecPointer<TControl> - the Parent (the default returns null but the TLayoutParentHolder will return the parent)
 */
TrecPointer<TControl> TLayoutParentHolder::GetParent()
{
	auto sub = TrecPointerKey::GetSubPointerFromSoft<TControl, TLayout>(parent);
	return TrecPointerKey::GetTrecPointerFromSub<TControl, TLayout>(sub);
}

/**
 * Method: TLayoutParentHolder::IsScroller
 * Purpose: Reports to the Child whether the parent holding it is a Scroller Control
 * Parameters: void
 * Returns: bool - whether or not the parent is a Scroller Control
 */
bool TLayoutParentHolder::IsScroller()
{
	return false;
}
