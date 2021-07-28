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
    counter = 0;
    streamingStopped = prerolled = false;
    rate = 1.0f;
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
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::ProcessInputNotify()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::BeginStream()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::EndStream()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::CheckStreamEnd()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::SetMediaType(IMFMediaType* type)
{
    return E_NOTIMPL;
}

void TEVRPresenter::ProcessOutputLoop()
{
}

HRESULT TEVRPresenter::PrepFrameStep(DWORD steps)
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::StopFrameStep()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::StartFrameStep()
{
    return E_NOTIMPL;
}

HRESULT TEVRPresenter::CancelFrameStep()
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
