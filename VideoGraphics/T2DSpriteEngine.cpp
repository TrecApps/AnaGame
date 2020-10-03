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

void T2DSprite::setCollisionUnit(UINT unit)
{
    collisionUnit = unit;
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

void T2DSpriteEngine::SetCenter(UINT row, UINT column, D2D1_RECT_F& space)
{
    if (!columns || !rows)
        return;

    D2D1_RECT_F margin;

    float locWidth = location.right - location.left;
    float locHeight = location.bottom - location.top;

    float spriteWidth = locWidth / static_cast<float>(columns);
    float spriteHeight = locHeight / static_cast<float>(rows);

    float spaceWidth = space.right - space.left;
    float spaceHeight = space.bottom - space.top;

    location.left = (space.left + (spaceWidth / 2.0f) - (spriteWidth / 2.0f + (spriteWidth * column)));

    location.top = (space.top + (spaceHeight / 2.0f) - (spriteHeight / 2.0f + (spriteHeight * row)));

    if (location.left > space.left)
        location.left = space.left;

    if (location.top > space.top)
        location.top = space.top;

    location.right = location.left + locWidth;
    location.bottom = location.top + locHeight;
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
                sprites[Rust]->OnDraw(location, board);
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


                sprites[Rust]->OnDraw(miniLoc, board);
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

void T2DSpriteEngine::SetSelf(TrecPointer<T2DSpriteEngine> self)
{
    if (self.Get() != this)
        throw L"Expected Self Pointer to point to this!";
    this->self = TrecPointerKey::GetSoftPointerFromTrec<T2DSpriteEngine>(self);
}

T2DMovableSprite::T2DMovableSprite(TDataArray<TrecSubPointer<TBrush, TBitmapBrush>>& brushes, UINT index, TrecPointer<T2DSpriteEngine> engine)
    : T2DSprite(brushes, index)
{
    this->engine = engine;
}

void T2DMovableSprite::SetSize(float height, float width)
{
    this->height = height;
    this->width = width;
}

void T2DMovableSprite::SetPosition(UINT column, UINT row)
{
    baseColumn = column;
    baseRow = row;
}

void T2DMovableSprite::removeWholeUnits()
{
    wholeUnits = false;
}

bool T2DMovableSprite::AttemptMove(float horizontal, float vertical, bool throughWall, UINT& objColl, UINT& envCol)
{
    return false;
}
