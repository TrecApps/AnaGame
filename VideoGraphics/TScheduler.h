#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <TrecReference.h>
#include <TLinkedList.h>
#include <mfobjects.h>
#include "TComCommon.h"
#include "TSampleTexture.h"

/**
 * Class: TScheduler
 * Purpose: Helps Anagame Sinks manage the Timing of Frame Presentation
 */
class TScheduler :
    public TObject
{
public:

    /**
     * Method: TScheduler::TScheduler
     * Purpose: Constructor
     * Parameters:TrecComPointer<IMFStreamSink> s - the stream sink using this scheduler
     * Returns: New Scheduler Object
     */
    TScheduler(TrecComPointer<IMFStreamSink> s);
    /**
     * Method: TScheduler::~TScheduler
     * Purpose: Destructor
     * Parameters: void
     * Returns: void
     */
    virtual ~TScheduler();

    /**
     * Method: TScheduler::SetFrameRate
     * Purpose: Sets the new Frame-rate
     * Parameters: const MFRatio& fps -  the frame rarte to work with
     * Returns: void
     */
    void SetFrameRate(const MFRatio& fps);
    /**
     * Method: TScheduler::SetClockRate
     * Purpose: Sets the clock rate
     * Parameters: float fRate -  the new rate to use
     * Returns: HRESULT - S_OK
     */
    HRESULT SetClockRate(float fRate);

    /**
     * Method: TScheduler::LastSampleTime
     * Purpose:Reports the time of the last sample
     * Parameters: 
     * Returns: LONGLONG - the last sample processed and the time it was
     * 
     * Note: Value not set
     */
    const LONGLONG& LastSampleTime(void) const;
    /**
     * Method: TScheduler::FrameDuration
     * Purpose: Reports the frame duration
     * Parameters: void
     * Returns:LONGLONG - the frame duration
     * 
     * Note: Value not set
     */
    const LONGLONG& FrameDuration(void) const;

    /**
     * Method: TScheduler::StartScheduler
     * Purpose: Sets up a New clock
     * Parameters:IMFClock* pClock - the clock to work with
     * Returns: HRESULT - should be S_OK
     */
    HRESULT StartScheduler(IMFClock* pClock);
    /**
     * Method: TScheduler::StopScheduler
     * Purpose: Closes the timer set up and releases the current clock
     * Parameters: void
     * Returns: HRESULT - S_OK
     */
    HRESULT StopScheduler(void);

    /**
     * Method: TScheduler::ScheduleSample
     * Purpose: Schedules a sample for presentation
     * Parameters: TSampleTexture* pSample - the sample to process 
     *              bool bPesentNow - whether to present the frame upon this call
     * Returns: HRESULT - Should be S_OK
     */
    HRESULT ScheduleSample(TSampleTexture* pSample, bool bPesentNow);
    /**
     * Method: TScheduler::ProcessSamplesInQueue
     * Purpose: Processes the samples currently in the queue
     * Parameters: LONG* plNextSleep - how long the thread should sleep
     * Returns: HRESULT - should be S_OK
     */
    HRESULT ProcessSamplesInQueue(LONG* plNextSleep);
    /**
     * Method: TScheduler::ProcessSample
     * Purpose: Checks the timing of the sample relative to the current time and determines if to present of store
     * Parameters: TSampleTexture* pSample - the sample to process
     *              LONG* plNextSleep - how long the thread should sleep
     * Returns: HRESULT - should be S_OK
     */
    HRESULT ProcessSample(TSampleTexture* pSample, LONG* plNextSleep);
    /**
     * Method: TScheduler::Flush
     * Purpose: Undoes the timer and removes all frames
     * Parameters: coid
     * Returns: HRESULT - S_OK
     */
    HRESULT Flush(void);

    /**
     * Method: TScheduler::GetSampleCount
     * Purpose: Reports the number of samples currently held by the scheduler
     * Parameters: void
     * Returns: UINT - number of samples currently being held
     */
    UINT GetSampleCount();

    //ULONG AddRef();
    //ULONG Release();
    //HRESULT QueryInterface(,void** ppv);

protected:
    /**
     * Method: TScheduler::StartProcessSample
     * Purpose: Begins the Processing queue, taking into account when the frame should be presented
     * Parameters: void
     * Returns: HRESULT - should be S_OK
     */
    HRESULT StartProcessSample();
    /**
     * Method: TScheduler::OnTimer
     * Purpose: Callback for running in a seperate thread
     * Parameters: IMFAsyncResult* pResult - standard parameter for Media Foundation callbacks
     * Returns: HRESULT - Result of processing the frames
     */
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
    TLinkedList<TSampleTexture*> samples;
};

