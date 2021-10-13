#include "TDataLayout.h"

#include <TContainerVariable.h>

TDataLayout::TDataLayout(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles): TRandomLayout(drawingBoard, styles)
{
	width = 120;
	height = 60;
	stackFirst = true;
	maxPrimaryCount = -1;
	org = data_org::do_default;
}

void TDataLayout::SetData(TrecPointer<TVariable> var)
{
	this->var = var;
}

void TDataLayout::Draw(TrecPointer<TVariable> object)
{
	TControl::Draw(object);
	
	auto tempRec = area;
	tempRec.bottom = tempRec.top + height;
	tempRec.right = tempRec.left + width;

	TrecPointer<TPage> tempPage = childControls.Size() ? childControls.at(0).control : TrecPointer<TPage>();
	if (!tempPage.Get())return;

	TDataArray<EventID_Cred> cred;
	TDataArray<EventArgs> args;
	TString varName;
	TrecPointer<TVariable> varValue;
	tempPage->OnResize(tempRec, 0, cred, args);

	if (dynamic_cast<TContainerVariable*>(var.Get()))
	{
		switch (org)
		{
		case data_org::do_default:
			if (stackFirst)
			{
				for (UINT Rust = 0; dynamic_cast<TContainerVariable*>(var.Get())->GetValueAt(Rust, varName, varValue); Rust++)
				{
					
					if (dynamic_cast<TContainerVariable*>(varValue.Get()))
					{
						TrecPointer<TVariable> varValue1;
						for (UINT C = 0; dynamic_cast<TContainerVariable*>(varValue.Get())->GetValueAt(C, varName, varValue1); C++)
						{
							tempPage->Draw(varValue1);
							tempRec.left += width;
							tempRec.right += width;
							tempPage->OnResize(tempRec, 0, cred, args);
						}
						tempRec.left = area.left;
						tempRec.right = tempRec.left + width;
					}
					else
						tempPage->Draw(varValue);
					tempRec.bottom += height;
					tempRec.top += height;
				}
			}
			else
			{
				for (UINT Rust = 0; dynamic_cast<TContainerVariable*>(var.Get())->GetValueAt(Rust, varName, varValue); Rust++)
				{

					if (dynamic_cast<TContainerVariable*>(varValue.Get()))
					{
						TrecPointer<TVariable> varValue1;
						for (UINT C = 0; dynamic_cast<TContainerVariable*>(varValue.Get())->GetValueAt(C, varName, varValue1); C++)
						{
							tempPage->Draw(varValue1);
							tempRec.top += height;
							tempRec.bottom += height;
							tempPage->OnResize(tempRec, 0, cred, args);
						}
						tempRec.top = area.top;
						tempRec.bottom = tempRec.top + height;
					}
					else
						tempPage->Draw(varValue);
					tempRec.left += width;
					tempRec.right += width;
				}
			}
			break;
		case data_org::do_limit_by_space:
			for (UINT Rust = 0; dynamic_cast<TContainerVariable*>(var.Get())->GetValueAt(Rust, varName, varValue); Rust++)
			{
				tempPage->Draw(varValue);

				if (stackFirst)
				{
					tempRec.top += height;
					tempRec.bottom += height;
					if (tempRec.bottom > area.bottom)
					{
						tempRec.top = area.top;
						tempRec.bottom = tempRec.top + height;
						tempRec.left += width;
						tempRec.right += width;
					}
				}
				else
				{
					tempRec.left += width;
					tempRec.right += width;
					if (tempRec.right > area.right)
					{
						tempRec.left = area.left;
						tempRec.right = tempRec.left + width;
						tempRec.top += height;
						tempRec.bottom += height;
					}
				}
			}


			break;
		}
	}
	else
		tempPage->Draw(var);
}

bool TDataLayout::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
	TString valpoint;
	
	if (attributes.retrieveEntry("|ColumnWidth", valpoint))
	{
		int value = 0;
		if (!valpoint.ConvertToInt(value))
		{
			width = value;
		}
	}

	;
	if (attributes.retrieveEntry(L"|RowHeight", valpoint))
	{
		int value = 0;
		if (!valpoint.ConvertToInt(value))
		{
			height = value;
		}
	}

	if (attributes.retrieveEntry(L"|DefaultVertical", valpoint))
	{
		if (!valpoint.CompareNoCase(L"false") || !valpoint.CompareNoCase(L'0'))
		{
			this->stackFirst = false;
		}
	}

	if (attributes.retrieveEntry(L"|LimitPrimaryDimension", valpoint))
	{
		if (!valpoint.CompareNoCase(L"true") || valpoint.CompareNoCase(L'0'))
		{
			org = data_org::do_limit_by_space;
		}
	}


	//if (attributes.retrieveEntry(L"|MaxItemCountPrim", valpoint))
	//{
	//	if (!valpoint.ConvertToInt(this->maxPrimaryCount))
	//	{
	//		
	//	}
	//	else
	//	{
	//		maxPrimaryCount = -1;
	//	}
	//}

	return TRandomLayout::onCreate(loc, d3d);
}

D2D1_RECT_F TDataLayout::GetArea()
{
	switch (org)
	{
	case data_org::do_default:
		return GetAreaDefault();
	case data_org::do_limit_by_item_count:
		return GetAreaByCount();
	default:
		return GetAreaByContainer();
	}

}

void TDataLayout::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
	bool isCurClick = this->isRightClicked;
	TControl::OnRButtonUp(nFlags, point, mOut, cred, args);

	if (mOut == message_output::mo_negative)
		return;

	int index = HasEvent(isCurClick ? R_Message_Type::On_Right_Click : R_Message_Type::On_R_Button_Up);
	if (index != -1)
	{
		int r = 0; int c = 0;
		index = -1;
		if (GetIndex(point, r, c))
		{
			index = ConvertCoordinates(r, c);
		}
		args.at(args.Size() - 1).arrayLabel = index;
	}
}

void TDataLayout::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
	TControl::OnRButtonUp(nFlags, point, mOut, cred, args);

	if (mOut == message_output::mo_negative)
		return;

	int index = HasEvent(R_Message_Type::On_R_Button_Down);
	if (index != -1)
	{
		int r = 0; int c = 0;
		index = -1;
		if (GetIndex(point, r, c))
		{
			index = ConvertCoordinates(r, c);
		}
		args.at(args.Size() - 1).arrayLabel = index;
	}
}

void TDataLayout::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
	bool isCurClick = this->isLeftClicked;
	TControl::OnRButtonUp(nFlags, point, mOut, cred, args);

	if (mOut == message_output::mo_negative)
		return;

	int index = HasEvent(isCurClick ? R_Message_Type::On_Click : R_Message_Type::On_L_Button_Up);
	if (index != -1)
	{
		int r = 0; int c = 0;
		index = -1;
		if (GetIndex(point, r, c))
		{
			index = ConvertCoordinates(r, c);
		}
		args.at(args.Size() - 1).arrayLabel = index;
	}
}

void TDataLayout::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
	TControl::OnRButtonUp(nFlags, point, mOut, cred, args);

	if (mOut == message_output::mo_negative)
		return;

	int index = HasEvent(R_Message_Type::On_L_Button_Down);
	if (index != -1)
	{
		int r = 0; int c = 0;
		index = -1;
		if (GetIndex(point, r, c))
		{
			index = ConvertCoordinates(r, c);
		}
		args.at(args.Size() - 1).arrayLabel = index;
	}
}

void TDataLayout::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& args)
{
	bool isCurClick = this->isMouseIn;
	TControl::OnRButtonUp(nFlags, point, mOut, cred, args);

	if (mOut == message_output::mo_negative)
		return;

	int index = HasEvent(isCurClick ? R_Message_Type::On_Hover : R_Message_Type::On_Hover_Enter);
	if (index != -1)
	{
		int r = 0; int c = 0;
		index = -1;
		if (GetIndex(point, r, c))
		{
			index = ConvertCoordinates(r, c);
		}
		args.at(args.Size() - 1).arrayLabel = index;
	}
}

void TDataLayout::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>& args)
{
	TControl::OnLButtonDblClk(nFlags, point, mOut, args);

	if (mOut == message_output::mo_negative)
		return;

	int index = HasEvent(R_Message_Type::On_LDoubleClick);
	if (index != -1)
	{
		int r = 0; int c = 0;
		index = -1;
		if (GetIndex(point, r, c))
		{
			index = ConvertCoordinates(r, c);
		}
		args.at(args.Size() - 1).arrayLabel = index;
	}
}

void TDataLayout::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	bounds = newLoc;
	auto curLoc = GetArea();

	if (curLoc.left > bounds.left)
	{
		OnScroll(false, TPoint(), TPoint(curLoc.left - bounds.left, 0.0f), args);
	}
	if (curLoc.top > bounds.top)
	{
		OnScroll(false, TPoint(), TPoint(0.0f, curLoc.top - bounds.top), args);
	}

	bounds = newLoc;

	if (curLoc.right > bounds.right || curLoc.bottom > bounds.bottom)
	{
		if (parent.Get())
			TrecPointerKey::GetTrecPointerFromSoft<>(parent)->InjectScrollerPage(bounds, curLoc, TrecPointerKey::GetTrecPointerFromSoft<>(self));
	}
	area = curLoc;
}

bool TDataLayout::GetIndex(const TPoint& point, int& row, int& col)
{
	if (!IsContained(point, area))
		return false;

	TPoint nPoint(point.x - area.left, point.y - area.top);

	col = static_cast<int>(nPoint.x) / width;
	row = static_cast<int>(nPoint.y) / height;

	return true;
}

D2D1_RECT_F TDataLayout::GetAreaDefault()
{
	D2D1_RECT_F ret = area;
	UINT actHeight = height;
	UINT actWidth = width;
	if (stackFirst)
	{
		actHeight *= (var.Get())->GetSize();
		UINT maxWidth = 1;
		TString key;
		TrecPointer<TVariable> value;
		for (UINT Rust = 0; dynamic_cast<TContainerVariable*>(var.Get())->GetValueAt(Rust, key, value); Rust++)
		{
			if (dynamic_cast<TContainerVariable*>(value.Get()))
				maxWidth = max(maxWidth, value->GetSize());
		}

		actWidth *= maxWidth;
	}
	else
	{
		actWidth *= (var.Get())->GetSize();
		UINT maxHeight = 1;
		TString key;
		TrecPointer<TVariable> value;
		for (UINT Rust = 0; dynamic_cast<TContainerVariable*>(var.Get())->GetValueAt(Rust, key, value); Rust++)
		{
			if (dynamic_cast<TContainerVariable*>(value.Get()))
				maxHeight = max(maxHeight, value->GetSize());
		}

		actHeight *= maxHeight;
	}

	ret.bottom = ret.top + actHeight;
	ret.right = ret.left + actWidth;
	return ret;
}

D2D1_RECT_F TDataLayout::GetAreaByContainer()
{
	D2D1_RECT_F ret = area;

	UINT primLim = stackFirst ? ((ret.bottom - ret.top) / height) : ((ret.right - ret.left) / width);

	if (!primLim)
		primLim = 1;

	UINT secCount = 1;

	if (dynamic_cast<TContainerVariable*>(var.Get()))
	{
		UINT totalCount = var->GetSize();

		secCount = totalCount / primLim + (totalCount % primLim ? 1 : 0);
	}


	if (stackFirst)
	{
		ret.bottom = ret.top + (primLim * height);
		ret.right = ret.left + (secCount * width);
	}
	else
	{
		ret.bottom = ret.top + (secCount * height);
		ret.right = ret.left + (primLim * width);
	}
	return ret;
}

D2D1_RECT_F TDataLayout::GetAreaByCount()
{
	return D2D1_RECT_F();
}

int TDataLayout::ConvertCoordinates(int row, int col)
{
	if (org == data_org::do_default)
	{
		return stackFirst ? row : col;
	}
	else if (org == data_org::do_limit_by_space)
	{
		UINT primLim = stackFirst ? ((area.bottom - area.top) / height) : ((area.right - area.left) / width);

		if (stackFirst)
		{
			return (col * primLim) + row;
		}
		else
			return (row * primLim) + col;
	}



	return -1;
}
