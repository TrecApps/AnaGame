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
    return E_NOTIMPL;
}

HRESULT TStreamSink::PresentFrame()
{
    return E_NOTIMPL;
}

TStreamSink::TStreamSink(TrecComPointer<TPresenter> present):
    callBack(this, TStreamSink::DispatchEvent)

{
    presenter = present;
    state = PlayState::State_TypeNotSet;
    isShutdown = false;
    processFrames = true;
    queueId = 0;
    startTime = 0;
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
