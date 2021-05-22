#include "TScheduler.h"
#include <mfapi.h>

TScheduler::TScheduler(TrecComPointer<IMFStreamSink> s)
{
	streamer = s;
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

void TScheduler::SetClockRate(float fRate)
{
	frameRate = fRate;
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

	// To-Do: Discard Samples

	// End To-Do
	timeEndPeriod(1);
	if (clock)
		clock->Release();
	return S_OK;
}
