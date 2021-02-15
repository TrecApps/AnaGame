#include "TStreamSink.h"

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::Flush(void)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetIdentifier(__RPC__out DWORD* pdwIdentifier)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMediaSink(__RPC__deref_out_opt IMFMediaSink** ppMediaSink)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMediaTypeHandler(__RPC__deref_out_opt IMFMediaTypeHandler** ppHandler)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::PlaceMarker(MFSTREAMSINK_MARKER_TYPE eMarkerType, __RPC__in const PROPVARIANT* pvarMarkerValue, __RPC__in const PROPVARIANT* pvarContextValue)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::ProcessSample(__RPC__in_opt IMFSample* pSample)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::EndGetEvent(IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetEvent(DWORD dwFlags, __RPC__deref_out_opt IMFMediaEvent** ppEvent)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::QueueEvent(MediaEventType met, __RPC__in REFGUID guidExtendedType, HRESULT hrStatus, __RPC__in_opt const PROPVARIANT* pvValue)
{
    return E_NOTIMPL;
}
