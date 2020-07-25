#pragma once
#include "TGadgetControl.h"
#include "TFlyout.h"

class TComboBox;

/**
 * Class: TComboBoxExtension
 * Purpose: Special control meant to be attached to a TComboBox via a TFlyout and providing the drop-down menu
 */
class TComboBoxExtension: public TControl
{
	friend class TComboBox;
public: 
	/**
	 * Method: TComboBoxExtension::TComboBoxExtension
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - The Drawing Board to draw on
	 * 				TrecPointer<TArray<styleTable>> ta - the Style Table to provide classes in
	 * 				TrecSubPointer<TControl, TComboBox> combo - the ComboBox to communicate with
	 * Returns: New Combobox Extension
	 */
	TComboBoxExtension(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, TrecSubPointer<TControl, TComboBox> combo);

	/**
	 * Method: TComboBoxExtension::addElement
	 * Purpose: Adds a String to the selection
	 * Parameters: TString& str - the String to add
	 * Returns: void
	 */
	void addElement(TString&);
	/**
	 * Method: TComboBoxExtension::removeElement
	 * Purpose: Removes a String from the list of selections
	 * Parameters: TString& str - the String to remove
	 * Returns: bool - whether the given string was in the selection 
	 */
	bool removeElement(TString&);

	/**
	 * Method: TComboBoxExtension::Resize
	 * Purpose: Resets the location of the selection
	 * Parameters: D2D1_RECT_F& r -  the bounds of the selection
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r) override;

	/**
	 * Method: TComboBoxExtension::onDraw
	 * Purpose: Draws the list of selections available to the Combobox
	 * Parameters: TObject* obj - the object whose data to present (should be NULL)
	 * Returns: void
	 */
	virtual void onDraw(TObject* obj)override;

	/**
	 * Method: TComboBoxExtension::OnLButtonDown
	 * Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)override;

	/**
	 * Method: TComboBoxExtension::OnLButtonUp
	 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: TComboBoxExtension::OnMouseMove
	 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	*				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
	 * Returns: void 
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls) override;
protected:
	/**
	 * List of Strings to present as options
	 */
	TDataArray<TString> elements;

	/**
	 * The amount of vertical space to offer each option
	 */
	UINT childHeight;

	/**
	 * the Max Hieght to draw before inserting a Scroller Control between itself and the parent 
	 * (0 for no limit)
	 */
	UINT maxHeight;

	/**
	 * Tracks which element the mouse is hovering over
	 */
	int hoverSelection;
	
	/**
	 * Tracks which selection was clicked upon so that if the Click release is over the same selection
	 * it can be reported to the combo-box properly
	 */
	int clickSelection;
	
	/**
	 * The Combobox that created the Extension
	 */
	TrecSubPointer<TControl, TComboBox> combo;
};


/*
* Class: TComboBox
* Purpose: Supports Anaface's ability to have drop down menus
* SuperClass: TGadgetControl - allows the combobox to have a feture within it
*/
class _ANAFACE_DLL TComboBox :
	public TGadgetControl
{
	friend TComboBox;
public:
	/**
	 * Method: TComboBox::TComboBox
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - The Drawing Board to draw on
	 * 				TrecPointer<TArray<styleTable>> ta - the Style Table to provide classes in
	 * Returns: New Combobox 
	 */
	TComboBox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);
	
	/**
	 * Method: TComboBox::~TComboBox
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TComboBox();

	/**
	 * Method: TComboBox::onCreate
	 * Purpose: Allows the Combobox to build itself and it's extension
	 * Parameters: D2D1_RECT_F - the region the combo box is to draw in
	 * 				TrecPointer<TWindowEngine> d3d - not used by the Combobox
	 * Returns: void
	 */
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;

	/**
	 * Method: TComboBox::OnLButtonDown
	 * Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)override;

	/**
	 * Method: TComboBox::OnLButtonUp
	 * Purpose: Allows Control to catch the RightmessageState::mouse button release event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: TComboBox::Resize
	 * Purpose: Resets the location of the combo-box
	 * Parameters: D2D1_RECT_F& r -  the bounds of the bos
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r) override;

	/**
	 * Method: TComboBox::UpdateCaption
	 * Purpose: Updates the main caption and sets up Event arguments
	 * Parameters: TString& str - the string to set the main contents to
	 * 				UINT index - the Index in the selection
	 * Returns: void
	 */
	void UpdateCaption(TString& str, UINT index);
private:
	/**
	 * The Flyout holding the extension (to feed the Page when clicked upon so that thw Window will draw it)
	 */
	TrecPointer<TFlyout> flyout;

	/**
	 * The actual extension itself
	 */
	TrecSubPointer<TControl, TComboBoxExtension> extension;

	/**
	 * The String to draw at first
 	 */ 
	TString defaultText;

	/**
	 * Used for drawing the arrow
	 */ 
	D2D1_POINT_2F leftpoint, vertexPoint, rightPoint;

	/**
	 * Tracks whether the box was clicked or not
	 */
	bool initClick;
};
