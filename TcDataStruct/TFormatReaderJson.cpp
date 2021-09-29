#include "TFormatReaderJson.h"

TString TFormatReaderJson::Read()
{
	return TString();
}

TrecPointer<TVariable> TFormatReaderJson::GetData()
{
	return TrecPointer<TVariable>();
}

TFormatReaderJson::TFormatReaderJson(TrecPointer<TFileShell> file)
{
}

TrecPointer<TFormatReader> TFormatReaderJson::TFormatReaderBuilderJson::GetReader(TrecPointer<TFileShell>)
{
	return TrecPointer<TFormatReader>();
}
