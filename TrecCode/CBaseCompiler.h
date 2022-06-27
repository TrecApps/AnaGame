#pragma once
#include <AgCompiler.h>
class CBaseCompiler : public AgCompiler
{
	friend class TrecPointerKey;
private:
	CBaseCompiler(TrecPointer<TFileShell> file);
	TrecPointer<TFileShell> file;

	TDataArray<TcPreStatement> preStatements;
	TDataArray<TrecPointer<TcBaseStatement>> baseStatements;
public:

	/**
	 * Class: AgCompiler::Builder
	 * Purpose: Interface for Building a Compiler
	 */
	class Builder: public AgCompiler::Builder {
	public:
		virtual TrecPointer<AgCompiler> GetCompiler(TrecPointer<TFileShell> file, ag_lang_output output) override;
	};

	virtual TString SupportsFiles() override;

	virtual TString SupportsOutput(TDataArray<ag_lang_output>& output) override;

	virtual TString Configure(ag_lang_output output, TString& outputDetails, TrecPointer<TVariable> config) override;

	virtual bool Prepare(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details) override;

	virtual int CompileResources(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details) override;

	virtual bool CompleteCompile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, TContainerVariable>& details) override;
};

