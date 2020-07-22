#include "TDropMenu.h"


/*
* Method: (TDropMenu) (Constructor)
* Purpose: Sets up the Control
* Parameters:  TrecPointer<DrawingBoard> rt - the render target to draw to
*				TrecPointer<TArray<styleTable>> ta - the style list for Anaface
* Returns: void
*/
TDropMenu::TDropMenu(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> styles) :TControl(rt,styles,false)
{
}

/*
* Method: (TDropMenu) (Destructor)
* Purpose: Cleans up the Drop Menu
* Parameters: void
* Returns: void
*/
TDropMenu::~TDropMenu()
{
}

/*
* Method: TDropMenu - onCreate
* Purpose: Sets up the brush used to draw the nodes
* Parameters: D2D1_RECT_F l - the location on the menu
* Returns: bool - true if drawingBoard is set
*/
bool TDropMenu::onCreate(D2D1_RECT_F l, TrecPointer<TWindowEngine> d3d)
{
	if (!drawingBoard.Get())
		return false;
	dotBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF(D2D1::ColorF::Black)));
	return true;
}

/*
* Method: TDropMenu - onDraw
* Purpose: Draws the Menu
* Parameters: void
* Returns: void
*/
void TDropMenu::onDraw(TObject* obj)
{
	TControl::onDraw(obj);
	TrecPointer<DropMenuNode> currentNode = rootNode;
	if (!currentNode.Get())
		return;
	
	DrawNode(location.top, currentNode);
}


/*
* Method: TDropMenu - SetFolderAsRoot
* Purpose: Enables a menu to be set using a directory as a root
* Parameters: TString& folder - the path of the directory to bind to
* Returns: bool - success or failure
*/
bool TDropMenu::SetFolderAsRoot(TString & folder)
{
	DWORD dwAttrib = GetFileAttributes(folder.GetConstantBuffer());

	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
		return false;

	

	return false;
}

/*
* Method: TDropMenu - GetAnaGameType
* Purpose: Retrieves the AnaGame type representation of the TObject
* Parameters: void
* Returns: UCHAR* - the AnaGame representation of the TDropMenu class 
*/
UCHAR * TDropMenu::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: TDropMenu - DrawNode
* Purpose: Draws the node and any shown children it has
* Parameters: float & top - the top of the node 
*				TrecPointer<DropMenuNode> node - the Node to begin drawing
* Returns: void
*/
void TDropMenu::DrawNode(float & top, TrecPointer<DropMenuNode> node)
{
	if (!node.Get() || !text1.Get() || !dotBrush.Get())
		return;
	D2D1_RECT_F curLoc = location;
	curLoc.top = top;
	curLoc.bottom = top + nodeHeight;
	curLoc.left + 10;
	top = curLoc.bottom;

	text1->setCaption(node->caption);
	//text1->setNewLocation(curLoc);
	text1->onDraw(location);

	if (node->hasChildren)
	{
		D2D1_ELLIPSE ellipse;
		ellipse.radiusX = ellipse.radiusY = 5;
		ellipse.point.x = curLoc.left + 5;
		ellipse.point.y = top - (nodeHeight / 2);
	
		if (node->childrenActive)
		{
			dotBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			dotBrush->FillEllipse(ellipse);
			for (UINT c = 0; c < node->children.Count(); c++)
			{
				DrawNode(top, node->children.ElementAt(c));
			}
		}
		else
		{
			dotBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			dotBrush->FillEllipse(ellipse);
		}
	}
}
