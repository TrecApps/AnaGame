#include "TcJavaScriptInterpretor.h"
#include <TSpecialVariable.h>
#include <TStringVariable.h>
#include <TContainerVariable.h>


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

int TcJavaScriptInterpretor::DetermineParenthStatus(const TString& string)
{
    WCHAR quote = L'\0';
    UINT backslashStack = 0;
    int ret = 0;
    for (UINT Rust = 0; Rust < string.GetSize(); Rust++)
    {
        WCHAR ch = string.GetAt(Rust);

        switch (ch)
        {
        case L'`':
        case L'\'':
        case L'\"':
            if (!quote)
                quote = ch;
            else if (quote == ch && !(backslashStack % 2))
                quote = ch;
            break;
        case L'(':
            if (!quote)
                ret++;
            break;
        case L')':
            if (!quote)
                ret--;
            break;
        }

        if (ch == L'\\')
            backslashStack++;
        else
            backslashStack = 0;
    }

    return ret;
}

void TcJavaScriptInterpretor::SetStatements(TDataArray<TrecPointer<CodeStatement>>& statements)
{
    this->statements = statements;
}

void TcJavaScriptInterpretor::SetStatementToBlock(TrecPointer<CodeStatement>& statement, ReturnObject& ret)
{
    if (statement->block.Size())
    {
        TrecSubPointer<TVariable, TcInterpretor> updatedSelf = TrecPointerKey::GetSubPointerFromSoft<>(self);
        TrecSubPointer<TVariable, TcJavaScriptInterpretor> tcJsInt = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcJavaScriptInterpretor>(updatedSelf, environment);

        tcJsInt->SetStatements(statement->block);
        tcJsInt->PreProcess(ret);
        if (ret.returnCode)
            return;

        statement->statementVar = TrecPointerKey::GetTrecPointerFromSub<TVariable, TcJavaScriptInterpretor>(tcJsInt);
        statement->block.RemoveAll();
    }
}

ReturnObject TcJavaScriptInterpretor::Run(TDataArray<TrecPointer<CodeStatement>>& statements)
{
    ReturnObject ret;
    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        ret = Run(statements, Rust, TrecPointer<CodeStatement>());
        if (ret.returnCode)
            break;
    }


    return ret;
}

ReturnObject TcJavaScriptInterpretor::Run(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, TrecPointer<CodeStatement> statement)
{
    ReturnObject ret;


    if(!statement.Get())
        statement = statements[index];

    switch (statement->statementType)
    {
    case code_statement_type::cst_break:

        break;
    case code_statement_type::cst_class:

        break;
    case code_statement_type::cst_const:
    case code_statement_type::cst_let:
    case code_statement_type::cst_var:
        ProcessAssignmentStatement(statement, ret);
        break;
    case code_statement_type::cst_continue:

        break;
    case code_statement_type::cst_do:

        break;

    case code_statement_type::cst_else:

        break;
    case code_statement_type::cst_else_if:

        break;
    case code_statement_type::cst_for:
    case code_statement_type::cst_for_3:
        ProcessFor(statements, index, ret);

        break;
    case code_statement_type::cst_function:

        break;
    case code_statement_type::cst_if:

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
    case code_statement_type::cst_finally:
        ProcessTryCatchFinally(statements, index, statement, ret);
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
        auto state = statements[Rust];
        PreProcess(ret, state);
        if (ret.returnCode)
            return;

        if (statements[Rust]->statementType == code_statement_type::cst_for)
        {
            // To-Do: Handle 3 statement for-block

            int parenthState = DetermineParenthStatus(state->statement);

            while (state->next.Get() && parenthState > 0)
            {
                state = state->next;
                parenthState += DetermineParenthStatus(state->statement);
            }

            if (parenthState < 0)
            {
                ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
                ret.errorMessage.Format(L"expected 'for' statement to be surrounded by (), but there are %d extra ')' detected!", abs(parenthState));
                return;
            }

            if (parenthState > 0)
            {
                // At this point, we expect two more statements before the for block.
                statements[Rust]->statementType = code_statement_type::cst_for_3;
                if (!((Rust + 2) < statements.Size()))
                {
                    ret.returnCode = ReturnObject::ERR_INCOMPLETE_STATEMENT;
                    ret.errorMessage.Set(L"Not enough statements to complete the 3-statement 'for' block!");
                    return;
                }
                PreProcess(ret, statements[Rust + 1]);
                if (ret.returnCode)
                    return;
                PreProcess(ret, statements[Rust + 2]);
                if (ret.returnCode)
                    return;

                // Assess parenthesis situation in regards to the middle statement
                state = statements[Rust + 1];
                parenthState += DetermineParenthStatus(state->statement);

                while (state->next.Get() && parenthState > 0)
                {
                    state = state->next;
                    parenthState += DetermineParenthStatus(state->statement);
                }

                if (parenthState < 1)
                {
                    ret.returnCode = ReturnObject::ERR_INCOMPLETE_STATEMENT;
                    ret.errorMessage.Set(L"Not enough statements to complete the 3-statement 'for' block!");
                    return;
                }

                // Assess parenthesis situation in rgards to the final statement (this statement will have the block (and thus the Interpretor)
                state = statements[Rust + 2];
                parenthState += DetermineParenthStatus(state->statement);

                while (state->next.Get() && parenthState > 0)
                {
                    state = state->next;
                    parenthState += DetermineParenthStatus(state->statement);
                }
                
                if (parenthState < 0)
                {
                    ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
                    ret.errorMessage.Format(L"expected 'for' statement to be surrounded by (), but there are %d extra ')' detected!", abs(parenthState));
                }
                else if (parenthState > 0)
                {
                    ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
                    ret.errorMessage.Format(L"expected 'for' statement to be surrounded by (), but statement needs %d more ')' to close!", abs(parenthState));
                }


                SetStatementToBlock(state, ret);
                if (ret.returnCode)
                    return;

                Rust += 2;
            }

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
            return;
        }

        auto curState = state;

        int parenthState = DetermineParenthStatus(state->statement);

        while (state->next.Get() && parenthState > 0)
        {
            state = state->next;
            parenthState += DetermineParenthStatus(state->statement);
        }

        if (parenthState < 0)
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Format(L"expected 'if' statement to be surrounded by (), but there are %d extra ')' detected!", abs(parenthState));
        }
        else if (parenthState > 0)
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Format(L"expected 'if' statement to be surrounded by (), but statement needs %d more ')' to close!", abs(parenthState));
        }
        else
        {
            SetStatementToBlock(state, ret);
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

            auto curState = state;

            int parenthState = DetermineParenthStatus(state->statement);

            while (state->next.Get() && parenthState > 0)
            {
                state = state->next;
                parenthState += DetermineParenthStatus(state->statement);
            }

            if (parenthState < 0)
            {
                ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
                ret.errorMessage.Format(L"expected 'if' statement to be surrounded by (), but there are %d extra ')' detected!", abs(parenthState));
                return;
            }
            else if (parenthState > 0)
            {
                ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
                ret.errorMessage.Format(L"expected 'if' statement to be surrounded by (), but statement needs %d more ')' to close!", abs(parenthState));
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
            ret.errorMessage.Set(L"expected 'if' statement to be surrounded by ()");
            return;
        }

        auto curState = state;

        int parenthState = DetermineParenthStatus(state->statement);

        while (state->next.Get() && parenthState > 0)
        {
            state = state->next;
            parenthState += DetermineParenthStatus(state->statement);
        }

        if (parenthState < 0)
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Format(L"expected 'while' statement to be surrounded by (), but there are %d extra ')' detected!", abs(parenthState));
        }
        else if (parenthState > 0)
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Format(L"expected 'while' statement to be surrounded by (), but statement needs %d more ')' to close!", abs(parenthState));
        }
        else
        {
            PreProcess(ret, state->block);
            if (ret.returnCode) return;
            PreProcess(ret, state->next);
        }
        return;
    }

    if (startStatement.StartsWith(L"for", false, true) || startStatement.StartsWith(L"for("))
    {
        state->statementType = code_statement_type::cst_while;
        state->statement.Delete(0, 3);

        PreProcess(ret, state->block);
        if (ret.returnCode) return;
        PreProcess(ret, state->next);
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
    // Attempt to divide the statement up by ',', taking into account strings, into sub-statements
    TDataArray<TString> subStatements;  // Holds the sub-Statements
    TString subStatement;               // The SubStatement in the moment

    WCHAR quote = L'\0', open = L'\0';
    UINT openStack = 0;

    for (UINT Rust = 0; Rust < statement->statement.GetSize(); Rust++)
    {
        WCHAR ch = statement->statement[Rust];

        switch (ch)
        {
        case L',':
            if (!quote && !openStack && subStatement.GetSize())
            {
                subStatements.push_back(subStatement);
                subStatement.Empty();
                continue;
            }
            break;
        case L'\'':
        case L'\"':
        case L'`':
            if (!quote)
                quote = ch;
            else if (quote == ch)
                quote = L'\0';
            break;
        case L'(':
        case L'[':
            if (!quote && (!openStack || open == ch))
            {
                openStack++;
                open = ch;
            }
            break;
        case L')':
            if (!quote && openStack && open == L'(')
            {
                openStack--;
                open = ch;
            }
            break;
        case L']':
            if (!quote && openStack && open == L'[')
            {
                openStack--;
                open = ch;
            }
            break;
        }
        subStatement.AppendChar(ch);
    }
    if (subStatement.GetSize())
    {
        subStatements.push_back(subStatement);
        subStatement.Empty();
    }


    // Go Through Each sub statement and assess the new variable
    for (UINT Rust = 0; Rust < subStatements.Size(); Rust++)
    {
        auto toks = subStatements[Rust].splitn(L"=", 2);

        for (UINT Rust = 0; Rust < toks->Size(); Rust++)
            toks->at(Rust).Trim();

        if (toks->at(0).EndsWith(L';'))
            toks->at(0).Delete(toks->at(0).GetSize() - 1);

        CheckVarName(toks->at(0), ret);
        
        if (ret.returnCode)
        {
            // To-Do: Add Error information

            return;
        }

        bool worked = false;
        TrecPointer<TVariable> car = GetVariable(toks->at(0), worked, true);

        if (worked)
        {
            ret.returnCode = ReturnObject::ERR_EXISTING_VAR;
            ret.errorMessage.Format(L"Variable '%ws' already exists in the current Scope!", toks->at(0).GetConstantBuffer());

            return;
        }
        if (toks->Size() > 1)
        {
            ProcessIndividualStatement(toks->at(1), ret);
            if (ret.returnCode)
            {
                // Add line info

                return;
            }
            TcVariableHolder varHold;
            varHold.mut = statement->statementType != code_statement_type::cst_const;
            varHold.value = ret.errorObject;
            variables.addEntry(toks->at(0), varHold);
        }
        else
        {
            TcVariableHolder varHold;
            varHold.mut = statement->statementType != code_statement_type::cst_const;
            varHold.value = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
            variables.addEntry(toks->at(0), varHold);
        }
    }
    
    if (statement->next.Get())
    {
        statement->next->statementType = statement->statementType;
        ProcessAssignmentStatement(statement->next, ret);
    }
}

void TcJavaScriptInterpretor::ProcessBasicFlow(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    TcJavaScriptInterpretor* jsVar = nullptr;
    switch (statement->statementType)
    {
    case code_statement_type::cst_if:
    case code_statement_type::cst_else_if:
        ProcessExpression(statement, ret);
        if (ret.returnCode)
            return;

        for (; ret.nextCount && statement.Get(); ret.nextCount--)
        {
            statement = statement->next;
        }

        if (!statement.Get())
        {
            ret.returnCode = ReturnObject::ERR_INTERNAL;
            ret.errorMessage.Set(L"INTERNAL ERROR! This is an error with the Interpretor itself and might not have been caused by your code!");
            return;
        }

        if (IsTruthful(ret.errorObject) && statement->statementVar.Get())
        {
            jsVar = dynamic_cast<TcJavaScriptInterpretor*>(statement->statementVar.Get());
            if (jsVar)
                ret = jsVar->Run();
        }
        else if (statement->next.Get())
            ProcessBasicFlow(statement->next, ret);
        break;
    case code_statement_type::cst_else:
        jsVar = dynamic_cast<TcJavaScriptInterpretor*>(statement->statementVar.Get());
        if (jsVar)
            ret = jsVar->Run();
        break;
    default:
        if (statement->next.Get())
            ProcessBasicFlow(statement->next, ret);
    }
}

void TcJavaScriptInterpretor::ProcessWhile(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    TrecPointer<CodeStatement> blockHolder;
    TrecPointer<CodeStatement> expreHolder;
    TcJavaScriptInterpretor* jsVar = nullptr;
    if (statement->statementType == code_statement_type::cst_do)
    {
        blockHolder = statement;
        expreHolder = statement->next;
        if (!expreHolder.Get() || expreHolder->statementType != code_statement_type::cst_while)
        {
            ret.errorMessage.Set(L"Error in Do-While Statement, expected while clause after do-block!");
            ret.returnCode = ReturnObject::ERR_INCOMPLETE_STATEMENT;
            ret;
        }
        jsVar = dynamic_cast<TcJavaScriptInterpretor*>(blockHolder->statementVar.Get());
        if (!jsVar)
            return;
        jsVar->Run();
        ProcessExpression(expreHolder, ret);
        if (ret.returnCode)
            return;
    }
    else
    {
        expreHolder = statement;
        blockHolder = statement;
        ProcessExpression(statement, ret);
        if (ret.returnCode)
            return;

        for (; ret.nextCount && blockHolder.Get(); ret.nextCount--)
        {
            blockHolder = blockHolder->next;
        }
        if (!blockHolder.Get())
        {
            ret.returnCode = ReturnObject::ERR_INTERNAL;
            ret.errorMessage.Set(L"INTERNAL ERROR! This is an error with the Interpretor itself and might not have been caused by your code!");
            return;
        }
        jsVar = dynamic_cast<TcJavaScriptInterpretor*>(blockHolder->statementVar.Get());
        if (!jsVar)
            return;
    }

    
    if (statement->statementType == code_statement_type::cst_do)
        jsVar->Run();

    while (IsTruthful(ret.errorObject))
    {
        jsVar->Run();
        ProcessExpression(expreHolder, ret);
        if (ret.returnCode)
            return;
    }

    //if (statement->statementType == code_statement_type::cst_do)
    //    ret = Run(expreHolder);
    //else ret = Run(blockHolder);
}

void TcJavaScriptInterpretor::ProcessFor(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, ReturnObject& ret)
{
    auto statement = statements[index];
    auto block = statement;

    if (statement->statementType == code_statement_type::cst_for)
    {
        // Single 
        int parenthStatus = DetermineParenthStatus(statement->statement);

        while (block->next.Get() && parenthStatus > 1)
        {
            parenthStatus += DetermineParenthStatus(block->next->statement);
            block = block->next;
        }

        if (parenthStatus < 0)
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Format(L"expected 'For' statement to be surrounded by (), but there are %d extra ')' detected!", abs(parenthStatus));
            return;
        }

        if (parenthStatus > 0)
        {
            ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
            ret.errorMessage.Format(L"expected 'For' statement to be surrounded by (), but statement needs %d more ')' to close!", abs(parenthStatus));
            return;
        }

        ProcessExpression(statement, ret);
        if (ret.returnCode)
            return;

        if (!ret.errorObject.Get() || ret.errorObject->GetVarType() != var_type::collection)
        {
            ret.returnCode = ret.ERR_IMPROPER_TYPE;
            ret.errorMessage.Set(L"Expected Collection type for single-statement For-loop!");
            return;
        }
        if (!ret.errorMessage.GetSize())
        {
            ret.returnCode = ReturnObject::ERR_IMPROPER_NAME;
            ret.errorMessage.Set(L"Expected Name to be provided for indexed Element!");
            return;
        }

        auto jsVar = dynamic_cast<TcJavaScriptInterpretor*>(block->statementVar.Get());
        if (!jsVar)
            return;
        auto container = dynamic_cast<TContainerVariable*>(ret.errorObject.Get());
        TrecPointer<TVariable> var;
        TString varName(ret.errorMessage), indexName;

        for (UINT Rust = 0; container->GetValueAt(Rust, indexName, var); Rust++)
        {
            jsVar->variables.clear();
            TcVariableHolder holder;
            holder.mut = true;
            holder.value = var;
            jsVar->variables.addEntry(varName, holder);
            ret = jsVar->Run();
            if (ret.returnCode)
                return;
        }

        if (block->next.Get())
        {
            Run(statements, index, block->next);
        }
    }
    else
    {
        ProcessExpression(statement, ret);
        if (ret.returnCode)
            return;

        auto jsVar = dynamic_cast<TcJavaScriptInterpretor*>(block->statementVar.Get());

        // To-Do: After implementing ProcessExpression, figure out how to respond to non-errors



        // End To-Do: Now Run the Other two statements

        if (jsVar)
            jsVar->ProcessExpression(statements[index + 1], ret);
        else
            ProcessExpression(statements[index + 1], ret);
        if (ret.returnCode)
            return;

        while (IsTruthful(ret.errorObject))
        {
            if(jsVar)
                ret = jsVar->Run();

            // Check to see if either an error occured or a return statement has been reached.
            // The For loop is not responsible for handling either scenario
            if (ret.returnCode || ret.mode == return_mode::rm_return)
                return;

            // For loop is responsible for handling break and for
            if (ret.mode != return_mode::rm_regular)
            {
                // here, we either break or continue
                return_mode m = ret.mode;
                ret.mode = return_mode::rm_regular;
                if (m == return_mode::rm_break)
                    return;
            }

            if (jsVar)
            {
                jsVar->ProcessExpression(statements[index + 2], ret);
                if (ret.returnCode)
                    return;
                jsVar->ProcessExpression(statements[index + 1], ret);
            }
            else
            {
                ProcessExpression(statements[index + 2], ret);
                if (ret.returnCode)
                    return;
                ProcessExpression(statements[index + 1], ret);
            }
            if (ret.returnCode)
                return;
        }
        
    }
}

void TcJavaScriptInterpretor::ProcessTryCatchFinally(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    auto jsVar = dynamic_cast<TcJavaScriptInterpretor*>(statement->statementVar.Get());
    switch (statement->statementType)
    {
    case code_statement_type::cst_try:
        if (!statement->next.Get() || (statement->statementType != code_statement_type::cst_break && statement->statementType != code_statement_type::cst_finally))
        {
            ret.errorMessage.Set(L"Try-block must be followed by a 'catch' or 'finally' block!");
            ret.returnCode = ReturnObject::ERR_INCOMPLETE_STATEMENT;
            return;
        }
        if(jsVar)
            ret = jsVar->Run();
        ProcessTryCatchFinally(statements, index, statement->next, ret);
        break;
    case code_statement_type::cst_catch:

        if (ret.returnCode)
        {
            TString e(statement->statement);
            e.Trim();
            int parenthState = DetermineParenthStatus(e);
            if (parenthState)
            {
                ret.returnCode = ReturnObject::ERR_PARENTH_MISMATCH;
                ret.errorMessage.Set(L"Error Processing Catch-Block!");
                return;
            }
            while (e.StartsWith(L'('))
            {
                e.Set(e.SubString(1, e.GetSize() - 1));
                e.Trim();
            }
            ReturnObject ret2;
            CheckVarName(e, ret2);
            if (ret2.returnCode)
            {
                ret = ret2;
                ret.errorMessage.Append(L" Detected processing Catch-Block!");
                return;
            }
            ret.returnCode = 0;
            if (jsVar)
            {
                jsVar->variables.clear();
                TcVariableHolder holder;
                holder.mut = true;
                holder.value = ret.errorObject;
                if (!holder.value.Get())
                    holder.value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(ret.errorMessage);

                jsVar->variables.addEntry(e, holder);
                ret = jsVar->Run();
            }
        }

        if (statement->next.Get())
        {
            if (!ret.returnCode || statement->next->statementType == code_statement_type::cst_finally)
                ret = Run(statements, index, statement->next);
        }

        break;
    default: // Finally
        if (jsVar)
            jsVar->Run();
    }
}

void TcJavaScriptInterpretor::ProcessSwitch(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

bool TcJavaScriptInterpretor::IsTruthful(TrecPointer<TVariable> var)
{
    if (!var.Get())
        return false;

    switch (var->GetVarType())
    {
    case var_type::string:
        // If a String, then only the Empty String is considered Falsy
        return dynamic_cast<TStringVariable*>(var.Get())->GetSize() > 0;
    case var_type::primitive:
        // If a Primitive value, boolean false, 
        return var->Get8Value() > 0;
    }


    return true;
}
