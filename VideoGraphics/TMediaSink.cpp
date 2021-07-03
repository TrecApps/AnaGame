#include "TMediaSink.h"
#include "TStreamSink.h"
#include <Shlwapi.h>
#include <Mferror.h>

TMediaSink::TMediaSink()
{
    this->m_nRefCount = 1;
    this->isShutdown = false;
    this->m_clock = nullptr;
}

TrecComPointer<TMediaSink> TMediaSink::CreateInstance(TrecPointer<DrawingBoard> board)
{
    TrecComPointer<TMediaSink>::TrecComHolder holder;
    TMediaSink* msink = new TMediaSink();
    msink->isShutdown = false;
    msink->m_clock = nullptr;
    msink->m_nRefCount = 1;
    TMediaSink** hsink = holder.GetPointerAddress();
    *hsink = msink;

    auto ret = holder.Extract();

    auto streamsink = TStreamSink::GetStreamSink(ret, board);

    if(!streamsink.Get())
       return TrecComPointer<TMediaSink>();
    ret->AddStreamSink(streamsink);
    
    dynamic_cast<TStreamSink*>(streamsink.Get())->Initialize();

    ret->schedule = TrecPointerKey::GetNewTrecPointer<TScheduler>(streamsink);
    dynamic_cast<TStreamSink*>(streamsink.Get())->schedule = ret->schedule;
    dynamic_cast<TStreamSink*>(streamsink.Get())->mediaSink = ret;
    return ret;
}

/**
 * Method: TMediaSink::AddRef
 * Purpose: Increments the COM Reference
 * Parameters: void
 * Returns: ULONG - the new counter
 */
ULONG TMediaSink::AddRef(void)
{
    return InterlockedIncrement(&m_nRefCount);
}

/**
 * Method: TMediaSink::QueryInterface
 * Purpose: Retrieves a pointer to an object based off of the specified interface
 * Parameters: REFIID riid - the id of the interface to get
 *              void** ppv - where to place the pointer, if riid is supported
 * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
 *
 * Note: Supported Interfaces are IUnknown, IMFMediaSink, IMFClockStateSink, and IMFSinkPreroll
 */
HRESULT TMediaSink::QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(static_cast<IMFMediaSink*>(this));
    }
    else if (riid == __uuidof(IMFMediaSink))
    {
        *ppv = static_cast<IMFMediaSink*>(this);
    }
    else if (riid == __uuidof(IMFClockStateSink))
    {
        *ppv = static_cast<IMFClockStateSink*>(this);
    }
    else if (riid == IID_IMFMediaSinkPreroll)
    {
        *ppv = static_cast<IMFMediaSinkPreroll*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

/**
 * Method: TMediaSink::Release
 * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
 * Parameters: void
 * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
 */
ULONG TMediaSink::Release(void)
{
    ULONG c = InterlockedDecrement(&m_nRefCount);
    if (!c)
    {
        delete this;
    }
    return c;
}

HRESULT TMediaSink::AddStreamSink(DWORD dwStreamSinkIdentifier, __RPC__in_opt IMFMediaType* pMediaType, __RPC__deref_out_opt IMFStreamSink** ppStreamSink)
{
    return MF_E_STREAMSINKS_FIXED;
}

HRESULT TMediaSink::GetCharacteristics(__RPC__out DWORD* pdwCharacteristics)
{
    if (!pdwCharacteristics)
        return E_POINTER;
    ThreadLock();
    bool tempShutdown = isShutdown;
    ThreadRelease();
    if (tempShutdown)
        return MF_E_SHUTDOWN;
    *pdwCharacteristics = MEDIASINK_FIXED_STREAMS;// | MEDIASINK_CAN_PREROLL;
    return S_OK;
}

HRESULT TMediaSink::GetPresentationClock(__RPC__deref_out_opt IMFPresentationClock** ppPresentationClock)
{
    if (!ppPresentationClock)
        return E_POINTER;
    ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    if (!m_clock)
    {
        ThreadRelease();
        return MF_E_NO_CLOCK;
    }
    *ppPresentationClock = m_clock;
    m_clock->AddRef();
    ThreadRelease();

    return S_OK;
}

HRESULT TMediaSink::GetStreamSinkById(DWORD dwIdentifier, __RPC__deref_out_opt IMFStreamSink** ppStreamSink)
{
    if (dwIdentifier != 1)
        return MF_E_INVALIDSTREAMNUMBER;
    if (!ppStreamSink)
        return E_POINTER;
    ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    auto ret = streamSink.Get();
    if (ret)
        ret->AddRef();
    *ppStreamSink = ret;
    ThreadRelease();
    return S_OK;
}

HRESULT TMediaSink::GetStreamSinkByIndex(DWORD dwIndex, __RPC__deref_out_opt IMFStreamSink** ppStreamSink)
{
    if (dwIndex > 0)
        return MF_E_INVALIDINDEX;
    if (!ppStreamSink)
        return E_POINTER;
    ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    auto ret = streamSink.Get();
    if (ret)
        ret->AddRef();
    *ppStreamSink = ret;
    ThreadRelease();
    return S_OK;
}

HRESULT TMediaSink::GetStreamSinkCount(__RPC__out DWORD* pcStreamSinkCount)
{
    ThreadLock();
    if (!pcStreamSinkCount)
    {
        ThreadRelease();
        return E_POINTER;
    }

    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    *pcStreamSinkCount = 1;
    ThreadRelease();
    return S_OK;
}

HRESULT TMediaSink::RemoveStreamSink(DWORD dwStreamSinkIdentifier)
{
    return MF_E_STREAMSINKS_FIXED;
}

HRESULT TMediaSink::SetPresentationClock(__RPC__in_opt IMFPresentationClock* pPresentationClock)
{
    
    if (!pPresentationClock)
        return E_POINTER;
    TObject::ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    if (m_clock)
        m_clock->RemoveClockStateSink(this);
    m_clock = pPresentationClock;
    m_clock->AddClockStateSink(this);
    TObject::ThreadRelease();
    return S_OK;
}

HRESULT TMediaSink::Shutdown(void)
{
    ThreadLock();
    isShutdown = true;
    ThreadRelease();
    return MF_E_SHUTDOWN;
}

HRESULT TMediaSink::OnClockPause(MFTIME time)
{
    ThreadLock();
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Pause();
    ThreadRelease();
    return ret;
}

HRESULT TMediaSink::OnClockRestart(MFTIME time)
{
    ThreadLock();
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Restart();
    ThreadRelease();
    return ret;
}

HRESULT TMediaSink::OnClockSetRate(MFTIME time, float rate)
{
    ThreadLock();
    HRESULT ret = schedule.Get() ? schedule->SetClockRate(rate) : E_NOT_SET;
    ThreadRelease();
    return ret;
}

HRESULT TMediaSink::OnClockStart(MFTIME time, LONGLONG startOffset)
{
    ThreadLock();
    HRESULT ret = S_OK;
    if (isShutdown)
        ret = MF_E_SHUTDOWN;
    else {
        TStreamSink* streamer = reinterpret_cast<TStreamSink*>(streamSink.Get());
        assert(streamer);
        if (streamer->IsActive())
            ret = streamer->Flush();
        else if (schedule.Get())
            ret = schedule->StartScheduler(m_clock);
        
        if(SUCCEEDED(ret))streamer->Start(startOffset);
    }
    ThreadRelease();
    return ret;
}

HRESULT TMediaSink::OnClockStop(MFTIME time)
{
    ThreadLock();
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Stop();

    if (SUCCEEDED(ret))
    {
        ret = schedule->StopScheduler();
    }

    ThreadRelease();
    return ret;
}

STDMETHODIMP_(HRESULT __stdcall) TMediaSink::NotifyPreroll(MFTIME hnsUpcomingStartTime)
{
    ThreadLock();
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Preroll();
    ThreadRelease();

    return ret;
}

TrecComPointer<TPresenter> TMediaSink::GetPresenter()
{
    ThreadLock();
    TStreamSink* sSink = dynamic_cast<TStreamSink*>(streamSink.Get());
    TrecComPointer<TPresenter> ret;
    if (sSink)
    {
        ret = sSink->presenter;
    }

    ThreadRelease();
    return ret;
}

bool TMediaSink::AddStreamSink(TrecComPointer<IMFStreamSink> sink)
{
    ThreadLock();
    bool ret = false;
    if (sink.Get())
    {
        this->streamSink = sink;
        ret = true;
    }
    ThreadRelease();
    return ret;
}
