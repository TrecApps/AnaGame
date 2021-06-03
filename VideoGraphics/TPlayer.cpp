#include "TPlayer.h"
#include <mfreadwrite.h>

#define SafeRelease(value) if(value) value->Release(); value = nullptr;

template <class Q>
HRESULT GetEventObject(IMFMediaEvent* pEvent, Q** ppObject)
{
	*ppObject = NULL;   // zero output

	PROPVARIANT var;
	HRESULT hr = pEvent->GetValue(&var);
	if (SUCCEEDED(hr))
	{
		if (var.vt == VT_UNKNOWN)
		{
			hr = var.punkVal->QueryInterface(ppObject);
		}
		else
		{
			hr = MF_E_INVALIDTYPE;
		}
		PropVariantClear(&var);
	}
	return hr;
}

HRESULT CreateMediaSource(PCWSTR pszURL, IMFMediaSource** ppSource);

HRESULT CreatePlaybackTopology(IMFMediaSource* pSource,
	IMFPresentationDescriptor* pPD, TrecPointer<DrawingBoard> board, IMFTopology** ppTopology, TrecComPointer<TMediaSink>& sink);




HRESULT TPlayer::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] = {
		QITABENT(TPlayer, IMFAsyncCallback), {0}
	};
	return QISearch(this, qit, riid, ppv);
}

ULONG TPlayer::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

ULONG TPlayer::Release()
{
	ULONG c = InterlockedDecrement(&m_nRefCount);
	if (!c)
	{
		delete this;
		
	}
	return c;
}

TrecComPointer<TPlayer> TPlayer::CreateInstance(HRESULT& res, TrecPointer<DrawingBoard> board)
{
	TrecComPointer<TPlayer> ret;

	TPlayer* play = new (std::nothrow) TPlayer(board);
	if (!play)
	{
		res = E_OUTOFMEMORY;
		return ret;
	}

	if (FAILED(res = play->Initialize()))
	{
		return ret;
	}
	TrecComPointer<TPlayer>::TrecComHolder hold;

	*hold.GetPointerAddress() = play;

	ret = hold.Extract();
	return ret;
}

HRESULT TPlayer::Initialize()
{
	HRESULT hr = MFStartup(MF_VERSION);

	if (SUCCEEDED(hr))
	{
		m_hCloseEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (!m_hCloseEvent)
			hr = HRESULT_FROM_WIN32(GetLastError());
	}
	return hr;
}

TPlayer::TPlayer(TrecPointer<DrawingBoard> board) :
	m_pSession(NULL),
	m_pSource(NULL),
	m_state(PlayerState::ps_closed),
	m_hCloseEvent(NULL),
	m_nRefCount(1)
{
	this->board = board;
}

TPlayer::~TPlayer()
{
	Shutdown();
}

HRESULT TPlayer::OpenURL(const TString& url)
{
	// assert(m_pSession);
	IMFTopology* top = nullptr;
	IMFPresentationDescriptor* present = nullptr;

	HRESULT hr = CreateSession();
	if (FAILED(hr))
		goto done;
	hr = CreateMediaSource(url.GetConstantBuffer().getBuffer(), &m_pSource);
	if (FAILED(hr))
		goto done;
	hr = m_pSource->CreatePresentationDescriptor(&present);
	if (FAILED(hr))
		goto done;
	hr = CreatePlaybackTopology(m_pSource, present, board, &top, this->m_sink);
	if (FAILED(hr))
		goto done;

	if (m_sink.Get())
		m_pVideoDisplay = m_sink->GetPresenter();

	hr = m_pSession->SetTopology(0, top);


	done:
	if (FAILED(hr))
		m_state = PlayerState::ps_closed;
	else
		m_state = PlayerState::ps_opening;

	if (top) top->Release();
	if (present) present->Release();
	top = nullptr;
	present = nullptr;
	return hr;
}

HRESULT TPlayer::CreateSession()
{
	HRESULT hr = CloseSession();

	if (FAILED(hr))
		return hr;

	//if (en.Get() && en->getDeviceD().Get())
	//{
	//	TrecComPointer<IMFAttributes>::TrecComHolder attsHolder;
	//	MFCreateAttributes(attsHolder.GetPointerAddress(), 5);
	//	TrecComPointer<IMFAttributes> atts = attsHolder.Extract();

	//	UINT token = 0;
	//	TrecComPointer<IMFDXGIDeviceManager>::TrecComHolder manHolder;
	//	MFCreateDXGIDeviceManager(&token, manHolder.GetPointerAddress());
	//	TrecComPointer<IMFDXGIDeviceManager> man = manHolder.Extract();


	//	TrecComPointer<ID3D11Device> dev = en->getDeviceD();
	//	ID3D10Multithread* multi = nullptr;
	//	if (SUCCEEDED(dev->QueryInterface(__uuidof(ID3D10Multithread), (void**)&multi)))
	//	{
	//		multi->SetMultithreadProtected(TRUE);
	//		multi->Release();
	//	}

	//	man->ResetDevice(dev.Get(), token);
	//	atts->SetUINT32(MF_SA_D3D11_AWARE, TRUE);
	//	atts->SetUnknown(MF_SOURCE_READER_D3D_MANAGER, man.Get());
	//	atts->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
	//	atts->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, TRUE);
	//	atts->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
	//	



	//	hr = MFCreateMediaSession(atts.Get(), &m_pSession);

	//}
	//else
		hr = MFCreateMediaSession(nullptr, &m_pSession);
	if (FAILED(hr))
		return hr;

	hr = m_pSession->BeginGetEvent(this, nullptr);
	if (SUCCEEDED(hr))
		m_state = PlayerState::ps_ready;
	return hr;
}

HRESULT TPlayer::Invoke(IMFAsyncResult* res)
{
	MediaEventType meType = MEUnknown;

	IMFMediaEvent* event = nullptr;

	HRESULT hr = m_pSession->EndGetEvent(res, &event);
	if (FAILED(hr))
		goto done;

	hr= event->GetType(&meType);
	if (FAILED(hr))
		goto done;

	if (meType == MESessionClosed)
	{
		SetEvent(m_hCloseEvent);
	}
	else
	{
		hr = m_pSession->BeginGetEvent(this, nullptr);
		if (FAILED(hr))
			goto done;
	}

	if (m_state != PlayerState::ps_closing)
	{
		event->AddRef();

		PostMessageW(board->GetWindowHandle(), WM_APP_PLAYER_EVENT, (WPARAM)event, (LPARAM)meType);
	}

done:
	if (event) event->Release(); event = nullptr;
	return S_OK;
}

HRESULT TPlayer::HandleEvent(UINT_PTR ev)
{
	IMFMediaEvent* pEv = (IMFMediaEvent*)ev;
	if (!pEv)
		return E_POINTER;
	HRESULT hrStat = S_OK;
	MediaEventType meType = MEUnknown;

	HRESULT hr = pEv->GetType(&meType);
	if (FAILED(hr))
		goto done;

	hr = pEv->GetStatus(&hrStat);
	if (SUCCEEDED(hr) && FAILED(hrStat))
		hr = hrStat;
	if (FAILED(hr))
		goto done;

	switch (meType)
	{
	case MESessionTopologyStatus:
		hr = OnTopologyStatus(pEv);
		break;

	case MEEndOfPresentation:
		hr = OnPresentationEnded(pEv);
		break;

	case MENewPresentation:
		hr = OnNewPresentation(pEv);
		break;

	default:
		hr = OnSessionEvent(pEv, meType);
		break;
	}


done: 
	if (pEv) pEv->Release();
	pEv = nullptr;
	return hr;
}

HRESULT TPlayer::OnTopologyStatus(IMFMediaEvent* ev)
{
	UINT status;

	HRESULT hr = ev->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &status);
	if (SUCCEEDED(hr) && (status == MF_TOPOSTATUS_READY))
	{
		hr = StartPlayback();
	}
	return hr;
}

HRESULT TPlayer::OnPresentationEnded(IMFMediaEvent* pEvent)
{
	// The session puts itself into the stopped state automatically.
	m_state = PlayerState::ps_stopped;
	return S_OK;
}

HRESULT TPlayer::OnNewPresentation(IMFMediaEvent* ev)
{
	IMFPresentationDescriptor* present = nullptr;
	IMFTopology* top = nullptr;

	HRESULT hr = GetEventObject(ev, &present);
	if (FAILED(hr))
		goto done;

	hr = CreatePlaybackTopology(m_pSource, present, board, &top, m_sink);
	if (FAILED(hr))
		goto done;

	if (m_sink.Get())
		m_pVideoDisplay = m_sink->GetPresenter();

	hr = m_pSession->SetTopology(0, top);

	if (SUCCEEDED(hr))
		m_state = PlayerState::ps_opening;


done:
	if (present) present->Release();
	if (top) top->Release();

	present = nullptr;
	top = nullptr;
	return hr;
}

HRESULT TPlayer::StartPlayback()
{
	assert(m_pSession);

	PROPVARIANT varStart;
	PropVariantInit(&varStart);

	HRESULT hr = m_pSession->Start(&GUID_NULL, &varStart);

	if (SUCCEEDED(hr))
		m_state = PlayerState::ps_started;
	PropVariantClear(&varStart);
	return hr;
}

HRESULT TPlayer::Play()
{
	if (m_state != PlayerState::ps_paused && m_state != PlayerState::ps_stopped)
	{
		return MF_E_INVALIDREQUEST;
	}
	if (m_pSession == NULL || m_pSource == NULL)
	{
		return E_UNEXPECTED;
	}
	return StartPlayback();
}

HRESULT TPlayer::Pause()
{
	if (m_state != PlayerState::ps_started)
	{
		return MF_E_INVALIDREQUEST;
	}
	if (m_pSession == NULL || m_pSource == NULL)
	{
		return E_UNEXPECTED;
	}

	HRESULT hr = m_pSession->Pause();
	if (SUCCEEDED(hr))
	{
		m_state = PlayerState::ps_paused;
	}

	return hr;
}

HRESULT TPlayer::Stop()
{
	if (m_state != PlayerState::ps_started && m_state != PlayerState::ps_paused)
	{
		return MF_E_INVALIDREQUEST;
	}
	if (m_pSession == NULL)
	{
		return E_UNEXPECTED;
	}

	HRESULT hr = m_pSession->Stop();
	if (SUCCEEDED(hr))
	{
		m_state = PlayerState::ps_stopped;
	}
	return hr;
}

HRESULT TPlayer::Repaint()
{
	if (m_pVideoDisplay.Get())
		return m_pVideoDisplay->RepaintVideo();
	return S_OK;
}

HRESULT TPlayer::ResizeVideo(RECT& loc)
{
	if (m_pVideoDisplay.Get())
		return m_pVideoDisplay->SetVideoPosition(nullptr, &loc);
	return S_OK;
}

HRESULT TPlayer::CloseSession()
{
	if (m_pVideoDisplay.Get()) m_pVideoDisplay->Release();
	m_pVideoDisplay.Nullify();

	HRESULT hr = S_OK;

	if (m_pSession)
	{
		DWORD waitRes = 0;

		m_state = PlayerState::ps_closing;

		hr = m_pSession->Close();

		if (SUCCEEDED(hr))
		{
			waitRes = WaitForSingleObject(m_hCloseEvent, 5000);
			if (waitRes == WAIT_TIMEOUT)
				assert(FALSE);
		}
	}

	if (SUCCEEDED(hr))
	{
		if (m_pSource)
		{
			m_pSource->Shutdown();
		}

		if (m_pSession)
			m_pSession->Shutdown();
	}

	if (m_pSource)
		m_pSource->Release();
	m_pSource = nullptr;

	if (m_pSession)
		m_pSession->Release();
	m_pSession = nullptr;

	m_state = PlayerState::ps_closed;
	return hr;
}

HRESULT TPlayer::Shutdown()
{
	HRESULT hr = CloseSession();

	MFShutdown();

	if (m_hCloseEvent)
	{
		CloseHandle(m_hCloseEvent);
		m_hCloseEvent = nullptr;
	}
	return hr;
}

HRESULT CreateMediaSinkActivate(
	IMFStreamDescriptor* pSourceSD,     // Pointer to the stream descriptor.
	TrecPointer<DrawingBoard> board,                  // Handle to the video clipping window.
	IMFActivate** ppActivate,
	TrecComPointer<TMediaSink>& sink
)
{
	IMFMediaTypeHandler* pHandler = NULL;
	IMFActivate* pActivate = NULL;
	TrecComPointer<TActivate> activate;
	// Get the media type handler for the stream.
	HRESULT hr = pSourceSD->GetMediaTypeHandler(&pHandler);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the major media type.
	GUID guidMajorType;
	hr = pHandler->GetMajorType(&guidMajorType);
	if (FAILED(hr))
	{
		goto done;
	}
	
	// Create an IMFActivate object for the renderer, based on the media type.
	if (MFMediaType_Audio == guidMajorType)
	{
		// Create the audio renderer.
		hr = MFCreateAudioRendererActivate(&pActivate);
	}
	else if (MFMediaType_Video == guidMajorType)
	{
		// Create the video renderer.
		activate = TActivate::CreateInstance(board);
		if (activate.Get())
		{
			pActivate = reinterpret_cast<IMFActivate*>(activate.Get());
			pActivate->AddRef();
			void* v = nullptr;
			activate->ActivateObject(__uuidof(IMFMediaSink), &v);
			sink = activate->GetSink();
			hr = S_OK;
		}
	}
	else
	{
		// Unknown stream type. 
		hr = E_FAIL;
		// Optionally, you could deselect this stream instead of failing.
	}
	if (FAILED(hr))
	{
		goto done;
	}

	// Return IMFActivate pointer to caller.
	*ppActivate = pActivate;
	(*ppActivate)->AddRef();

done:
	SafeRelease(pHandler);
	SafeRelease(pActivate);
	return hr;
}

// Add a source node to a topology.
HRESULT AddSourceNode(
	IMFTopology* pTopology,           // Topology.
	IMFMediaSource* pSource,          // Media source.
	IMFPresentationDescriptor* pPD,   // Presentation descriptor.
	IMFStreamDescriptor* pSD,         // Stream descriptor.
	IMFTopologyNode** ppNode)         // Receives the node pointer.
{
	IMFTopologyNode* pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the attributes.
	hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(pNode);
	return hr;
}

// Add an output node to a topology.
HRESULT AddOutputNode(
	IMFTopology* pTopology,     // Topology.
	IMFStreamSink* pActivate,     // Media sink activation object.
	DWORD dwId,                 // Identifier of the stream sink.
	IMFTopologyNode** ppNode)   // Receives the node pointer.
{
	IMFTopologyNode* pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the object pointer.
	hr = pNode->SetObject(pActivate);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the stream sink ID attribute.
	//hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId);
	//if (FAILED(hr))
	//{
	//	goto done;
	//}

	hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(pNode);
	return hr;
}


HRESULT AddBranchToPartialTopology(
	IMFTopology* pTopology,         // Topology.
	IMFMediaSource* pSource,        // Media source.
	IMFPresentationDescriptor* pPD, // Presentation descriptor.
	DWORD iStream,                  // Stream index.
	TrecPointer<DrawingBoard> board,
	TrecComPointer<TMediaSink>& sink)                 // Window for video playback.
{
	IMFStreamDescriptor* pSD = NULL;
	IMFTopologyNode* pSourceNode = NULL;
	IMFTopologyNode* pOutputNode = NULL;

	BOOL fSelected = FALSE;

	HRESULT hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD);
	if (FAILED(hr))
	{
		goto done;
	}

	if (fSelected)
	{
		LockWindowUpdate(nullptr);
		// Create the media sink activation object.

		sink = TMediaSink::CreateInstance(board);

		
		if (!sink.Get())
		{
			hr = E_FAIL;
			goto done;
		}

		// Add a source node for this stream.
		hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pSourceNode);
		if (FAILED(hr))
		{
			goto done;
		}

		// Create the output node for the renderer.
		IMFStreamSink* streamer = nullptr;
		sink->GetStreamSinkByIndex(0, &streamer);
		hr = AddOutputNode(pTopology, streamer, 0, &pOutputNode);
		if (FAILED(hr))
		{
			goto done;
		}

		// Connect the source node to the output node.
		hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
	}
	// else: If not selected, don't add the branch. 

done:
	SafeRelease(pSD);
	SafeRelease(pSourceNode);
	SafeRelease(pOutputNode);
	return hr;
}





HRESULT CreateMediaSource(PCWSTR pszURL, IMFMediaSource** ppSource)
{
	MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

	IMFSourceResolver* pSourceResolver = NULL;
	IUnknown* pSource = NULL;

	// Create the source resolver.
	HRESULT hr = MFCreateSourceResolver(&pSourceResolver);
	if (FAILED(hr))
	{
		goto done;
	}

	// Use the source resolver to create the media source.

	// Note: For simplicity this sample uses the synchronous method to create 
	// the media source. However, creating a media source can take a noticeable
	// amount of time, especially for a network source. For a more responsive 
	// UI, use the asynchronous BeginCreateObjectFromURL method.

	hr = pSourceResolver->CreateObjectFromURL(
		pszURL,                       // URL of the source.
		MF_RESOLUTION_MEDIASOURCE,  // Create a source object.
		NULL,                       // Optional property store.
		&ObjectType,        // Receives the created object type. 
		&pSource            // Receives a pointer to the media source.
	);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the IMFMediaSource interface from the media source.
	hr = pSource->QueryInterface(IID_PPV_ARGS(ppSource));

done:
	SafeRelease(pSourceResolver);
	SafeRelease(pSource);
	return hr;
}

HRESULT CreatePlaybackTopology(IMFMediaSource* pSource, IMFPresentationDescriptor* pPD, TrecPointer<DrawingBoard> board, IMFTopology** ppTopology, TrecComPointer<TMediaSink>& sink)
{
	IMFTopology* pTopology = NULL;
	DWORD cSourceStreams = 0;

	// Create a new topology.
	HRESULT hr = MFCreateTopology(&pTopology);
	if (FAILED(hr))
	{
		goto done;
	}




	// Get the number of streams in the media source.
	hr = pPD->GetStreamDescriptorCount(&cSourceStreams);
	if (FAILED(hr))
	{
		goto done;
	}

	// For each stream, create the topology nodes and add them to the topology.
	for (DWORD i = 0; i < cSourceStreams; i++)
	{
		hr = AddBranchToPartialTopology(pTopology, pSource, pPD, i, board,sink);
		if (FAILED(hr))
		{
			goto done;
		}
	}

	// Return the IMFTopology pointer to the caller.
	*ppTopology = pTopology;
	(*ppTopology)->AddRef();

done:
	SafeRelease(pTopology);
	return hr;
}
