#include "TEVRPresenter.h"

HRESULT __stdcall TEVRPresenter::QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
    return E_NOTIMPL;
}

ULONG __stdcall TEVRPresenter::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

ULONG __stdcall TEVRPresenter::Release(void)
{
    ULONG ret = InterlockedDecrement(&counter);
    if (!ret)
        delete this;
    return ret;
}

HRESULT __stdcall TEVRPresenter::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::OnClockStop(MFTIME hnsSystemTime)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::OnClockPause(MFTIME hnsSystemTime)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::OnClockRestart(MFTIME hnsSystemTime)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::GetService(__RPC__in REFGUID guidService,
    __RPC__in REFIID riid, __RPC__deref_out_opt LPVOID* ppvObject)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::InitServicePointers(_In_ IMFTopologyServiceLookup* pLookup)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::ReleaseServicePointers(void)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::GetDeviceID(_Out_ IID* pDeviceID)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::ProcessMessage(MFVP_MESSAGE_TYPE eMessage, ULONG_PTR ulParam)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::GetCurrentMediaType(_Outptr_ IMFVideoMediaType** ppMediaType)
{
    return E_NOTIMPL;
}
