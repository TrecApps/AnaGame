#include "TMediaSink.h"
#include <Shlwapi.h>
#include <Mferror.h>

TMediaSink::TMediaSink()
{
    this->m_nRefCount = 1;
    this->isShutdown = false;
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

HRESULT TMediaSink::AddStreamSink(DWORD dwStreamSinkIdentifier, __RPC__in_opt IMFMediaType* pMediaType, __RPC__deref_out_opt IMFStreamSink** ppStreamSink)
{
    return MF_E_STREAMSINKS_FIXED;
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

bool TMediaSink::AddStreamSink(TrecComPointer<IMFStreamSink> sink)
{
    if(!sink.Get())
        return false;

    for (UINT Rust = 0; Rust < streamSinks.Size(); Rust++)
    {
        if (sink.Get() == streamSinks[Rust].Get())
            return false;
    }
    streamSinks.push_back(sink);
    return true;
}
