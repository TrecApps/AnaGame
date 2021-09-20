#pragma once
#include "TEnvironmentBuilder.h"
#include <Page.h>



/**
 * Class: TAnagameEnvironmentBuilder
 * Purpose: Provides Anagame with a built-in Environment Builder
 * 
 * SuperClass: TEnvironmentBuilder
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
	 * Method: TAnagameEnvironmentBuilder::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;


	/**
	 * Method: TAnagameEnvironmentBuilder::GetEnvironment
	 * Purpose: Retrieves an environment based off of the key provided
	 * Parameters: const TString& envType - the type of environment to collect
	 *				TrecPointer<TFileShell> dir - the Directory to launch the environment in
	 * Returns: TrecPointer<TEnvironment> - the environment requestd (null if the builder did not recognize the request
     * 
     * Attributes: override
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
     * 
     * Attributes: override
	 */
	virtual TString GetEnvironmentTypes(TrecPointer<TFileShell> rootDirectory) override;


	/**
	 * Method: TAnagameEnvironmentBuilder::GetEnvironmentTypes
	 * Purpose: Allows Anagame to probe each Environment Builder and assess which Builder can offer an environment object
	 * Parameters: TrecPointer<TFileShell> rootDirectory - the directory of the given project
	 * Returns: TString - semicolon seperated list of possible environments that this Builder could create
	 *
	 * Note: this is intended for the purposes of creating a new project
     * 
     * Attributes: override
	 */
	virtual TString GetEnvironmentTypes() override;

	/**
	 * Method: TAnagameEnvironmentBuilder::GetPageSupport
	 * Purpose: Retrieves Supported Pages based off of the page type
	 * Parameters: page_type - type of page to look at
	 *				TDataArray<TString>& types - holds list of Page Types supported by this Builder
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void GetPageSupport(page_type type, TDataArray<TString>& types) override;

	/**
	 * Method: TAnagameEnvironmentBuilder::GetPageSupport
	 * Purpose: Retrieves Supported Pages based off of the page type
	 * Parameters: const TString& fileType - file type to examine
	 *				TDataArray<TString>& types - holds list of Page Types supported by this Builder
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void GetPageSupport(const TString& fileType, TDataArray<TString>& types) override;

	/**
	 * Method: TAnagameEnvironmentBuilder::GetPage
	 * Purpose: Retrieves a page based off of type
	 * Parameters: const TString& type - the type of page to retrieve
	 * Returns: TrecPointer<Page> - the page requested, complete with a handler (if supported, null if otherwise)
	 * 
	 * Attributes: override
	 */
	virtual TrecPointer<Page> GetPage(const TString& type) override;
};

