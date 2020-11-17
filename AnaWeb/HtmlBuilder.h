#pragma once
#include <TObject.h>
#include "HtmlHeader.h"
#include "HtmlBody.h"
#include <TEnvironment.h>
class ANA_WEB_DLL HtmlBuilder : public TObject
{
public:
	HtmlBuilder(TrecPointer<TEnvironment> env);

	TString BuildPage(TrecPointer<TFile> file);

private:
	TrecPointer<HtmlHeader> header;
	TrecPointer<HtmlBody> body;
	TString topLine;
	TrecPointer<TEnvironment> environment;
	TrecPointer<TFileShell> directory;
};

