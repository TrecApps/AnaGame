#include "TPlayer2.h"

void SafeRelease(IUnknown** u)
{
	if (!u || !(*u))
		return;
	(*u)->Release();
	(*u) = nullptr;
}

GUID clInputTypes[] = {
	MFVideoFormat_ARGB32,
	MFVideoFormat_AYUV,
	MFVideoFormat_I420,
	MFVideoFormat_IYUV,
	MFVideoFormat_NV11,
	MFVideoFormat_NV12,
	MFVideoFormat_RGB24,
	MFVideoFormat_RGB32,
	MFVideoFormat_RGB555,
	MFVideoFormat_RGB565,
	MFVideoFormat_RGB8,
	MFVideoFormat_UYVY,
	MFVideoFormat_v410,
	MFVideoFormat_Y216,
	MFVideoFormat_Y41P,
	MFVideoFormat_Y41T,
	MFVideoFormat_Y42T,
	MFVideoFormat_YUY2,
	MFVideoFormat_YV12,
	MFVideoFormat_YVYU
};

GUID clOutputTypes[] = {
	MFVideoFormat_ARGB32,
	MFVideoFormat_AYUV,
	MFVideoFormat_I420,
	MFVideoFormat_IYUV,
	MFVideoFormat_NV12,
	MFVideoFormat_RGB24,
	MFVideoFormat_RGB32,
	MFVideoFormat_RGB555,
	MFVideoFormat_RGB565,
	MFVideoFormat_UYVY,
	MFVideoFormat_Y216,
	MFVideoFormat_YUY2,
	MFVideoFormat_YV12,
};

class TransformTypeNode
{
public:
	TransformTypeNode()
	{
		type = nullptr;
	}
	~TransformTypeNode()
	{
		SafeRelease((IUnknown**)&type);
	}

	void SetType(IMFMediaType* t)
	{
		if (t)
			t->AddRef();
		SafeRelease((IUnknown**)&type);
		type = t;
	}
	TDataArray<TrecPointer<TransformTypeNode>> outputs;
	IMFMediaType* type;
	UINT nodeLevel;
};

bool outputSupportedAsInput(IMFMediaType* output)
{
	GUID type = GUID_NULL;
	assert(SUCCEEDED(output->GetGUID(MF_MT_SUBTYPE, &type)));

	for (UINT Rust = 0; Rust < ARRAYSIZE(clInputTypes); Rust++)
	{
		if (type == clInputTypes[Rust])
			return true;
	}
	return false;
}


void PrepNodeLinks(IMFTransform* transform, TrecPointer<TransformTypeNode> typeNode, UINT maxParentAllowed)
{
	if (typeNode->nodeLevel == maxParentAllowed)
		return;
	IMFMediaType* type = nullptr;
	for (UINT Rust = 0; SUCCEEDED(transform->GetOutputAvailableType(0, Rust, &type)); Rust++)
	{
		TrecPointer<TransformTypeNode> newNode = TrecPointerKey::GetNewTrecPointer<TransformTypeNode>();

		newNode->nodeLevel = typeNode->nodeLevel + 1;
		newNode->type = type;

		typeNode->outputs.push_back(newNode);

		if (outputSupportedAsInput(newNode->type))
			PrepNodeLinks(transform, newNode, maxParentAllowed);
	}
}

TrecPointer<TransformTypeNode> PrepNodeLinks(IMFTransform* transform, IMFMediaType* mType, UINT maxParentAllowed)
{
	// Set initial type for transform
	if(FAILED(transform->SetInputType(0, mType, 0)))
		return TrecPointer<TransformTypeNode>();


	TrecPointer<TransformTypeNode> ret = TrecPointerKey::GetNewTrecPointer<TransformTypeNode>();
	ret->SetType(mType);
	
	ret->nodeLevel = 0;

	PrepNodeLinks(transform, ret, maxParentAllowed);

	return ret;
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

IMFMediaSource* PlayerNode::GetMediaSource()
{
	if (source)
		source->AddRef();
	return source;
}

IMFTransform* PlayerNode::GetMediaTransform()
{
	if (transform)
		transform->AddRef();
	return transform;
}

IMFMediaSink* PlayerNode::GetMediaSink()
{
	if (sink)
		sink->AddRef();
	return sink;
}

player_node_type PlayerNode::GetType()
{
	return type;
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
	if (!source.Get())
		return L"Media Source not Prepared";

	if (!sinks.Size())
		return L"No Media Sinks Prepared";

	// Get Information about the Media Type of the Source
	IMFMediaSource* sSource = source->GetMediaSource();
	assert(sSource);
	IMFPresentationDescriptor* desc = nullptr;
	assert(SUCCEEDED(sSource->CreatePresentationDescriptor(&desc)));
	DWORD streamCount = 0;
	assert(SUCCEEDED(desc->GetStreamDescriptorCount(&streamCount)));

	for (UINT Rust = 0; Rust < streamCount; Rust++)
	{
		IMFStreamDescriptor* pSD = NULL;
		BOOL fSelected = FALSE;
		assert(SUCCEEDED(desc->GetStreamDescriptorByIndex(Rust, &fSelected, &pSD)));

		IMFMediaTypeHandler* typeHandler = nullptr;
		GUID streamType = GUID_NULL;
		if (fSelected && SUCCEEDED(pSD->GetMediaTypeHandler(&typeHandler)) && SUCCEEDED(typeHandler->GetMajorType(&streamType)))
		{
			if (MFMediaType_Video == streamType)
			{
				
			}
		}
	}

	// Get Information about the Sinks

	

	return TString();
}
