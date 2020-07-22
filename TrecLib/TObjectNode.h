#pragma once
#include "TObject.h"
#include "TrecReference.h"

/*
 * class TObjectNode
 * Purpose: Provides an Interface for which Trees of ny type of Object can be formed
 */
class _TREC_LIB_DLL TObjectNode :
	public TObject
{
public:
	/*
	* Method: TObjectNode::TObjectNode
	* Purpose: Constructor
	* Parameters: UINT l - the level (i.e. distance) from the head node
	* Returns: void
	*/
	TObjectNode(UINT l);
	/*
	* Method: TObjectNode::~TObjectNode
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TObjectNode();



	/**
	 * Method: TObjectNode::GetContent
	 * Purpose: Retrievs the "content" of the node
	 * Parameters: void
	 * Returns: TString - the "content" of the node
	 */
	virtual TString GetContent() = 0;
	/**
	 * Method: TObjectNode::IsExtendable
	 * Purpose: Whether the ndoe can be extended
	 * Parameters: void
	 * Returns: bool - whether the node can be extended
	 */
	virtual bool IsExtendable() = 0;
	/**
	 * Method: TObjectNode::IsExtended
	 * Purpose: Whether the node currently has children or not
	 * Parameters: void
	 * Returns: bool - whether the node has children or not
	 */
	virtual bool IsExtended() = 0;
	/**
	 * Method: TObjectNode::Initialize
	 * Purpose: Allows for basic initialiation
	 * Parameters: void
	 * Returns: bool - whether initialization worked
	 */
	virtual bool Initialize() = 0;
	/**
	 * Method: TObjectNode::GetNodeAt
	 * Purpose: Retrieves the node at the specified location 
	 * Parameters: UINT target - the intended target of the node
	 *			UINT current - lets this node know what location it is in
	 * Returns:
	 */
	virtual TrecPointer<TObjectNode> GetNodeAt(UINT target, UINT current) =0;
	/**
	 * Method: TObjectNode::TotalChildren
	 * Purpose: Reports the total number of "decendants" of this node
	 * Parameters: void
	 * Returns: UINT - total "decendant" count
	 */
	virtual UINT TotalChildren() = 0;

	/**
	 * Method: TObjectNode::Initialize
	 * Purpose: Initializes the data of the object based off of the provided string
	 * Parameters: TString& value -  info on how to initialize
	 * Returns: bool - whether initialization was successful
	 */
	virtual bool Initialize(TString& value) = 0;
	/**
	 * Method: TObjectNode::Extend
	 * Purpose: Extends the node so that it has child nodes (if possible)
	 * Parameters: void
	 * Returns: void
	 */
	virtual void Extend() = 0;
	/**
	 * Method: TObjectNode::GetChildNodes
	 * Purpose: Retirevs the Child node at the given index
	 * Parameters: UINT location of the target node
	 * Returns: TrecPointer<TObjectNode> - the targeted node
	 */
	virtual TrecPointer<TObjectNode> GetChildNodes(UINT) = 0;
	/**
	 * Method: TObjectNode::DropChildNodes
	 * Purpose: Drops any child nodes this node currently holds
	 * Parameters: void
	 * Returns: void
	 */
	virtual void DropChildNodes() = 0;

	/*
	* Method: TObjectNode::GetLevel
	* Purpose: Retrieves the Level of this particular node
	* Parameters: void
	* Returns: UINT - the distance from the head node
	*/
	UINT GetLevel();

	/*
	* Method: TObjectNode::SetParent
	* Purpose: Sets the parent node of this particular node
	* Parameters: TrecPointerSoft<TObjectNode> p - reference to the parent
	* Returns: void
	*
	* Note: THe parameter uses the Soft Pointer to prevent a circular reference
	*/
	void SetParent(TrecPointerSoft<TObjectNode> p);
	/*
	* Method: TObjectNode::SetSelf
	* Purpose: Sets the objects reference to itself
	* Parameters: TrecPointer<TObjectNode> s - self reference
	* Returns: void
	*/
	void SetSelf(TrecPointer<TObjectNode> s);

	/**
	 * Method: TObjectNode::getVariableValueStr
	 * Purpose: Gets Value String
	 * Parameters: const TString& varName - not used here, but declared in TObject version of this method
	 * Returns: TString
	 */
	virtual TString getVariableValueStr(const TString& varName)override;

protected:
	/**
	 * The distance from the root (0 means this is the root node)
	 */
	UINT level;

	/**
	 * Reference to the parent node
	 */
	TrecPointerSoft<TObjectNode> parentNode;
	/**
	 * Reference to self
	 */
	TrecPointerSoft<TObjectNode> self;
};

