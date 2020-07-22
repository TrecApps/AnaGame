#pragma once
#include "TControl.h"
#include <TObjectNode.h>

/*
 * class TTreeDataBind
 * Purpose: Support for depicting a tree-based system on Anaface
 */
class _ANAFACE_DLL TTreeDataBind :
	public TControl
{
public:
	/**
	 * Method: TTreeDataBind::TTreeDataBind
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Returns: New Tree Object
	 */
	TTreeDataBind(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
	/**
	 * Method: TTreeDataBind::~TTreeDataBind
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TTreeDataBind();

	/**
	 * Method: TTreeDataBind::onDraw
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
	 * Method: TTreeDataBind::onCreate
	 * Purose: the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: RECT contain - the area it can use
	 * Returns: bool - success
	 */
	bool onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d) override;

	/**
	 * Method: TTreeDataBind::OnLButtonDown
	 * Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons) override;
	/**
	 * Method: TTreeDataBind::OnLButtonUp
	 * Purpose: Allows control to catch the Left Button Up event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;
	/**
	 * Method: TTreeDataBind::OnMouseMove
	 * Purpose: Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;
	/**
	 * Method: TTreeDataBind::OnLButtonDblClk
	 * Purpose: Allows control to catch the DOuble Click event and act accordingly
	 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
	 *				TPoint point - the point on screen where the event occured
	 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
	 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
	 * Returns: void
	 */
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	/**
	 * Method: TTreeDataBind::SetNode
	 * Purpose: Sets the node of the Control so that when it is time to draw, the control has content to produce
	 * Parameters: TrecPointer<TObjectNode> newNode - the node of the tree to depict when drawing
	 * Returns: void
	 */
	void SetNode(TrecPointer<TObjectNode> newNode);
	/**
	 * Method: TTreeDataBind::getLocation
	 * Purpose: Reports how much space this object will actually need to draw
	 * Parameters: void
	 * Returns: D2D1_RECT_F -  the Rectangle of the content that would be drawn (even if it was officially allocated less space)
	 */
	D2D1_RECT_F getLocation()override;
	/**
	 * Method: TTreeDataBind::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: RECT r - the new location for the control
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r)override;


	/* Purpose: to allow the TControl to shift it's contents according to how it is scrolled.
	 * Caller: Member TScrollBars, either vScroll or hScroll
	 * Parameter: int (x) how to shift itself horizontally
	 * Parameter: int (y) how to shift itself vertically
	 * return: bool, just because
	 * Details: Changes the contents location on the board, reflected in the changes to
	 *		TControl's location, but not where it draws, hence why it's own snip stays the same
	 *		However, the contents might include child locations so their snips might have to be
	 *		updated
	 */
	virtual bool onScroll(int x, int y)override;
protected:
	/**
	 * The root node that starts the tree this control goes down
	 */
	TrecPointer<TObjectNode> mainNode;

	/**
	 * Used to track whether a node is selected and if the tick is selected
	 */
	bool isNodeSelected, isTickSelected;

	/**
	 * Used to trck the initial click down so when the user clicks up, the node can be determined
	 */
	UINT nodeSelected;

	/**
	 * Brush that paints the outside of the arrow
	 */
	TrecPointer<TBrush> outerBrush;
	/**
	 * Brush that paint the inside of the arrow
	 */
	TrecPointer<TBrush> innerBrush;
};

