#pragma once
#include "TRandomLayout.h"
#include <TObjectNode.h>

/**
 * Class: TDataNodeControl
 * Purpose: Support for depicting a tree-based system on Anaface 2
 * 
 * Note: Based heavily on the TTreeDataBind class in the original Anaface
 */
class TDataNodeControl :
    public TRandomLayout
{
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
	 * Whether expansion needs to be blocked
	 */
	bool blockExpansion;
	/**
	 * Used to trck the initial click down so when the user clicks up, the node can be determined
	 */
	UINT nodeSelected;

	/**
	 * Used to know when to highlight a given node
	 */
	UINT highlightNodeSelected;

	/**
	 * The color to use for Highlighing
	 */
	TrecPointer<TBrush> nodeBrush;

	/**
	 * Brush that paints the outside of the arrow
	 */
	TrecPointer<TBrush> outerBrush;
	/**
	 * Brush that paint the inside of the arrow
	 */
	TrecPointer<TBrush> innerBrush;

public:
	TrecPointer<TObjectNode> GetNode();


	TDataNodeControl(TrecPointer<DrawingBoard> board, TrecPointer<TArray<styleTable>> ta);

	/**
	 * Method: TTreeDataBind::SetNode
	 * Purpose: Sets the node of the Control so that when it is time to draw, the control has content to produce
	 * Parameters: TrecPointer<TObjectNode> newNode - the node of the tree to depict when drawing
	 * Returns: void
	 */
	void SetNode(TrecPointer<TObjectNode> newNode);

	/**
	 * Method: AnafacePage::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: AnafacePage::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: AnafacePage::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;

	/**
	 * Method: AnafacePage::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


	/**
	 * Method: AnafacePage::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;

	/**
	 * Method: TTreeDataBind::onCreate
	 * Purose: the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: RECT contain - the area it can use
	 * Returns: bool - success
	 */
	bool onCreate(const D2D1_RECT_F& r, TrecPointer<TWindowEngine> d3d) override;

	/**
	 * Method: AnafacePage::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;

	/**
	 * Method: TPage::GetArea
	 * Purpose: Retrieves the Area held by the Page
	 * Parameters: void
	 * Returns: D2D1_RECT_F -  the Region the Page claims it is drawing to
	 *
	 * Attributes: virtual
	 */
	virtual D2D1_RECT_F GetArea();
};

