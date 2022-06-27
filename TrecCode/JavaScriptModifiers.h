#pragma once
#include <TFileModifier.h>
#include <TFile.h>

/**
 * Class: JSCommentModifier
 * Purpose: Filters comments from JavaScript Files
 * 
 * SUperClass: TFileModifier - interface so classes can use them
 */
class JSCommentModifier : public TFileModifier
{
public:
    /**
     * Method: JSCommentModifier::JSCommentModifier
     * Purpose: Constructor
     * Parameters: TrecPointer<TFileShell> inputFile - input file
     *              const TString& output - output file (in string form as the output file might not exist yet)
     * Returns: new File Modifier
     */
    JSCommentModifier(TrecPointer<TFileShell> inputFile, const TString& outputFile);

    /**
     * Method: JSCommentModifier::Modify
     * Purpose: Calls the Default operation
     * Parameters: const TString& details - information that might influence the process of File Modification
     * Returns: TString - error information (empty string means no error)
     *
     * Attributes: override
     */
    virtual TString Modify(const TString& details) override;
};


/**
 * Class: JSSemiColonModifier
 * Purpose: Provides SemiColon insertion for JS Files
 *
 * SUperClass: TFileModifier - interface so classes can use them
 */
class JSSemiColonModifier : public TFileModifier
{
public:
    /**
     * Method: JSSemiColonModifier::JSSemiColonModifier
     * Purpose: Constructor
     * Parameters: TrecPointer<TFileShell> inputFile - input file
     *              const TString& output - output file (in string form as the output file might not exist yet)
     * Returns: new File Modifier
     */
    JSSemiColonModifier(TrecPointer<TFileShell> inputFile, const TString& outputFile);

    /**
     * Method: JSSemiColonModifier::Modify
     * Purpose: Calls the Default operation
     * Parameters: const TString& details - information that might influence the process of File Modification
     * Returns: TString - error information (empty string means no error)
     *
     * Attributes: override
     */
    virtual TString Modify(const TString& details) override;

protected:
    bool hasOddMiltiLineStrMarkers(const TString& str);
};