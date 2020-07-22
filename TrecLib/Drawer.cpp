#include "Drawer.h"

/**
 * Method: Drawer::GetWindowHandle
 * Purpose: Returns the Window handle
 * Parameters: void
 * Returns: HWND - the handle to the Window object in the OS
 */
HWND Drawer::GetWindowHandle()
{
	return currentWindow;
}

/**
 * Method: Drawer::GetFactory
 * Purpose: Returns the Direct2D Factory used by the window
 * Parameters: void
 * Returns: TrecComPointer<ID2D1Factory1> -  the Pointer to the factory
 */
TrecComPointer<ID2D1Factory1> Drawer::GetFactory()
{
	return directFactory;
}
