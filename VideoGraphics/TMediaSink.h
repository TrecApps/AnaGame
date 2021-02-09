#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <TrecReference.h>

/**
 * Class: TMediaSink
 * Purpose: Anagame's custom implementation of the IMFMediaSink Interface, allowing Anagame to Create one without using an out of date HWND 
 */
class TMediaSink :
    public TObject,
    public IMFMediaSink
{
public:
    // Static method to create the object.
    static TrecComPointer<TMediaSink> CreateInstance();

    // IUnknown
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv);
    STDMETHODIMP_(ULONG) Release(void);

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

private:

    TMediaSink();

    long m_nRefCount;
};

