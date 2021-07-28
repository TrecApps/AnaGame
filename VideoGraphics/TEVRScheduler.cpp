#include "TEVRScheduler.h"
#include <Mferror.h>
#define EVR_E_TERMINATE WM_USER
#define EVR_E_SCHEDULE  WM_USER + 1
#define EVR_E_FLUSH     WM_USER + 2


TEVRScheduler::TEVRScheduler(TrecPointer<TPresentEngine> p)
{
    scheduleThread = readyThread = flushThread = nullptr;
    threadId = 0;
    presenter = p;
}

HRESULT TEVRScheduler::StartScheduler(IMFClock* pClock)
{
    if (scheduleThread)
        return E_UNEXPECTED;

    if (!pClock)
        return E_POINTER;

    if (clock)
        clock->Release();
    clock = pClock;
    clock->AddRef();

    HRESULT ret = S_OK;
    DWORD dId = 0, dWait = 0;

    HANDLE hObjs[2];

    timeBeginPeriod(1);

    readyThread = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!readyThread)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    flushThread = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!flushThread)
    {
        ret = HRESULT_FROM_WIN32(GetLastError());
        goto done;
    }

    scheduleThread = CreateThread(nullptr, 0, MainScheduleProc, (LPVOID)this, 0, &dId);
    if (!scheduleThread)
    {
        ret = HRESULT_FROM_WIN32(GetLastError());
        goto done;
    }

    hObjs[0] = readyThread;
    hObjs[1] = scheduleThread;

    dWait = WaitForMultipleObjects(2, hObjs, FALSE, INFINITE);

    if (dWait != WAIT_OBJECT_0)
    {
        CloseHandle(scheduleThread);
        scheduleThread = nullptr;
        ret = E_UNEXPECTED;
        goto done;
    }
    threadId = dId;

done:
    if (readyThread)
    {
        CloseHandle(readyThread);
        readyThread = nullptr;
    }

    return ret;
}

HRESULT TEVRScheduler::StopScheduler(void)
{
    if (!scheduleThread)
        return S_OK;
    // Send message to thread
    PostThreadMessage(threadId, EVR_E_TERMINATE, 0, 0);

    // Wait for thread to close
    WaitForSingleObject(scheduleThread, INFINITE);

    // Close handle
    CloseHandle(scheduleThread);
    scheduleThread = nullptr;
    CloseHandle(flushThread);
    flushThread = nullptr;

    samples.Flush();

    return S_OK;
}

HRESULT TEVRScheduler::ScheduleSample(TrecComPointer<IMFSample> pSample, bool bPresentNow)
{
    if (!presenter.Get() || !scheduleThread)
        return MF_E_NOT_INITIALIZED;
    if (!pSample.Get())
        return E_POINTER;



    DWORD exitCode = 0;
    GetExitCodeThread(scheduleThread, &exitCode);
    if (exitCode != STILL_ACTIVE)
        return E_FAIL;

    if (bPresentNow || !clock)
        presenter;
    else
    {
        samples.Push(pSample);
        PostThreadMessage(threadId, EVR_E_SCHEDULE, 0, 0);
    }


        return S_OK;
}

DWORD __stdcall TEVRScheduler::MainScheduleProc(LPVOID lpParameter)
{
    TEVRScheduler* sch = reinterpret_cast<TEVRScheduler*>(lpParameter);
    if (!sch)
        return -1;
    return sch->ScheduleProc();
}

HRESULT TEVRScheduler::Flush(void)
{
    if (scheduleThread)
    {
        PostThreadMessage(threadId, EVR_E_FLUSH, 0, 0);

        HANDLE hObjs[]= { flushThread, scheduleThread };
        WaitForMultipleObjects(ARRAYSIZE(hObjs), hObjs, FALSE, 5000);


    }
    return S_OK;
}

bool TEVRScheduler::IsInit()
{
    return scheduleThread != nullptr;
}

DWORD TEVRScheduler::ScheduleProc()
{
    HRESULT res = S_OK;
    MSG m;
    LONG wait = INFINITE;
    BOOL exitThread = FALSE;

    // Get A message for this thread
    PeekMessage(&m, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

    // Signal that thread is ready
    SetEvent(readyThread);

    while (!exitThread)
    {
        // Wait for thread message
        DWORD dRes = MsgWaitForMultipleObjects(0, nullptr, FALSE, wait, QS_POSTMESSAGE);

        if (dRes == WAIT_TIMEOUT)
        {
            // If we timed out, then process the samples
            if(FAILED(ProcessSamplesInQueue(&wait)))
                exitThread = TRUE;
        }

        while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE))
        {
            bool processSamp = true;

            switch (m.message)
            {
            case EVR_E_TERMINATE:
                exitThread = true;
                break;
            case EVR_E_SCHEDULE:
                if (processSamp)
                {
                    if(FAILED(ProcessSamplesInQueue(&wait)))
                        exitThread = TRUE;
                    processSamp = (wait != INFINITE);
                }
                break;
            case EVR_E_FLUSH:
                wait = INFINITE;
                samples.Flush();
                SetEvent(flushThread);
            }
        }
    }

    return SUCCEEDED(res) ? 0:1;
}
