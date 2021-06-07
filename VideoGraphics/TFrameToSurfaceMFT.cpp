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

HRESULT __stdcall TFrameToSurfaceMFT::GetInputStreamInfo(DWORD dwInputStreamID, MFT_INPUT_STREAM_INFO* pStreamInfo)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputStreamInfo(DWORD dwOutputStreamID, MFT_OUTPUT_STREAM_INFO* pStreamInfo)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetAttributes(IMFAttributes** pAttributes)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetInputStreamAttributes(DWORD dwInputStreamID, IMFAttributes** pAttributes)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputStreamAttributes(DWORD dwOutputStreamID, IMFAttributes** pAttributes)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::DeleteInputStream(DWORD dwStreamID)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::AddInputStreams(DWORD cStreams, DWORD* adwStreamIDs)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetInputAvailableType(DWORD dwInputStreamID, DWORD dwTypeIndex, IMFMediaType** ppType)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputAvailableType(DWORD dwOutputStreamID, DWORD dwTypeIndex, IMFMediaType** ppType)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::SetInputType(DWORD dwInputStreamID, IMFMediaType* pType, DWORD dwFlags)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::SetOutputType(DWORD dwOutputStreamID, IMFMediaType* pType, DWORD dwFlags)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetInputCurrentType(DWORD dwInputStreamID, IMFMediaType** ppType)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputCurrentType(DWORD dwOutputStreamID, IMFMediaType** ppType)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetInputStatus(DWORD dwInputStreamID, DWORD* pdwFlags)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputStatus(DWORD* pdwFlags)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::SetOutputBounds(LONGLONG hnsLowerBound, LONGLONG hnsUpperBound)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessEvent(DWORD dwInputStreamID, IMFMediaEvent* pEvent)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessMessage(MFT_MESSAGE_TYPE eMessage, ULONG_PTR ulParam)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessInput(DWORD dwInputStreamID, IMFSample* pSample, DWORD dwFlags)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER* pOutputSamples, DWORD* pdwStatus)
{
    return E_NOTIMPL;
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
