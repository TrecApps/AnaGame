#include "TStreamSink.h"
#include <Shlwapi.h>
#include <Mferror.h>
#include "TVideoMarker.h"
#include <mfapi.h>

STDMETHODIMP_(ULONG __stdcall) TStreamSink::AddRef(void)
{
    return InterlockedIncrement(&m_nRefCount);
}

HRESULT TStreamSink::QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    static const QITAB qit[] = {
    QITABENT(TStreamSink, IMFStreamSink), {0}
    };
    return QISearch(this, qit, riid, ppv);
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
    /*ThreadLock();
    samples.Flush();
    ThreadRelease();*/
    //processFrames = false;
    return E_NOTIMPL;
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
    auto uSample = (IUnknown*)pSample;
    samples.Push(uSample);

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

TrecComPointer<IMFStreamSink> TStreamSink::GetStreamSink(TrecComPointer<TMediaSink> sink, TrecPointer<DrawingBoard> board)
{
    if(!sink.Get() || !board.Get() || !board->GetWindowEngine().Get())
        return TrecComPointer<IMFStreamSink>();


    TrecComPointer<IMFStreamSink>::TrecComHolder holder;
    TStreamSink* tsink = new TStreamSink(TPresenter::GetTPresenter(board->GetWindowEngine(), board));

    auto mem = holder.GetPointerAddress();
    auto isink = (IMFStreamSink*)tsink;
    mem = &isink;
    tsink->m_nRefCount = 1;
    tsink->isShutdown = false;
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
    ///TObject::ThreadLock();
    //HRESULT ret = S_OK;
    //TObject::ThreadRelease();
    //return ret;
    return E_NOTIMPL;
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
    return E_NOTIMPL;
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

HRESULT TStreamSink::ProcessQueueSamples(bool doProcess)
{
    HRESULT ret = S_OK;
    IUnknown* samp = nullptr;

    bool repeat = true;

    while (samples.Dequeue(samp) && repeat)
    {
        TVideoMarker* mSample = nullptr;
        IMFSample* fSample = nullptr;

        if ((ret = samp->QueryInterface<TVideoMarker>(&mSample)) == E_NOINTERFACE)
            ret = samp->QueryInterface<IMFSample>(&fSample);

        if (SUCCEEDED(ret))
        {
            if (mSample)
            {
                PROPVARIANT var;
                PropVariantInit(&var);

                ret = mSample->GetContext(&var);
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
                ret = presenter->ProcessFrame(nullptr, fSample, &laceMode, &deviceChanged, &outSamp);

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
    callBack(this, TStreamSink::DispatchEvent)

{
    presenter = present;
    state = PlayState::State_TypeNotSet;
    isShutdown = waitForClock = false;
    processFrames = true;
    queueId = 0;
    startTime = 0;
    sampleRequests = 0;
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
