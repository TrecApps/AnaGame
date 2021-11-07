#pragma once

#include "TObject.h"
#include "TFileShell.h"

/**
 * Class: TFileHandler
 * Purpose: Allows Files to be Modified before they are actually used (Can be used to clean up)
 */
class _TREC_LIB_DLL TFileModifier :
    public TObject
{
public:
    /**
     * Method: TFileModifier::TFileModifier
     * Purpose: Constructor
     * Parameters: TrecPointer<TFileShell> inputFile - input file
     *              const TString& output - output file (in string form as the output file might not exist yet)
     * Returns: new File Modifier
     */
    TFileModifier(TrecPointer<TFileShell> inputFile, const TString& outputFile);

    /**
     * Method: TFileModifier::Modify
     * Purpose: Calls the Default operation
     * Parameters: const TString& details - information that might influence the process of File Modification
     * Returns: TString - error information (empty string means no error)
     * 
     * Attributes: abstract
     */
    virtual TString Modify(const TString& details) = 0;

    /**
     * Method: TFileModifier::Modify
     * Purpose: Calls the Default operation
     * Parameters: void
     * Returns: TString - error information (empty string means no error)
     */
    TString Modify();

    /**
     * Method: TFileModifier::GetOutputFile
     * Purpose: Retrives the output file for the modifier
     * Parameters: void
     * Returns: TrecPointer<TFileShell> the file that was written to
     */
    TrecPointer<TFileShell> GetOutputFile();

protected:

    /**
     * Input File
     */
    TrecPointer<TFileShell> inputFile;

    /**
     * Output File Path
     */
    TString outputFile;
};
