#pragma once
#include "TObject.h"
#include "TrecReference.h"
#include <d2d1_1.h>

/*
 * class Drawer
 * Provides a low-level interface for the Window class so that libraries below the Window class could still call upon them to draw
 */
class _TREC_LIB_DLL Drawer :
	public TObject
{
public:
    /**
     * Method: Drawer::Draw
     * Purpose: Draws to the screen
     * Parameters: void
     * Returns: void
     *
     * Note: Abstract as the resources to draw are not present at this class level
     */
	virtual void Draw()=0;

    /**
     * Method: Drawer::GetWindowHandle
     * Purpose: Returns the Window handle
     * Parameters: void
     * Returns: HWND - the handle to the Window object in the OS
     */
    HWND GetWindowHandle();

    /**
     * Method: Drawer::GetFactory
     * Purpose: Returns the Direct2D Factory used by the window
     * Parameters: void
     * Returns: TrecComPointer<ID2D1Factory1> -  the Pointer to the factory
     */
    TrecComPointer<ID2D1Factory1> GetFactory();

protected:
    /**
     * the Direct2D factory used by this drawer
     */
    TrecComPointer<ID2D1Factory1> directFactory;

    /**
     * parent: the Handle to the window owning this window
     * currentWindow: the Handle to the window
     */
    HWND parent, currentWindow;
};

