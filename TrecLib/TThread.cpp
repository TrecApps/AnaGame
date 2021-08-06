#include "TThread.h"
#include "TDataArray.h"
#include <cassert>

SECURITY_ATTRIBUTES att{
	sizeof(SECURITY_ATTRIBUTES),
	nullptr,
	TRUE
};

TDataArray<TThread> threadList;




DWORD __stdcall RunThread(LPVOID param)
{
	ThreadDetails* threadParam = reinterpret_cast<ThreadDetails*>(param);

	if (!threadParam->function || !threadParam->functionParams ||
		!threadParam->handle)
		throw L"Null Value detected in Thread Management!";

	Sleep(20);

	DWORD res = threadParam->function(threadParam->functionParams);

	CloseHandle(threadParam->handle);
	threadParam->handle = 0;
	threadParam->threadId = 0;
	threadParam->function = nullptr;
	threadParam->functionParams = nullptr;
	return res;
}

TThread::TThread()
{
	details.function = 0;
	details.functionParams = 0;
	details.handle = 0;
	details.threadId = 0;
	details.sleep = false;
	details.type = ThreadType::tt_regular;
	object = nullptr;
}

TThread::TThread(LPTHREAD_START_ROUTINE routine, LPVOID params)
{
	details.function = routine;
	details.functionParams = params;

	details.threadId = 0;
	details.handle = 0;
	details.sleep = false;

	details.type = ThreadType::tt_regular;

	object = nullptr;
}

TThread::TThread(const TThread& orig)
{
	details.threadId = orig.details.threadId;
	details.handle = orig.details.handle;
	details.function = orig.details.function;
	details.functionParams = orig.details.functionParams;
	details.sleep = orig.details.sleep;
	details.type = orig.details.type;
	object = orig.object;
}

bool TThread::Run()
{
	if (!details.handle)
		return false;
	ResumeThread(details.handle);
	return true;
}






void TThread::SetMainThread()
{
	if (threadList.Size())
	{
		throw L"Attempt to set second Main thread detected!";
	}

	TThread thread;
	thread.details.handle = GetCurrentThread();
	thread.details.threadId = GetCurrentThreadId();
	thread.details.type = ThreadType::tt_main;

	threadList.push_back(thread);
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
	if (!threadList.Size())
		throw L"No Main Thread Set up!";

	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (!threadList[Rust].details.handle)
		{
			threadList[Rust].details.function = routine;
			threadList[Rust].details.functionParams = params;
			threadList[Rust].details.handle = CreateThread(&att, 0, RunThread,
				&threadList[Rust].details, CREATE_SUSPENDED, &threadList[Rust].details.threadId);
			threadList[Rust].details.type = ThreadType::tt_regular;
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

void TThread::WakableSleep(const TObject* obj)
{
	DWORD id = GetCurrentThreadId();
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].details.threadId == id)
		{
			threadList[Rust].details.sleep = true;

			while (threadList[Rust].details.sleep && (!obj || threadList[Rust].object == obj))
				Sleep(20);
			threadList[Rust].details.sleep = false;
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

void TThread::Suspend(const TObject* obj)
{
	DWORD id = GetCurrentThreadId();
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].details.threadId == id)
		{
			memcpy_s(&threadList[Rust].object, sizeof(TObject*), obj, sizeof(TObject*));
			SuspendThread(threadList[Rust].details.handle);
			return;
		}
	}
	// We should never reach this point
	assert(false);
}

void TThread::Resume(const TObject* obj)
{
	if (!obj)
		return;
	for (UINT Rust = 0; Rust < threadList.Size(); Rust++)
	{
		if (threadList[Rust].object == obj)
		{
			threadList[Rust].object = nullptr;
			ResumeThread(threadList[Rust].details.handle);
			return;
		}
	}
}

void TThread::Initialize()
{
	details.handle = CreateThread(&att, 0, RunThread, &details, CREATE_SUSPENDED, &details.threadId);
}