#include "TEnvironment.h"
#include "TMap.h"
#include "DirectoryInterface.h"
#include "TcInterpretor.h"
#include "TML_Reader_.h"

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
void TEnvironment::SetSelf(TrecPointer<TEnvironment> self_)
{
	if (self_.Get() != this) throw L"Error";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TEnvironment>(self_);
}

/**
 * Method: TEnvironment::AddVariable
 * Purpose: Adds a Variable to the environment
 * Parameters: const TString& name - variable name
 *				TrecPointer<TVariable> var - the variable to hold
 * Returns: void
 *
 */
void TEnvironment::AddVariable(const TString& name_, TrecPointer<TVariable> var)
{
	AG_THREAD_LOCK
	envVariables.addEntry(name_, var);
	RETURN_THREAD_UNLOCK;
}

/**
 * Method: TEnvironment::GetName
 * Purpose: Retrieves the name of the environment
 * Parameters: void
 * Returns: TString - the name derived
 */
TString TEnvironment::GetName()
{
	return name;
}

/**
 * Method: TEnvironment::UpdateProjectRepo
 * Purpose: Allows Environment Objects to update the Repository for Environment Projects, called by Objects
 * Parameters: TrecPointer<TFileShell> file - the file to save
 *				const TString& envSource - where the Environment can be found (in Anagame itself or a third party Library)
 *				const TString& envType - the actual type of environment used
 * Returns: void
 */
void TEnvironment::UpdateProjectRepo(TrecPointer<TFileShell> file, const TString& envSource, const TString& envType, const TString& name_)
{
	if (!file.Get() || file->IsDirectory())
		return;

	TString repoName = GetDirectoryWithSlash(CentralDirectories::cd_AppData) + L"AnaGame\\";

	ForgeDirectory(repoName);
	repoName.Append(L"Envronments.tml");

	TrecPointer<TFile> repoFile = TrecPointerKey::GetNewTrecPointer<TFile>(repoName, TFile::t_file_open_existing | TFile::t_file_read);

	TDataArray<EnvironmentEntry> envEntries;

	if (repoFile->IsOpen())
	{
		TrecPointer<Parser_> parser = TrecPointerKey::GetNewTrecPointerAlt<Parser_, EnvironmentEntryParser>();
		TML_Reader_ reader(repoFile, parser);
		int num = 0;
		if (reader.read(&num))
		{
			dynamic_cast<EnvironmentEntryParser*>(parser.Get())->GetEntries(envEntries);
		}

		repoFile->Close();
	}

	EnvironmentEntry curEntry;
	curEntry.filePath.Set(file->GetPath());
	curEntry.source.Set(envSource);
	curEntry.type.Set(envType);
	curEntry.name.Set(name_);

	bool doPush = true;
	for (UINT Rust = 0; Rust < envEntries.Size(); Rust++)
	{
		if (envEntries[Rust].IsEqual(curEntry))
		{
			doPush = false;
			break;
		}
	}

	if (doPush)
		envEntries.push_back(curEntry);

	repoFile->Open(repoName, TFile::t_file_create_always | TFile::t_file_write);

	repoFile->WriteString(L"->TML\n");
	repoFile->WriteString(L"-|Type: Environment_Repo\n");
	repoFile->WriteString(L"-|Version: 0.0.1\n");
	repoFile->WriteString(L"-/\n");

	for (UINT Rust = 0; Rust < envEntries.Size(); Rust++)
	{
		repoFile->WriteString(L"->Environment\n");
		repoFile->WriteString(TString(L"-|Source: ") + envEntries[Rust].source + L'\n');
		repoFile->WriteString(TString(L"-|Type: ") + envEntries[Rust].type + L'\n');
		repoFile->WriteString(TString(L"-|Path: ") + envEntries[Rust].filePath + L'\n');
		repoFile->WriteString(TString(L"-|Name: ") + envEntries[Rust].name + L'\n');
	}

	repoFile->Close();

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

void TEnvironment::AddEnvironment(TrecPointer<TEnvironment> env)
{
	if (!env.Get()) return;

	for (UINT Rust = 0; Rust < environments.Size(); Rust++)
	{
		if (env.Get() == environments[Rust].Get())
			return;
	}
	environments.push_back(env);
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
void TEnvironment::SetUrl(const TString& url_)
{
	this->url.Set(url_);
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
TEnvironment::TEnvironment(TrecPointer<TConsoleHolder> prompt)
{
	shellRunner = prompt;
}

/**
 * Method: TEnvironment::SetPrompt
 * Purpose: Allows a prompt to be set after the construction
 * Parameters: TrecPointer<TConsoleHolder> prompt - the Command Prompt to work with
 * Returns: void
 */
void TEnvironment::SetPrompt(TrecPointer<TConsoleHolder> prompt)
{
	shellRunner = prompt;
}


/**
 * Method: TEnvironment::GetPrompt
 * Purpose: Returns the Current Prompt being used by the Environment
 * Parameters: void
 * Returns:TrecPointer<TConsoleHolder> - the Command Prompt to work with
 */
TrecPointer<TConsoleHolder> TEnvironment::GetPrompt()
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

		if (fileName.Find(TString(L".snl")) == static_cast<int>(fileName.GetSize()) - 4 || fileName.Find(TString(L".vcxproj")) == static_cast<int>(fileName.GetSize()) - 8)
		{
			environmentType.push_back(TString(L"VisualStudio"));
			continue;
		}
	}
}

EnvironmentEntry::EnvironmentEntry()
{
}

EnvironmentEntry::EnvironmentEntry(const EnvironmentEntry& copy)
{
	filePath.Set(copy.filePath);
	source.Set(copy.source);
	type.Set(copy.type);
	name.Set(copy.name);
}

bool EnvironmentEntry::IsEqual(const EnvironmentEntry& ent)
{
	return !ent.filePath.CompareNoCase(filePath);
}

TString EnvironmentEntryParser::GetType()
{
	return TString(L"EnvironmentntryParser;") + Parser_::GetType();
}

EnvironmentEntryParser::EnvironmentEntryParser()
{
}

EnvironmentEntryParser::~EnvironmentEntryParser()
{
}

bool EnvironmentEntryParser::Obj(TString& v)
{
	UNREFERENCED_PARAMETER(v);

	if (entry.filePath.GetSize() && entry.source.GetSize() && entry.type.GetSize())
		entries.push_back(entry);

	entry.filePath.Empty();
	entry.source.Empty();
	entry.type.Empty();

	return true;
}

bool EnvironmentEntryParser::Attribute(TString& v, TString e)
{
	bool ret = false;
	if (!e.Compare(L"|Source"))
	{
		entry.source.Set(v);
		ret = true;
	}
	else if (!e.Compare(L"|Type"))
	{
		entry.type.Set(v);
		ret = true;
	}
	else if (!e.Compare(L"|Path"))
	{
		entry.filePath.Set(v);
		ret = true;
	}
	else if (!e.Compare(L"|Name"))
	{
		entry.name.Set(v);
		ret = true;
	}

	return ret;
}

bool EnvironmentEntryParser::Attribute(TrecPointer<TString> v, TString& e)
{
	if(!v.Get())
		return false;
	return Attribute(*v.Get(), e);
}

bool EnvironmentEntryParser::submitType(TString v)
{
	return !v.CompareNoCase(L"Environment_Repo");
}

bool EnvironmentEntryParser::submitEdition(TString v)
{
	return true;
}

bool EnvironmentEntryParser::goChild()
{
	return true;
}

void EnvironmentEntryParser::goParent()
{
}

void EnvironmentEntryParser::GetEntries(TDataArray<EnvironmentEntry>& entries_)
{
	TString v;
	Obj(v);

	for (UINT Rust = 0; Rust < this->entries.Size(); Rust++)
	{
		entries_.push_back(this->entries[Rust]);
	}
}

TConsoleHolder::TConsoleHolder()
{
	groupLevel = 0;
}

TConsoleHolder::~TConsoleHolder()
{
}

UINT TConsoleHolder::Group(bool collapsed)
{
	UNREFERENCED_PARAMETER(collapsed);

	tabs.AppendChar(L'\t');
	return ++groupLevel;
}

UINT TConsoleHolder::EndGroup()
{
	if (groupLevel)
	{
		groupLevel--;
		tabs.Delete(0);
	}
	return groupLevel;
}
