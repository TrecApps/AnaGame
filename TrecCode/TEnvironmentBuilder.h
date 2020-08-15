#pragma once
#include <TObject.h>
#include "TrecCode.h"
#include <TEnvironment.h>

/**
 * Class: TEnvironmentBuilder
 * Purpose: Base class for various Environemnt Builders. This class is intended to allow for Anagame to offer it's own
 *		Environment type and for third part projects to extend Anagame with their own Builder and Environment Types
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
	 */
	virtual TString GetEnvironmentTypes(TrecPointer<TFileShell> rootDirectory) = 0;


	/**
	 * Method: TEnvironmentBuilder::GetEnvironmentTypes
	 * Purpose: Allows Anagame to probe each Environment Builder and assess which Builder can offer an environment object
	 * Parameters: TrecPointer<TFileShell> rootDirectory - the directory of the given project
	 * Returns: TString - semicolon seperated list of possible environments that this Builder could create
	 *
	 * Note: this is intended for the purposes of creating a new project
	 */
	virtual TString GetEnvironmentTypes() = 0;

	/**
	 * Method: TEnvironmentBuilder::SubmitBuilder
	 * Purpose: Allows third Party libraries to submit their own builders so that Anagame hass access to the Environments that they offer
	 * Parameters: const TString& builderKey - the key to refer to the builder (recommended that it is the library name)
	 *				TrecPointer<TEnvironmentBuilder> builder - the builder to supply
	 * Returns: Whether the key was submitted or not
	 */

	static bool SubmitBuilder(const TString& builderKey, TrecPointer<TEnvironmentBuilder> builder);
};

