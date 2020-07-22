#include "TAnaGameCodeEnvironment.h"

TAnaGameCodeEnvironment::TAnaGameCodeEnvironment(TrecPointer<TFileShell> shell): TEnvironment(shell)
{
}

void TAnaGameCodeEnvironment::PreProcessSingleFile(TrecPointer<TFile> file)
{
	//if (!file.Get() || !file->IsOpen())
	//	return;

	//TString fileExt(file->GetFileExtension());

	//TrecPointer<TLanguage> lang = languages.retrieveEntry(fileExt);

	//if (!lang.Get())
	//{
	//	SetUpLanguageExtensionMapping();

	//	TString langName(retrieveLanguageByExtension(fileExt));
	//	if (!langName.GetSize())
	//		return;

	//	auto language = TLanguage::getLanguage(langName);
	//	if (!language.Get())
	//		return;

	//	for (UINT C = 0; C < languageList.Size(); C++)
	//	{
	//		if (languageList[C].language.Compare(langName))
	//			continue;
	//		for (UINT Rust = 0; Rust < languageList[C].fileExtensions.Size(); Rust++)
	//		{
	//			languages.addEntry(languageList[C].fileExtensions[Rust], language);
	//		}
	//		break;
	//	}

	//	lang = language;
	//}

	//lang->PreProcessFile(file);
}

UINT TAnaGameCodeEnvironment::RunTask(TString& task)
{
	return 0;
}

UINT TAnaGameCodeEnvironment::SetUpEnv()
{
	return 0;
}

void TAnaGameCodeEnvironment::Compile()
{
}

void TAnaGameCodeEnvironment::Compile(TrecPointer<TFile> logFile)
{
}

void TAnaGameCodeEnvironment::Log()
{
}

void TAnaGameCodeEnvironment::Run()
{
}
