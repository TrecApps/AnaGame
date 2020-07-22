#include "Tester.h"

Tester::Tester()
{
	testCount = passed = 0;
}

void Tester::Run(std::string& output)
{
	output += "\n\n--------" + GetTestName() + " --------\n\n";

	Before();

	RunTests(output);

	output += "\n\nTotalTests: " + std::to_string(testCount) + "\nPassed Tests: " + std::to_string(passed)
		+ "\nFailed Tests: " + std::to_string(testCount - passed);
}

void Tester::Before()
{
}

void Tester::After()
{
}

void Tester::ExpectTrue(std::string& output, std::string& testName, bool value)
{
	testCount++;
	
	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	if (value) // Assume passed
	{
		output += "PASSED!"; passed++;
	}
	else
	{
		output += "FAILED! Value reported was false!";
	}

	output += "\n";
}

void Tester::ExpectEquals(std::string& output, std::string& testName, const char* value1, const char* value2)
{
	testCount++;

	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	if (!strcmp(value1, value2)) // Assume passed
	{
		output += "PASSED!"; passed++;
	}
	else
	{
		output += "FAILED! Expected string \"" + std::string(value1) + "\" to equal \"" + std::string(value2) + "\"";
	}

	output += "\n";
}

void Tester::ExpectEquals(std::string& output, std::string& testName, const WCHAR* value1, const WCHAR* value2)
{
	testCount++;

	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	UINT wSize1 = lstrlenW(value1), wSize2 = lstrlenW(value2);


	char* cValue1 = new char[wSize1 + 2];
	char* cValue2 = new char[wSize2 + 2];

	size_t con1 = 0;
	wcstombs_s(&con1, cValue1, lstrlenW(value1) + 1, value1, lstrlenW(value1) + 1);
	size_t con2 = 0;
	wcstombs_s(&con2, cValue2, lstrlenW(value2) + 1, value2, lstrlenW(value2) + 1);

	cValue1[con1] = '\0';
	cValue2[con2] = '\0';

	if (!wcscmp(value1, value2)) // Assume passed
	{
		output += "PASSED!"; passed++;
	}
	else
	{
		output += "FAILED! Expected string \"" + std::string(cValue1) + "\" to equal \"" + std::string(cValue2) + "\"";
	}

	delete[] cValue1;
	delete[] cValue2;

	output += "\n";
}

void Tester::ExpectEquals(std::string& output, std::string& testName, char ch1, char ch2)
{
	testCount++;

	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	if (ch1 == ch2) // Assume passed
	{
		output += "PASSED!";
		passed++;
	}
	else
	{
		output += std::string("FAILED! Expected string \"") + ch1 + std::string("\" to equal \"") + ch2 + std::string("\"");
	}

	output += "\n";
}

void Tester::ExpectEquals(std::string& output, std::string& testName, WCHAR wch1, WCHAR wch2)
{
	testCount++;

	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	if (wch1 == wch2) // Assume passed
	{
		output += "PASSED!"; passed++;
	}
	else
	{
		output += "FAILED! Expected string \"" + std::to_string(wch1) + "\" to equal \"" + std::to_string(wch2) + "\"";
	}

	output += "\n";
}

void Tester::ExpectEquals(std::string& output, std::string& testName, long long expected, long long actual)
{
	testCount++;

	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	if (expected == actual) // Assume passed
	{
		output += "PASSED!"; passed++;
	}
	else
	{
		output += "FAILED! Expected int \"" + std::to_string(expected) + "\" to equal \"" + std::to_string(actual) + "\"";
	}

	output += "\n";
}

void Tester::ExpectEquals(std::string& output, std::string& testName, double expected, double actual)
{
	testCount++;

	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	if (expected == actual) // Assume passed
	{
		output += "PASSED!"; passed++;
	}
	else
	{
		output += "FAILED! Expected double \"" + std::to_string(expected) + "\" to equal \"" + std::to_string(actual) + "\"";
	}

	output += "\n";
}

void Tester::ExpectEquals(std::string& output, std::string& testName, UINT expected, UINT actual)
{
	testCount++;

	output += std::string("Test ") + std::to_string(testCount) + " \"" + testName + "\" ";

	if (expected == actual) // Assume passed
	{
		output += "PASSED!"; passed++;
	}
	else
	{
		output += "FAILED! Expected int \"" + std::to_string(expected) + "\" to equal \"" + std::to_string(actual) + "\"";
	}

	output += "\n";
}
