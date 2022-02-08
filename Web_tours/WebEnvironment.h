#pragma once
#include <TEnvironment.h>
#include <TJavaScriptInterpretor.h>
#include <TAnascriptInterpretor.h>
#include <TWebWindow.h>
#include <TcJavaScriptInterpretor.h>

class WebEnvGenerator: public EnvironmentGenerator
{
public:
	WebEnvGenerator(TrecPointer<TProcess> i, TrecPointer<TWindow> w);

	virtual TrecPointer<TEnvironment> GetEnvironment(TrecPointer<TFileShell> shell) override;

private:
	/**
	 * The TInstance holding this environment
	 */
	TrecPointer<TProcess> instance;

	/**
	 * The Window this is running in
	 */
	TrecPointer<TWindow> window;
};


class WebEnvironment : public TEnvironment
{
	friend class WebEnvGenerator;
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
	WebEnvironment(TrecPointer<TConsoleHolder> prompt);


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

	/** 
	 * Method: WebEnvironment::GetWindow
	 * Purpose: Retrieves the main window
	 * Parameters: void
	 * Returns: TrecPointer<TWindow> - the window requested
	 */
	TrecPointer<TWindow> GetWindow();


	/**
	 * Method: WebEnvironment::GetInstance
	 * Purpose: Retrieves the instance
	 * Parameters: void
	 * Returns: TrecPointer<TInstance> - the instance requested
	 */
	TrecPointer<TProcess> GetInstance();
	/**
	 * Method: WebEnvironment::GetVariable
	 * Purpose: Retrieves the variable requested by the interpretor
	 * Parameters: TString& var - the name of the variable requested
	 *				bool& present - whether the variable was present or not (used to distinguish between 'null' and 'undefined')
	 * Returns: TrecPointer<TVariable> - the variable requested
	 */
	virtual TrecPointer<TVariable> GetVariable(TString& var, bool& present, env_var_type evtType = env_var_type::evt_any) override;

	/**
	 * Method: WebEnvironment::SaveEnv
	 * Purpose: Saves the Current Status of the Environment itself
	 * Parameters: void
	 * Returns: UINT - error code (0 for no error)
	 *
	 * Attributes: abstract
	 */
	virtual UINT SaveEnv() override;


	/**
	 * Method: WebEnvironment::AddResource
	 * Purpose: Adds a file Resource to the Environment
	 * Parameters: TrecPointer<TFileShell> fileResorce - the file to add
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void AddResource(TrecPointer<TFileShell> fileResource) override;

	/**
	 * Method: WebEnvironment::SetLoadFile
	 * Purpose: Allows Environment to load itself
	 * Parameters: TrecPointer<TFileShell> file - the file to load from
	 * Returns: TString - error information (blank means success)
	 *
	 * Attributes: abstract
	 */
	virtual TString SetLoadFile(TrecPointer<TFileShell> file) override;


	/**
	 * Method: WebEnvironment::GetProjectLayout
	 * Purpose: Reports the Layout of the Project
	 * Parameters: void
	 * Returns: TrecPointer<TObjectNode> nodes - the nodes that represent the layout of the Project
	 *
	 * Attributes: abstract
	 */
	virtual TrecPointer<TObjectNode> GetProjectLyout() override;

private:

	/*
	 * The Root JavaScript interpretor
	 */
	TrecSubPointer<TVariable, TcJavaScriptInterpretor> mainJavaScript;

	/**
	 * The Root Anascript interpretor
	 */
	TrecSubPointer<TVariable, TAnascriptInterpretor> mainAnaScript;

	/**
	 * The TInstance holding this environment
	 */
	TrecPointer<TProcess> instance;

	/**
	 * The Window this is running in
	 */
	TrecPointer<TWindow> window;

	/**
	 * Method: WebEnvironment::SetResources
	 * Purpose: Allows Generators to provide this environment with the instance and window objects
	 * Parameters: TrecPointer<TInstance> instance - the instance
	 *				TrecPointer<TWindow> window -  the window held
	 * Returns: void
	 * 
	 * Note: both parameters must have active objects. Otherwise, an exception will be thrown
	 */
	void SetResources(TrecPointer<TProcess> instance, TrecPointer<TWindow> window);
};

