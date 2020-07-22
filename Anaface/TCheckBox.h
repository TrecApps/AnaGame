#pragma once
#include "TGadgetControl.h"
/**
 * Class: TCheckBox
 * Purpose: Allows Anagame to provide checkboxes in it's interfaces
 */
class _ANAFACE_DLL TCheckBox :
	public TGadgetControl
{
public:
	/*
	* Method: TCheckBox::TCheckBox
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> rt - the render target to draw to
	*				TrecPointer<TArray<styleTable>> ta - the style list for Anaface
	* Returns: void
	*/
	TCheckBox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);
	/*
	* Method: TCheckBox::~TCheckBox
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TCheckBox();

	/*
	* Method: TCheckBox::onCreate
	* Purpose: Sets up the check-boxes attribute
	* Parameters: RECT r - the location for the check-box
	* Returns: bool - ignore
	*/
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	/*
	* Method: TCheckBox::onDraw
	* Purpose: Draws the check-box
	* Parameters: void
	* Returns: void
	*/
	void onDraw(TObject* obj = nullptr) override;
	/*
	* Method: TCheckBox::IsClicked
	* Purpose: Reports whether the Check-box is in a clicked state
	* Parameters: void
	* Returns: bool - whether the check-box has been "selected" or not
	*/
	bool IsClicked();

/*
* Method: TCheckBox::OnLButtonDown
* Purpose: Catches the OnClick event and allows the check box to add a check-box event to the Event List
* Parameters: UINT nFlags - Details provided by Windows
*				CPoint point - The location of the click
*				messageOutput * mOut - The results of the Event (was a control updated?)
*				TDataArray<EventID_Cred>& eventAr - the List of Events in a User's actions
* Returns: void
*/
	afx_msg void OnLButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	/*
	* Method: TCheckBox::OnLButtonUp
	* Purpose: Catches the Click Release method
	* Parameters: UINT nFlags - Details provided by Windows
	*				CPoint point - The location of the click
	*				messageOutput * mOut - The results of the Event (was a control updated?)
	*				TDataArray<EventID_Cred>& eventAr - the List of Events in a User's actions
	* Returns: void
	*/
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	

	/*
	* Method: TCheckBox::GetAnaGameType
	* Purpose: Retrieves the Structure that lets AnaGame know that this is a TCheck-Box
	* Parameters: void
	* Returns: UCHAR* - The AnaGame type structure
	*/
	virtual UCHAR* GetAnaGameType()override;
private:

	bool isClicked;
	int width, height;
	bool onClick;

};

