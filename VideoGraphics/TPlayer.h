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

	static TrecComPointer<TPlayer> CreateInstance(HRESULT& res, HWND window, HWND ev);

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFAsyncCallback methods
    STDMETHODIMP  GetParameters(DWORD*, DWORD*)
    {
        // Implementation of this method is optional.
        return E_NOTIMPL;
    }
    STDMETHODIMP  Invoke(IMFAsyncResult* pAsyncResult);

    // Playback
    HRESULT       OpenURL(const TString& url);
    HRESULT       Play();
    HRESULT       Pause();
    HRESULT       Stop();
    HRESULT       Shutdown();
    HRESULT       HandleEvent(UINT_PTR pUnkPtr);
    PlayerState   GetState() const { return m_state; }

    // Video functionality
    HRESULT       Repaint();
    HRESULT       ResizeVideo(RECT& loc);

    BOOL          HasVideo() const { return (m_pVideoDisplay != NULL); }

protected:

    // Constructor is private. Use static CreateInstance method to instantiate.
    TPlayer(HWND hVideo, HWND hEvent);

    // Destructor is private. Caller should call Release.
    virtual ~TPlayer();

    HRESULT Initialize();
    HRESULT CreateSession();
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
    IMFVideoDisplayControl* m_pVideoDisplay;

    HWND                    m_hwndVideo;        // Video window.
    HWND                    m_hwndEvent;        // App window to receive events.
    PlayerState             m_state;            // Current state of the media session.
    HANDLE                  m_hCloseEvent;      // Event to wait on while closing.
};

