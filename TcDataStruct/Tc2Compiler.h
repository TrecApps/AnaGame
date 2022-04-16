#pragma once
#include "TcType.h"
#include <TFileShell.h>
#include <TMap.h>
#include "TcLanguage.h"



class Tc2Compiler
{

public:
	class CompilerHolder
	{
	public:
		TrecPointer<TFileShell> rootDirectory;

		TDataMap<TrecPointer<Tc2Compiler>> fileCompilers, moduleCompilers;

		TDataArray<TrecPointer<TcLanguage>> languages;

		TrecPointer<Tc2Compiler> GetCompilerByFile(const TString& fileName);
		TrecPointer<Tc2Compiler> GetCompilerByModule(const TString& moduleName);


	};

	Tc2Compiler(TrecPointer<CompilerHolder> parent, TrecPointer<TFileShell> file, TrecPointer<TcLanguage> language);
protected:
	TDataArray<Tc2Compiler> otherCompilers;
	TrecPointer<TcLanguage> language;

	TMap<TcType> types;
	TrecPointer<TFileShell> file;
	TrecPointerSoft<CompilerHolder> parent;
};

