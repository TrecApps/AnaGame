#include "TScheduler.h"
#include "TStreamSink.h"
#include <mfapi.h>
#include <Mferror.h>

TScheduler::TScheduler(TrecComPointer<IMFStreamSink> s):
	callBack(this, &TScheduler::OnTimer)
{
	streamer = s;
	waiter = 0;
	this->timerKey = 0;
}

TScheduler::~TScheduler()
{
}

void TScheduler::SetFrameRate(const MFRatio& fps)
{
	UINT64 timeFrame = 0;

	MFFrameRateToAverageTimePerFrame(fps.Numerator, fps.Denominator, &timeFrame);
	perFrameInterval = (MFTIME)timeFrame;
	perFrameQuarter = perFrameInterval / 4;
}

HRESULT TScheduler::SetClockRate(float fRate)
{
	frameRate = fRate;
	return S_OK;
}

const LONGLONG& TScheduler::LastSampleTime(void) const
{
	return lastSampleTime;
}

const LONGLONG& TScheduler::FrameDuration(void) const
{
	return perFrameInterval;
}

HRESULT TScheduler::StartScheduler(IMFClock* pClock)
{
	if (clock)
		clock->Release();
	clock = pClock;
	if (clock)
		clock->AddRef();

	timeBeginPeriod(1);

	waitTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);
	return waitTimer ? S_OK : HRESULT_FROM_WIN32(GetLastError());
}

HRESULT TScheduler::StopScheduler(void)
{
	if (waitTimer)
	{
		CloseHandle(waitTimer);
		waitTimer = nullptr;
	}

	samples.Flush();
	timeEndPeriod(1);
	if (clock)
		clock->Release();
	return S_OK;
}

HRESULT TScheduler::ScheduleSample(IMFSample* pSample, bool now)
{
	if (!streamer.Get())
		return MF_E_NOT_INITIALIZED;
	HRESULT ret = S_OK;
	ThreadLock();

	if (now || !clock)
	{
		dynamic_cast<TStreamSink*>(streamer.Get())->PresentFrame();
	}
	else
	{
		samples.Push(pSample);
		ret = MFPutWorkItem(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, &callBack, nullptr);
	}

	ThreadRelease();
	return ret;
}

HRESULT TScheduler::ProcessSamplesInQueue(LONG* plNextSleep)
{
	HRESULT ret = S_OK;
	LONG wait = 0;
	ThreadLock();
	IMFSample* sample = nullptr;
	while (samples.Dequeue(sample) && sample)
	{
		ret = ProcessSample(sample, &wait);
		sample->Release();
		if (FAILED(ret) || wait > 0)
			break;
	}

	*plNextSleep = wait == 0 ? INFINITE : wait;

	ThreadRelease();

	return ret;
}

HRESULT TScheduler::ProcessSample(IMFSample* pSample, LONG* plNextSleep)
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

		ret = pSample->GetSampleTime(&presentTime);
		if (SUCCEEDED(ret))
		{
			ret =clock->GetCorrelatedTime(0, &currentTime, &systemTime);
		}
		if (SUCCEEDED(ret))
		{
			delta = presentTime = currentTime;
			if (frameRate < 0)
				delta = -delta;

			if (delta < -this->perFrameQuarter)
				doPresent = true;
			else if (delta > (3 * perFrameQuarter))
			{
				nextSleep = (delta - (3 * perFrameQuarter)) / (10000.0 * frameRate);
				doPresent = false;
			}
		}

	}

	if (doPresent)
		ret = dynamic_cast<TStreamSink*>(streamer.Get())->PresentFrame();
	else
		samples.Push(pSample);

	ThreadRelease();
	return ret;
}

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

UINT TScheduler::GetSampleCount()
{
	return samples.GetSize();
}

HRESULT TScheduler::StartProcessSample()
{
	LONG wait = INFINITE;
	bool exitThread = false;
	IMFAsyncResult* asyncRes = nullptr;

	HRESULT ret = ProcessSamplesInQueue(&wait);

	if (SUCCEEDED(ret))
	{
		if (INFINITE != wait && 0 < wait)
		{
			LARGE_INTEGER llDueTime;
			llDueTime.QuadPart = -10000 * wait;
			if (SetWaitableTimer(waitTimer, &llDueTime, 0, NULL, NULL, FALSE) == 0)
			{
				ret = HRESULT_FROM_WIN32(GetLastError());
			}

			if (SUCCEEDED(ret))
			{
				// queue a waititem to wait for timer completion
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

HRESULT TScheduler::OnTimer(IMFAsyncResult* pResult)
{
	ThreadLock();

	this->timerKey = 0;
	HRESULT ret = StartProcessSample();

	ThreadRelease();
	return ret;
}
