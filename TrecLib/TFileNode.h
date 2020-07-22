#pragma once
#include "TObjectNode.h"
#include "TFileShell.h"

/*
 * class TFileNode
 * Purpose: Implements the TObjectNode interface for Files found on the local hard drive
 */
class _TREC_LIB_DLL TFileNode :
	public TObjectNode
{
public:
	/*
	* Method: TDirectory::Constructor
	* Purpose: Constructs the TFileNode
	* Parameters: UINT l - level of the node (used by the TObjectNode class)
	* Returns: void
	*/
	TFileNode(UINT l);

	/*
	 * Method: TDirectory::GetContent
	 * Purpose: Retrieves the Content to present about this object
	 * Parameters: void
	 * Returns: TString - the name of the File/directory this node references
	 */
	TString GetContent() override;

	/*
	* Method: TDirectory::IsExtendable
	* Purpose: Reports whether this node is extendable - in this case, whether the file is a directory or not
	* Parameters: void
	* Returns: bool - whether the node is extendable (i.e. references a directory)
	*/
	bool IsExtendable() override;

	/*
	* Method: TDirectory::IsExtended
	* Purpose: Reports whether this node is currently extended
	* Parameters: void
	* Returns: bool - whether the node is extended
	*/
	virtual bool IsExtended();

	/*
	* Method: TDirectory::GetNodeAt
	* Purpose: Return a specific node, with an assumption on where "this" node is in the heirarchy
	* Parameters: UINT target - the desired node
	*				UINT current - the node that this node should assume to be
	* Returns: TrecPointer<TObjectNode> - the desired node (null if the node is not in this nodes reach)
	*/
	virtual TrecPointer<TObjectNode> GetNodeAt(UINT target, UINT current);

	/*
	* Method: TDirectory::TotalChildren
	* Purpose: Reports Reports the total number of children, grand children, and so on held by this node
	* Parameters: void
	* Returns: UINT - number of nodes for which this node is an ancestor
	*/
	virtual UINT TotalChildren();

	/*
	* Method: TDirectory::Initialize
	* Purpose: Supposed to initialize the node tree
	* Parameters: void
	* Returns: bool - false as the Node cannot be initialized without the path provided
	*/
	virtual bool Initialize() override;

	/*
	* Method: TDirectory::Initialize
	* Purpose: Initializes the node with the path of the file/Directory to start with
	* Parameters: TStrng& value - the path of the File to reference
	* Returns: bool - true of a file object was generated, false otherwise
	*/
	virtual bool Initialize(TString& value) override;

	/*
	* Method: TDirectory::Extend
	* Purpose: Epands the Tree by getting all of the files in the directory
	* Parameters: void
	* Returns: void
	*
	* Note: This method only functions of the file in question is a directory, this can be determined via the IsExtendable method
	*/
	virtual void Extend() override;

	/*
	* Method: TDirectory::GetChildNodes
	* Purpose: Retrieves Child Nodes one at a time via an index.
	* Parameters: UINT index - the index of the child node to retrieve
	* Returns: TrecPointer<TObjectNode> - the node in question
	*
	* Note: Pointer returned is null if the index is too high. To browse the tree, start the index at 0 and once the result is null, you can be sure there are no more nodes
	*/
	virtual TrecPointer<TObjectNode> GetChildNodes(UINT index) override;

	/*
	* Method: TDirectory::DropChildNodes
	* Purpose: Drops the child nodes, provided with a control in mind
	* Parameters: void
	* Returns: void
	*/
	virtual void DropChildNodes() override;


	/*
	* Method: TDirectory::getVariableValueStr
	* Purpose: Reports the desired Content
	* Parameters: const TString& varName - the name
	* Returns: TString - the file/Directory name held by this object
	*/
	virtual TString getVariableValueStr(const TString& varName)override;


	/*
	* Method: TDirectory::SetFile
	* Purpose: Manually Initialize a node with an existing File Record
	* Parameters: TrecPointer<TFileShell>& d - the file to reference
	* Returns: void
	*/
	void SetFile(TrecPointer<TFileShell>& d);


	/**
	 * Method: TFileNode::GetData
	 * Purpose: retireves the underlying file shell object
	 * Parameters: void
	 * Returns: TrecPointer<TFileShell> - the data the node is holding
	 */
	TrecPointer<TFileShell> GetData();
protected:
	/**
	 * the data held by this node
	 */
	TrecPointer<TFileShell> data;

	/**
	 * list of files held by this node (if object is a TDirectory
	 */
	TDataArray<TrecPointer<TObjectNode>> files;
};

