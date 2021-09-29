#include "TFormatReaderTml.h"

TrecPointer<TFormatReader> TFormatReaderTml::TFormatReaderBuilderTml::GetReader(TrecPointer<TFileShell>)
{
    return TrecPointer<TFormatReader>();
}

TString TFormatReaderTml::Read()
{
    return TString();
}

TrecPointer<TVariable> TFormatReaderTml::GetData()
{
    return TrecPointer<TVariable>();
}

TFormatReaderTml::TFormatReaderTml(TrecPointer<TFileShell> file)
{
    this->fileShell = file;
}

