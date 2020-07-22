#pragma once
#include "Tester.h"
class TFileTester :
	public Tester
{
public:

	TFileTester(const std::string& f1, const std::string& f2);

	virtual void RunTests(std::string& output);
	virtual std::string GetTestName();


	void Test1(std::string& output);
	void Test2(std::string& output);

	virtual void Before();
	virtual void After();

private:
	std::string file1, file2;

};

