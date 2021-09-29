#include "TFormatReaderXml.h"

TString TFormatReaderXml::Read()
{
	return TString();
}

TrecPointer<TVariable> TFormatReaderXml::GetData()
{
	return TrecPointer<TVariable>();
}

TFormatReaderXml::TFormatReaderXml(TrecPointer<TFileShell> file)
{
}

TrecPointer<TFormatReader> TFormatReaderXml::TFormatReaderBuilderXml::GetReader(TrecPointer<TFileShell>)
{
	return TrecPointer<TFormatReader>();
}
