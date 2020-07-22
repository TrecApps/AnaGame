#pragma once
#include <TEnvironment.h>
#include <TDirectory.h>
#include "TLanguage.h"

typedef enum class CompileErrorHandling
{
	ceh_stop,
	ceh_contain_to_block,
	ceh_document_errors
}CompileErrorHandling;

typedef enum class TargetAnagameMachine
{
	tam_object_register,
	tam_object_stack,
	tam_binary_register,
	tam_binary_stack
}TargetAnagameMachine;

typedef struct Variable
{
	TString name;
	TrecPointer<TType> type;
	UINT location;
}Variable;


class TAnaGameCodeEnvironment :
	public TEnvironment
{
public:
	TAnaGameCodeEnvironment(TrecPointer<TFileShell> shell);
	void PreProcessSingleFile(TrecPointer<TFile> file);

	virtual UINT RunTask(TString& task);
	virtual UINT SetUpEnv();

	virtual void Compile();
	virtual void Compile(TrecPointer<TFile> logFile);

	virtual void Log();

	virtual void Run();


protected:
	TString targetExtensions;	// Extensions to target for compilation

	CompileErrorHandling compileErrorHandling;
	TargetAnagameMachine targetMachine;
	TString endEnvironment;

	TMap<TLanguage> languages;

	TDataArray<Variable> globalVariables;
};

