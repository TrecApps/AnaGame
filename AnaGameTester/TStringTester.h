#pragma once
#include "Tester.h"
#include <TString.h>
class TStringTester :
	public Tester
{
	virtual void RunTests(std::string& output);
	virtual std::string GetTestName();

	virtual void Before();
	virtual void After();


	// The Tests themselves
	void TestFind(std::string& output);
	void TestInsert(std::string& output);
	void TestAppendChar(std::string& output);
	void TestFormet(std::string& output);
	void TestDelete(std::string& output);
	void TestLower(std::string& output);
	void TestUpper(std::string& output);
	void TestReverse(std::string& output);
	void TestTrim(std::string& output);
	void TestReplace(std::string& output);


protected:
	TString test1, trim, test2, set1, set2, append1, append2, test3;
};

