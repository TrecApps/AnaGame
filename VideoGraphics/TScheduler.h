#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <TrecReference.h>
#include <TLinkedList.h>
#include <mfobjects.h>
#include "TComCommon.h"

class TScheduler :
    public TObject
{
public:

    TScheduler(TrecComPointer<IMFStreamSink> s);
    virtual ~TScheduler();

    void SetFrameRate(const MFRatio& fps);
    HRESULT SetClockRate(float fRate);// { m_fRate = fRate; }

    const LONGLONG& LastSampleTime(void) const;// { return m_LastSampleTime; }
    const LONGLONG& FrameDuration(void) const;// { return m_PerFrameInterval; }

    HRESULT StartScheduler(IMFClock* pClock);
    HRESULT StopScheduler(void);

    HRESULT ScheduleSample(IMFSample* pSample, bool bPesentNow);
    HRESULT ProcessSamplesInQueue(LONG* plNextSleep);
    HRESULT ProcessSample(IMFSample* pSample, LONG* plNextSleep);
    HRESULT Flush(void);

    UINT GetSampleCount();

    //ULONG AddRef();
    //ULONG Release();
    //HRESULT QueryInterface(,void** ppv);

protected:
    HRESULT StartProcessSample();
    HRESULT OnTimer(__RPC__in_opt IMFAsyncResult* pResult);
    TComCallback<TScheduler> callBack;                 // Callback mechanism

    IMFClock* clock;            // Presentation clock. Can be NULL.
    float frameRate;            // Playback rate.
    MFTIME lastSampleTime;      // Most recent sample time.
    MFTIME perFrameInterval;    // Duration of each frame.
    LONGLONG perFrameQuarter;   // 1/4th of the frame duration.
    HANDLE waitTimer;           // Handle to the wait timer 
    LONG waiter;
    LONGLONG nextSleep;
    ULONG64 timerKey;

    TrecComPointer<IMFStreamSink> streamer;
    TLinkedList<IMFSample*> samples;
};

