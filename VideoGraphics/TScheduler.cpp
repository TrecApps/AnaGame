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
 * Method: TScheduler::SetFrameRate
 * Purpose: Sets the new Frame-rate
 * Parameters: const MFRatio& fps -  the frame rarte to work with
 * Returns: void
 */
void TScheduler::SetFrameRate(const MFRatio& fps)
{
	// Set basic value
	UINT64 timeFrame = 0;

	// Perform calculation
	MFFrameRateToAverageTimePerFrame(fps.Numerator, fps.Denominator, &timeFrame);
	// Convert the value result
	perFrameInterval = (MFTIME)timeFrame;
	// Gets used often, save at least one calculation
	perFrameQuarter = perFrameInterval / 4;
}

/**
 * Method: TScheduler::SetClockRate
 * Purpose: Sets the clock rate
 * Parameters: float fRate -  the new rate to use
 * Returns: HRESULT - S_OK
 */
HRESULT TScheduler::SetClockRate(float fRate)
{
	frameRate = fRate;
	return S_OK;
}

/**
 * Method: TScheduler::LastSampleTime
 * Purpose: Reports the time of the last sample
 * Parameters: void
 * Returns: LONGLONG - the last sample processed and the time it was
 *
 * Note: Value not set
 */
const LONGLONG& TScheduler::LastSampleTime(void) const
{
	return lastSampleTime;
}

/**
 * Method: TScheduler::FrameDuration
 * Purpose: Reports the frame duration
 * Parameters: void
 * Returns:LONGLONG - the frame duration
 *
 * Note: Value not set
 */
const LONGLONG& TScheduler::FrameDuration(void) const
{
	return perFrameInterval;
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
 * Method: TScheduler::ScheduleSample
 * Purpose: Schedules a sample for presentation
 * Parameters: TSampleTexture* pSample - the sample to process
 *              bool bPesentNow - whether to present the frame upon this call
 * Returns: HRESULT - Should be S_OK
 */
HRESULT TScheduler::ScheduleSample(TSampleTexture* pSample, bool now)
{
	if (!streamer.Get())
		return MF_E_NOT_INITIALIZED;
	HRESULT ret = S_OK;
	ThreadLock();

	// If told to present now (or we don't have a clock), present the current frame
	if (now || !clock)
	{
		dynamic_cast<TStreamSink*>(streamer.Get())->PresentFrame();
	}
	else
	{
		pSample->AddRef();
		samples.Push(pSample);
		ret = MFPutWorkItem(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, &callBack, nullptr);
	}

	ThreadRelease();
	return ret;
}


/**
 * Method: TScheduler::ProcessSamplesInQueue
 * Purpose: Processes the samples currently in the queue
 * Parameters: LONG* plNextSleep - how long the thread should sleep
 * Returns: HRESULT - should be S_OK
 */
HRESULT TScheduler::ProcessSamplesInQueue(LONG* plNextSleep)
{
	if (!plNextSleep) return E_POINTER;
	HRESULT ret = S_OK;
	LONG wait = 0;
	ThreadLock();
	TSampleTexture* sample = nullptr;
	while (samples.Dequeue(sample) && sample)
	{
		// Process the sample from the queue
		ret = ProcessSample(sample, &wait);
		sample->Release();
		// If we need to sleep, go ahead and sleep
		if (FAILED(ret) || wait > 0)
			break;
	}

	*plNextSleep = wait == 0 ? INFINITE : wait;

	ThreadRelease();

	return ret;
}

/**
 * Method: TScheduler::ProcessSample
 * Purpose: Checks the timing of the sample relative to the current time and determines if to present of store
 * Parameters: TSampleTexture* pSample - the sample to process
 *              LONG* plNextSleep - how long the thread should sleep
 * Returns: HRESULT - should be S_OK
 */
HRESULT TScheduler::ProcessSample(TSampleTexture* pSample, LONG* plNextSleep)
{
	ThreadLock();
	
	HRESULT ret = S_OK;
	bool doPresent = true;

	if (clock)
	{
		LONGLONG presentTime = 0;
		LONGLONG currentTime = 0;
		MFTIME   systemTime = 0;
		LONGLONG delta = 0;

		// Get the IMFSample
		IMFSample* samp = nullptr;
		pSample->QueryInterface(__uuidof(IMFSample), (void**)&samp);

		// Get the sample time
		ret = samp->GetSampleTime(&presentTime);
		if (SUCCEEDED(ret))
		{
			// Get info on the current time
			ret =clock->GetCorrelatedTime(0, &currentTime, &systemTime);
		}
		if (SUCCEEDED(ret))
		{
			// Figure out time between now and when sample should be displayed
			delta = presentTime - currentTime;
			// In case video is playing backwards
			if (frameRate < 0)
				delta = -delta;

			// Check of negative
			if (delta < -this->perFrameQuarter)
				doPresent = true;
			else if (delta > (3 * perFrameQuarter))
			{
				// Prepare to hold off on presentig the sample
				nextSleep = (delta - (3 * perFrameQuarter)) / (10000.0 * frameRate);
				doPresent = false;
			}
		}

	}

	// Check to see if sample needs to be presented now
	if (doPresent)
		ret = dynamic_cast<TStreamSink*>(streamer.Get())->PresentFrame();
	else
	{
		// Push back to queue
		pSample->AddRef();
		samples.Push(pSample);
	}

	*plNextSleep = nextSleep;
	ThreadRelease();
	return ret;
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
 * Method: TScheduler::GetSampleCount
 * Purpose: Reports the number of samples currently held by the scheduler
 * Parameters: void
 * Returns: UINT - number of samples currently being held
 */
UINT TScheduler::GetSampleCount()
{
	return samples.GetSize();
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
