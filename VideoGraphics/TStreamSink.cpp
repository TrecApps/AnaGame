#include "TStreamSink.h"
#include <Shlwapi.h>
#include <Mferror.h>
#include "TVideoMarker.h"
#include <mfapi.h>
#include "TSampleTexture.h"

/**
 * struct: guid_to_dxgi_vid_formats
 * Purpose: Serves as a link between MF Video Formats and their DXGI Format counterparts 
 */
typedef struct guid_to_dxgi_vid_formats
{
    GUID guidFormat;        // Reference to a Video Format
    DXGI_FORMAT dxgiFormat; // THe DXGI Format
}guid_to_dxgi_vid_formats;

/**
 * Array of format links
 */
guid_to_dxgi_vid_formats const formatTable[] = {
    {MFVideoFormat_NV12, DXGI_FORMAT_NV12},
    {MFVideoFormat_YUY2, DXGI_FORMAT_YUY2},
    {MFVideoFormat_RGB32, DXGI_FORMAT_R32G32B32_FLOAT},
    //{MFVideoFormat_RGB24, DXGI_FORMAT_RGB24},
    //{MFVideoFormat_RGB555, DXGI_FORMAT_RGB555},
    //{MFVideoFormat_RGB565, DXGI_FORMAT_RGB565},
    //{MFVideoFormat_RGB8, DXGI_FORMAT_RGB8},
    {MFVideoFormat_AYUV, DXGI_FORMAT_AYUV},
    {MFVideoFormat_NV11, DXGI_FORMAT_NV11}
};

/**
 * Function: GetBytesPerPixel
 * Purpose: Sets the ideal Bytes per pixel based off of the provided Video Format
 * Parameters: GUID guidSubtype - the Video Format
 *               sFraction& m_imageBytesPP - the bytes/pixel ratio requested
 * Returns: void
 */
void GetBytesPerPixel(GUID guidSubtype, sFraction& m_imageBytesPP)
{
    if ((guidSubtype == MFVideoFormat_NV12) ||
        (guidSubtype == MFVideoFormat_YV12) ||
        (guidSubtype == MFVideoFormat_IYUV) ||
        (guidSubtype == MFVideoFormat_YVU9) ||
        (guidSubtype == MFVideoFormat_I420))
    {
        m_imageBytesPP.Numerator = 3;
        m_imageBytesPP.Denominator = 2;
    }
    else if ((guidSubtype == MFVideoFormat_YUY2) ||
        (guidSubtype == MFVideoFormat_RGB555) ||
        (guidSubtype == MFVideoFormat_RGB565) ||
        (guidSubtype == MFVideoFormat_UYVY) ||
        (guidSubtype == MFVideoFormat_YVYU))
    {
        m_imageBytesPP.Numerator = 2;
        m_imageBytesPP.Denominator = 1;
    }
    else if (guidSubtype == MFVideoFormat_RGB24)
    {
        m_imageBytesPP.Numerator = 3;
        m_imageBytesPP.Denominator = 1;
    }
    else if (guidSubtype == MFVideoFormat_RGB32)
    {
        m_imageBytesPP.Numerator = 4;
        m_imageBytesPP.Denominator = 1;
    }
    else if (guidSubtype == MFVideoFormat_v410)
    {
        m_imageBytesPP.Numerator = 5;
        m_imageBytesPP.Denominator = 4;
    }
    else // includes:
        // MFVideoFormat_RGB8
        // MFVideoFormat_AYUV
        // MFVideoFormat_NV11
    {
        // This is just a fail-safe
        m_imageBytesPP.Numerator = 1;
        m_imageBytesPP.Denominator = 1;
    }
}


/**
 * List of supported video types
 */
GUID const* const s_pVideoFormats[] =
{
    &MFVideoFormat_NV12,
    //&MFVideoFormat_IYUV,
    &MFVideoFormat_YUY2,
    //&MFVideoFormat_YV12,
    &MFVideoFormat_RGB32,
    //&MFVideoFormat_RGB32,
    //&MFVideoFormat_RGB24,
    //&MFVideoFormat_RGB555,
    //&MFVideoFormat_RGB565,
    //&MFVideoFormat_RGB8,
    &MFVideoFormat_AYUV,
    //&MFVideoFormat_UYVY,
    //&MFVideoFormat_YVYU,
    //&MFVideoFormat_YVU9,
    //&MFVideoFormat_v410,
    //&MFVideoFormat_I420,
    &MFVideoFormat_NV11//,
    //&MFVideoFormat_420O
};

/**
 * number of video formats supported
 */
const DWORD s_dwNumVideoFormats = sizeof(s_pVideoFormats) / sizeof(s_pVideoFormats[0]);

/**
 * Function: GetFormatFromVideoFormat
 * Purpose: Retrieves the DXGI_FORMAT that corresponds with the supported video format
 * Parameters: GUID vidForm - the Video Format
 * Returns: DXGI_FORMAT - the DXGI format for the Supported Video Format (or DXGI_FORMAT_UNKNOWN if not supported)
 */
DXGI_FORMAT GetFormatFromVideoFormat(GUID vidForm)
{
    for (UINT Rust = 0; Rust < s_dwNumVideoFormats; Rust++)
    {
        if (formatTable[Rust].guidFormat == vidForm)
            return formatTable[Rust].dxgiFormat;
    }
    return DXGI_FORMAT_UNKNOWN;
}

/**
 * set a default framerate
 */
const MFRatio s_DefaultFrameRate = { 30, 1 };

/**
 * Method: TStreamSink::AddRef
 * Purpose: Increments the COM Reference
 * Parameters: void
 * Returns: ULONG - the new counter
 */
STDMETHODIMP_(ULONG __stdcall) TStreamSink::AddRef(void)
{
    return InterlockedIncrement(&m_nRefCount);
}

/**
 * Method: TStreamSink::QueryInterface
 * Purpose: Retrieves a pointer to an object based off of the specified interface
 * Parameters: REFIID riid - the id of the interface to get
 *              void** ppv - where to place the pointer, if riid is supported
 * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
 *
 * Note: Supported Interfaces are IUnknown, IMFStreamSink, and IMFMediaTypeHandler
 */
HRESULT TStreamSink::QueryInterface(REFIID iid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(static_cast<IMFStreamSink*>(this));
    }
    else if (iid == __uuidof(IMFStreamSink))
    {
        *ppv = static_cast<IMFStreamSink*>(this);
    }
    else if (iid == __uuidof(IMFMediaEventGenerator))
    {
        *ppv = static_cast<IMFMediaEventGenerator*>(this);
    }
    else if (iid == __uuidof(IMFMediaTypeHandler))
    {
        *ppv = static_cast<IMFMediaTypeHandler*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

/**
 * Method: TStreamSink::Release
 * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
 * Parameters: void
 * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
 */
STDMETHODIMP_(ULONG __stdcall) TStreamSink::Release(void)
{
    ULONG c = InterlockedDecrement(&m_nRefCount);
    if (!c)
    {
        Shutdown();
        delete this;
    }
    return c;
}

/**
 * Method: TStreamSink::Flush
 * Purpose: Removes all frames held by the sink
 * Parameters: void
 * Returns: HRESULT - S_OK
 */
HRESULT TStreamSink::Flush(void)
{
    ThreadLock();
    samples.Flush();
    processFrames = false;
    ThreadRelease();
    return S_OK;
}

/**
 * Method: TStreamSink::GetIdentifier
 * Purpose: Retrieves the ID according to the Media Sink
 * Parameters: DWORD* pdwIdentifier - holds the identifier of this sink (1)
 * Returns: HRESULT - S_OK unless parameter is null (E_POINTER) or sink is shutting down (MF_E_SHUTDOWN)
 */
HRESULT TStreamSink::GetIdentifier(__RPC__out DWORD* pdwIdentifier)
{
    // Handle Null pointer parameter
    if (!pdwIdentifier)
        return E_POINTER;
    ThreadLock();
    HRESULT ret = S_OK;
    if (CheckShutdown())
        ret = MF_E_SHUTDOWN; // Check shutdown status
    else
        *pdwIdentifier = 1; // Admit id is 1
    ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::GetMediaSink
 * Purpose: Returns the Media Sink associaed with this stream sink
 * Parameters: IMFMediaSink** ppMediaSink - holder for media sink pointer
 * Returns: HRESULT - S_OK unless parameter is null (E_POINTER) or sink is null (E_NOT_SET)
 */
HRESULT TStreamSink::GetMediaSink(__RPC__deref_out_opt IMFMediaSink** ppMediaSink)
{
    HRESULT ret = S_OK;
    ThreadLock();
    IMFMediaSink* sink = mediaSink.Get();
    // Handle null holder and sink scenarios
    if (!ppMediaSink)
        ret = E_POINTER;
    else if (!sink)
        ret = E_NOT_SET;
    
    if (ret == S_OK)
    {
        // Since we are returning a media sink, update the reference on the sink
        sink->AddRef();
        *ppMediaSink = sink;
    }
    ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::GetMediaTypeHandler
 * Purpose: Returns the Media Type handler Interface (this object)
 * Parameters: IMFMediaTypeHandler** ppHandler - the handler object holder
 * Returns: HRESULT - S_OK unless parameter is null (E_POINTER) or sink is shutting down (MF_E_SHUTDOWN)
 */
HRESULT TStreamSink::GetMediaTypeHandler(__RPC__deref_out_opt IMFMediaTypeHandler** ppHandler)
{
    //Handle Null pointer parameter
    if (!ppHandler)
        return E_POINTER;

    ThreadLock();
    HRESULT ret = S_OK;
    if (CheckShutdown())  // Handle shutdown case
        ret = MF_E_SHUTDOWN;
    else
        // Use Query Interface o get the pointer as this object fullfills that interface
        ret = this->QueryInterface(IID_IMFMediaTypeHandler, (void**)ppHandler);
    ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::PlaceMarker
 * Purpose: Allows markers to be placed in the stream
 * Parameters: MFSTREAMSINK_MARKER_TYPE eMarkerType - type of marker to deal with
 *              const PROPVARIANT* pvarMarkerValue - the value of the marker
 *              const PROPVARIANT* pvarContextValue - information about the context
 * Returns: HRESULT - S_OK unless parameter is null (E_POINTER), or shutdown is in effect (MF_E_SHUTDOWN)
 */
HRESULT TStreamSink::PlaceMarker(MFSTREAMSINK_MARKER_TYPE eMarkerType, __RPC__in const PROPVARIANT* pvarMarkerValue, __RPC__in const PROPVARIANT* pvarContextValue)
{
    // Handle null parameters
    if (!pvarContextValue || !pvarMarkerValue)
        return E_POINTER;
    ThreadLock();
    HRESULT ret = S_OK;

    if (CheckShutdown()) // Handle shutdown scenario
    {
        ret = MF_E_SHUTDOWN;
    }
    else
    {
        TVideoMarker* mark = nullptr;
        // Use TVideoMarker to hold the marker and place it in the queue
        ret = TVideoMarker::Create(eMarkerType, pvarMarkerValue, pvarContextValue, &mark);
        if (SUCCEEDED(ret))
        {
            auto umark = (IUnknown*)mark;
            samples.Push(umark);
        }
    }
    return ret;
}

/**
 * Method: TStreamSink::ProcessSample
 * Purpose: Takes in a sample and attempts to convert it to a texture (not currently working properly, needs inspection)
 * Parameters: IMFSample* pSample - the sample to process and store
 * Returns: HRESULT - S_OK unless parameter is null (E_POINTER), or shutdown is in effect (MF_E_SHUTDOWN), or a DirectX texture generation fails
 */
HRESULT TStreamSink::ProcessSample(__RPC__in_opt IMFSample* pSample)
{
    // Handle null parameter
    if (!pSample)
        return E_POINTER;
    ThreadLock();

    // Double check that the height and width have been set
    if (!textureDesc.Width || !textureDesc.Height)
    {
        ThreadRelease();
        return E_NOT_SET;
    }

    DWORD sampleSize = 0;

    // Determine how many buffers are in the sample
    HRESULT ret = pSample->GetBufferCount(&sampleSize);
    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }
    // Get the biffer from the sample
    IMFMediaBuffer* pBuffer = nullptr;
    if (1 == sampleSize)
    {
        ret = pSample->GetBufferByIndex(0, &pBuffer);
    }
    else
        ret = pSample->ConvertToContiguousBuffer(&pBuffer);

    if (FAILED(ret))
        return ret;

    IMF2DBuffer* p2Buff = nullptr;

    // Attempt to connvert to a 2D buffer (ideal)
    ret = pBuffer->QueryInterface<IMF2DBuffer>(&p2Buff);
    BYTE* picByte = nullptr;
    LONG picLen = 0;
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(resourceData));


    if (FAILED(ret))
    {
        p2Buff = nullptr;
        DWORD maxLen = 0, curLen = 0;
        ret = pBuffer->Lock((BYTE**)(&resourceData.pSysMem), &maxLen, &curLen);
        if (FAILED(ret))
        {
            pBuffer->Release();
            return ret;
        }
        picLen = maxLen / textureDesc.Height;
    }
    else
    {
        ret = p2Buff->Lock2D((BYTE**)(&resourceData.pSysMem), &picLen);
        if (FAILED(ret))
        {
            ThreadRelease();
            pBuffer->Release();
            p2Buff->Release();
            return ret;
        }
        if (picLen < 0)
        {
            // To-Do: Handle Scenario
            int e = 0;
        }
    }
    
    picLen = textureDesc.Width;



    resourceData.SysMemPitch = picLen;



    ID3D11Texture2D* textures = nullptr;
    ret = device->CreateTexture2D(&textureDesc, &resourceData, &textures);

    if (p2Buff)
        p2Buff->Unlock2D();
    else
        pBuffer->Unlock();

    if(FAILED(ret))
    {
        ThreadRelease();
        pBuffer->Release();
        if(p2Buff) p2Buff->Release();
        return ret;
    }

    TSampleTexture* tst = nullptr;
    TSampleTexture::GetTSampleTexture(pSample, textures, &tst);

    auto uSample = (IUnknown*)tst;
    samples.Push(uSample);

    if (isPrerolling)
    {
        isPrerolling = false;
        ret = QueueEvent(MEStreamSinkPrerolled, GUID_NULL, ret, nullptr);
    }
    else if(state != PlayState::State_Paused && state != PlayState::State_Stopped)
    {
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpProcessSample);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }

    ThreadRelease();
    return S_OK;
}

/**
 * Method: TStreamSink::BeginGetEvent
 * Purpose: Begins an event
 * Parameters: IMFAsyncCallback* pCallback - call back mthod to call
 *              IUnknown* punkState - the state involved in the event
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
 */
HRESULT TStreamSink::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState)
{
    if (!pCallback || !punkState)
        return E_POINTER;
    HRESULT ret = S_OK;

    TObject::ThreadLock();
    ret = CheckShutdown() ? MF_E_SHUTDOWN : m_pEventQueue->BeginGetEvent(pCallback, punkState);
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::EndGetEvent
 * Purpose: Completes an event
 * Parameters: IMFAsyncResult* pResult - Result to return
 *              IMFMediaEvent** ppEvent - the event in question
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
 */
HRESULT TStreamSink::EndGetEvent(IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent)
{
    HRESULT ret = S_OK;

    TObject::ThreadLock();
    ret = CheckShutdown() ? MF_E_SHUTDOWN : m_pEventQueue->EndGetEvent(pResult, ppEvent);
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::GetEvent
 * Purpose: Retrieves the event requested
 * Parameters: DWORD dwFlags - code for the target event
 *              IMFMediaEvent** ppEvent - the event being held
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
 */
HRESULT TStreamSink::GetEvent(DWORD dwFlags, __RPC__deref_out_opt IMFMediaEvent** ppEvent)
{
    IMFMediaEventQueue* pQueue = NULL;
    TObject::ThreadLock();
    HRESULT ret = CheckShutdown() ? MF_E_SHUTDOWN : S_OK;
    if (SUCCEEDED(ret))
    {
        pQueue = m_pEventQueue;
        pQueue->AddRef();
    }
    TObject::ThreadRelease();
    if (SUCCEEDED(ret))
    {
        ret = pQueue->GetEvent(dwFlags, ppEvent);
        pQueue->Release();
    }
    return ret;
}

/**
 * Method: TStreamSink::QueueEvent
 * Purpose: Requests an event to be initiated
 * Parameters: MediaEventType met - event type
 *              REFGUID guidExtendedType - the GUID type of the event (usuallu GUID_NULL)
 *              HRESULT hrStatus - status to hold in the event itself
 *              const PROPVARIANT* pvValue
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or whatever is returned by the event queue
 */
HRESULT TStreamSink::QueueEvent(MediaEventType met, __RPC__in REFGUID guidExtendedType, HRESULT hrStatus, __RPC__in_opt const PROPVARIANT* pvValue)
{
    TObject::ThreadLock();
    HRESULT ret = CheckShutdown() ? MF_E_SHUTDOWN : m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::GetCurrentMediaType
 * Purpose: Retrieves the current media type that has been set
 * Parameters: IMFMediaType** ppMediaType - holder to the current type
 * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or type is not currently set (MF_E_NOT_INITIALIZED)
 */
STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetCurrentMediaType(_Outptr_ IMFMediaType** ppMediaType)
{
    if (!ppMediaType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    ThreadLock();
    HRESULT ret = S_OK;
    if (currType)
    {
        *ppMediaType = currType;
        currType->AddRef();
    }
    else
        ret = MF_E_NOT_INITIALIZED;

        ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::GetMajorType
 * Purpose: Retreives the major type of the media format currently set in the sink
 * Parameters: GUID* pguidMajorType - the current major type being supported
 * Returns:  HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or type is not currently set (MF_E_NOT_INITIALIZED)
 */
STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMajorType(__RPC__out GUID* pguidMajorType)
{
    if (!pguidMajorType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    ThreadLock();
    HRESULT ret = S_OK;
    if (currType)
    {
        currType->GetGUID(MF_MT_MAJOR_TYPE, pguidMajorType);
    }
    else
        ret = MF_E_NOT_INITIALIZED;

    ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::GetMediaTypeByIndex
 * Purpose: Returns a Media Type by the index provided
 * Parameters: DWORD dwIndex - the index of the supported type
 *              IMFMediaType** ppType - the type being held (compiled by the method)
 * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or index is out of bounds (MF_E_NO_MORE_TYPES)
 */
STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMediaTypeByIndex(DWORD dwIndex, _Outptr_ IMFMediaType** ppType)
{
    if (!ppType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    if (dwIndex >= s_dwNumVideoFormats)
        return MF_E_NO_MORE_TYPES;

    IMFMediaType* pType = nullptr;

    ThreadLock();

    HRESULT ret = MFCreateMediaType(&pType);
    if (FAILED(ret))
        goto done;

    ret = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    if (FAILED(ret))
        goto done;

    ret = pType->SetGUID(MF_MT_SUBTYPE, *(s_pVideoFormats[dwIndex]));
    if (FAILED(ret))
        goto done;

    pType->AddRef();
    *ppType = pType;

done:
    if (pType) { pType->Release(); pType = nullptr; }
    ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::GetMediaTypeCount
 * Purpose: Reports the number of formas supported by this sink
 * Parameters: DWORD* pdwTypeCount - holds the count
 * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN)
 */
STDMETHODIMP_(HRESULT __stdcall) TStreamSink::GetMediaTypeCount(__RPC__out DWORD* pdwTypeCount)
{
    if (!pdwTypeCount)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    *pdwTypeCount = s_dwNumVideoFormats;
    return S_OK;
}

/**
 * Method: TStreamSink::IsMediaTypeSupported
 * Purpose: Allows MF to test a type and deduce if this stream supports it
 * Parameters: IMFMediaType* pMediaType - the type to check
 *              IMFMediaType** ppMediaType - hint to the caller of the closest supported type that is supported
 * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN), or if type is not supported (MF_E_INVALIDMEDIATYPE)
 */
STDMETHODIMP_(HRESULT __stdcall) TStreamSink::IsMediaTypeSupported(IMFMediaType* pMediaType, _Outptr_opt_result_maybenull_ IMFMediaType** ppMediaType)
{
    if (!pMediaType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    

    GUID subType = GUID_NULL;
    HRESULT ret = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType);
    if (FAILED(ret))
        return ret;
    // Start by assuming type is not valid
    ret = MF_E_INVALIDMEDIATYPE;

    // Scout supported types to see if the type being checked matches one of them
    for (DWORD i = 0; i < s_dwNumVideoFormats; i++)
    {
        if (subType == (*s_pVideoFormats[i]))
        {
            ret = S_OK;
            break;
        }
    }
    // Don't offer an alternative
    if(ppMediaType)
        *ppMediaType = nullptr;

    return ret;
}

/**
 * Method: TStreamSink::SetCurrentMediaType
 * Purpose: Updates the Stream about the type of data that the stream will be getting
 * Parameters: IMFMediaType* pMediaType - information about the data we are expected to get
 * Returns: HRESULT - Should be S_OK unless param is null (E_POINER) or stream is shutting down (MF_E_SHUTDOWN)
 */
STDMETHODIMP_(HRESULT __stdcall) TStreamSink::SetCurrentMediaType(IMFMediaType* pMediaType)
{
    if (!pMediaType)
        return E_POINTER;

    if (CheckShutdown())
        return MF_E_SHUTDOWN;

    ThreadLock();
    HRESULT ret = IsMediaTypeSupported(pMediaType, nullptr);

    MFRatio fps = { 0, 0 };
    GUID formatType = GUID_NULL;
    if (FAILED(ret))
        goto done;

    pMediaType->GetUINT32(MF_MT_INTERLACE_MODE, &m_unInterlaceMode);

    
    pMediaType->GetGUID(MF_MT_SUBTYPE, &formatType);

    textureDesc.Format = GetFormatFromVideoFormat(formatType);
    GetBytesPerPixel(formatType, this->m_imageBytesPP);

    // Set the frame rate on the scheduler.
    if (SUCCEEDED(MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, (UINT32*)&fps.Numerator, (UINT32*)&fps.Denominator)) 
        && (fps.Numerator != 0) && (fps.Denominator != 0))
    {
        if (MFVideoInterlace_FieldInterleavedUpperFirst == m_unInterlaceMode ||
            MFVideoInterlace_FieldInterleavedLowerFirst == m_unInterlaceMode ||
            MFVideoInterlace_FieldSingleUpper == m_unInterlaceMode ||
            MFVideoInterlace_FieldSingleLower == m_unInterlaceMode ||
            MFVideoInterlace_MixedInterlaceOrProgressive == m_unInterlaceMode)
        {
            fps.Numerator *= 2;
        }

        schedule->SetFrameRate(fps);
    }
    else
    {
        // NOTE: The mixer's proposed type might not have a frame rate, in which case
        // we'll use an arbitary default. (Although it's unlikely the video source
        // does not have a frame rate.)
        schedule->SetFrameRate(s_DefaultFrameRate);
    }

    

    if (currType)
        currType->Release();
    if (SUCCEEDED(MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &textureDesc.Width, &textureDesc.Height)))
    {
        currType = pMediaType;
        currType->AddRef();
    }
    if (PlayState::State_Started != this->state && PlayState::State_Paused != state)
    {
        state = PlayState::State_Ready;
    }
    else
    {
        //Flush all current samples in the Queue as this is a format change
        ret = Flush();
    }

    done:
    ThreadRelease();

    return ret;
}

/**
 * Method: TStreamSink::GetStreamSink
 * Purpose: Sets up a stream sink for use with the media sink
 * Parameters: TrecComPointer<TMediaSink> sink - he media sink to attch to
 *              TrecPointer<DrawingBoard> board - the drawing board to work with
 * Returns: TrecComPointer<IMFStreamSink> -  the Stream sink to use
 *
 * Attributes: static
 */
TrecComPointer<IMFStreamSink> TStreamSink::GetStreamSink(TrecComPointer<TMediaSink> sink, TrecPointer<DrawingBoard> board)
{
    if(!sink.Get() || !board.Get() || !board->GetWindowEngine().Get())
        return TrecComPointer<IMFStreamSink>();


    TrecComPointer<IMFStreamSink>::TrecComHolder holder;
    auto engine = board->GetWindowEngine();
    TStreamSink* tsink = new TStreamSink(TPresenter::GetTPresenter(engine, board));

    IMFStreamSink** mem = holder.GetPointerAddress();
    IMFStreamSink* isink = (IMFStreamSink*)tsink;
    *mem = isink;
    tsink->m_nRefCount = 1;
    tsink->isShutdown = false;
    tsink->device = engine->getDeviceD();
    return holder.Extract();
}

/**
 * Method: TStreamSink::Initialize
 * Purpose: Sets up the Queue needed to manage events and run asynchronously
 * Parameters: void
 * Returns: HRESULT - should be S_OK
 */
HRESULT TStreamSink::Initialize()
{
    HRESULT ret = S_OK;
    TObject::ThreadLock();
    ret = MFCreateEventQueue(&this->m_pEventQueue);
    if (SUCCEEDED(ret))
        ret = MFAllocateWorkQueue(&queueId);
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::Pause
 * Purpose: Sets event to get the stream to pause
 * Parameters: void
 * Returns: HRESULT - MF_E_INVALIDREQUEST if state ready or paused, otherise should be S_OK
 */
HRESULT TStreamSink::Pause(void)
{
    TObject::ThreadLock();
    HRESULT ret = (state == PlayState::State_TypeNotSet || state == PlayState::State_Stopped) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        state = PlayState::State_Paused;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpPause);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::Preroll
 * Purpose: Causes initial requests for samples to be made
 * Parameters: void
 * Returns: HRESULT - S_OK unless shutting down (MF_E_SHUTDOWN)
 */
HRESULT TStreamSink::Preroll(void)
{
    TObject::ThreadLock();
    HRESULT ret = MF_E_SHUTDOWN;
    if (!isShutdown)
    {
        isPrerolling = waitForClock = true;
        this->sampleRequests++;
        ret = S_OK;
        ret = QueueEvent(MEStreamSinkRequestSample, GUID_NULL, ret, nullptr);
    }
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::Restart
 * Purpose: Sets event to get the stream to restart
 * Parameters: void
 * Returns: HRESULT - MF_E_INVALIDREQUEST if state ready or paused, otherise should be S_OK
 */
HRESULT TStreamSink::Restart(void)
{
    TObject::ThreadLock();
    HRESULT ret = (state != PlayState::State_Ready && state != PlayState::State_Paused) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        state = PlayState::State_Paused;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpRestart);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::Shutdown
 * Purpose: Shutsdown the Stream sink, rendering most operations invalid
 * Parameters: void
 * Returns: HRESULT - S_OK unless the event queue is not set up (E_POINTER)
 */
HRESULT TStreamSink::Shutdown(void)
{
    if (!m_pEventQueue)
        return E_POINTER;
    ThreadLock();
    m_pEventQueue->Shutdown();
    m_pEventQueue->Release();
    m_pEventQueue = nullptr;
    isShutdown = true;
    ThreadRelease();
    return S_OK;
}

/**
 * Method: TStreamSink::Start
 * Purpose: Sets event to get the stream to stop
 * Parameters: MFTIME start - Starting point
 * Returns: HRESULT - MF_E_INVALIDREQUEST if state not set, otherise should be S_OK
 */
HRESULT TStreamSink::Start(MFTIME start)
{
    TObject::ThreadLock();
    HRESULT ret = (state == PlayState::State_TypeNotSet) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        if (start != PRESENTATION_CURRENT_POSITION)
            startTime = start;
        state = PlayState::State_Started;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpStart);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    waitForClock = false;
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::Stop
 * Purpose: Sets event to get the stream to stop
 * Parameters: void
 * Returns: HRESULT - MF_E_INVALIDREQUEST if state not set, otherise should be S_OK
 */
HRESULT TStreamSink::Stop(void)
{
    TObject::ThreadLock();
    HRESULT ret = (state == PlayState::State_TypeNotSet) ? MF_E_INVALIDREQUEST : S_OK;
    if (SUCCEEDED(ret))
    {
        state = PlayState::State_Stopped;
        TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpStop);
        ret = MFPutWorkItem(this->queueId, &callBack, newOp);
    }
    TObject::ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::DispatchEvent
 * Purpose: Call Back function to run in a seperate MF thread
 * Parameters: IMFAsyncResult* result - the interface holding callback data
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or the results of Processing the result
 */
HRESULT TStreamSink::DispatchEvent(IMFAsyncResult* result)
{
    ThreadLock();

    // Handle shutdown situation
    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }
    IUnknown* pState = nullptr;
    // Get the State
    HRESULT ret = result->GetState(&pState);
    if (SUCCEEDED(ret))
    {
        // Convert to TAsyncOp class
        TAsyncOp* asyncOp = (TAsyncOp*)pState;
        // Get the Operation o perform
        StreamOperation op = asyncOp->m_op;

        switch (op)
        {
            // Start or restart
        case StreamOperation::OpStart:
        case StreamOperation::OpRestart:
            // Let MF know stream sink has started
            ret = QueueEvent(MEStreamSinkStarted, GUID_NULL, ret, nullptr);
            
            if (SUCCEEDED(ret))
            {
                // Begin to request samples
                sampleRequests++;
                ret = QueueEvent(MEStreamSinkRequestSample, GUID_NULL, ret, NULL);
            }

            // Begin processing samples
            if (SUCCEEDED(ret))
            {
                ret = ProcessQueueSamples(processFrames);
            }
            break;
        case StreamOperation::OpStop:
            // Make sure full screen is false if stopping
            presenter->SetFullscreen(FALSE);
            Flush();
            // No requests
            sampleRequests = 0;
            // Queue event sink stopped
            ret = QueueEvent(MEStreamSinkStopped, GUID_NULL, ret, nullptr);
            break;
        case StreamOperation::OpPause:
            // Queue event sink paused
            ret = QueueEvent(MEStreamSinkPaused, GUID_NULL, ret, nullptr);
            break;
        case StreamOperation::OpProcessSample:
        case StreamOperation::OpPlaceMarker:
            // Dispatch the sample
            if (!waitForClock)
            {
                ret = this->DispatchSample(asyncOp);
            }
            break;
        }
    }

    if (pState)
        pState->Release();

    ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::PresentFrame
 * Purpose: Causes the current frame to be present
 * Parameters: void
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down, or the result of presneting the frame, and asyncronously process samples
 */
HRESULT TStreamSink::PresentFrame()
{
    ThreadLock();
    HRESULT ret = S_OK;

    if (processFrames)
    {
        // Handle Shutdown scenario, or tell presenter to present the current frame
        ret = isShutdown ? MF_E_SHUTDOWN : presenter->PresentFrame();
    }
    if (SUCCEEDED(ret))
    {
        // set up sample processing if currently stopped or paused
        if (state == PlayState::State_Stopped || state == PlayState::State_Paused)
        {
            TAsyncOp* newOp = new TAsyncOp(StreamOperation::OpProcessSample);
            ret = MFPutWorkItem(queueId, &callBack, newOp);
            newOp->Release();
        }
    }
    else
    {
        // Queue event for error
        ret = QueueEvent(MEError, GUID_NULL, ret, nullptr);
    }

    ThreadRelease();
    return ret;
}

/**
 * Method: TStreamSink::IsActive
 * Purpose: Reports whether it is active (playing or paused)
 * Parameters: void
 * Returns: bool - true if sink is playing or paused, false otherwise
 */
bool TStreamSink::IsActive()
{
    return ((state == PlayState::State_Started) || (state == PlayState::State_Paused));
}

/**
 * Method: TStreamSink::ProcessQueueSamples
 * Purpose: Processes Samples by queuing them if markers or Processing them if texture samples
 * Parameters: bool doProcess - whether we are supposed to process the frame (mostly true)
 * Returns: HRESULT - The Result of Queing events or Processing frames
 */
HRESULT TStreamSink::ProcessQueueSamples(bool doProcess)
{
    HRESULT ret = S_OK;
    IUnknown* samp = nullptr;

    // Assume that samples still need to be dispatched
    bool repeat = true;

    // Process Samples as long as there are samples and we have no reason not to
    while (samples.Dequeue(samp) && repeat)
    {
        IVideoMarker* mSample = nullptr;
        ISampleTexture* fSample = nullptr;

        // Retrieve either sample or a marker
        if ((ret = samp->QueryInterface<IVideoMarker>(&mSample)) == E_NOINTERFACE)
            ret = samp->QueryInterface<ISampleTexture>(&fSample);

        if (SUCCEEDED(ret))
        {
            if (mSample)
            {
                // We have a marker sample, in Anagame, guaranteed to be a TVideoMarker object
                TVideoMarker* tMSample = (TVideoMarker*)mSample;

                // Set up data
                PROPVARIANT var;
                PropVariantInit(&var);

                ret = tMSample->GetContext(&var);
                if (SUCCEEDED(ret))
                    // Send Data to the Marker 
                    ret = QueueEvent(MEStreamSinkMarker, GUID_NULL, doProcess ? S_OK : E_ABORT, &var);

                PropVariantClear(&var);
                mSample->Release();
            }
            else if(doProcess)
            {
                // Only send to presenter if told to
                UINT laceMode = 0;
                BOOL deviceChanged = false;
                BOOL processAgain = false;
                IMFSample* outSamp = nullptr;

                // Have the presenter process the frame
                ret = presenter->ProcessFrame(this->currType, (TSampleTexture*)fSample, &laceMode, &deviceChanged, &outSamp);

                if (SUCCEEDED(ret))
                {
                    // Handle scenario where device has changed
                    if (deviceChanged)
                        QueueEvent(MEStreamSinkDeviceChanged, GUID_NULL, S_OK, nullptr);
                    // if we need this sample, put it back
                    if (processAgain)
                        samples.PushHead(samp);
                }
                if (SUCCEEDED(ret) && outSamp)
                {
                    // Inform the scheduler
                    ret = schedule->ScheduleSample((TSampleTexture*)fSample, state != PlayState::State_Started);
                    // Dont repeat if we have an output sample
                    repeat = false;
                }
                fSample->Release();
                if (outSamp)
                    outSamp->Release();
            }
        }
    }


    return ret;
}

/**
 * Method: TStreamSink::DispatchSample
 * Purpose: Causes our samples to be dispatched to the presentation
 * Parameters: TAsyncOp* op - operation to use
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or the results of processing samples and requesting samples
 */
HRESULT TStreamSink::DispatchSample(TAsyncOp* op)
{
    if(isShutdown)
        return MF_E_SHUTDOWN;

    HRESULT ret = ProcessQueueSamples(true);

    if (SUCCEEDED(ret) && op->m_op == StreamOperation::OpProcessSample)
    {
        ret = RequestSample();
    }
    return ret;
}

/**
 * Method: TStreamSink::RequestSample
 * Purpose: Tells the Media Framework that this stream needs more framessamples
 * Parameters: void
 * Returns: HRESULT - MF_E_SHUTDOWN if shutting down or the result of sending the Request Samples event
 */
HRESULT TStreamSink::RequestSample()
{
    HRESULT ret = S_OK;

    // While we have fewer samples and sample requests than 5, make a request
    while ((sampleRequests + samples.GetSize()) < 5)
    {
        // handle shutdown scenario
        if (isShutdown)
        {
            ret = MF_E_SHUTDOWN;
            break;
        }

        // increment sample request count
        sampleRequests++;
        ret = QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr);

        // Queue Error event
        if (FAILED(ret))
            ret = QueueEvent(MEError, GUID_NULL, ret, nullptr);
    }

    return ret;
}

/**
 * Method: TStreamSink::TStreamSink
 * Purpose: Constructor
 * Parameters: TrecComPointer<TPresenter> present - the presenter to use
 * Returns: new stream sink object
 */
TStreamSink::TStreamSink(TrecComPointer<TPresenter> present):
    callBack(this, &TStreamSink::DispatchEvent)

{
    presenter = present;
    state = PlayState::State_TypeNotSet;
    isShutdown = waitForClock = isPrerolling = false;
    processFrames = true;
    queueId = 0;
    startTime = 0;
    sampleRequests = 0;
    m_unInterlaceMode = 0;

    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_NV12;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DECODER;
    textureDesc.Height = textureDesc.Width = 0; // Set those when we have samples to process
    textureDesc.MipLevels = 1;
    m_imageBytesPP.Denominator = m_imageBytesPP.Numerator = 1;
}

/**
 * Method: TStreamSink::CheckShutdown
 * Purpose: Reports whether the sink is shutting down
 * Parameters: void
 * Returns: bool - whether we are shutting down
 */
bool TStreamSink::CheckShutdown()
{
    return isShutdown;
}

TStreamSink::TAsyncOp::TAsyncOp(StreamOperation op)
{
    m_op = op;
    refCount = 1;
}

STDMETHODIMP_(ULONG __stdcall) TStreamSink::TAsyncOp::AddRef(void)
{
    return InterlockedIncrement(&refCount);
}

STDMETHODIMP_(HRESULT __stdcall) TStreamSink::TAsyncOp::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
        return E_POINTER;
    if (iid == IID_IUnknown)
        *ppv = static_cast<IUnknown*>(this);
    else{
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG __stdcall) TStreamSink::TAsyncOp::Release(void)
{
    auto ret = InterlockedDecrement(&refCount);
    if (!ret)
        delete this;
    return ret;
}
