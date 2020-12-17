#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TEnvironment.h>
#include "AnaWeb.h"
#include "TWebNode.h"

class ANA_WEB_DLL HtmlBody : public TObject
{
public:
    /**
     * Method: HtmlBody::HtmlHeader
     * Purpose: Constructor
     * Parameters: TrecPointer<TEnvironment> env - the environment to work with
     * Returns: New HtmlHeaderObject
     */
    HtmlBody(TrecPointer<TEnvironment> env, TrecPointer<DrawingBoard> board);

    /**
     * Method: HtmlBody::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TrecPointer<TFile> file - the file to read from
     *              const TString& data - The line that was already read
     * Returns: UINT - error code (0 for success)
     */
    TString ProcessHtml(TrecPointer<TFile> file, const TString& data);

    TrecPointer<TWebNode> RetrieveWebNode();

private:
    TrecPointer<TEnvironment> env;

    TrecPointer<TWebNode> rootNode;

    TrecSubPointer<TSliceManager,TStringSliceManager> bodyContents;
    TrecPointer<DrawingBoard> board;
};

