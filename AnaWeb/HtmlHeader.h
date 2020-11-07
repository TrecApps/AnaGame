#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TEnvironment.h>
#include <TDataMap.h>
#include "HtmlScriptBlock.h"

/**
 * Class: HtmlHeader
 * Purpose: Represents the Html Header in Anagame
 */
class HtmlHeader : public TObject
{
public:
    /**
     * Method: HtmlHeader::HtmlHeader
     * Purpose: Constructor
     * Parameters: TrecPointer<TEnvironment> env - the environment to work with
     * Returns: New HtmlHeaderObject
     */
    HtmlHeader(TrecPointer<TEnvironment> env);

    /**
     * Method: HtmlHeader::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TrecPointer<TFile> file - the file to read from
     *              const TString& data - The line that was already read
     *              TrecPointer<TEnvironment> env - the Environment to work with
     * Returns: UINT - error code (0 for success)
     */
    TString ProcessHtml(TrecPointer<TFile> file, const TString& data);


private:
    /**
     * The Environment to work with
     */
    TrecPointer<TEnvironment> env;

    /**
     * The attributes gained
     */
    TDataMap<TString> attributes, meta;

    /**
     * 
     */
    TDataArray<TrecPointer<HtmlScriptBlock>> scripts;

    TString css, title;
};

