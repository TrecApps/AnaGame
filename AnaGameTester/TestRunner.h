#pragma once

#include "TStringTester.h"
#include "TFileTester.h"

class TestRunner
{
public:
	void DoTests(std::string& directory);

	void Draw(HWND window, PAINTSTRUCT structure);

private:
	std::string testResults;
};


void RunTests();
void DoDraw(HWND window, PAINTSTRUCT structure);

