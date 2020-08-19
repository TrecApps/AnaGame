#pragma once
#include "TEnvironmentBuilder.h"

/**
 * Class: TAnagameEnvironmentBuilder
 * Purpose: Provides Anagame with a built-in Environment Builder
 */
class TAnagameEnvironmentBuilder :
    public TEnvironmentBuilder
{
public:

	/**
	 * Method: TAnagameEnvironmentBuilder::TEnvironmentBuilder
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New TEnvironmentBuilder Object
	 */
    TAnagameEnvironmentBuilder();

	/**
	 * Method: TAnagameEnvironmentBuilder::~TEnvironmentBuilder
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
    virtual ~TAnagameEnvironmentBuilder();


	/**
	 * Method: TAnagameEnvironmentBuilder::GetEnvironment
	 * Purpose: Retrieves an environment based off of the key provided
	 * Parameters: const TString& envType - the type of environment to collect
	 *				TrecPointer<TFileShell> dir - the Directory to launch the environment in
	 * Returns: TrecPointer<TEnvironment> - the environment requestd (null if the builder did not recognize the request
	 */
	virtual TrecPointer<TEnvironment> GetEnvironment(const TString& envType, TrecPointer<TFileShell> dir) override;

	/**
	 * Method: TAnagameEnvironmentBuilder::GetEnvironmentTypes
	 * Purpose: Allows Anagame to probe each Environment Builder and assess which Builder can offer an environment object
	 *		that would recognize certain file Specs
	 * Parameters: TrecPointer<TFileShell> rootDirectory - the directory of the given project
	 * Returns: TString - semicolon seperated list of possible environments that could take up the project based off of possible project files found
	 *
	 * Note: this is intended for the purposes of importing an existing project.
	 */
	virtual TString GetEnvironmentTypes(TrecPointer<TFileShell> rootDirectory) override;


	/**
	 * Method: TAnagameEnvironmentBuilder::GetEnvironmentTypes
	 * Purpose: Allows Anagame to probe each Environment Builder and assess which Builder can offer an environment object
	 * Parameters: TrecPointer<TFileShell> rootDirectory - the directory of the given project
	 * Returns: TString - semicolon seperated list of possible environments that this Builder could create
	 *
	 * Note: this is intended for the purposes of creating a new project
	 */
	virtual TString GetEnvironmentTypes() override;
};
