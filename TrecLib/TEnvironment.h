#pragma once
#include "TObject.h"
#include "TFile.h"
#include "TMap.h"
#include "TrecLib.h"
#include "TDirectory.h"
#include "TObjectNode.h"
#include "TVariable.h"
#include "Parser_.h"
#include "TObjectNode.h"


typedef enum class env_var_type
{
	evt_any,
	evt_interpretor
};

class _TREC_LIB_DLL EnvironmentEntry
{
public:
	EnvironmentEntry();
	EnvironmentEntry(const EnvironmentEntry& copy);

	bool IsEqual(const EnvironmentEntry& ent);

	TString filePath, source, type, name;
};

class _TREC_LIB_DLL EnvironmentEntryParser : public Parser_
{
public:


	/**
	 * Method: EnvironmentntryParser::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/*
	* Method: EnvironmentntryParser::EnvironmentntryParser
	* Purpose: Constructor
	* Parameters: void
	* Returns: void
	*/
	EnvironmentEntryParser();
	/*
	* Method: EnvironmentntryParser::~EnvironmentntryParser
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~EnvironmentEntryParser();

	// for the initial object type
	/*
	 * Method: EnvironmentntryParser::Obj
	 * Purpose: Takes in a String that represents an Object title
	 * Parameters: TString* v - the name of a class
	 * Returns: bool - false for the base class
	 *
	 * Attributes: virtual
	 */
	virtual bool Obj(TString& v) override;
	// for the attribute name

	/*
	 * Method: EnvironmentntryParser::Attribute
	 * Purpose: Sets up attributes for the current object being analyzed
	 * Parameters: TString* v - the value to parse in an attribute
	 *			TString e - the attribute name
	 * Returns: bool - success result
	 *
	 * Attributes: virtual
	 */
	virtual bool Attribute(TString& v, TString e) override;


	/*
	* Method: EnvironmentntryParser
	* Purpose: Sets up attributes for the current object being analyzed
	* Parameters: TrecPointer<TString> v - the value to parse in an attribute
	*			TString& e - the attribute name
	* Returns: bool - success result
	 *
	 * Attributes: virtual
	*/
	virtual bool Attribute(TrecPointer<TString> v, TString& e) override;
	// for the attribute value (here it may be good that TStrings are used)


	/*
	 * Method: EnvironmentntryParser::submitType
	 * Purpose: For a parser to process the type it is parsing, incase the information is incompatible with its purpose
	 * Parameters: TString v - the Parse type to check
	 * Returns: bool - whether the parser type is the correct type
	 *
	 * Attributes: virtual
	 */
	virtual bool submitType(TString v) override;
	/**
	 * Method: EnvironmentntryParser::submitEdition
	 * Purpose: Version of the Parse type, to handle incompatabilities between version
	 * Parameters: TString v - the version string
	 * Returns: bool - whether or not a version is compatible
	 *
	 * Attributes: virtual
	 */
	virtual bool submitEdition(TString v) override;

	/**
	 * Method: EnvironmentntryParser::goChild
	 * Purpose: Objects submitted will be children of the current object
	 * Parameters: void
	 * Returns: bool
	 *
	 * Attributes: virtual
	 */
	virtual bool goChild() override;
	/**
	 * Method: EnvironmentntryParser::goParent
	 * Purpose: Go up a node in a parsed object tree
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	virtual void goParent() override;

	/**
	 * Method: EnvironmentEntryParser::GetEntries
	 * Purpose: Retrieves the entries generated
	 * Parameters: TDataArray<EnvironmentEntry>& entries - held by the caller
	 * Raturns: void
	 */
	void GetEntries(TDataArray<EnvironmentEntry>& entries);

	TDataArray<EnvironmentEntry> entries;

	EnvironmentEntry entry;
};


/**
 * Class: TConsoleHolder
 * Purpose: Provides an abstract way to print to the console, regardless of the object type that is acting like a console
 */
class _TREC_LIB_DLL TConsoleHolder
{
public:
	/**
	 * Method: TConsoleHolder::TConsoleHolder
	 * Purpose: default Constructor
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: virtual
	 */
	TConsoleHolder();
	/**
	 * Method: TConsoleHolder::~TConsoleHolder
	 * Purpose: virtualized destructor
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual ~TConsoleHolder();

	/**
	 * Method: TConsoleHolder::Warn
	 * Purpose: A warning message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void Warn(TrecPointer<TVariable> var) = 0;
	
	/**
	 * Method: TConsoleHolder::Error
	 * Purpose: An error message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Error(TrecPointer<TVariable> var) = 0;

	/**
	 * Method: TConsoleHolder::Info
	 * Purpose: An information message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Info(TrecPointer<TVariable> var) = 0;

	/**
	 * Method: TConsoleHolder::Log
	 * Purpose: A regular message is sent
	 * Parameters: TrecPointer<TVariable> var - the variable to output
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void Log(TrecPointer<TVariable> var) = 0;

	/**
	 * Method: TConsoleHolder::Group
	 * Purpose: Sets Up indentation
	 * Parameters: bool collapsed = false - whether any logging should be hidden in a drop-down
	 * Returns: UINT - the new group Indent
	 *
	 * Attributes: virtual
	 * 
	 * Note: collapse was provided with JavaScript's "console.groupCollapsed" method in mind. Not all consoles would support such a feature 
	 *		and thus ignore the parameter, but it is provided for implementations that do support this feature
	 */
	virtual UINT Group(bool collapsed = false);

	/**
	 * Method: TConsoleHolder::EndGroup
	 * Purpose: Ends the Group
	 * Parameters: void
	 * Returns: UINT - the new group Indent
	 * 
	 * Attributes: virtual
	 */
	virtual UINT EndGroup();


protected:
	UINT groupLevel;
	TString tabs;
};


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
	 * Method: TEnvironment::GetProjectLayout
	 * Purpose: Reports the Layout of the Project
	 * Parameters: void
	 * Returns: TrecPointer<TObjectNode> nodes - the nodes that represent the layout of the Project
	 * 
	 * Attributes: abstract
	 */
	virtual TrecPointer<TObjectNode> GetProjectLyout() = 0;

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
	 * Parameters: TrecPointer<TConsoleHolder> prompt - the Command Prompt to work with
	 * Returns: new Environment object
	 */
	TEnvironment(TrecPointer<TConsoleHolder> prompt);

	/**
	 * Method: TEnvironment::SetPrompt
	 * Purpose: Allows a prompt to be set after the construction
	 * Parameters: TrecPointer<TConsoleHolder> prompt - the Command Prompt to work with
	 * Returns: void
	 */
	void SetPrompt(TrecPointer<TConsoleHolder> prompt);


	/**
	 * Method: TEnvironment::GetPrompt
	 * Purpose: Returns the Current Prompt being used by the Environment
	 * Parameters: void
	 * Returns: TrecPointer<TConsoleHolder> - the Command Prompt to work with
	 */
	TrecPointer<TConsoleHolder> GetPrompt();


	/**
	 * Method: TEnvironment::Print
	 * Purpose: Allows external code to manually add something to print out
	 * Parameters: TString& input - the command to enter
	 * Returns: bool - whether a prompt was set or not
	 * 
	 * Attributes: virtual
	 */
	virtual bool Print(const TString& input);

	/**
	 * Method: TEnvironment::PrintLine
	 * Purpose: Allows external code to manually add something to print out, adding an extra new line at the end
	 * Parameters: TString& input - the command to enter
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual bool PrintLine(const TString& input);

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
	virtual TrecPointer<TVariable> GetVariable(TString& var, bool& present, env_var_type evtType = env_var_type::evt_any);

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

	/**
	 * Method: TEnvironment::SaveEnv
	 * Purpose: Saves the Current Status of the Environment itself
	 * Parameters: void
	 * Returns: UINT - error code (0 for no error)
	 * 
	 * Attributes: abstract
	 */
	virtual UINT SaveEnv() = 0;

	/**
	 * Method: TEnvironment::AddResource
	 * Purpose: Adds a file Resource to the Environment
	 * Parameters: TrecPointer<TFileShell> fileResorce - the file to add
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void AddResource(TrecPointer<TFileShell> fileResource) = 0;

	/**
	 * Method: TEnvironment::SetLoadFile
	 * Purpose: Allows Environment to load itself
	 * Parameters: TrecPointer<TFileShell> file - the file to load from
	 * Returns: TString - error information (blank means success)
	 * 
	 * Attributes: abstract
	 */
	virtual TString SetLoadFile(TrecPointer<TFileShell> file) = 0;

	/**
	 * Method: TEnvironment::GetName
	 * Purpose: Retrieves the name of the environment
	 * Parameters: void
	 * Returns: TString - the name derived
	 */
	TString GetName();



protected:

	/**
	 * Method: TEnvironment::UpdateProjectRepo
	 * Purpose: Allows Environment Objects to update the Repository for Environment Projects, called by Objects
	 * Parameters: TrecPointer<TFileShell> file - the file to save
	 *				const TString& envSource - where the Environment can be found (in Anagame itself or a third party Library)
	 *				const TString& envType - the actual type of environment used
	 *				const TString& name - the name of the environment
	 * Returns: void
	 */
	void UpdateProjectRepo(TrecPointer<TFileShell> file, const TString& envSource, const TString& envType, const TString& name);


	/**
	 * Method: TEnvironment::SetUpLanguageExtensionMapping
	 * Purpose: Sets up mapping to langugaes by extension
	 * Parameters: void
	 * Returns: void
	 */
	void SetUpLanguageExtensionMapping();

	/**
	 * 
	 */

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
	TrecPointer<TConsoleHolder> shellRunner;

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

	/**
	 * The Name of the Environment
	 */
	TString name;
};

