#include "pch.h"
#include "HtmlHeader.h"

HtmlHeader::HtmlHeader(TrecPointer<TEnvironment> env)
{
    this->env = env;
}

TString HtmlHeader::ProcessHtml(TrecPointer<TFile> file, const TString& data)
{
    return TString();
}
