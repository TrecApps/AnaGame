#pragma once
#include <evr.h>
#include <TObject.h>
#include "TWindowEngine.h"
#include "DrawingBoard.h"
class TPresenter :
    public IMFVideoDisplayControl, public TObject
{
public:
    static TrecComPointer<TPresenter> GetTPresenter(TrecPointer<TWindowEngine> engine, TrecPointer<DrawingBoard> board);

    // IUnknown
    STDMETHODIMP_(ULONG) AddRef(void) override;
    STDMETHODIMP QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv) override;
    STDMETHODIMP_(ULONG) Release(void) override;

    // IMFVideoDisplayControl
    STDMETHODIMP GetAspectRatioMode(__RPC__out DWORD* pdwAspectRatioMode) { return E_NOTIMPL; }
    STDMETHODIMP GetBorderColor(__RPC__out COLORREF* pClr) { return E_NOTIMPL; }
    STDMETHODIMP GetCurrentImage(__RPC__inout BITMAPINFOHEADER* pBih, __RPC__deref_out_ecount_full_opt(*pcbDib) BYTE** pDib, __RPC__out DWORD* pcbDib, __RPC__inout_opt LONGLONG* pTimestamp) { return E_NOTIMPL; }
    STDMETHODIMP GetFullscreen(__RPC__out BOOL* pfFullscreen) { return E_NOTIMPL; }
    STDMETHODIMP GetIdealVideoSize(__RPC__inout_opt SIZE* pszMin, __RPC__inout_opt SIZE* pszMax) { return E_NOTIMPL; }
    STDMETHODIMP GetNativeVideoSize(__RPC__inout_opt SIZE* pszVideo, __RPC__inout_opt SIZE* pszARVideo) { return E_NOTIMPL; }
    STDMETHODIMP GetRenderingPrefs(__RPC__out DWORD* pdwRenderFlags) { return E_NOTIMPL; }
    STDMETHODIMP GetVideoPosition(__RPC__out MFVideoNormalizedRect* pnrcSource, __RPC__out LPRECT prcDest) { return E_NOTIMPL; }
    STDMETHODIMP GetVideoWindow(__RPC__deref_out_opt HWND* phwndVideo) { return E_NOTIMPL; }
    STDMETHODIMP RepaintVideo(void) { return E_NOTIMPL; }
    STDMETHODIMP SetAspectRatioMode(DWORD dwAspectRatioMode) { return E_NOTIMPL; }
    STDMETHODIMP SetBorderColor(COLORREF Clr) { return E_NOTIMPL; }
    STDMETHODIMP SetFullscreen(BOOL fFullscreen) { return E_NOTIMPL; }
    STDMETHODIMP SetRenderingPrefs(DWORD dwRenderingPrefs) { return E_NOTIMPL; }
    STDMETHODIMP SetVideoPosition(__RPC__in_opt const MFVideoNormalizedRect* pnrcSource, __RPC__in_opt const LPRECT prcDest) { return E_NOTIMPL; }
    STDMETHODIMP SetVideoWindow(__RPC__in HWND hwndVideo) { return E_NOTIMPL; }

    // Other
    void Shutdown();
    HRESULT ProcessFrame(IMFMediaType* pCurrentType, IMFSample* pSample, UINT32* punInterlaceMode, BOOL* pbDeviceChanged, IMFSample** ppOutputSample = NULL);
    HRESULT SetCurrentMediaType(IMFMediaType* pMediaType);

private:
    TPresenter(TrecPointer<TWindowEngine> engine, TrecPointer<DrawingBoard> board);

    TrecPointer<TWindowEngine> engine;
    TrecPointer<DrawingBoard> board;
    ULONG m_nRefCount;
    bool isShutdown;

    //Presenter Details
    ID3D11VideoDevice* videoDevice;
    UINT frameWidth, frameHeight;
};

