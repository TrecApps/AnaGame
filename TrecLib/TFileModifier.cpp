#include "TFileModifier.h"
#include "TFile.h"

TFileModifier::TFileModifier(TrecPointer<TFileShell> inputFile, const TString& outputFile)
{
    this->inputFile = inputFile;
    this->outputFile.Set(outputFile);
    if (!inputFile.Get())
        throw L"Invalid Input File";
    TFile file(outputFile, TFile::t_file_open_always);
    if (!file.IsOpen())
        throw L"Invalid Output File Path";
    file.Close();
}

TString TFileModifier::Modify()
{
    return Modify(L"default");
}
