#include "TPlayer2.h"

void SafeRelease(IUnknown** u)
{
	if (!u || !(*u))
		return;
	(*u)->Release();
	(*u) = nullptr;
}

PlayerNode::PlayerNode(IMFMediaSource* source)
{
	assert(source);
	this->source = source;
	this->transform = nullptr;
	this->sink = nullptr;
	this->type = player_node_type::pnt_source;
}

PlayerNode::PlayerNode(IMFTransform* transform)
{
	assert(transform);
	this->source = nullptr;
	this->transform = transform;
	this->sink = nullptr;
	this->type = player_node_type::pnt_transform;
}

PlayerNode::PlayerNode(IMFMediaSink* sink)
{
	assert(sink);
	this->source = nullptr;
	this->transform = nullptr;
	this->sink = sink;
	this->type = player_node_type::pnt_sink;
}

PlayerNode::~PlayerNode()
{
	SafeRelease((IUnknown**)&source);
	SafeRelease((IUnknown**)&transform);
	SafeRelease((IUnknown**)&sink);
}

void PlayerNode::AttachOutput(TrecPointer<PlayerNode> node)
{
	if (node.Get())
		output.push_back(node);
}

TPlayer2::TPlayer2()
{
}

bool TPlayer2::SetSourceUrl(const TString& url)
{
	return false;
}

bool TPlayer2::SetSinkFile(TrecPointer<TFileShell> file)
{
	return false;
}

bool TPlayer2::SetBoardSink(TrecPointer<DrawingBoard> board, const D2D1_RECT_F& location)
{
	return false;
}

TString TPlayer2::PrepTopology(UINT maxTransforms)
{
	return TString();
}
