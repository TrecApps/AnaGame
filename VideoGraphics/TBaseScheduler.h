#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include <TLinkedList.h>
#include <mfapi.h>
#include <mfidl.h>

class TBaseScheduler :
    public TObject
{
public:
    /**
     * Method: TBaseScheduuler::TBaseScheduler
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: new Base Scheduler
     */
    TBaseScheduler();

    /**
     * Method: TBaseScheduler::TBaseScheduler
     * Purpose: Destructor
     * Parameters: void
     * Returns: void
     */
    virtual ~TBaseScheduler();

    /**
     * Method: TBaseScheduler::SetClockRate
     * Purpose: Sets the clock rate
     * Parameters: float fRate -  the new rate to use
     * Returns: HRESULT - S_OK
     */
    HRESULT SetClockRate(float fRate);

    /**
     * Method: TBaseScheduler::Flush
     * Purpose: Undoes the timer and removes all frames
     * Parameters: coid
     * Returns: HRESULT - S_OK
     */
    virtual HRESULT Flush(void);


    /**
     * Method: TBaseScheduler::ScheduleSample
     * Purpose: Schedules a sample for presentation
     * Parameters: TSampleTexture* pSample - the sample to process
     *              bool bPesentNow - whether to present the frame upon this call
     * Returns: HRESULT - Should be S_OK
     */
    virtual HRESULT ScheduleSample(TrecComPointer<IMFSample> pSample, bool bPesentNow) = 0;

    /**
     * Method: TScheduler::ProcessSample
     * Purpose: Checks the timing of the sample relative to the current time and determines if to present of store
     * Parameters: TSampleTexture* pSample - the sample to process
     *              LONG* plNextSleep - how long the thread should sleep
     * Returns: HRESULT - should be S_OK
     */
    HRESULT ProcessSample(TrecComPointer<IMFSample> pSample, LONG* plNextSleep);

    /**
     * Method: TScheduler::ProcessSamplesInQueue
     * Purpose: Processes the samples currently in the queue
     * Parameters: LONG* plNextSleep - how long the thread should sleep
     * Returns: HRESULT - should be S_OK
     */
    HRESULT ProcessSamplesInQueue(LONG* plNextSleep);

    /**
     * Method: TScheduler::SetFrameRate
     * Purpose: Sets the new Frame-rate
     * Parameters: const MFRatio& fps -  the frame rarte to work with
     * Returns: void
     */
    void SetFrameRate(const MFRatio& fps);

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
     * Method: TScheduler::GetSampleCount
     * Purpose: Reports the number of samples currently held by the scheduler
     * Parameters: void
     * Returns: UINT - number of samples currently being held
     */
    UINT GetSampleCount();
protected:

    /**
     * Method: TBaseScheduler::IsInit
     * Purpose: Reports whether the Scheduler is initialized
     * Parameters: void
     * Returns: bool - true if initialized
     * 
     * Attributes: abstract
     */
    virtual bool IsInit() = 0;

    IMFClock* clock;            // Presentation clock. Can be NULL.


    float frameRate;            // Playback rate.
    HANDLE waitTimer;           // Handle to the wait timer 
    ULONG64 timerKey;
    LONGLONG perFrameQuarter;   // 1/4th of the frame duration.
    MFTIME lastSampleTime;      // Most recent sample time.
    MFTIME perFrameInterval;    // Duration of each frame.
    LONGLONG nextSleep;



    TLinkedList<TrecComPointer<IMFSample>> samples;
};

