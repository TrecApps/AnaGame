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

	/**
	 * Method: TEnvironment::SaveEnv
	 * Purpose: Saves the Current Status of the Environment itself
	 * Parameters: void
	 * Returns: UINT - error code (0 for no error)
	 */
	virtual UINT SaveEnv() override;


	/**
	 * Method: TEnvironment::GetProjectLayout
	 * Purpose: Reports the Layout of the Project
	 * Parameters: void
	 * Returns: TrecPointer<TObjectNode> nodes - the nodes that represent the layout of the Project
	 *
	 * Attributes: abstract
	 */
	virtual TrecPointer<TObjectNode> GetProjectLyout() override;
};

