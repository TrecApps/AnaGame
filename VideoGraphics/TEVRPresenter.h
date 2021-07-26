#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <evr.h>
#include "TPresentEngine.h"
#include <TrecReference.h>

typedef enum class render_state {
    rs_started = 1,
    rs_stopped,
    rs_paused,
    rs_shutdown
} render_state;


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

    // Helper Methods
    HRESULT Flush();
    HRESULT NegitiateType();
    HRESULT ProcessInputNotify();
    HRESULT BeginStream();
    HRESULT EndStream();
    HRESULT CheckStreamEnd();

    // Format methods
    HRESULT SetMediaType(IMFMediaType* type);

    // Fame-Stepping
    HRESULT PrepFrameStep(DWORD steps);
    HRESULT StopFrameStep();

    // The Render State of this Presenter
    render_state renderState;

    // frame-step
    enum class framestep_state {
        fs_none,
        fs_waiting_start,
        fs_pending,
        fs_scheduled,
        fs_complete
    } frameStep;



    UINT counter;
    bool streamingStopped;

    TrecPointer<TPresentEngine> presenter;

    TrecComPointer<IMFClock> clock;
    TrecComPointer<IMFTransform> transform;
    TrecComPointer<IMediaEventSink> sink;
    TrecComPointer<IMFMediaType> mediaType;
};

