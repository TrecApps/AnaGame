#include "TGradleEnvironment.h"

TGradleEnvironment::TGradleEnvironment(TrecPointer<TFileShell> shell):TEnvironment(shell)
{
}

TGradleEnvironment::~TGradleEnvironment()
{
}

UINT TGradleEnvironment::SetUpEnv()
{
	return 0;
}

void TGradleEnvironment::Compile()
{
}

void TGradleEnvironment::Compile(TrecPointer<TFile> logFile)
{
}

void TGradleEnvironment::Log()
{
}

void TGradleEnvironment::Run()
{
}

UINT TGradleEnvironment::RunTask(TString& task)
{
	return 0;
}
