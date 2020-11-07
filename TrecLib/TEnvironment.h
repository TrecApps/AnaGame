#pragma once
#include "TObject.h"
#include "TFile.h"
#include "TMap.h"
#include "TrecLib.h"
#include "TType.h"
#include "TDirectory.h"
#include "TObjectNode.h"
#include "TVariable.h"

class TControl;
class TPromptControl;



void GetAnagameProvidedEnvironmentList(TrecPointer<TFileShell> directory, TDataArray<TString>& environmentType);

/** 
 * Class: LangNames
 * Purpose: holds the name of a Programming language and the file extensions associated with them
 * 
 * SuperClass: TObject
 */
class _TREC_LIB_DLL LangNames : public TObject
{
public:
	TString language;
	TDataArray<TString> fileExtensions;
};

/**
 * Class: TEnvironment
 * Purpose: Provides information about the Project currently managed in the scope of an IDE Window
 * 
 * SuperClass: TObject
 *
 * Note: This class was originally in the TrecCode Library. However, it was moved to the Tap library as a means to 
 *  provide the Ide Window with a means of knowing what directory it is focusing on
 */
class _TREC_LIB_DLL TEnvironment :
	public TObject
{
public:

	/**
	 * Method: TEnvironment::GetUrl
	 * Purpose: Returns The URL of the Srting
	 * Parameters: void
	 * Returns: TString - the URL of the environment
	 */
	TString GetUrl();

	/**
	 * Method: TEnvironment::SetUrl
	 * Purpose: Sets the URL of the Environment
	 * Parameters: const TString& url
	 * Returns: void
	 */
	void SetUrl(const TString& url);

	/**
	 * Method: TEnvironment::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

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
	 * Method: TEnvironment::SetPrompt
	 * Purpose: Allows a prompt to be set after the construction
	 * Parameters: TrecSubPointer<TControl, TPromptControl> prompt - the Command Prompt to work with
	 * Returns: void
	 */
	void SetPrompt(TrecSubPointer<TControl, TPromptControl> prompt);


	/**
	 * Method: TEnvironment::GetPrompt
	 * Purpose: Returns the Current Prompt being used by the Environment
	 * Parameters: void
	 * Returns: TrecSubPointer<TControl, TPromptControl> - the Command Prompt to work with
	 */
	TrecSubPointer<TControl, TPromptControl> GetPrompt();


	/**
	 * Method: TEnvironment::Print
	 * Purpose: Allows external code to manually add something to print out
	 * Parameters: TString& input - the command to enter
	 * Returns: bool - whether a prompt was set or not
	 * 
	 * Attributes: virtual
	 */
	virtual bool Print(TString& input);

	/**
	 * Method: TEnvironment::PrintLine
	 * Purpose: Allows external code to manually add something to print out, adding an extra new line at the end
	 * Parameters: TString& input - the command to enter
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual bool PrintLine(TString& input);

	/**
	 * Method: TEnvironment::GetTaskList
	 * Purpose: Retrieves the list of tasks offered by the Environment
	 * Parameters: void
	 * Returns: TDataArray<TString> - list of tasks offered by the Environment
	 * 
	 * Attributes: virtual
	 */
	virtual TDataArray<TString> GetTaskList();

	/**
	 * Method: TEnvironment::SetUpEnv
	 * Purpose: Initializes the Environment
	 * Parameters: void
	 * Returns: UINT - error code (0 for success)
	 * 
	 * Attributes: abstract
	 */
	virtual UINT SetUpEnv() = 0;


	/**
	 * Method: TEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void Compile() = 0;

	/**
	 * Method: TEnvironment::Compile
	 * Purpose: Support for a Compile task
	 * Parameters: TrecPointer<TFile> logFile - the Log file to log errors to
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void Compile(TrecPointer<TFile> logFile) = 0;


	/**
	 * Method: TEnvironment::Log
	 * Purpose: Starts logging 
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void Log() = 0;


	/**
	 * Method: TEnvironment::Run
	 * Purpose: Support for the Run Task
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void Run() = 0;


	/**
	 * Method: TEnvironment::RunFile
	 * Purpose: Support for the Run Task
	 * Parameters: TrecPointer<TFileShell> shell - the file to run
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void Run(TrecPointer<TFileShell> shell) = 0;

	

	/**
	 * Method: TEnvironment::RunTask
	 * Purpose: Runs a specific task
	 * Parameters: TString& task - the task to run
	 * Returns: UINT - Error code (0 for success)
	 * 
	 * Attributes: abstract
	 */
	virtual UINT RunTask(TString& task) = 0;


	/**
	 * Method: TEnvironment::GetRootDirectory
	 * Purpose: Retrieves the Working Directory of the Environment
	 * Parameters: void
	 * Returns: TrecPointer<TFileShell> - object representing the working directory of the environment
	 */
	TrecPointer<TFileShell> GetRootDirectory();

	/**
	 * Method: TEnvironment::GetVariable
	 * Purpose: Retrieves the variable requested by the interpretor
	 * Parameters: TString& var - the name of the variable requested
	 *				bool& present - whether the variable was present or not (used to distinguish between 'null' and 'undefined')
	 * Returns: TrecPointer<TVariable> - the variable requested
	 */
	TrecPointer<TVariable> GetVariable(TString& var, bool& present);

	/**
	 * Method: TEnvironment::SetSelf
	 * Purpose: Sets up the reference to itself
	 * Parameters: TrecPointer<TEnvironment> self - reference to itself
	 * Returns: void
	 */
	void SetSelf(TrecPointer<TEnvironment> self);

	/**
	 * Method: TEnvironment::GetBrowsingNode
	 * Purpose: Retrieves the Node relative to the environment
	 * Parameters: void
	 * Returns: TrecPinter<TObjectMode> - the nodes relevant to the environment
	 * 
	 * Attributes: abstract
	 */
	virtual TrecPointer<TObjectNode> GetBrowsingNode() = 0;

	/**
	 * Method: TEnvironment::SupportsFileExt
	 * Purpose: Reports whether the Environment supports the file type proposed
	 * Parameters: const TString& ext - the extension to test
	 * Returns: bool - whether the environment supports the file type
	 * 
	 * Attributes: abstract
	 */
	virtual bool SupportsFileExt(const TString& ext) = 0;


	/**
	 * Method: TEnvironment::SupportsFileExt
	 * Purpose: Reports whether the Environment supports the file type proposed
	 * Parameters: TDataArray<TString>& ext - the list of extenstions to add
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void SupportsFileExt(TDataArray<TString>& ext) = 0;


	/**
	 * Method: TEnvironment::AddVariable
	 * Purpose: Adds a Variable to the environment
	 * Parameters: const TString& name - variable name
	 *				TrecPointer<TVariable> var - the variable to hold
	 * Returns: void
	 * 
	 */
	void AddVariable(const TString& name, TrecPointer<TVariable> var);

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

	/**
	 * Holds a collection of variables so that interpretors underneath them could have access to them
	 */
	TMap<TVariable> envVariables;

	/**
	 * Allows Environment to provide references to itself
	 */
	TrecPointerSoft<TEnvironment> self;

	/**
	 * The Base URL for the environment, for most, it will be the same as the root directory,
	 * For Web, it would be what the user typed into the search box and the base Website url that results
	 */
	TString url;
};

