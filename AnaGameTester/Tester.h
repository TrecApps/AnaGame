#pragma once
#include <string>
#include <Windows.h>
class Tester
{
public:
	Tester();
	void Run(std::string& output);

protected:
	virtual void RunTests(std::string& output) = 0;
	virtual std::string GetTestName() = 0;

	virtual void Before();
	virtual void After();

	void ExpectTrue(std::string& output, std::string& testName, bool value);
	void ExpectEquals(std::string& output, std::string& testName, const char* value1, const char* value2);
	void ExpectEquals(std::string& output, std::string& testName, const WCHAR* value1, const WCHAR* value2);
	void ExpectEquals(std::string& output, std::string& testName, char ch1, char ch2);
	void ExpectEquals(std::string& output, std::string& testName, WCHAR wch1, WCHAR wch2);

	void ExpectEquals(std::string& output, std::string& testName, long long expected, long long actual);
	void ExpectEquals(std::string& output, std::string& testName, double expected, double actual);
	void ExpectEquals(std::string& output, std::string& testName, UINT expected, UINT actual);

	UINT testCount, passed;
};

