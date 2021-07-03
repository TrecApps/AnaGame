#include "T2DSpriteEngine.h"
#include <TFile.h>

T2DSprite::T2DSprite(TDataArray<TrecSubPointer<TBrush, TBitmapBrush>>& brushes, UINT index)
{
    tilt = 0.0f;
    this->index = index;
    brush = brushes.at(index);
    collisionUnit = 0;
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

UINT T2DSprite::GetCollisionUnit()
{
    return collisionUnit;
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

UINT T2DSpriteEngine::Initialize(TDataArray<TrecPointer<TFileShell>>& files)
{
    if (files.Size() < 2)
        return 1;

    for (UINT Rust = 0; files.Size(); Rust++)
        if (!files[Rust].Get())
            return 2;

    TFile layout(files[0]->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);

    if (!layout.IsOpen())
        return 3;
    auto rec = D2D1::RectF(0.0f, 0.0f, 500, 500);

    TDataArray<TrecSubPointer<TBrush, TBitmapBrush>> brushes;

    for (UINT Rust = 1; Rust < files.Size(); Rust++)
    {
        TFile pic(files[Rust]->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);
        if (!pic.IsOpen())
            return 4;
        pic.Close();
        TrecSubPointer<TBrush, TBitmapBrush> brush = board->GetBrush(files[Rust], rec);

        if (!brush.Get() || !brush->IsValid())
            return 5;

        brushes.push_back(brush);
    }

    TString value1, value2;

    int iRows, iColumns;

    if (!layout.ReadString(value1) || !layout.ReadString(value2))
        return 6;

    if (value1.ConvertToInt(iRows) || value2.ConvertToInt(iColumns))
        return 7;

    rows = iRows;
    columns = iColumns;

    do
    {
        if (!layout.ReadString(value1))
            return 8;
    } while (!value1.GetSize());

    // Here, we have arrived at the rows
    UINT rowCount = 1;
    do
    {
        TrecPointer<TDataArray<TString>> numbers = value1.split(L" ");
        int val;
        for (UINT Rust = 0; Rust < numbers->Size(); Rust++)
        {
            if (!numbers->at(Rust).GetSize())
            {
                numbers->RemoveAt(Rust--);
                continue;
            }
            TrecPointer<TDataArray<TString>> data = numbers->at(Rust).split(L";");

            if (data->at(0).ConvertToInt(val))
                return 9;

            if (static_cast<UINT>(val) >= brushes.Size())
                return 10;

            TrecPointer<T2DSprite> sprite = TrecPointerKey::GetNewTrecPointer<T2DSprite>(brushes, val);
            if (data->Size() > 1)
            {
                if (data->at(1).ConvertToInt(val))
                    return 11;
                sprite->setCollisionUnit(val);
            }
            sprites.push_back(sprite);
        }

        if (numbers->Size() != columns)
            return 12;
        rowCount++;
        if (!layout.ReadString(value1))
            return 13;
    } while (value1.GetSize());

    if (rowCount != rows)
        return 14;

    do
    {
        if (!layout.ReadString(value1))
            return 0;
    } while (!value1.GetSize());

    do
    {
        TrecPointer<TDataArray<TString>> numbers = value1.split(L";");
        int moveRow, moveCol, moveSprite, moveColl;
        moveRow = moveCol = moveSprite = moveColl = 0;
        float tilt;

        if (numbers->Size() < 4)
            return 15;

        if (numbers->at(0).ConvertToInt(moveRow) && numbers->at(1).ConvertToInt(moveCol) && numbers->at(2).ConvertToInt(moveSprite) && numbers->at(3).ConvertToInt(moveColl))
            return 16;

        TrecPointer<T2DSprite> sprite = TrecPointerKey::GetNewTrecPointerAlt<T2DSprite, T2DMovableSprite>(brushes, moveSprite, TrecPointerKey::GetTrecPointerFromSoft<T2DSpriteEngine>(self));
        sprite->setCollisionUnit(moveColl);
        dynamic_cast<T2DMovableSprite*>(sprite.Get())->SetPosition(moveCol, moveRow);

        if (numbers->Size() >= 5)
        {
            if (numbers->at(4).ConvertToFloat(tilt))
                return 17;
            sprite->SetTiltDegrees(tilt);
        }

    } while (layout.ReadString(value1));

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

                D2D1_RECT_F miniLoc = { 0,0,0,0 };

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

bool T2DSpriteEngine::GetCollisionSprite(UINT row, UINT column, UINT& result)
{
    if (row >= rows || column >= columns)
        return false;

    auto index = (row * rows) + column;
    if(index >= sprites.Size())
        return false;

    auto sprite = sprites[index];

    if (sprite.Get())
    {
        result = sprite->GetCollisionUnit();
        return true;
    }
    return false;
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
    if (!engine.Get())
        return false;
    objColl = collisionUnit;

    if (horizontal == 0.0f && vertical == 0.0f)
    {
        return engine->GetCollisionSprite(baseRow, baseColumn, envCol);
    }
    UINT newColumn = baseColumn + horizontal;
    UINT newRow = baseRow + vertical;
    if (throughWall)
    {
        return engine->GetCollisionSprite(newRow, newColumn, envCol);
    }
    else
    {
        UINT start = 0;
        envCol = start;
        UINT min = min(newColumn, baseColumn);
        UINT max = max(newColumn, baseColumn);
        for (UINT Rust = min; Rust <= max; Rust++)
        {
            if (!engine->GetCollisionSprite(Rust, baseColumn, start))
                return false;

            if (start > envCol)
                envCol = start;
        }

        min = min(newRow, baseRow);
        max = max(newRow, baseRow);

        for (UINT Rust = min; Rust <= max; Rust++)
        {
            if (!engine->GetCollisionSprite(newColumn, Rust, start))
                return false;

            if (start > envCol)
                envCol = start;
        }
        return true;
    }
}
