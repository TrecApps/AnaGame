#include "TGrid.h"

bool TGrid::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
    return TRandomLayout::onCreate(loc, d3d);
}

TGrid::TGrid(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles): TLayout(drawingBoard, styles)
{
}

TGrid::~TGrid()
{
}

void TGrid::Draw(TrecPointer<TVariable> object)
{
    TRandomLayout::Draw(object);
}

void TGrid::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
    TControl::OnResize(newLoc, nFlags, eventAr, args);
    UINT height = newLoc.bottom - newLoc.top;
    UINT width = newLoc.right - newLoc.left;

    bool widthOverflow = ParseDimensions(primDem, primaryStack ? height : width);
    bool heightOverflow = ParseDimensions(secDem, primaryStack ? width : height);

    if (widthOverflow || heightOverflow)
    {
        // To-Do: Handle
    }

    D2D1_RECT_F cLoc = newLoc;

    TDataArray<LayoutSpace>* rows = primaryStack ? &primDem : &secDem;
    TDataArray<LayoutSpace>* cols = primaryStack ? &secDem : &primDem;

    if (rows->Size() && cols->Size())
    {
        cLoc.bottom = cLoc.top + rows->at(0).actualSpace;
        cLoc.right = cLoc.left + cols->at(0).actualSpace;

        UINT colNum = cols->Size();


        if (childControls[0].control.Get())
            childControls[0].control->OnResize(cLoc, nFlags, eventAr, args);

        for (UINT iRow = 0; iRow < rows->Size(); iRow++)
        {
            if (childControls[colNum * iRow].control.Get())
                childControls[colNum * iRow].control->OnResize(cLoc, nFlags, eventAr, args);
            for (UINT iCol = 1; iCol < cols->Size(); iCol++)
            {
                cLoc.right += primDem[iCol].actualSpace;
                cLoc.left += primDem[iCol].actualSpace;


                if (childControls[colNum * iRow + iCol].control.Get())
                    childControls[colNum * iRow + iCol].control->OnResize(cLoc, nFlags, eventAr, args);
            }

            cLoc.bottom += primDem[iRow].actualSpace;
            cLoc.top += primDem[iRow].actualSpace;
            cLoc.left = newLoc.left;
            cLoc.right = cLoc.left + cols->at(0).actualSpace;

        }
    }
}

bool TGrid::OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>& args)
{
    return TRandomLayout::OnScroll(false, point, direction, args);
}

bool TGrid::AddPage(TrecPointer<TPage> page, UINT row, UINT col, bool doOverride)
{
    TDataArray<LayoutSpace>* rows = primaryStack ? &primDem : &secDem;
    TDataArray<LayoutSpace>* cols = primaryStack ? &secDem : &primDem;

    if(row >= rows->Size() || col >= cols->Size())
        return false;

    ChildControl& cc = childControls[rows->Size() * row + col];
    if (cc.control.Get() || !doOverride)
        return false;
    childControls[rows->Size() * row + col].control = page;
    return true;
}

bool TGrid::RemovePage(UINT row, UINT col)
{
    TDataArray<LayoutSpace>* rows = primaryStack ? &primDem : &secDem;
    TDataArray<LayoutSpace>* cols = primaryStack ? &secDem : &primDem;

    if (row >= rows->Size() || col >= cols->Size())
        return false;
    childControls[rows->Size() * row + col].control.Nullify();
    return true;
}

void TGrid::GetControlCapacity(UINT& row, UINT& col)
{
    TDataArray<LayoutSpace>* rows = primaryStack ? &primDem : &secDem;
    TDataArray<LayoutSpace>* cols = primaryStack ? &secDem : &primDem;
    row = rows->Size();
    col = cols->Size();
}

int TGrid::AddRow(UINT space, bool isFlex)
{
    LayoutSpace layoutSpace;
    layoutSpace.space = layoutSpace.actualSpace = space;
    layoutSpace.isFlex = isFlex;

    int ret = primaryStack ? primDem.push_back(layoutSpace) : secDem.push_back(layoutSpace);
    RefreshChildControls(true);
    return ret;
}

int TGrid::AddCol(UINT space, bool isFlex)
{
    LayoutSpace layoutSpace;
    layoutSpace.space = layoutSpace.actualSpace = space;
    layoutSpace.isFlex = isFlex;

    int ret = primaryStack ? secDem.push_back(layoutSpace) : primDem.push_back(layoutSpace);
    RefreshChildControls(true);
    return ret;
}

void TGrid::RefreshChildControls(bool newRow)
{
    

    if (newRow)
    {
        int row = (primaryStack ? primDem.Size() : secDem.Size()) - 1;
        if (row < 0) return;
        for (UINT Rust = 0; Rust < (primaryStack ? secDem.Size() : primDem.Size()); Rust++)
        {
            ChildControl cc;
            cc.col = Rust;
            cc.row = row;
            childControls.push_back(cc);
        }
    }
    else
    {
        TDataArray<ChildControl> newControls;
        int col = (primaryStack ? secDem.Size() : primDem.Size()) - 1;
        if (col < 0) return;
        for (UINT Rust = 0; Rust < childControls.Size(); Rust++)
        {
            newControls.push_back(childControls[Rust]);
            if (childControls[Rust].col == col - 1)
            {
                ChildControl cc;
                cc.row = childControls[Rust].row;
                cc.col = col;
                newControls.push_back(cc);
            }
        }
        childControls = newControls;
    }
}
