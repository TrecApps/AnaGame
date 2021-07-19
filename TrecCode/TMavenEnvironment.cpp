#include "TMavenEnvironment.h"

TMavenEnvironment::TMavenEnvironment(TrecPointer<TFileShell> shell): TEnvironment(shell)
{
}

TMavenEnvironment::~TMavenEnvironment()
{
}

TString TMavenEnvironment::GetType()
{
	return TString(L"TMavenEnvironment;") + TEnvironment::GetType();
}

UINT TMavenEnvironment::SetUpEnv()
{
	return 0;
}

void TMavenEnvironment::Compile()
{
}

void TMavenEnvironment::Compile(TrecPointer<TFile> logFile)
{
}

void TMavenEnvironment::Log()
{
}

void TMavenEnvironment::Run()
{
}

void TMavenEnvironment::Run(TrecPointer<TFileShell> file)
{
}

UINT TMavenEnvironment::RunTask(TString& task)
{
	return 0;
}

TrecPointer<TObjectNode> TMavenEnvironment::GetBrowsingNode()
{
	return TrecPointer<TObjectNode>();
}

bool TMavenEnvironment::SupportsFileExt(const TString& ext)
{
	return false;
}

void TMavenEnvironment::SupportsFileExt(TDataArray<TString>& ext)
{
}

UINT TMavenEnvironment::SaveEnv()
{
	return 0;
}

TrecPointer<TObjectNode> TMavenEnvironment::GetProjectLyout()
{
	return TrecPointer<TObjectNode>();
}

void TMavenEnvironment::AddResource(TrecPointer<TFileShell> fileResource)
{
}

TString TMavenEnvironment::SetLoadFile(TrecPointer<TFileShell> file)
{
	return L"Not Implemented";
}
