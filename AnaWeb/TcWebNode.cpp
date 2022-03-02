#include "TcWebNode.h"

TcWebNode::NodeContainerType TcWebNode::TcTextNodeElement::GetElementType()
{
    return NodeContainerType::nct_text;
}

D2D1_RECT_F TcWebNode::TcTextNodeElement::GetLocation()
{
    return D2D1_RECT_F();
}

TcWebNode::NodeContainerType TcWebNode::TcWebNodeElement::GetElementType()
{
    return NodeContainerType::nct_web;
}

D2D1_RECT_F TcWebNode::TcWebNodeElement::GetLocation()
{
    return D2D1_RECT_F();
}

TcWebNode::TcWebNode(TrecPointer<DrawingBoard> board): TPage(board)
{
}

bool TcWebNode::HandlesEvents()
{
    return false;
}

void TcWebNode::Draw(TrecPointer<TVariable> object)
{
}

ag_msg void TcWebNode::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& eventAr)
{

}

ag_msg void TcWebNode::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{

}

ag_msg bool TcWebNode::OnDestroy()
{
    return true;
}

ag_msg bool TcWebNode::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&)
{
    return false;
}

UINT TcWebNode::ProcessHtml(TStringSliceManager& html, UINT& start, HWND win)
{
    return 0;
}

UINT TcWebNode::ProcessInnerHtml(TStringSliceManager& html, UINT& start, HWND win)
{
    return 0;
}


