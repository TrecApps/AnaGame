#include "TAnagameEnvironmentBuilder.h"

#include "TMavenEnvironment.h"
#include "TGradleEnvironment.h"
#include "TAnaGameCodeEnvironment.h"

TAnagameEnvironmentBuilder::TAnagameEnvironmentBuilder()
{
}

TAnagameEnvironmentBuilder::~TAnagameEnvironmentBuilder()
{
}

TrecPointer<TEnvironment> TAnagameEnvironmentBuilder::GetEnvironment(const TString& envType, TrecPointer<TFileShell> dir)
{
	if (!dir.Get() || !dir->IsDirectory())
		return TrecPointer<TEnvironment>();

	if (!envType.Compare(L"Maven"))
		return TrecPointerKey::GetNewTrecPointerAlt<TEnvironment, TMavenEnvironment>(dir);

	if (!envType.Compare(L"Gradle"))
		return TrecPointerKey::GetNewTrecPointerAlt<TEnvironment, TGradleEnvironment>(dir);

	if (!envType.Compare(L"AnaCode"))
		return TrecPointerKey::GetNewTrecPointerAlt<TEnvironment, TAnaGameCodeEnvironment>(dir);

	return TrecPointer<TEnvironment>();
}

TString TAnagameEnvironmentBuilder::GetEnvironmentTypes(TrecPointer<TFileShell> rootDirectory)
{
	if (!rootDirectory.Get() || !rootDirectory->IsDirectory())
		return TString();
	TString ret(L" ");

	TDataArray<TrecPointer<TFileShell>> files;

	dynamic_cast<TDirectory*>(rootDirectory.Get())->GetFileListing(files);

	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		if (!files[Rust].Get())
			continue;

		auto name = files[Rust]->GetName();

		if (!name.Compare(L"pom.xml"))
		{
			ret.Append(L"Maven ");
			continue;
		}

		if (!name.Compare(L"build.gradle"))
		{
			ret.Append(L"Gradle ");
			continue;
		}

		if (!name.Compare(L"code_project.tml"))
			ret.Append(L"AnaCode ");

	}
	int useless;
	return ret.GetTrim().GetReplace(useless, L' ', L';');
}

TString TAnagameEnvironmentBuilder::GetEnvironmentTypes()
{
	return TString(L"Gradle;Maven;AnaCode");
}
