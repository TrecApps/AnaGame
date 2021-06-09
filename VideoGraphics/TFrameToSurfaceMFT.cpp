#include "TFrameToSurfaceMFT.h"
#include <mfreadwrite.h>
#include <Mferror.h>
#include <d3d11.h>
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
    if (!pStreamInfo)
        return E_POINTER;

    pStreamInfo->dwFlags = MFT_OUTPUT_STREAM_PROVIDES_SAMPLES;
    pStreamInfo->cbSize = 0;
    pStreamInfo->cbAlignment = 0;


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
    return E_NOTIMPL; // Fixed Input Streams
}

HRESULT __stdcall TFrameToSurfaceMFT::AddInputStreams(DWORD cStreams, DWORD* adwStreamIDs)
{
    // Fixed Streams
    return E_NOTIMPL;
}

HRESULT TFrameToSurfaceMFT::GetInputAvailableType(DWORD dwInputStreamID, DWORD dwTypeIndex, IMFMediaType** ppType)
{
    return E_NOTIMPL; // We Don't care about the stream type
}

HRESULT TFrameToSurfaceMFT::GetOutputAvailableType(DWORD dwOutputStreamID, DWORD dwTypeIndex, IMFMediaType** ppType)
{
    if (!ppType)
        return E_POINTER;

    if (inputType == GUID_NULL)
        return MF_E_TRANSFORM_TYPE_NOT_SET;

    if (dwTypeIndex)
        return MF_E_NO_MORE_TYPES;

    IMFMediaType* pType = nullptr;
    HRESULT ret = MFCreateMediaType(&pType);
    if (FAILED(ret))
        return ret;

    pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB8);
    MFSetAttributeSize(pType, MF_MT_FRAME_SIZE, decodeDesc.SampleWidth, decodeDesc.SampleHeight);

    // Go ahead and set the texture description here
    textureDesc.Width = decodeDesc.SampleWidth;
    textureDesc.Height = decodeDesc.SampleHeight;
    textureDesc.ArraySize = 5;
    textureDesc.Format = DXGI_FORMAT_NV12;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DECODER;



    return S_OK; // We don't care about the Output
}

HRESULT  TFrameToSurfaceMFT::SetInputType(DWORD dwInputStreamID, IMFMediaType* pType, DWORD dwFlags)
{
    if (!pType)
        return E_POINTER;

    
    if (!vDevice)
        return E_NOT_SET;
    UINT decoderCount = vDevice->GetVideoDecoderProfileCount();
    GUID guid = GUID_NULL;
    guids.RemoveAll();
    for (UINT Rust = 0; Rust < decoderCount && SUCCEEDED(vDevice->GetVideoDecoderProfile(Rust, &guid)); Rust++)
    {
        guids.push_back(guid);
    }
    inputType = GUID_NULL;
    for (UINT Rust = 0; Rust < guids.Size(); Rust++)
    {
        if (SUCCEEDED(pType->GetGUID(guids[Rust], &inputType)))
        {
            BOOL sup = TRUE;
            if (FAILED(vDevice->CheckVideoDecoderFormat(&inputType, DXGI_FORMAT_NV12, &sup) || !sup))
            {
                inputType = GUID_NULL;
                continue;
            }


            break;
        }
    }
    if (inputType == GUID_NULL)
        return MF_E_UNSUPPORTED_D3D_TYPE;

    

    ZeroMemory(&decodeDesc, sizeof(decodeDesc));

    HRESULT ret = vDevice->GetVideoDecoderConfigCount(&decodeDesc, &decoderCount);
    if (FAILED(ret))
    {
        inputType = GUID_NULL;
        return ret;
    }

    decodeConfigs.RemoveAll();
    D3D11_VIDEO_DECODER_CONFIG decodeConfig;
    ZeroMemory(&decodeConfig, sizeof(decodeConfig));

    for (UINT Rust = 0; Rust < decoderCount && SUCCEEDED(vDevice->GetVideoDecoderConfig(&decodeDesc, Rust, &decodeConfig)); Rust++)
    {
        decodeConfigs.push_back(decodeConfig);
    }


    return decodeConfigs.Size() ? S_OK : MF_E_UNSUPPORTED_D3D_TYPE;
}

HRESULT __stdcall TFrameToSurfaceMFT::SetOutputType(DWORD dwOutputStreamID, IMFMediaType* pType, DWORD dwFlags)
{
    return S_OK;
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
    if (MFT_MESSAGE_SET_D3D_MANAGER == eMessage)
    {
        IUnknown* unknownParam =(IUnknown*) ulParam;

        if (!unknownParam)
            return E_POINTER;


        if (manager)
            manager->Release();
        manager = nullptr;
        if (vDevice) vDevice->Release();
        vDevice = nullptr;
        vService = nullptr;

        HRESULT ret = unknownParam->QueryInterface<IMFDXGIDeviceManager>(&manager);
        if (FAILED(ret))
            return ret;

        // To-Do: Set up Use of the video Service
        return SetUpDevices();
    }
    if (eMessage == MFT_MESSAGE_NOTIFY_BEGIN_STREAMING)
        return S_OK;
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessInput(DWORD dwInputStreamID, IMFSample* pSample, DWORD dwFlags)
{
    if (!pSample)
        return E_POINTER;

    if (!manager)
        return E_NOT_SET;

    samples.Push(pSample);


    return S_OK;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER* pOutputSamples, DWORD* pdwStatus)
{
    if (!pOutputSamples)
        return E_POINTER;
    if (!manager || !device)
        return E_NOT_SET; 
    if (cOutputBufferCount > samples.GetSize())
        return MF_E_TRANSFORM_NEED_MORE_INPUT;

    HRESULT ret = S_OK;

    ID3D11VideoDecoder* vDecode = nullptr;

    for (UINT Rust = 0; Rust < cOutputBufferCount; Rust++)
    {
        if (FAILED(manager->TestDevice(devHand)))
        {
            if (vDecode)
                vDecode->Release();
            vDecode = nullptr;
            if(FAILED(ret = SetUpDevices()))
                return ret;
        }
        if (!vDecode)
            ret = vDevice->CreateVideoDecoder(&decodeDesc, &decodeConfigs[0], &vDecode);
        if (FAILED(ret))
            return ret;
        ID3D11Texture2D* textures = nullptr;
        ret = device->CreateTexture2D(&textureDesc, nullptr, &textures);

        if (FAILED(ret))
            return ret;

        for (UINT C = 0; C < textureDesc.ArraySize; C++)
        {
            //ret = vDevice->CreateVideoDecoderOutputView()
        }
    }


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

    atts->SetUINT32(MF_SA_D3D11_AWARE, TRUE);

    return ret;
}

TFrameToSurfaceMFT::TFrameToSurfaceMFT()
{
    manager = nullptr;
    atts = nullptr;
    vDevice = nullptr;
    inputId = outputId = 0;
    decoderCount = 0;
    counter = 1;
    ZeroMemory(&this->decodeDesc, sizeof(decodeDesc));
    ZeroMemory(&textureDesc, sizeof(textureDesc));
}

TFrameToSurfaceMFT::~TFrameToSurfaceMFT()
{    if (atts) atts->Release();
    atts = nullptr;
    if (vDevice)
        vDevice->Release();
    vDevice = nullptr;
    if (device)
        device->Release();
    device = nullptr;
    if (devHand && manager)
        manager->CloseDeviceHandle(devHand);
    devHand = nullptr;
    if (manager) manager->Release();
    manager = nullptr;

}

HRESULT TFrameToSurfaceMFT::SetUpDevices()
{

    if (!manager)
        return E_NOT_SET;
 
    // Release devices that currently exist
    if (vDevice)
        vDevice->Release();
    vDevice = nullptr;
    if (device)
        device->Release();
    device = nullptr;
    if(devHand)
        manager->CloseDeviceHandle(devHand);
    HRESULT ret = manager->OpenDeviceHandle(&devHand);
    if (FAILED(ret))
        return ret;
    ret = manager->GetVideoService(devHand, __uuidof(ID3D11VideoDevice), (void**)&vDevice);
    HRESULT ret2 = manager->GetVideoService(devHand, __uuidof(ID3D11Device), (void**)&device);
   

    if (FAILED(ret2))
        ret = ret2;

    if (FAILED(ret))
        return ret;

    decoderCount = vDevice->GetVideoDecoderProfileCount();
    GUID guid = GUID_NULL;
    for (UINT Rust = 0; Rust < decoderCount && SUCCEEDED(vDevice->GetVideoDecoderProfile(Rust, &guid)); Rust++)
        guids.push_back(guid);



    return S_OK;

}
