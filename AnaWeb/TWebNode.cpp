#include "pch.h"
#include "TWebNode.h"

TWebNode::TWebNode(TrecPointer<DrawingBoard> board)
{
	this->board = board;
}

UINT TWebNode::ProcessHtml(TrecPointer<TFile> file, const TString& tag)
{
	return 0;
}

UINT TWebNode::ProcessInnerHtml()
{
	return 0;
}

UINT TWebNode::SetInnerHtml(const TString& html)
{
	return 0;
}

void TWebNode::GetElementsByName(const TString& name, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!name.GetSize())
		return;

	if (!name.Compare(this->name))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		childNodes[Rust]->GetElementsByName(name, nodes);
	}
}

void TWebNode::GetElementsByTag(const TString& tag, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!tag.GetSize())
		return;
	if (!tag.Compare(this->tagName))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		childNodes[Rust]->GetElementsByTag(name, nodes);
	}
}

void TWebNode::GetElementsByClass(const TString& nodeClass, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!nodeClass.GetSize())
		return;
	if (!nodeClass.Compare(this->nodeClass))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		childNodes[Rust]->GetElementsByClass(name, nodes);
	}
}

TrecPointer<TWebNode> TWebNode::GetElementById(const TString& id)
{
	// If an empty string, just return
	if(!id.GetSize())
		return TrecPointer<TWebNode>();

	// If this is the node
	if (!id.Compare(this->id))
		return TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self);

	TrecPointer<TWebNode> ret;

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		ret = childNodes.at(Rust)->GetElementById(id);
		if (ret.Get())
			break;
	}
	return ret;
}

void TWebNode::SetSelf(TrecPointer<TWebNode> s)
{
	if (s.Get() != this)
		throw L"Expected Self to be this";
	self = TrecPointerKey::GetSoftPointerFromTrec<TWebNode>(s);
}

TString TWebNode::GetType()
{
	return TString(L"TWebNode;TObject");
}
