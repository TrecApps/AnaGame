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
    return E_NOTIMPL;
}

HRESULT TStreamSink::EndGetEvent(IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent)
{
    return E_NOTIMPL;
}

HRESULT TStreamSink::GetEvent(DWORD dwFlags, __RPC__deref_out_opt IMFMediaEvent** ppEvent)
{
    return E_NOTIMPL;
}

HRESULT TStreamSink::QueueEvent(MediaEventType met, __RPC__in REFGUID guidExtendedType, HRESULT hrStatus, __RPC__in_opt const PROPVARIANT* pvValue)
{
    return E_NOTIMPL;
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

    ret = MFCreateEventQueue(&this->m_pEventQueue);

    return ret;
}

HRESULT TStreamSink::Pause(void)
{
    return E_NOTIMPL;
}

HRESULT TStreamSink::Preroll(void)
{
    return E_NOTIMPL;
}

HRESULT TStreamSink::Restart(void)
{
    return E_NOTIMPL;
}

HRESULT TStreamSink::Shutdown(void)
{
    if (!m_pEventQueue)
        return E_POINTER;
    m_pEventQueue->Shutdown();
    m_pEventQueue->Release();
    m_pEventQueue = nullptr;
    return S_OK;
}

HRESULT TStreamSink::Start(MFTIME start)
{
    return E_NOTIMPL;
}

HRESULT TStreamSink::Stop(void)
{
    return E_NOTIMPL;
}

TStreamSink::TStreamSink(TrecComPointer<TPresenter> present)
{
    presenter = present;
}

bool TStreamSink::CheckShutdown()
{
    return isShutdown;
}
