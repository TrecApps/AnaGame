#pragma once
#include "TLayout.h"
class _ANAFACE_DLL TLayoutEx :	public TLayout
{
public:
	/*
	 * Method: TLayoutEx::TLayoutEx
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt -  the Render target to draw to (TControl handles it)
	*				TrecPointer<TArray<styleTable>> ta - the Class Style list (TControl handles this)
	 * Returns: New TLayoutEx Object
	 */
	TLayoutEx(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
	/*
	 * Method: TLayoutEx::~TLayoutEx
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TLayoutEx();

	/*
	 * Method: TLayoutEx::RemoveChildWithPrejudice
	 * Purpose: Performs Delete on a child control
	 * Parameters: UINT x - the column of the control
	 *				UINT y - the row of the control
	 * Returns: int - 0
	 */
	int RemoveChildWithPrejudice(UINT x, UINT y);
	/*
	 * Method: TLayoutEx::RemoveColumn
	 * Purpose: Removes the specified column from the control
	 * Parameters: UINT c - index of the column to remove
	 * Returns: int - error code (0 if successful)
	 */
	int RemoveColumn(UINT c);
	/*
	 * Method: TLayoutEx::RemoveRow
	 * Purpose: Removes the specified row from the control
	 * Parameters: UINT c - index of the row to remove
	 * Returns: int - error code (0 if successful)
	 */
	int RemoveRow(UINT c);

	/*
	 * Method: TLayoutEx::AddRow
	 * Purpose: Appends a Row to the Layout
	 * Parameters: UINT size - the height of the row to add
	 * Returns: UINT - 0 if falied, otherwise the number of rows currently in the control
	 */
	UINT AddRow(UINT size);
	/*
	 * Method: TLayoutEx::AddCol
	 * Purpose: Appends a Column to the Layout
	 * Parameters: UINT size - the width of the column to add
	 * Returns: UINT - 0 if falied, otherwise the number of collumns currently in the control
	 */
	UINT AddCol(UINT size);

	/*
	 * Method: TLayoutEx::RemoveChild
	 * Purpose: Removes a certain control from the layout
	 * Parameters: UINT x - the column of the control
	 *				UINT y - the row of the control
	 * Returns: TrecPointer<TControl> - the control that has been removed
	 */
	TrecPointer<TControl> RemoveChild(UINT x, UINT y);

	/*
	 * Method: TLayoutEx::onCreate
	 * Purpose: Sets up the attributes of the TLayout by processing the attributes and playing it out
	 * Parameters: RECT margin - the location the layout has to work with
	 * Returns: bool - success
	 */
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;

	/*
	 * Method: TLayoutEx::onDraw
	 * Purpose: Draws out the Layout Generated
	 * Parameters: void
	 * Returns: void
	 */
	virtual void onDraw(TObject* obj = nullptr) override;
	virtual UCHAR* GetAnaGameType()override;
};

