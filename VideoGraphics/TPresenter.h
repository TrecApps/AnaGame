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
    /**
     * Method: TPresenter::GetAspectRatioMode
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: DWORD* pdwAspectRatioMode - holder to the ratio mode
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetAspectRatioMode(__RPC__out DWORD* pdwAspectRatioMode) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::GetBorderColor
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: COLORREF* pClr -  not used
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetBorderColor(__RPC__out COLORREF* pClr) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::GetCurrentImage
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: BITMAPINFOHEADER* pBih - not used
     *              BYTE** pDib - not used
     *              DWORD* pcbDib - not used
     *              LONGLONG* pTimestamp - not used
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetCurrentImage(__RPC__inout BITMAPINFOHEADER* pBih, __RPC__deref_out_ecount_full_opt(*pcbDib) BYTE** pDib, __RPC__out DWORD* pcbDib, __RPC__inout_opt LONGLONG* pTimestamp) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::GetFullscreen
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: BOOL* pfFullscreen - not used (anagame does not support full screen)
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetFullscreen(__RPC__out BOOL* pfFullscreen) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::GetIdealVideoSize
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: SIZE* pszMin - not used
     *              SIZE* pszMax - not used
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetIdealVideoSize(__RPC__inout_opt SIZE* pszMin, __RPC__inout_opt SIZE* pszMax) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::GetNativeVideoSize
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: SIZE* pszVideo - not used
     *              SIZE* pszARVideo - not used
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetNativeVideoSize(__RPC__inout_opt SIZE* pszVideo, __RPC__inout_opt SIZE* pszARVideo) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::GetRenderingPrefs
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: DWORD* pdwRenderFlags - holder for flags (not used)
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetRenderingPrefs(__RPC__out DWORD* pdwRenderFlags) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::GetVideoPosition
     * Purpose: Reports the video Position
     * Parameters: MFVideoNormalizedRect* pnrcSource, __RPC__out LPRECT prcDest
     * Returns:
     */
    STDMETHODIMP GetVideoPosition(__RPC__out MFVideoNormalizedRect* pnrcSource, __RPC__out LPRECT prcDest);
    /**
     * Method: TPresenter::GetVideoWindow
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters:HWND* phwndVideo - holder to the window handle - not applicable as Anagame uses the DrawingBoard and TWindowEngine to handle window ops
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP GetVideoWindow(__RPC__deref_out_opt HWND* phwndVideo) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::RepaintVideo
     * Purpose: Calls on the DrawingBoard to presnt the frame
     * Parameters: void
     * Returns: HRESULT - S_OK if the Board was set in initialization (E_NOT_SET otherwise)
     */
    STDMETHODIMP RepaintVideo(void);
    /**
     * Method: TPresenter::SetAspectRatioMode
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters:DWORD dwAspectRatioMode
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP SetAspectRatioMode(DWORD dwAspectRatioMode) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::SetBorderColor
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: COLORREF Clr - color of the border
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP SetBorderColor(COLORREF Clr) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::SetFullscreen
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: BOOL fFullscreen - Unseless, Anagame does not support Full-Screen
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP SetFullscreen(BOOL fFullscreen) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::SetRenderingPrefs
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters:DWORD dwRenderingPrefs - not applicable as currently has no rendering modes
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP SetRenderingPrefs(DWORD dwRenderingPrefs) { return E_NOTIMPL; }
    /**
     * Method: TPresenter::SetVideoPosition
     * Purpose: Sets the Video Position with the Drawing Board
     * Parameters: const MFVideoNormalizedRect* pnrcSource - not used
     *              const LPRECT prcDest -  the rectangle to set
     * Returns: HRESULT - S_OK if the Board was set in initialization (E_NOT_SET otherwise), unless prcDest was null (E_POINTER)
     */
    STDMETHODIMP SetVideoPosition(__RPC__in_opt const MFVideoNormalizedRect* pnrcSource, __RPC__in_opt const LPRECT prcDest);

    /**
     * Method: TPresenter::SetVideoWindow
     * Purpose: FIller method for the IMFVideoDisplayControl interface
     * Parameters: HWND hwndVideo - not used as window manipulation is done through the TWindowEngine and the DrawingBoard
     * Returns: HRESULT - E_NOTIMPL, not applicable to Anagame's use
     */
    STDMETHODIMP SetVideoWindow(__RPC__in HWND hwndVideo) { return E_NOTIMPL; }

    // Other

    /**
     * Method: TPresenter::Shutdown
     * Purpose: Relinquishes hold on the Drawing Board
     * Parameters: void
     * Returns: void
     */
    void Shutdown();
    /**
     * Method: TPresenter::ProcessFrame
     * Purpose: Processes the frame and makes it read for presentation
     * Parameters: IMFMediaType* pCurrentType - the type being used
     *              TSampleTexture* pSample - the sample to prepare
     *              UINT32* punInterlaceMode - info about the interlacing process
     *              BOOL* pbDeviceChanged - set to false
     *              IMFSample** ppOutputSample = NULL - not used
     * Returns: HRESULT - should be S_OK
     */
    HRESULT ProcessFrame(IMFMediaType* pCurrentType, IMFSample* pSample, UINT32* punInterlaceMode, BOOL* pbDeviceChanged, IMFSample** ppOutputSample = NULL);
    /**
     * Method: TPresenter::SetCurrentMediaType
     * Purpose: Sets the current media type, getting the frame width and frame height
     * Parameters: IMFMediaType* pMediaType - the type to set
     * Returns: HRESULT - E_POINTER -f null param, MF_E_SHUTDOWN if shutting down, or the result of getting attributes (Should be S_OK)
     */
    HRESULT SetCurrentMediaType(IMFMediaType* pMediaType);
    /**
     * Method: TPresenter::PresentFrame
     * Purpose: Presents the current Frame
     * Parameters: void
     * Returns: HRESULT - Should be S_OK (E_POINTER if board not set)
     */
    HRESULT PresentFrame();

    // IMFGetService

    /**
     * Method: TPresenter::GetService
     * Purpose: Retrieves service for the IMFGetService interface (uses QueryInterface)
     * Parameters: REFGUID guidService - the service referenced (must be MR_VIDEO_RENDER_SERVICE)
     *              REFIID riid - the service requested (interface)
     *              LPVOID* ppvObject - the output
     * Returns: HRESULT - S_OK unless interfce is not supported or MF_E_UNSUPPORTED_SERVICE if guidService is not MR_VIDEO_RENDER_SERVICE
     */
    STDMETHODIMP GetService(__RPC__in REFGUID guidService, __RPC__in REFIID riid, __RPC__deref_out_opt LPVOID* ppvObject);

private:
    /**
     * Method: TPresenter::TPresenter
     * Purpose: Constructor
     * Parameters: TrecPointer<TWindowEngine> engine - Helps with frame manipulation
     *              TrecPointer<DrawingBoard> board - Presents the frames
     * Returns: new TPresenter object
     */
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

