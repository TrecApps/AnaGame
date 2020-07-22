#pragma once
#include "TString.h"
#include "TMap.h"
/**
 * File: Configuration
 * Purpose: Provides resources through which Anagame can be configured, whether through AnaGame Properties or System Variables
 */


/**
 * enum class config_source
 * Purpose: Influences the Configuration Manager on where to look for a given configuration
 */
typedef enum class config_source
{
    config_source_default,          // The default setting (which happens to be user, then system)
    config_source_system,           // Only looks at OS environment variables for the value
    config_source_public,           // Only looks at the Public configuration environment (accessible to all users)
    config_source_user,             // Only looks at the User configuration environment (accessible to the user)
    config_source_system_user,      // Looks at the system variables and then the user porperties
 
};


/**
 * Function: GetAnagameVariable
 * Purpose: Enables Code to retrieve a given property
 * Parameters: TString& porperty - the property to scan for
 *              config_source source - where to look for the property
 * Returns: TString - the value of the property (if the property is not found, the string will be empty)
 */
TString _TREC_LIB_DLL GetAnagameVariable(TString& property, config_source source = config_source::config_source_default);

/**
 * Function: SetAnagameVariable
 * Purpose: Allows code to set Anagame properties.
 * Parameters: TString& property - the property to set
 *              TString& value - the value to set it to
 *              config_source source - the source to apply the change to (Default: user)
 * Returns: bool - whether the change was made
 *
 * Note: This Function will only apply to once source at a time. Therefore, source specifications for multiple sources will
 *      case false to be returned and no change was made
 * Note: To protect all users on a given computer, any attempt to change the public source will require administrative approval. Once the
 *      change was made, Anagame will give up admin priveledges.
 * Note: Because changing a property can affect Anagame as a whole and some could cause security vulnerabilities, furture versions should 
 *      implement some check on the function to ensure that changes aren't made without the user's knowledge
 */
bool _TREC_LIB_DLL SetAnagameVariable(TString& property, TString& value, config_source source = config_source::config_source_user);

/**
 * class AnagamePropertySource
 * Purpose: Provide a means to oversee Property sources
 *
 * Note: this class will be internal to the TrecLib library as it's not needed outside of it. Property manipulation
 *      can be done through the exported methods
 */
class AnagamePropertySource
{
public:
    /**
     * Method: AnagamePropertySource::AnagamePropertySource
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New AnagamePropertySource Object
     */
    AnagamePropertySource();


    /**
     * Method: AnagamePropertySource::SetUp
     * Purpose: Sets up the path to the config file
     * Parameters: config_source source - the source this object is
     * Returns: void
     */
    void SetUp(config_source source);

    /**
     * Method: AnagamePropertySource::Persist
     * Purpose: Persists the properties to file
     * Parameters: void
     * Returns: void
     */
    void Persist();

    /**
     * Method: AnagamePropertySource::GetProperty
     * Purpose: Retrieves the property that it has picked up
     * Parameters: TString& property - the property to retrieve
     * Returns: TString - the value of the property (empty if the property does not exist)
     */
    TString GetProperty(TString& property);

    /**
     * Method: AnagamePropertySource::SetProperty
     * Purpose: Sets the property to use later
     * Parameters: TString& property - The property to set
     *              TString& value - the value to set the property there
     * Returns: New AnagamePropertySource Object
     */
    void SetProperty(TString& property, TString& value);
private:
    /**
     * Holds the list of properties
     */
    TMap<TString> properties;

    /**
     * The path of the file to persist the properties to and later read from
     */
    TString path;
};


/**
 * Function: PersistConfig
 * Purpose: ensures that User and Public Property sources are persisted Once Anagame shuts down
 * Parameters: void
 * Retruns: void
 * Called by: DLL Shutdown main function
 */
void PersistConfig();