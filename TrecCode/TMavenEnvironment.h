#pragma once
#include <TEnvironment.h>
class TMavenEnvironment : public TEnvironment
{
public:

	TMavenEnvironment(TrecPointer<TFileShell> shell);
	~TMavenEnvironment();

	virtual UINT SetUpEnv();

	virtual void Compile();
	virtual void Compile(TrecPointer<TFile> logFile);

	virtual void Log();

	virtual void Run();



	virtual UINT RunTask(TString& task);
};

