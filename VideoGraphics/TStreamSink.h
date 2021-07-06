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
     * Method: TStreamSink::BeginGetEvent
     * Purpose: Begins an event
     * Parameters: IMFAsyncCallback* pCallback - call back mthod to call
     *              IUnknown* punkState - the state involved in the event
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
     */
    STDMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState) override;
    /**
     * Method: TStreamSink::EndGetEvent
     * Purpose: Completes an event
     * Parameters: IMFAsyncResult* pResult - Result to return
     *              IMFMediaEvent** ppEvent - the event in question
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
     */
    STDMETHODIMP EndGetEvent(IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent) override;
    /**
     * Method: TStreamSink::GetEvent
     * Purpose: Retrieves the event requested
     * Parameters: DWORD dwFlags - code for the target event
     *              IMFMediaEvent** ppEvent - the event being held
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
     */
    STDMETHODIMP GetEvent(DWORD dwFlags, __RPC__deref_out_opt IMFMediaEvent** ppEvent) override;
    /**
     * Method: TStreamSink::QueueEvent
     * Purpose: Requests an event to be initiated. Many methods of this class run in a seperate thread, out of Anagame's reach
     * Parameters: MediaEventType met - event type
     *              REFGUID guidExtendedType - the GUID type of the event (usually GUID_NULL)
     *              HRESULT hrStatus - status to hold in the event itself
     *              const PROPVARIANT* pvValue
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
     */
    STDMETHODIMP QueueEvent(MediaEventType met, __RPC__in REFGUID guidExtendedType, HRESULT hrStatus, __RPC__in_opt const PROPVARIANT* pvValue) override;

    // IMFMediaTypeHandler

    /**
     * Method: TStreamSink::GetCurrentMediaType
     * Purpose: Retrieves the current media type that has been set
     * Parameters: IMFMediaType** ppMediaType - holder to the current type
     * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or type is not currently set (MF_E_NOT_INITIALIZED)
     */
    STDMETHODIMP GetCurrentMediaType(_Outptr_ IMFMediaType** ppMediaType) override;
    /**
     * Method: TStreamSink::GetMajorType
     * Purpose: Retreives the major type of the media format currently set in the sink
     * Parameters: GUID* pguidMajorType - the current major type being supported
     * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or type is not currently set (MF_E_NOT_INITIALIZED)
     */
    STDMETHODIMP GetMajorType(__RPC__out GUID* pguidMajorType) override;
    /**
     * Method: TStreamSink::GetMediaTypeByIndex
     * Purpose: Returns a Media Type by the index provided
     * Parameters: DWORD dwIndex - the index of the supported type
     *              IMFMediaType** ppType - the type being held (compiled by the method)
     * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or index is out of bounds (MF_E_NO_MORE_TYPES)
     */
    STDMETHODIMP GetMediaTypeByIndex(DWORD dwIndex, _Outptr_ IMFMediaType** ppType) override;
    /**
     * Method: TStreamSink::GetMediaTypeCount
     * Purpose: Reports the number of formas supported by this sink
     * Parameters: DWORD* pdwTypeCount - holds the count
     * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN)
     */
    STDMETHODIMP GetMediaTypeCount(__RPC__out DWORD* pdwTypeCount) override;
    /**
     * Method: TStreamSink::IsMediaTypeSupported
     * Purpose: Allows MF to test a type and deduce if this sink supports it
     * Parameters: IMFMediaType* pMediaType - the type to check
     *              IMFMediaType** ppMediaType - hint to the caller of the closest supported type that is supported
     * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or if type is not supported (MF_E_INVALIDMEDIATYPE)
     */
    STDMETHODIMP IsMediaTypeSupported(IMFMediaType* pMediaType, _Outptr_opt_result_maybenull_ IMFMediaType** ppMediaType) override;
    /**
     * Method: TStreamSink::SetCurrentMediaType
     * Purpose: Updates the Stream about the type of data that the stream will be getting
     * Parameters: IMFMediaType* pMediaType - information about the data we are expected to get
     * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN)
     */
    STDMETHODIMP SetCurrentMediaType(IMFMediaType* pMediaType) override;

    /**
     * Method: TStreamSink::GetStreamSink
     * Purpose: Sets up a stream sink for use with the media sink
     * Parameters: TrecComPointer<TMediaSink> sink - he media sink to attch to
     *              TrecPointer<DrawingBoard> board - the drawing board to work with
     * Returns: TrecComPointer<IMFStreamSink> -  the Stream sink to use
     * 
     * Attributes: static
     */
    static TrecComPointer<IMFStreamSink> GetStreamSink(TrecComPointer<TMediaSink> sink, TrecPointer<DrawingBoard> board);


    /**
     * Method: TStreamSink::Initialize
     * Purpose: Sets up the Queue needed to manage events and run asynchronously
     * Parameters: void 
     * Returns: HRESULT - should be S_OK
     */
    HRESULT Initialize();
    /**
     * Method: TStreamSink::Pause
     * Purpose: Sets event to get the stream to pause
     * Parameters: void
     * Returns: HRESULT - MF_E_INVALIDREQUEST if state ready or paused, otherise should be S_OK
     */
    HRESULT Pause(void);
    /**
     * Method: TStreamSink::Preroll
     * Purpose: Causes initial requests for samples to be made
     * Parameters: void
     * Returns: HRESULT - S_OK unless shutting down (MF_E_SHUTDOWN)
     */
    HRESULT Preroll(void);
    /**
     * Method: TStreamSink::Restart
     * Purpose: Sets event to get the stream to restart
     * Parameters: void
     * Returns: HRESULT - MF_E_INVALIDREQUEST if state ready or paused, otherise should be S_OK
     */
    HRESULT Restart(void);
    /**
     * Method: TStreamSink::Shutdown
     * Purpose: Shutsdown the Stream sink, rendering most operations invalid
     * Parameters: void
     * Returns: HRESULT - S_OK unless the event queue is not set up (E_POINTER)
     */
    HRESULT Shutdown(void);
    /**
     * Method: TStreamSink::Start
     * Purpose: Sets event to get the stream to start
     * Parameters: MFTIME start - Starting point
     * Returns: HRESULT - MF_E_INVALIDREQUEST if state not set, otherise should be S_OK
     */
    HRESULT Start(MFTIME start);
    /**
     * Method: TStreamSink::Stop
     * Purpose: Sets event to get the stream to stop
     * Parameters: void
     * Returns: HRESULT - MF_E_INVALIDREQUEST if state not set, otherise should be S_OK
     */
    HRESULT Stop(void);

    // Callback

    /**
     * Method: TStreamSink::DispatchEvent
     * Purpose: Call Back function to run in a seperate MF thread
     * Parameters: IMFAsyncResult* result - the interface holding callback data
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or the results of Processing the result
     */
    HRESULT DispatchEvent(IMFAsyncResult* result);

    /**
     * Method: TStreamSink::PresentFrame
     * Purpose: Causes the current frame to be present
     * Parameters: void
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down, or the result of presneting the frame, and asyncronously process samples
     */
    HRESULT PresentFrame();

    //Anagame Methods

    /**
     * Method: TStreamSink::IsActive
     * Purpose: Reports whether it is active (playing or paused)
     * Parameters: void
     * Returns: bool - true if sink is playing or paused, false otherwise
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
     * Method: TStreamSink::ProcessQueueSamples
     * Purpose: Processes Samples by queuing them if markers or Processing them if texture samples
     * Parameters: bool doProcess - whether we are supposed to process the frame (mostly true)
     * Returns: HRESULT - The Result of Queing events or Processing frames
     */
    HRESULT ProcessQueueSamples(bool doProcess);
    /**
     * Method: TStreamSink::DispatchSample
     * Purpose: Causes our samples to be dispatched to the presentation
     * Parameters: TAsyncOp* op - operation to use
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or the results of processing samples and requesting samples
     */
    HRESULT DispatchSample(TAsyncOp* op);
    /**
     * Method: TStreamSink::RequestSample
     * Purpose: Tells the Media Framework that this stream needs more framessamples
     * Parameters: void
     * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or the result of sending the Request Samples event
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

