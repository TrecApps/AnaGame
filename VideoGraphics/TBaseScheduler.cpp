#include "TBaseScheduler.h"
#include <mfplay.h>
#include <Mferror.h>

TBaseScheduler::TBaseScheduler()
{
	frameRate = 1.0f;
}

TBaseScheduler::~TBaseScheduler()
{
}

/**
 * Method: TScheduler::ProcessSamplesInQueue
 * Purpose: Processes the samples currently in the queue
 * Parameters: LONG* plNextSleep - how long the thread should sleep
 * Returns: HRESULT - should be S_OK
 */
HRESULT TBaseScheduler::ProcessSamplesInQueue(LONG* plNextSleep)
{
	if (!plNextSleep) return E_POINTER;
	HRESULT ret = S_OK;
	LONG wait = 0;
	ThreadLock();
	TrecComPointer<IMFSample> sample;
	while (samples.Dequeue(sample) && sample.Get())
	{
		// Process the sample from the queue
		ret = ProcessSample(sample, &wait);
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
HRESULT TBaseScheduler::ProcessSample(TrecComPointer<IMFSample> pSample, LONG* plNextSleep)
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
			ret = clock->GetCorrelatedTime(0, &currentTime, &systemTime);
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
	{
		// ret = dynamic_cast<TStreamSink*>(streamer.Get())->PresentFrame();
	}
	else
	{
		// Push back to queue
		samples.Push(pSample);
	}

	*plNextSleep = nextSleep;
	ThreadRelease();
	return ret;
}

/**
 * Method: TBaseScheduler::Flush
 * Purpose: Undoes the timer and removes all frames
 * Parameters: coid
 * Returns: HRESULT - S_OK
 */
HRESULT TBaseScheduler::Flush(void)
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
 * Method: TBaseScheduler::LastSampleTime
 * Purpose: Reports the time of the last sample
 * Parameters: void
 * Returns: LONGLONG - the last sample processed and the time it was
 *
 * Note: Value not set
 */
const LONGLONG& TBaseScheduler::LastSampleTime(void) const
{
	return lastSampleTime;
}

/**
 * Method: TBaseScheduler::FrameDuration
 * Purpose: Reports the frame duration
 * Parameters: void
 * Returns:LONGLONG - the frame duration
 *
 * Note: Value not set
 */
const LONGLONG& TBaseScheduler::FrameDuration(void) const
{
	return perFrameInterval;
}

/**
 * Method: TBaseScheduler::SetFrameRate
 * Purpose: Sets the new Frame-rate
 * Parameters: const MFRatio& fps -  the frame rarte to work with
 * Returns: void
 */
void TBaseScheduler::SetFrameRate(const MFRatio& fps)
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
 * Method: TBaseScheduler::SetClockRate
 * Purpose: Sets the clock rate
 * Parameters: float fRate -  the new rate to use
 * Returns: HRESULT - S_OK
 */
HRESULT TBaseScheduler::SetClockRate(float fRate)
{
	frameRate = fRate;
	return S_OK;
}

/**
 * Method: TScheduler::GetSampleCount
 * Purpose: Reports the number of samples currently held by the scheduler
 * Parameters: void
 * Returns: UINT - number of samples currently being held
 */
UINT TBaseScheduler::GetSampleCount()
{
	return samples.GetSize();
}
