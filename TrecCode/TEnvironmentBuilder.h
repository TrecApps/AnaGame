#pragma once
#include <TObject.h>
#include "TrecCode.h"
#include <TEnvironment.h>
#include <Page.h>

/**
 * Enum Class: page_type
 * Purpose: TYpe of Pages that Anagame Supports at the IDE level
 */
typedef enum class page_type
{
	pt_ribbon,  // Page fits in with the Ribbon
	pt_regular,  // Regular page
	pt_helper
}page_type;

/**
 * Class: TEnvironmentBuilder
 * Purpose: Base class for various Environemnt Builders. This class is intended to allow for Anagame to offer it's own
 *		Environment type and for third part projects to extend Anagame with their own Builder and Environment Types
 * 
 * SuperClass: TObject
 */
class _TREC_CODE_DLL TEnvironmentBuilder : public TObject
{
public:

	/**
	 * Method: TEnvironmentBuilder::TEnvironmentBuilder
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New TEnvironmentBuilder Object
	 */
	TEnvironmentBuilder();


	/**
	 * Method: TEnvironmentBuilder::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
     * 
     * Attributes: override
	 */
	virtual TString GetType()override;

	/**
	 * Method: TEnvironmentBuilder::~TEnvironmentBuilder
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TEnvironmentBuilder();

	/**
	 * Method: TEnvironmentBuilder::GetEnvironment
	 * Purpose: Retrieves an environment based off of the key provided
	 * Parameters: const TString& envType - the type of environment to collect
	 *				TrecPointer<TFileShell> dir - the Directory to launch the environment in
	 * Returns: TrecPointer<TEnvironment> - the environment requestd (null if the builder did not recognize the request
     * 
     * Attributes: abstract
	 */
	virtual TrecPointer<TEnvironment> GetEnvironment(const TString& envType, TrecPointer<TFileShell> dir) = 0;

	/**
	 * Method: TEnvironmentBuilder::GetEnvironmentTypes
	 * Purpose: Allows Anagame to probe each Environment Builder and assess which Builder can offer an environment object
	 *		that would recognize certain file Specs 
	 * Parameters: TrecPointer<TFileShell> rootDirectory - the directory of the given project
	 * Returns: TString - semicolon seperated list of possible environments that could take up the project based off of possible project files found
	 * 
	 * Note: this is intended for the purposes of importing an existing project.
     * 
     * Attributes: abstract
	 */
	virtual TString GetEnvironmentTypes(TrecPointer<TFileShell> rootDirectory) = 0;


	/**
	 * Method: TEnvironmentBuilder::GetEnvironmentTypes
	 * Purpose: Allows Anagame to probe each Environment Builder and assess which Builder can offer an environment object
	 * Parameters: TrecPointer<TFileShell> rootDirectory - the directory of the given project
	 * Returns: TString - semicolon seperated list of possible environments that this Builder could create
	 *
	 * Note: this is intended for the purposes of creating a new project
     * 
     * Attributes: abstract
	 */
	virtual TString GetEnvironmentTypes() = 0;

	/**
	 * Method: TEnvironmentBuilder::SubmitBuilder
	 * Purpose: Allows third Party libraries to submit their own builders so that Anagame hass access to the Environments that they offer
	 * Parameters: const TString& builderKey - the key to refer to the builder (recommended that it is the library name)
	 *				TrecPointer<TEnvironmentBuilder> builder - the builder to supply
	 * Returns: Whether the key was submitted or not
     * 
     * Attributes: static
	 */
	static bool SubmitBuilder(const TString& builderKey, TrecPointer<TEnvironmentBuilder> builder);


	/**
	 * Method: TEnvironmentBuilder::RefreshDefault
	 * Purpose: Makes sure that the Built-in Builder is available
	 * Parameters: void
	 * Returns: void
     * 
     * Attributes: static
	 */
	static void RefreshDefault();

	/**
	 * Method: TEnvironmentBuilder::GetAvailableEnvironments
	 * Purpose: Allows Anagame to know what environment types are available to it
	 * Parameters: TMap<TString>& env - holds list of available environments by Provider and Environemnt Type
	 *				TrecPointer<TFileShell> dir - the directory to fish out
	 * Returns: void
	 * 
	 * Note: the 'dir' parameter MUST be initialized with a directory. Otherwise, the env parameter will be empty. 
	 *		In order to get data without a directory, call the version of this method tht only takes in the 'env' parameter
     * 
     * Attributes: static
	 */
	static void GetAvailableEnvironments(TMap<TString>& env, TrecPointer<TFileShell> dir);


	/**
	 * Method: TEnvironmentBuilder::GetAvailableEnvironments
	 * Purpose: Allows Anagame to know what environment types are available to it
	 * Parameters: TMap<TString>& env - holds list of available environments by Provider and Environemnt Type
	 * Returns: void
     * 
     * Attributes: static
	 */
	static void GetAvailableEnvironments(TMap<TString>& env);


	/**
	 * Method: TEnvironmentBuilder::GetEnvironment
	 * Purpose: Retrieves an environment based off of the key provided
	 * Parameters: const TString& envType - the type of environment to collect
	 *				const TString& builder -  the Builder Object in charge of getting the Environment
	 *				TrecPointer<TFileShell> dir - the Directory to launch the environment in
	 * Returns: TrecPointer<TEnvironment> - the environment requestd (null if the builder did not recognize the request
     * 
     * Attributes: static
	 */
	static TrecPointer<TEnvironment> GetEnvironment(const TString& envType, const TString& builder, TrecPointer<TFileShell> dir);


	/**
	 * Method: TAnagameEnvironmentBuilder::GetPageSupport
	 * Purpose: Retrieves Supported Pages based off of the page type
	 * Parameters: page_type - type of page to look at
	 *				TDataArray<TString>& types - holds list of Page Types supported by this Builder
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void GetPageSupport(page_type type, TDataArray<TString>& types) = 0;

	/**
	 * Method: TAnagameEnvironmentBuilder::GetPageSupport
	 * Purpose: Retrieves Supported Pages based off of the page type
	 * Parameters: const TString& fileType - file type to examine
	 *				TDataArray<TString>& types - holds list of Page Types supported by this Builder
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void GetPageSupport(const TString& fileType, TDataArray<TString>& types) = 0;

	/**
	 * Method: TAnagameEnvironmentBuilder::GetPage
	 * Purpose: Retrieves a page based off of type
	 * Parameters: const TString& type - the type of page to retrieve
	 * Returns: TrecPointer<Page> - the page requested, complete with a handler (if supported, null if otherwise)
	 *
	 * Attributes: abstract
	 */
	virtual TrecPointer<Page> GetPage(const TString& type) = 0;
};

