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

/**
 * Method: TMediaSink::CreateInstance
 * Purpose: Provides external code a way to obtain a TMediaSink
 * Parameters: TrecPointer<DrawingBoard> board - the Resource the sink needs to operate effectively
 * Returns: TrecComPointer<TMediaSink> - the Requested Object
 *
 * Attributes: static
 */
TrecComPointer<TMediaSink> TMediaSink::CreateInstance(TrecPointer<DrawingBoard> board)
{
    TrecComPointer<TMediaSink>::TrecComHolder holder;   // Prepare to create the object
    TMediaSink* msink = new TMediaSink();               
    msink->isShutdown = false;                          // not in shutdown mode
    msink->m_clock = nullptr;                           // We are supplied with a clock
    msink->m_nRefCount = 1;                             // Have one reference for the COM Pointer
    TMediaSink** hsink = holder.GetPointerAddress();
    *hsink = msink;

    auto ret = holder.Extract();

    auto streamsink = TStreamSink::GetStreamSink(ret, board); // Generate the Stream SInk for this sink

    if(!streamsink.Get())
       return TrecComPointer<TMediaSink>();
    ret->AddStreamSink(streamsink);
    
    dynamic_cast<TStreamSink*>(streamsink.Get())->Initialize();

    // Set up a scheduler
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

/**
 * Method: TMediaSink::AddStreamSink
 * Purpose: Filler Method to implement the IMFMediaSink Interface, supposed to add a Stream sink, but this class provisions its own stream sink
 * Parameters: DWORD dwStreamSinkIdentifier - The id of the sink
 *           IMFMediaType* pMediaType - the type of media this stream sink handles
 *           IMFStreamSink** ppStreamSink - the stream sink Added
 * Returns: HESULT - MF_E_STREAMSINKS_FIXED (this media sink provisions its own stream sink)
 */
HRESULT TMediaSink::AddStreamSink(DWORD dwStreamSinkIdentifier, __RPC__in_opt IMFMediaType* pMediaType, __RPC__deref_out_opt IMFStreamSink** ppStreamSink)
{
    return MF_E_STREAMSINKS_FIXED; // Stream sink is fixed
}

/**
 * Method: TMediaSink::GetCharacteristics
 * Purpose: Reports that this media sink has a fixed number of stream sinks
 * Parameters: DWORD* pdwCharacteristics - pointer to memory that holds the characteristics
 * Returns: HESULT - E_POINTER if parameter is null, MF_E_SHUTDOWN if shutdown, or S_OK
 */
HRESULT TMediaSink::GetCharacteristics(__RPC__out DWORD* pdwCharacteristics)
{
    // Don't write to a null pointer
    if (!pdwCharacteristics)
        return E_POINTER;
    // Retrieve Object Info
    ThreadLock();
    bool tempShutdown = isShutdown;
    ThreadRelease();
    // Handle Shutdown mode
    if (tempShutdown)
        return MF_E_SHUTDOWN;
    *pdwCharacteristics = MEDIASINK_FIXED_STREAMS;// | MEDIASINK_CAN_PREROLL;
    return S_OK;
}

/**
 * Method: TMediaSink::GetPresentationClock
 * Purpose: Returns the Presentation clock used by this sink
 * Parameters: IMFPresentationClock** ppPresentationClock - holds a pointer to the presentation clock used
 * Returns: HESULT - E_POINTER if holder is null, MF_E_SHUTDOWN if shutting down, MF_E_NO_CLOCK if clock is not set, or S_OK
 */
HRESULT TMediaSink::GetPresentationClock(__RPC__deref_out_opt IMFPresentationClock** ppPresentationClock)
{
    // Don't accept a null holder
    if (!ppPresentationClock)
        return E_POINTER;
    // Access object attributes
    ThreadLock();
    // Handle Shutdown case
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    // If not clock set, report that 
    if (!m_clock)
    {
        ThreadRelease();
        return MF_E_NO_CLOCK;
    }

    // Return the clock
    *ppPresentationClock = m_clock;
    m_clock->AddRef();
    ThreadRelease();

    return S_OK;
}

/**
 * Method: TMediaSink::GetStreamSinkById
 * Purpose: Retrieves the Stream Sink
 * Parameters: DWORD dwIdentifier - id if the stream sink (expected to be 1, else MF_E_INVALIDSTREAMNUMBER is returned)
 *              IMFStreamSink** ppStreamSink - holder for the Stream Sink held
 * Returns: HESULT - E_POINTER (if null holder is provided, MF_E_SHUTDOWN if shutting down, or S_OK)
 */
HRESULT TMediaSink::GetStreamSinkById(DWORD dwIdentifier, __RPC__deref_out_opt IMFStreamSink** ppStreamSink)
{
    // Only one valid id
    if (dwIdentifier != 1)
        return MF_E_INVALIDSTREAMNUMBER;
    // Don't use a null holder
    if (!ppStreamSink)
        return E_POINTER;
    ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    // Return the stream sink
    auto ret = streamSink.Get();
    if (ret)
        ret->AddRef();
    *ppStreamSink = ret;
    ThreadRelease();
    return S_OK;
}

/**
 * Method: TMediaSink::GetStreamSinkByIndex
 * Purpose: Retrieves the Stream Sink
 * Parameters: DWORD dwIndex - index if the stream sink (expected to be 0, else MF_E_INVALIDINDEX is returned)
 *              IMFStreamSink** ppStreamSink - holder for the Stream Sink held
 * Returns: HESULT - E_POINTER (if null holder is provided, MF_E_SHUTDOWN if shutting down, or S_OK)
 */
HRESULT TMediaSink::GetStreamSinkByIndex(DWORD dwIndex, __RPC__deref_out_opt IMFStreamSink** ppStreamSink)
{
    // Index must be 0
    if (dwIndex > 0)
        return MF_E_INVALIDINDEX;
    // Don't use a null holder
    if (!ppStreamSink)
        return E_POINTER;
    ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    // Return the stream sink
    auto ret = streamSink.Get();
    if (ret)
        ret->AddRef();
    *ppStreamSink = ret;
    ThreadRelease();
    return S_OK;
}

/**
 * Method: TMediaSink::GetStreamSinkCount
 * Purpose: Reports to the Session that this Media Sink has one Stream Sink
 * Parameters: DWORD* pcStreamSinkCount - Pointer to hold 1
 * Returns: HESULT - E_POINTER if param is null, MF_E_SHUTDOWN if shutting down, or S_OK
 */
HRESULT TMediaSink::GetStreamSinkCount(__RPC__out DWORD* pcStreamSinkCount)
{
    // Handle null pointer
    if (!pcStreamSinkCount)
    {
        return E_POINTER;
    }
    // Handle Shutdown case
    ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    // report the 1 stream sink
    *pcStreamSinkCount = 1;
    ThreadRelease();
    return S_OK;
}

/**
 * Method: TMediaSink::RemoveStreamSink
 * Purpose: Filler Method to implement the IMFMediaSink Interface, supposed to add a Stream sink, but this class provisions its own stream sink
 * Parameters: DWORD dwStreamSinkIdentifier -  the id of the sink to remove
 * Returns: HESULT - MF_E_STREAMSINKS_FIXED
 */
HRESULT TMediaSink::RemoveStreamSink(DWORD dwStreamSinkIdentifier)
{
    return MF_E_STREAMSINKS_FIXED; // fixed stream sink
}

/**
 * Method: TMediaSink::SetPresentationClock
 * Purpose: Sets the Presentation clock to use to sync the frames
 * Parameters: IMFPresentationClock* pPresentationClock - the clock to hold
 * Returns: HESULT - E_POINTER if param is null, MF_E_SHUTDOWN if shutting down, or S_OK
 */
HRESULT TMediaSink::SetPresentationClock(__RPC__in_opt IMFPresentationClock* pPresentationClock)
{
    // Don't accept a Null pointer
    if (!pPresentationClock)
        return E_POINTER;
    // Handle Shutdown case
    TObject::ThreadLock();
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    // Remove this sink from the current clock
    if (m_clock)
        m_clock->RemoveClockStateSink(this);
    // Set the new clock
    m_clock = pPresentationClock;
    // Add this sink to the new clock
    m_clock->AddClockStateSink(this);
    TObject::ThreadRelease();
    return S_OK;
}

/**
 * Method: TMediaSink::Shutdown
 * Purpose: Initiates Shutdown procedure, causing most other methods to return MF_E_SHUTDOWN that would have previously been S_OK
 * Parameters: void
 * Returns: HESULT - MF_E_SHUTDOWN
 */
HRESULT TMediaSink::Shutdown(void)
{
    ThreadLock();
    isShutdown = true;
    ThreadRelease();
    return MF_E_SHUTDOWN;
}

/**
 * Method: TMediaSink::OnClockPause
 * Purpose: Pauses the sink
 * Parameters: MFTIME time - not used
 * Returns: HESULT - MF_E_SHUTDOWN if shutting down or what the Stream Sink reports
 */
HRESULT TMediaSink::OnClockPause(MFTIME time)
{
    ThreadLock();
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Pause();
    ThreadRelease();
    return ret;
}

/**
 * Method: TMediaSink::OnClockRestart
 * Purpose: Restarts the Sink
 * Parameters: MFTIME time - not used
 * Returns: HESULT - MF_E_SHUTDOWN if shutting down or what the Stream Sink reports
 */
HRESULT TMediaSink::OnClockRestart(MFTIME time)
{
    ThreadLock();
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Restart();
    ThreadRelease();
    return ret;
}

/**
 * Method: TMediaSink::OnClockSetRate
 * Purpose: Sets the clock rate
 * Parameters: MFTIME time - not used
 *              float rate
 * Returns: HESULT - E_NOT_SET if scheduler is not set or the result that the scheduler reports
 */
HRESULT TMediaSink::OnClockSetRate(MFTIME time, float rate)
{
    ThreadLock();
    HRESULT ret = schedule.Get() ? schedule->SetClockRate(rate) : E_NOT_SET;
    ThreadRelease();
    return ret;
}

/**
 * Method: TMediaSink::OnClockStart
 * Purpose: Initiates a Start operation
 * Parameters: MFTIME time - not used
 *              LONGLONG startOffset - sends to the stream sink
 * Returns: HESULT - S_OK, MF_E_SHUTDOWN, or the Error from the corresponding operation if the TStreamSink or TPresenter
 */
HRESULT TMediaSink::OnClockStart(MFTIME time, LONGLONG startOffset)
{
    ThreadLock();
    // Assume reutrn of S_OK until dicovered otherwise
    HRESULT ret = S_OK;
    if (isShutdown)
        ret = MF_E_SHUTDOWN; // Handle Shutdown case
    else {
        TStreamSink* streamer = reinterpret_cast<TStreamSink*>(streamSink.Get());
        assert(streamer);
        // Reset the streamer
        if (streamer->IsActive())
            ret = streamer->Flush();
        else if (schedule.Get()) // Scheduler needs to be started
            ret = schedule->StartScheduler(m_clock);
        // Start the stream sink
        if(SUCCEEDED(ret))streamer->Start(startOffset);
    }
    ThreadRelease();
    return ret;
}

/**
 * Method: TMediaSink::OnClockStop
 * Purpose: Initiates a Stop operation
 * Parameters: MFTIME time - not used
 * Returns: HESULT - S_OK, MF_E_SHUTDOWN, or the Error from the corresponding operation if the TStreamSink or TPresenter
 */
HRESULT TMediaSink::OnClockStop(MFTIME time)
{
    ThreadLock();
    // First stop the stream sink
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Stop();

    if (SUCCEEDED(ret))
    {
        ret = schedule->StopScheduler(); // Stop the scheduler
    }

    ThreadRelease();
    return ret;
}


/**
 * Method: TMediaSink::NotifyPreroll
 * Purpose: Initiates a preroll operation
 * Parameters: MFTIME hnsUpcomingStartTime - not used
 * Returns: HESULT - Returns whatever the Stream sink returns in the "Preroll" method
 */
STDMETHODIMP_(HRESULT __stdcall) TMediaSink::NotifyPreroll(MFTIME hnsUpcomingStartTime)
{
    ThreadLock();
    HRESULT ret = isShutdown ? MF_E_SHUTDOWN : reinterpret_cast<TStreamSink*>(streamSink.Get())->Preroll();
    ThreadRelease();

    return ret;
}

/**
 * Method: TMediaSink::GetPresenter
 * Purpose: Returns the Presenter being used
 * Parameters: void
 * Returns: TrecComPointer<TPresenter> - the Presenter held
 */
TrecComPointer<TPresenter> TMediaSink::GetPresenter()
{
    ThreadLock();
    // Presenter is held by the stream sink
    TStreamSink* sSink = dynamic_cast<TStreamSink*>(streamSink.Get());
    TrecComPointer<TPresenter> ret;
    if (sSink)
    {
        ret = sSink->presenter;
    }

    ThreadRelease();
    return ret;
}

/**
 * Method: TMediaSink::AddStreamSink
 * Purpose: Sets the Stream Sink internal by Anagame's standards
 * Parameters: TrecComPointer<IMFStreamSink> sink - holds a pointer to the stream sink
 * Returns: bool - true since Anagame's framework will provide a legitimate sink
 */
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
