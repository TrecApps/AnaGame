#include "TBlankNode.h"

/**
 * Method: TBlankNode::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TBlankNode::GetType()
{
	return TString(L"TBlankNode;") + TObjectNode::GetType();
}

/*
* Method: TBlankNode::TBlankNode
* Purpose: Constructor
* Parameters: UINT l - the level (i.e. distance) from the head node
* Returns: void
*/

TBlankNode::TBlankNode(UINT l) : TObjectNode(l)
{
	isExtended = false;
}

/*
* Method: TBlankNode::~TBlankNode
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TBlankNode::~TBlankNode()
{
}

/**
 * Method: TBlankNode::GetContent
 * Purpose: Retrievs the "content" of the node
 * Parameters: void
 * Returns: TString - the "content" of the node
 */
TString TBlankNode::GetContent()
{
	AG_THREAD_LOCK
	TString ret(content);
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TBlankNode::IsExtendable
 * Purpose: Whether the ndoe can be extended
 * Parameters: void
 * Returns: bool - whether the node can be extended
 */
bool TBlankNode::IsExtendable()
{
	return nodes.Size();
}

/**
 * Method: TBlankNode::IsExtended
 * Purpose: Whether the node currently has children or not
 * Parameters: void
 * Returns: bool - whether the node has children or not
 */
bool TBlankNode::IsExtended()
{
	return isExtended;
}

/**
 * Method: TBlankNode::Initialize
 * Purpose: Allows for basic initialiation
 * Parameters: void
 * Returns: bool - whether initialization worked
 */
bool TBlankNode::Initialize()
{
	return true;
}

/**
 * Method: TBlankNode::GetNodeAt
 * Purpose: Retrieves the node at the specified location
 * Parameters: UINT target - the intended target of the node
 *			UINT current - lets this node know what location it is in
 * Returns:
 */
TrecPointer<TObjectNode> TBlankNode::GetNodeAt(UINT target, UINT current)
{
	if (target == current)
		return TrecPointerKey::GetTrecPointerFromSoft<TObjectNode>(self);

	if (target < current)
		return TrecPointer<TObjectNode>();

	TrecPointer<TObjectNode> ret;

	current++;
	AG_THREAD_LOCK
	for (UINT rust = 0; rust < nodes.Size(); rust++)
	{
		ret = nodes[rust]->GetNodeAt(target, current);
		if (ret.Get())
		{
			RETURN_THREAD_UNLOCK ret;
		}
		current += nodes[rust]->TotalChildren() + 1;
	}
	RETURN_THREAD_UNLOCK TrecPointer<TObjectNode>();
}


/**
 * Method: TBlankNode::Initialize
 * Purpose: Initializes the data of the object based off of the provided string
 * Parameters: TString& value -  info on how to initialize
 * Returns: bool - whether initialization was successful
 */
bool TBlankNode::Initialize(TString& value)
{
	AG_THREAD_LOCK
	if (content.GetSize())
		RETURN_THREAD_UNLOCK false;
	content.Set(value);
	RETURN_THREAD_UNLOCK true;
}

/**
 * Method: TBlankNode::Extend
 * Purpose: Extends the node so that it has child nodes (if possible)
 * Parameters: void
 * Returns: void
 */
void TBlankNode::Extend()
{
	isExtended = true;
}

/**
 * Method: TBlankNode::GetChildNodes
 * Purpose: Retirevs the Child node at the given index
 * Parameters: UINT location of the target node
 * Returns: TrecPointer<TObjectNode> - the targeted node
 */
TrecPointer<TObjectNode> TBlankNode::GetChildNodes(UINT index)
{
	AG_THREAD_LOCK
		if (index < nodes.Size())
		{
			TrecPointer<TObjectNode> ret = nodes[index];
			RETURN_THREAD_UNLOCK ret;
		}
	RETURN_THREAD_UNLOCK TrecPointer<TObjectNode>();
}

/**
 * Method: TBlankNode::DropChildNodes
 * Purpose: Drops any child nodes this node currently holds
 * Parameters: void
 * Returns: void
 */
void TBlankNode::DropChildNodes()
{
	isExtended = false;
}

/**
 * Method: TBlankNode::AddNode
 * Purpose: Adds a node to this blank one
 * Parameters: TrecPointer<TObjectNode> node - the node to insert
 * Returns: void
 */
void TBlankNode::AddNode(TrecPointer<TObjectNode> node)
{
	AG_THREAD_LOCK
	if(node.Get())
		nodes.push_back(node);
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TBlankNode::RemoveNode
 * Purpose: Removes the specified node
 * Parameteres: TrecPointer<TObjectNode> - the node to remove
 * Returns: bool - whether the node was found
 *
 * Attributes: override
 */
bool TBlankNode::RemoveNode(TrecPointer<TObjectNode> obj)
{
	if (!obj.Get())
		return false;
	AG_THREAD_LOCK
	for (UINT Rust = 0; Rust < nodes.Size(); Rust++)
	{
		auto f = nodes[Rust].Get();
		if (!f)
			continue;

		if (f == obj.Get())
		{
			nodes.RemoveAt(Rust);
			RETURN_THREAD_UNLOCK true;
		}

		if (f->RemoveNode(obj))
			RETURN_THREAD_UNLOCK true;
	}
	RETURN_THREAD_UNLOCK false;
}

/*
* Method: TBlankNode::TotalChildren
* Purpose: Reports Reports the total number of children, grand children, and so on held by this node
* Parameters: void
* Returns: UINT - number of nodes for which this node is an ancestor
*/
UINT TBlankNode::TotalChildren()
{
	UINT ret = 0;
	AG_THREAD_LOCK
	for (UINT rust = 0; rust < nodes.Size(); rust++)
	{
		ret += nodes[rust]->TotalChildren() + 1;
	}
	RETURN_THREAD_UNLOCK ret;
}