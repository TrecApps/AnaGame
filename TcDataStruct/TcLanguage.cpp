#include "TcLanguage.h"

TcLanguage::TcLanguage(TrecPointer<TVariable> languageData)
{
	languageComponents = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(languageData);
}

void TcLanguage::PrepLanguage(TDataArray<CompileMessage>& errorList)
{
	if (!languageComponents.Get())
	{
		CompileMessage message;
		message.isError = true;
		message.message.Set(L"LANGUAGE SPEC ERROR! Variable specifying the language was either null or not a container variable!");
		errorList.push_back(message);
		return;
	}

	bool present = true;

	TrecSubPointer<TVariable, TContainerVariable> bnf = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(languageComponents->GetValue(L"BNF", present));

	if (!languageComponents.Get())
	{
		CompileMessage message;
		message.isError = true;
		message.message.Set(L"LANGUAGE SPEC ERROR! Variable specifying the language did not have a valid 'BNF' attribute!");
		errorList.push_back(message);
		return;
	}

	TString bnfKey;
	TrecPointer<TVariable> bnfValue;
	for (UINT Rust = 0; bnf->GetValueAt(Rust, bnfKey, bnfValue); Rust++)
	{
		TrecSubPointer<TVariable, TContainerVariable> bnfFields = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(bnfValue);
		if (!bnfFields.Get())
		{
			CompileMessage message;
			message.message.Format(L"Warning! Expected BNF field '%ws' to be an array! For flexibility reasons, this is not an error, but could cause the compiler to not process certain statements correctly!"
				, bnfKey.GetConstantBuffer().getBuffer());
			errorList.push_back(message);
			continue;
		}
		TDataArray<TString> bnfStrings;
		for (UINT C = 0; C < bnfFields->GetSize(); C++)
		{
			bnfStrings.push_back(bnfFields->GetValueAt(C)->GetString());
		}

		languageBnf.addEntry(bnfKey, BNFBase::GetBnf(bnfKey, bnfStrings));
	}

}
