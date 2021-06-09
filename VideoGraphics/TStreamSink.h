#pragma once
#include <mfidl.h>
#include <TObject.h>
#include <TrecReference.h>
#include "TMediaSink.h"
#include <TLinkedList.h>
#include "TPresenter.h"
#include "TComCommon.h"

class TStreamSink : public IMFStreamSink, public IMFMediaTypeHandler, public TObject
{
    friend class TMediaSink;
public:
    // IUnknown
    STDMETHODIMP_(ULONG) AddRef(void) override;
    STDMETHODIMP QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv) override;
    STDMETHODIMP_(ULONG) Release(void) override;

    // IMFStreamSink
    STDMETHODIMP Flush(void) override;
    STDMETHODIMP GetIdentifier(__RPC__out DWORD* pdwIdentifier) override;
    STDMETHODIMP GetMediaSink(__RPC__deref_out_opt IMFMediaSink** ppMediaSink) override;
    STDMETHODIMP GetMediaTypeHandler(__RPC__deref_out_opt IMFMediaTypeHandler** ppHandler) override;
    STDMETHODIMP PlaceMarker(MFSTREAMSINK_MARKER_TYPE eMarkerType, __RPC__in const PROPVARIANT* pvarMarkerValue, __RPC__in const PROPVARIANT* pvarContextValue) override;
    STDMETHODIMP ProcessSample(__RPC__in_opt IMFSample* pSample) override;

    // IMFMediaEventGenerator (from IMFStreamSink)
    STDMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState) override;
    STDMETHODIMP EndGetEvent(IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent) override;
    STDMETHODIMP GetEvent(DWORD dwFlags, __RPC__deref_out_opt IMFMediaEvent** ppEvent) override;
    STDMETHODIMP QueueEvent(MediaEventType met, __RPC__in REFGUID guidExtendedType, HRESULT hrStatus, __RPC__in_opt const PROPVARIANT* pvValue) override;

    // IMFMediaTypeHandler
    STDMETHODIMP GetCurrentMediaType(_Outptr_ IMFMediaType** ppMediaType) override;
    STDMETHODIMP GetMajorType(__RPC__out GUID* pguidMajorType) override;
    STDMETHODIMP GetMediaTypeByIndex(DWORD dwIndex, _Outptr_ IMFMediaType** ppType) override;
    STDMETHODIMP GetMediaTypeCount(__RPC__out DWORD* pdwTypeCount) override;
    STDMETHODIMP IsMediaTypeSupported(IMFMediaType* pMediaType, _Outptr_opt_result_maybenull_ IMFMediaType** ppMediaType) override;
    STDMETHODIMP SetCurrentMediaType(IMFMediaType* pMediaType) override;

    static TrecComPointer<IMFStreamSink> GetStreamSink(TrecComPointer<TMediaSink> sink, TrecPointer<DrawingBoard> board);

    HRESULT Initialize();
    HRESULT Pause(void);
    HRESULT Preroll(void);
    HRESULT Restart(void);
    HRESULT Shutdown(void);
    HRESULT Start(MFTIME start);
    HRESULT Stop(void);

    // Callback
    HRESULT DispatchEvent(IMFAsyncResult* result);

    HRESULT PresentFrame();

    //Anagame Methods
    bool IsActive();

private:

    enum class PlayState
    {
        State_TypeNotSet = 0,   // No media type is set
        State_Ready,            // Media type is set, Start has never been called.
        State_Started,
        State_Paused,
        State_Stopped
    };

    enum class StreamOperation
    {
        OpSetMediaType = 0,
        OpStart,
        OpRestart,
        OpPause,
        OpStop,
        OpProcessSample,
        OpPlaceMarker
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

    HRESULT ProcessQueueSamples(bool doProcess);
    HRESULT DispatchSample(TAsyncOp* op);
    HRESULT RequestSample();
    PlayState state;
    DWORD queueId;
    DWORD sampleRequests;   // Outstanding reuqests for samples.

    UINT32                      m_unInterlaceMode;

    TStreamSink(TrecComPointer<TPresenter> present);

    bool isShutdown;
    bool processFrames;
    bool waitForClock;
    bool isPrerolling;
    bool CheckShutdown();

    TrecComPointer<TMediaSink> mediaSink;
    TrecComPointer<TPresenter> presenter;
    long m_nRefCount;

    TLinkedList<IUnknown*> samples;
    MFTIME startTime;

    D3D11_TEXTURE2D_DESC textureDesc;

    // Stream Specific References
    IMFMediaEventQueue* m_pEventQueue;                  // Event queue
    TComCallback<TStreamSink> callBack;                 // Callback mechanism
    IMFMediaType* currType;                             // Type of Media
    TrecPointer<TScheduler> schedule;
    TrecComPointer<ID3D11Device> device;
};

