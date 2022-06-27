#pragma once
#include <TFileShell.h>
#include "TcCompiler.h"
#include <TcRunner.h>
#include "TContainerVariable.h"

typedef enum class ag_lang_output
{
	// Intermediate Code that can be processed further
	ag_runner,		// Can Create an Anagame Runner
	transpile,		// Can convert code to that of a different Language (Read the String to determine specific languages)
	intermediate,	// Intermediate Representation

	// Native Architextures (Read the String to see if it supports assembly or direct bibary files)
	x86,			// Intel 32-bit
	x86_64,			// Intel 64-bit
	arm,			// ARM 32-bit
	arm_64,			// ARM 64-bit

	// Other notable Virtual Machines
	jvm,			// The Java Virtual Machine
	dot_net,		// Microsoft's .NET Framework

	// All other cases (read the string for details
	other
}ag_lang_output;

/**
 * Class: AgCompiler 
 * Purpose: Represents any Compiler that is to be recognized by Anagame
 */
class AgCompiler
{
public:

	/**
	 * Class: AgCompiler::Builder
	 * Purpose: Interface for Building a Compiler
	 */
	class Builder {
	public:
		virtual TrecPointer<AgCompiler> GetCompiler(TrecPointer<TFileShell> file, ag_lang_output output) = 0;
	};

	virtual TString SupportsFiles() = 0;

	virtual TString SupportsOutput(TDataArray<ag_lang_output>& output) = 0;

	virtual TString Configure(ag_lang_output output, TString& outputDetails, TrecPointer<TVariable> config) = 0;

	virtual bool Prepare(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details) = 0;

	virtual int CompileResources(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details) = 0;

	virtual bool CompleteCompile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details) = 0;



	// Allows Registering of Compilers
	static void EstablishDefaultCompilers();


	static UINT SubmitBuilder(TrecPointer<Builder> builder);

	/**
	 * Method: TFormatReader::GetReader
	 * Purpose: retrieves a reader capable of parsing the provieded file (or null of builder has not been registered)
	 * Parameters: TrecPointer<TFileShell> file - the file to read
	 * Returns: TrecPointer<TFileReader> - the reader to read the file (or null if type is not supported)
	 *
	 * Attributes: static
	 */
	static TrecPointer<AgCompiler> GetReader(TrecPointer<TFileShell> file, ag_lang_output output);

	/**
	 * Method: TFormatReader::RemoveBuilder
	 * Purpose: Removes the Builder from the registry (this os for DLLs that are loaded dynamically during runtime so that their code isn't used after unloading)
	 * Parameters: UINT index - the index of the Builder to remove
	 * Returns: bool - whether the operation was done
	 *
	 * Attributes: static
	 */
	static bool RemoveBuilder(UINT index);
};

