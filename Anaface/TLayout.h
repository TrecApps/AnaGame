#pragma once
#include "TControl.h"

typedef enum class orgLayout
{
	grid,
	VStack,
	HStack,
	VBuff,
	HBuff,
	VMix,
	HMix,
	tCanvas
}orgLayut;

typedef enum class conflictRes
{
	default_Margin,
	default_size

}conflictRes;

/**
 * DEPRECATED
 */
typedef enum class specialLayout
{
	Basic,
	comboBox,
	list,
	Flyout

}specialLayout;

/**
 * Class: containerControl
 * Purpose: holds child controls in the layout and information on where in the layout the controls are
 */
class containerControl : public TObject
{
public:
	/**
	 * the column and row the control starts in
	 */
	UINT x = 0, y=0;
	/**
	 * the control being held
	 */
	TrecPointer<TControl> contain;
	
	/**
	 * whether the control takes up multiple columns/rows
	 */
	bool extend;
	/**
	 * the column/row the control extens to (ignored if "extend" is false)
	 */
	UINT x2 = 0, y2 = 0;
};

/**
 * Class: TLayout
 * Purpose: Extends TControl to hold child controls in an organized manner
 */
class _ANAFACE_DLL TLayout :
	public TControl
{
	friend class AnafaceUI;
public:
	/*
	* Method: TLayout::TLayout
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> rt -  the Render target to draw to (TControl handles it)
	*				TrecPointer<TArray<styleTable>> ta - the Class Style list (TControl handles this)
	* Returns: New TLayout
	*/
	TLayout(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
	/*
	* Method: TLayout::~TLayout
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	* Note: Currently, the destructor is more ceremonial than functional
	*/
	virtual ~TLayout();

	/*
	* Method: TLayout::setLayout
	* Purpose:Sets the layout mode of the TLayout
	* Parameters: orgLayout ol - the layout mode to use
	* Returns: bool - success, true if no children are present, false if a child has been added
	* Note: Call this method before adding any Controls to the Layout, otherwise the method call will fail
	*/
	bool setLayout(orgLayout);
	/*
	* Method: TLayout::setConflictResolutionMode
	* Purpose: Sets the Layouts mechanism for handling layouting conflict amongst its controls
	* Parameters: conflictRes cr - the mode for resolving conflict
	* Returns: bool - success, true if no children are present, false if a child has been added
	* Note: Call this method before adding any Controls to the Layout, otherwise the method call will fail
	*/
	bool setConflictResolutionMode(conflictRes);
	/*
	* Method: TLayout::setSpecialFunction
	* Purpose: Sets the Basic mechanism for what this layout will do
	* Parameters: specialLayout sl - sets the special mode that the Layout will perform
	* Returns: bool - success, true if no children are present, false if a child has been added
	* Note: Call this method before adding any Controls to the Layout, otherwise the method call will fail
	*/
	bool setSpecialFunction(specialLayout);

	/*
	* Method: TLayout::addColunm
	* Purpose: Adds a column to the Layout, as long as it is not a vertical stack
	* Parameters: int x - the size of the new column
	*				bool markDetected - whether the size is absolut or relative compared to available space
	* Returns: whether or not the layout organization was compatible with the method call
	*/
	bool addColunm(int, bool markDetected);

	/*
	* Method: TLayout::addRow
	* Purpose: Adds a new row to the Layout, as long as the layout is not a vertical gallery
	* Parameters: int y - the size of the new row
	*				bool markDetected - whether or not the new size is relative to available space or absolute
	* Returns: whether the method call was compatible with the layout mode
	*/
	bool addRow(int, bool markDetected);
	/*
	* Method: TLayout::addChild
	* Purpose: Adds a new Child Control to the layout at the specified location
	* Parameters: TrecPointer<TControl> tc - the control to add
	*				UINT x - the column to target
	*				UINT y -  he row to target
	* Returns: int - error (0 for no error)
	*/
	int addChild(TrecPointer<TControl>, UINT, UINT);
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
	int addChild(TrecPointer<TControl>, UINT, UINT, UINT, UINT);
	/*
	* Method: TLayout::setGrid
	* Purpose: Sets up a predetermined Grid
	* Parameters: TArray<int>* col - the column layout of the grid
	*				TArray<int>* row - the row layout of the grid
	* Returns: bool - success (method fails if a control is already added or if the layout is not a grid)
	*
	* Note: the columns and rows provided will be given a relative spacing based off of the layouts real size
	*	if you intend to set at least one row and/or column to have an absolute size, don't use this method
	*/
	bool setGrid(TDataArray<int>&, TDataArray<int>&);
	/*
	* Method: TLayout::setStack
	* Purpose: Sets either the Column layout or the Row Layout, based off of the Layout mode
	* Parameters: TArray<int>* nums - the layout of the row/column
	* Returns: bool - success (method fails if layout is a grid or if a control has already been added)
	*/
	bool setStack(TDataArray<int>&);
	//bool setMainChild(int, int);


	/*
	* Method: TLayout::loadFromHTML
	* Purpose: Allows the TLayout to Extract itself from an HTML File
	* Parameters: CArchive* ar - the file to read from
	* Returns: int - 0
	* Note: DEPRECIATED - HTML reading functionality is to be handled by an HTML parser
	 */
	int loadFromHTML(TFile* ar) override;
	/*
	* Method: TLayout::storeInTML
	* Purpose: Allows the TLayout to save itself in an Anaface TML file
	* Parameters: CArchive * ar - the file to read from
	*				int childLevel - the generation the Layout is (how many dashes to write
	*				bool ov - not used
	* Returns: void
	*/
	void storeInTML(TFile* ar, int childLevel,bool ov = false)override;
	/*
	* Method: TLayout::storeInHTML
	* Purpose: Allows the layout to save itself as an HTML tag
	* Parameters: CArchive * ar - the file to write to
	* Returns: void
	*/
	void storeInHTML(TFile* ar)override;

	/*
	* Method: TLayout::onCreate
	* Purpose: Sets up the attributes of the TLayout by processing the attributes and playing it out
	* Parameters: RECT margin - the location the layout has to work with
	* Returns: bool - success
	*/
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	/*
	* Method: TLayout::onDraw
	* Purpose: Draws out the Layout Generated
	* Parameters: void
	* Returns: void
	*/
	virtual void onDraw(TObject* obj = nullptr) override;

	/*
	* Method: TLayout::returnColumnsWidth
	* Purpose: Retrieves the Total Current width of all columns up to a certain column
	* Parameters: int x - the column to stop at
	* Returns: int - The total witdh (-1 if Layout uses only rows)
	*/
	int returnColumnsWidth(int x);

	/*
	* Method: TLayout::returnRowsHeight
	* Purpose: Retrieves the total current height of all rows down to a certain row
	* Parameters: int y - the row to stop at
	* Returns: int - the total height (-1 if Layout uses only columns)
	*/
	int returnRowsHeight(int y);
	/*
	* Method: TLayout::getColunmWidth
	* Purpose: Retrieves the Width of a specific column
	* Parameters: int x - the column to inspect
	* Returns: int - the Size of the column (Zero if column does not exist)
	*/
	int getColunmWidth(int x);
	/*
	* Method: TLayout::getRowHeight
	* Purpose: Retrieves the height of a specific row
	* Parameters: int y - the row to inspect
	* Returns: int - the Size of the Row (0 if row does not exist)
	*/
	int getRowHeight(int y);
	/*
	* Method: TLayout::determineMinHeightNeeded
	* Purpose: Reports the minimum height needed by the Layout and the child controls
	* Parameters: void
	* Returns: UINT - Returns the minimum height needed by the Layout, not the current height
	*/
	virtual UINT determineMinHeightNeeded()override;
	/*
	* Method: TLayout::SetNewLocation
	* Purpose: Sets the new location of the Layout and adjusts the child controls accordingly
	* Parameters: RECT& r - the new location to occupy
	* Returns: void
	*/
	void SetNewLocation(const D2D1_RECT_F& r)override;
	/*
	* Method: TLayout::ShrinkHeight
	* Purpose: Shrinks the Layout to a minimum (Used by Web-tours to make web-page look closer to real web-pages)
	* Parameters: void
	* Returns: void
	*/
	void ShrinkHeight()override;

	/*
	* Method: TLayout::setNewColunmSize
	* Purpose: Sets the size of a given column
	* Parameters: int xLoc - the Column to modify
	*				int x - the size to set that column to
	* Returns: void
	*/
	void setNewColunmSize(int xLoc, int x);
	/*
	* Method: TLayout::setNewRowSize
	* Purpose: Sets the size of a given row
	* Parameters: int yLoc - the row to modify
	*				int y - the size to set that column to
	* Returns: void
	*/
	void setNewRowSize(int yLoc, int y);

	/*
	* Method: TLayout::getRowNumber
	* Purpose: Retireves the number of rows present
	* Parameters: void
	* Returns: int - the number of rows
	*/
	int getRowNumber();
	/*
	* Method: TLayout::getColumnNumber
	* Purpose: Retrieves the number of Columns present
	* Parameters: void
	* Returns: int - the number of columns
	*/
	int getColumnNumber();

	/*
	* Method: TLayout::getRawSectionLocation
	* Purpose: Gets the Location of the section specified by the coordinates and permitted to any control occupied there
	* Parameters: int r - the row to look at
	*				int c - the column to look at
	* Returns: D2D1_RECT_F - the location allocated to the specified location
	*/
	D2D1_RECT_F getRawSectionLocation(int r, int c);

	/*
	* Method: TLayout::GetLayoutChild
	* Purpose: Retrieves the Child located at the specified coordinates
	* Parameters: int x - the x-coordinate of the child control
	*				int y - the y-coordinate of the child control
	* Returns: TrecPointer<TControl> - the child control held at the coordinates (Null if child is absent or if a parameter is out of bounds)
	*/
	TrecPointer<TControl> GetLayoutChild(int x, int y);
	/*
	* Method: TLayout::GetOrganization
	* Purpose: Retrieves the organization mode of the layout
	* Parameters: void
	* Returns: orgLayout - the organization the layout is configured to
	*/
	orgLayout GetOrganization();

	virtual UCHAR* GetAnaGameType()override;


protected:
	/**
	 * used to organize children more easily than regular TControl
	 */
	TArray<containerControl> lChildren;

	/*
	 * Method: TLayout::SwitchChildControl
	 * Purpose: Allows a Child Control to be swapped
	 * Parameters: TrecPointerSoft<TControl> curControl - the control making the call
	 *				TrecPointer<TControl> newTControl - the Control to replace it with
	 * Returns: void
	 */
	void SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl)override;
	
	/**
	 * the number of columns/rows
	 */
	int colunms, rows;
	/**
	 * How the Child controls are organized
	 */
	orgLayout organization;
	/**
	 * other details regarding the layout (possibly redundant?)
	 */
	specialLayout specialFunction;
	/**
	 * how to handle conflicts between the size and margin
	 */
	conflictRes conflictResolute;



	/*
	 * Method: TLayout::returnMinX
	 * Purpose: Returns the left-x coordinate of the right most column depending on whether a new column is expected
	 * Parameters: bool newColumn - whether a new column is expected
	 * Returns: int - the minimum x-coordinate the right most column available
	 */
	int returnMinX(bool newColumn);
	/*
	 * Method: TLayout::returnMinY
	 * Purpose: Returns the bottom-y coordinate of the bottom row depending on whether a new row is expected
	 * Parameters: bool newRow - whether a new row is expected
	 * Returns: int - the top coordinate of the bottom row whether it is the current bottom or a new row
	 */
	int returnMinY(bool newRow);
	/*
	* Method: TLayout::returnRectX
	* Purpose: Returns the location of the given of the column (and first row)
	* Parameters: int x - the column to look at
	* Returns: D2D1_RECT_F - the location of that area
	*/
	D2D1_RECT_F returnRectX(int);
	/*
	* Method: TLayout::returnRectY
	* Purpose: Returns the location of the given of the row (and first column)
	* Parameters: int y - the row to look st
	* Returns: D2D1_RECT_F the location of the area
	*/
	D2D1_RECT_F returnRectY(int);


	/**
	 * Holder for a child container
	 */
	TrecPointer<containerControl> tempContC;
	/**
	 * DEPRECATED
	 */
	TControl* main;

	/**
	 * the width/height of the columns/rows
	 */
	TDataArray<int> columnLines, rowLines;
	/**
	 * Used to determin which columns/rows are flexible
	 */
	TDataArray<UCHAR> columnFlex, rowFlex;
	/*
	* Method: TLayout::GetColumnFlexAt
	* Purpose: Determines whether a given column is flexible and able to change size
	* Parameters: UINT col - the column to check
	* Returns: bool - whether the column is flexible
	*/
	bool GetColumnFlexAt(UINT col);
	/*
	* Method: TLayout::GetRowFlexAt
	* Purpose: Determines whether a given row is flexible and able to change size
	* Parameters: UINT row - the row to check
	* Returns: bool whether the row is flexible
	*/
	bool GetRowFlexAt(UINT row);

	/*
	* Method: TLayout::AddToColFlex
	* Purpose: Adds a new column to the column flexibility array
	* Parameters: bool val - whether the new Column is to be flexible or not
	* Returns: void
	*/
	void AddToColFlex(bool val);
	/*
	* Method: TLayout::AddToRowFlex
	* Purpose: Adds a new row to the row flexibility array
	* Parameters: bool var - whether the new row is to be flexible or not
	* Returns: void
	*/
	void AddToRowFlex(bool val);

	/*
	* Method: TLayout::GetTotalFlexRow
	* Purpose: Retrieves the total height of all rows marked as flexible
	* Parameters: void
	* Returns: UINT - current height of all flexible rows
	*/
	UINT GetTotalFlexRow();
	/*
	* Method: TLayout::GetTotalSetRow
	* Purpose: Retrieves the total height of all rows marked as static
	* Parameters: void
	* Returns: UINT - current height of all fixed sized rows
	*/
	UINT GetTotalSetRow();
	/*
	* Method: TLayout::GetTotalFlexCol
	* Purpose: Retrieves the total width of all columns marked as flexible
	* Parameters: void
	* Returns: UINT - current width of all flexible columns
	*/
	UINT GetTotalFlexCol();
	/*
	* Method: TLayout::GetTotalSetCol
	* Purpose: Retrieves the total width of all columns marked as static
	* Parameters: void
	* Returns: UINT - current width of all fixed sized columns
	*/
	UINT GetTotalSetCol();
	/*
	 * Method: TLayout::Resize
	 * Purpose: resets the Size of the TLayout
	 * Parameters: D2D1_RECT_F& r - the location to set the TLayout to
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r)override;

	/**
	 * whether the rows or columns have been initializes
	 */
	bool updateRow, updateColumn;


	// Brushes unique to TLayouts

	/**
	 * the Brush to draw internal boundaries with
	 */
	TrecPointer<TBrush> internalBrush;

	/**
	 * the color of the brush
	 */
	D2D1_COLOR_F internalColor;

	/**
	 * Whether the brish has been initialized
	 */
	bool internalInit;

	/**
	 * thickness of the internal brush
	 */
	float thickness;
};

