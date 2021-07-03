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
    STDMETHODIMP AddStreamSink(DWORD dwStreamSinkIdentifier, __RPC__in_opt IMFMediaType* pMediaType, __RPC__deref_out_opt IMFStreamSink** ppStreamSink);
    STDMETHODIMP GetCharacteristics(__RPC__out DWORD* pdwCharacteristics);
    STDMETHODIMP GetPresentationClock(__RPC__deref_out_opt IMFPresentationClock** ppPresentationClock);
    STDMETHODIMP GetStreamSinkById(DWORD dwIdentifier, __RPC__deref_out_opt IMFStreamSink** ppStreamSink);
    STDMETHODIMP GetStreamSinkByIndex(DWORD dwIndex, __RPC__deref_out_opt IMFStreamSink** ppStreamSink);
    STDMETHODIMP GetStreamSinkCount(__RPC__out DWORD* pcStreamSinkCount);
    STDMETHODIMP RemoveStreamSink(DWORD dwStreamSinkIdentifier);
    STDMETHODIMP SetPresentationClock(__RPC__in_opt IMFPresentationClock* pPresentationClock);
    STDMETHODIMP Shutdown(void);

    // IMFClockStateSink
    HRESULT STDMETHODCALLTYPE OnClockPause(MFTIME time) override;
    HRESULT STDMETHODCALLTYPE OnClockRestart(MFTIME time) override;
    HRESULT STDMETHODCALLTYPE OnClockSetRate(MFTIME time, float rate) override;
    HRESULT STDMETHODCALLTYPE OnClockStart(MFTIME time, LONGLONG startOffset) override;
    HRESULT STDMETHODCALLTYPE OnClockStop(MFTIME time) override;

    // IMFMediaSinkPreroll
    STDMETHODIMP NotifyPreroll(MFTIME hnsUpcomingStartTime);

    // Anagame Alternative Methods
    
    TrecComPointer<TPresenter> GetPresenter();

private:
    bool AddStreamSink(TrecComPointer<IMFStreamSink> sink);
    TMediaSink();

    IMFPresentationClock* m_clock;

    long m_nRefCount;
    bool isShutdown;
    TrecComPointer<IMFStreamSink> streamSink;
    TrecPointer<TScheduler> schedule;
};

