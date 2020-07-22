#pragma once
#include "TGadgetControl.h"

/*
* Class TRadioButton
* Purpose: Implements Anaface's version of the radio buttom
*/
class _ANAFACE_DLL TRadioButton :
	public TGadgetControl
{
	friend class TRadioButton;
public:
	/*
	* Method: TRadioButton::TRadioButton
	* Purpose: Cunstructor
	* Parameters: TrecPointer<DrawingBoard> rt - render target to use
	*				TrecPointer<TArray<styleTable>> ta - the list of Anaface styles
	* Returns: void
	*/
	TRadioButton(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);
	/*
	* Method: TRadioButton::~TRadioButton
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TRadioButton();

	/*
	* Method: TRadioButton::onCreate
	* Purpose: Sets up the control, including radio button specific aspects
	* Parameters: RECT r - the loaction of the button
	* Returns: bool - false (ignore)
	*/
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	/*
	* Method: TRadioButton::onDraw
	* Purpose: Draws Radio Button specific visuals
	* Parameters: void
	* Returns: void
	*/
	void onDraw(TObject* obj = nullptr) override;
	/*
	* Method: TRadioButton::onCreateClass
	* Purpose: Creaes a new class of radio button, used to group buttons
	* Parameters: void
	* Returns: void
	*/
	void onCreateClass();

	void storeInTML(TFile* ar, int childLevel, bool overrideChildren = false) override;


	/*
	* Method: TRadioButton::OnLButtonDown
	* Purpose: Allows Radio buttons thair own control of the the click event
	* Parameters: UINT nFlags - flags involved
	*				CPoint point - point on screen affected
	*				messageOutput * mOut - results of message checking
	*				TDataArray<EventID_Cred>& eventAr - list of events to respond to
	* Returns: void
	*/
	afx_msg void OnLButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);

	/*
	* Method: TRadioButton::OnLButtonUp
	* Purpose: Calls the TControls version (Don't know why this version is here)
	* Parameters: UINT nFlags - flags involved
	*				CPoint point - point on screen affected
	*				messageOutput * mOut - results of message checking
	*				TDataArray<EventID_Cred>& eventAr - list of events to respond to
	* Returns: void
	*/
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/*
	* Method: TRadioButton::GetAnaGameType
	* Purpose: Returns the AnaGame type marker for Radio Buttons
	* Parameters: void
	* Returns: UCHAR* - pointer to AnaGame marker for radio button class
	*/
	virtual UCHAR* GetAnaGameType()override;

	/*
	 * Method: TRadioButton::Resize
	 * Purpose: Resizes the control upon the window being resized, applies to the box inside the control
	 * Parameters: D2D1_RECT_F& r - the new location for the control
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r)override;

private:
	/**
	 * DEPRECATED
	 */
	int otherButtonLocation;
	/**
	 * Other radio buttons in the same group
	 */
	TDataArray<TRadioButton*> otherSameButtons;
	/**
	 * whether the button is clicked or not
	 */
	bool isClicked;
	/**
	 * location of the circle to click
	 */
	D2D1_ELLIPSE ellBut;
	/**
	 * Class of the Radio button
	 */
	TString* buttonClass;
	/*
	* Method: TRadioButton::addButton
	* Purpose: Adds a new radio button to the group
	* Parameters: TRadioButton* trb - button to add
	* Returns: void
	*/
	void addButton(TRadioButton*);
	/**
	 * DEPRECATED
	 */
	bool onClick;
};

