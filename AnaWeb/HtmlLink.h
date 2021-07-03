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

    /**
     * Method: TObject::getVariableValueStr
     * Purpose: Returns the String value of a TObjects given variable, meant for databinding
     * Parameters: TString& varName - the name of the variable to seek
     * Returns: TString - the variable value in String form, or an empty string if variable does not exist
     *
     * Attributes: override
     */
    virtual TString getVariableValueStr(const TString& varName)override;

private:
    TrecPointer<TEnvironment> env;

    TString href, rel, crossorigin, integrity, type, media,as;
};

