#include "TEVRPresenter.h"
#include <d3d11.h>
#include <d3d11_2.h>
#include <Mferror.h>
#include <wincodec.h>
#include <d3d9.h>


bool IsActive(render_state s)
{
    return (s == render_state::rs_started || s == render_state::rs_paused);
}


TEVRPresenter::TEVRPresenter(TrecPointer<TPresentEngine> p): scheduler(p)
{
    renderState = render_state::rs_shutdown;
    counter = typeCounter = 0;
    streamingStopped = prerolled = sampleNotify = endStream = false;
    rate = 1.0f;
    presenter = p;
}

HRESULT __stdcall TEVRPresenter::QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppv)
{
    if (!ppv) return E_POINTER;

    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (riid == __uuidof(IMFClockStateSink))
    {
        *ppv = static_cast<IMFClockStateSink*>(this);
    }
    else if (riid == __uuidof(IMFGetService))
    {
        *ppv = static_cast<IMFGetService*>(this);
    }
    else if (riid == __uuidof(IMFTopologyServiceLookupClient))
    {
        *ppv = static_cast<IMFTopologyServiceLookupClient*>(this);
    }
    else if (riid == __uuidof(IMFVideoDeviceID))
    {
        *ppv = static_cast<IMFVideoDeviceID*>(this);
    }
    else if (riid == __uuidof(IMFVideoPresenter))
    {
        *ppv = static_cast<IMFVideoPresenter*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

ULONG __stdcall TEVRPresenter::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

ULONG __stdcall TEVRPresenter::Release(void)
{
    ULONG ret = InterlockedDecrement(&counter);
    if (!ret)
        delete this;
    return ret;
}

HRESULT __stdcall TEVRPresenter::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
{
    ThreadLock();
    if (renderState == render_state::rs_shutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    renderState = render_state::rs_started;

    if (IsActive(renderState))
    {
        if (llClockStartOffset != PRESENTATION_CURRENT_POSITION)
            Flush();
    }
    else
    {
        HRESULT res = StartFrameStep();
        if (FAILED(res))
        {
            ThreadRelease();
            return res;
        }
    }

    ProcessOutputLoop();
    ThreadRelease();
    return S_OK;
}

HRESULT __stdcall TEVRPresenter::OnClockStop(MFTIME hnsSystemTime)
{
    ThreadLock();
    if (renderState == render_state::rs_shutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    if (renderState == render_state::rs_stopped)
    {
        renderState = render_state::rs_stopped;
        Flush();

        if (frameStep.state != framestep_state::fs_none)
            CancelFrameStep();
    }
    ThreadRelease();
    return S_OK;
}

HRESULT __stdcall TEVRPresenter::OnClockPause(MFTIME hnsSystemTime)
{
    ThreadLock();
    if (renderState == render_state::rs_shutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    renderState = render_state::rs_paused;

    ThreadRelease();
    return S_OK;
}

HRESULT __stdcall TEVRPresenter::OnClockRestart(MFTIME hnsSystemTime)
{
    ThreadLock();
    if (renderState == render_state::rs_shutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    renderState = render_state::rs_started;

    HRESULT ret = StartFrameStep();

    if (SUCCEEDED(ret))
    {
        ProcessOutputLoop();
    }
    ThreadRelease();
    return ret;
}

HRESULT __stdcall TEVRPresenter::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
{
    ThreadLock();
    if (renderState == render_state::rs_shutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    if ((rate == 0.0f) && (flRate != 0.0f))
    {
        CancelFrameStep();
        frameStep.samples.Flush();
    }

    rate = flRate;

    scheduler.SetClockRate(rate);

    ThreadRelease();
    return S_OK;
}

HRESULT __stdcall TEVRPresenter::GetService(__RPC__in REFGUID guidService,
    __RPC__in REFIID riid, __RPC__deref_out_opt LPVOID* ppvObject)
{
    if (!ppvObject)
        return E_POINTER;

    if (guidService != MR_VIDEO_RENDER_SERVICE)
        return MF_E_UNSUPPORTED_SERVICE;

    HRESULT ret = presenter->GetService(guidService, riid, ppvObject);
    if (FAILED(ret))
        ret = QueryInterface(riid, ppvObject);

    return ret;
}

HRESULT __stdcall TEVRPresenter::InitServicePointers(_In_ IMFTopologyServiceLookup* pLookup)
{
    if (!pLookup)
        return E_POINTER;

    ThreadLock();

    if (IsActive(renderState))
    {
        ThreadRelease();
        return MF_E_INVALIDREQUEST;
    }

    DWORD objCount = 1;

    clock.Nullify();
    transform.Nullify();
    sink.Nullify();

    TrecComPointer<IMFClock>::TrecComHolder clockHolder;
    HRESULT ret = pLookup->LookupService(
        MF_SERVICE_LOOKUP_GLOBAL, 0, MR_VIDEO_RENDER_SERVICE, __uuidof(IMFClock), (void**)clockHolder.GetPointerAddress(), &objCount);

    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }
    clock = clockHolder.Extract();

    TrecComPointer<IMFTransform>::TrecComHolder transformHolder;
    ret = pLookup->LookupService(
        MF_SERVICE_LOOKUP_GLOBAL, 0, MR_VIDEO_MIXER_SERVICE, __uuidof(IMFTransform), (void**)transformHolder.GetPointerAddress(), &objCount);

    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }
    transform = transformHolder.Extract();

    TrecComPointer<IMediaEventSink>::TrecComHolder sinkHolder;
    ret = pLookup->LookupService(
        MF_SERVICE_LOOKUP_GLOBAL, 0, MR_VIDEO_RENDER_SERVICE, __uuidof(IMediaEventSink), (void**)sinkHolder.GetPointerAddress(), &objCount);

    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }
    sink = sinkHolder.Extract();

    renderState = render_state::rs_stopped;

    ThreadRelease();
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::ReleaseServicePointers(void)
{
    ThreadLock();
    clock.Nullify();
    transform.Nullify();
    sink.Nullify();
    renderState = render_state::rs_shutdown;
    ThreadRelease();

    Flush();
    SetMediaType(nullptr);

    return S_OK;
}

HRESULT __stdcall TEVRPresenter::GetDeviceID(_Out_ IID* pDeviceID)
{
    if (!pDeviceID) return E_POINTER;
    *pDeviceID = __uuidof(IDirect3DDevice9);
    return S_OK;
}

HRESULT __stdcall TEVRPresenter::ProcessMessage(MFVP_MESSAGE_TYPE eMessage, ULONG_PTR ulParam)
{
    HRESULT ret = S_OK;
    ThreadLock();

    if (renderState == render_state::rs_shutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    switch (eMessage)
    {
    // Flush All Samples
    case MFVP_MESSAGE_FLUSH:
        ret = Flush();
        break;

        // Figure out media type
    case MFVP_MESSAGE_INVALIDATEMEDIATYPE:
        ret = NegitiateType();
        break;

        //new Input Sample
    case MFVP_MESSAGE_PROCESSINPUTNOTIFY:
        ret = ProcessInputNotify();
        break;

        // Begin Streaming
    case MFVP_MESSAGE_BEGINSTREAMING:
        ret = BeginStream();
        break;

        // End Streaming
    case MFVP_MESSAGE_ENDSTREAMING:
        ret = EndStream();
        break;

        // No more streaming
    case MFVP_MESSAGE_ENDOFSTREAM:
        streamingStopped = true;
        ret = CheckStreamEnd();
        break;

        // Begin Frame-Stepping
    case MFVP_MESSAGE_STEP:
        ret = PrepFrameStep(LODWORD(ulParam));
        break;

        // Cancel Frame
    case MFVP_MESSAGE_CANCELSTEP:
        ret = StopFrameStep();

        break;
        // Last Resort
    default:
        ret = E_INVALIDARG;
    }

    ThreadRelease();
    return E_NOTIMPL;
}

HRESULT __stdcall TEVRPresenter::GetCurrentMediaType(_Outptr_ IMFVideoMediaType** ppMediaType)
{
    if (!ppMediaType)
        return E_POINTER;
    HRESULT ret = S_OK;
    ThreadLock();

    if (renderState == render_state::rs_shutdown)
        ret = MF_E_SHUTDOWN;
    else
    {
        ret = mediaType.Get() ? mediaType->QueryInterface(__uuidof(IMFVideoMediaType), (void**)ppMediaType) : MF_E_NOT_INITIALIZED;
    }

    ThreadRelease();
    return ret;
}

HRESULT TEVRPresenter::Flush()
{
    prerolled = false;

    scheduler.Flush();
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::NegitiateType()
{
    HRESULT ret = S_OK;
    bool found = false;

    IMFMediaType* mixType = nullptr;
    IMFMediaType* optType = nullptr;

    IMFVideoMediaType* vidType = nullptr;

    if (!transform.Get())
        return MF_E_INVALIDREQUEST;

    DWORD typeInd = 0;

    for(DWORD Rust = 0; !found && (ret != MF_E_NO_MORE_TYPES); Rust++)
    {
        if (mixType) mixType->Release(); mixType = nullptr;
        if (optType) optType->Release(); optType = nullptr;

        ret = transform->GetInputAvailableType(0, Rust, &mixType);
        if (FAILED(ret)) break;

        ret = IsMediaTypeSupported(mixType);
        if (FAILED(ret)) continue;

        ret = CreateOptimalMediaType(mixType, &optType);
        if (FAILED(ret)) continue;

        ret = transform->SetOutputType(0, optType, MFT_SET_TYPE_TEST_ONLY);
        if (FAILED(ret)) continue;

        ret = SetMediaType(optType);
        if (FAILED(ret)) continue;

        ret = transform->SetOutputType(0, optType, MFT_SET_TYPE_TEST_ONLY);
        if (SUCCEEDED(ret))
        {
            found = true;
        }
        else
            SetMediaType(nullptr);
    }
    if (mixType) mixType->Release(); mixType = nullptr;
    if (optType) optType->Release(); optType = nullptr;

    return ret;
}

HRESULT TEVRPresenter::ProcessInputNotify()
{
    HRESULT ret = S_OK;
    sampleNotify = true;
    if (mediaType.Get())
        ProcessOutputLoop();
    else ret = MF_E_TRANSFORM_TYPE_NOT_SET;
   
    return ret;
}

HRESULT TEVRPresenter::BeginStream()
{
    return scheduler.StartScheduler(clock.Get());
}

HRESULT TEVRPresenter::EndStream()
{
    return scheduler.StopScheduler();
}

HRESULT TEVRPresenter::CheckStreamEnd()
{
    if (!endStream && sampleNotify)
        return S_OK;
    if (!samples.GetSize())
        return S_OK;

    if (sink.Get())
        sink->Notify(0x01, (LONG_PTR)S_OK, 0);
    endStream = false;

    return S_OK;
}

HRESULT TEVRPresenter::SetMediaType(IMFMediaType* type)
{
    if (!type)
    {
        mediaType.Delete();
        ReleaseResources();
        return S_OK;
    }

    DWORD flags = 0;
    if (mediaType.Get() && mediaType->IsEqual(type, &flags))
        return S_OK;
    HRESULT ret = S_OK;
    MFRatio fps = { 0,0 };

    mediaType.Delete();

    ReleaseResources();






    return E_NOTIMPL;
}

HRESULT TEVRPresenter::IsMediaTypeSupported(IMFMediaType* type)
{
    return E_NOTIMPL;
}

void MakeArea(MFVideoArea& rect, float left, float top, DWORD width, DWORD height)
{
    rect.Area.cx = width;
    rect.Area.cy = height;
    rect.OffsetX = MakeOffset(left);
    rect.OffsetY = MakeOffset(top);
}

RECT CorrectAspectRatio(const RECT& src, const MFRatio& srcPAR, const MFRatio& destPAR)
{
    // Start with a rectangle the same size as src, but offset to the origin (0,0).
    RECT rc = { 0, 0, src.right - src.left, src.bottom - src.top };

    // If the source and destination have the same PAR, there is nothing to do.
    // Otherwise, adjust the image size, in two steps:
    //  1. Transform from source PAR to 1:1
    //  2. Transform from 1:1 to destination PAR.

    if ((srcPAR.Numerator != destPAR.Numerator) || (srcPAR.Denominator != destPAR.Denominator))
    {
        // Correct for the source's PAR.

        if (srcPAR.Numerator > srcPAR.Denominator)
        {
            // The source has "wide" pixels, so stretch the width.
            rc.right = MulDiv(rc.right, srcPAR.Numerator, srcPAR.Denominator);
        }
        else if (srcPAR.Numerator < srcPAR.Denominator)
        {
            // The source has "tall" pixels, so stretch the height.
            rc.bottom = MulDiv(rc.bottom, srcPAR.Denominator, srcPAR.Numerator);
        }
        // else: PAR is 1:1, which is a no-op.


        // Next, correct for the target's PAR. This is the inverse operation of the previous.

        if (destPAR.Numerator > destPAR.Denominator)
        {
            // The destination has "wide" pixels, so stretch the height.
            rc.bottom = MulDiv(rc.bottom, destPAR.Numerator, destPAR.Denominator);
        }
        else if (destPAR.Numerator < destPAR.Denominator)
        {
            // The destination has "tall" pixels, so stretch the width.
            rc.right = MulDiv(rc.right, destPAR.Denominator, destPAR.Numerator);
        }
        // else: PAR is 1:1, which is a no-op.
    }

    return rc;
}

HRESULT TEVRPresenter::CalcOutputRect(IMFMediaType* type, RECT& rect)
{
    HRESULT ret = S_OK;
    UINT width = 0, height = 0;


    MFRatio inputPar = { 0,0 };
    MFRatio outputPar = { 0,0 };
    RECT oRect = { 0,0,0,0 };

    MFVideoArea display;
    ZeroMemory(&display, sizeof(display));

    LONG offX = 0, offY = 0;

    ret = MFGetAttributeSize(type, MF_MT_FRAME_SIZE, &width, &height);
    if (FAILED(ret)) goto done;

    if (FAILED(type->GetBlob(MF_MT_MINIMUM_DISPLAY_APERTURE, (UINT8*)&display, sizeof(display), nullptr)) &&
        FAILED(type->GetBlob(MF_MT_GEOMETRIC_APERTURE, (UINT8*)&display, sizeof(display), nullptr)))
        MakeArea(display, 0.0, 0.0, width, height);

    offX = (LONG)(display.OffsetX.value + (static_cast<float>(display.OffsetX.fract) / 65536.0f));
    offY = (LONG)(display.OffsetY.value + (static_cast<float>(display.OffsetY.fract) / 65536.0f));

    if (display.Area.cx != 0 &&
        display.Area.cy != 0 &&
        offX + display.Area.cx <= (LONG)(width) &&
        offY + display.Area.cy <= (LONG)(height))
    {
        oRect.left = offX;
        oRect.right = offX + display.Area.cx;
        oRect.top = offY;
        oRect.bottom = offY + display.Area.cy;
    }
    else
    {
        oRect.left = 0;
        oRect.top = 0;
        oRect.right = width;
        oRect.bottom = height;
    }

    if (FAILED(MFGetAttributeRatio(type, MF_MT_PIXEL_ASPECT_RATIO, (UINT*)&inputPar.Numerator, (UINT*)&inputPar.Denominator)))
        inputPar = { 1,1 };

    outputPar = { 1,1 };

    rect = CorrectAspectRatio(oRect, inputPar, outputPar);
done:
    return ret;
}


MFOffset MakeOffset(float value)
{
    MFOffset ret;
    ret.value = short(value);
    ret.fract = WORD(65536 * (value - ret.value));
}


HRESULT TEVRPresenter::CreateOptimalMediaType(IMFMediaType* prop, IMFMediaType** opt)
{
    if (!prop || !opt)
        return E_POINTER;

    RECT rcOut{ 0,0,0,0 };
    MFVideoArea vidArea;
    ZeroMemory(&vidArea, sizeof(vidArea));

    UINT width = 0, height = 0;

    IMFMediaType* newOpt = nullptr;
    HRESULT ret = MFCreateMediaType(&newOpt);
    if (FAILED(ret)) return ret;
    
    ret = prop->CopyAllItems(newOpt);
    if (FAILED(ret)) goto done;

    ret = MFSetAttributeRatio(newOpt, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
    if (FAILED(ret)) goto done;

    rcOut = presenter->GetDestinationRect();

    if (IsRectEmpty(&rcOut))
    {
        ret = CalcOutputRect(prop, rcOut);
        if (FAILED(ret))
            goto done;
    }

    ret = newOpt->SetUINT32(MF_MT_YUV_MATRIX, MFVideoTransferMatrix_BT709);
    if (FAILED(ret)) goto done;

    ret = newOpt->SetUINT32(MF_MT_TRANSFER_FUNCTION, MFVideoTransFunc_709);
    if (FAILED(ret)) goto done;

    ret = newOpt->SetUINT32(MF_MT_VIDEO_PRIMARIES, MFVideoPrimaries_BT709);
    if (FAILED(ret)) goto done;

    ret = newOpt->SetUINT32(MF_MT_VIDEO_NOMINAL_RANGE, MFNominalRange_16_235);
    if (FAILED(ret)) goto done;

    ret = newOpt->SetUINT32(MF_MT_VIDEO_LIGHTING, MFVideoLighting_dim);
    if (FAILED(ret)) goto done;

    width = rcOut.right - rcOut.left, height = rcOut.bottom - rcOut.top;

    ret = MFSetAttributeSize(newOpt, MF_MT_FRAME_SIZE, width, height);
    if (FAILED(ret)) goto done;

    MakeArea(vidArea, rcOut.left, rcOut.top, width, height);

    ret = newOpt->SetUINT32(MF_MT_PAN_SCAN_ENABLED, 0);
    if (FAILED(ret)) goto done;

    ret = newOpt->SetBlob(MF_MT_GEOMETRIC_APERTURE, (UINT8*)&vidArea, sizeof(vidArea));
    if (FAILED(ret)) goto done;

    ret = newOpt->SetBlob(MF_MT_PAN_SCAN_APERTURE, (UINT8*)&vidArea, sizeof(vidArea));
    if (FAILED(ret)) goto done;

    ret = newOpt->SetBlob(MF_MT_MINIMUM_DISPLAY_APERTURE, (UINT8*)&vidArea, sizeof(vidArea));
    if (FAILED(ret)) goto done;


    *opt = newOpt;

done:
    if (FAILED(ret))
        newOpt->Release();

    return ret;
}

void TEVRPresenter::ProcessOutputLoop()
{
}

HRESULT TEVRPresenter::DeliverSample(TrecComPointer<IMFSample> samp, bool repaint)
{
    return E_NOTIMPL;
}

void TEVRPresenter::ReleaseResources()
{
    typeCounter++;
    Flush();
    samples.Flush();
    presenter->ReleaseResources();
}

HRESULT TEVRPresenter::PrepFrameStep(DWORD steps)
{
    frameStep.steps += steps;
    frameStep.state = framestep_state::fs_waiting_start;

    return (renderState == render_state::rs_started) ? StartFrameStep() : S_OK;
}

HRESULT TEVRPresenter::StopFrameStep()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::StartFrameStep()
{
    HRESULT ret = S_OK;
    TrecComPointer<IMFSample> sample;

    if (frameStep.state == framestep_state::fs_pending)
    {
        frameStep.state = framestep_state::fs_pending;
        while (frameStep.samples.GetSize() && frameStep.state == framestep_state::fs_pending && SUCCEEDED(ret))
        {
            if (!frameStep.samples.Dequeue(sample))
            {
                ret = E_FAIL;
                goto done;
            }
            ret = DeliverFrameStep(sample);
        }

    }
    else if (frameStep.state == framestep_state::fs_none)
    {
        while (frameStep.samples.GetSize() && SUCCEEDED(ret))
        {
            if (!frameStep.samples.Dequeue(sample))
            {
                ret = E_FAIL;
                goto done;
            }
            ret = DeliverSample(sample, false);
        }
    }

    done:
    return ret;
}

HRESULT TEVRPresenter::CancelFrameStep()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::DeliverFrameStep(TrecComPointer<IMFSample> samp)
{
    return E_NOTIMPL;
}

TEVRPresenter::FrameStep::FrameStep()
{
    state = framestep_state::fs_none;
    steps = 0;
    sampleRef = NULL;
}

TEVRPresenter::FrameStep::FrameStep(const FrameStep& copy)
{
    this->sampleRef = copy.sampleRef;
    this->samples = copy.samples;
    this->state = copy.state;
    this->steps = copy.steps;
}
