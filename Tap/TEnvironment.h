#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TMap.h>
#include "Tap_dll.h"
#include <TType.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <TPromptControl.h>




void GetAnagameProvidedEnvironmentList(TrecPointer<TFileShell> directory, TDataArray<TString>& environmentType);

/** struct LangNames
 * Purpose: holds the name of a Programming language and the file extensions associated with them
 */
class _TAP_DLL LangNames : public TObject
{
public:
	TString language;
	TDataArray<TString> fileExtensions;
};

/**
 * Class: TEnvironment
 * Purpose: Provides information about the Project currently managed in the scope of an IDE Window
 *
 * Note: This class was originally in the TrecCode Library. However, it was moved to the Tap library as a means to 
 *  provide the Ide Window with a means of knowing what directory it is focusing on
 */
class _TAP_DLL TEnvironment :
	public TObject
{
public:

	/**
	 * Method: TEnvironment::TEnvironment
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TFileShell> shell - the Root Directory to focus on (working Directory)
	 * Returns: New Environment object
	 */
	TEnvironment(TrecPointer<TFileShell> shell);

	/**
	 * Method: TEnvironment::~TEnvironment
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TEnvironment();


	/**
	 * Method: TEnvironment::TEnvironment
	 * Purpose: Constructor
	 * Parameters: TrecSubPointer<TControl, TPromptControl> prompt - the Command Prompt to work with
	 * Returns: new Environment object
	 */
	TEnvironment(TrecSubPointer<TControl, TPromptControl> prompt);

	/**
	 * Method: TEnvironment::GetTaskList
	 * Purpose: Retrieves the list of tasks offered by the Environment
	 * Parameters: void
	 * Returns: TDataArray<TString> - list of tasks offered by the Environment
	 */
	virtual TDataArray<TString> GetTaskList();

	/**
	 * Method: TEnvironment::SetUpEnv
	 * Purpose: Initializes the Environment
	 * Parameters: void
	 * Returns: UINT - error code (0 for success)
	 */
	virtual UINT SetUpEnv() = 0;


	/**
	 * Method: TEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: void
	 * Returns: void
	 */
	virtual void Compile() = 0;

	/**
	 * Method: TEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: TrecPointer<TFile> logFile - the Log file to log errors to
	 * Returns: void
	 */
	virtual void Compile(TrecPointer<TFile> logFile) = 0;


	/**
	 * Method: TEnvironment::Log
	 * Purpose: Starts logging 
	 * Parameters: void
	 * Returns: void
	 */
	virtual void Log() = 0;


	/**
	 * Method: TEnvironment::Run
	 * Purpose: Support for the Run Task
	 * Parameters: void
	 * Returns: void
	 */
	virtual void Run() = 0;

	

	/**
	 * Method: TEnvironment::RunTask
	 * Purpose: Runs a specific task
	 * Parameters: TString& task - the task to run
	 * Returns: UINT - Error code (0 for success)
	 */
	virtual UINT RunTask(TString& task) = 0;


	/**
	 * Method: TEnvironment::GetRootDirectory
	 * Purpose: Retrieves the Working Directory of the Environment
	 * Parameters: void
	 * Returns: TrecPointer<TFileShell> - object representing the working directory of the environment
	 */
	TrecPointer<TFileShell> GetRootDirectory();

protected:

	/**
	 * Method: TEnvironment::SetUpLanguageExtensionMapping
	 * Purpose: Sets up mapping to langugaes by extension
	 * Parameters: void
	 * Returns: void
	 */
	void SetUpLanguageExtensionMapping();

	/**
	 * the Working directory of the Environment
	 */
	TrecPointer<TFileShell> rootDirectory;

	/**
	 * List of tasks offered by the Envirnment
	 */
	TDataArray<TString> taskList;

	/**
	 * The Command Prompt to work with (send shell commands to)
	 */
	TrecSubPointer<TControl, TPromptControl> shellRunner;
};

