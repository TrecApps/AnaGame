#pragma once
#include <TObject.h>
#include <mfidl.h>
#include <TrecReference.h>
#include <TLinkedList.h>
#include <mfobjects.h>
#include "TComCommon.h"
#include "TSampleTexture.h"
#include "TBaseScheduler.h"

/**
 * Class: TScheduler
 * Purpose: Helps Anagame Sinks manage the Timing of Frame Presentation
 */
class TScheduler :
    public TBaseScheduler
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


    //ULONG AddRef();
    //ULONG Release();
    //HRESULT QueryInterface(,void** ppv);

protected:

    /**
     * Method: TScheduler::IsInit
     * Purpose: Reports whether the Scheduler is initialized
     * Parameters: void
     * Returns: bool - true if initialized
     *
     * Attributes: override
     */
    virtual bool IsInit() override;


    virtual HRESULT Flush(void) override;
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

    float frameRate;            // Playback rate.


    LONG waiter;

    TrecComPointer<IMFStreamSink> streamer;

};

