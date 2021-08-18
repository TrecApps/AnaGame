#include "TFrameToSurfaceMFT.h"
#include <mfreadwrite.h>
#include <Mferror.h>
#include <d3d11.h>

HRESULT GetDefaulStride(IMFMediaType* type, UINT& stride)
{
    UINT lStride = 0;

    HRESULT ret = type->GetUINT32(MF_MT_DEFAULT_STRIDE, &lStride);

    if (FAILED(ret))
    {
        GUID guid = GUID_NULL;

        UINT w = 0, h = 0;

        ret = type->GetGUID(MF_MT_SUBTYPE, &guid);
        if (SUCCEEDED(ret))
            ret = MFGetAttributeSize(type, MF_MT_FRAME_SIZE, &w, &h);
        if (SUCCEEDED(ret))
            ret = MFGetStrideForBitmapInfoHeader(guid.Data1, w, (LONG*)&lStride);

        // Set for later reference
        if (SUCCEEDED(ret))
            type->SetUINT32(MF_MT_DEFAULT_STRIDE, lStride);
    }

    if (SUCCEEDED(ret))
        stride = lStride;
    return ret;
}

void FillRect(PBYTE scanLine, DWORD width, DWORD height, LONG strideBytes, COLORREF color)
{
    PBYTE curScanLine = scanLine;

    for (DWORD count = 0; count < height; count++)
    {
        DWORD* word = (DWORD*)curScanLine;

        for (DWORD pixel = 0; pixel < width; pixel++)
        {
            word[pixel] = (DWORD)color;
        }
        curScanLine += strideBytes;
    }

}

//void DrawText(PBYTE scanLine, DWORD width, DWORD height, LONG strideBytes, LPCTSTR buff)
//{
//    SIZE size = { 0 ,0 };
//
//    PBYTE 
//}




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
    //if (!pdwInputIDs || !pdwOutputIDs)
    //    return E_POINTER;
    //if (!dwInputIDArraySize || !dwOutputIDArraySize)
    //    return E_INVALIDARG;

    //*pdwInputIDs = inputId;
    //*pdwOutputIDs = outputId;

    //return S_OK;
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetInputStreamInfo(DWORD dwInputStreamID, MFT_INPUT_STREAM_INFO* pStreamInfo)
{
    return E_NOTIMPL;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputStreamInfo(DWORD dwOutputStreamID, MFT_OUTPUT_STREAM_INFO* pStreamInfo)
{
    if (!pStreamInfo)
        return E_POINTER;

    if (dwOutputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;

    TObject::ThreadLock();
    

    pStreamInfo->dwFlags = 
        MFT_OUTPUT_STREAM_PROVIDES_SAMPLES | 
        MFT_OUTPUT_STREAM_WHOLE_SAMPLES |
        MFT_OUTPUT_STREAM_SINGLE_SAMPLE_PER_BUFFER |
        MFT_OUTPUT_STREAM_FIXED_SAMPLE_SIZE;

    if (outputType.Get())
    {
        pStreamInfo->cbSize = imageSize;
        pStreamInfo->cbAlignment = 1;
    }
    else
    {
        pStreamInfo->cbSize = 0;
        pStreamInfo->cbAlignment = 0;
    }

    TObject::ThreadRelease();
    return S_OK;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetAttributes(IMFAttributes** pAttributes)
{
    if (!pAttributes)
        return E_POINTER;
    TObject::ThreadLock();
    HRESULT ret = S_OK;
    if (!atts)
    {
        ret = MFCreateAttributes(&atts, 0);
        if (SUCCEEDED(ret))
            atts->SetUINT32(MF_SA_D3D11_AWARE, TRUE);
        else atts = nullptr;
    }
    *pAttributes = atts;

    TObject::ThreadRelease();
    return ret;
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

    if (dwOutputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;

    if (inputType == GUID_NULL)
        return MF_E_TRANSFORM_TYPE_NOT_SET;

    if (dwTypeIndex)
        return MF_E_NO_MORE_TYPES;

    IMFMediaType* pType = nullptr;
    HRESULT ret = MFCreateMediaType(&pType);
    if (FAILED(ret))
        return ret;
    TObject::ThreadLock();
    ret = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    if(SUCCEEDED(ret))ret = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
    if (SUCCEEDED(ret))ret = pType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, TRUE);
    if (SUCCEEDED(ret))ret = pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
    if (SUCCEEDED(ret))ret = pType->SetUINT32(MF_MT_SAMPLE_SIZE, pixelHeight * pixelWidth * 4);

    if (SUCCEEDED(ret))ret = MFSetAttributeSize(pType, MF_MT_FRAME_SIZE, pixelWidth, pixelHeight);
    if (SUCCEEDED(ret))ret = MFSetAttributeRatio(pType, MF_MT_FRAME_RATE, frameRate.Numerator, frameRate.Denominator);
    if (SUCCEEDED(ret))ret = pType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
    if (SUCCEEDED(ret))ret = MFSetAttributeRatio(pType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);

    if (SUCCEEDED(ret))
    {
        // Go ahead and set the texture description here
        textureDesc.Width = decodeDesc.SampleWidth;
        textureDesc.Height = decodeDesc.SampleHeight;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_NV12;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_DECODER;

        *ppType = pType;
        pType->AddRef();
    }

    if (pType)pType->Release();
    pType = nullptr;
    ThreadRelease();
    return ret; 
}

HRESULT  TFrameToSurfaceMFT::SetInputType(DWORD dwInputStreamID, IMFMediaType* pType, DWORD dwFlags)
{
    if (!pType)
        return E_POINTER;
    if (dwInputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;
    
    ThreadLock();
    if (!vDevice)
    {
        ThreadRelease();
        return E_NOT_SET;
    }
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
    {
        ThreadRelease();
        return MF_E_UNSUPPORTED_D3D_TYPE;
    }
    

    ZeroMemory(&decodeDesc, sizeof(decodeDesc));

    HRESULT ret = vDevice->GetVideoDecoderConfigCount(&decodeDesc, &decoderCount);
    if (FAILED(ret))
    {
        inputType = GUID_NULL;
        ThreadRelease();
        return ret;
    }

    decodeConfigs.RemoveAll();
    D3D11_VIDEO_DECODER_CONFIG decodeConfig;
    ZeroMemory(&decodeConfig, sizeof(decodeConfig));

    for (UINT Rust = 0; Rust < decoderCount && SUCCEEDED(vDevice->GetVideoDecoderConfig(&decodeDesc, Rust, &decodeConfig)); Rust++)
    {
        decodeConfigs.push_back(decodeConfig);
    }


    //outputDesc.DecodeProfile = ;

    ret = decodeConfigs.Size() ? S_OK : MF_E_UNSUPPORTED_D3D_TYPE;
    if (SUCCEEDED(ret))
    {
        TrecComPointer<IMFMediaType>::TrecComHolder holder;
        *holder.GetPointerAddress() = pType;
        this->mediaInputType = holder.Extract();
    }
    ThreadRelease();
    return ret;
}

HRESULT __stdcall TFrameToSurfaceMFT::SetOutputType(DWORD dwOutputStreamID, IMFMediaType* pType, DWORD dwFlags)
{
    if (!pType)
        return E_POINTER;
    if (dwOutputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;

    ThreadLock();
    if (!vDevice)
    {
        ThreadRelease();
        return E_NOT_SET;
    }
    UINT decoderCount = vDevice->GetVideoDecoderProfileCount();
    GUID guid = GUID_NULL;
    guids.RemoveAll();
    for (UINT Rust = 0; Rust < decoderCount && SUCCEEDED(vDevice->GetVideoDecoderProfile(Rust, &guid)); Rust++)
    {
        guids.push_back(guid);
    }
    GUID outputType = GUID_NULL;
    for (UINT Rust = 0; Rust < guids.Size(); Rust++)
    {
        if (SUCCEEDED(pType->GetGUID(guids[Rust], &outputType)))
        {
            BOOL sup = TRUE;
            if (FAILED(vDevice->CheckVideoDecoderFormat(&outputType, DXGI_FORMAT_NV12, &sup))|| !sup)
            {
                outputType = GUID_NULL;
                continue;
            }


            break;
        }
    }
    if (outputType == GUID_NULL)
    {
        ThreadRelease();
        return MF_E_UNSUPPORTED_D3D_TYPE;
    }


    ZeroMemory(&decodeDesc, sizeof(decodeDesc));

    HRESULT ret = vDevice->GetVideoDecoderConfigCount(&decodeDesc, &decoderCount);
    if (FAILED(ret))
    {
        outputType = GUID_NULL;
        ThreadRelease();
        return ret;
    }

    decodeConfigs.RemoveAll();
    D3D11_VIDEO_DECODER_CONFIG decodeConfig;
    ZeroMemory(&decodeConfig, sizeof(decodeConfig));

    for (UINT Rust = 0; Rust < decoderCount && SUCCEEDED(vDevice->GetVideoDecoderConfig(&decodeDesc, Rust, &decodeConfig)); Rust++)
    {
        decodeConfigs.push_back(decodeConfig);
    }


    //outputDesc.DecodeProfile = ;

    ret = decodeConfigs.Size() ? S_OK : MF_E_UNSUPPORTED_D3D_TYPE;
    if (SUCCEEDED(ret))
    {
        TrecComPointer<IMFMediaType>::TrecComHolder holder;
        *holder.GetPointerAddress() = pType;
        this->outputType = holder.Extract();
    }
    ThreadRelease();
    return ret;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetInputCurrentType(DWORD dwInputStreamID, IMFMediaType** ppType)
{
    if (!ppType)
        return E_POINTER;

    if (dwInputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;

    ThreadLock();
    *ppType = mediaInputType.Get();

    HRESULT ret = S_OK;
    if (*ppType)
        (*ppType)->AddRef();
    else
        ret = MF_E_TRANSFORM_TYPE_NOT_SET;

    ThreadRelease();
    return ret;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputCurrentType(DWORD dwOutputStreamID, IMFMediaType** ppType)
{
    if (!ppType)
        return E_POINTER;

    if (dwOutputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;

    ThreadLock();
    *ppType = outputType.Get();

    HRESULT ret = S_OK;
    if (*ppType)
        (*ppType)->AddRef();
    else
        ret = MF_E_TRANSFORM_TYPE_NOT_SET;

    ThreadRelease();
    return ret;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetInputStatus(DWORD dwInputStreamID, DWORD* pdwFlags)
{
    if (dwInputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;

    if (!pdwFlags)
        return E_POINTER;

    ThreadLock();
    *pdwFlags = (mediaBuffer) ? MFT_INPUT_STATUS_ACCEPT_DATA : 0;
    ThreadRelease();
    return S_OK;
}

HRESULT __stdcall TFrameToSurfaceMFT::GetOutputStatus(DWORD* pdwFlags)
{
    if (!pdwFlags)
        return E_POINTER;

    ThreadLock();
    *pdwFlags = (mediaBuffer) ? MFT_OUTPUT_STATUS_SAMPLE_READY : 0;
    ThreadRelease();
    return S_OK;
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

        ThreadLock();
        if (manager)
            manager->Release();
        manager = nullptr;
        if (vDevice) vDevice->Release();
        vDevice = nullptr;
        vService = nullptr;

        HRESULT ret = unknownParam->QueryInterface<IMFDXGIDeviceManager>(&manager);
        ThreadRelease();
        if (FAILED(ret))
            return ret;

        // To-Do: Set up Use of the video Service
        return SetUpDevices();
    }
    HRESULT ret = E_NOTIMPL;
    ThreadLock();
    switch (eMessage)
    {
    case MFT_MESSAGE_COMMAND_FLUSH:
        ret = OnFlush();
        break;
    case MFT_MESSAGE_COMMAND_DRAIN:
        ret = OnDiscontinue();
        break;
    case MFT_MESSAGE_NOTIFY_BEGIN_STREAMING:
        ret = AllocateStreamers();
        break;
    case MFT_MESSAGE_NOTIFY_END_STREAMING:
        ret = FreeStreamers();
        break;
    case MFT_MESSAGE_NOTIFY_END_OF_STREAM:
    case MFT_MESSAGE_NOTIFY_START_OF_STREAM:
        ret = S_OK;
    }
    
    ThreadRelease();

    return ret;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessInput(DWORD dwInputStreamID, IMFSample* pSample, DWORD dwFlags)
{
    if (!pSample)
        return E_POINTER;

    if (!manager)
        return E_NOT_SET;
    if (dwInputStreamID)
        return MF_E_INVALIDSTREAMNUMBER;

    if (dwFlags)
        return E_INVALIDARG;

    HRESULT ret = S_OK;
    LONGLONG timeStamp = 0;

    ThreadLock();

    if (!mediaInputType.Get() || !outputType.Get() || mediaBuffer)
        ret = MF_E_NOTACCEPTING;
    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }

    ret = pSample->ConvertToContiguousBuffer(&mediaBuffer);
    if (SUCCEEDED(ret))
        ret = mediaBuffer->Lock(&mediaData, nullptr, &mediaDataSize);
    if (SUCCEEDED(ret))
    {
        if (FAILED(pSample->GetSampleTime(&timeStamp)))
            timeStamp = _I64_MAX;

        frameTime = timeStamp;
        ret = Process();
    }

    ThreadRelease();

    return ret;
}

HRESULT __stdcall TFrameToSurfaceMFT::ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER* pOutputSamples, DWORD* pdwStatus)
{
    if (!pOutputSamples || !pdwStatus)
        return E_POINTER;

    if (dwFlags || cOutputBufferCount != 1 || !pOutputSamples[0].pSample)
        return E_INVALIDARG;

    HRESULT ret = S_OK;
    ThreadLock();

    if (!mediaBuffer)
        ret = MF_E_TRANSFORM_NEED_MORE_INPUT;

    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }

    DWORD cData = 0;
    IMFMediaBuffer* localBuffer = nullptr;

    ret = pOutputSamples[0].pSample->GetBufferByIndex(0, &localBuffer);
    if (SUCCEEDED(ret))
        ret = localBuffer->GetMaxLength(&cData);

    if (SUCCEEDED(ret))
        ret = cData < imageSize ? E_INVALIDARG : S_OK;

    if (SUCCEEDED(ret))
    {

    }

    if (SUCCEEDED(ret))
    {
        hasPicture = false;

        if (SUCCEEDED(Process()))
            pOutputSamples[0].dwStatus |= MFT_OUTPUT_DATA_BUFFER_INCOMPLETE;
    }
    ThreadRelease();

    return ret;
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
    ZeroMemory(&outputDesc, sizeof(&outputDesc));
    mediaBuffer = nullptr;
    mediaData = nullptr;
    mediaDataSize = 0;
    hasPicture = false;
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

void TFrameToSurfaceMFT::ResetState()
{
}

HRESULT TFrameToSurfaceMFT::SetUpDevices()
{

    if (!manager)
        return E_NOT_SET;
    ThreadLock();
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
    {
        ThreadRelease();
        return ret;
    }
    ret = manager->GetVideoService(devHand, __uuidof(ID3D11VideoDevice), (void**)&vDevice);
    HRESULT ret2 = manager->GetVideoService(devHand, __uuidof(ID3D11Device), (void**)&device);
   

    if (FAILED(ret2))
        ret = ret2;

    if (SUCCEEDED(ret))
    {
        decoderCount = vDevice->GetVideoDecoderProfileCount();
        GUID guid = GUID_NULL;
        for (UINT Rust = 0; Rust < decoderCount && SUCCEEDED(vDevice->GetVideoDecoderProfile(Rust, &guid)); Rust++)
            guids.push_back(guid);
    }
    ThreadRelease();
    return ret;

}

HRESULT TFrameToSurfaceMFT::OnFlush()
{
    if (mediaBuffer)
        mediaBuffer->Release();
    mediaBuffer = nullptr;
    return OnDiscontinue();
}

HRESULT TFrameToSurfaceMFT::OnDiscontinue()
{
    ResetState();
    hasPicture = false;
    return S_OK;
}

HRESULT TFrameToSurfaceMFT::AllocateStreamers()
{
    return OnDiscontinue();
}

HRESULT TFrameToSurfaceMFT::FreeStreamers()
{
    return S_OK;
}

HRESULT TFrameToSurfaceMFT::Process()
{
    return E_NOTIMPL;
}
