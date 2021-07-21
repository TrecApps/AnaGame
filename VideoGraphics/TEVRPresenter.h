#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <evr.h>
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
    UINT counter;
};

