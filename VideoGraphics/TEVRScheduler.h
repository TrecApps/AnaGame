#pragma once
#include "TBaseScheduler.h"
#include <mfplay.h>
#include "TPresentEngine.h"
class TEVRScheduler :
    public TBaseScheduler
{
public:
    /**
     * Method: TEVRScheduler::TEVRScheduler
     * Purpose: Constructor
     * Parameters:
     * Returns: the Evr scheduler
     */
    TEVRScheduler(TrecPointer<TPresentEngine> p);

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
     * Method: TBaseScheduler::ScheduleSample
     * Purpose: Schedules a sample for presentation
     * Parameters: TSampleTexture* pSample - the sample to process
     *              bool bPesentNow - whether to present the frame upon this call
     * Returns: HRESULT - Should be S_OK
     */
    virtual HRESULT ScheduleSample(TrecComPointer<IMFSample> pSample, bool bPesentNow) override;

    static DWORD WINAPI MainScheduleProc(LPVOID lpParameter);
    virtual HRESULT Flush(void) override;
protected:



    /**
     * Method: TBaseScheduler::IsInit
     * Purpose: Reports whether the Scheduler is initialized
     * Parameters: void
     * Returns: bool - true if initialized
     *
     * Attributes: abstract
     */
    virtual bool IsInit() override;

    DWORD ScheduleProc();

    TrecPointer<TPresentEngine> presenter;


    HANDLE scheduleThread, readyThread, flushThread;
    DWORD threadId;
};

