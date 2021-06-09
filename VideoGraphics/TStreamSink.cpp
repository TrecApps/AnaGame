#include "TStreamSink.h"
#include <Shlwapi.h>
#include <Mferror.h>
#include "TVideoMarker.h"
#include <mfapi.h>
#include "TSampleTexture.h"

GUID const* const s_pVideoFormats[] =
{
    &MFVideoFormat_NV12,
    &MFVideoFormat_IYUV,
    &MFVideoFormat_YUY2,
    &MFVideoFormat_YV12,
    &MFVideoFormat_RGB32,
    &MFVideoFormat_RGB32,
    &MFVideoFormat_RGB24,
    &MFVideoFormat_RGB555,
    &MFVideoFormat_RGB565,
    &MFVideoFormat_RGB8,
    &MFVideoFormat_AYUV,
    &MFVideoFormat_UYVY,
    &MFVideoFormat_YVYU,
    &MFVideoFormat_YVU9,
    &MFVideoFormat_v410,
    &MFVideoFormat_I420,
    &MFVideoFormat_NV11,
    &MFVideoFormat_420O
};

const DWORD s_dwNumVideoFormats = sizeof(s_pVideoFormats) / sizeof(s_pVideoFormats[0]);

const MFRatio s_DefaultFrameRate = { 30, 1 };

STDMETHODIMP_(ULONG __stdcall) TStreamSink::AddRef(void)
{
    return InterlockedIncrement(&m_nRefCount);
}

HRESULT TStreamSink::QueryInterface(REFIID iid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(static_cast<IMFStreamSink*>(this));
    }
    else if (iid == __uuidof(IMFStreamSink))
    {
        *ppv = static_cast<IMFStreamSink*>(this);
    }
    else if (iid == __uuidof(IMFMediaEventGenerator))
    {
        *ppv = static_cast<IMFMediaEventGenerator*>(this);
    }
    else if (iid == __uuidof(IMFMediaTypeHandler))
    {
        *ppv = static_cast<IMFMediaTypeHandler*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG __stdcall) TStreamSink::Release(void)
{
    ULONG c = InterlockedDecrement(&m_nRefCount);
    if (!c)
    {
        Shutdown();
        delete this;
    }
    return c;
}

HRESULT TStreamSink::Flush(void)
{
    ThreadLock();
    samples.Flush();
    ThreadRelease();
    processFrames = false;
    return S_OK;
}

HRESULT TStreamSink::GetIdentifier(__RPC__out DWORD* pdwIdentifier)
{
    if (!pdwIdentifier)
        return E_POINTER;
    ThreadLock();
    HRESULT ret = S_OK;
    if (CheckShutdown())
        ret = MF_E_SHUTDOWN;
    else
        *pdwIdentifier = 1;
    ThreadRelease();
    return ret;
}

HRESULT TStreamSink::GetMediaSink(__RPC__deref_out_opt IMFMediaSink** ppMediaSink)
{
    HRESULT ret = S_OK;
    ThreadLock();
    IMFMediaSink* sink = mediaSink.Get();
    if (!ppMediaSink)
        ret = E_POINTER;
    else if (!sink)
        ret = E_NOT_SET;
    
    if (ret == S_OK)
    {
        sink->AddRef();
        *ppMediaSink = sink;
    }
    ThreadRelease();
    return ret;
}

HRESULT TStreamSink::GetMediaTypeHandler(__RPC__deref_out_opt IMFMediaTypeHandler** ppHandler)
{
    if (!ppHandler)
        return E_POINTER;

    ThreadLock();
    HRESULT ret = S_OK;
    if (CheckShutdown())
        ret = MF_E_SHUTDOWN;
    else
        ret = this->QueryInterface(IID_IMFMediaTypeHandler, (void**)ppHandler);
    ThreadRelease();
    return ret;
}

HRESULT TStreamSink::PlaceMarker(MFSTREAMSINK_MARKER_TYPE eMarkerType, __RPC__in const PROPVARIANT* pvarMarkerValue, __RPC__in const PROPVARIANT* pvarContextValue)
{
    if (!pvarContextValue || !pvarMarkerValue)
        return E_POINTER;
    ThreadLock();
    HRESULT ret = S_OK;

    if (CheckShutdown())
    {
        ret = MF_E_SHUTDOWN;
    }
    else
    {
        TVideoMarker* mark = nullptr;
        ret = TVideoMarker::Create(eMarkerType, pvarMarkerValue, pvarContextValue, &mark);
        if (SUCCEEDED(ret))
        {
            auto umark = (IUnknown*)mark;
            samples.Push(umark);
        }
    }
    return ret;
}

HRESULT TStreamSink::ProcessSample(__RPC__in_opt IMFSample* pSample)
{
    if (!pSample)
        return E_POINTER;
    ThreadLock();
    DWORD sampleSize = 0;
    HRESULT ret = pSample->GetBufferCount(&sampleSize);
    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }

    IMFMediaBuffer* pBuffer = nullptr;
    if (1 == sampleSize)
    {
        ret = pSample->GetBufferByIndex(0, &pBuffer);
    }
    else
        ret = pSample->ConvertToContiguousBuffer(&pBuffer);

    if (FAILED(ret))
        return ret;

    IMF2DBuffer* p2Buff = nullptr;
    ret = pBuffer->QueryInterface<IMF2DBuffer>(&p2Buff);
    if (FAILED(ret))
    {
        pBuffer->Release();
        return ret;
    }
    BYTE* picByte = nullptr;
    LONG picLen = 0;
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(resourceData));

    ret = p2Buff->Lock2D((BYTE**)(&resourceData.pSysMem), &picLen);

    if (FAILED(ret))
    {
        pBuffer->Release();
        p2Buff->Release();
        return ret;
    }

    if (picLen < 0)
    {
        // To-Do: Handle Scenario
        int e = 0;
    }
    resourceData.SysMemPitch = picLen;

    if (!textureDesc.Width || !textureDesc.Height)
    {
        return E_NOT_SET;    
    }

    ID3D11Texture2D* textures = nullptr;
    ret = device->CreateTexture2D(&textureDesc, &resourceData, &textures);

    auto uSample = (IUnknown*)textures;
    samples.Push(uSample);

    if (isPrerolling)
    {
        isPrerolling = false;
        ret = QueueEvent(MEStreamSinkPrerolled, GUID_NULL, ret, nullptr);
    }
    else if(state != PlayState::State_Paused && state != PlayState::State_Stopped)
    {
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpProcessSample);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }

    ThreadRelease();
    return S_OK;
}

HRESULT TStreamSink::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState)
{
    if (!pCallback || !punkState)
        return E_POINTER;
    HRESULT ret = S_OK;

    TObject::ThreadLock();
    ret = CheckShutdown() ? MF_E_SHUTDOWN : m_pEventQueue->BeginGetEvent(pCallback, punkState);
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::EndGetEvent(IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent)
{
    HRESULT ret = S_OK;

    TObject::ThreadLock();
    ret = CheckShutdown() ? MF_E_SHUTDOWN : m_pEventQueue->EndGetEvent(pResult, ppEvent);
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::GetEvent(DWORD dwFlags, __RPC__deref_out_opt IMFMediaEvent** ppEvent)
{
    IMFMediaEventQueue* pQueue = NULL;
    TObject::ThreadLock();
    HRESULT ret = CheckShutdown() ? MF_E_SHUTDOWN : S_OK;
    if (SUCCEEDED(ret))
    {
        pQueue = m_pEventQueue;
        pQueue->AddRef();
    }
    TObject::ThreadRelease();
    if (SUCCEEDED(ret))
    {
        pQueue->GetEvent(dwFlags, ppEvent);
        pQueue->Release();
    }
    return ret;
}

HRESULT TStreamSink::QueueEvent(MediaEventType met, __RPC__in REFGUID guidExtendedType, HRESULT hrStatus, __RPC__in_opt const PROPVARIANT* pvValue)
{
    TObject::ThreadLock();
    HRESULT ret = CheckShutdown() ? MF_E_SHUTDOWN : m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    TObject::ThreadRelease();
    return ret;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetCurrentMediaType(_Outptr_ IMFMediaType** ppMediaType)
{
    if (!ppMediaType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    ThreadLock();
    HRESULT ret = S_OK;
    if (currType)
    {
        *ppMediaType = currType;
        currType->AddRef();
    }
    else
        ret = MF_E_NOT_INITIALIZED;

        ThreadRelease();
    return ret;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMajorType(__RPC__out GUID* pguidMajorType)
{
    if (!pguidMajorType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    ThreadLock();
    HRESULT ret = S_OK;
    if (currType)
    {
        currType->GetGUID(MF_MT_MAJOR_TYPE, pguidMajorType);
    }
    else
        ret = MF_E_NOT_INITIALIZED;

    ThreadRelease();
    return ret;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMediaTypeByIndex(DWORD dwIndex, _Outptr_ IMFMediaType** ppType)
{
    if (!ppType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    if (dwIndex >= s_dwNumVideoFormats)
        return MF_E_NO_MORE_TYPES;

    IMFMediaType* pType = nullptr;

    ThreadLock();

    HRESULT ret = MFCreateMediaType(&pType);
    if (FAILED(ret))
        goto done;

    ret = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    if (FAILED(ret))
        goto done;

    ret = pType->SetGUID(MF_MT_SUBTYPE, *(s_pVideoFormats[dwIndex]));
    if (FAILED(ret))
        goto done;

    pType->AddRef();
    *ppType = pType;

done:
    if (pType) { pType->Release(); pType = nullptr; }
    ThreadRelease();
    return ret;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMediaTypeCount(__RPC__out DWORD* pdwTypeCount)
{
    if (!pdwTypeCount)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    *pdwTypeCount = s_dwNumVideoFormats;
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::IsMediaTypeSupported(IMFMediaType* pMediaType, _Outptr_opt_result_maybenull_ IMFMediaType** ppMediaType)
{
    if (!pMediaType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    

    GUID subType = GUID_NULL;
    HRESULT ret = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType);
    if (FAILED(ret))
        return ret;
    ret = MF_E_INVALIDMEDIATYPE;

    for (DWORD i = 0; i < s_dwNumVideoFormats; i++)
    {
        if (subType == (*s_pVideoFormats[i]))
        {
            ret = S_OK;
            break;
        }
    }
    if(ppMediaType)
        *ppMediaType = nullptr;

    return ret;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::SetCurrentMediaType(IMFMediaType* pMediaType)
{
    if (!pMediaType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    ThreadLock();
    HRESULT ret = IsMediaTypeSupported(pMediaType, nullptr);

    MFRatio fps = { 0, 0 };

    if (FAILED(ret))
        goto done;

    pMediaType->GetUINT32(MF_MT_INTERLACE_MODE, &m_unInterlaceMode);

    // Set the frame rate on the scheduler.
    if (SUCCEEDED(MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, (UINT32*)&fps.Numerator, (UINT32*)&fps.Denominator)) 
        && (fps.Numerator != 0) && (fps.Denominator != 0))
    {
        if (MFVideoInterlace_FieldInterleavedUpperFirst == m_unInterlaceMode ||
            MFVideoInterlace_FieldInterleavedLowerFirst == m_unInterlaceMode ||
            MFVideoInterlace_FieldSingleUpper == m_unInterlaceMode ||
            MFVideoInterlace_FieldSingleLower == m_unInterlaceMode ||
            MFVideoInterlace_MixedInterlaceOrProgressive == m_unInterlaceMode)
        {
            fps.Numerator *= 2;
        }

        schedule->SetFrameRate(fps);
    }
    else
    {
        // NOTE: The mixer's proposed type might not have a frame rate, in which case
        // we'll use an arbitary default. (Although it's unlikely the video source
        // does not have a frame rate.)
        schedule->SetFrameRate(s_DefaultFrameRate);
    }

    

    if (currType)
        currType->Release();
    if (SUCCEEDED(MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &textureDesc.Width, &textureDesc.Height)))
    {
        currType = pMediaType;
        currType->AddRef();
    }
    if (PlayState::State_Started != this->state && PlayState::State_Paused != state)
    {
        state = PlayState::State_Ready;
    }
    else
    {
        //Flush all current samples in the Queue as this is a format change
        ret = Flush();
    }

    done:
    ThreadRelease();

    return ret;
}

TrecComPointer<IMFStreamSink> TStreamSink::GetStreamSink(TrecComPointer<TMediaSink> sink, TrecPointer<DrawingBoard> board)
{
    if(!sink.Get() || !board.Get() || !board->GetWindowEngine().Get())
        return TrecComPointer<IMFStreamSink>();


    TrecComPointer<IMFStreamSink>::TrecComHolder holder;
    auto engine = board->GetWindowEngine();
    TStreamSink* tsink = new TStreamSink(TPresenter::GetTPresenter(engine, board));

    IMFStreamSink** mem = holder.GetPointerAddress();
    IMFStreamSink* isink = (IMFStreamSink*)tsink;
    *mem = isink;
    tsink->m_nRefCount = 1;
    tsink->isShutdown = false;
    tsink->device = engine->getDeviceD();
    return holder.Extract();
}

HRESULT TStreamSink::Initialize()
{
    HRESULT ret = S_OK;
    TObject::ThreadLock();
    ret = MFCreateEventQueue(&this->m_pEventQueue);
    if (SUCCEEDED(ret))
        ret = MFAllocateWorkQueue(&queueId);
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::Pause(void)
{
    TObject::ThreadLock();
    HRESULT ret = (state == PlayState::State_TypeNotSet || state == PlayState::State_Stopped) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        state = PlayState::State_Paused;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpPause);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::Preroll(void)
{
    TObject::ThreadLock();
    HRESULT ret = MF_E_SHUTDOWN;
    if (!isShutdown)
    {
        isPrerolling = waitForClock = true;
        this->sampleRequests++;
        ret = S_OK;
        ret = QueueEvent(MEStreamSinkRequestSample, GUID_NULL, ret, nullptr);
    }
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::Restart(void)
{
    TObject::ThreadLock();
    HRESULT ret = (state != PlayState::State_Ready && state != PlayState::State_Paused) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        state = PlayState::State_Paused;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpRestart);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::Shutdown(void)
{
    if (!m_pEventQueue)
        return E_POINTER;
    ThreadLock();
    m_pEventQueue->Shutdown();
    m_pEventQueue->Release();
    m_pEventQueue = nullptr;
    isShutdown = true;
    ThreadRelease();
    return S_OK;
}

HRESULT TStreamSink::Start(MFTIME start)
{
    TObject::ThreadLock();
    HRESULT ret = (state == PlayState::State_TypeNotSet) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        if (start != PRESENTATION_CURRENT_POSITION)
            startTime = start;
        state = PlayState::State_Started;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpStart);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    waitForClock = false;
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::Stop(void)
{
    TObject::ThreadLock();
    HRESULT ret = (state == PlayState::State_TypeNotSet) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        state = PlayState::State_Stopped;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpStop);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    TObject::ThreadRelease();
    return ret;
}

HRESULT TStreamSink::DispatchEvent(IMFAsyncResult* result)
{
    ThreadLock();

    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    IUnknown* pState = nullptr;
    HRESULT ret = result->GetState(&pState);
    if (SUCCEEDED(ret))
    {
        TAsyncOp* asyncOp = (TAsyncOp*)pState;
        StreamOperation op = asyncOp->m_op;

        switch (op)
        {
        case StreamOperation::OpStart:
        case StreamOperation::OpRestart:
            ret = QueueEvent(MEStreamSinkStarted, GUID_NULL, ret, nullptr);
            
            if (SUCCEEDED(ret))
            {
                sampleRequests++;
                ret = QueueEvent(MEStreamSinkRequestSample, GUID_NULL, ret, NULL);
            }

            if (SUCCEEDED(ret))
            {
                ret = ProcessQueueSamples(processFrames);
            }
            break;
        case StreamOperation::OpStop:
            presenter->SetFullscreen(FALSE);
            Flush();
            sampleRequests = 0;
            ret = QueueEvent(MEStreamSinkStopped, GUID_NULL, ret, nullptr);
            break;
        case StreamOperation::OpPause:
            ret = QueueEvent(MEStreamSinkPaused, GUID_NULL, ret, nullptr);
            break;
        case StreamOperation::OpProcessSample:
        case StreamOperation::OpPlaceMarker:
            if (!waitForClock)
            {
                ret = this->DispatchSample(asyncOp);
            }
            break;
        }
    }

    if (pState)
        pState->Release();

    ThreadRelease();
    return ret;
}

HRESULT TStreamSink::PresentFrame()
{
    ThreadLock();
    HRESULT ret = S_OK;

    if (processFrames)
    {
        ret = isShutdown ? MF_E_SHUTDOWN : presenter->PresentFrame();
    }
    if (SUCCEEDED(ret))
    {
        if (state == PlayState::State_Stopped || state == PlayState::State_Paused)
        {
            TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpProcessSample);
            ret = MFPutWorkItem(queueId, &callBack, newOp);
            newOp->Release();
        }
    }
    else
    {
        ret = QueueEvent(MEError, GUID_NULL, ret, nullptr);
    }

    ThreadRelease();
    return ret;
}

bool TStreamSink::IsActive()
{
    return ((state == PlayState::State_Started) || (state == PlayState::State_Paused));
}

HRESULT TStreamSink::ProcessQueueSamples(bool doProcess)
{
    HRESULT ret = S_OK;
    IUnknown* samp = nullptr;

    bool repeat = true;

    while (samples.Dequeue(samp) && repeat)
    {
        IVideoMarker* mSample = nullptr;
        TSampleTexture* fSample = nullptr;

        if ((ret = samp->QueryInterface<IVideoMarker>(&mSample)) == E_NOINTERFACE)
            ret = samp->QueryInterface<TSampleTexture>(&fSample);

        if (SUCCEEDED(ret))
        {
            if (mSample)
            {
                TVideoMarker* tMSample = (TVideoMarker*)mSample;
                PROPVARIANT var;
                PropVariantInit(&var);

                ret = tMSample->GetContext(&var);
                if (SUCCEEDED(ret))
                    ret = QueueEvent(MEStreamSinkMarker, GUID_NULL, doProcess ? S_OK : E_ABORT, &var);

                PropVariantClear(&var);
                mSample->Release();
            }
            else if(doProcess)
            {
                UINT laceMode = 0;
                BOOL deviceChanged = false;
                BOOL processAgain = false;
                IMFSample* outSamp = nullptr;
                ret = presenter->ProcessFrame(this->currType, fSample, &laceMode, &deviceChanged, &outSamp);

                if (SUCCEEDED(ret))
                {
                    if (deviceChanged)
                        QueueEvent(MEStreamSinkDeviceChanged, GUID_NULL, S_OK, nullptr);
                    if (processAgain)
                        samples.PushHead(samp);
                }
                if (SUCCEEDED(ret))
                {
                    if (outSamp)
                    {
                        ret = schedule->ScheduleSample(fSample, state != PlayState::State_Started);
                        repeat = false;
                    }
                }
                fSample->Release();
                if (outSamp)
                    outSamp->Release();
            }
        }
    }


    return ret;
}

HRESULT TStreamSink::DispatchSample(TAsyncOp* op)
{
    if(isShutdown)
        return MF_E_SHUTDOWN;

    HRESULT ret = ProcessQueueSamples(true);

    if (SUCCEEDED(ret) && op->m_op == StreamOperation::OpProcessSample)
    {
        ret = RequestSample();
    }
    return ret;
}

HRESULT TStreamSink::RequestSample()
{
    HRESULT ret = S_OK;

    while ((sampleRequests + samples.GetSize()) < 5)
    {
        if (isShutdown)
        {
            ret = MF_E_SHUTDOWN;
            break;
        }

        sampleRequests++;
        ret = QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr);

        if (FAILED(ret))
            ret = QueueEvent(MEError, GUID_NULL, ret, nullptr);
    }

    return ret;
}

TStreamSink::TStreamSink(TrecComPointer<TPresenter> present):
    callBack(this, &TStreamSink::DispatchEvent)

{
    presenter = present;
    state = PlayState::State_TypeNotSet;
    isShutdown = waitForClock = isPrerolling = false;
    processFrames = true;
    queueId = 0;
    startTime = 0;
    sampleRequests = 0;
    m_unInterlaceMode = 0;

    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_NV12;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DECODER;
    textureDesc.Height = textureDesc.Width = 0; // Set those when we have samples to process
}

bool TStreamSink::CheckShutdown()
{
    return isShutdown;
}

TStreamSink::TAsyncOp::TAsyncOp(StreamOperation op)
{
    m_op = op;
    refCount = 1;
}

STDMETHODIMP_(ULONG __stdcall) TStreamSink::TAsyncOp::AddRef(void)
{
    return InterlockedIncrement(&refCount);
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::TAsyncOp::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
        return E_POINTER;
    if (iid == IID_IUnknown)
        *ppv = static_cast<IUnknown*>(this);
    else{
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG __stdcall) TStreamSink::TAsyncOp::Release(void)
{
    auto ret = InterlockedDecrement(&refCount);
    if (!ret)
        delete this;
    return ret;
}
