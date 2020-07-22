#pragma once
#include <TEnvironment.h>
class TGradleEnvironment :
	public TEnvironment
{
public:
	TGradleEnvironment(TrecPointer<TFileShell> shell);

	~TGradleEnvironment();

	virtual UINT SetUpEnv();

	virtual void Compile();
	virtual void Compile(TrecPointer<TFile> logFile);

	virtual void Log();

	virtual void Run();



	virtual UINT RunTask(TString& task);
};

