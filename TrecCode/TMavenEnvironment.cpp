#include "TMavenEnvironment.h"

TMavenEnvironment::TMavenEnvironment(TrecPointer<TFileShell> shell): TEnvironment(shell)
{
}

TMavenEnvironment::~TMavenEnvironment()
{
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
