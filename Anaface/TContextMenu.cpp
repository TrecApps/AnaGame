
#include "TContextMenu.h"

/*
* Method: (TContextMenu) (Constructor) 
* Purpose: Sets up the context menu with drawing resources
* Parameters:  TrecPointer<DrawingBoard> rt - the render target to draw to
*				TrecPointer<TArray<styleTable>> ta - the style list for Anaface
* Returns: void
*/
TContextMenu::TContextMenu(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta):TFlyout(TrecPointer<TControl>())
{

}

/*
* Method: (TContextMenu) (Destructor)
* Purpose: Cleans up Context Menu
* Parameters: void
* Returns: void
*/
TContextMenu::~TContextMenu()
{
}


/*
* Method: TContextMenu - addOption
* Purpose: Adds an option to the Context Menu
* Parameters: TrecPointer<TString> c - the option to add
* Returns: bool - false
*/
bool TContextMenu::addOption(TrecPointer<TString> c)
{
	return false;
}



/*
* Method: TContextMenu - onDraw
* Purpose: Draws the Context Menu at a specific point
* Parameters: CPoint cp - the location to draw the context menu
* Returns: void
*/
void TContextMenu::onDraw(TPoint cp)
{

}


