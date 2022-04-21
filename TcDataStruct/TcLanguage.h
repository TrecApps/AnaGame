#pragma once
#include "BNFBase.h"
#include <TMap.h>
#include "TContainerVariable.h"
#include "TcCompiler.h"

class TcLanguage
{
protected:
	TMap<BNFBase> languageBnf;
	TrecSubPointer<TVariable, TContainerVariable> languageComponents;
public:

	TcLanguage(TrecPointer<TVariable> languageData);

	void PrepLanguage(TDataArray<CompileMessage>& errorList);
};

