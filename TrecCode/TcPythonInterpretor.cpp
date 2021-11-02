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

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        switch (statements[Rust]->statementType) {
        case code_statement_type::cst_if:
            ProcessIf(Rust, ret);
            break;
        case code_statement_type::cst_else:
        case code_statement_type::cst_else_if:
            // Set up error
            break;
        case code_statement_type::cst_while:
            ProcessWhile(Rust, ret);
            break;
        case code_statement_type::cst_for:
            ProcessFor(Rust, ret);
        }
    }
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
    PythonPreProcess(statements, false, ret);
    if (!ret.returnCode)
        preProcessed = true;
}

void PythonPreProcess(TDataArray<TrecPointer<CodeStatement>>& statements, bool indentationExpected, ReturnObject& ret)
{
    if (!statements.Size())
    {
        ret.returnCode = ret.ERR_BROKEN_REF;
        ret.errorMessage.Set(indentationExpected ? L"IndentationError: expected an indented block" : L"No Code in File");
        return;
    }

    for (UINT Rust = 0; Rust < statements.Size() && !ret.returnCode; Rust++)
    {
        PythonPreProcess(statements[Rust], ret);
    }
}

void PythonPreProcess(TrecPointer<CodeStatement>& statement, ReturnObject& ret)
{
    if (!statement.Get())
    {
        ret.returnCode = ret.ERR_BROKEN_REF;
        return;
    }
    UINT deleteCount = 0;

    if (statement->statement.StartsWith(L"if", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 2;
        statement->statementType = code_statement_type::cst_if;
    }
    else if (statement->statement.StartsWith(L"elif", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 4;
        statement->statementType = code_statement_type::cst_else_if;
    }
    else if (statement->statement.StartsWith(L"else", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 4;
        statement->statementType = code_statement_type::cst_else;
    }
    else if (statement->statement.StartsWith(L"while", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 5;
        statement->statementType = code_statement_type::cst_while;
    }
    else if (statement->statement.StartsWith(L"for", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 3;
        statement->statementType = code_statement_type::cst_for;
    }
    else if (statement->statement.StartsWith(L"def", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 3;
        statement->statementType = code_statement_type::cst_function;
    }
    else if (statement->statement.StartsWith(L"class", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 5;
        statement->statementType = code_statement_type::cst_class;
    }
    else if (statement->statement.StartsWith(L"del", false, true))
    {
        deleteCount = 3;
        statement->statementType = code_statement_type::cst_delete;
    }
    else if (statement->statement.StartsWith(L"try", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 3;
        statement->statementType = code_statement_type::cst_try;
    }
    else if (statement->statement.StartsWith(L"except", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 6;
        statement->statementType = code_statement_type::cst_catch;
    }
    else if (statement->statement.StartsWith(L"finally", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 7;
        statement->statementType = code_statement_type::cst_finally;
    }
    else if (statement->statement.StartsWith(L"", false, true))
    {

    }
    else if (statement->statement.StartsWith(L"", false, true))
    {

    }

    if (!ret.returnCode && deleteCount)
    {
        statement->statement.Delete(0, deleteCount);
        statement->statement.Trim();
    }
}


void TcPythonInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
    TrecPointer<CodeStatement> state = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
    state->statement.Set(statement);

    //ProcessExpression(state, ret, 0);
}

void TcPythonInterpretor::ProcessIf(UINT& index, ReturnObject& ret)
{
}

void TcPythonInterpretor::ProcessWhile(UINT index, ReturnObject& ret)
{
}

void TcPythonInterpretor::ProcessFor(UINT index, ReturnObject& ret)
{
}
