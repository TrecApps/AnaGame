#pragma once
#include "TrecLib.h"
#include "TString.h"
#include "TFile.h"

/**
 * class PathConfiguration
 * Purpose: Hold information about the path to a given command or executable. Meant to be a more structured
 *      means of finding the path of a given executable
 */
class PathConfiguration
{
    friend class PathConfiguration;
public:

    /**
     * Method: PathConfiguration::PathConfiguration
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New Path Configuration Object
     */
    PathConfiguration();

    /**
     * Method: PathConfiguration::PathConfiguration
     * Purpose: Copy Constructor
     * Parameters: PathConfiguration& pathConfiguration - the object to copy
     * Returns: New Path Configuration based off of an existing one
     */
    PathConfiguration(PathConfiguration& pathConfig);

    /**
     * Method: PathConfiguration::~PathConfiguration
     * Purpose: Destructor
     * Parameters: void
     * Returns: void
     */
    ~PathConfiguration();


    /**
     * Method: PathConfiguration::SetName
     * Purpose: Sets the name of the path Config
     * Parameters: TString& name - the name of the path config
     * Returns: void
     */
    void SetName(TString& name);

    /**
     * Method: PathConfiguration::GetName
     * Purpose: Retrieves the name of the path config so that the command can be used properly
     * Parameters: void
     * Returns: TString - the name of the path/command
     */
    TString GetName();


    /**
     * Method: PathConfiguration::SetCurrentPath
     * Purpose: Sets the current path to a path available to the path config object
     * Parameters: UINT index - the index of available paths to set
     * Returns: bool - true if the index was valid, false if the index was out of bounds
     */
    bool SetCurrentPath(UINT index);

    /**
     * Method: PathConfiguration::SetCurrentPath
     * Purpose: Sets the Current Path of the configuration
     * Parameters: TString& path -  the path to set
     * Returns: void
     *
     * Note: this is meant to be called while the object is being read from the file and should not be used during the regular Runtime of Anagame
     */
    void SetCurrentPath(TString& path);

    /**
     * Method: PathConfiguration::GetCurrentPath
     * Purpose: Retrieves the current path to use for the given command
     * Parameters: void
     * Returns: TString - the current path of the command
     */
    TString GetCurrentPath();


    /**
     * Method: PathConfiguration::AddToPath
     * Purpose: Adds a new possible path to the Path Configuration
     * Parameters: TString& path - the path to add
     * Returns: bool - whether it was a valid path on the system
     */
    bool AddToPath(TString& path);

    /**
     * Method: PathConfiguration::Persist
     * Purpose: Persists the contents of the Path Configuration to the provided file
     * Parameters: TFile& file - the file to write to
     * Returns: void
     */
    void Persist(TFile& file);

    /**
     * Method: PathConfiguration::GetPathAt
     * Purpose: Returns the path at the index
     * Parameters: UINT index - the idex of the pth to get
     * Returns: TString - the path at the location (empty if out of bounds)
     */
    TString GetPathAt(UINT index);

private:
    /**
     * the name of the path config
     */
    TString name;

    /**
     * the list of available paths
     */
    TDataArray<TString> paths;

    /**
     * The Current path to return
     */
    TString currentPath;
};


/**
 * Function:
 * Purpose: 
 * Parameters:
 * Returns:
 */


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
bool SubmitPossiblePath(TString& command, TString& path);

/**
 * Function: SetCurrentPath
 * Purpose: Sets the current path of the path config 
 * Parameters: TString& command - the command to set the current path on
 *               UINT number - the index in the command to set the current command to
 * Returns: UINT - Error code. 0 = Success, 1 = number was out of bounds, 2 = command did not exist
 */
UINT SetCurrentPath(TString& command, UINT number);

/**
 * Function: GetCurrentPath
 * Purpose: Retrieves the path of the command
 * Parameters: TString& command - the command to get the path from
 * Returns: TString - the current path of the command
 */
TString GetCurrentPath(TString& command);

/**
 * Function: PersistPathConfig
 * Purpose: Saves the path configuration to File
 * Parameters: void
 * Returns: void
 *
 * Note: Meant to be called when the DLL shuts down
 */
void PersistPathConfig();

/**
 * Function: GetPossiblePath
 * Purpose: Retrieves the possible path of the given command/path config
 * Parameters: TString& command - the command to get the path from
 *               UINT number - the index of the possible path
 * Returns: TString - the path at the given location (empty if out of bounds)
 */
TString GetPossiblePath(TString& command, UINT number);