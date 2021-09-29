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

TString TAnagameEnvironmentBuilder::GetType()
{
	return TString(L"TAnagameEnvironmentBuilder;") + TEnvironmentBuilder::GetType();
}

TrecPointer<TEnvironment> TAnagameEnvironmentBuilder::GetEnvironment(const TString& envType, TrecPointer<TFileShell> dir)
{
	if (!dir.Get() || !dir->IsDirectory())
		return TrecPointer<TEnvironment>();

	if (!envType.Compare(L"Maven"))
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TEnvironment, TMavenEnvironment>(dir);

	if (!envType.Compare(L"Gradle"))
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TEnvironment, TGradleEnvironment>(dir);

	if (!envType.Compare(L"AnaCode"))
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TEnvironment, TAnaGameCodeEnvironment>(dir);

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

void TAnagameEnvironmentBuilder::GetPageSupport(page_type type, TDataArray<TString>& types)
{
	types.RemoveAll();

	switch (type)
	{
	case page_type::pt_regular:
		types.push_back(L"source_code");
		types.push_back(L"base_arena");
		// types.push_back(L"base_model");
		return;
	case page_type::pt_ribbon:
		types.push_back(L"ribbon_source");
		types.push_back(L"ribbon_arena");
		//types.push_back(L"");
		return;
	case page_type::pt_helper:
		types.push_back(L"helper_arena_camera");
		types.push_back(L"helper_properties");
	}
}

void TAnagameEnvironmentBuilder::GetPageSupport(const TString& fileType, TDataArray<TString>& types)
{
	types.RemoveAll();
}

TrecPointer<Page> TAnagameEnvironmentBuilder::GetPage(const TString& type)
{
	if (!type.Compare(L"source_code"))
	{
		//TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>();
	}
	return TrecPointer<Page>();
}
