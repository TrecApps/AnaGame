#include "TTreeDataBind.h"
#include "TScrollerControl.h"
#include <d2d1.h>

/**
 * Method: TTreeDataBind::TTreeDataBind
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
 * Returns: New Tree Object
 */
TTreeDataBind::TTreeDataBind(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta): TControl(rt, ta)
{
	isNodeSelected = isTickSelected = false;
	nodeSelected = 0;
}

/**
 * Method: TTreeDataBind::~TTreeDataBind
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TTreeDataBind::~TTreeDataBind()
{
	int e = 3;
}

/**
 * Method: TTreeDataBind::onDraw
* Purpose: Draws the control
* Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
* Returns: void
 */
void TTreeDataBind::onDraw(TObject* obj)
{
	if (!mainNode.Get())
		return;

	TrecPointer<TControl> cont = children.ElementAt(0);
	if (!cont.Get())
		return;

	TrecPointer<TObjectNode> curNode = mainNode;

	D2D1_RECT_F cLoc = location;
	cLoc.bottom = cLoc.top + 30;
	TrecPointer<TGeometry> path;
	
	for (UINT c = 0; curNode.Get(); c++)
	{
		bool ext = curNode->IsExtended();
		bool extDraw = curNode->IsExtendable();

		UINT level = curNode->GetLevel();
		D2D1_RECT_F triLoc = cLoc;
		triLoc.bottom -= 5;
		triLoc.top += 5;
		triLoc.left += 5 + level * 5;
		triLoc.right = triLoc.left + 20;

		
		if (extDraw)
		{

			TDataArray<POINT_2D> points;

			if (ext)
			{
				points.push_back(D2D1::Point2F(triLoc.left, triLoc.bottom));
				points.push_back(D2D1::Point2F(triLoc.right, triLoc.top));
				points.push_back(D2D1::Point2F(triLoc.right, triLoc.bottom));
			}
			else
			{
				points.push_back(D2D1::Point2F(triLoc.left, triLoc.bottom));
				points.push_back(D2D1::Point2F(triLoc.left, triLoc.top));
				points.push_back(D2D1::Point2F(triLoc.right, (triLoc.top + triLoc.bottom) / 2.0f));
			}
			
			path = drawingBoard->GetGeometry(points);

			if (path.Get())
			{
				if (outerBrush.Get())
					outerBrush->DrawGeometry(path, 0.5f);
				if (innerBrush.Get())
					innerBrush->FillGeometry(path);

			}
			path.Delete();
		}


		UINT r = triLoc.right;

		triLoc = cLoc;

		triLoc.left = r;

		cont->setLocation(triLoc);
		cont->onDraw(curNode.Get());

		// Prepare for the next draw
		cLoc.bottom += 30;
		cLoc.top += 30;

		curNode = mainNode->GetNodeAt(c + 1, 0);
	}

}
/**
 * DEPRECATED
 */
UCHAR* TTreeDataBind::GetAnaGameType()
{
	return nullptr;
}

/**
 * Method: TTreeDataBind::onCreateAllows
 * Purose: the Control To contstruct itself based off of the location it has and the
 *		screen space it is given
 * Parameters: RECT contain - the area it can use
 * Returns: bool - success
 */
bool TTreeDataBind::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TControl::onCreate(r, d3d);
	if (drawingBoard.Get())
	{
		outerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Black));
		innerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Wheat));
	}
	if (children.Count() && children.ElementAt(0).Get())
	{
		children.ElementAt(0)->onCreate(r, d3d);
	}
	return false;
}
/**
 * Method: TTreeDataBind::OnLButtonDown
* Purpose: Allows Control to catch the LeftmessageState::mouse Button Down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
 */
void TTreeDataBind::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	if (isContained(&point, &location))
	{
		if (mainNode.Get())
		{
			float dist = point.y - location.top;

			UINT targetNode = static_cast<UINT>(dist) / 30;

			TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(targetNode, 0);

			if (tNode.Get())
			{
				UINT level = tNode->GetLevel();
				D2D1_RECT_F cLoc = location;
				cLoc.top = cLoc.top + 30 * targetNode;
				cLoc.bottom = cLoc.top + 30;
				D2D1_RECT_F triLoc = cLoc;
				triLoc.bottom -= 5;
				triLoc.top += 5;
				triLoc.left += 5 + level * 5;
				triLoc.right = triLoc.left + 20;

				if (isContained(&point, &triLoc))
				{
					isTickSelected = true;
					
				}
				clickedButtons.push_back(this);
				isNodeSelected = true;
				nodeSelected = targetNode;
			}
		}

		TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
	}
}
/**
 * Method: TTreeDataBind::OnLButtonUp
 * Purpose: Allows control to catch the Left Button Up event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 */
void TTreeDataBind::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (isContained(&point, &location))
	{
		if (mainNode.Get())
		{
			float dist = point.y - location.top;

			UINT targetNode = static_cast<UINT>(dist) / 30;

			TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(targetNode, 0);

			if (tNode.Get())
			{
				UINT level = tNode->GetLevel();
				D2D1_RECT_F cLoc = location;
				cLoc.top = cLoc.top + 30 * targetNode;
				cLoc.bottom = cLoc.top + 30;
				D2D1_RECT_F triLoc = cLoc;
				triLoc.bottom -= 5;
				triLoc.top += 5;
				triLoc.left += 5 + level * 5;
				triLoc.right = triLoc.left + 20;

				if (isContained(&point, &triLoc) && isTickSelected && targetNode == nodeSelected)
				{
					if (tNode->IsExtended())
						tNode->DropChildNodes();
					else if(tNode->IsExtendable())
					{
						tNode->Extend();
						Resize(location);
					}
				}
			}
		}
		TControl::OnLButtonUp(nFlags, point, mOut, eventAr);
	}
	isTickSelected = isNodeSelected = false;
	nodeSelected = 0;
}
/**
 * Method: TTreeDataBind::OnMouseMove Allows Controls to catch themessageState::mouse Move event and deduce if the cursor has hovered over it
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 *				TDataArray<TControl*>& clickedControls - list of controls that exprienced the on Button Down Event to alert when the button is released
 * Returns: void
 */
void TTreeDataBind::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
	TControl::OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
}
/**
 * Method: TTreeDataBind::OnLButtonDblClk
 * Purpose: Allows control to catch the DOuble Click event and act accordingly
 * Parameters: UINT nFlags - flags provided by MFC's Message system, not used
 *				TPoint point - the point on screen where the event occured
 *				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
 *				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
 * Returns: void
 */
void TTreeDataBind::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	TControl::OnLButtonDblClk(nFlags, point, mOut, eventAr);

	
	if (isContained(&point, &location) && mainNode.Get())
	{
		float dist = point.y - location.top;

		UINT targetNode = static_cast<UINT>(dist) / 30;

		TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(targetNode, 0);
		
		if (tNode.Get())
			args.object = tNode;
	}
}

/**
 * Method: TTreeDataBind::SetNode
 * Purpose: Sets the node of the Control so that when it is time to draw, the control has content to produce
 * Parameters: TrecPointer<TObjectNode> newNode - the node of the tree to depict when drawing
 * Returns: void
 */
void TTreeDataBind::SetNode(TrecPointer<TObjectNode> newNode)
{
	mainNode = newNode;
}

/**
 * Method: TTreeDataBind::getLocation
 * Purpose: Reports how much space this object will actually need to draw
 * Parameters: void
 * Returns: D2D1_RECT_F -  the Rectangle of the content that would be drawn (even if it was officially allocated less space)
 */
D2D1_RECT_F TTreeDataBind::getLocation()
{
	D2D1_RECT_F ret = location;
	if (mainNode.Get())
		ret.bottom = ret.top + 30 * (mainNode->TotalChildren() + 1);
	return ret;
}

/**
 * Method: TTreeDataBind::Resize
 * Purpose: Resizes the control upon the window being resized
 * Parameters: RECT r - the new location for the control
 * Returns: void
 */
void TTreeDataBind::Resize(D2D1_RECT_F& r)
{
	// First Check to see if we need a new scroll control
	D2D1_RECT_F tempLoc = this->getLocation();
	if ((tempLoc.bottom - tempLoc.top > r.bottom - r.top) ||
		(tempLoc.right - tempLoc.left > r.right - r.left))
	{
		if (parent.Get() && !parent->IsScroller())
		{
			TrecPointer<TControl> scrollControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TScrollerControl>(drawingBoard, styles);
			scrollControl->onCreate(r, TrecPointer<TWindowEngine>());

			auto oldParent = parent;

			dynamic_cast<TScrollerControl*>(scrollControl.Get())->SetChildControl(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));
			oldParent->SwitchChildControl(tThis, scrollControl);
			location.left = r.left;
			location.top = r.top;
		}
		return;
	}


	// We do not, so proceed
	location = r;
	updateComponentLocation();

/*
	TControl* ele = nullptr;

	
	if (children.Count() && (ele = children.ElementAt(0).Get()))
	{
		D2D1_RECT_F loc;
		if (this->isStack)
		{
			loc.left = location.left;
			loc.right = location.right;
			loc.top = location.top;
			loc.bottom = loc.top + this->widthHeight;
		}
		else
		{
			loc.top = location.top;
			loc.bottom = location.bottom;
			loc.left = location.left;
			loc.right = loc.left + this->widthHeight;
		}
		ele->Resize(loc);
	}*/
}

bool TTreeDataBind::onScroll(int x, int y)
{
	location.left += x;
	location.right += x;
	location.top += y;
	location.bottom += y;


	return true;
}
