#include "TFormatReaderHtml.h"

TString TFormatReaderHtml::Read()
{
	return TString();
}

TrecPointer<TVariable> TFormatReaderHtml::GetData()
{
	return TrecPointer<TVariable>();
}

TFormatReaderHtml::TFormatReaderHtml(TrecPointer<TFileShell> file)
{
}

TrecPointer<TFormatReader> TFormatReaderHtml::TFormatReaderBuilderHtml::GetReader(TrecPointer<TFileShell>)
{
	return TrecPointer<TFormatReader>();
}
