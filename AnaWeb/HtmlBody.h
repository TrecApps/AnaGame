#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TEnvironment.h>
#include "AnaWeb.h"

class ANA_WEB_DLL HtmlBody : public TObject
{
public:
    /**
     * Method: HtmlBody::HtmlHeader
     * Purpose: Constructor
     * Parameters: TrecPointer<TEnvironment> env - the environment to work with
     * Returns: New HtmlHeaderObject
     */
    HtmlBody(TrecPointer<TEnvironment> env);

    /**
     * Method: HtmlBody::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TrecPointer<TFile> file - the file to read from
     *              const TString& data - The line that was already read
     * Returns: UINT - error code (0 for success)
     */
    TString ProcessHtml(TrecPointer<TFile> file, const TString& data);

private:
    TrecPointer<TEnvironment> env;
};

