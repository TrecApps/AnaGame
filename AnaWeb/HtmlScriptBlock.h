#pragma once
#include <TObject.h>
#include <TEnvironment.h>
#include <TDataMap.h>
#include "AnaWeb.h"
class ANA_WEB_DLL HtmlScriptBlock : public TObject
{
public:
	HtmlScriptBlock(TrecPointer<TEnvironment> env);

    /**
     * Method: HtmlScriptBlock::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TrecPointer<TFile> file - the file to read from
     *              const TString& data - The line that was already read
     *              TrecPointer<TEnvironment> env - the Environment to work with
     * Returns: UINT - error code (0 for success)
     */
    TString ProcessHtml(TrecPointer<TFile> file, const TString& data);

    TrecPointer<TFileShell> GetFile();

private:
	TrecPointer<TEnvironment> env;
    /**
     * The attributes gained
     */
    TDataMap<TString> attributes;
};

