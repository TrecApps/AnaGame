#pragma once
#include "TObjectNode.h"

#include "TDataArray.h"
#include "TString.h"
/**
 * Class: TBlankNode
 * Purpose: Provides a concrete TObject Node that can hold any type of node
 * 
 * SuperClass: TObjectNode - Allows TBlankNode to be used by the TTreeDataBind control
 */
class _TREC_LIB_DLL TBlankNode : public TObjectNode
{
public:
	/**
	 * Method: TBlankNode::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/*
	* Method: TBlankNode::TBlankNode
	* Purpose: Constructor
	* Parameters: UINT l - the level (i.e. distance) from the head node
	* Returns: void
	*/
	TBlankNode(UINT l);

	/*
	* Method: TBlankNode::~TBlankNode
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~TBlankNode();


	/**
	 * Method: TBlankNode::GetContent
	 * Purpose: Retrievs the "content" of the node
	 * Parameters: void
	 * Returns: TString - the "content" of the node
	 * 
	 * Attributes: virtual;override
	 */
	virtual TString GetContent()override;
	/**
	 * Method: TBlankNode::IsExtendable
	 * Purpose: Whether the ndoe can be extended
	 * Parameters: void
	 * Returns: bool - whether the node can be extended
	 * 
	 * Attributes: virtual;override
	 */
	virtual bool IsExtendable()override;
	/**
	 * Method: TBlankNode::IsExtended
	 * Purpose: Whether the node currently has children or not
	 * Parameters: void
	 * Returns: bool - whether the node has children or not
	 * 
	 * Attributes: virtual;override
	 */
	virtual bool IsExtended()override;
	/**
	 * Method: TBlankNode::Initialize
	 * Purpose: Allows for basic initialiation
	 * Parameters: void
	 * Returns: bool - whether initialization worked
	 * 
	 * Attributes: virtual;override
	 */
	virtual bool Initialize()override;
	/**
	 * Method: TBlankNode::GetNodeAt
	 * Purpose: Retrieves the node at the specified location
	 * Parameters: UINT target - the intended target of the node
	 *			UINT current - lets this node know what location it is in
	 * 
	 * Attributes: virtual;override
	 * Returns: TrecPointer<TObjectNode> - the requested node (null if it doesn't exist)
	 */
	virtual TrecPointer<TObjectNode> GetNodeAt(UINT target, UINT current)override;
	/**
	 * Method: TBlankNode::TotalChildren
	 * Purpose: Reports the total number of "decendants" of this node
	 * Parameters: void
	 * Returns: UINT - total "decendant" count
	 * 
	 * Attributes: virtual;override
	 */
	virtual UINT TotalChildren()override;

	/**
	 * Method: TBlankNode::Initialize
	 * Purpose: Initializes the data of the object based off of the provided string
	 * Parameters: TString& value -  info on how to initialize
	 * Returns: bool - whether initialization was successful
	 * 
	 * Attributes: virtual;override
	 */
	virtual bool Initialize(TString& value)override;
	/**
	 * Method: TBlankNode::Extend
	 * Purpose: Extends the node so that it has child nodes (if possible)
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: virtual;override
	 */
	virtual void Extend()override;
	/**
	 * Method: TBlankNode::GetChildNodes
	 * Purpose: Retirevs the Child node at the given index
	 * Parameters: UINT location of the target node
	 * Returns: TrecPointer<TObjectNode> - the targeted node
	 * 
	 * Attributes: virtual;override
	 */
	virtual TrecPointer<TObjectNode> GetChildNodes(UINT)override;
	/**
	 * Method: TBlankNode::DropChildNodes
	 * Purpose: Drops any child nodes this node currently holds
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: virtual;override
	 */
	virtual void DropChildNodes()override;

	/**
	 * Method: TBlankNode::AddNode
	 * Purpose: Adds a node to this blank one
	 * Parameters: TrecPointer<TObjectNode> node - the node to insert
	 * Returns: void
	 */
	void AddNode(TrecPointer<TObjectNode> node);

protected:

	/**
	 * The 'content' of this node
	 */
	TString content;

	/**
	 * because this is blank, this is an easier way to report whether the node is extended or not
	 */
	bool isExtended;

	/**
	 * list of files held by this node (if object is a TDirectory
	 */
	TDataArray<TrecPointer<TObjectNode>> nodes;
};

