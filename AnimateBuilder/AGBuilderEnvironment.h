#pragma once
#include <TWindow.h>
class AGBuilderEnvironment :
    public TPageEnvironment
{
protected:

	TrecPointer<PageHandlerBuilder> codePageBuilder;
protected:


	/**
	 * Method: TPageEnvironment::GetPageAndHandler
	 * Purpose: Retrieves the Pages by the Specified name
	 * Parameters: const TString& name, TrecPointer<TPage> page, TrecPointer<TPage::EventHandler> handler
	 * Returns: void
	 *
	 * Note: If successful, the page and handler params should be assigned once the method returns
	 */
	virtual void GetPageAndHandler_(handler_type hType, const TString& name, TrecPointer<PageHandlerBuilder>& builder) override;

	/**
	 * Method: TPageEnvironment::GetPageList
	 * Purpose: Retrieves the available Page Names by extension
	 * Parameters: const TString& ext, TDataArray<TString>& extensions
	 * Returns: void
	 *
	 * Note: if ext is an empty String, then return all that the Environment has to offer
	 */
	virtual void GetPageList_(handler_type hType, const TString& ext, TDataArray<TString>& extensions) override;



public: // Required Method from TEnvironment

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
	 * Method: TEnvironment::SetUpEnv
	 * Purpose: Initializes the Environment
	 * Parameters: void
	 * Returns: UINT - error code (0 for success)
	 *
	 * Attributes: abstract
	 */
	virtual UINT SetUpEnv() override;


	/**
	 * Method: TEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Compile() override;

	/**
	 * Method: TEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: TrecPointer<TFile> logFile - the Log file to log errors to
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Compile(TrecPointer<TFile> logFile) override;


	/**
	 * Method: TEnvironment::Log
	 * Purpose: Starts logging
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Log() override;


	/**
	 * Method: TEnvironment::Run
	 * Purpose: Support for the Run Task
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Run() override;


	/**
	 * Method: TEnvironment::RunFile
	 * Purpose: Support for the Run Task
	 * Parameters: TrecPointer<TFileShell> shell - the file to run
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Run(TrecPointer<TFileShell> shell) override;



	/**
	 * Method: TEnvironment::RunTask
	 * Purpose: Runs a specific task
	 * Parameters: TString& task - the task to run
	 * Returns: UINT - Error code (0 for success)
	 *
	 * Attributes: abstract
	 */
	virtual UINT RunTask(TString& task) override;

	/**
	 * Method: TEnvironment::GetBrowsingNode
	 * Purpose: Retrieves the Node relative to the environment
	 * Parameters: void
	 * Returns: TrecPinter<TObjectMode> - the nodes relevant to the environment
	 *
	 * Attributes: abstract
	 */
	virtual TrecPointer<TObjectNode> GetBrowsingNode() override;

	/**
	 * Method: TEnvironment::SupportsFileExt
	 * Purpose: Reports whether the Environment supports the file type proposed
	 * Parameters: const TString& ext - the extension to test
	 * Returns: bool - whether the environment supports the file type
	 *
	 * Attributes: abstract
	 */
	virtual bool SupportsFileExt(const TString& ext) override;


	/**
	 * Method: TEnvironment::SupportsFileExt
	 * Purpose: Reports whether the Environment supports the file type proposed
	 * Parameters: TDataArray<TString>& ext - the list of extenstions to add
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void SupportsFileExt(TDataArray<TString>& ext) override;

	/**
	 * Method: TEnvironment::SaveEnv
	 * Purpose: Saves the Current Status of the Environment itself
	 * Parameters: void
	 * Returns: UINT - error code (0 for no error)
	 *
	 * Attributes: abstract
	 */
	virtual UINT SaveEnv() override;

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

	// Constructor

	AGBuilderEnvironment(TrecPointer<TFileShell> shell);
};

