#pragma once
#include "Tester.h"
#include <Windows.h>
#include <TString.h>

/**
 * Class: ThreadData
 * Purpose: Holds information needed by the Animation threads created by the StoryBoard
 */
class ThreadData
{
public:
	TrecPointer<TString> string;
	HANDLE handle;
	int value;
	DWORD id;
};

DWORD WINAPI TestThread(LPVOID param);

class TThreadTester :
	public Tester
{
protected:
	virtual void RunTests(std::string& output) override;
	virtual std::string GetTestName() override;

	virtual void Before() override;
	virtual void After() override;

	ThreadData data1, data2;
	void multiThreaded(std::string& output);
};