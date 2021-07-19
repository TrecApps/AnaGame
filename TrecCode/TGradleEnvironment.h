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


	/**
	 * Method: TEnvironment::AddResource
	 * Purpose: Adds a file Resource to the Environment
	 * Parameters: TrecPointer<TFileShell> fileResorce - the file to add
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void AddResource(TrecPointer<TFileShell> fileResource) override;


	/**
	 * Method: TEnvironment::SetLoadFile
	 * Purpose: Allows Environment to load itself
	 * Parameters: TrecPointer<TFileShell> file - the file to load from
	 * Returns: TString - error information (blank means success)
	 *
	 * Attributes: abstract
	 */
	virtual TString SetLoadFile(TrecPointer<TFileShell> file) override;
};

