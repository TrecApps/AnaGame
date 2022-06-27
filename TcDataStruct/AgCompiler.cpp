#include "AgCompiler.h"

static TDataArray<TrecPointer<AgCompiler::Builder>> compilerBuiders;

void AgCompiler::EstablishDefaultCompilers()
{
	if (!compilerBuiders.Size())
	{

	}
}

UINT AgCompiler::SubmitBuilder(TrecPointer<Builder> builder)
{
	EstablishDefaultCompilers();
	if(!builder.Get())
		return 0;

	for (UINT Rust = 0; compilerBuiders.Size(); Rust++)
	{
		if (!compilerBuiders[Rust].Get())
		{
			compilerBuiders[Rust] = builder;
			return Rust;
		}
	}

	return compilerBuiders.push_back(builder);
}

TrecPointer<AgCompiler> AgCompiler::GetReader(TrecPointer<TFileShell> file, ag_lang_output output)
{
	TrecPointer<AgCompiler> ret;
	for (UINT Rust = 0; Rust < compilerBuiders.Size(); Rust++)
	{
		if (!compilerBuiders[Rust].Get()) continue;
		ret = compilerBuiders[Rust]->GetCompiler(file, output);
		if (ret.Get())
			return ret;
	}
	return ret;
}

bool AgCompiler::RemoveBuilder(UINT index)
{
	if(index >= compilerBuiders.Size())
	return false;
	compilerBuiders[index].Nullify();
}
