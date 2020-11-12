#pragma once
#include <TObject.h>
#include <TEnvironment.h>


class HtmlLink :
    public TObject
{
public:
    HtmlLink(TrecPointer<TEnvironment> env);

    /**
     * Method: HtmlLink::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TrecPointer<TFile> file - the file to read from
     *              const TString& data - The line that was already read
     *              TrecPointer<TEnvironment> env - the Environment to work with
     * Returns: UINT - error code (0 for success)
     */
    TString ProcessHtml(TrecPointer<TFile> file, const TString& data);

private:
    TrecPointer<TEnvironment> env;

    TString href, rel, crossorigin, integrity, type, media,as;
};

