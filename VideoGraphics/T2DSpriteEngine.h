#pragma once
#include <TObject.h>
#include "DrawingBoard.h"

class T2DSprite
{
public:
    T2DSprite(TDataArray<TrecSubPointer<TBrush, TBitmapBrush>>& brushes, UINT index);

    void SetTiltDegrees(float degrees);
    float GetTiltDegrees();

    void OnDraw(D2D1_RECT_F& loc, TrecPointer<DrawingBoard> board);

    UINT GetIndex();

protected:
    float tilt;
    UINT index;

    UINT collisionUnit;

    TrecSubPointer<TBrush, TBitmapBrush> brush;
};

/**
 * Class: T2DSpriteEngine
 * Purpose: Hosts a simple Sprite collection
 * 
 * SuperClass: TObject - allows access by Environment Scripts
 */
class _VIDEO_GRAPHICS T2DSpriteEngine :
    public TObject
{
public: 


    /**
     * Method: T2DSpriteEngine::GetType
     * Purpose: Returns a String Representation of the object type
     * Parameters: void
     * Returns: TString - representation of the object type
     */
    virtual TString GetType();


    T2DSpriteEngine(TrecPointer<DrawingBoard>);

    virtual ~T2DSpriteEngine();

    UINT Initialize(TDataArray<TFileShell>& files);

    void SetCenter(UINT row, UINT column, D2D1_RECT_F& space);

    void Draw(D2D1_RECT_F& space);

protected:

    TrecPointer<DrawingBoard> board;
    D2D1_RECT_F location;

    UINT rows, columns;
    UINT centerRow, centerColumn;

    UINT wholeSprites;

    TDataArray<TrecPointer<T2DSprite>> sprites;
};

