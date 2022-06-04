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
	protected:
		TrecPointerSoft<CompilerHolder> holder;
	public:
		void SetSelf(TrecPointer<CompilerHolder> self);

		TrecPointer<TFileShell> rootDirectory;
		TDataArray<TrecPointer<TFileShell>> otherDirectories;

		TDataMap<TrecPointer<Tc2Compiler>> fileCompilers, moduleCompilers;

		TDataArray<TrecPointer<TcLanguage>> languages;

		TrecPointer<Tc2Compiler> GetCompilerByFile(const TString& fileName);
		TrecPointer<Tc2Compiler> GetCompilerByModule(const TString& moduleName);

		TrecPointer<Tc2Compiler> SetUpCompiler(const TString& str);

		void Compile(TDataArray<CompileMessage>& messages);
	};

	Tc2Compiler(TrecPointer<CompilerHolder> parent, TrecPointer<TFileShell> file, TrecPointer<TcLanguage> language);


	bool PreCompile(TDataArray<CompileMessage>& messages);
	bool ParseStatementTypes(TDataArray<CompileMessage>& messages);
	bool PrepDependencies(TDataArray<CompileMessage>& messages);

protected:
	TDataArray<TrecPointerSoft<Tc2Compiler>> otherCompilers;
	TrecPointer<TcLanguage> language;

	TMap<TcType> types;
	TrecPointer<TFileShell> file;
	TrecPointerSoft<CompilerHolder> parent;

	TDataArray<TcPreStatement> preStatements;
	TDataArray<TrecPointer<TcBaseStatement>> baseStatements;
	UINT stage;
};

