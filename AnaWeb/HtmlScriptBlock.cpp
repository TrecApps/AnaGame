#include "pch.h"
#include "HtmlScriptBlock.h"

HtmlScriptBlock::HtmlScriptBlock(TrecPointer<TEnvironment> env)
{
	this->env = env;
}

TString HtmlScriptBlock::ProcessHtml(TrecPointer<TFile> file, const TString& data)
{
	return TString();
}
