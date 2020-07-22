#pragma once
#include "TLayoutEx.h"


typedef struct _ANAFACE_DLL BorderList
{
	float thickness;
	D2D1_COLOR_F color;
	bool isVertical;
	USHORT singlDimension;
	USHORT doubDum1, doubDum2;
}BorderList;

/**
 * Class: TSpreadSheet
 * Purpose: Supplies the Spreadsheet control
 */
class _ANAFACE_DLL TSpreadSheet :
	public TLayoutEx
{
public:


	/**
	 * Method: TSpreadSheet::TSpreadSheet
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Returns: New Tree Object
	 */
	TSpreadSheet(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta, HWND w);
	/**
	 * Method: TSpreadSheet::~TSpreadSheet
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TSpreadSheet();

	/**
	 * Method: TSpreadSheet::onCreate
	 * Purose: the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: RECT contain - the area it can use
	 * Returns: bool - success
	 */
	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	/**
	 * Method: TSpreadSheet::onDraw
	 * Purpose: Draws the control
	 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
	 * Returns: void
	 */
	virtual void onDraw(TObject* obj = nullptr) override;
	/**
	 * DEPRECATED
	 */
	virtual UCHAR* GetAnaGameType()override;

	/**
	 * Method: TSpreadSheet::GetData
	 * Purpose: Returns the contents of the Spreadsheet in each cell
	 * Parameters: void
	 * Returns: TString - the contents of the spreadsheet
	 */
	TString GetData();	
	/**
	 * Method: TSpreadSheet::GetDataSplitTokens
	 * Purpose: Reports the tokens being used to divide the contents of the "GetData()" method
	 * Parameters: void
	 * Returns: TString - the divider used in "GetData()"
	 */
	TString GetDataSplitTokens();

	/**
	 * Method: TSpreadSheet::OnLButtonDown
	* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	*				TPoint point - the point on screen where the event occured
	*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	* Returns: void
	 */
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl) override;

protected:
	/**
	 * The Title of the spreadsheet and the divider to use
	 */
	TString title, splitTokens;
	/**
	 * Currently not used
	 */
	UCHAR header_x_span, header_y_span;
	/**
	 * Instructions on how to manage the Spreadsheet
	 */
	bool initializeFields, drawLines, stickToNums, hasTitle;
	/**
	 * Borders between cells
	 */
	TDataArray<BorderList> borders;
	/**
	 * Handle to the window drawing upon
	 */
	HWND window;
	/**
	 * Allows creation of new Controls under this control
	 */
	TrecPointer<TWindowEngine> winEngine;
};

