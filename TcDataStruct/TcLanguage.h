#pragma once
#include "BNFBase.h"
#include <TMap.h>
#include "TContainerVariable.h"
#include "TStringVariable.h"
#include "TcCompiler.h"

class TcLanguage
{
protected:
	TMap<BNFBase> languageBnf;
	TrecSubPointer<TVariable, TContainerVariable> languageComponents;
public:

	bool SupportsFile(TrecPointer<TFileShell> file);

	TcLanguage(TrecPointer<TVariable> languageData);

	TrecPointer<TFileShell> GetFile(const TString& file, TrecPointer<TFileShell> rootDirectory, TDataArray<TrecPointer<TFileShell>>& otherDirectories, bool& isFile);

	TDataArray<TString> GetOtherModules(TrecPointer<TFileShell> fileToRead);

	void PrepLanguage(TDataArray<CompileMessage>& errorList);

	TrecPointer<TVariable> GetLanguageComponents();

	void GenerateRegularStatements(TDataArray<TrecPointer<TcBaseStatement>>& regularStatements, TDataArray<TcPreStatement>& preStatements, TDataArray<CompileMessage>& errorList);
};

