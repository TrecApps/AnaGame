#include "Configuration.h"
#include "DirectoryInterface.h"

/**
 * The Property Source for all users
 */
static AnagamePropertySource propertySourcePublic;

/**
 * The property Source for the current user
 */
static AnagamePropertySource propertySourceUser;

/**
 * Function: Initialize
 * Purpose: Makes sure tht both Porperty Sources are ready for use
 * Prameters: void
 * Returns: void
 */
void Initialize()
{
	propertySourcePublic.SetUp(config_source::config_source_public);
	propertySourceUser.SetUp(config_source::config_source_user);
}


/**
 * Function: SetUserProperty
 * Purpose:  Sets the specified property at the User level
 * Prameters: void
 * Returns: void
 */
void SetUserProperty(TString& prop, TString& val)
{
	if (prop.GetSize() && val.GetSize())
		propertySourceUser.SetProperty(prop, val);
}

/**
 * Function: SetPublicProperty
 * Purpose:  Sets the specified property at the Public level
 * Prameters: TString& prop - the proerty to set
 *				 TString& val - the Value to set the property to
 * Returns: void
 */
void SetPublicProperty(TString& prop, TString& val)
{
	// To-Do: Find a way to ensure that an admin is doing this


	if (prop.GetSize() && val.GetSize())
		propertySourcePublic.SetProperty(prop, val);

}

/**
 * Function: SetOSProperty
 * Purpose: Sets the specified property at the OS level
 * Prameters: TString& prop - the proerty to set
 *				 TString& val - the Value to set the property to
 * Returns: void
 */
void SetOSProperty(TString& prop, TString& val)
{
	if (prop.GetSize() && val.GetSize())
	{
		prop.Replace(L'.', L'_');
		prop.SetUpper();
		SetEnvironmentVariableW(prop.GetConstantBuffer(), val.GetConstantBuffer());
	}
}

/**
 * Function: GetUserProperty
 * Purpose: Retrieves the property at the User level
 * Prameters: TString& prop - the property to retrieve
 * Returns: TString - The value of the User Property
 */
TString GetUserProperty(TString& prop)
{
	return propertySourceUser.GetProperty(prop);
}

/**
 * Function: GetPublicProperty
 * Purpose: Retrieves the property at the Public level
 * Prameters: TString& prop - the property to retrieve
 * Returns: TString - The value of the Public Property
 */
TString GetPublicProperty(TString& prop)
{
	return propertySourcePublic.GetProperty(prop);
}

/**
 * Function: GetOSPropery
 * Purpose: Retrieves the property at the OS level
 * Prameters: TString& prop - The environment variable to look for
 * Returns: TString - the Property value of the OS Environment variable
 *
 * Note: While regular properties allow '.' as seperators, Environment variables do not. As a Reault, '.'
 *		is converted to '_' before the scan. Because It is also a convention to use all caps, Anagame will first attempt
 *		the String as it (with the . to _ replacement) and then attempt to use all caps.
 */
TString GetOSPropery(TString& prop)
{
	prop.Replace(L'.', L'_');
	bool upper = false;

	UINT alloc = 100;

	WCHAR* buffer = nullptr;

	DWORD result = 0;

	bool keepTrying = true;


	TString ret;


	while (keepTrying)
	{
		buffer = new WCHAR[alloc];
		ZeroMemory(buffer, alloc * sizeof(WCHAR));
		result = GetEnvironmentVariableW(prop.GetConstantBuffer(), buffer, alloc - 1);

		if (result > alloc)
		{
			alloc = result + 1;
		}
		else if (!result || upper)
		{
			keepTrying = false;
		}
		else if (!result || !upper)
		{
			prop.SetUpper();
			upper = true;
		}
		else
		{
			ret.Set(buffer);
			keepTrying = false;
		}
		
		
		delete[] buffer;
		buffer = nullptr;
	}

	return ret;
}

/**
 * Method: AnagamePropertySource::AnagamePropertySource
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New AnagamePropertySource Object
 */
AnagamePropertySource::AnagamePropertySource()
{
}

/**
 * Method: AnagamePropertySource::SetUp
 * Purpose: Sets up the path to the config file
 * Parameters: config_source source - the source this object is
 * Returns: void
 */
void AnagamePropertySource::SetUp(config_source source)
{
	if (path.GetSize())
		return;

	if (source == config_source::config_source_public)
		path.Set(GetDirectoryWithSlash(CentralDirectories::cd_public));
	else path.Set(GetDirectoryWithSlash(CentralDirectories::cd_User));

	path.Append(L"Anagame\\Properties");

	ForgeDirectory(path);
	path.Append(L"\\anagame.properties");

	TFile readFile(path, TFile::t_file_read);

	if (readFile.IsOpen())
	{
		TString line;

		while (readFile.ReadString(line))
		{
			if (line.GetSize() && line[0] == L'#')
				continue;			// The whole line is a comment

			int hashLoc = line.Find(L'#', 0, false);
			
			TString actualLine;
			
			if (hashLoc == -1)
				actualLine.Set(line);
			else
				actualLine.Set(line.SubString(0, hashLoc));

			actualLine.Trim();

			int colonLoc = actualLine.Find(L'=');
			if (colonLoc == -1)
				continue;

			TString prop(actualLine.SubString(0, colonLoc));
			TrecPointer<TString> val = TrecPointerKey::GetNewTrecPointer<TString>(actualLine.SubString(colonLoc + 1));

			prop.Trim();
			val->Trim();

			if(val->GetSize() && prop.GetSize())
				properties.addEntry(prop, val);
		}



		readFile.Close();
	}
}

/**
 * Method: AnagamePropertySource::Persist
 * Purpose: Persists the properties to file
 * Parameters: void
 * Returns: void
 */
void AnagamePropertySource::Persist()
{
	TFile writeFile(path, TFile::t_file_write | TFile::t_file_create_always | TFile::t_file_truncate_existing);
	if (writeFile.IsOpen())
	{
		for (UINT Rust = 0; Rust < properties.count(); Rust++)
		{
			auto entry = properties.GetEntryAt(Rust);
			if (entry.Get() && entry->key.GetSize() && entry->object.Get() && entry->object->GetSize())
				writeFile.WriteString(entry->key + TString(L"=") + entry->object.Get() + L'\n');
		}

		writeFile.Close();
	}
}

/**
 * Method: AnagamePropertySource::GetProperty
 * Purpose: Retrieves the property that it has picked up
 * Parameters: TString& property - the property to retrieve
 * Returns: TString - the value of the property (empty if the property does not exist)
 */
TString AnagamePropertySource::GetProperty(TString& property)
{
	auto value = properties.retrieveEntry(property);
	if(!value.Get())
		return TString();
	return TString(value.Get());
}

/**
 * Method: AnagamePropertySource::SetProperty
 * Purpose: Sets the property to use later
 * Parameters: TString& property - The property to set
 *              TString& value - the value to set the property there
 * Returns: New AnagamePropertySource Object
 */
void AnagamePropertySource::SetProperty(TString& property, TString& value)
{
	TString trimValue(value.GetTrim());
	auto curValue = properties.retrieveEntry(property);
	if (curValue.Get())
		curValue->Set(trimValue);
	else if(trimValue.GetSize() && property.GetSize())
		properties.addEntry(property, TrecPointerKey::GetNewTrecPointer<TString>(trimValue));
}

/**
 * Function: GetAnagameVariable
 * Purpose: Enables Code to retrieve a given property
 * Parameters: TString& porperty - the property to scan for
 *              config_source source - where to look for the property
 * Returns: TString - the value of the property (if the property is not found, the string will be empty)
 */
TString _TREC_LIB_DLL GetAnagameVariable(TString& property, config_source source)
{
	TString ret;
	Initialize();

	switch (source)
	{
	case config_source::config_source_default:
		ret.Set(GetUserProperty(property));
		if (!ret.GetSize())
			ret.Set(GetOSPropery(property));
		break;
	case config_source::config_source_public:
		ret.Set(GetPublicProperty(property));
		break;
	case config_source::config_source_system:
		ret.Set(GetOSPropery(property));
		break;
	case config_source::config_source_system_user:
		ret.Set(GetOSPropery(property));
		if (!ret.GetSize())
			ret.Set(GetUserProperty(property));
		break;
	case config_source::config_source_user:
		ret.Set(GetUserProperty(property));
		break;

	}


	return ret;
}

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
bool _TREC_LIB_DLL SetAnagameVariable(TString& property, TString& value, config_source source)
{
	Initialize();

	switch (source)
	{
	case config_source::config_source_public:
		SetPublicProperty(property, value);
		return true;
	case config_source::config_source_system:
		SetOSProperty(property, value);
		return true;
	case config_source::config_source_user:
		SetUserProperty(property, value);
		return true;
	}

	return false;
}

/**
 * Function: PersistConfig
 * Purpose: ensures that User and Public Property sources are persisted Once Anagame shuts down
 * Parameters: void
 * Retruns: void
 * Called by: DLL Shutdown main function
 */
void PersistConfig()
{
	propertySourcePublic.Persist();
	propertySourceUser.Persist();
}
