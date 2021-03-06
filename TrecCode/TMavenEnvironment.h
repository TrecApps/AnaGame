#pragma once
#include <TEnvironment.h>
class TMavenEnvironment : public TEnvironment
{
public:

	TMavenEnvironment(TrecPointer<TFileShell> shell);
	~TMavenEnvironment();

	virtual TString GetType()override;

	virtual UINT SetUpEnv();

	virtual void Compile()override;
	virtual void Compile(TrecPointer<TFile> logFile)override;

	virtual void Log()override;

	virtual void Run()override;

	virtual void Run(TrecPointer<TFileShell> file)override;


	virtual UINT RunTask(TString& task);

	virtual TrecPointer<TObjectNode> GetBrowsingNode() override;

	virtual bool SupportsFileExt(const TString& ext) override;
	virtual void SupportsFileExt(TDataArray<TString>& ext) override;
};

