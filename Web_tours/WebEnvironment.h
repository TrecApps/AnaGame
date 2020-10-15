#pragma once
#include <TEnvironment.h>
#include <TPromptControl.h>
#include <TJavaScriptInterpretor.h>
#include <TAnascriptInterpretor.h>
#include <TWebWindow.h>

class WebEnvGenerator: public EnvironmentGenerator
{
	virtual TrecPointer<TEnvironment> GetEnvironment(TrecPointer<TFileShell> shell) override;
};


class WebEnvironment : public TEnvironment
{
public:


	/**
	 * Method: WebEnvironment::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: WebEnvironment::WebEnvironment
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TFileShell> shell - the Root Directory to focus on (working Directory)
	 * Returns: New Environment object
	 */
	WebEnvironment(TrecPointer<TFileShell> shell);

	/**
	 * Method: WebEnvironment::WebEnvironment
	 * Purpose: Constructor
	 * Parameters: TrecSubPointer<TControl, TPromptControl> prompt - the Command Prompt to work with
	 * Returns: new Environment object
	 */
	WebEnvironment(TrecSubPointer<TControl, TPromptControl> prompt);


	/**
	 * Method: WebEnvironment::SetUpEnv
	 * Purpose: Initializes the Environment
	 * Parameters: void
	 * Returns: UINT - error code (0 for success)
	 *
	 * Attributes: abstract
	 */
	virtual UINT SetUpEnv() override;


	/**
	 * Method: WebEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Compile() override;

	/**
	 * Method: WebEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: TrecPointer<TFile> logFile - the Log file to log errors to
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Compile(TrecPointer<TFile> logFile) override;


	/**
	 * Method: WebEnvironment::Log
	 * Purpose: Starts logging
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Log() override;


	/**
	 * Method: WebEnvironment::Run
	 * Purpose: Support for the Run Task
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Run() override;


	/**
	 * Method: WebEnvironment::RunFile
	 * Purpose: Support for the Run Task
	 * Parameters: TrecPointer<TFileShell> shell - the file to run
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Run(TrecPointer<TFileShell> shell) override;

	/**
	 * Method: WebEnvironment::RunTask
	 * Purpose: Runs a specific task
	 * Parameters: TString& task - the task to run
	 * Returns: UINT - Error code (0 for success)
	 *
	 * Attributes: abstract
	 */
	virtual UINT RunTask(TString& task) override;

	/**
	 * Method: WebEnvironment::GetBrowsingNode
	 * Purpose: Retrieves the Node relative to the environment
	 * Parameters: void
	 * Returns: TrecPinter<TObjectMode> - the nodes relevant to the environment
	 *
	 * Attributes: abstract
	 */
	virtual TrecPointer<TObjectNode> GetBrowsingNode() override;

	/**
	 * Method: WebEnvironment::SupportsFileExt
	 * Purpose: Reports whether the Environment supports the file type proposed
	 * Parameters: const TString& ext - the extension to test
	 * Returns: bool - whether the environment supports the file type
	 *
	 * Attributes: abstract
	 */
	virtual bool SupportsFileExt(const TString& ext) override;


	/**
	 * Method: WebEnvironment::SupportsFileExt
	 * Purpose: Reports whether the Environment supports the file type proposed
	 * Parameters: TDataArray<TString>& ext - the list of extenstions to add
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void SupportsFileExt(TDataArray<TString>& ext) override;

private:

	/*
	 * The Root JavaScript interpretor
	 */
	TrecSubPointer<TVariable, TJavaScriptInterpretor> mainJavaScript;

	/**
	 * The Root Anascript interpretor
	 */
	TrecSubPointer<TVariable, TAnascriptInterpretor> mainAnaScript;

};

