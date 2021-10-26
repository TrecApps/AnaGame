#include "TcPythonInterpretor.h"

TcPythonInterpretor::TcPythonInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env): 
	TcTypeInterpretor(parentInterpretor, env)
{
    preProcessed = true;
}

COMPILE_TYPE TcPythonInterpretor::CanCompile()
{
	return 0;
}

void TcPythonInterpretor::Compile(COMPILE_TYPE type, ReturnObject& ret)
{
	ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
	ret.errorMessage.Set(L"Support for JavaScript Compilation is not available at this time!");
}

void TcPythonInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
    // Make sure the file reference isn't null
    if (!codeFile.Get())
    {
        ret.errorMessage.Set(L"Null File Reference passed!");
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    }

    // Make sure we are dealing with a JS file
    if (!codeFile->GetPath().EndsWith(L".py", true))
    {
        ret.errorMessage.Set(L"Expected a .py file!");
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    }

    StatementCollector collector;

    // Collector is optimized for JavaScript by Default, so we need to configure it for Python
    TDataArray<TString> tokens;

    // Remove Support for Token based blocks - python uses indentation
    collector.SetBlockStart(tokens);
    collector.SetBlockEnd(tokens);

    // Now to deal with Comments
    collector.SetMultiLineStart(tokens); // Python doesn't have "multi-line" comments
    collector.SetMultiLineEnd(tokens);
    tokens.push_back(L"#");                 // But it does have single line comments
    collector.SetSingleLine(tokens);

    // Deal with Quotes
    tokens.RemoveAll();
    tokens.push_back(L"\"");
    tokens.push_back(L"\'");
    collector.SetSingleString(tokens);
    tokens.RemoveAll();
    tokens.push_back(L"\"\"\"");
    tokens.push_back(L"\'\'\'");
    collector.SetMultiString(tokens);

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
    TObjectLocker lock(&thread);
    collector.CollectStatement(statements);
}

ReturnObject TcPythonInterpretor::Run()
{
    TObjectLocker lock(&this->thread);
    ReturnObject ret;
    if (!preProcessed)
    {
        ret.errorMessage.Set(L"Interpretor not ready to run. Make sure you are successful in calling 'SetFile' and 'PreProcess' before calling this method");
        ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
        return ret;
    }

    bool ifDone = false;
}

void TcPythonInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
    UINT foundStar = 0;

    for (UINT Rust = 0; Rust < this->paramNames.Size(); Rust++)
    {
        if (paramNames[Rust].StartsWith(L'*'))
            foundStar++;
    }

    if (foundStar == 1)
    {
        if (!paramNames[paramNames.Size() - 1].StartsWith(L'*'))
            throw L"Found Parameter with the * operator not at the end";

        UINT Rust = 0;
        TrecSubPointer<TVariable, TContainerVariable> cont = // Documentation says it is a tuple
            TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_tuple);
        for (; Rust < params.Size(); Rust++)
        {
            if (Rust < paramNames.Size() - 1)
                variables.addEntry(paramNames[Rust], TcVariableHolder(true, L"", params[Rust]));
            else
                assert(cont->AppendValue(params[Rust]));
        }

        if (Rust < paramNames.Size())
            throw L"Insufficient number of Parameters Provided!";

        TString colName(paramNames[paramNames.Size() - 1].SubString(1));
        variables.addEntry(paramNames[Rust], TcVariableHolder(true, L"", TrecPointerKey::GetTrecPointerFromSub<>(cont)));
        return;
    }
    else if (foundStar > 2)
    {
        throw L"Found more than one Parameter with the * operator";
    }

    // Here, the parameter count entered must match the function expectations
    if (params.Size() != paramNames.Size())
        throw L"Missing Parameters";


    for (UINT Rust = 0; Rust < params.Size(); Rust++)
    {
        variables.addEntry(paramNames[Rust], TcVariableHolder(true, L"", params[Rust]));
    }
}

void TcPythonInterpretor::PreProcess(ReturnObject& ret)
{
}

void TcPythonInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
    TrecPointer<CodeStatement> state = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
    state->statement.Set(statement);

    //ProcessExpression(state, ret, 0);
}
