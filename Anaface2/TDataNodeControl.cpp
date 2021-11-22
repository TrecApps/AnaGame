#include "TDataNodeControl.h"
#include <TScrollerPage.h>
#include <TStringVariable.h>

TrecPointer<TObjectNode> TDataNodeControl::GetNode()
{
    return mainNode;
}

TDataNodeControl::TDataNodeControl(TrecPointer<DrawingBoard> board, TrecPointer<TArray<styleTable>> ta) : TRandomLayout(board, ta)
{
    isNodeSelected = isTickSelected = false;
    nodeSelected = 0;
    highlightNodeSelected = UINT32_MAX;

    blockExpansion = false;
}

void TDataNodeControl::SetNode(TrecPointer<TObjectNode> newNode)
{
	mainNode = newNode;
}

bool TDataNodeControl::OnScroll(bool b, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>& args)
{
    return TControl::OnScroll(b, point, direction, args);
}

void TDataNodeControl::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&eventAr)
{
	if (IsContained(point, GetArea()) && mainNode.Get())
	{
		TControl::OnLButtonDblClk(nFlags, point, mOut, eventAr);
		float dist = point.y - location.top;
		UINT targetNode = static_cast<UINT>(dist) / 30;
		TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(targetNode, 0);

		if (tNode.Get())
			args.object = tNode;
	}
}

void TDataNodeControl::OnResize(D2D1_RECT_F& r, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&)
{
	D2D1_RECT_F tempLoc = this->GetArea();
	if ((tempLoc.bottom - tempLoc.top > r.bottom - r.top) ||
		(tempLoc.right - tempLoc.left > r.right - r.left))
	{
		auto activeParent = TrecPointerKey::GetTrecPointerFromSoft<>(parent);
		if (activeParent.Get() && !activeParent->IsScroller())
		{
			activeParent->InjectScrollerPage(r, tempLoc, TrecPointerKey::GetTrecPointerFromSoft<>(self));
		}
	}
	location = r;
}

void TDataNodeControl::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	if (IsContained(point, GetArea()))
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
					if (IsContained(point, triLoc) && isTickSelected && !blockExpansion)
					{
						if (tNode->IsExtended())
							tNode->DropChildNodes();
						else if (tNode->IsExtendable())
						{
							tNode->Extend();
							OnResize(location, 0, eventAr,args);
						}


					}
					else
					{
						int eIndex = HasEvent(R_Message_Type::On_sel_change);
						if (eIndex != -1)
						{
							this->args.eventType = R_Message_Type::On_sel_change;
							this->args.point = point;
							this->args.methodID = eIndex;
							this->args.isClick = true;
							this->args.isLeftClick = false;
							this->args.object = tNode;

							args.push_back(this->args);
						}
					}

					highlightNodeSelected = nodeSelected;
				}
			}
		}
		TControl::OnLButtonUp(nFlags, point, mOut, eventAr, args);
	}
	isTickSelected = isNodeSelected = false;
	nodeSelected = 0;
}

void TDataNodeControl::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	if (IsContained(point, GetArea()))
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

				if (IsContained(point, triLoc))
				{
					isTickSelected = true;

				}
				isNodeSelected = true;
				nodeSelected = targetNode;
			}
		}
		TControl::OnLButtonDown(nFlags, point, mOut, eventAr, args);
	}
}


bool TDataNodeControl::onCreate(const D2D1_RECT_F& r, TrecPointer<TWindowEngine> d3d)
{
	TControl::onCreate(r, d3d);
	if (drawingBoard.Get())
	{
		outerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Black));
		innerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Wheat));

		TString valpoint; 

		TColor nodeColor( 1.0f,1.0f,1.0f, 0.0f );

		if (attributes.retrieveEntry(L"|NodeHighlightColor", valpoint))
		{
			bool w;
			nodeColor = TColor::GetColorFromString(valpoint, w);
		}

		nodeBrush = drawingBoard->GetBrush(TColor(nodeColor));

		if (attributes.retrieveEntry(L"|BlockExtension", valpoint) && !valpoint.CompareNoCase(L"true"))
		{
			blockExpansion = true;
		}

	}
	if (childControls.Size() && dynamic_cast<TControl*>(childControls.at(0).control.Get()))
	{
		dynamic_cast<TControl*>(childControls.at(0).control.Get())->onCreate(r, d3d);
	}
    return false;
}

void TDataNodeControl::Draw(TrecPointer<TVariable> object)
{
	TObjectLocker lock(&thread);
	if (!mainNode.Get())
	{
		return;
	}
	TrecPointer<TPage> cont = childControls.at(0).control;
	if (!cont.Get())
	{
		return;
	}
	TrecPointer<TObjectNode> curNode = mainNode;

	D2D1_RECT_F cLoc = location;
	cLoc.bottom = cLoc.top + 30;

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;

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
		cont->OnResize(triLoc, 0, cred, args);
		cont->Draw(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(curNode->GetContent()));

		if (nodeBrush.Get() && c == highlightNodeSelected)
			nodeBrush->FillRectangle(cLoc);

		// Prepare for the next draw
		cLoc.bottom += 30;
		cLoc.top += 30;

		curNode = mainNode->GetNodeAt(c + 1, 0);
	}
}

D2D1_RECT_F TDataNodeControl::GetArea()
{
	D2D1_RECT_F ret = area;
	if (mainNode.Get())
		ret.bottom = ret.top + 30 * (mainNode->TotalChildren() + 1);
	return ret;
}
