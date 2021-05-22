#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <TrecReference.h>

class TScheduler :
    public TObject
{
public:

    TScheduler(TrecComPointer<IMFStreamSink> s);
    virtual ~TScheduler();

    void SetFrameRate(const MFRatio& fps);
    void SetClockRate(float fRate);// { m_fRate = fRate; }

    const LONGLONG& LastSampleTime(void) const;// { return m_LastSampleTime; }
    const LONGLONG& FrameDuration(void) const;// { return m_PerFrameInterval; }

    HRESULT StartScheduler(IMFClock* pClock);
    HRESULT StopScheduler(void);

protected:
    IMFClock* clock;            // Presentation clock. Can be NULL.
    float frameRate;            // Playback rate.
    MFTIME lastSampleTime;      // Most recent sample time.
    MFTIME perFrameInterval;    // Duration of each frame.
    LONGLONG perFrameQuarter;   // 1/4th of the frame duration.
    HANDLE waitTimer;           // Handle to the wait timer 

    TrecComPointer<IMFStreamSink> streamer;
};

