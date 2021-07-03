#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TEnvironment.h>
#include <TDataMap.h>
#include "HtmlScriptBlock.h"
#include "HtmlLink.h"

#include "AnaWeb.h"

class HtmlHeaderBase
{
public:
    HtmlHeaderBase();

    bool isSet;
    TString url, target;
};

/**
 * Class: HtmlHeader
 * Purpose: Represents the Html Header in Anagame
 */
class ANA_WEB_DLL HtmlHeader : public TObject
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

    TString RetrieveCss();

    TString RetrieveTitle();

    TrecPointer<HtmlLink> GetLink(UINT index);


private:
    /**
     * The Environment to work with
     */
    TrecPointer<TEnvironment> env;

    void getBaseElement(const TString& att, const TString& val);

    /**
     * The attributes gained
     */
    TDataMap<TString> attributes, meta;

    /**
     * 
     */
    TDataArray<TrecPointer<HtmlScriptBlock>> scripts;

    /**
     * Basic String details (note, this css field should only hold direct css data from "<style>" tags
     */
    TString css, title;

    /**
     * List of Links detected
     */
    TDataArray<TrecPointer<HtmlLink>> links;

    /**
     * Refers to the base block of an html header
     */
    HtmlHeaderBase base;
};

