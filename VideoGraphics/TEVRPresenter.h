#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <evr.h>
#include "TPresentEngine.h"
#include <TrecReference.h>
#include <TLinkedList.h>
#include "TBaseScheduler.h"
#include "TEVRScheduler.h"

typedef enum class render_state {
    rs_started = 1,
    rs_stopped,
    rs_paused,
    rs_shutdown
} render_state;

typedef enum class framestep_state {
    fs_none,
    fs_waiting_start,
    fs_pending,
    fs_scheduled,
    fs_complete
} framestep_state;


class TEVRPresenter :
    public TObject,
    public IUnknown,
    public IMFClockStateSink,
    public IMFGetService,
    public IMFTopologyServiceLookupClient,
    public IMFVideoDeviceID,
    public IMFVideoPresenter
{
public: 

    TEVRPresenter(TrecPointer<TPresentEngine>);

    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    virtual ULONG STDMETHODCALLTYPE Release(void) override;

    // IMFClockStateSink
    virtual HRESULT STDMETHODCALLTYPE OnClockStart(
        MFTIME hnsSystemTime,
        LONGLONG llClockStartOffset) override;

    virtual HRESULT STDMETHODCALLTYPE OnClockStop(
        MFTIME hnsSystemTime) override;

    virtual HRESULT STDMETHODCALLTYPE OnClockPause(
        MFTIME hnsSystemTime) override;

    virtual HRESULT STDMETHODCALLTYPE OnClockRestart(
        MFTIME hnsSystemTime) override;

    virtual HRESULT STDMETHODCALLTYPE OnClockSetRate(
        MFTIME hnsSystemTime,
        float flRate) override;

    // IMFGetService
    virtual HRESULT STDMETHODCALLTYPE GetService(
        __RPC__in REFGUID guidService,
        __RPC__in REFIID riid,
        __RPC__deref_out_opt LPVOID* ppvObject) override;

    // IMFTopologyServiceLookupClient
    virtual HRESULT STDMETHODCALLTYPE InitServicePointers(
        _In_  IMFTopologyServiceLookup* pLookup) override;

    virtual HRESULT STDMETHODCALLTYPE ReleaseServicePointers(void) override;

    // IMFVideoDeviceID
    virtual HRESULT STDMETHODCALLTYPE GetDeviceID(
        _Out_  IID* pDeviceID) override;

    // IMFVideoPresenter
    virtual HRESULT STDMETHODCALLTYPE ProcessMessage(
        MFVP_MESSAGE_TYPE eMessage,
        ULONG_PTR ulParam) override;

    virtual HRESULT STDMETHODCALLTYPE GetCurrentMediaType(
        _Outptr_  IMFVideoMediaType** ppMediaType) override;

protected:

    class FrameStep {
    public:
        FrameStep();
        FrameStep(const FrameStep& copy);

        framestep_state state;
        TLinkedList<TrecComPointer<IMFSample>> samples;
        DWORD steps;
        DWORD_PTR sampleRef;
    } frameStep;

    // Helper Methods
    HRESULT Flush();
    HRESULT NegitiateType();
    HRESULT ProcessInputNotify();
    HRESULT BeginStream();
    HRESULT EndStream();
    HRESULT CheckStreamEnd();

    // Format methods
    HRESULT SetMediaType(IMFMediaType* type);
    HRESULT IsMediaTypeSupported(IMFMediaType* type);
    HRESULT CalcOutputRect(IMFMediaType* type, RECT& rect);
    HRESULT CreateOptimalMediaType(IMFMediaType* prop, IMFMediaType** opt);

    // Sample Management
    void ProcessOutputLoop();
    HRESULT DeliverSample(TrecComPointer<IMFSample> samp, bool repaint);
    void ReleaseResources();

    // Fame-Stepping
    HRESULT PrepFrameStep(DWORD steps);
    HRESULT StopFrameStep();
    HRESULT StartFrameStep();
    HRESULT CancelFrameStep();
    HRESULT DeliverFrameStep(TrecComPointer<IMFSample> samp);

    // The Render State of this Presenter
    render_state renderState;

    // frame-step




    UINT counter, typeCounter;
    bool streamingStopped, prerolled, sampleNotify, endStream;
    float rate;

    TEVRScheduler scheduler;

    TrecPointer<TPresentEngine> presenter;

    TLinkedList<TrecComPointer<IMFSample>> samples;

    TrecComPointer<IMFClock> clock;
    TrecComPointer<IMFTransform> transform;
    TrecComPointer<IMediaEventSink> sink;
    TrecComPointer<IMFMediaType> mediaType;
};

