#include "TcLanguage.h"

bool TcLanguage::SupportsFile(TrecPointer<TFileShell> file)
{
	if(!file.Get() || file->IsDirectory())
	return false;
	bool present = true;

	TString ext(file->GetName());
	ext.Set(ext.SubString(ext.FindLast(L'.')));
	TrecSubPointer<TVariable, TContainerVariable> extList = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(languageComponents->GetValue(L"FileExt", present));
	TrecSubPointer<TVariable, TStringVariable> extStr = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TStringVariable>(languageComponents->GetValue(L"FileExt", present));
	if (extStr.Get())
		return !ext.CompareNoCase(extStr->GetString());
	if (!extList.Get())
		return false;
	present = true;
	TString ind;
	TrecPointer<TVariable> var;
	for (UINT Rust = 0; extList->GetValueAt(Rust, ind, var); Rust++)
	{
		if (var.Get() && !ext.CompareNoCase(var->GetString())) return true;
	}
	return false;
}

TcLanguage::TcLanguage(TrecPointer<TVariable> languageData)
{
	languageComponents = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(languageData);
}

TrecPointer<TFileShell> TcLanguage::GetFile(const TString& file, TrecPointer<TFileShell> rootDirectory, TDataArray<TrecPointer<TFileShell>>& otherDirectories, bool& isFile)
{
	//if()
	return TrecPointer<TFileShell>();
}



TDataArray<TString> TcLanguage::GetOtherModules(TrecPointer<TFileShell> fileToRead)
{
	return TDataArray<TString>();
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

TrecPointer<TVariable> TcLanguage::GetLanguageComponents()
{
	return TrecPointerKey::GetTrecPointerFromSub<>(this->languageComponents);
}

void TcLanguage::GenerateRegularStatements(TDataArray<TrecPointer<TcBaseStatement>>& regularStatements, TDataArray<TcPreStatement>& preStatements, TDataArray<CompileMessage>& errorList)
{
	TDataMap<TrecPointer<BNFBase>> bnfData;
	for (UINT Rust = 0; Rust < languageBnf.count(); Rust++)
	{
		auto bnfEntry = languageBnf.GetEntryAt(Rust);
		bnfData.addEntry(bnfEntry->key, bnfEntry->object);
	}
	for (UINT Rust = 0; Rust < preStatements.Size(); Rust++)
	{
		for(UINT C = 0; C < languageBnf.count(); C++)
		{
			auto bnfEntry = languageBnf.GetEntryAt(C);
			UINT start = 0;
			if (bnfEntry->object->IsStatement(bnfData, preStatements[Rust].GetStatementText(), start, false))
			{
				TcBaseStatement baseStatement;
				baseStatement.bnfLine = start;
				baseStatement.startLine = preStatements[Rust].GetStartLine();
				baseStatement.statementText.Set(preStatements[Rust].GetStatementText());

				TDataArray<TcPreStatement> preStatements;
				start = 0;
				TcPreStatement statement;
				while (preStatements[Rust].GetStatementAt(start++, statement))
					preStatements.push_back(statement);
				GenerateRegularStatements(baseStatement.subStatements, preStatements, errorList);

				baseStatement.type = BNFBase::GetStatementType(bnfEntry->key);
				regularStatements.push_back(TrecPointerKey::GetNewTrecPointer<TcBaseStatement>(baseStatement));
				break;
			}
		}
	}
}
