#pragma once
#include "TObjectNode.h"
#include "TFileShell.h"

/**
 * Enum Class: file_node_filter_mode
 * Purpose: Allows costomization for filering out directories that appear in these nodes
 */
typedef enum class file_node_filter_mode
{
	fnfm_blank,                   // no directory filtering
	fnfm_block_upper,             // block the upper directory
	fnfm_block_current,           // block the current directory
	fnfm_block_both,              // block both the upper and current directory
	fnfm_block_upper_and_files,   // Only show regular and current directories
	fnfm_block_current_and_files, // Only show regular and upper directories
	fnfm_block_both_and_files,    // Only show regular directories
	fnfm_block_directories,       // Only show files
	fnfm_block_files              // Show all Directories and no files
}file_node_filter_mode;

/*
 * Class: TFileNode
 * Purpose: Implements the TObjectNode interface for Files found on the local hard drive
 * 
 * SuperClass: TObjectNode - Allows TBlankNode to be used by the TTreeDataBind control
 */
class _TREC_LIB_DLL TFileNode :
	public TObjectNode
{
public:

	/**
	 * Method: TFileNode::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;


	/*
	* Method: TFileNode::Constructor
	* Purpose: Constructs the TFileNode
	* Parameters: UINT l - level of the node (used by the TObjectNode class)
	* Returns: void
	*/
	TFileNode(UINT l);

	/*
	 * Method: TFileNode::GetContent
	 * Purpose: Retrieves the Content to present about this object
	 * Parameters: void
	 * Returns: TString - the name of the File/directory this node references
	 * 
	 * Attributes: virtual;override
	 */
	TString GetContent() override;

	/*
	* Method: TFileNode::IsExtendable
	* Purpose: Reports whether this node is extendable - in this case, whether the file is a directory or not
	* Parameters: void
	* Returns: bool - whether the node is extendable (i.e. references a directory)
	 * 
	 * Attributes: virtual;override
	*/
	bool IsExtendable() override;

	/*
	* Method: TFileNode::IsExtended
	* Purpose: Reports whether this node is currently extended
	* Parameters: void
	* Returns: bool - whether the node is extended
	 * 
	 * Attributes: virtual;override
	*/
	virtual bool IsExtended()override;

	/*
	* Method: TFileNode::GetNodeAt
	* Purpose: Return a specific node, with an assumption on where "this" node is in the heirarchy
	* Parameters: UINT target - the desired node
	*				UINT current - the node that this node should assume to be
	* Returns: TrecPointer<TObjectNode> - the desired node (null if the node is not in this nodes reach)
	*/
	virtual TrecPointer<TObjectNode> GetNodeAt(UINT target, UINT current);

	/*
	* Method: TFileNode::TotalChildren
	* Purpose: Reports Reports the total number of children, grand children, and so on held by this node
	* Parameters: void
	* Returns: UINT - number of nodes for which this node is an ancestor
	*/
	virtual UINT TotalChildren();

	/*
	* Method: TFileNode::Initialize
	* Purpose: Supposed to initialize the node tree
	* Parameters: void
	* Returns: bool - false as the Node cannot be initialized without the path provided
	 * 
	 * Attributes: virtual;override
	*/
	virtual bool Initialize() override;

	/*
	* Method: TFileNode::Initialize
	* Purpose: Initializes the node with the path of the file/Directory to start with
	* Parameters: TStrng& value - the path of the File to reference
	* Returns: bool - true of a file object was generated, false otherwise
	 * 
	 * Attributes: virtual;override
	*/
	virtual bool Initialize(TString& value) override;

	/*
	* Method: TFileNode::Extend
	* Purpose: Epands the Tree by getting all of the files in the directory
	* Parameters: void
	* Returns: void
	*
	* Note: This method only functions of the file in question is a directory, this can be determined via the IsExtendable method
	 * 
	 * Attributes: virtual;override
	*/
	virtual void Extend() override;

	/*
	* Method: TFileNode::GetChildNodes
	* Purpose: Retrieves Child Nodes one at a time via an index.
	* Parameters: UINT index - the index of the child node to retrieve
	* Returns: TrecPointer<TObjectNode> - the node in question
	*
	* Note: Pointer returned is null if the index is too high. To browse the tree, start the index at 0 and once the result is null, you can be sure there are no more nodes
	 * 
	 * Attributes: virtual;override
	*/
	virtual TrecPointer<TObjectNode> GetChildNodes(UINT index) override;

	/*
	* Method: TFileNode::DropChildNodes
	* Purpose: Drops the child nodes, provided with a control in mind
	* Parameters: void
	* Returns: void
	 * 
	 * Attributes: virtual;override
	*/
	virtual void DropChildNodes() override;


	/*
	* Method: TFileNode::getVariableValueStr
	* Purpose: Reports the desired Content
	* Parameters: const TString& varName - the name
	* Returns: TString - the file/Directory name held by this object
	 * 
	 * Attributes: virtual;override
	*/
	virtual TString getVariableValueStr(const TString& varName)override;


	/*
	* Method: TFileNode::SetFile
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

	/**
	 * Method: TFileNode::SetShowAllFiles
	 * Purpose: Sets whether to show all files or whether a filter is in place
	 * Parameters: bool show - whether to show all files or only the target extensions
	 * Returns: void
	 */
	void SetShowAllFiles(bool show);


	/**
	 * Method: TFileNode::GetShowAllFiles
	 * Purpose: Reports whether to show all files or whether a filter is in place
	 * Parameters: void
	 * Returns: bool - whether to show all files or only the target extensions
	 */
	bool GetShowAllFiles();


	/**
	 * Method: TFileNode::AddExtension
	 * Purpose: Adds a file extension to report
	 * Parameters: const TString& extension - the extension to add
	 * Returns: bool - whether the extension is in the list (no if not an extension)
	 */
	bool AddExtension(const TString& extension);


	/**
	 * Method: TFileNode::SetFilterMode
	 * Purpose: Sets the filter mode of the nodes
	 * Parameters: file_node_filter_mode mode - the mode to filter
	 * Returns: void
	 */
	void SetFilterMode(file_node_filter_mode mode);


	/**
	 * Method: TFileNode::GetFilterMode
	 * Purpose: Gets the filter mode of the nodes
	 * Parameters: void
	 * Returns: file_node_filter_mode - the filter mode of the node
	 */
	file_node_filter_mode GetFilterMode();

protected:


	/**
	 * Method: TFileNode::ShouldShow
	 * Purpose: Takes the filter configuration to determine if to should be shown
	 * Parameters: TrecPointer<TFileShell> node -  the file entry to subject to filtering
	 * Returns: bool - whether the node passes the criteria to be shown
	 */
	bool ShouldShow(TrecPointer<TFileShell> node);

	/**
	 * the data held by this node
	 */
	TrecPointer<TFileShell> data;

	/**
	 * list of files held by this node (if object is a TDirectory
	 */
	TDataArray<TrecPointer<TObjectNode>> files;

	/**
	 * The Directory filtering mode
	 */
	file_node_filter_mode filter_mode;

	/**
	 * Whether to show all files or only the target ones
	 */
	bool showAllFiles;

	/**
	 * Extension Filter, only report these files 
	 */
	TDataArray<TString> extensions;
};

