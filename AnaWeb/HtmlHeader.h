#pragma once
#include <TObject.h>
#include <TFile.h>

/**
 * Class: HtmlHeader
 * Purpose: Represents the Html Header in Anagame
 */
class HtmlHeader : public TObject
{
    /**
     * Method: TWebNode::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TrecPointer<TFile> file - the file to read from
     *              const TString& tag - the tag this Node identifies as
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessHtml(TrecPointer<TFile> file);
};

