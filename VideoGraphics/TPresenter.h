#pragma once
#include <evr.h>
#include <TObject.h>
#include "TWindowEngine.h"
#include "DrawingBoard.h"
#include "TSampleTexture.h"

/**
 * Class: TPresenter
 * Purpose: Manages the display of the Frames, Integrating with the DrawingBoard
 */
class TPresenter :
    public IMFVideoDisplayControl,
    public IMFGetService,
    public TObject
{
public:
    static TrecComPointer<TPresenter> GetTPresenter(TrecPointer<TWindowEngine> engine, TrecPointer<DrawingBoard> board);

    // IUnknown
    /**
     * Method: TPresenter::AddRef
     * Purpose: Increments the COM Reference
     * Parameters: void
     * Returns: ULONG - the new counter
     */
    STDMETHODIMP_(ULONG) AddRef(void) override;
    /**
     * Method: TPresenter::QueryInterface
     * Purpose: Retrieves a pointer to an object based off of the specified interface
     * Parameters: REFIID riid - the id of the interface to get
     *              void** ppv - where to place the pointer, if riid is supported
     * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
     *
     * Note: Supported Interfaces are IUnknown, IMFVideoDisplayControl, and IMFGetService
     */
    STDMETHODIMP QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv) override;
    /**
     * Method: TPresenter::Release
     * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
     * Parameters: void
     * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
     */
    STDMETHODIMP_(ULONG) Release(void) override;

    // IMFVideoDisplayControl
    STDMETHODIMP GetAspectRatioMode(__RPC__out DWORD* pdwAspectRatioMode) { return E_NOTIMPL; }
    STDMETHODIMP GetBorderColor(__RPC__out COLORREF* pClr) { return E_NOTIMPL; }
    STDMETHODIMP GetCurrentImage(__RPC__inout BITMAPINFOHEADER* pBih, __RPC__deref_out_ecount_full_opt(*pcbDib) BYTE** pDib, __RPC__out DWORD* pcbDib, __RPC__inout_opt LONGLONG* pTimestamp) { return E_NOTIMPL; }
    STDMETHODIMP GetFullscreen(__RPC__out BOOL* pfFullscreen) { return E_NOTIMPL; }
    STDMETHODIMP GetIdealVideoSize(__RPC__inout_opt SIZE* pszMin, __RPC__inout_opt SIZE* pszMax) { return E_NOTIMPL; }
    STDMETHODIMP GetNativeVideoSize(__RPC__inout_opt SIZE* pszVideo, __RPC__inout_opt SIZE* pszARVideo) { return E_NOTIMPL; }
    STDMETHODIMP GetRenderingPrefs(__RPC__out DWORD* pdwRenderFlags) { return E_NOTIMPL; }
    STDMETHODIMP GetVideoPosition(__RPC__out MFVideoNormalizedRect* pnrcSource, __RPC__out LPRECT prcDest);// { return E_NOTIMPL; }
    STDMETHODIMP GetVideoWindow(__RPC__deref_out_opt HWND* phwndVideo) { return E_NOTIMPL; }
    STDMETHODIMP RepaintVideo(void);// { return E_NOTIMPL; }
    STDMETHODIMP SetAspectRatioMode(DWORD dwAspectRatioMode) { return E_NOTIMPL; }
    STDMETHODIMP SetBorderColor(COLORREF Clr) { return E_NOTIMPL; }
    STDMETHODIMP SetFullscreen(BOOL fFullscreen) { return E_NOTIMPL; }
    STDMETHODIMP SetRenderingPrefs(DWORD dwRenderingPrefs) { return E_NOTIMPL; }
    STDMETHODIMP SetVideoPosition(__RPC__in_opt const MFVideoNormalizedRect* pnrcSource, __RPC__in_opt const LPRECT prcDest);// { return E_NOTIMPL; }
    STDMETHODIMP SetVideoWindow(__RPC__in HWND hwndVideo) { return E_NOTIMPL; }

    // Other
    void Shutdown();
    HRESULT ProcessFrame(IMFMediaType* pCurrentType, TSampleTexture* pSample, UINT32* punInterlaceMode, BOOL* pbDeviceChanged, IMFSample** ppOutputSample = NULL);
    HRESULT SetCurrentMediaType(IMFMediaType* pMediaType);
    HRESULT PresentFrame();

    // IMFGetService
    STDMETHODIMP GetService(__RPC__in REFGUID guidService, __RPC__in REFIID riid, __RPC__deref_out_opt LPVOID* ppvObject);

private:
    TPresenter(TrecPointer<TWindowEngine> engine, TrecPointer<DrawingBoard> board);

    TrecPointer<TWindowEngine> engine;
    TrecPointer<DrawingBoard> board;
    ULONG m_nRefCount;
    bool isShutdown;

    UINT boardSlot;

    //Presenter Details
    ID3D11VideoDevice* videoDevice;
    UINT frameWidth, frameHeight;

    IMFVideoSampleAllocatorEx* m_pSampleAllocatorEx;
};

