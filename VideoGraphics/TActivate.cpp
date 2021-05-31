#include "TActivate.h"

TrecComPointer<TActivate> TActivate::CreateInstance(TrecPointer<DrawingBoard> board)
{
    TrecComPointer<TActivate> ret;
    if (!board.Get())
        return ret;
    TrecComPointer<TActivate>::TrecComHolder retHolder;
    *retHolder.GetPointerAddress() = new TActivate();
    ret = retHolder.Extract();
    ret->board = board;
    return ret;
}

STDMETHODIMP_(ULONG __stdcall) TActivate::AddRef(void)
{
    return InterlockedIncrement(&count);;
}

STDMETHODIMP_(HRESULT __stdcall) TActivate::QueryInterface(REFIID iid, void** ppvObject)
{
    if (!ppvObject)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
        *ppvObject = static_cast<IUnknown*>(static_cast<IMFActivate*>(this));
    }
    else if (iid == __uuidof(IMFActivate))
    {
        *ppvObject = static_cast<IMFActivate*>(this);
    }

    else if (iid == __uuidof(IMFAttributes))
    {
        *ppvObject = static_cast<IMFAttributes*>(this);
    }
    else
    {
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG __stdcall) TActivate::Release(void)
{
    return InterlockedDecrement(&count);
}

STDMETHODIMP_(HRESULT __stdcall) TActivate::ActivateObject(REFIID riid, void** ppvObject)
{
    if (!sink.Get())
    {
        if (!board.Get())
            return E_POINTER;
        sink = TMediaSink::CreateInstance(board);
    }
    return sink->QueryInterface(riid, ppvObject);
}

STDMETHODIMP_(HRESULT __stdcall) TActivate::DetachObject(void)
{
    sink.Nullify();
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) TActivate::ShutdownObject(void)
{
    if (sink.Get())
    {
        sink->Shutdown();
        sink.Nullify();
    }
    return S_OK;
}

HRESULT __stdcall TActivate::GetItem(REFGUID guidKey, PROPVARIANT* pValue)
{
    return atts.Get() ? atts->GetItem(guidKey, pValue) : E_POINTER;
}

HRESULT __stdcall TActivate::GetItemType(REFGUID guidKey, MF_ATTRIBUTE_TYPE* pType)
{
    return atts.Get() ? atts->GetItemType(guidKey, pType) : E_POINTER;
}

HRESULT __stdcall TActivate::CompareItem(REFGUID guidKey, REFPROPVARIANT Value, BOOL* pbResult)
{
    return atts.Get() ? atts->CompareItem(guidKey, Value, pbResult) : E_POINTER;
}

HRESULT __stdcall TActivate::Compare(IMFAttributes* pTheirs, MF_ATTRIBUTES_MATCH_TYPE MatchType, BOOL* pbResult)
{
    return atts.Get() ? atts ->Compare(pTheirs, MatchType, pbResult) : E_POINTER;
}

HRESULT __stdcall TActivate::GetUINT32(REFGUID guidKey, UINT32* punValue)
{
    return atts.Get() ? atts ->GetUINT32(guidKey, punValue) : E_POINTER;
}

HRESULT __stdcall TActivate::GetUINT64(REFGUID guidKey, UINT64* punValue)
{
    return atts.Get() ? atts ->GetUINT64(guidKey, punValue) : E_POINTER;
}

HRESULT __stdcall TActivate::GetDouble(REFGUID guidKey, double* pfValue)
{
    return atts.Get() ? atts ->GetDouble(guidKey, pfValue) : E_POINTER;
}

HRESULT __stdcall TActivate::GetGUID(REFGUID guidKey, GUID* pguidValue)
{
    return atts.Get() ? atts ->GetGUID(guidKey, pguidValue) : E_POINTER;
}

HRESULT __stdcall TActivate::GetStringLength(REFGUID guidKey, UINT32* pcchLength)
{
    return atts.Get() ? atts ->GetStringLength(guidKey, pcchLength) : E_POINTER;
}

HRESULT __stdcall TActivate::GetString(REFGUID guidKey, LPWSTR pwszValue, UINT32 cchBufSize, UINT32* pcchLength)
{
    return atts.Get() ? atts ->GetString(guidKey, pwszValue, cchBufSize, pcchLength) : E_POINTER;
}

HRESULT __stdcall TActivate::GetAllocatedString(REFGUID guidKey, LPWSTR* ppwszValue, UINT32* pcchLength)
{
    return atts.Get() ? atts ->GetAllocatedString(guidKey, ppwszValue, pcchLength) : E_POINTER;
}

HRESULT __stdcall TActivate::GetBlobSize(REFGUID guidKey, UINT32* pcbBlobSize)
{
    return atts.Get() ? atts ->GetBlobSize(guidKey, pcbBlobSize) : E_POINTER;
}

HRESULT __stdcall TActivate::GetBlob(REFGUID guidKey, UINT8* pBuf, UINT32 cbBufSize, UINT32* pcbBlobSize)
{
    return atts.Get() ? atts ->GetBlob(guidKey, pBuf, cbBufSize, pcbBlobSize) : E_POINTER;
}

HRESULT __stdcall TActivate::GetAllocatedBlob(REFGUID guidKey, UINT8** ppBuf, UINT32* pcbSize)
{
    return atts.Get() ? atts ->GetAllocatedBlob(guidKey, ppBuf, pcbSize) : E_POINTER;
}

HRESULT __stdcall TActivate::GetUnknown(REFGUID guidKey, REFIID riid, LPVOID* ppv)
{
    return atts.Get() ? atts ->GetUnknown(guidKey, riid, ppv) : E_POINTER;
}

HRESULT __stdcall TActivate::SetItem(REFGUID guidKey, REFPROPVARIANT Value)
{
    return atts.Get() ? atts ->SetItem(guidKey, Value) : E_POINTER;
}

HRESULT __stdcall TActivate::DeleteItem(REFGUID guidKey)
{
    return atts.Get() ? atts ->DeleteItem(guidKey) : E_POINTER;
}

HRESULT __stdcall TActivate::DeleteAllItems(void)
{
    return atts.Get() ? atts ->DeleteAllItems() : E_POINTER;
}

HRESULT __stdcall TActivate::SetUINT32(REFGUID guidKey, UINT32 unValue)
{
    return atts.Get() ? atts ->SetUINT32(guidKey, unValue) : E_POINTER;
}

HRESULT __stdcall TActivate::SetUINT64(REFGUID guidKey, UINT64 unValue)
{
    return atts.Get() ? atts ->SetUINT64(guidKey, unValue) : E_POINTER;
}

HRESULT __stdcall TActivate::SetDouble(REFGUID guidKey, double fValue)
{
    return atts.Get() ? atts ->SetDouble(guidKey, fValue) : E_POINTER;
}

HRESULT __stdcall TActivate::SetGUID(REFGUID guidKey, REFGUID guidValue)
{
    return atts.Get() ? atts ->SetGUID(guidKey, guidValue) : E_POINTER;
}

HRESULT __stdcall TActivate::SetString(REFGUID guidKey, LPCWSTR wszValue)
{
    return atts.Get() ? atts ->SetString(guidKey, wszValue) : E_POINTER;
}

HRESULT __stdcall TActivate::SetBlob(REFGUID guidKey, const UINT8* pBuf, UINT32 cbBufSize)
{
    return atts.Get() ? atts ->SetBlob(guidKey, pBuf, cbBufSize) : E_POINTER;
}

HRESULT __stdcall TActivate::SetUnknown(REFGUID guidKey, IUnknown* pUnknown)
{
    return atts.Get() ? atts ->SetUnknown(guidKey, pUnknown) : E_POINTER;
}

HRESULT __stdcall TActivate::LockStore(void)
{
    return atts.Get() ? atts ->LockStore() : E_POINTER;
}

HRESULT __stdcall TActivate::UnlockStore(void)
{
    return atts.Get() ? atts ->UnlockStore() : E_POINTER;
}

HRESULT __stdcall TActivate::GetCount(UINT32* pcItems)
{
    return atts.Get() ? atts ->GetCount(pcItems) : E_POINTER;
}

HRESULT __stdcall TActivate::GetItemByIndex(UINT32 unIndex, GUID* pguidKey, PROPVARIANT* pValue)
{
    return atts.Get() ? atts ->GetItemByIndex(unIndex, pguidKey, pValue) : E_POINTER;
}

HRESULT __stdcall TActivate::CopyAllItems(IMFAttributes* pDest)
{
    return atts.Get() ? atts->CopyAllItems(pDest) : E_POINTER;
}

TActivate::TActivate()
{
    TrecComPointer<IMFAttributes>::TrecComHolder attsHolder;
    if (SUCCEEDED(MFCreateAttributes(attsHolder.GetPointerAddress(), 0)))
        atts = attsHolder.Extract();
    count = 1;
}
