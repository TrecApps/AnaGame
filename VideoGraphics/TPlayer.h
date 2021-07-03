#pragma once

#include <Windows.h>
#include <TrecReference.h>
#include <TString.h>
#include <shobjidl.h> 
#include <shlwapi.h>
#include <assert.h>
#include <strsafe.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <evr.h>

#include "VideoGraphics.h"
#include "TWindowEngine.h"
#include "TActivate.h"

const UINT WM_APP_PLAYER_EVENT = WM_APP + 1;


enum class PlayerState
{
	ps_closed = 0,
	ps_ready,
	ps_opening,
	ps_started,
	ps_paused,
	ps_stopped,
	ps_closing
};

class _VIDEO_GRAPHICS TPlayer : public IMFAsyncCallback
{
public:

	static TrecComPointer<TPlayer> CreateInstance(HRESULT& res, TrecPointer<DrawingBoard> board);

    // IUnknown methods
    /**
     * Method: TPlayer::QueryInterface
     * Purpose: Retrieves a pointer to an object based off of the specified interface
     * Parameters: REFIID riid - the id of the interface to get
     *              void** ppv - where to place the pointer, if riid is supported
     * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
     *
     * Note: Supported Interfaces are IUnknown, and IMFAsyncCallback
     */
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    /**
     * Method: TPlayer::AddRef
     * Purpose: Increments the COM Reference
     * Parameters: void
     * Returns: ULONG - the new counter
     */
    STDMETHODIMP_(ULONG) AddRef();
    /**
     * Method: TPlayer::Release
     * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
     * Parameters: void
     * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
     */
    STDMETHODIMP_(ULONG) Release();

    // IMFAsyncCallback methods
    STDMETHODIMP  GetParameters(DWORD*, DWORD*)
    {
        // Implementation of this method is optional.
        return E_NOTIMPL;
    }
    STDMETHODIMP  Invoke(IMFAsyncResult* pAsyncResult);

    // Playback
    HRESULT       OpenURL(const TString& url, TrecPointer<TWindowEngine> en);
    HRESULT       Play();
    HRESULT       Pause();
    HRESULT       Stop();
    HRESULT       Shutdown();
    HRESULT       HandleEvent(UINT_PTR pUnkPtr);
    PlayerState   GetState() const { return m_state; }

    // Video functionality
    HRESULT       Repaint();
    HRESULT       ResizeVideo(RECT& loc);

    BOOL          HasVideo() { return (m_pVideoDisplay.Get() != nullptr); }

protected:

    // Constructor is private. Use static CreateInstance method to instantiate.
    TPlayer(TrecPointer<DrawingBoard> board);

    // Destructor is private. Caller should call Release.
    virtual ~TPlayer();

    HRESULT Initialize();
    HRESULT CreateSession(TrecPointer<TWindowEngine> en);
    HRESULT CloseSession();
    HRESULT StartPlayback();

    // Media event handlers
    virtual HRESULT OnTopologyStatus(IMFMediaEvent* pEvent);
    virtual HRESULT OnPresentationEnded(IMFMediaEvent* pEvent);
    virtual HRESULT OnNewPresentation(IMFMediaEvent* pEvent);

    // Override to handle additional session events.
    virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType)
    {
        return S_OK;
    }

protected:
    long                    m_nRefCount;        // Reference count.

    IMFMediaSession* m_pSession;
    IMFMediaSource* m_pSource;
    TrecComPointer<TPresenter> m_pVideoDisplay;
    TrecComPointer<TMediaSink> m_sink;
    // TrecComPointer<TActivate> activate;
    TrecPointer<DrawingBoard> board;

    PlayerState             m_state;            // Current state of the media session.
    HANDLE                  m_hCloseEvent;      // Event to wait on while closing.
};

