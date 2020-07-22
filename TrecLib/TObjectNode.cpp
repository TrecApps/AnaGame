#include "TObjectNode.h"
#include "TString.h"

/*
* Method: TObjectNode::TObjectNode
* Purpose: Constructor
* Parameters: UINT l - the level (i.e. distance) from the head node 
* Returns: void
*/
TObjectNode::TObjectNode(UINT l)
{
	level = l;
}

/*
* Method: TObjectNode::~TObjectNode
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TObjectNode::~TObjectNode()
{
}

/*
* Method: TObjectNode::GetLevel
* Purpose: Retrieves the Level of this particular node
* Parameters: void
* Returns: UINT - the distance from the head node
*/
UINT TObjectNode::GetLevel()
{
	return level;
}

/*
* Method: TObjectNode::SetParent
* Purpose: Sets the parent node of this particular node
* Parameters: TrecPointerSoft<TObjectNode> p - reference to the parent
* Returns: void
*
* Note: THe parameter uses the Soft Pointer to prevent a circular reference
*/
void TObjectNode::SetParent(TrecPointerSoft<TObjectNode> p)
{
	parentNode = p;
}

/*
* Method: TObjectNode::SetSelf
* Purpose: Sets the objects reference to itself
* Parameters: TrecPointer<TObjectNode> s - self reference
* Returns: void
*/
void TObjectNode::SetSelf(TrecPointer<TObjectNode> s)
{
	if (s.Get() != this)
		throw L"Error! Pointer needs to reference 'this' object!";
	self = TrecPointerKey::GetSoftPointerFromTrec<TObjectNode>(s);
}


/**
 * Method: TObjectNode::getVariableValueStr
 * Purpose: Gets Value String
 * Parameters: const TString& varName - not used here, but declared in TObject version of this method
 * Returns: TString
 */
TString TObjectNode::getVariableValueStr(const TString& varName)
{
	return TString(L"{}");
}
