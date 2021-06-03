#pragma once
#include <mfobjects.h>
#include "TMediaSink.h"
#include <mfapi.h>

class TActivate :
    public IMFActivate
{
public:

    static TrecComPointer<TActivate> CreateInstance(TrecPointer<DrawingBoard> board);

    // IUnknown
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppvObject);
    STDMETHODIMP_(ULONG) Release(void);

    // IMFActivate
    STDMETHODIMP ActivateObject(__RPC__in REFIID riid, __RPC__deref_out_opt void** ppvObject);
    STDMETHODIMP DetachObject(void);
    STDMETHODIMP ShutdownObject(void);

    // IMFAttributes (most will simply call a standard object)
    virtual HRESULT STDMETHODCALLTYPE GetItem(__RPC__in REFGUID guidKey, __RPC__inout_opt PROPVARIANT* pValue) override;

    virtual HRESULT STDMETHODCALLTYPE GetItemType(__RPC__in REFGUID guidKey, __RPC__out MF_ATTRIBUTE_TYPE* pType) override;

    virtual HRESULT STDMETHODCALLTYPE CompareItem(__RPC__in REFGUID guidKey, __RPC__in REFPROPVARIANT Value, __RPC__out BOOL* pbResult) override;

    virtual HRESULT STDMETHODCALLTYPE Compare(__RPC__in_opt IMFAttributes* pTheirs, MF_ATTRIBUTES_MATCH_TYPE MatchType, __RPC__out BOOL* pbResult) override;

    virtual HRESULT STDMETHODCALLTYPE GetUINT32(__RPC__in REFGUID guidKey, __RPC__out UINT32* punValue) override;

    virtual HRESULT STDMETHODCALLTYPE GetUINT64(__RPC__in REFGUID guidKey, __RPC__out UINT64* punValue) override;

    virtual HRESULT STDMETHODCALLTYPE GetDouble(__RPC__in REFGUID guidKey, __RPC__out double* pfValue) override;

    virtual HRESULT STDMETHODCALLTYPE GetGUID(__RPC__in REFGUID guidKey, __RPC__out GUID* pguidValue) override;

    virtual HRESULT STDMETHODCALLTYPE GetStringLength(__RPC__in REFGUID guidKey, __RPC__out UINT32* pcchLength) override;

    virtual HRESULT STDMETHODCALLTYPE GetString(__RPC__in REFGUID guidKey, __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue, UINT32 cchBufSize, __RPC__inout_opt UINT32* pcchLength) override;

    virtual HRESULT STDMETHODCALLTYPE GetAllocatedString(__RPC__in REFGUID guidKey, __RPC__deref_out_ecount_full_opt((*pcchLength + 1)) LPWSTR* ppwszValue, __RPC__out UINT32* pcchLength) override;

    virtual HRESULT STDMETHODCALLTYPE GetBlobSize(__RPC__in REFGUID guidKey, __RPC__out UINT32* pcbBlobSize) override;

    virtual HRESULT STDMETHODCALLTYPE GetBlob(__RPC__in REFGUID guidKey,__RPC__out_ecount_full(cbBufSize) UINT8* pBuf, UINT32 cbBufSize, __RPC__inout_opt UINT32* pcbBlobSize) override;

    virtual HRESULT STDMETHODCALLTYPE GetAllocatedBlob(__RPC__in REFGUID guidKey, __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8** ppBuf, __RPC__out UINT32* pcbSize) override;

    virtual HRESULT STDMETHODCALLTYPE GetUnknown(__RPC__in REFGUID guidKey, __RPC__in REFIID riid, __RPC__deref_out_opt LPVOID* ppv) override;

    virtual HRESULT STDMETHODCALLTYPE SetItem(__RPC__in REFGUID guidKey, __RPC__in REFPROPVARIANT Value) override;

    virtual HRESULT STDMETHODCALLTYPE DeleteItem(__RPC__in REFGUID guidKey) override;

    virtual HRESULT STDMETHODCALLTYPE DeleteAllItems(void) override;

    virtual HRESULT STDMETHODCALLTYPE SetUINT32(__RPC__in REFGUID guidKey, UINT32 unValue) override;

    virtual HRESULT STDMETHODCALLTYPE SetUINT64(__RPC__in REFGUID guidKey, UINT64 unValue) override;

    virtual HRESULT STDMETHODCALLTYPE SetDouble(__RPC__in REFGUID guidKey, double fValue) override;

    virtual HRESULT STDMETHODCALLTYPE SetGUID(__RPC__in REFGUID guidKey, __RPC__in REFGUID guidValue) override;

    virtual HRESULT STDMETHODCALLTYPE SetString(__RPC__in REFGUID guidKey, __RPC__in_string LPCWSTR wszValue) override;

    virtual HRESULT STDMETHODCALLTYPE SetBlob(__RPC__in REFGUID guidKey, __RPC__in_ecount_full(cbBufSize) const UINT8* pBuf, UINT32 cbBufSize) override;

    virtual HRESULT STDMETHODCALLTYPE SetUnknown(__RPC__in REFGUID guidKey, __RPC__in_opt IUnknown* pUnknown) override;

    virtual HRESULT STDMETHODCALLTYPE LockStore(void) override;

    virtual HRESULT STDMETHODCALLTYPE UnlockStore(void) override;

    virtual HRESULT STDMETHODCALLTYPE GetCount(__RPC__out UINT32* pcItems) override;

    virtual HRESULT STDMETHODCALLTYPE GetItemByIndex(UINT32 unIndex, __RPC__out GUID* pguidKey, __RPC__inout_opt PROPVARIANT* pValue) override;

    virtual HRESULT STDMETHODCALLTYPE CopyAllItems(__RPC__in_opt IMFAttributes* pDest) override;

    // Anagame Specific Methods
    TrecComPointer<TMediaSink> GetSink();

private:
    TActivate();
    UINT count = 0;
    TrecPointer<DrawingBoard> board;
    TrecComPointer<TMediaSink> sink;
    TrecComPointer<IMFAttributes> atts;
};

