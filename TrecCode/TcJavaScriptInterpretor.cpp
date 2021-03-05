#include "TcJavaScriptInterpretor.h"

COMPILE_TYPE TcJavaScriptInterpretor::CanCompile()
{
    return COMPILE_TYPE(0);
}

void TcJavaScriptInterpretor::Compile(COMPILE_TYPE type, ReturnObject& ret)
{
    ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
    ret.errorMessage.Set(L"Support for JavaScript Compilation is not available at this time!");
}

void TcJavaScriptInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{
    // Make sure the file reference isn't null
    if (!codeFile.Get())
    {
        ret.errorMessage.Set(L"Null File Reference passed!");
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    }

    // Make sure we are dealing with a JS file
    if (!codeFile->GetPath().EndsWith(L".js", true))
    {
        ret.errorMessage.Set(L"Expected a .js file!");
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    }

    StatementCollector collector;
    UINT line = 0;
    TString stack;
    switch (collector.RunCollector(codeFile, ret.errorMessage, line))
    {
    case 1:
        ret.errorMessage.Format(L"Could not Open JavaScript File '%ws' for reading!", codeFile->GetPath().GetConstantBuffer());
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    case 2:
        ret.returnCode = ReturnObject::ERR_GENERIC_ERROR;
        stack.Format(L"In File '%ws'", codeFile->GetPath().GetConstantBuffer());
        ret.stackTrace.push_back(stack);
        stack.Format(L"At Line: %d", line);
        ret.stackTrace.push_back(stack);
        return;
    }

    collector.CollectStatement(statements);
    readyToRun = false;
}

ReturnObject TcJavaScriptInterpretor::Run()
{
    ReturnObject ret;
    if (!readyToRun)
    {
        ret.errorMessage.Set(L"Interpretor not ready to run. Make sure you are successful in calling 'SetFile' and 'PreProcess' before calling this method");
        ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
        return;
    }

    return Run(statements);
}

void TcJavaScriptInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
}

void TcJavaScriptInterpretor::PreProcess(ReturnObject& ret)
{
    if (readyToRun)
        return;
    PreProcess(ret, statements);
    if (!ret.returnCode)
        readyToRun = true;
}

void TcJavaScriptInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
}

TcJavaScriptInterpretor::TcJavaScriptInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env):
    TcInterpretor(parentInterpretor, env)
{
    readyToRun = false;
}

ReturnObject TcJavaScriptInterpretor::Run(TDataArray<TrecPointer<CodeStatement>>& statements)
{
    ReturnObject ret;
    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        ret = Run(statements[Rust]);
        if (ret.returnCode)
            break;
    }


    return ret;
}

ReturnObject TcJavaScriptInterpretor::Run(TrecPointer<CodeStatement>& statement)
{
    ReturnObject ret;

    switch (statement->statementType)
    {
    case code_statement_type::cst_break:

        break;
    case code_statement_type::cst_catch:

        break;
    case code_statement_type::cst_class:

        break;
    case code_statement_type::cst_const:

        break;
    case code_statement_type::cst_continue:

        break;
    case code_statement_type::cst_do:

        break;

    case code_statement_type::cst_else:

        break;
    case code_statement_type::cst_else_if:

        break;
    case code_statement_type::cst_finally:

        break;
    case code_statement_type::cst_for:

        break;
    case code_statement_type::cst_function:

        break;
    case code_statement_type::cst_if:

        break;
    case code_statement_type::cst_let:

        break;
    case code_statement_type::cst_regular:

        break;
    case code_statement_type::cst_return:

        break;
    case code_statement_type::cst_switch:

        break;
    case code_statement_type::cst_throw:

        break;
    case code_statement_type::cst_try:

        break;
    case code_statement_type::cst_var:

        break;
    case code_statement_type::cst_while:

        break;
    }

    return ReturnObject();
}

void TcJavaScriptInterpretor::PreProcess(ReturnObject& ret, TDataArray<TrecPointer<CodeStatement>>& statements)
{
    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        PreProcess(ret, statements[Rust]);
        if (ret.returnCode)
            return;

        if (statements[Rust]->statementType == code_statement_type::cst_for &&
            statements[Rust]->block.Size() == 0)
        {
            // To-Do: Handle 3 statement for-block
        }
    }
}

void TcJavaScriptInterpretor::PreProcess(ReturnObject& ret, TrecPointer<CodeStatement>& state)
{
    if (!state.Get())
        return;

    state->statement.Trim();

    TString startStatement(state->statement);

    if (startStatement.StartsWith(L"if", false, true) || startStatement.StartsWith(L"if("))
    {
        state->statementType = code_statement_type::cst_if;
        state->statement.Delete(0, 2);

        state->statement.Trim();
        if (!state->statement.StartsWith(L'(') || !state->statement.EndsWith(L')'))
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Set(L"expected 'if' statement to be surrounded by ()");
        }
        else
        {
            PreProcess(ret, state->block);
            if (ret.returnCode) return;
            PreProcess(ret, state->next);
        }
        return;
    }

    if (startStatement.StartsWith(L"else"))
    {
        if (!state->parent.Get())
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unepected 'else' token detected!");
            return;
        }

        code_statement_type parentType = state->parent.Get()->statementType;
        if (parentType != code_statement_type::cst_else_if && parentType != code_statement_type::cst_if)
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unepected 'else' token detected! Epected Previous statement to be an 'if' block or an 'else if' block!");
            return;
        }

        state->statement.Delete(0, 4);
        state->statement.Trim();

        if (startStatement.StartsWith(L"if", false, true) || startStatement.StartsWith(L"if("))
        {
            state->statementType = code_statement_type::cst_else_if;
            state->statement.Delete(0, 2);

            state->statement.Trim();
            if (!state->statement.StartsWith(L'(') || !state->statement.EndsWith(L')'))
            {
                ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
                ret.errorMessage.Set(L"expected 'if' statement to be surrounded by ()");
                return;
            }
        }
        else
        {
            state->statementType = code_statement_type::cst_else;
        }
        PreProcess(ret, state->block);
        if (ret.returnCode) return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"while", false, true) || startStatement.StartsWith(L"while("))
    {
        state->statementType = code_statement_type::cst_while;
        state->statement.Delete(0, 5);

        state->statement.Trim();
        if (!state->statement.StartsWith(L'(') || !state->statement.EndsWith(L')'))
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Set(L"expected 'while' statement to be surrounded by ()");
        }
        else
        {
            PreProcess(ret, state->block);
            if (ret.returnCode) return;
            PreProcess(ret, state->next);
        }
        return;
    }

    if (startStatement.StartsWith(L"const", false, true))
    {
        state->statementType = code_statement_type::cst_const;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        if (state->statement.Find(L'=') < 1)
        {
            ret.errorMessage.Format(L"Const statement '%ws'; Must have a value assigned to value!");
            ret.returnCode = ReturnObject::ERR_INCOMPLETE_STATEMENT;
            return;
        }

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
    }

    if (startStatement.StartsWith(L"function", false, true))
    {
        state->statementType = code_statement_type::cst_function;
        state->statement.Delete(0, 8);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"let", false, true))
    {
        state->statementType = code_statement_type::cst_let;
        state->statement.Delete(0, 3);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
    }

    if (startStatement.StartsWith(L"var", false, true))
    {
        state->statementType = code_statement_type::cst_var;
        state->statement.Delete(0, 3);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
    }

    if (startStatement.StartsWith(L"class", false, true))
    {
        state->statementType = code_statement_type::cst_class;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"return", false, true) || startStatement.StartsWith(L"return;"))
    {
        state->statementType = code_statement_type::cst_return;
        state->statement.Delete(0, 6);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
    }

    if (startStatement.StartsWith(L"break", false, true) || startStatement.StartsWith(L"break;"))
    {
        state->statementType = code_statement_type::cst_break;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
    }

    if (startStatement.StartsWith(L"try", false, true))
    {
        state->statementType = code_statement_type::cst_try;
        state->statement.Delete(0, 3);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode) return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"catch", false, true) || startStatement.StartsWith(L"catch("))
    {
        if (!state->parent.Get())
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unepected 'catch' token detected!");
            return;
        }

        code_statement_type parentType = state->parent.Get()->statementType;
        if (parentType != code_statement_type::cst_try)
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unepected 'catch' token detected! Epected Previous statement to be a 'try' block!");
            return;
        }

        state->statementType = code_statement_type::cst_catch;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"finally", false, true) || startStatement.StartsWith(L"finally{"))
    {
        if (!state->parent.Get())
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unepected 'catch' token detected!");
            return;
        }

        code_statement_type parentType = state->parent.Get()->statementType;
        if (parentType != code_statement_type::cst_try && parentType != code_statement_type::cst_catch)
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unepected 'catch' token detected! Epected Previous statement to be a 'try' or a 'catch' block!");
            return;
        }

        state->statementType = code_statement_type::cst_catch;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"continue", false, true) || startStatement.StartsWith(L"continue;"))
    {
        state->statementType = code_statement_type::cst_break;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
    }

    state->statementType = code_statement_type::cst_regular;
    PreProcess(ret, state->block);
    if (ret.returnCode)return;
    PreProcess(ret, state->next);
}

void TcJavaScriptInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::ProcessAssignmentStatement(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::ProcessBasicFlow(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::ProcessWhile(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::ProcessFor(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::ProcessTryCatchFinally(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

void TcJavaScriptInterpretor::ProcessSwitch(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}
