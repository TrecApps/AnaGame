#include "TThreadTester.h"
#include <TThread.h>

void TThreadTester::RunTests(std::string& output)
{
	multiThreaded(output);
}

std::string TThreadTester::GetTestName()
{
	return std::string("Threading");
}

void TThreadTester::Before()
{
}

void TThreadTester::After()
{
}

void TThreadTester::multiThreaded(std::string& output)
{
	TrecPointer<TString> str = TrecPointerKey::GetNewTrecPointer<TString>(L"This string!");

	ThreadData data1, data2;

	data1.string = str;
	data2.string = str;

	data1.value = 111;
	data2.value = 222;


	data1.id = TThread::CreateTThread(TestThread, &data1);
	data1.handle = TThread::GetHandleFromId(data1.id);
	data2.id = TThread::CreateTThread(TestThread, &data2);
	data2.handle = TThread::GetHandleFromId(data2.id);


	std::string testName("Multi-Threaded");

	ExpectTrue(output, testName, data1.handle && data2.handle);

	if (data1.handle > 0)
		TThread::Resume(data1.id);

	if (data2.handle > 0)
		TThread::Resume(data2.id);

	HANDLE handles[2] = { data1.handle, data2.handle };

	HANDLE thisThread = OpenThread(SYNCHRONIZE | THREAD_SUSPEND_RESUME, 0, GetCurrentThreadId());

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);

	ExpectEquals(output, testName, (UINT)0, data1.id);

	bool valid = str.Get() != nullptr;
	ExpectTrue(output, testName, valid);

	if (valid)
	{
		output += "\n\n" + str->GetRegString() + "\n\n";
	}
}

DWORD __stdcall TestThread(LPVOID param)
{
	ThreadData* data = reinterpret_cast<ThreadData*>(param);

	//Sleep(1000);

	TrecPointer<TString> str = data->string;

	for (UINT Rust = 0; Rust < 20; Rust++)
	{
		if (!str.Get())
			return 1;

		str->AppendFormat(L" %i ", data->value);
	}

	return 0;
}