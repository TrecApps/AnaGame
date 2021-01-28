#include "TThread.h"

#include "TThread.h"
#include "TDataArray.h"

static SECURITY_ATTRIBUTES att{
	sizeof(SECURITY_ATTRIBUTES),
	nullptr,
	TRUE
};

static TDataArray<TThread> threadList;




DWORD __stdcall RunThread(LPVOID param)
{
	ThreadDetails* threadParam = reinterpret_cast<ThreadDetails*>(param);

	if (!threadParam->function || !threadParam->functionParams ||
		!threadParam->handle)
		throw L"Null Value detected in Thread Management!";



	DWORD res = threadParam->function(threadParam->functionParams);

	CloseHandle(threadParam->handle);
	threadParam->handle = 0;
	threadParam->threadId = 0;
	return res;
}

TThread::TThread()
{
	details.function = 0;
	details.functionParams = 0;
	details.handle = 0;
	details.threadId = 0;
	details.sleep = false;
}

TThread::TThread(LPTHREAD_START_ROUTINE routine, LPVOID params)
{
	details.function = routine;
	details.functionParams = params;

	details.threadId = 0;
	details.handle = 0;
	details.sleep = false;
}

TThread::TThread(const TThread& orig)
{
	details.threadId = orig.details.threadId;
	details.handle = orig.details.handle;
	details.function = orig.details.function;
	details.functionParams = orig.details.functionParams;
	details.sleep = orig.details.sleep;
}

bool TThread::Run()
{
	if (!details.handle)
		return false;
	ResumeThread(details.handle);
	return true;
}






bool TThread::Suspend(DWORD id)
{
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].details.threadId == id)
		{
			SuspendThread(threadList[Rust].details.handle);
			return true;
		}
	}
	return false;
}

bool TThread::Resume(DWORD id)
{
	Sleep(100);

	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].details.threadId == id)
		{
			DWORD res = ResumeThread(threadList[Rust].details.handle);
			int e = GetLastError();
			return true;
		}
	}
	return false;
}

DWORD TThread::Sleep(DWORD dwMilliseconds)
{
	return SleepEx(dwMilliseconds, TRUE);
}

DWORD TThread::CreateTThread(LPTHREAD_START_ROUTINE routine, LPVOID params)
{
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (!threadList[Rust].details.handle)
		{
			threadList[Rust].details.function = routine;
			threadList[Rust].details.functionParams = params;
			threadList[Rust].details.handle = CreateThread(&att, 0, RunThread,
				&threadList[Rust].details, CREATE_SUSPENDED, &threadList[Rust].details.threadId);
			return threadList[Rust].details.threadId;
		}
	}
	threadList.push_back(TThread(routine, params));
	threadList[threadList.Size() - 1].Initialize();
	return threadList[threadList.Size() - 1].details.threadId;
}

HANDLE TThread::GetHandleFromId(DWORD id)
{
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].details.threadId == id)
		{
			return threadList[Rust].details.handle;
		}
	}
	return nullptr;
}

void TThread::WakableSleep()
{
	DWORD id = GetCurrentThreadId();
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].details.threadId == id)
		{
			threadList[Rust].details.sleep = true;

			while (threadList[Rust].details.sleep)
				Sleep(50);
			return;
		}
	}
}

void TThread::WakeFromSleep(DWORD id)
{
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].details.threadId == id)
		{
			threadList[Rust].details.sleep = false;
			Sleep(60);
		}
	}
}

void TThread::Initialize()
{
	details.handle = CreateThread(&att, 0, RunThread, &details, CREATE_SUSPENDED, &details.threadId);
}