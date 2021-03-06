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
	showAllFiles = false;
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
	AG_THREAD_LOCK
		if (!data.Get())
		{
			RETURN_THREAD_UNLOCK TString();
		}
	TString ret(data->GetName());
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TDirectory::IsExtendable
* Purpose: Reports whether this node is extendable - in this case, whether the file is a directory or not
* Parameters: void
* Returns: bool - whether the node is extendable (i.e. references a directory)
*/
bool TFileNode::IsExtendable()
{
	AG_THREAD_LOCK
		if (!data.Get())
		{
			RETURN_THREAD_UNLOCK false;
		}
	RETURN_THREAD_UNLOCK data->IsDirectory();
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
	AG_THREAD_LOCK
	for (UINT rust = 0; rust < files.Size(); rust++)
	{
		ret = files[rust]->GetNodeAt(target, current);
		if (ret.Get())
		{
			RETURN_THREAD_UNLOCK ret;
		}
		current += files[rust]->TotalChildren() + 1;
	}
	RETURN_THREAD_UNLOCK TrecPointer<TObjectNode>();
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
	AG_THREAD_LOCK
	for (UINT rust = 0; rust < files.Size(); rust++)
	{
		ret += files[rust]->TotalChildren() + 1;
	}
	RETURN_THREAD_UNLOCK ret;
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
	AG_THREAD_LOCK
	data = TFileShell::GetFileInfo(value);
	RETURN_THREAD_UNLOCK data.Get() != nullptr;
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
	AG_THREAD_LOCK
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
	RETURN_THREAD_UNLOCK;
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
	AG_THREAD_LOCK
	if (index < files.Size())
	{
		auto ret = files[index];
		RETURN_THREAD_UNLOCK ret;
	}
	RETURN_THREAD_UNLOCK TrecPointer<TObjectNode>();
}

/*
* Method: TDirectory::DropChildNodes
* Purpose: Drops the child nodes, provided with a control in mind
* Parameters: void
* Returns: void
*/
void TFileNode::DropChildNodes()
{
	AG_THREAD_LOCK
	files.RemoveAll();
	RETURN_THREAD_UNLOCK;
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
	AG_THREAD_LOCK
	data = d;
	DropChildNodes();
	RETURN_THREAD_UNLOCK;
}


/**
 * Method: TFileNode::GetData
 * Purpose: retireves the underlying file shell object
 * Parameters: void
 * Returns: TrecPointer<TFileShell> - the data the node is holding
 */
TrecPointer<TFileShell> TFileNode::GetData()
{
	AG_THREAD_LOCK
		auto ret = data;
	RETURN_THREAD_UNLOCK ret;
}

void TFileNode::SetShowAllFiles(bool show)
{
	AG_THREAD_LOCK
	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		auto fileNode = dynamic_cast<TFileNode*>(files[Rust].Get());

		if (fileNode)
		{
			fileNode->SetShowAllFiles(show);
		}
	}

	showAllFiles = show;
	RETURN_THREAD_UNLOCK;
}

bool TFileNode::GetShowAllFiles()
{
	AG_THREAD_LOCK
		bool ret = showAllFiles || !extensions.Size();
	RETURN_THREAD_UNLOCK ret;
}

bool TFileNode::AddExtension(const TString& extension)
{
	AG_THREAD_LOCK
		for (UINT Rust = 0; Rust < extensions.Size(); Rust++)
		{
			if (!extensions[Rust].CompareNoCase(extension))
			{
				RETURN_THREAD_UNLOCK true;
			}
		}

	if (extension.GetSize() < 2)
	{
		RETURN_THREAD_UNLOCK false;
	}
	if (extension[0] == L'.')
	{
		for (UINT Rust = 1; Rust < extension.GetSize(); Rust++)
		{
			WCHAR letter = extension[Rust];

			if ((letter >= L'A' && letter <= L'Z') ||
				(letter >= L'a' && letter <= L'z'))
				continue;
			RETURN_THREAD_UNLOCK false;
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

		RETURN_THREAD_UNLOCK true;
	}
	RETURN_THREAD_UNLOCK false;
}

void TFileNode::SetFilterMode(file_node_filter_mode mode)
{
	AG_THREAD_LOCK
	filter_mode = mode;

	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		auto fileNode = dynamic_cast<TFileNode*>(files[Rust].Get());
		if (fileNode)
			fileNode->SetFilterMode(mode);
	}
	RETURN_THREAD_UNLOCK;
}

file_node_filter_mode TFileNode::GetFilterMode()
{
	return filter_mode;
}

/**
 * Method: TFileNode::RemoveNode
 * Purpose: Removes the specified node
 * Parameteres: TrecPointer<TObjectNode> - the node to remove
 * Returns: bool - whether the node was found
 *
 * Attributes: override
 */
bool TFileNode::RemoveNode(TrecPointer<TObjectNode> obj)
{
	AG_THREAD_LOCK
		if (!obj.Get())
		{
			RETURN_THREAD_UNLOCK false;
		}
	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		auto f = files[Rust].Get();
		if (!f)
			continue;

		if (f == obj.Get())
		{
			files.RemoveAt(Rust);
			RETURN_THREAD_UNLOCK true;
		}

		if (f->RemoveNode(obj))
		{
			RETURN_THREAD_UNLOCK true;
		}
	}
	RETURN_THREAD_UNLOCK false;
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
	AG_THREAD_LOCK
	if (node->IsDirectory())
	{
		switch (filter_mode)
		{
		case file_node_filter_mode::fnfm_blank:
		case file_node_filter_mode::fnfm_block_files:
			RETURN_THREAD_UNLOCK true;
		case file_node_filter_mode::fnfm_block_current:
		case file_node_filter_mode::fnfm_block_current_and_files:
			RETURN_THREAD_UNLOCK nodeName.Compare(L".");
		case file_node_filter_mode::fnfm_block_upper:
		case file_node_filter_mode::fnfm_block_upper_and_files:
			RETURN_THREAD_UNLOCK nodeName.Compare(L"..");
		case file_node_filter_mode::fnfm_block_both:
		case file_node_filter_mode::fnfm_block_both_and_files:
			RETURN_THREAD_UNLOCK nodeName.Compare(L".") && nodeName.Compare(L"..");
		case file_node_filter_mode::fnfm_block_directories:
			RETURN_THREAD_UNLOCK false;
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
			RETURN_THREAD_UNLOCK false;
		default:
			if (showAllFiles && extensions.Size())
			{
				for (UINT Rust = 0; Rust < extensions.Size(); Rust++)
				{
					if (nodeName.FindLast(extensions[Rust]) == nodeName.GetSize() - extensions[Rust].GetSize())
					{
						RETURN_THREAD_UNLOCK true;
					}
				}
				RETURN_THREAD_UNLOCK false;
			}
			else
			{
				RETURN_THREAD_UNLOCK true;
			}
		}
	}
}
