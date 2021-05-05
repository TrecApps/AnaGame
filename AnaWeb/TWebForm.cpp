#include "TWebForm.h"

TWebForm::TWebForm(TrecPointer<DrawingBoard> board) : TWebNode(board)
{
    doGet = true; // GET is default
}

UINT TWebForm::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window)
{
    // For now, just call parent
    return TWebNode::CreateWebNode(location, d3dEngine, window);
}


TWebInput::TWebInput(TrecPointer<DrawingBoard> board) : TWebNode(board)
{
    inputType = web_input_type::wit_text; // Text is the default
}

UINT TWebInput::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window)
{
    return TWebNode::CreateWebNode(location, d3dEngine, window);
}
