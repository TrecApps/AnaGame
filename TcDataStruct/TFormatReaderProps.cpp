#include "TFormatReaderProps.h"

TString TFormatReaderProps::Read()
{
	return TString();
}

TrecPointer<TVariable> TFormatReaderProps::GetData()
{
	return TrecPointer<TVariable>();
}

TFormatReaderProps::TFormatReaderProps(TrecPointer<TFileShell> file)
{
}

TrecPointer<TFormatReader> TFormatReaderProps::TFormatReaderBuilderProps::GetReader(TrecPointer<TFileShell>)
{
	return TrecPointer<TFormatReader>();
}
