#include "TFormatReaderProps.h"
#include "TContainerVariable.h"
#include "TStringVariable.h"

TString TFormatReaderProps::Read()
{
	file.Open(fileShell->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);
	if (!file.IsOpen())
		return L"Failed to Open TML file!";

	TString lineStr;

	TrecSubPointer<TVariable, TContainerVariable> vRet =
		TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_properties);

	// Read the file, looking for new lines, taking into account backslashes at the end
	while (file.ReadString(lineStr, L'\n', 0b00000100))
	{
		lineStr.Trim();

		// Lines that are blank, Or start with ! or #, can be ignored
		if (!lineStr.GetSize() || lineStr.StartsWith(L'!') || lineStr.StartsWith(L'#'))
			continue;

		auto pieces = lineStr.splitn(L" :=", 2);

		TrecPointer<TVariable> prop = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(pieces->Size() > 1 ? pieces->at(1) : L"");
		vRet->SetValue(pieces->at(0), prop);
	}

	variable = TrecPointerKey::GetTrecPointerFromSub<>(vRet);
	return L"";
}

TrecPointer<TVariable> TFormatReaderProps::GetData()
{
	return variable;
}

TFormatReaderProps::TFormatReaderProps(TrecPointer<TFileShell> file)
{
	fileShell = file;
}

TrecPointer<TFormatReader> TFormatReaderProps::TFormatReaderBuilderProps::GetReader(TrecPointer<TFileShell> file)
{
	if (file.Get() && file->GetName().EndsWith(L".properties", true))
		return TrecPointerKey::GetNewTrecPointerAlt<TFormatReader, TFormatReaderProps>(file);
	return TrecPointer<TFormatReader>();
}
