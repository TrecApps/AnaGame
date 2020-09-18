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


	/**
	 * Method: TAnaGameCodeEnvironment::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	void PreProcessSingleFile(TrecPointer<TFile> file);

	virtual UINT RunTask(TString& task)override;
	virtual UINT SetUpEnv()override;

	virtual void Compile()override;
	virtual void Compile(TrecPointer<TFile> logFile)override;

	virtual void Log()override;

	virtual void Run()override;
	virtual void Run(TrecPointer<TFileShell> file)override;

	virtual TrecPointer<TObjectNode> GetBrowsingNode() override;

	virtual bool SupportsFileExt(const TString& ext) override;
	virtual void SupportsFileExt(TDataArray<TString>& ext) override;

	/**
	 * Method: TEnvironment::Print
	 * Purpose: Allows external code to manually add something to print out
	 * Parameters: TString& input - the command to enter
	 * Returns: bool - whether a prompt was set or not
	 */
	virtual bool Print(TString& input)override;

	/**
	 * Method: TEnvironment::PrintLine
	 * Purpose: Allows external code to manually add something to print out, adding an extra new line at the end
	 * Parameters: TString& input - the command to enter
	 * Returns: void
	 */
	virtual bool PrintLine(TString& input)override;

protected:
	TString targetExtensions;	// Extensions to target for compilation

	CompileErrorHandling compileErrorHandling;
	TargetAnagameMachine targetMachine;
	TString endEnvironment;

	TMap<TLanguage> languages;

	TDataArray<Variable> globalVariables;
};

