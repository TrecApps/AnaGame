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

void TGradleEnvironment::Run(TrecPointer<TFileShell> file)
{
}

UINT TGradleEnvironment::RunTask(TString& task)
{
	return 0;
}

TrecPointer<TObjectNode> TGradleEnvironment::GetBrowsingNode()
{
	return TrecPointer<TObjectNode>();
}

bool TGradleEnvironment::SupportsFileExt(const TString& ext)
{
	return false;
}

void TGradleEnvironment::SupportsFileExt(TDataArray<TString>& ext)
{
}
