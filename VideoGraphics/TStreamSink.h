#pragma once
#include <mfidl.h>
#include <TObject.h>
#include <TrecReference.h>
#include "TMediaSink.h"
#include <TLinkedList.h>
#include "TPresenter.h"
#include "TComCommon.h"

/**
 * Struct: sFraction
 * Purpose: Represents a fraction
 */
typedef struct sFraction
{
    DWORD Numerator;
    DWORD Denominator;
}sFraction;

/**
 * Class: TStreamSink
 * Purpose: Sub sink within the TMedia Sink that actually manages frames
 */
class TStreamSink : public IMFStreamSink, public IMFMediaTypeHandler, public TObject
{
    friend class TMediaSink;
public:
    // IUnknown
    /**
     * Method: TStreamSink::AddRef
     * Purpose: Increments the COM Reference
     * Parameters: void
     * Returns: ULONG - the new counter
     */
    STDMETHODIMP_(ULONG) AddRef(void) override;
    /**
     * Method: TStreamSink::QueryInterface
     * Purpose: Retrieves a pointer to an object based off of the specified interface
     * Parameters: REFIID riid - the id of the interface to get
     *              void** ppv - where to place the pointer, if riid is supported
     * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
     *
     * Note: Supported Interfaces are IUnknown, IMFStreamSink, and IMFMediaTypeHandler
     */
    STDMETHODIMP QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv) override;
    /**
     * Method: TStreamSink::Release
     * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
     * Parameters: void
     * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
     */
    STDMETHODIMP_(ULONG) Release(void) override;

    // IMFStreamSink

    /**
     * Method: TStreamSink::Flush
     * Purpose: Removes all frames held by the sink
     * Parameters: void
     * Returns: HRESULT - S_OK
     */
    STDMETHODIMP Flush(void) override;
    /**
     * Method: TStreamSink::GetIdentifier
     * Purpose: Retrieves the ID according to the Media Sink
     * Parameters: DWORD* pdwIdentifier - holds the identifier of this sink (1)
     * Returns: HRESULT - S_OK unless parameter is null (E_POINTER) or sink is shutting down (MF_E_SHUTDOWN) 
     */
    STDMETHODIMP GetIdentifier(__RPC__out DWORD* pdwIdentifier) override;
    /**
     * Method: TStreamSink::GetMediaSink
     * Purpose: Returns the Media Sink associaed with this stream sink
     * Parameters: IMFMediaSink** ppMediaSink - holder for media sink pointer
     * Returns: HRESULT - S_OK unless parameter is null (E_POINTER) or sink is null (E_NOT_SET) 
     */
    STDMETHODIMP GetMediaSink(__RPC__deref_out_opt IMFMediaSink** ppMediaSink) override;
    /**
     * Method: TStreamSink::GetMediaTypeHandler
     * Purpose: Returns the Media Type handler Interface (this object)
     * Parameters: IMFMediaTypeHandler** ppHandler - the handler object holder
     * Returns: HRESULT - S_OK unless parameter is null (E_POINTER) or sink is shutting down (MF_E_SHUTDOWN) 
     */
    STDMETHODIMP GetMediaTypeHandler(__RPC__deref_out_opt IMFMediaTypeHandler** ppHandler) override;
    /**
     * Method: TStreamSink::PlaceMarker
     * Purpose: Allows markers to be placed in the stream
     * Parameters: MFSTREAMSINK_MARKER_TYPE eMarkerType - type of marker to deal with
     *              const PROPVARIANT* pvarMarkerValue - the value of the marker
     *              const PROPVARIANT* pvarContextValue - information about the context
     * Returns: HRESULT - S_OK unless parameter is null (E_POINTER), or shutdown is in effect (MF_E_SHUTDOWN)
     */
    STDMETHODIMP PlaceMarker(MFSTREAMSINK_MARKER_TYPE eMarkerType, __RPC__in const PROPVARIANT* pvarMarkerValue, __RPC__in const PROPVARIANT* pvarContextValue) override;
    /**
     * Method: TStreamSink::ProcessSample
     * Purpose: Takes in a sample and attempts to convert it to a texture (not currently working properly, needs inspection)
     * Parameters: IMFSample* pSample - the sample to process and store
     * Returns: HRESULT - S_OK unless parameter is null (E_POINTER), or shutdown is in effect (MF_E_SHUTDOWN), or a DirectX texture generation fails
     */
    STDMETHODIMP ProcessSample(__RPC__in_opt IMFSample* pSample) override;

    // IMFMediaEventGenerator (from IMFStreamSink)

    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP EndGetEvent(IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP GetEvent(DWORD dwFlags, __RPC__deref_out_opt IMFMediaEvent** ppEvent) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP QueueEvent(MediaEventType met, __RPC__in REFGUID guidExtendedType, HRESULT hrStatus, __RPC__in_opt const PROPVARIANT* pvValue) override;

    // IMFMediaTypeHandler

    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP GetCurrentMediaType(_Outptr_ IMFMediaType** ppMediaType) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP GetMajorType(__RPC__out GUID* pguidMajorType) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP GetMediaTypeByIndex(DWORD dwIndex, _Outptr_ IMFMediaType** ppType) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP GetMediaTypeCount(__RPC__out DWORD* pdwTypeCount) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP IsMediaTypeSupported(IMFMediaType* pMediaType, _Outptr_opt_result_maybenull_ IMFMediaType** ppMediaType) override;
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    STDMETHODIMP SetCurrentMediaType(IMFMediaType* pMediaType) override;

    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    static TrecComPointer<IMFStreamSink> GetStreamSink(TrecComPointer<TMediaSink> sink, TrecPointer<DrawingBoard> board);


    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT Initialize();
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT Pause(void);
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT Preroll(void);
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT Restart(void);
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT Shutdown(void);
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT Start(MFTIME start);
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT Stop(void);

    // Callback

    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT DispatchEvent(IMFAsyncResult* result);

    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT PresentFrame();

    //Anagame Methods

    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    bool IsActive();

private:

    /**
     * Enum Class: PlayState
     * Purpose: Manages The current state of the stream sink
     */
    enum class PlayState
    {
        State_TypeNotSet = 0,   // No media type is set
        State_Ready,            // Media type is set, Start has never been called.
        State_Started,
        State_Paused,
        State_Stopped
    };

    /**
     * Enum Class: StreamOperation
     * Purpose: 
     */
    enum class StreamOperation
    {
        OpSetMediaType = 0, // Sets the media type
        OpStart,            // Starts the stream
        OpRestart,          // Restarts the stream
        OpPause,            // Pauses the stream
        OpStop,             // Stops the stream
        OpProcessSample,    // Processes a sample
        OpPlaceMarker       // Places a marker
    };

    class TAsyncOp : public IUnknown
    {
    public:

        TAsyncOp(StreamOperation op);

        // IUnknown methods.
        STDMETHODIMP_(ULONG) AddRef(void);
        STDMETHODIMP QueryInterface(REFIID iid, __RPC__deref_out _Result_nullonfailure_ void** ppv);
        STDMETHODIMP_(ULONG) Release(void);

        // Structure data.
        StreamOperation m_op;   // The operation to perform.

    private:

        long refCount;
    };

    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT ProcessQueueSamples(bool doProcess);
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT DispatchSample(TAsyncOp* op);
    /**
     * Method: TStreamSink::
     * Purpose:
     * Parameters:
     * Returns:
     */
    HRESULT RequestSample();
    /**
     * Current state of the Stream
     */
    PlayState state;
    DWORD queueId;
    DWORD sampleRequests;   // Outstanding reuqests for samples.

    UINT32                      m_unInterlaceMode;

    /**
     * Method: TStreamSink::TStreamSink
     * Purpose: Constructor
     * Parameters: TrecComPointer<TPresenter> present - the presenter to use
     * Returns: new stream sink object
     */
    TStreamSink(TrecComPointer<TPresenter> present);

    bool isShutdown;
    bool processFrames;
    bool waitForClock;
    bool isPrerolling;
    /**
     * Method: TStreamSink::CheckShutdown
     * Purpose: Reports whether the sink is shutting down
     * Parameters: void
     * Returns: bool - whether we are shutting down
     */
    bool CheckShutdown();

    TrecComPointer<TMediaSink> mediaSink;
    TrecComPointer<TPresenter> presenter;
    long m_nRefCount;

    TLinkedList<IUnknown*> samples;
    MFTIME startTime;

    D3D11_TEXTURE2D_DESC textureDesc;
    sFraction m_imageBytesPP;

    // Stream Specific References
    IMFMediaEventQueue* m_pEventQueue;                  // Event queue
    TComCallback<TStreamSink> callBack;                 // Callback mechanism
    IMFMediaType* currType;                             // Type of Media
    TrecPointer<TScheduler> schedule;
    TrecComPointer<ID3D11Device> device;
};

