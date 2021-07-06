#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <TrecReference.h>
#include <TDataArray.h> 
#include "DrawingBoard.h"
#include "TScheduler.h"
#include "TPresenter.h"

/**
 * Class: TMediaSink
 * Purpose: Anagame's custom implementation of the IMFMediaSink Interface, allowing Anagame to Create one without using an out of date HWND 
 */
class TMediaSink :
    public TObject,             // Thread Protection
    public IMFMediaSink,        // Interface that the Media Session interacts with
    public IMFClockStateSink,   // Required for Media Sinks
    public IMFMediaSinkPreroll  // 
{
public:
    // Static method to create the object.

    /**
     * Method: TMediaSink::CreateInstance
     * Purpose: Provides external code a way to obtain a TMediaSink
     * Parameters: TrecPointer<DrawingBoard> board - the Resource the sink needs to operate effectively
     * Returns: TrecComPointer<TMediaSink> - the Requested Object
     * 
     * Attributes: static
     */
    static TrecComPointer<TMediaSink> CreateInstance(TrecPointer<DrawingBoard> board);

    // IUnknown

    /**
     * Method: TMediaSink::AddRef
     * Purpose: Increments the COM Reference
     * Parameters: void
     * Returns: ULONG - the new counter
     */
    STDMETHODIMP_(ULONG) AddRef(void) override;
    /**
     * Method: TMediaSink::QueryInterface
     * Purpose: Retrieves a pointer to an object based off of the specified interface
     * Parameters: REFIID riid - the id of the interface to get
     *              void** ppv - where to place the pointer, if riid is supported
     * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
     * 
     * Note: Supported Interfaces are IUnknown, IMFMediaSink, IMFClockStateSink, and IMFSinkPreroll
     */
    STDMETHODIMP QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv) override;

    /**
     * Method: TMediaSink::Release
     * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
     * Parameters: void
     * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
     */
    STDMETHODIMP_(ULONG) Release(void) override;

    // IMFMediaSink methods

    /**
     * Method: TMediaSink::AddStreamSink
     * Purpose: Filler Method to implement the IMFMediaSink Interface, supposed to add a Stream sink, but this class provisions its own stream sink
     * Parameters: DWORD dwStreamSinkIdentifier - The id of the sink
     *           IMFMediaType* pMediaType - the type of media this stream sink handles
     *           IMFStreamSink** ppStreamSink - the stream sink Added
     * Returns: HESULT - MF_E_STREAMSINKS_FIXED (this media sink provisions its own stream sink)
     */
    STDMETHODIMP AddStreamSink(DWORD dwStreamSinkIdentifier, __RPC__in_opt IMFMediaType* pMediaType, __RPC__deref_out_opt IMFStreamSink** ppStreamSink);
    /**
     * Method: TMediaSink::GetCharacteristics
     * Purpose: Reports that this media sink has a fixed number of stream sinks
     * Parameters: DWORD* pdwCharacteristics - pointer to memory that holds the characteristics
     * Returns: HESULT - E_POINTER if parameter is null, MF_E_SHUTDOWN if shutdown, or S_OK
     */
    STDMETHODIMP GetCharacteristics(__RPC__out DWORD* pdwCharacteristics);
    /**
     * Method: TMediaSink::GetPresentationClock
     * Purpose: Returns the Presentation clock used by this sink
     * Parameters: IMFPresentationClock** ppPresentationClock - holds a pointer to the presentation clock used
     * Returns: HESULT - E_POINTER if holder is null, MF_E_SHUTDOWN if shutting down, MF_E_NO_CLOCK if clock is not set, or S_OK
     */
    STDMETHODIMP GetPresentationClock(__RPC__deref_out_opt IMFPresentationClock** ppPresentationClock);
    /**
     * Method: TMediaSink::GetStreamSinkById
     * Purpose: Retrieves the Stream Sink
     * Parameters: DWORD dwIdentifier - id if the stream sink (expected to be 1, else MF_E_INVALIDSTREAMNUMBER is returned)
     *              IMFStreamSink** ppStreamSink - holder for the Stream Sink held
     * Returns: HESULT - E_POINTER (if null holder is provided, MF_E_SHUTDOWN if shutting down, or S_OK)
     */
    STDMETHODIMP GetStreamSinkById(DWORD dwIdentifier, __RPC__deref_out_opt IMFStreamSink** ppStreamSink);
    /**
     * Method: TMediaSink::GetStreamSinkByIndex
     * Purpose: Retrieves the Stream Sink
     * Parameters: DWORD dwIndex - index if the stream sink (expected to be 0, else MF_E_INVALIDINDEX is returned)
     *              IMFStreamSink** ppStreamSink - holder for the Stream Sink held
     * Returns: HESULT - E_POINTER (if null holder is provided, MF_E_SHUTDOWN if shutting down, or S_OK)
     */
    STDMETHODIMP GetStreamSinkByIndex(DWORD dwIndex, __RPC__deref_out_opt IMFStreamSink** ppStreamSink);
    /**
     * Method: TMediaSink::GetStreamSinkCount
     * Purpose: Reports to the Session that this Media Sink has one Stream Sink
     * Parameters: DWORD* pcStreamSinkCount - Pointer to hold 1
     * Returns: HESULT - E_POINTER if param is null, MF_E_SHUTDOWN if shutting down, or S_OK
     */
    STDMETHODIMP GetStreamSinkCount(__RPC__out DWORD* pcStreamSinkCount);
    /**
     * Method: TMediaSink::RemoveStreamSink
     * Purpose: Filler Method to implement the IMFMediaSink Interface, supposed to add a Stream sink, but this class provisions its own stream sink
     * Parameters: DWORD dwStreamSinkIdentifier -  the id of the sink to remove
     * Returns: HESULT - MF_E_STREAMSINKS_FIXED
     */
    STDMETHODIMP RemoveStreamSink(DWORD dwStreamSinkIdentifier);
    /**
     * Method: TMediaSink::SetPresentationClock
     * Purpose: Sets the Presentation clock to use to sync the frames
     * Parameters: IMFPresentationClock* pPresentationClock - the clock to hold
     * Returns: HESULT - E_POINTER if param is null, MF_E_SHUTDOWN if shutting down, or S_OK
     */
    STDMETHODIMP SetPresentationClock(__RPC__in_opt IMFPresentationClock* pPresentationClock);
    /**
     * Method: TMediaSink::Shutdown
     * Purpose: Initiates Shutdown procedure, causing most other methods to return MF_E_SHUTDOWN that would have previously been S_OK
     * Parameters: void
     * Returns: HESULT - MF_E_SHUTDOWN
     */
    STDMETHODIMP Shutdown(void);

    // IMFClockStateSink

    /**
     * Method: TMediaSink::OnClockPause
     * Purpose: Pauses the sink
     * Parameters: MFTIME time - not used
     * Returns: HESULT - MF_E_SHUTDOWN if shutting down or what the Stream Sink reports
     */
    HRESULT STDMETHODCALLTYPE OnClockPause(MFTIME time) override;
    /**
     * Method: TMediaSink::OnClockRestart
     * Purpose: Restarts the Sink
     * Parameters: MFTIME time - not used
     * Returns: HESULT - MF_E_SHUTDOWN if shutting down or what the Stream Sink reports
     */
    HRESULT STDMETHODCALLTYPE OnClockRestart(MFTIME time) override;
    /**
     * Method: TMediaSink::OnClockSetRate
     * Purpose: Sets the clock rate
     * Parameters: MFTIME time - not used
     *              float rate
     * Returns: HESULT - E_NOT_SET if scheduler is not set or the result that the scheduler reports
     */
    HRESULT STDMETHODCALLTYPE OnClockSetRate(MFTIME time, float rate) override;
    /**
     * Method: TMediaSink::OnClockStart
     * Purpose: Initiates a Start operation
     * Parameters: MFTIME time - not used
     *              LONGLONG startOffset - sends to the stream sink
     * Returns: HESULT - S_OK, MF_E_SHUTDOWN, or the Error from the corresponding operation if the TStreamSink or TPresenter
     */
    HRESULT STDMETHODCALLTYPE OnClockStart(MFTIME time, LONGLONG startOffset) override;
    /**
     * Method: TMediaSink::OnClockStop
     * Purpose: Initiates a Stop operation
     * Parameters: MFTIME time - not used
     * Returns: HESULT - S_OK, MF_E_SHUTDOWN, or the Error from the corresponding operation if the TStreamSink or TPresenter
     */
    HRESULT STDMETHODCALLTYPE OnClockStop(MFTIME time) override;

    // IMFMediaSinkPreroll

    /**
     * Method: TMediaSink::NotifyPreroll
     * Purpose: Initiates a preroll operation
     * Parameters: MFTIME hnsUpcomingStartTime - not used
     * Returns: HESULT - Returns whatever the Stream sink returns in the "Preroll" method
     */
    STDMETHODIMP NotifyPreroll(MFTIME hnsUpcomingStartTime);

    // Anagame Alternative Methods

    /**
     * Method: TMediaSink::GetPresenter
     * Purpose: Returns the Presenter being used 
     * Parameters: void
     * Returns: TrecComPointer<TPresenter> - the Presenter held
     */
    TrecComPointer<TPresenter> GetPresenter();

private:
    /**
     * Method: TMediaSink::AddStreamSink
     * Purpose: Sets the Stream Sink internal by Anagame's standards
     * Parameters: TrecComPointer<IMFStreamSink> sink - holds a pointer to the stream sink
     * Returns: bool - true since Anagame's framework will provide a legitimate sink
     */
    bool AddStreamSink(TrecComPointer<IMFStreamSink> sink);
    /**
     * Method: TMediaSink::TMediaSink
     * Purpose: Constructor
     * Parameters: void
     * Returns: New TMediaSink object that implements the IMFMediaSink interface
     */
    TMediaSink();

    /**
     * Helps Synchronize streams so frames match up with the audio
     */
    IMFPresentationClock* m_clock;

    /**
     * How many COM References currently exist
     */
    long m_nRefCount;
    /**
     * Is currently in shutdown mode
     */
    bool isShutdown;
    /**
     * The Sreamsink that this media sink uses 
     */
    TrecComPointer<IMFStreamSink> streamSink;
    /**
     * Manages the scheduler
     */
    TrecPointer<TScheduler> schedule;
};

