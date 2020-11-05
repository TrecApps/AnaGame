#include "pch.h"
#include "HtmlBody.h"

HtmlBody::HtmlBody(TrecPointer<TEnvironment> env)
{
    this->env = env;
}

TString HtmlBody::ProcessHtml(TrecPointer<TFile> file, const TString& data)
{
    return TString();
}
