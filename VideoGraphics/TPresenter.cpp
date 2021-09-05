#include "TPresenter.h"
#include <Shlwapi.h>
#include <Mferror.h>
#include <mfapi.h>
#include <atlbase.h>

/**
 * Method: TPresenter::TPresenter
 * Purpose: Constructor
 * Parameters: TrecPointer<TWindowEngine> engine - Helps with frame manipulation
 *              TrecPointer<DrawingBoard> board - Presents the frames
 * Returns: new TPresenter object
 */
TPresenter::TPresenter(TrecPointer<TWindowEngine> engine, TrecPointer<DrawingBoard> board)
{
    this->engine = engine;
    this->board = board;

    boardSlot = board->AddFrameSlot();
    m_nRefCount = 1;
    isShutdown = false;

    frameWidth = frameHeight = 0;
    m_pSampleAllocatorEx = nullptr;

    videoDevice = nullptr;
    SUCCEEDED(engine->getDeviceD()->QueryInterface<ID3D11VideoDevice>(&videoDevice));
}

TrecComPointer<TPresenter> TPresenter::GetTPresenter(TrecPointer<TWindowEngine> engine, TrecPointer<DrawingBoard> board)
{
    if(!engine.Get() || !engine->getDeviceD().Get() || !engine->getDevice().Get() || !board.Get())
        return TrecComPointer<TPresenter>();

    TrecComPointer<TPresenter>::TrecComHolder retHolder;
    *(retHolder.GetPointerAddress()) = new TPresenter(engine,board);
    return retHolder.Extract();
}

/**
 * Method: TPresenter::AddRef
 * Purpose: Increments the COM Reference
 * Parameters: void
 * Returns: ULONG - the new counter
 */
STDMETHODIMP_(ULONG __stdcall) TPresenter::AddRef(void)
{
    return InterlockedIncrement(&m_nRefCount);
}

/**
 * Method: TPresenter::QueryInterface
 * Purpose: Retrieves a pointer to an object based off of the specified interface
 * Parameters: REFIID riid - the id of the interface to get
 *              void** ppv - where to place the pointer, if riid is supported
 * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
 *
 * Note: Supported Interfaces are IUnknown, IMFVideoDisplayControl, and IMFGetService
 */
HRESULT TPresenter::QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    static const QITAB qit[] = {
    QITABENT(TPresenter, IMFVideoDisplayControl), {0}
    };
    return QISearch(this, qit, riid, ppv);
}
/**
 * Method: TPresenter::Release
 * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
 * Parameters: void
 * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
 */
STDMETHODIMP_(ULONG __stdcall) TPresenter::Release(void)
{
    ULONG c = InterlockedDecrement(&m_nRefCount);
    if (!c)
    {
        delete this;
    }
    return c;
}

/**
 * Method: TPresenter::GetVideoPosition
 * Purpose: Reports the video Position
 * Parameters: MFVideoNormalizedRect* pnrcSource, __RPC__out LPRECT prcDest
 * Returns:
 */
STDMETHODIMP_(HRESULT __stdcall) TPresenter::GetVideoPosition(__RPC__out MFVideoNormalizedRect* pnrcSource, __RPC__out LPRECT prcDest)
{
    if (!prcDest)
        return E_POINTER;
    HRESULT ret = E_FAIL;
    ThreadLock();
    if (board.Get())
    {
        // Gets the video Location
        D2D1_RECT_F loc;
        if(board->GetVideoPosition(boardSlot, loc))
        {
            prcDest->top = loc.top;
            prcDest->left = loc.left;
            prcDest->right = loc.right;
            prcDest->bottom = loc.bottom;
            ret = S_OK;
        }
        
    }
    ThreadRelease();
    return ret;
}

/**
 * Method: TPresenter::RepaintVideo
 * Purpose: Calls on the DrawingBoard to presnt the frame
 * Parameters: void
 * Returns: HRESULT - S_OK if the Board was set in initialization (E_NOT_SET otherwise)
 */
STDMETHODIMP_(HRESULT __stdcall) TPresenter::RepaintVideo(void)
{
    HRESULT ret = E_NOT_SET;
    ThreadLock();
    if (board.Get())
    {
        board->PresentFrame(boardSlot);
        ret = S_OK;
    }
    ThreadRelease();
    return ret;
}

/**
 * Method: TPresenter::SetVideoPosition
 * Purpose: Sets the Video Position with the Drawing Board
 * Parameters: const MFVideoNormalizedRect* pnrcSource - not used
 *              const LPRECT prcDest -  the rectangle to set
 * Returns: HRESULT - S_OK if the Board was set in initialization (E_NOT_SET otherwise), unless prcDest was null (E_POINTER)
 */
STDMETHODIMP_(HRESULT __stdcall) TPresenter::SetVideoPosition(__RPC__in_opt const MFVideoNormalizedRect* pnrcSource, __RPC__in_opt const LPRECT prcDest)
{
    if (!prcDest)
        return E_POINTER;

    ThreadLock();
    HRESULT ret = E_FAIL;
    if (board.Get())
    {
        D2D1_RECT_F add{ 0.0f,0.0f,0.0f,0.0f };
        add.top = prcDest->top;
        add.left = prcDest->left;
        add.right = prcDest->right;
        add.bottom = prcDest->bottom;
        if (board->SetVideoPosition(boardSlot, add))
            ret = S_OK;
    }
    ThreadRelease();
    return ret;
}

/**
 * Method: TPresenter::Shutdown
 * Purpose: Relinquishes hold on the Drawing Board
 * Parameters: void
 * Returns: void
 */
void TPresenter::Shutdown()
{
    TObject::ThreadLock();
    
    isShutdown = true;
    if (board.Get())
        board->ReleaseFrame(boardSlot);
    board.Nullify();
    engine.Nullify();
    TObject::ThreadRelease();
}

HRESULT TPresenter::ProcessFrame(TrecComPointer<IMFMediaType> pCurrentType, IMFSample* pSample, UINT32* punInterlaceMode, BOOL* pbDeviceChanged, IMFSample** ppOutputSample)
{
    TObject::ThreadLock();

    if (isShutdown)
    {
        TObject::ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    if (punInterlaceMode == NULL || pCurrentType.Get() == NULL || pSample == NULL || pbDeviceChanged == NULL)
    {
        TObject::ThreadRelease();
        return E_POINTER;
    }

    *pbDeviceChanged = FALSE;

    MFVideoInterlaceMode unInterlaceMode = pCurrentType.Get() ? (MFVideoInterlaceMode)MFGetAttributeUINT32(pCurrentType.Get(), MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive) :
        MFVideoInterlaceMode::MFVideoInterlace_Unknown;

    //
        // Check the per-sample attributes
        //
    if (MFVideoInterlace_MixedInterlaceOrProgressive == unInterlaceMode)
    {
        BOOL fInterlaced = MFGetAttributeUINT32(pSample, MFSampleExtension_Interlaced, FALSE);
        if (!fInterlaced) // Progressive sample
            *punInterlaceMode = MFVideoInterlace_Progressive;
       else
        {
            BOOL fBottomFirst = MFGetAttributeUINT32(pSample, MFSampleExtension_BottomFieldFirst, FALSE);
            if (fBottomFirst)
                *punInterlaceMode = MFVideoInterlace_FieldInterleavedLowerFirst;
            else
                *punInterlaceMode = MFVideoInterlace_FieldInterleavedUpperFirst;
        }
    }
    DWORD bufferCount = 0;
    
    HRESULT ret = pSample->GetBufferCount(&bufferCount);

    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }

    TrecComPointer<IMFMediaBuffer>::TrecComHolder medBuf;
    ret = pSample->ConvertToContiguousBuffer(medBuf.GetPointerAddress());
    if (FAILED(ret))
    {
        ThreadRelease();
        return ret;
    }

    TString res(board->SetFrame(boardSlot, pCurrentType, medBuf.Extract()));

    if (res.GetSize())
    {
        assert(!res.GetSize());
    }
    
    // To-Do: Add Support for 

    /*
    TrecComPointer<ID3D11Device> d3dDevice = engine->getDeviceD();
    TrecComPointer<ID3D11DeviceContext> d3dContext = engine->getDevice();
    if (!d3dDevice.Get() || !d3dContext.Get())
    {
        ThreadRelease();
        return E_POINTER;
    }
    
    ID3D11Texture2D* d3dText = nullptr;
    UINT resourceIndex = 0;
    


    HRESULT res = pSample->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&d3dText);
    if (FAILED(res))
    {
        ThreadRelease();
        if (d3dText) d3dText->Release();
        return res;
    }


    if (!videoDevice && FAILED(res = d3dDevice->QueryInterface<>(&videoDevice)))
    {
        ThreadRelease();
        if (d3dText) d3dText->Release();
        return res;
    }

    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    d3dText->GetDesc(&td);
    td.Usage = D3D11_USAGE_STAGING;
    td.BindFlags = 0;
    td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    ID3D11Texture2D* d3dText2 = nullptr;
    res = d3dDevice->CreateTexture2D(&td, nullptr, &d3dText2);
    if (FAILED(res))
    {
        ThreadRelease();
        if (d3dText) d3dText->Release();
        return res;
    }
    d3dContext->CopyResource(d3dText2, d3dText);
    IDXGISurface* dxgiSurf = nullptr;
    d3dText2->QueryInterface<>(&dxgiSurf);

    DXGI_MAPPED_RECT bitmap2Dmap;
    dxgiSurf->Map(&bitmap2Dmap, DXGI_MAP_READ);



    D2D1_SIZE_U size = D2D1::SizeU(td.Width, td.Height);

    // To-Do: Add Frame Setting functionality to the Drawing Board and call it
    if (!board->SetFrame(bitmap2Dmap, size, 0))
        res = E_CHANGED_STATE;
    dxgiSurf->Unmap();
    dxgiSurf->Release();
    if (d3dText) d3dText->Release();
    return res;*/
}
/**
 * Method: TPresenter::SetCurrentMediaType
 * Purpose: Sets the current media type, getting the frame width and frame height
 * Parameters: IMFMediaType* pMediaType - the type to set
 * Returns: HRESULT - E_POINTER -f null param, MF_E_SHUTDOWN if shutting down, or the result of getting attributes (Should be S_OK)
 */
HRESULT TPresenter::SetCurrentMediaType(IMFMediaType* pMediaType)
{
    if (!pMediaType)
        return E_POINTER;

    ThreadLock();
    IMFAttributes* pAttributes = NULL;

    if (isShutdown)
    {
        ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    HRESULT res = pMediaType->QueryInterface<>(&pAttributes);
    if (FAILED(res))
    {
        ThreadRelease();
        return res;
    }

    res = MFGetAttributeSize(pAttributes, MF_MT_FRAME_SIZE, &frameWidth, &frameHeight);

    pAttributes->Release();
    pAttributes = nullptr;
    ThreadRelease();

    return res;
}

/**
 * Method: TPresenter::PresentFrame
 * Purpose: Presents the current Frame
 * Parameters: void
 * Returns: HRESULT - Should be S_OK (E_POINTER if board not set)
 */
HRESULT TPresenter::PresentFrame()
{
    ThreadLock();
    HRESULT ret = S_OK;
    if (board.Get())
        board->PresentFrame(boardSlot);
    else
        ret = E_POINTER;
    ThreadRelease();
    return ret;
}

/**
 * Method: TPresenter::GetService
 * Purpose: Retrieves service for the IMFGetService interface (uses QueryInterface)
 * Parameters: REFGUID guidService - the service referenced (must be MR_VIDEO_RENDER_SERVICE)
 *              REFIID riid - the service requested (interface)
 *              LPVOID* ppvObject - the output
 * Returns: HRESULT - S_OK unless interfce is not supported or MF_E_UNSUPPORTED_SERVICE if guidService is not MR_VIDEO_RENDER_SERVICE
 */
STDMETHODIMP_(HRESULT __stdcall) TPresenter::GetService(REFGUID guidService, REFIID riid, LPVOID* ppvObject)
{
    HRESULT hr = S_OK;

     hr = (guidService == MR_VIDEO_RENDER_SERVICE) ? QueryInterface(riid, ppvObject) : MF_E_UNSUPPORTED_SERVICE;


    return hr;
}
