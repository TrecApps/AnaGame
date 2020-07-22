#include "PathConfiguration.h"
#include "TFileShell.h"
#include "DirectoryInterface.h"
/**
 * List of all the Path commands currently documented by Anagame
 */
TDataArray<PathConfiguration> paths;



/**
 * Method: PathConfiguration::PathConfiguration
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Path Configuration Object
 */
PathConfiguration::PathConfiguration()
{
}

/**
 * Method: PathConfiguration::PathConfiguration
 * Purpose: Copy Constructor
 * Parameters: PathConfiguration& pathConfiguration - the object to copy
 * Returns: New Path Configuration based off of an existing one
 */
PathConfiguration::PathConfiguration(PathConfiguration& pathConfig)
{
	name.Set(pathConfig.GetName());
	currentPath.Set(pathConfig.GetCurrentPath());

	paths = pathConfig.paths;
}

/**
 * Method: PathConfiguration::~PathConfiguration
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
PathConfiguration::~PathConfiguration()
{
}

/**
 * Method: PathConfiguration::SetName
 * Purpose: Sets the name of the path Config
 * Parameters: TString& name - the name of the path config
 * Returns: void
 */
void PathConfiguration::SetName(TString& name)
{
	name.Set(name);
}

/**
 * Method: PathConfiguration::GetName
 * Purpose: Retrieves the name of the path config so that the command can be used properly
 * Parameters: void
 * Returns: TString - the name of the path/command
 */
TString PathConfiguration::GetName()
{
	return name;
}

/**
 * Method: PathConfiguration::SetCurrentPath
 * Purpose: Sets the current path to a path available to the path config object
 * Parameters: UINT index - the index of available paths to set
 * Returns: bool - true if the index was valid, false if the index was out of bounds
 */
bool PathConfiguration::SetCurrentPath(UINT index)
{
	if (index < paths.Size() && paths[index].GetSize() && TFileShell::GetFileInfo(paths[index]).Get())
	{
		currentPath.Set(paths[index]);
		return true;
	}
	return false;
}

/**
 * Method: PathConfiguration::SetCurrentPath
 * Purpose: Sets the Current Path of the configuration
 * Parameters: TString& path -  the path to set
 * Returns: void
 *
 * Note: this is meant to be called while the object is being read from the file and should not be used during the regular Runtime of Anagame
 */
void PathConfiguration::SetCurrentPath(TString& path)
{
	currentPath.Set(path);
}

/**
 * Method: PathConfiguration::GetCurrentPath
 * Purpose: Retrieves the current path to use for the given command
 * Parameters: void
 * Returns: TString - the current path of the command
 */
TString PathConfiguration::GetCurrentPath()
{
	return currentPath;
}

/**
 * Method: PathConfiguration::AddToPath
 * Purpose: Adds a new possible path to the Path Configuration
 * Parameters: TString& path - the path to add
 * Returns: bool - whether it was a valid path on the system
 */
bool PathConfiguration::AddToPath(TString& path)
{
	if (!TFileShell::GetFileInfo(path).Get())
		return false;
	paths.push_back(path);
	return true;
}

/**
 * Method: PathConfiguration::Persist
 * Purpose: Persists the contents of the Path Configuration to the provided file
 * Parameters: TFile& file - the file to write to
 * Returns: void
 */
void PathConfiguration::Persist(TFile& file)
{
	if (!file.IsOpen() || !name.GetSize() || !currentPath.GetSize())
		return;

	file.WriteString(name + TString(L".currentPath=") + currentPath + L'\n');

	for (UINT Rust = 0; Rust < paths.Size(); Rust++)
	{
		if (paths[Rust].GetSize())
			file.WriteString(name + TString(L".possiblePath=") + currentPath + L'\n');
	}
}

/**
 * Method: PathConfiguration::GetPathAt
 * Purpose: Returns the path at the index
 * Parameters: UINT index - the idex of the pth to get
 * Returns: TString - the path at the location (empty if out of bounds)
 */
TString PathConfiguration::GetPathAt(UINT index)
{
	if (index < paths.Size())
		return paths[index];
	return TString();
}


/**
 * Function: InitializePathConfig
 * Purpose: Initializes the set of path configuration
 * Parameters: void
 * Returns: void
 */
void InitializePathConfig()
{
	// If the initialization is already complete, don't do it again
	if (paths.Size()) return;


	// Set up the read file
	TString path(GetDirectoryWithSlash(CentralDirectories::cd_User));

	path.Append(L"Anagame\\Properties");

	ForgeDirectory(path);
	path.Append(L"\\path.properties");

	TFile readFile(path, TFile::t_file_read);

	// make sure the file can be read
	if (readFile.IsOpen())
	{
		TString line;

		while (readFile.ReadString(line))
		{
			// Check to make sure we don't have comments 
			if (line.GetSize() && line[0] == L'#')
				continue;			// The whole line is a comment

			int hashLoc = line.Find(L'#', 0, false);

			TString actualLine;
			
			// Set the actual line depending on the location of the # symbol
			if (hashLoc == -1)
				actualLine.Set(line);
			else
				actualLine.Set(line.SubString(0, hashLoc));

			actualLine.Trim();

			// Prepare the split between the = 
			int colonLoc = actualLine.Find(L'=');
			if (colonLoc == -1)
				continue;

			// Get the property
			TString prop(actualLine.SubString(0, colonLoc));
			// Get the path toset to
			TString path(actualLine.SubString(colonLoc + 1));

			// Make sure that the path is a real path
			if (!TFileShell::GetFileInfo(path).Get())
				continue;

			int periodLoc = prop.Find(L'.');

			if (periodLoc == -1)
				continue;

			TString command(prop.SubString(0, periodLoc));
			TString attribute(prop.SubString(periodLoc + 1));

			path.Trim();
			command.Trim();
			attribute.Trim();

			if (!command.GetSize())
				continue;

			// Used in case the config does not currently exist. Assume false until it is found
			bool attSet = false;
			for (UINT Rust = 0; paths.Size(); Rust++)
			{
				if (!paths[Rust].GetName().Compare(command))
				{
					if (!attribute.Compare(L"currentPath"))
					{
						paths[Rust].SetCurrentPath(path);
					}
					else if (!attribute.Compare(L"possiblePath"))
					{
						paths[Rust].AddToPath(path);
					}
					attSet = true;
					break;
				}
			}

			// If the config did not exist, the create it
			if (!attSet)
			{
				PathConfiguration pathConfig;
				pathConfig.SetName(command);
				if (!attribute.Compare(L"currentPath"))
				{
					pathConfig.SetCurrentPath(path);
				}
				else if (!attribute.Compare(L"possiblePath"))
				{
					pathConfig.AddToPath(path);
				}
				paths.push_back(pathConfig);
			}
		}


		readFile.Close();
	}
}



/**
 * Function: SubmitPossiblePath
 * Purpose: Adds a possible path to the command/path config
 * Parameters:TString& command - the command to add the path to
 *             TString& path - the path to add
 * Returns: bool - whether the path was a valid path
 *
 * Note: If the command is brand new (meaning it was not already documented, the new object will have the current path set to command.
 *    If the command currently exists, the current path will not be set to command.
 */
bool SubmitPossiblePath(TString& command, TString& path)
{
	InitializePathConfig();
	if (!TFileShell::GetFileInfo(path).Get())
		return false;

	for (UINT Rust = 0; Rust < paths.Size(); Rust++)
	{
		if (!command.Compare(paths[Rust].GetName()))
		{
			return paths[Rust].AddToPath(path);
		}
	}

	PathConfiguration pathConfig;
	pathConfig.SetName(command);
	
	pathConfig.AddToPath(path);
	pathConfig.SetCurrentPath(0);
	
	paths.push_back(pathConfig);

	return true;
}

/**
 * Function: SetCurrentPath
 * Purpose: Sets the current path of the path config
 * Parameters: TString& command - the command to set the current path on
 *               UINT number - the index in the command to set the current command to
 * Returns: UINT - Error code. 0 = Success, 1 = number was out of bounds, 2 = command did not exist
 */
UINT SetCurrentPath(TString& command, UINT number)
{
	InitializePathConfig();
	for (UINT Rust = 0; Rust < paths.Size(); Rust++)
	{
		if (!command.Compare(paths[Rust].GetName()))
		{
			return paths[Rust].SetCurrentPath(number) ? 0: 1;
		}
	}
	return 2;
}

/**
 * Function: GetCurrentPath
 * Purpose: Retrieves the path of the command
 * Parameters: TString& command - the command to get the path from
 * Returns: TString - the current path of the command
 */
TString GetCurrentPath(TString& command)
{
	InitializePathConfig();
	for (UINT Rust = 0; Rust < paths.Size(); Rust++)
	{
		if (!command.Compare(paths[Rust].GetName()))
		{
			return paths[Rust].GetCurrentPath();
		}
	}
	return TString();
}

/**
 * Function: PersistPathConfig
 * Purpose: Saves the path configuration to File
 * Parameters: void
 * Returns: void
 *
 * Note: Meant to be called when the DLL shuts down
 */
void PersistPathConfig()
{
	TString path(GetDirectoryWithSlash(CentralDirectories::cd_User));

	path.Append(L"Anagame\\Properties");

	ForgeDirectory(path);
	path.Append(L"\\path.properties");

	TFile writeFile(path, TFile::t_file_write | TFile::t_file_create_always | TFile::t_file_truncate_existing);
	if (writeFile.IsOpen())
	{
		for (UINT Rust = 0; Rust < paths.Size(); Rust++)
		{
			paths[Rust].Persist(writeFile);

		}

		writeFile.Close();
	}
}

/**
 * Function: GetPossiblePath
 * Purpose: Retrieves the possible path of the given command/path config
 * Parameters: TString& command - the command to get the path from
 *               UINT number - the index of the possible path
 * Returns: TString - the path at the given location (empty if out of bounds)
 */
TString GetPossiblePath(TString& command, UINT number)
{
	InitializePathConfig();

	for (UINT Rust = 0; Rust < paths.Size(); Rust++)
	{
		if (!command.Compare(paths[Rust].GetName()))
		{
			return paths[Rust].GetPathAt(number);
		}
	}
	return TString();

}
