#include "TTreeDataBind.h"
#include "TScrollerControl.h"
#include <d2d1.h>

/**
 * Method: TTreeDataBind::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TTreeDataBind::GetType()
{
	return TString(L"TTreeDataBind;") + TControl::GetType();
}

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
	highlightNodeSelected = UINT32_MAX;

	blockExpansion = false;
}

/**
 * Method: TTreeDataBind::~TTreeDataBind
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TTreeDataBind::~TTreeDataBind()
{
}

/**
 * Method: TTreeDataBind::onDraw
* Purpose: Draws the control
* Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
* Returns: void
 */
void TTreeDataBind::onDraw(TObject* obj)
{
	ThreadLock();
	if (!mainNode.Get())
	{
		ThreadRelease();
		return;
	}
	TrecPointer<TControl> cont = children.ElementAt(0);
	if (!cont.Get())
	{
		ThreadRelease();
		return;
	}
	TrecPointer<TObjectNode> curNode = mainNode;

	D2D1_RECT_F cLoc = location;
	cLoc.bottom = cLoc.top + 30;
	
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

			auto path = drawingBoard->GetGeometry(points);
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

		if (nodeBrush.Get() && c == highlightNodeSelected)
			nodeBrush->FillRectangle(cLoc);
		
		// Prepare for the next draw
		cLoc.bottom += 30;
		cLoc.top += 30;

		curNode = mainNode->GetNodeAt(c + 1, 0);
	}
	ThreadRelease();
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
	ThreadLock();
	drawBackground = false;
	TControl::onCreate(r, d3d);
	if (drawingBoard.Get())
	{
		outerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Black));
		innerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Wheat));

		TrecPointer<TString> valpoint = attributes.retrieveEntry(L"|NodeHighlightColor");

		D2D1_COLOR_F nodeColor{ 1.0f,1.0f,1.0f, 0.0f };

		if (valpoint.Get())
		{
			nodeColor = convertStringToD2DColor(valpoint.Get());
		}

		nodeBrush = drawingBoard->GetBrush(TColor(nodeColor));


		valpoint = attributes.retrieveEntry(L"|BlockExtension");

		if (valpoint.Get() && !valpoint->CompareNoCase(L"true"))
		{
			blockExpansion = true;
		}

	}
	if (children.Count() && children.ElementAt(0).Get())
	{
		children.ElementAt(0)->onCreate(r, d3d);
	}
	ThreadRelease();

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
	ThreadLock();
	if (isContained(point, getLocation()))
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
	ThreadRelease();
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
	ThreadLock();
	if (isContained(point, getLocation()))
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

				if (targetNode == nodeSelected)
				{
					if (isContained(&point, &triLoc) && isTickSelected && !blockExpansion)
					{
						if (tNode->IsExtended())
							tNode->DropChildNodes();
						else if (tNode->IsExtendable())
						{
							tNode->Extend();
							Resize(location);
						}

						
					}
					else
					{
						resetArgs();

						args.eventType = R_Message_Type::On_sel_change;
						args.point = point;
						args.methodID = getEventID(R_Message_Type::On_sel_change);
						args.isClick = true;
						args.isLeftClick = false;
						args.control = this;
						args.object = tNode;

						eventAr.push_back(EventID_Cred(R_Message_Type::On_sel_change, TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis)));
					}
					
					highlightNodeSelected = nodeSelected;
				}
			}
		}
		TControl::OnLButtonUp(nFlags, point, mOut, eventAr);
	}
	isTickSelected = isNodeSelected = false;
	nodeSelected = 0;
	ThreadRelease();
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
	ThreadLock();
	TControl::OnMouseMove(nFlags, point, mOut, eventAr, hoverControls);
	ThreadRelease();
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
	ThreadLock();
	if (isContained(point, getLocation()) && mainNode.Get())
	{
		TControl::OnLButtonDblClk(nFlags, point, mOut, eventAr);
		float dist = point.y - location.top;
		UINT targetNode = static_cast<UINT>(dist) / 30;
		TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(targetNode, 0);
		
		if (tNode.Get())
			args.object = tNode;
	}
	ThreadRelease();
}

/**
 * Method: TTreeDataBind::SetNode
 * Purpose: Sets the node of the Control so that when it is time to draw, the control has content to produce
 * Parameters: TrecPointer<TObjectNode> newNode - the node of the tree to depict when drawing
 * Returns: void
 */
void TTreeDataBind::SetNode(TrecPointer<TObjectNode> newNode)
{
	ThreadLock();
	mainNode = newNode;
	ThreadRelease();
}

/**
 * Method: TTreeDataBind::getLocation
 * Purpose: Reports how much space this object will actually need to draw
 * Parameters: void
 * Returns: D2D1_RECT_F -  the Rectangle of the content that would be drawn (even if it was officially allocated less space)
 */
D2D1_RECT_F TTreeDataBind::getLocation()
{
	ThreadLock();
	D2D1_RECT_F ret = location;
	if (mainNode.Get())
		ret.bottom = ret.top + 30 * (mainNode->TotalChildren() + 1);
	ThreadRelease();
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
	ThreadLock();
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
		ThreadRelease();
		return;
	}


	// We do not, so proceed
	location = r;
	updateComponentLocation();
	ThreadRelease();
	// First Check to see if we need a new scroll control

}

bool TTreeDataBind::onScroll(float x, float y)
{
	ThreadLock();
	location.left += x;
	location.right += x;
	location.top += y;
	location.bottom += y;
	ThreadRelease();

	return true;
}

TrecPointer<TObjectNode> TTreeDataBind::GetNode()
{
	ThreadLock();
	auto ret = mainNode;
	ThreadRelease();
	return ret;
}
