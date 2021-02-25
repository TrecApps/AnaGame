#pragma once
#include <mfidl.h>
#include <TObject.h>
#include <TrecReference.h>
#include "TMediaSink.h"
#include <TLinkedList.h>
class TStreamSink : public IMFStreamSink, public TObject
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

private:
    TrecComPointer<TMediaSink> mediaSink;
    long m_nRefCount;

    TLinkedList<TrecComPointer<ID2D1Bitmap1>> samples;
};

