#include "TPresenter.h"
#include <Shlwapi.h>
#include <Mferror.h>
#include <mfapi.h>

TPresenter::TPresenter(TrecPointer<TWindowEngine> engine, TrecPointer<DrawingBoard> board)
{
    this->engine = engine;
    this->board = board;

    boardSlot = board->AddFrameSlot();
    m_nRefCount = 1;
    isShutdown = false;

    frameWidth = frameHeight = 0;

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

STDMETHODIMP_(ULONG __stdcall) TPresenter::AddRef(void)
{
    return InterlockedIncrement(&m_nRefCount);
}

HRESULT TPresenter::QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    static const QITAB qit[] = {
    QITABENT(TPresenter, IMFVideoDisplayControl), {0}
    };
    return QISearch(this, qit, riid, ppv);
}

STDMETHODIMP_(ULONG __stdcall) TPresenter::Release(void)
{
    ULONG c = InterlockedDecrement(&m_nRefCount);
    if (!c)
    {
        delete this;
    }
    return c;
}

STDMETHODIMP_(HRESULT __stdcall) TPresenter::GetVideoPosition(__RPC__out MFVideoNormalizedRect* pnrcSource, __RPC__out LPRECT prcDest)
{
    if (!prcDest)
        return E_POINTER;
    HRESULT ret = E_FAIL;
    ThreadLock();
    if (board.Get())
    {
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
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TPresenter::RepaintVideo(void)
{
    HRESULT ret = E_FAIL;
    ThreadLock();
    if (board.Get())
    {
        board->PresentFrame(boardSlot);
        ret = S_OK;
    }
    ThreadRelease();
    return ret;
}

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

HRESULT TPresenter::ProcessFrame(IMFMediaType* pCurrentType, IMFSample* pSample, UINT32* punInterlaceMode, BOOL* pbDeviceChanged, IMFSample** ppOutputSample)
{
    TObject::ThreadLock();

    if (isShutdown)
    {
        TObject::ThreadRelease();
        return MF_E_SHUTDOWN;
    }

    if (punInterlaceMode == NULL || pCurrentType == NULL || pSample == NULL || pbDeviceChanged == NULL)
    {
        TObject::ThreadRelease();
        return E_POINTER;
    }

    *pbDeviceChanged = FALSE;
    DWORD cBuffers = 0;
    HRESULT res = pSample->GetBufferCount(&cBuffers);

    if (FAILED(res))
    {
        TObject::ThreadRelease();
        return res;
    }
    IMFMediaBuffer* pBuffer = nullptr;
    IMFMediaBuffer* pEVBuffer = nullptr;
    if (1 == cBuffers)
    {
        res = pSample->GetBufferByIndex(0, &pBuffer);
    }
    //else if (2 == cBuffers && m_b3DVideo && 0 != m_vp3DOutput)
    //{
    //    res = pSample->GetBufferByIndex(0, &pBuffer);
    //    if (FAILED(res))
    //    {
    //        ThreadRelease();
    //        return res;
    //    }

    //    res = pSample->GetBufferByIndex(1, &pEVBuffer);
    //}
    else
        res = pSample->ConvertToContiguousBuffer(&pBuffer);

    MFVideoInterlaceMode unInterlaceMode = (MFVideoInterlaceMode)MFGetAttributeUINT32(pCurrentType, MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);

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
    TrecComPointer<ID3D11Device> d3dDevice = engine->getDeviceD();
    TrecComPointer<ID3D11DeviceContext> d3dContext = engine->getDevice();
    if (d3dDevice.Get() || d3dContext.Get())
    {
        ThreadRelease();
        return E_POINTER;
    }
    IMFDXGIBuffer* mfdxgiBuff = nullptr;
    ID3D11Texture2D* d3dText = nullptr;
    UINT resourceIndex = 0;
    res = pBuffer->QueryInterface<IMFDXGIBuffer>(&mfdxgiBuff);
    if (SUCCEEDED(res))
        res = mfdxgiBuff->QueryInterface<ID3D11Texture2D>(&d3dText);
    if (SUCCEEDED(res))
        res = mfdxgiBuff->GetSubresourceIndex(&resourceIndex);
    if (FAILED(res))
    {
        ThreadRelease();
        if (mfdxgiBuff) mfdxgiBuff->Release();
        if (d3dText) d3dText->Release();
        return res;
    }


    if (!videoDevice && FAILED(res = d3dDevice->QueryInterface<>(&videoDevice)))
    {
        ThreadRelease();
        if (mfdxgiBuff) mfdxgiBuff->Release();
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
        if (mfdxgiBuff) mfdxgiBuff->Release();
        if (d3dText) d3dText->Release();
        return res;
    }
    d3dContext->CopyResource(d3dText2, d3dText);
    IDXGISurface* dxgiSurf = nullptr;
    d3dText2->QueryInterface<>(&dxgiSurf);

    DXGI_MAPPED_RECT bitmap2Dmap;
    dxgiSurf->Map(&bitmap2Dmap, DXGI_MAP_READ);



    D2D1_SIZE_U size = D2D1::SizeU(frameWidth, frameHeight);

    // To-Do: Add Frame Setting functionality to the Drawing Board and call it
    if (!board->SetFrame(bitmap2Dmap, size, 0))
        res = E_CHANGED_STATE;
    dxgiSurf->Unmap();
    dxgiSurf->Release();
    if (mfdxgiBuff) mfdxgiBuff->Release();
    if (d3dText) d3dText->Release();
    return res;
}

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
