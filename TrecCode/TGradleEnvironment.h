#pragma once
#include <TEnvironment.h>
class TGradleEnvironment :
	public TEnvironment
{
public:
	TGradleEnvironment(TrecPointer<TFileShell> shell);

	virtual TString GetType()override;

	~TGradleEnvironment();

	virtual UINT SetUpEnv()override;

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

