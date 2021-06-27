#include "TEnvironment.h"
#include "TMap.h"
#include "DirectoryInterface.h"
#include "TcInterpretor.h"

static bool languagesMapped = false;

// Holds list of Languages and their file extensions
static TDataArray<LangNames> languageList;


/**
 * Method: TEnvironment::GetRootDirectory
 * Purpose: Retrieves the Working Directory of the Environment
 * Parameters: void
 * Returns: TrecPointer<TFileShell> - object representing the working directory of the environment
 */
TrecPointer<TFileShell> TEnvironment::GetRootDirectory()
{
	AG_THREAD_LOCK
		auto ret = rootDirectory;
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TEnvironment::GetVariable
 * Purpose: Retrieves the variable requested by the interpretor
 * Parameters: TString& var - the name of the variable requested
 *				bool& present - whether the variable was present or not (used to distinguish between 'null' and 'undefined')
 * Returns: TrecPointer<TVariable> - the variable requested
 */
TrecPointer<TVariable> TEnvironment::GetVariable(TString& var, bool& present, env_var_type evtType)
{
	AG_THREAD_LOCK
	for (UINT Rust = 0; Rust < envVariables.count(); Rust++)
	{
		auto entry = envVariables.GetEntryAt(Rust);
		if (!entry.Get())
		{
			present = false;
			RETURN_THREAD_UNLOCK TrecPointer<TVariable>();
		}

		if (!entry->key.Compare(var))
		{
			present = true;
			ThreadRelease();
			if(evtType == env_var_type::evt_any || dynamic_cast<TcInterpretor*>(entry->object.Get()))
				return entry->object;
		}
	}
	present = false;
	RETURN_THREAD_UNLOCK TrecPointer<TVariable>();
}

/**
 * Method: TEnvironment::SetSelf
 * Purpose: Sets up the reference to itself
 * Parameters: TrecPointer<TEnvironment> self - reference to itself
 * Returns: void
 */
void TEnvironment::SetSelf(TrecPointer<TEnvironment> self)
{
	if (self.Get() != this) throw L"Error";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TEnvironment>(self);
}

/**
 * Method: TEnvironment::AddVariable
 * Purpose: Adds a Variable to the environment
 * Parameters: const TString& name - variable name
 *				TrecPointer<TVariable> var - the variable to hold
 * Returns: void
 *
 */
void TEnvironment::AddVariable(const TString& name, TrecPointer<TVariable> var)
{
	AG_THREAD_LOCK
	envVariables.addEntry(name, var);
	RETURN_THREAD_UNLOCK;
}


/**
 * Method: TEnvironment::SetUpLanguageExtensionMapping
 * Purpose: Sets up mapping to langugaes by extension
 * Parameters: void
 * Returns: void
 */
void TEnvironment::SetUpLanguageExtensionMapping()
{
	AG_THREAD_LOCK
	if (languagesMapped)
		RETURN_THREAD_UNLOCK;

	TString languageFolder = GetDirectoryWithSlash(CentralDirectories::cd_Executable) + TString(L"Languages");

	if (languageList.Size())
		RETURN_THREAD_UNLOCK;

	TFile languageLister;

	languageLister.Open(languageFolder + TString(L"\\languages.properties"), TFile::t_file_read);

	if (!languageLister.IsOpen())
	{
		//char errorBuf[100];
		//ex.GetErrorMessage((LPTSTR)errorBuf, 99);
		RETURN_THREAD_UNLOCK;
	}
	TString line;
	while (languageLister.ReadString(line))
	{
		line.Trim();
		TrecPointer<TDataArray<TString>> sep = line.split(TString(L":"));

		if (sep->Size() != 2)
			continue;
		LangNames ln;
		ln.language = sep->at(0);
		sep = sep->at(1).split(TString(L";"));

		for (UINT rust = 0; rust < sep->Size(); rust++)
		{
			if (!sep->at(rust).GetSize())
				continue;
			sep->at(rust).Trim();
			ln.fileExtensions.push_back(sep->at(rust));
		}
		languageList.push_back(ln);
	}

	languageLister.Close();
	RETURN_THREAD_UNLOCK;
}

TString retrieveLanguageByExtension(TString ext)
{

	for (UINT c = 0; c < languageList.Size(); c++)
	{
		for (UINT Rust = 0; Rust < languageList[c].fileExtensions.Size(); Rust++)
		{
			if (languageList[c].fileExtensions[Rust] == ext)
				return languageList[c].language;
		}
	}
	return TString();
}

/**
 * Method: TEnvironment::GetUrl
 * Purpose: Returns The URL of the Srting
 * Parameters: void
 * Returns: TString - the URL of the environment
 */
TString TEnvironment::GetUrl()
{
	AG_THREAD_LOCK
		TString ret(url);
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TEnvironment::SetUrl
 * Purpose: Sets the URL of the Environment
 * Parameters: const TString& url
 * Returns: void
 */
void TEnvironment::SetUrl(const TString& url)
{
	this->url.Set(url);
}


/**
 * Method: TEnvironment::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TEnvironment::GetType()
{
	return TString(L"TEnvironment;") + TObject::GetType();
}

/**
 * Method: TEnvironment::TEnvironment
 * Purpose: Constructor
 * Parameters: TrecPointer<TFileShell> shell - the Root Directory to focus on (working Directory)
 * Returns: New Environment object
 */

TEnvironment::TEnvironment(TrecPointer<TFileShell> shell)
{
	rootDirectory = shell;
}

/**
 * Method: TEnvironment::~TEnvironment
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TEnvironment::~TEnvironment()
{
}

/**
 * Method: TEnvironment::TEnvironment
 * Purpose: Constructor
 * Parameters: TrecSubPointer<TControl, TPromptControl> prompt - the Command Prompt to work with
 * Returns: new Environment object
 */
TEnvironment::TEnvironment(TrecSubPointer<TControl, TPromptControl> prompt)
{
	shellRunner = prompt;
}

/**
 * Method: TEnvironment::SetPrompt
 * Purpose: Allows a prompt to be set after the construction
 * Parameters: TrecSubPointer<TControl, TPromptControl> prompt - the Command Prompt to work with
 * Returns: void
 */
void TEnvironment::SetPrompt(TrecSubPointer<TControl, TPromptControl> prompt)
{
	shellRunner = prompt;
}


/**
 * Method: TEnvironment::GetPrompt
 * Purpose: Returns the Current Prompt being used by the Environment
 * Parameters: void
 * Returns: TrecSubPointer<TControl, TPromptControl> - the Command Prompt to work with
 */
TrecSubPointer<TControl, TPromptControl> TEnvironment::GetPrompt()
{
	return shellRunner;
}

/**
 * Method: TEnvironment::Print
 * Purpose: Allows external code to manually add something to print out
 * Parameters: TString& input - the command to enter
 * Returns: bool - whether a prompt was set or not
 */
bool TEnvironment::Print(const TString& input)
{
	return false;
}

/**
 * Method: TEnvironment::PrintLine
 * Purpose: Allows external code to manually add something to print out, adding an extra new line at the end
 * Parameters: TString& input - the command to enter
 * Returns: bool - whether a prompt was set or not
 */
bool TEnvironment::PrintLine(const TString& input)
{
	return false;
}




/**
 * Method: TEnvironment::GetTaskList
 * Purpose: Retrieves the list of tasks offered by the Environment
 * Parameters: void
 * Returns: TDataArray<TString> - list of tasks offered by the Environment
 */
TDataArray<TString> TEnvironment::GetTaskList()
{
	return taskList;
}

void GetAnagameProvidedEnvironmentList(TrecPointer<TFileShell> directory, TDataArray<TString>& environmentType)
{
	if (!directory.Get() || !directory->IsDirectory())
		return;

	auto files = dynamic_cast<TDirectory*>(directory.Get())->GetFileListing();

	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		if (!files[Rust].Get())
			continue;

		TString fileName = files[Rust]->GetName();

		if (!fileName.CompareNoCase(TString(L"build.gradle")))
		{
			environmentType.push_back(TString(L"Gradle"));
			continue;
		}

		if (!fileName.CompareNoCase(TString(L"pom.xml")))
		{
			environmentType.push_back(TString(L"Maven"));
			continue;
		}

		if (!fileName.CompareNoCase(TString(L"treccode.tml")))
		{
			environmentType.push_back(TString(L"TrecCode"));
			continue;
		}

		if (!fileName.CompareNoCase(TString(L"ag_vid.tml")))
		{
			environmentType.push_back(TString(L"AnagameVideo"));
			continue;
		}

		// file.sln
		// 012345678

		if (fileName.Find(TString(L".snl")) == fileName.GetSize() - 4 || fileName.Find(TString(L".vcxproj")) == fileName.GetSize() - 8)
		{
			environmentType.push_back(TString(L"VisualStudio"));
			continue;
		}
	}
}
