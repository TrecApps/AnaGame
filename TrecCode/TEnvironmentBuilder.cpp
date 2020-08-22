#include "TEnvironmentBuilder.h"
#include "TAnagameEnvironmentBuilder.h"

TMap<TEnvironmentBuilder> builders;

/**
 * Method: TEnvironmentBuilder::TEnvironmentBuilder
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New TEnvironmentBuilder Object
 */
TEnvironmentBuilder::TEnvironmentBuilder()
{
}

/**
 * Method: TEnvironmentBuilder::~TEnvironmentBuilder
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TEnvironmentBuilder::~TEnvironmentBuilder()
{
}


/**
 * Method: TEnvironmentBuilder::SubmitBuilder
 * Purpose: Allows third Party libraries to submit their own builders so that Anagame hass access to the Environments that they offer
 * Parameters: const TString& builderKey - the key to refer to the builder (recommended that it is the library name)
 *				TrecPointer<TEnvironmentBuilder> builder - the builder to supply
 * Returns: Whether the key was submitted or not
 */
bool TEnvironmentBuilder::SubmitBuilder(const TString& builderKey, TrecPointer<TEnvironmentBuilder> builder)
{
    if (builderKey.Compare(L"Anagame"))
    {
        RefreshDefault();
    }

    if(!builderKey.GetSize() || !builder.Get())
        return false;

    builders.addEntry(builderKey, builder);
}

/**
 * Method: TEnvironmentBuilder::RefreshDefault
 * Purpose: Makes sure that the Built-in Builder is available
 * Parameters: void
 * Returns: void
 */
void TEnvironmentBuilder::RefreshDefault()
{
    auto builder = builders.retrieveEntry(L"Anagame");

    if (!builder.Get())
    {
        SubmitBuilder(TString(L"Anagame"), TrecPointerKey::GetNewTrecPointerAlt<TEnvironmentBuilder, TAnagameEnvironmentBuilder>());
    }
}

/**
 * Method: TEnvironmentBuilder::GetAvailableEnvironments
 * Purpose: Allows Anagame to know what environment types are available to it
 * Parameters: TMap<TString>& env - holds list of available environments by Provider and Environemnt Type
 *				TrecPointer<TFileShell> dir - the directory to fish out
 * Returns: void
 *
 * Note: the 'dir' parameter MUST be initialized with a directory. Otherwise, the env parameter will be empty.
 *		In order to get data without a directory, call the version of this method tht only takes in the 'env' parameter
 */
void TEnvironmentBuilder::GetAvailableEnvironments(TMap<TString>& env, TrecPointer<TFileShell> dir)
{
    RefreshDefault();

    env.clear();

    if (!dir.Get() || !dir->IsDirectory())
        return;

    for (UINT Rust = 0; Rust < builders.count(); Rust++)
    {
        auto envEntry = builders.GetEntryAt(Rust);
        if (!envEntry.Get() || !envEntry->object.Get() || !envEntry->key.GetSize())
            continue;

        TrecPointer<TString> data = TrecPointerKey::GetNewTrecPointer<TString>(envEntry->object->GetEnvironmentTypes(dir));

        env.addEntry(envEntry->key, data);
    }
}

/**
 * Method: TEnvironmentBuilder::GetAvailableEnvironments
 * Purpose: Allows Anagame to know what environment types are available to it
 * Parameters: TMap<TString>& env - holds list of available environments by Provider and Environemnt Type
 * Returns: void
 */
void TEnvironmentBuilder::GetAvailableEnvironments(TMap<TString>& env)
{
    RefreshDefault();
    env.clear();
    for (UINT Rust = 0; Rust < builders.count(); Rust++)
    {
        auto envEntry = builders.GetEntryAt(Rust);
        if (!envEntry.Get() || !envEntry->object.Get() || !envEntry->key.GetSize())
            continue;

        TrecPointer<TString> data = TrecPointerKey::GetNewTrecPointer<TString>(envEntry->object->GetEnvironmentTypes());

        env.addEntry(envEntry->key, data);
    }
}
