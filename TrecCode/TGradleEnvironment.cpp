#include "TGradleEnvironment.h"

TGradleEnvironment::TGradleEnvironment(TrecPointer<TFileShell> shell):TEnvironment(shell)
{
}

TString TGradleEnvironment::GetType()
{
	return TString(L"TGradleEnvironment;") + TEnvironment::GetType();
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

UINT TGradleEnvironment::SaveEnv()
{
	return 0;
}

TrecPointer<TObjectNode> TGradleEnvironment::GetProjectLyout()
{
	return TrecPointer<TObjectNode>();
}

void TGradleEnvironment::AddResource(TrecPointer<TFileShell> fileResource)
{
}

TString TGradleEnvironment::SetLoadFile(TrecPointer<TFileShell> file)
{
	return L"Not Implemented";
}
