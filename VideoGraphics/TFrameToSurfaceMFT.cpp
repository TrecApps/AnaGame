#include "TFrameToSurfaceMFT.h"
#include <mfreadwrite.h>
HRESULT __stdcall TFrameToSurfaceMFT::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
        return E_POINTER;
    if (iid == IID_IUnknown)
        *ppv = static_cast<IUnknown*>(this);
    else if (iid == IID_IMFTransform)
        *ppv = static_cast<IMFTransform*>(this);
    else{
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}
ULONG __stdcall TFrameToSurfaceMFT::AddRef(void)
{
    return InterlockedIncrement(&counter);
}
ULONG __stdcall TFrameToSurfaceMFT::Release(void)
{
    ULONG ret = InterlockedDecrement(&counter);
    if (!ret)
        delete this;
    return ret;
}
HRESULT __stdcall TFrameToSurfaceMFT::GetStreamLimits(__RPC__out DWORD* pdwInputMinimum, __RPC__out DWORD* pdwInputMaximum, __RPC__out DWORD* pdwOutputMinimum, __RPC__out DWORD* pdwOutputMaximum)
{
    if (!pdwInputMaximum || !pdwInputMinimum || !pdwOutputMaximum || !pdwOutputMinimum)
        return E_POINTER;

    *pdwInputMaximum = *pdwInputMinimum = *pdwOutputMaximum = *pdwOutputMinimum = 1;
    return S_OK;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetStreamCount(__RPC__out DWORD* pcInputStreams, __RPC__out DWORD* pcOutputStreams)
{
    if (!pcInputStreams || !pcOutputStreams )
        return E_POINTER;

    *pcInputStreams = *pcOutputStreams = 1;
    return S_OK;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetStreamIDs(DWORD dwInputIDArraySize, __RPC__out_ecount_full(dwInputIDArraySize) DWORD* pdwInputIDs,
    DWORD dwOutputIDArraySize, __RPC__out_ecount_full(dwOutputIDArraySize) DWORD* pdwOutputIDs)
{
    if (!pdwInputIDs || !pdwOutputIDs)
        return E_POINTER;
    if (!dwInputIDArraySize || !dwOutputIDArraySize)
        return E_INVALIDARG;

    *pdwInputIDs = inputId;
    *pdwOutputIDs = outputId;

    return S_OK;
}

HRESULT TFrameToSurfaceMFT::CreateInstance(TFrameToSurfaceMFT** in)
{
    if (!in) return E_POINTER;

    TFrameToSurfaceMFT* i = new TFrameToSurfaceMFT();

    HRESULT ret = MFCreateAttributes(&i->atts, 5);
    if (FAILED(ret))
    {
        i->Release();
        return ret;
    }

    return ret;
}

TFrameToSurfaceMFT::TFrameToSurfaceMFT()
{
    manager = nullptr;
    atts = nullptr;
    inputId = outputId = 0;
    counter = 1;
}

TFrameToSurfaceMFT::~TFrameToSurfaceMFT()
{
    if (manager) manager->Release();
    manager = nullptr;
    if (atts) atts->Release();
}
