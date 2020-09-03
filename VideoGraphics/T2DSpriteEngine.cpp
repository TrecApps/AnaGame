#include "T2DSpriteEngine.h"

T2DSprite::T2DSprite(TDataArray<TrecSubPointer<TBrush, TBitmapBrush>>& brushes, UINT index)
{
}

void T2DSprite::SetTiltDegrees(float degrees)
{
}

float T2DSprite::GetTiltDegrees()
{
    return 0.0f;
}

void T2DSprite::OnDraw(D2D1_RECT_F& loc)
{
}

UINT T2DSprite::GetIndex()
{
    return 0;
}

T2DSpriteEngine::T2DSpriteEngine(TrecPointer<DrawingBoard>)
{
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
}
