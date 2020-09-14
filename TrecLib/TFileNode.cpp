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
	filter_mode = file_node_filter_mode::fnfm_blank;
}

/**
 * Method: TFileNode::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TFileNode::GetType()
{
	return TString(L"TFileNode;") + TObjectNode::GetType();
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
			if (ShouldShow(f))
			{
				TrecPointer<TObjectNode> n = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(level + 1);
				files.push_back(n);
				auto fileNode = dynamic_cast<TFileNode*>(n.Get());
				fileNode->SetFile(f);
				fileNode->SetFilterMode(filter_mode);
				fileNode->SetShowAllFiles(showAllFiles);

				for (UINT Rust = 0; Rust < extensions.Size(); Rust++)
				{
					fileNode->AddExtension(extensions[Rust]);
				}
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

void TFileNode::SetShowAllFiles(bool show)
{
	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		auto fileNode = dynamic_cast<TFileNode*>(files[Rust].Get());

		if (fileNode)
		{
			fileNode->SetShowAllFiles(show);
		}
	}

	showAllFiles = show;
}

bool TFileNode::GetShowAllFiles()
{
	return showAllFiles || !extensions.Size();
}

bool TFileNode::AddExtension(const TString& extension)
{
	for (UINT Rust = 0; Rust < extensions.Size(); Rust++)
	{
		if (!extensions[Rust].CompareNoCase(extension))
			return true;
	}

	if (extension.GetSize() < 2)
		return false;

	if (extension[0] == L'.')
	{
		for (UINT Rust = 1; Rust < extension.GetSize(); Rust++)
		{
			WCHAR letter = extension[Rust];

			if ((letter >= L'A' && letter <= L'Z') ||
				(letter >= L'a' && letter <= L'z'))
				continue;
			return false;
		}

		extensions.push_back(extension);

		for (UINT Rust = 0; Rust < files.Size(); Rust++)
		{
			auto fileNode = dynamic_cast<TFileNode*>(files[Rust].Get());

			if (fileNode)
			{
				fileNode->AddExtension(extension);
			}

		}

		return true;
	}
	return false;
}

void TFileNode::SetFilterMode(file_node_filter_mode mode)
{
	filter_mode = mode;

	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		auto fileNode = dynamic_cast<TFileNode*>(files[Rust].Get());
		if (fileNode)
			fileNode->SetFilterMode(mode);
	}
}

file_node_filter_mode TFileNode::GetFilterMode()
{
	return filter_mode;
}

/**
 * Method: TFileNode::ShouldShow
 * Purpose: Takes the filter configuration to determine if to should be shown
 * Parameters: TrecPointer<TFileShell> node -  the file entry to subject to filtering
 * Returns: bool - whether the node passes the criteria to be shown
 */
bool TFileNode::ShouldShow(TrecPointer<TFileShell> node)
{
	if(!node.Get())
		return false;

	TString nodeName(node->GetName());

	if (node->IsDirectory())
	{
		switch (filter_mode)
		{
		case file_node_filter_mode::fnfm_blank:
		case file_node_filter_mode::fnfm_block_files:
			return true;
		case file_node_filter_mode::fnfm_block_current:
		case file_node_filter_mode::fnfm_block_current_and_files:
			return nodeName.Compare(L".");
		case file_node_filter_mode::fnfm_block_upper:
		case file_node_filter_mode::fnfm_block_upper_and_files:
			return nodeName.Compare(L"..");
		case file_node_filter_mode::fnfm_block_both:
		case file_node_filter_mode::fnfm_block_both_and_files:
			return nodeName.Compare(L".") && nodeName.Compare(L"..");
		case file_node_filter_mode::fnfm_block_directories:
			return false;
		}
	}
	else
	{
		// We have a regular file
		switch (filter_mode)
		{
		case file_node_filter_mode::fnfm_block_current_and_files:
		case file_node_filter_mode::fnfm_block_upper_and_files:
		case file_node_filter_mode::fnfm_block_both_and_files:
		case file_node_filter_mode::fnfm_block_files:
			return false;
		default:
			if (showAllFiles && extensions.Size())
			{
				for (UINT Rust = 0; Rust < extensions.Size(); Rust++)
				{
					if (nodeName.FindLast(extensions[Rust]) == nodeName.GetSize() - extensions[Rust].GetSize())
						return true;
				}
				return false;
			}
			else
			{
				return true;
			}
		}
	}
}
