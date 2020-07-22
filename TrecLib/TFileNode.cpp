#include "TFileNode.h"
#include "TDirectory.h"

/*
* Method: TDirectory::Constructor
* Purpose: Constructs the TFileNode
* Parameters: UINT l - level of the node (used by the TObjectNode class)
* Returns: void
*/
TFileNode::TFileNode(UINT l) : TObjectNode(l)
{
}

/*
 * Method: TDirectory::GetContent
 * Purpose: Retrieves the Content to present about this object
 * Parameters: void 
 * Returns: TString - the name of the File/directory this node references
 */
TString TFileNode::GetContent()
{
	if (!data.Get())
		return TString();

	return data->GetName();
}

/*
* Method: TDirectory::IsExtendable
* Purpose: Reports whether this node is extendable - in this case, whether the file is a directory or not
* Parameters: void
* Returns: bool - whether the node is extendable (i.e. references a directory)
*/
bool TFileNode::IsExtendable()
{
	if (!data.Get())
		return false;
	return data->IsDirectory();
}

/*
* Method: TDirectory::IsExtended
* Purpose: Reports whether this node is currently extended
* Parameters: void
* Returns: bool - whether the node is extended
*/
bool TFileNode::IsExtended()
{
	return files.Size();
}

/*
* Method: TDirectory::GetNodeAt
* Purpose: Return a specific node, with an assumption on where "this" node is in the heirarchy
* Parameters: UINT target - the desired node
*				UINT current - the node that this node should assume to be
* Returns: TrecPointer<TObjectNode> - the desired node (null if the node is not in this nodes reach)
*/
TrecPointer<TObjectNode> TFileNode::GetNodeAt(UINT target, UINT current)
{
	if (target == current)
		return TrecPointerKey::GetTrecPointerFromSoft<TObjectNode>(self);

	if(target < current)
		return TrecPointer<TObjectNode>();

	TrecPointer<TObjectNode> ret;

	current++;

	for (UINT rust = 0; rust < files.Size(); rust++)
	{
		ret = files[rust]->GetNodeAt(target, current);
		if (ret.Get())
			return ret;

		current += files[rust]->TotalChildren() + 1;
	}
	return TrecPointer<TObjectNode>();
}

/*
* Method: TDirectory::TotalChildren
* Purpose: Reports Reports the total number of children, grand children, and so on held by this node
* Parameters: void
* Returns: UINT - number of nodes for which this node is an ancestor
*/
UINT TFileNode::TotalChildren()
{
	UINT ret = 0;
	for (UINT rust = 0; rust < files.Size(); rust++)
	{
		ret += files[rust]->TotalChildren() + 1;
	}
	return ret;
}

/*
* Method: TDirectory::Initialize
* Purpose: Supposed to initialize the node tree
* Parameters: void
* Returns: bool - false as the Node cannot be initialized without the path provided
*/
bool TFileNode::Initialize()
{
	return false;
}

/*
* Method: TDirectory::Initialize
* Purpose: Initializes the node with the path of the file/Directory to start with
* Parameters: TStrng& value - the path of the File to reference
* Returns: bool - true of a file object was generated, false otherwise
*/
bool TFileNode::Initialize(TString& value)
{
	data = TFileShell::GetFileInfo(value);
	return data.Get();
}

/*
* Method: TDirectory::Extend
* Purpose: Epands the Tree by getting all of the files in the directory
* Parameters: void
* Returns: void
*
* Note: This method only functions of the file in question is a directory, this can be determined via the IsExtendable method
*/
void TFileNode::Extend()
{
	files.RemoveAll();
	if (data.Get() && data->IsDirectory())
	{
		TDataArray<TrecPointer<TFileShell>> shellFiles;
		dynamic_cast<TDirectory*>(data.Get())->GetFileListing(shellFiles);

		for (UINT Rust = 0; Rust < shellFiles.Size(); Rust++)
		{
			TrecPointer<TFileShell> f = shellFiles[Rust];
			if (f.Get())
			{
				TrecPointer<TObjectNode> n = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(level + 1);
				files.push_back(n);
				dynamic_cast<TFileNode*>(n.Get())->SetFile(f);
			}
		}
	}
}

/*
* Method: TDirectory::GetChildNodes
* Purpose: Retrieves Child Nodes one at a time via an index.
* Parameters: UINT index - the index of the child node to retrieve
* Returns: TrecPointer<TObjectNode> - the node in question
*
* Note: Pointer returned is null if the index is too high. To browse the tree, start the index at 0 and once the result is null, you can be sure there are no more nodes
*/
TrecPointer<TObjectNode> TFileNode::GetChildNodes(UINT index)
{
	if (index < files.Size())
		return files[index];
	return TrecPointer<TObjectNode>();
}

/*
* Method: TDirectory::DropChildNodes
* Purpose: Drops the child nodes, provided with a control in mind
* Parameters: void
* Returns: void
*/
void TFileNode::DropChildNodes()
{
	files.RemoveAll();
}


/*
* Method: TDirectory::getVariableValueStr
* Purpose: Reports the desired Content
* Parameters: const TString& varName - the name
* Returns: TString - the file/Directory name held by this object
*/
TString TFileNode::getVariableValueStr(const TString& varName)
{
	return GetContent();
}

/*
* Method: TDirectory::SetFile
* Purpose: Manually Initialize a node with an existing File Record
* Parameters: TrecPointer<TFileShell>& d - the file to reference
* Returns: void
*/
void TFileNode::SetFile(TrecPointer<TFileShell>& d)
{
	data = d;
	DropChildNodes();
}


/**
 * Method: TFileNode::GetData
 * Purpose: retireves the underlying file shell object
 * Parameters: void
 * Returns: TrecPointer<TFileShell> - the data the node is holding
 */
TrecPointer<TFileShell> TFileNode::GetData()
{
	return data;
}