#include "TScheduler.h"
#include "TStreamSink.h"
#include <mfapi.h>
#include <Mferror.h>

/**
 * Method: TScheduler::TScheduler
 * Purpose: Constructor
 * Parameters:TrecComPointer<IMFStreamSink> s - the stream sink using this scheduler
 * Returns: New Scheduler Object
 */
TScheduler::TScheduler(TrecComPointer<IMFStreamSink> s):
	callBack(this, &TScheduler::OnTimer)
{
	streamer = s;
	waiter = 0;
	this->timerKey = 0;
	frameRate = 1.0f;
	lastSampleTime = 0;
}

/**
 * Method: TScheduler::~TScheduler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TScheduler::~TScheduler()
{
}




/**
 * Method: TScheduler::StartScheduler
 * Purpose: Sets up a New clock
 * Parameters:IMFClock* pClock - the clock to work with
 * Returns: HRESULT - should be S_OK
 */
HRESULT TScheduler::StartScheduler(IMFClock* pClock)
{
	// If we currently have a clock, prepare to get rid of it
	if (clock)
		clock->Release();
	clock = pClock;
	// If we have a new clock, update reference counter
	if (clock)
		clock->AddRef();

	timeBeginPeriod(1);

	waitTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);
	return waitTimer ? S_OK : HRESULT_FROM_WIN32(GetLastError());
}

/**
 * Method: TScheduler::StopScheduler
 * Purpose: Closes the timer set up and releases the current clock
 * Parameters: void
 * Returns: HRESULT - S_OK
 */
HRESULT TScheduler::StopScheduler(void)
{
	// If we have a timer, close it and nullify it
	if (waitTimer)
	{
		CloseHandle(waitTimer);
		waitTimer = nullptr;
	}
	// Remove any sample we may have
	samples.Flush();
	timeEndPeriod(1);

	// Remove the exising clock
	if (clock)
		clock->Release();
	clock = nullptr;
	return S_OK;
}




/**
 * Method: TScheduler::IsInit
 * Purpose: Reports whether the Scheduler is initialized
 * Parameters: void
 * Returns: bool - true if initialized
 *
 * Attributes: override
 */
bool TScheduler::IsInit()
{
	return streamer.Get() != nullptr;
}

/**
 * Method: TScheduler::Flush
 * Purpose: Undoes the timer and removes all frames
 * Parameters: coid
 * Returns: HRESULT - S_OK
 */
HRESULT TScheduler::Flush(void)
{
	ThreadLock();

	samples.Flush();
	if (timerKey)
	{
		MFCancelWorkItem(timerKey);
		timerKey = 0;
	}

	ThreadRelease();
	return S_OK;
}


/**
 * Method: TScheduler::StartProcessSample
 * Purpose: Begins the Processing queue, taking into account when the frame should be presented
 * Parameters: void
 * Returns: HRESULT - should be S_OK
 */
HRESULT TScheduler::StartProcessSample()
{
	LONG wait = INFINITE;
	bool exitThread = false;
	IMFAsyncResult* asyncRes = nullptr;

	// Start Processing Samples
	HRESULT ret = ProcessSamplesInQueue(&wait);

	if (SUCCEEDED(ret))
	{
		// If we need to sleep (because the sample is not scheduled to proceed
		if (INFINITE != wait && 0 < wait)
		{
			// Set a timer for when to present
			LARGE_INTEGER llDueTime;
			llDueTime.QuadPart = -10000 * wait;
			if (SetWaitableTimer(waitTimer, &llDueTime, 0, NULL, NULL, FALSE) == 0)
			{
				ret = HRESULT_FROM_WIN32(GetLastError());
			}

			if (SUCCEEDED(ret))
			{
				// queue a wait item to wait for timer completion
				ret = MFCreateAsyncResult(nullptr, &callBack, nullptr, &asyncRes);
				if (SUCCEEDED(ret))
				{
					ret = MFPutWaitingWorkItem(waitTimer, 0, asyncRes, &timerKey);
				}
			}
		}
	}
	if (asyncRes)
		asyncRes->Release();

	return ret;
}

/**
 * Method: TScheduler::ScheduleSample
 * Purpose: Schedules a sample for presentation
 * Parameters: TSampleTexture* pSample - the sample to process
 *              bool bPesentNow - whether to present the frame upon this call
 * Returns: HRESULT - Should be S_OK
 */
HRESULT TScheduler::ScheduleSample(TrecComPointer<IMFSample> pSample, bool now)
{
	if (!IsInit())
		return MF_E_NOT_INITIALIZED;
	HRESULT ret = S_OK;
	ThreadLock();

	// If told to present now (or we don't have a clock), present the current frame
	if (now || !clock)
	{
		// dynamic_cast<TStreamSink*>(streamer.Get())->PresentFrame();
	}
	else
	{
		samples.Push(pSample);
		//ret = MFPutWorkItem(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, &callBack, nullptr);
	}

	ThreadRelease();
	return ret;
}

/**
 * Method: TScheduler::OnTimer
 * Purpose: Callback for running in a seperate thread
 * Parameters: IMFAsyncResult* pResult - standard parameter for Media Foundation callbacks
 * Returns: HRESULT - Result of processing the frames
 */
HRESULT TScheduler::OnTimer(__RPC__in_opt IMFAsyncResult* pResult)
{
	ThreadLock();

	this->timerKey = 0;
	HRESULT ret = StartProcessSample();

	ThreadRelease();
	return ret;
}
