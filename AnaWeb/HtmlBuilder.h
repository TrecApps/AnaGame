#pragma once
#include <TObject.h>
#include "HtmlHeader.h"
#include "HtmlBody.h"
#include <TEnvironment.h>
class ANA_WEB_DLL HtmlBuilder : public TObject
{
public:
	HtmlBuilder(TrecPointer<TEnvironment> env, TrecPointer<DrawingBoard> board);

	TString BuildPage(TrecPointer<TFile> file, HWND win, TrecPointer<TArray<styleTable>>& styles);

	TrecPointer<HtmlHeader> RetrieveHeader();

	TrecPointer<HtmlBody> RetrieveBody();

private:
	TrecPointer<HtmlHeader> header;
	TrecPointer<HtmlBody> body;
	TString topLine;
	TrecPointer<TEnvironment> environment;
	TrecPointer<TFileShell> directory;
	TrecPointer<DrawingBoard> board;
};

