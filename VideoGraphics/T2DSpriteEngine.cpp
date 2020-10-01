#include "T2DSpriteEngine.h"

T2DSprite::T2DSprite(TDataArray<TrecSubPointer<TBrush, TBitmapBrush>>& brushes, UINT index)
{
    tilt = 0.0f;
    this->index = index;
    brush = brushes.at(index);
}

void T2DSprite::SetTiltDegrees(float degrees)
{
    tilt = degrees;
}

float T2DSprite::GetTiltDegrees()
{
    return tilt;
}

void T2DSprite::OnDraw(D2D1_RECT_F& loc, TrecPointer<DrawingBoard> board)
{
    if (!brush.Get() || ! board.Get())
        return;

    if (tilt)
    {

        D2D1_POINT_2F center;

        center.x = (loc.right - loc.left) / 2 + loc.left;
        center.y = (loc.bottom - loc.top) / 2 + loc.top;

        D2D1_MATRIX_3X2_F curTransform;

        assert(board->GetTransform(curTransform));

        board->SetTransform(curTransform * D2D1::Matrix3x2F::Rotation(tilt, center));

        brush->FillRectangle(loc);

        board->SetTransform(curTransform);
    }
    else
        brush->FillRectangle(loc);
}

UINT T2DSprite::GetIndex()
{
    return index;
}

TString T2DSpriteEngine::GetType()
{
    return TString(L"T2DSpriteEngine;") + TObject::GetType();
}

T2DSpriteEngine::T2DSpriteEngine(TrecPointer<DrawingBoard> board)
{
    this->board = board;
    wholeSprites = 0;
    rows = columns = centerRow = centerColumn = 0;
    location = { 0.0f,0.0f,0.0f,0.0f };
}

T2DSpriteEngine::~T2DSpriteEngine()
{
}

UINT T2DSpriteEngine::Initialize(TDataArray<TFileShell>& files)
{
    return 0;
}

void T2DSpriteEngine::SetCenter(UINT row, UINT column)
{
}

void T2DSpriteEngine::Draw(D2D1_RECT_F& space)
{
    // if we don't have a board, then we can't 
    if (!board.Get())
        return;

    board->AddLayer(space);

    UINT whole = wholeSprites;

    UINT currentColumn = 0;
    UINT currentRow = 0;


    for (UINT Rust = 0; Rust < sprites.Size(); Rust++)
    {
        if (whole)
        {
            if (sprites[Rust].Get())
            {
                sprites[Rust]->OnDraw(location);
            }

            whole--;
        }
        else
        {
            if (sprites[Rust].Get())
            {
                float spriteWidth = (location.right - location.left) / static_cast<float>(columns);
                float spriteHeight = (location.bottom - location.top) / static_cast<float>(rows);

                D2D1_RECT_F miniLoc;

                miniLoc.top = location.top + (spriteHeight * currentRow);
                miniLoc.bottom = miniLoc.top + spriteHeight;

                miniLoc.left = location.left + (spriteWidth * currentColumn);
                miniLoc.right = miniLoc.right + spriteWidth;


                sprites[Rust]->OnDraw(miniLoc);
            }
            if (++currentColumn >= columns)
            {
                currentRow++;
                currentColumn = 0;
            }
        }
    }





    board->PopLayer();
}
