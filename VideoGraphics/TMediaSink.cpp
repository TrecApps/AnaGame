#include "TMediaSink.h"
#include <Shlwapi.h>

TMediaSink::TMediaSink()
{
    this->m_nRefCount = 1;
}

ULONG TMediaSink::AddRef(void)
{
    return InterlockedIncrement(&m_nRefCount);
}

HRESULT TMediaSink::QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    static const QITAB qit[] = {
    QITABENT(TMediaSink, IMFMediaSink), {0}
    };
    return QISearch(this, qit, riid, ppv);
}

ULONG TMediaSink::Release(void)
{
    ULONG c = InterlockedDecrement(&m_nRefCount);
    if (!c)
    {
        delete this;
    }
    return c;
}

HRESULT TMediaSink::AddStreamSink(DWORD dwStreamSinkIdentifier, IMFMediaType* pMediaType, IMFStreamSink** ppStreamSink)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::GetCharacteristics(DWORD* pdwCharacteristics)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::GetPresentationClock(IMFPresentationClock** ppPresentationClock)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::GetStreamSinkById(DWORD dwIdentifier, IMFStreamSink** ppStreamSink)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::GetStreamSinkByIndex(DWORD dwIndex, IMFStreamSink** ppStreamSink)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::GetStreamSinkCount(DWORD* pcStreamSinkCount)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::RemoveStreamSink(DWORD dwStreamSinkIdentifier)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::SetPresentationClock(IMFPresentationClock* pPresentationClock)
{
    return E_NOTIMPL;
}

HRESULT TMediaSink::Shutdown(void)
{
    return E_NOTIMPL;
}
