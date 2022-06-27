#include "TcBashInterpretor.h"
#include "CodeStatement.h"


void TcBashInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
    UNREFERENCED_PARAMETER(isFirst);

    // Make sure the file reference isn't null
    if (!codeFile.Get())
    {
        ret.errorMessage.Set(L"Null File Reference passed!");
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    }

    StatementCollector collector;
    
    // Collector Defaults to JavaScript so need to configure it for Bash
    TDataArray<TString> setting;
    setting.push_back(L"do");
    setting.push_back(L"then");
    collector.SetBlockStart(setting);
    setting.RemoveAll();
    
    setting.push_back(L"done");
    setting.push_back(L"fi");
    collector.SetBlockEnd(setting);
    setting.RemoveAll();

    collector.SetMultiLineStart(setting);
    collector.SetMultiLineEnd(setting);
    collector.SetSingleString(setting);


    setting.push_back(L"#");
    collector.SetSingleLine(setting);
    setting.RemoveAll();

    setting.push_back(L'\'');
    setting.push_back(L'\"');
    collector.SetMultiString(setting);
    setting.RemoveAll();

    collector.TurnOnOneLinePerStatement();

    UINT line = 0;
    TString stack;

    switch (collector.RunCollector(codeFile, ret.errorMessage, line))
    {
    case 1:
        ret.errorMessage.Format(L"Could not Open JavaScript File '%ws' for reading!", codeFile->GetPath().GetConstantBuffer().getBuffer());
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    case 2:
        ret.returnCode = ReturnObject::ERR_GENERIC_ERROR;
        stack.Format(L"In File '%ws'", codeFile->GetPath().GetConstantBuffer().getBuffer());
        ret.stackTrace.push_back(stack);
        stack.Format(L"At Line: %d", line);
        ret.stackTrace.push_back(stack);
        return;
    }

    collector.CollectStatement(statements);
}

ReturnObject TcBashInterpretor::Run()
{
    return ReturnObject();
}

void TcBashInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
    UNREFERENCED_PARAMETER(params);
}

void TcBashInterpretor::PreProcess(ReturnObject& ret)
{
    UNREFERENCED_PARAMETER(ret);
}

void TcBashInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
    UNREFERENCED_PARAMETER(statement);
    UNREFERENCED_PARAMETER(ret);
}

TcBashInterpretor::TcBashInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env)
    : TcInterpretor(parentInterpretor, env)
{

}