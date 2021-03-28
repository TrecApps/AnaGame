#include "TcJavaScriptInterpretor.h"
#include <TSpecialVariable.h>
#include <TStringVariable.h>
#include <TContainerVariable.h>
#include <TPrimitiveVariable.h>
#include <TAccessorVariable.h>
#include <cassert>


static TDataArray<TString> standardOperators;

static TrecPointer<ObjectOperator> jsOperators;

static TrecPointer<TVariable> one;

bool IsEqual(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2, bool isEqual, bool castType)
{
    bool eqVal = false, eqType;

    if (var1.Get() && var2.Get())
    {
        DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(var1);
        DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(var2);

        eqType = dl1.type == dl2.type && var1->GetVarType() == var2->GetVarType() && var1->GetType();

        eqVal = dl1 == dl2;
    }
    else if (var1.Get())
    {
        eqType = false;
        DoubleLong dl = DoubleLong::GetValueFromPrimitive(var1);

        eqVal = dl.type == double_long::dl_invalid || dl == DoubleLong((LONG64)0);
    }
    else if (var2.Get())
    {
        eqType = false;
        DoubleLong dl = DoubleLong::GetValueFromPrimitive(var2);

        eqVal = dl.type == double_long::dl_invalid || dl == DoubleLong((LONG64)0);
    }
    else eqType = true;

    if (isEqual)
    {
        // Dealing with == or ===
        return eqVal && ((castType) ? true : eqType);
    }
    else
    {
        // Dealing with != or !==
        return (castType) ? !eqVal : (!eqVal && !eqType);
    }
}

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
        return ret;
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

    selfWord.Set(L"this");

    if (!jsOperators.Get())
        jsOperators = TrecPointerKey::GetNewTrecPointerAlt<ObjectOperator, JsObjectOperator>();

    if (!standardOperators.Size())
    {
        standardOperators.push_back(L">>>=");
        standardOperators.push_back(L"===");
        standardOperators.push_back(L"!==");
        standardOperators.push_back(L">>>");
        standardOperators.push_back(L"<<=");
        standardOperators.push_back(L">>=");
        standardOperators.push_back(L"&&=");
        standardOperators.push_back(L"||=");
        standardOperators.push_back(L"??=");
        standardOperators.push_back(L"**=");
        standardOperators.push_back(L"++");
        standardOperators.push_back(L"--");
        standardOperators.push_back(L"**");
        standardOperators.push_back(L"<<");
        standardOperators.push_back(L">>");
        standardOperators.push_back(L"<=");
        standardOperators.push_back(L">=");
        standardOperators.push_back(L"==");
        standardOperators.push_back(L"!=");
        standardOperators.push_back(L"&&");
        standardOperators.push_back(L"||");
        standardOperators.push_back(L"??");
        standardOperators.push_back(L"+=");
        standardOperators.push_back(L"-=");
        standardOperators.push_back(L"*=");
        standardOperators.push_back(L"/=");
        standardOperators.push_back(L"%=");
        standardOperators.push_back(L"&=");
        standardOperators.push_back(L"^=");
        standardOperators.push_back(L"|=");
    }

    if (!one.Get())
    {
        one = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<int>(1));
    }
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

ReturnObject TcJavaScriptInterpretor::Run(TDataArray<TrecPointer<CodeStatement>>& statements, UINT start)
{
    ReturnObject ret;
    for (UINT Rust = start; Rust < statements.Size(); Rust++)
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

    case code_statement_type::cst_for:
    case code_statement_type::cst_for_3:
        ProcessFor(statements, index, ret);

        break;
    case code_statement_type::cst_function:

        break;
    case code_statement_type::cst_if:
        ProcessBasicFlow(statement, ret);
        break;
    case code_statement_type::cst_regular:
        ProcessExpression(statement, ret);
        break;
    case code_statement_type::cst_return:
        ProcessExpression(statement, ret);
        if (!ret.returnCode)
            ret.mode = return_mode::rm_return;
        break;
    case code_statement_type::cst_switch:
        ProcessSwitch(statement, ret);
        break;
    case code_statement_type::cst_case:
    case code_statement_type::cst_default:
        if(statement->next.Get())
            ret = Run(statements, index, statement->next);
        break;
    case code_statement_type::cst_throw:
        ProcessExpression(statement, ret);
        if (!ret.returnCode)
            ret.returnCode = ReturnObject::ERR_THROWN;
        break;
    case code_statement_type::cst_try:
    case code_statement_type::cst_finally:
        ProcessTryCatchFinally(statements, index, statement, ret);
        break;
    case code_statement_type::cst_do:
    case code_statement_type::cst_while:
        ProcessWhile(statement, ret);
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

    if (startStatement.StartsWith(L"case", false, true))
    {
        state->statementType = code_statement_type::cst_case;

        auto parseState = state;
        while (parseState->statement.Find(L':') == -1 && parseState.Get())
            parseState = parseState->next;

        if (!parseState.Get())
        {
            ret.returnCode = ReturnObject::ERR_INCOMPLETE_STATEMENT;
            ret.errorMessage.Set("'case' statement does not contain a propert ':' terminater!");
            return;
        }

        int foundTok = parseState->statement.Find(L':');

        TString nextStatement(parseState->statement.SubString(foundTok + 1).GetTrim());

        if (nextStatement.GetSize())
        {
            // Cut the current Statement String short
            parseState->statement.Set(parseState->statement.SubString(0, foundTok));

            TrecPointer<CodeStatement> newState = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
            newState->statement.Set(nextStatement);
            newState->lineStart = parseState->lineStart + parseState->statement.CountFinds(L"\n");
            newState->lineEnd = newState->lineStart + nextStatement.CountFinds(L"\n");
            newState->block = parseState->block;
            parseState->block.RemoveAll();

            newState->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parseState);
            
            newState->next = parseState->next;
            parseState->next = newState;
        }

        PreProcess(ret, parseState->next);
        return;
    }

    if (startStatement.StartsWith(L"default:") || startStatement.StartsWith(L"default", false, true))
    {
        state->statementType = code_statement_type::cst_default;
        auto parseState = state;
        while (parseState->statement.Find(L':') == -1 && parseState.Get())
            parseState = parseState->next;

        if (!parseState.Get())
        {
            ret.returnCode = ReturnObject::ERR_INCOMPLETE_STATEMENT;
            ret.errorMessage.Set("'case' statement does not contain a propert ':' terminater!");
            return;
        }

        int foundTok = parseState->statement.Find(L':');

        TString nextStatement(parseState->statement.SubString(foundTok + 1).GetTrim());

        if (nextStatement.GetSize())
        {
            // Cut the current Statement String short
            parseState->statement.Set(parseState->statement.SubString(0, foundTok));

            TrecPointer<CodeStatement> newState = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
            newState->statement.Set(nextStatement);
            newState->lineStart = parseState->lineStart + parseState->statement.CountFinds(L"\n");
            newState->lineEnd = newState->lineStart + nextStatement.CountFinds(L"\n");
            newState->block = parseState->block;
            parseState->block.RemoveAll();

            newState->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parseState);

            newState->next = parseState->next;
            parseState->next = newState;
        }

        PreProcess(ret, parseState->next);
        return;
    }

    if (startStatement.StartsWith(L"else", false, true))
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
        state->statementType = code_statement_type::cst_for;
        state->statement.Delete(0, 3);
        state->statement.Trim();

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

UINT TcJavaScriptInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, TrecSubPointer<TVariable, TcJavaScriptInterpretor> in)
{
    UINT parenth = 0, square = 0, index = 0;
    TDataArray<JavaScriptExpression2> exp;
    TDataArray<TString> ops;
    TString statementString(statement->statement.SubString(statement->statement.Find(L'(') + 1));
    statementString.Trim();
    TrecPointer<TDataArray<TString>> tokens;

    TString retString;
    UINT returnable = 0;
    switch (statement->statementType)
    {
    case code_statement_type::cst_for:
        if (statementString.StartsWith(L"let", false, true) || statementString.StartsWith(L"var", false, true)
            || statementString.StartsWith(L"const", false, true))
        {
            tokens = statementString.splitn(L"\s\t\n\r", 4);
            retString.Set(tokens->at(0) + L'\s');
            tokens->RemoveAt(0);
        }
        else
            tokens = statementString.splitn(L"\s\t\n\r", 3);

        if (tokens->Size() < 3)
        {
            PrepReturn(ret, L"Incomplete For statement!", L"", ReturnObject::ERR_INCOMPLETE_STATEMENT, statement->lineStart);
            return returnable;
        }
        CheckVarName(tokens->at(0), ret);
        if (ret.returnCode)
            return returnable;
        retString.Append(tokens->at(0));
        
        if (tokens->at(1).Compare(L"of") && tokens->at(1).Compare(L"in"))
        {
            PrepReturn(ret, L"Expected 'in' or 'of' token in for statement!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
            return returnable;
        }

        parenth = 1;
        index = statement->statement.Find(tokens->at(1), statement->statement.Find(tokens->at(0)) + tokens->at(0).GetSize());
        returnable += ProcessExpression(parenth, square, index, statement, ret, exp, ops);

        if (!ret.returnCode)
            ret.errorMessage.Set(retString);
        break;
    case code_statement_type::cst_for_3:

        if (statementString.StartsWith(L"let", false, true))
        {
            index = 3;
            statement->statementType = code_statement_type::cst_let;
        }
        else if (statementString.StartsWith(L"var", false, true))
        {
            index = 3;
            statement->statementType = code_statement_type::cst_var;
        }
        else if (statementString.StartsWith(L"const", false, true))
        {
            index = 5;
            statement->statementType = code_statement_type::cst_const;
        }
        index += statement->statement.Find(L'(') + 1;

        switch (code_statement_type::cst_for_3)
        {
        case code_statement_type::cst_const:
        case code_statement_type::cst_var:
        case code_statement_type::cst_let:
            if (!in.Get())
            {
                PrepReturn(ret, L"INTERNAL ERROR! For Loop method should have proveded ProcessExpression Method with an interpretor!", L"", ReturnObject::ERR_INTERNAL, statement->lineStart);
                return returnable;
            }
            in->ProcessAssignmentStatement(statement, ret, index);
            break;
        default:
            returnable += ProcessExpression(parenth, square, index, statement, ret, exp, ops);
        }
        statement->statementType = code_statement_type::cst_for_3;

        break;
    default:
        returnable += ProcessExpression(parenth, square, index, statement, ret, exp, ops);
    }
    return returnable;
}

void TcJavaScriptInterpretor::ProcessAssignmentStatement(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT stringStart)
{
    // Attempt to divide the statement up by ',', taking into account strings, into sub-statements
    TDataArray<TString> subStatements;  // Holds the sub-Statements
    TString subStatement;               // The SubStatement in the moment

    WCHAR quote = L'\0', open = L'\0';
    UINT openStack = 0;

    for (UINT Rust = stringStart; Rust < statement->statement.GetSize(); Rust++)
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

        bool thisScope = true, makeMut = true;
        if (varName.StartsWith(L"var", false, true) || varName.StartsWith(L"let", false, true))
        {
            thisScope = false;
            varName.Delete(0, 3);
        }
        else if (varName.StartsWith(L"const", false, true))
        {
            thisScope = false;
            makeMut = false;
            varName.Delete(0, 5);
        }
        varName.Trim();

        for (UINT Rust = 0; container->GetValueAt(Rust, indexName, var); Rust++)
        {
            jsVar->variables.clear();
            TcVariableHolder holder;
            holder.mut = makeMut;
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
       auto jsVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcJavaScriptInterpretor>(block->statementVar);
        ProcessExpression(statement, ret, jsVar);
        if (ret.returnCode)
            return;

        

        // To-Do: After implementing ProcessExpression, figure out how to respond to non-errors



        // End To-Do: Now Run the Other two statements

        if (jsVar.Get())
            jsVar->ProcessExpression(statements[index + 1], ret);
        else
            ProcessExpression(statements[index + 1], ret);
        if (ret.returnCode)
            return;

        while (IsTruthful(ret.errorObject))
        {
            if(jsVar.Get())
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

            if (jsVar.Get())
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
    UINT next = ProcessExpression(statement, ret);
    if (ret.returnCode)
        return;

    while (statement->next.Get() && next)
    {
        statement = statement->next;
        next--;
    }

    TrecPointer<TVariable> switchExpress = ret.errorObject;

    // Right now, assume we do not have a default statement!
    int defaultStatement = -1;
    bool ran = false;
    for (UINT Rust = 0; Rust < statement->block.Size(); Rust++)
    {
        bool def = false;
        TrecPointer<CodeStatement> state = statement->block[Rust];
        if (state->statementType == code_statement_type::cst_default)
        {
            if(defaultStatement != -1)
            {
                PrepReturn(ret, L"Second Default statement detected in 'switch' statement!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return;
            }
            defaultStatement = Rust;
            continue;
        }
        if (state->statementType == code_statement_type::cst_default && ProcessCase(state, statement, switchExpress, def, ret))
        {
            ran = true;
            ret = Run(statement->block, Rust);
            break;
        }
        if (def)
        {
            if (defaultStatement == -1)
                defaultStatement = Rust;
            else
            {
                PrepReturn(ret, L"Second Default statement detected in 'switch' statement!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return;
            }
        }

        if (ret.returnCode)
            return;
    }

    if (!ran && defaultStatement > -1)
    {
        ret = Run(statement->block, defaultStatement);
    }
}

bool TcJavaScriptInterpretor::ProcessCase(TrecPointer<CodeStatement> caseStatement, TrecPointer<CodeStatement> switchStatement, TrecPointer<TVariable> var, bool& hadDefault, ReturnObject& ret)
{
    bool seeking = true;
    bool found = false;
    hadDefault = false;
    while (seeking)
    {
        if (caseStatement->statementType == code_statement_type::cst_default)
        {
            hadDefault = true;
        }
        else
        {
            UINT jumps = ProcessExpression(caseStatement, ret);
            if (ret.returnCode)
                return false;

            if (IsEqual(ret.errorObject, var, true, true))
            {
                found = true;
            }

            while (caseStatement->next.Get() && jumps)
            {
                jumps--;
                caseStatement = caseStatement->next;
            }
        }

        caseStatement = caseStatement->next;
        seeking = false;
        if (caseStatement.Get() && (caseStatement->statementType == code_statement_type::cst_case || caseStatement->statementType == code_statement_type::cst_default))
            seeking = true;
    }
    return found;
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

TrecPointer<TVariable> JsObjectOperator::Add(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
    TrecPointer<TVariable> ret = DefaultObjectOperator::Add(var1, var2);
    if (ret.Get())
        return ret;

    if (!var1.Get() && !var2.Get())
    {
        return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0ULL);
    }

    if (var1.Get() && var1->GetVarType() == var_type::string)
    {
        TString value(var1->GetString());
        value.Append(var2.Get() ? var2->GetString() : L"null");
        return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
    }
    else if (var2.Get() && var2->GetVarType() == var_type::string)
    {
        TString value(var1.Get() ? var1->GetString() : L"null");
        value.Append(var2->GetString());
        return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
    }

    if (var1.Get() && !var2.Get())
    {
        if (var1->GetVarType() == var_type::collection)
        {
            auto varColl = dynamic_cast<TContainerVariable*>(var1.Get());
            TString value;
            for (UINT Rust = 0; Rust < varColl->GetSize(); Rust++)
            {
                auto val = varColl->GetValueAt(Rust);
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer() : L"null");
            }
            if (value.StartsWith(L","))
                value.Delete(0);
            value.Append(L"null");
            return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
        }
        if (var1->GetVarType() == var_type::primitive)
        {
            return var1;
        }
    }

    if (var2.Get() && !var1.Get())
    {
        if (var2->GetVarType() == var_type::collection)
        {
            auto varColl = dynamic_cast<TContainerVariable*>(var2.Get());
            TString value;
            for (UINT Rust = 0; Rust < varColl->GetSize(); Rust++)
            {
                auto val = varColl->GetValueAt(Rust);
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer() : L"null");
            }
            if (value.StartsWith(L","))
                value.Delete(0);
            value.Set(TString(L"null") + value);
            return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
        }
        if (var2->GetVarType() == var_type::primitive)
        {
            return var2;
        }
    }
    return ret;
}

JavaScriptExpression2::JavaScriptExpression2()
{
}

JavaScriptExpression2::JavaScriptExpression2(const JavaScriptExpression2& orig)
{
    varName.Set(orig.varName);
    value = orig.value;
}

JavaScriptExpression2::JavaScriptExpression2(const TString& name, TrecPointer<TVariable> value)
{
    varName.Set(name);
    this->value = value;
}


void JavaScriptExpression2::operator=(const JavaScriptExpression2& orig)
{
    varName.Set(orig.varName);
    value = orig.value;
}

UINT TcJavaScriptInterpretor::ProcessExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{
    bool processed = false; // False, we are looking for an expression, true, look for operator or end
    UINT nodes = 0, fullNodes = 0; // Track how many nodes have been traversed during processing
    for (; index < statement->statement.GetSize(); index++)
    {
        bool processed = false; // Did we process some expression upon hitting a certain character
        WCHAR ch = statement->statement[index];
        switch (ch)
        {
        case L'(':
            if (processed)
            {
                PrepReturn(ret, L"Unepected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            parenth++;
            nodes = ProcessExpression(parenth, square, ++index, statement, ret, expressions, ops);
            break;
        case L'[':
            if (processed)
            {
                PrepReturn(ret, L"Unepected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            square++;
            nodes = ProcessArrayExpression(parenth, square, ++index, statement, ret, expressions, ops);
            break;
        case L'\'':
        case L'\"':
        case L'`':
            if (processed)
            {
                PrepReturn(ret, L"Unepected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            nodes = ProcessStringExpression(parenth, square, index, statement, ret, expressions, ops);
            break;
        case L'.':
        case L'0':
        case L'1':
        case L'2':
        case L'3':
        case L'4':
        case L'5':
        case L'6':
        case L'7':
        case L'8':
        case L'9':
            if (processed)
            {
                TString message;
                message.Format(L"Unexpected '%c' character detected", ch);
                PrepReturn(ret, message, L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            nodes = ProcessNumberExpression(parenth, square, index, statement, ret, expressions, ops);

        }


        if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || ch == L'_')
        {
            if (processed)
            {
                TString message;
                message.Format(L"Unexpected '%c' character detected", ch);
                PrepReturn(ret, message, L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            nodes = ProcessVariableExpression(parenth, square, index, statement, ret, expressions, ops);
        }

        if (processed)
        {
            if (ret.returnCode)
                return fullNodes;
            JavaScriptExpression2 exp;
            exp.value = ret.errorObject;
            ret.errorObject.Nullify();
            exp.varName.Set(ret.errorMessage);
            expressions.push_back(exp);
            fullNodes += nodes;
            if (nodes && !statement->next.Get())
            {
                break;
            }

            while (nodes && statement->next.Get())
            {
                statement = statement->next;
                nodes--;
            }
        }
        TString exp(statement->statement.SubString(index));

        // Handle Post-Increment/Decrement
        if (expressions.Size() && expressions[expressions.Size() - 1].varName.GetSize() && ((exp.StartsWith(L"++") || exp.StartsWith(L"--"))))
        {
            index += 2;
            bool inc = exp.StartsWith(L"++");

            JavaScriptExpression2 jExp(expressions[expressions.Size() - 1]);

            jExp.value = (jExp.value.Get()) ? jExp.value->Clone() : jExp.value;


            // Now handle the increment or decrement step
            bool present;
            TrecPointer<TVariable> tempValue = GetVariable(jExp.varName, present);

            if (!tempValue.Get())
            {
                ret.returnCode = ret.ERR_BROKEN_REF;

                ret.errorMessage.Format(L"Post-%ws attempt encountered %ws variable!",
                    inc ? L"Increment" : L"Decrement",
                    present ? L"Null" : L"Default");
                return fullNodes;
            }

            auto result = inc ? jsOperators->  Add(tempValue, one) :
                jsOperators->Subtract(tempValue, one);

            if (!result.Get())
            {
                TString message;
                message.Format(L"Invalid type found for variable '%ws' in pre-%ws operation!", 
                    jExp.varName.GetConstantBuffer(), inc ? L"increment" : L"decrement");
                PrepReturn(ret, message, L"", ReturnObject::ERR_IMPROPER_TYPE, statement->lineStart);
                return fullNodes;
            }
            UINT updateResult = UpdateVariable(jExp.varName, ret.errorObject);

            ret.errorObject.Nullify();
            if (updateResult == 1)
            {
                ret.returnCode = ret.ERR_BROKEN_REF;
                ret.errorMessage.Format(L"Post-%ws attempt encountered %ws variable!",
                    inc ? L"Increment" : L"Decrement",
                    present ? L"Null" : L"Default");
                return fullNodes;
            }
            if (updateResult == 2)
            {
                ret.returnCode = ret.ERR_IMPROPER_NAME;
                ret.errorMessage.Format(L"Post-%ws attempt to update immutable variable!",
                    inc ? L"Increment" : L"Decrement");
                return fullNodes;
            }

            exp.Delete(0, 2);
            exp.Trim();
        }

        bool foundOp = false;
        for (UINT Rust = 0; Rust < standardOperators.Size(); Rust++)
        {
            if (exp.StartsWith(standardOperators[Rust]))
            {
                ops.push_back(standardOperators[Rust]);
                foundOp = true;
                exp.Delete(0, standardOperators[Rust].GetSize());
                break;
            }
        }

        if (!foundOp)
        {
            if (!exp.FindOneOf(L"+-*/%^&|<>?=~!,"))
            {
                ops.push_back(TString(exp[0]));
                exp.Delete(0, 1);
                foundOp = true;
            }
        }

        if (foundOp)
        {
            processed = false;
            continue;
        }

        switch (ch)
        {
        case L']':
            if (!square)
            {
                PrepReturn(ret, L"Unexpected ']' token detected!", L"", ReturnObject::ERR_PARENTH_MISMATCH, statement->lineStart);
                return fullNodes;
            }
            square--;
            index++;
            goto crunch;
        case L')':
            if(!parenth)
                if (!square)
                {
                    PrepReturn(ret, L"Unexpected ')' token detected!", L"", ReturnObject::ERR_PARENTH_MISMATCH, statement->lineStart);
                    return fullNodes;
                }
            square--;
        case L',':
        case L';':
            index++;
            goto crunch;
        }
        
    }

    if (index == statement->statement.GetSize() && statement->block.Size())
    {
        ProcessJsonExpression(parenth, square, index, statement, ret, expressions, ops);
        if (ret.returnCode)
            return fullNodes;
        JavaScriptExpression2 exp;
        exp.value = ret.errorObject;
        ret.errorObject.Nullify();
        exp.varName.Set(ret.errorMessage);
        expressions.push_back(exp);
        if (statement->next.Get())
        {
            index = 0;
            nodes = ProcessExpression(parenth, square, index, statement->next, ret, expressions, ops);
            if (ret.returnCode)
                return fullNodes;
            fullNodes += nodes;
            while (nodes && statement->next.Get())
            {
                statement = statement->next;
                nodes--;
            }
            if (index)
            {
                int e = 0;
            }
        }
    }

crunch:

    HandlePreExpr(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleExponents(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleMultDiv(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleAddSub(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleBitwiseShift(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleLogicalComparison(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleEquality(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleBitwiseAnd(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleBitwiseXor(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleBitwiseOr(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleLogicalAnd(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleLogicalOr(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleNullish(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleConditional(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleAssignment(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    HandleComma(expressions, ops, ret);
    if (ret.returnCode) return fullNodes;

    if (expressions.Size() == 1)
        ret.errorObject = expressions[0].value;

    return fullNodes;
}

UINT TcJavaScriptInterpretor::ProcessArrayExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{
    TrecSubPointer<TVariable, TContainerVariable> arrayVar = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

    if (statement->statement[index] == L'[')
        index++;

    bool processArray = true;
    UINT curParenth = parenth;
    UINT curSquare = square;
    UINT nodeRet = 0;
    UINT nodes = 0;
    while (square >= curSquare)
    {
        nodes = ProcessExpression(parenth, square, index, statement, ret, expressions, ops);
        if (ret.returnCode)
            return nodeRet;

        if (curParenth != parenth)
        {
            PrepReturn(ret, L"Parenthesis mismatch in array expression!", L"", ReturnObject::ERR_PARENTH_MISMATCH, statement->lineStart);
            return nodeRet;
        }

        arrayVar->AppendValue(ret.errorObject);
        ret.errorObject.Nullify();
        nodeRet += nodes;
        while (nodes && statement->next.Get())
        {
            nodes--;
            statement = statement->next;
        }
    }

    ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(arrayVar);

    return nodeRet;
}

UINT TcJavaScriptInterpretor::ProcessJsonExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{

    return 0;
}

UINT TcJavaScriptInterpretor::ProcessVariableExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{
    TDataArray<TString> pieces;

    TString phrase, wholePhrase;
    bool spaceDetected = false;
    UINT uret = 0;

    UCHAR attribute = 0;
    bool loopAround;

    TrecPointer<TVariable> var, vThis;

throughString:
    loopAround = false;
    for (; index < statement->statement.GetSize(); index++)
    {
        WCHAR ch = statement->statement[index];

        if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || ch == L'_')
        {
            if (spaceDetected)
            {
                if (!phrase.Compare(L"new"))
                    attribute = 1;
                else if (!phrase.Compare(L"get"))
                    attribute = 2;
                else if (!phrase.Compare(L"set"))
                    attribute = 3;
                else if (!phrase.Compare(L"function"))
                    attribute = 4;
                else
                {
                    TString message;
                    message.Format(L"Unexpected Expression after '%ws' token detected!", phrase.GetConstantBuffer());
                    PrepReturn(ret, message, L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                    return uret;
                }
                phrase.Empty();
                spaceDetected = false;
            }

            phrase.AppendChar(ch);
            continue;
        }

        if (ch == L'\s')
        {
            if (phrase.GetSize())
                spaceDetected = true;
            continue;
        }


        break;
    }

    if (phrase.GetSize())
    {
        if (!phrase.Compare(L"null"))
            var.Nullify();
        else if (!phrase.Compare(L"undefined"))
            var = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
        else if (!phrase.Compare(L"this"))
            var = methodObject;
        else if (var.Get())
        {
            if (var->GetVarType() == var_type::collection)
            {
                vThis = var;
                bool pres;
                var = dynamic_cast<TContainerVariable*>(var.Get())->GetValue(phrase, pres);
                if (!pres)
                {
                    var = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
                }
            }
        }
        else
        {
            bool pres;
            var = GetVariable(phrase, pres);
        }
        wholePhrase.Append(phrase);
        phrase.Empty();
    }

    if (index < statement->statement.GetSize())
    {
        WCHAR ch = statement->statement[index++];
        if (ch == L'(' && var.Get())
        {
            wholePhrase.AppendChar(ch);
            parenth++;

            UINT curIndex = index;
            if (var->GetVarType() == var_type::interpretor)
            {
                UINT curRet = uret;

                if (attribute == 1)// calling new
                {
                    vThis = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TContainerVariable>(ContainerType::ct_array);
                }
                uret += ProcessProcedureCall(parenth, square, index, vThis, var, statement, ret, expressions, ops);

                if (ret.returnCode)
                    return uret;

                if (uret > curRet)
                {
                    wholePhrase.Append(statement->statement.SubString(curIndex) + L"{...}");
                    while (statement->next.Get() && uret > curRet)
                    {
                        statement = statement->next;
                        curRet++;
                    }
                    wholePhrase.Append(statement->statement.SubString(0, index + 1));
                }
                var = ret.errorObject;
            }
            else if (var->GetVarType() == var_type::accessor)
            {
                auto getter = dynamic_cast<TAccessorVariable*>(var.Get())->GetGetter();
                if (getter.Get())
                {
                    auto oRet = getter->Run();
                    if (oRet.returnCode)
                    {
                        // To-Do: Replace Getters and Setters with Interpretors
                        ret.returnCode = oRet.returnCode;
                        ret.errorMessage.Set(oRet.errorMessage);
                        return 0;
                    }
                    var = oRet.errorObject;
                    int iIndex = statement->statement.Find(L')', index);

                    if (iIndex == -1)
                    {
                        TString message;
                        message.Format(L"Incomplete getter call detected on variable expression '%ws'", wholePhrase.GetConstantBuffer());
                        PrepReturn(ret, message, L"", ReturnObject::ERR_INCOMPLETE_STATEMENT, statement->lineStart);
                        return 0;
                    }
                    index = iIndex;
                }
            }
            else
                var = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
            spaceDetected = false;
            loopAround = true;

            wholePhrase.AppendChar(L')');
        }
        else if (ch == L'(')
        {
            if (attribute == 1)
            {
                // Expect to deal with a type
                
            }

            // Likely dealing with function defintion
            ret.errorObject.Nullify();
            UINT jumps = ProcessFunctionExpression(parenth, square, index, statement, ret, expressions, ops);
            if (ret.returnCode)
                return 0;
            if (ret.errorObject.Get())
            {
                bool worked = true;
                switch (attribute)
                {
                case 1: // new

                    break;
                case 2: // get
                case 3: // set
                    wholePhrase.Delete(0, 3);
                    wholePhrase.Trim();
                    if (this->methodObject.Get())
                    {
                        if (methodObject->GetVarType() == var_type::collection)
                        {
                            auto coll = dynamic_cast<TContainerVariable*>(methodObject.Get());
                            bool accessPres = false;
                            auto access = coll->GetValue(wholePhrase, accessPres);
                            if (!access.Get())
                            {
                                access = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAccessorVariable>();
                                coll->SetValue(wholePhrase, access);
                            }
                            else if (access->GetVarType() != var_type::accessor)
                            {
                                TString message;
                                message.Format(L"Cannot assign getter/setter to attribute %ws since it is already assigned and not an accessor!", wholePhrase.GetConstantBuffer());
                                PrepReturn(ret, message, L"", ReturnObject::ERR_IMPROPER_TYPE, statement->lineStart);
                            }
                            auto tAccess = dynamic_cast<TAccessorVariable*>(access.Get());
                            //if (attribute == 2)
                            //    tAccess->SetGetter(ret.errorObject);
                            //else
                            //    tAccess->SetSetter(ret.errorObject);
                        }
                        else if (methodObject->GetVarType() == var_type::accessor)
                        {
                            auto tAccess = dynamic_cast<TAccessorVariable*>(methodObject.Get());
                            //if (attribute == 2)
                            //    tAccess->SetGetter(ret.errorObject);
                            //else
                            //    tAccess->SetSetter(ret.errorObject);
                        }
                        else
                        {
                            // To-Do: Handle Error
                        }
                    }
                    else
                    {
                        // To-Do: Handle Error!
                    }
                
                    break;
                case 4: // Function
                    wholePhrase.Delete(0, 8);
                    wholePhrase.Trim();

                }
                var = ret.errorObject;
                loopAround = false;
            }
        }


        if (ch == L'.' || ch == L'?')
        {
            loopAround = true;
            spaceDetected = false;
            wholePhrase.AppendChar(ch);
        }
    }

    if (loopAround)
        goto throughString;

    ret.errorMessage.Set(wholePhrase);
    ret.errorObject = var;


    return 0;
}

UINT TcJavaScriptInterpretor::ProcessFunctionExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{
    int startP = statement->statement.Find(L'(', index), endP = statement->statement.Find(L')');

    if (startP == -1 || endP < startP)
    {
        ret.errorObject.Nullify();
        ret.errorMessage.Empty();
        return 0;
    }
    TString paramNames(statement->statement.SubString(startP + 1, endP));
    TDataArray<TString> params;

    paramNames.Trim();
    if (paramNames.GetSize())
    {
        auto paramPieces = paramNames.split(L',');
        for (UINT Rust = 0; Rust < paramPieces->Size(); Rust++)
        {
            TString iParam(paramPieces->at(Rust));
            iParam.Trim();

            //auto paramPieces = iParam.splitn(L'=', 2);

            CheckVarName(iParam, ret);
            if (ret.returnCode)
            {
                ret.returnCode = 0;
                ret.errorObject.Nullify();
                ret.errorMessage.Empty();
                return 0;
            }
            params.push_back(iParam);
        }
    }
    TrecSubPointer<TVariable, TcInterpretor> updatedSelf = TrecPointerKey::GetSubPointerFromSoft<>(self);
    auto function = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcJavaScriptInterpretor>(updatedSelf, environment);

    function->SetParamNames(params);
    function->SetStatements(statement->block);

    return 0;
}

UINT TcJavaScriptInterpretor::ProcessNumberExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{
    UINT start = index;
    for (; index < statement->statement.GetSize(); index++)
    {
        switch (statement->statement[index])
        {
        case L'\s':
        case L'\n':
        case L'\r':
        case L'\t':
        case L',':
        case L';':
            goto broken;
        }
    }
    broken:
    TString numStr(statement->statement.SubString(start, index));
    double d = 0.0;
    LONG64 s = 0;
    UINT u = 0;
    if (!numStr.ConvertToDouble(d))
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(d);
    else if (!numStr.ConvertToLong(s))
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(s);
    else if (TString::ConvertStringToUint(numStr, u))
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(u);
    else
    {
        TString message;
        message.Format(L"Failed to convert token '%ws' to a number!", numStr.GetConstantBuffer());
        PrepReturn(ret, message, L"", ReturnObject::ERR_NOT_NUMBER, statement->lineStart);
    }
    return 0;
}

UINT TcJavaScriptInterpretor::ProcessStringExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{
    WCHAR quote = statement->statement[index];
    bool istemplated = false;
    int quoteLoc = -1;
    switch (quote)
    {
    case L'`':
        istemplated = true;
    case L'\'':
    case L'\"':
        quoteLoc = statement->statement.Find(quote, index + 1, false);
        if (quoteLoc == -1)
        {
            PrepReturn(ret, L"Incomplete String detected!", L"", ReturnObject::ERR_INCOMPLETE_STATEMENT, statement->lineStart);
            return 0;
        }
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(statement->statement.SubString(index + 1, quoteLoc));
        break;
    default:
        PrepReturn(ret, L"INTERNAL ERROR! Expected Quote character!", L"", ReturnObject::ERR_INTERNAL, statement->lineStart);
    }
    return 0;
}

UINT TcJavaScriptInterpretor::ProcessProcedureCall(UINT& parenth, UINT& square, UINT& index, TrecPointer<TVariable> object, TrecPointer<TVariable> proc, TrecPointer<CodeStatement> statement, 
    ReturnObject& ret, TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops)
{
    UINT curParenth = parenth;
    UINT nextRet = 0;

    TDataArray<TrecPointer<TVariable>> params;

    while (parenth >= curParenth)
    {
        // First check to see if the net token is a close parenthesis
        TString left(statement->statement.SubString(index));
        left.Trim();

        if (left.StartsWith(L')'))
        {
            index = statement->statement.Find(L')', index) + 1;
            parenth--;
            continue;
        }

        // Get the Parameter
        UINT uret = ProcessExpression(parenth, square, index, statement, ret, expressions, ops);
        nextRet += uret;

        // If an error was detected, return
        if (ret.returnCode)
            return nextRet;

        // If blocks were detected, go through and retrieve the current statement we are on
        while (uret && statement->next.Get())
        {
            uret--;
            statement = statement->next;
        }
        // Add the parameter
        params.push_back(ret.errorObject);
    }

    // Convert to a function we can call
    TrecSubPointer<TVariable, TcInterpretor> function = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(proc);
    assert(function.Get());

    // This is a method if object is valid
    function->SetActiveObject(object);
    // Set the parameters
    function->SetIntialVariables(params);

    // Run the function and get the results
    ret = function->Run();

    // Return how many statments were jumped
    return nextRet;
}

void TcJavaScriptInterpretor::HandlePreExpr(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ro)
{
    for (UINT Rust = 0; Rust < operators.Size(); Rust++)
    {
        if (Rust >= expresions.Size())
        {
            PrepReturn(ro, L"Too many operators for the expression List!", L"", ReturnObject::ERR_TOO_FEW_PARMS, -1);
            return;
        }


        bool remove = false;
        if (!operators[Rust].Compare(L"++"))
        {
            TrecPointer<TVariable> var = expresions[Rust].value;
            remove = true;
            // To-Do: Add 1 to var
            ro.errorObject = jsOperators->Add(var, one);
            if (!ro.errorObject.Get())
            {
                PrepReturn(ro, L"Invalid Types detected for Addition Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            expresions[Rust].value = ro.errorObject;
        }
        else if (!operators[Rust].Compare(L"--"))
        {
            TrecPointer<TVariable> var = expresions[Rust].value;
            remove = true;
            // To-Do: Subtract 1 from var
            ro.errorObject = jsOperators->Subtract(var, one);
            if (!ro.errorObject.Get())
            {
                PrepReturn(ro, L"Invalid Types detected for Subtraction Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            expresions[Rust].value = ro.errorObject;
        }

        if (remove)
        {
            if (expresions[Rust].varName.GetSize())
            {
                this->UpdateVariable(expresions[Rust].varName, expresions[Rust].value);
            }
            operators.RemoveAt(Rust--);
        }

    }
}

void TcJavaScriptInterpretor::HandleExponents(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ro)
{
    if (expresions.Size() != operators.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Exponent handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    if (operators.Size())
    {
        // JavaScript Processes exponents from right to left, so make sure that operators are available and 
        // Start towars the end instead of at 0
        for (int Rust = static_cast<int>(operators.Size()) - 1; Rust >= 0; Rust--)
        {
            if (!operators[Rust].Compare(L"**"))
            {
                ro.errorObject = jsOperators->Pow(expresions[Rust].value, expresions[Rust + 1].value);
                if (!ro.errorObject.Get())
                {
                    PrepReturn(ro, L"Invalid Types detected for Exponent Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                    return;
                }

                expresions[Rust].value = ro.errorObject;
                expresions.RemoveAt(Rust + 1);
                operators.RemoveAt(Rust--);
            }
        }
    }
}

void TcJavaScriptInterpretor::HandleMultDiv(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Multiplication-Division handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].CompareNoCase(L"*"))
        {
            ro.errorObject = jsOperators->Multiply(expressions[Rust].value, expressions[Rust + 1].value);
            if (!ro.errorObject.Get())
            {
                PrepReturn(ro, L"Invalid Types detected for Multiplication Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }

            expressions[Rust].value = ro.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"/"))
        {
            ro.errorObject = jsOperators->Divide(expressions[Rust].value, expressions[Rust + 1].value);
            if (!ro.errorObject.Get())
            {
                PrepReturn(ro, L"Invalid Types detected for Division Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }

            expressions[Rust].value = ro.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"%"))
        {
            ro.errorObject = jsOperators->Mod(expressions[Rust].value, expressions[Rust + 1].value);
            if (!ro.errorObject.Get())
            {
                PrepReturn(ro, L"Invalid Types detected for Mod-Division Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }

            expressions[Rust].value = ro.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }
    }
}

void TcJavaScriptInterpretor::HandleAddSub(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ret)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ret, L"The JS-addition/subtraction handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].CompareNoCase(L"+"))
        {
            ret.errorObject = jsOperators->Add(expressions[Rust].value, expressions[Rust + 1].value);
            if (!ret.errorObject.Get())
            {
                PrepReturn(ret, L"Invalid Types detected for Addition Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }

            expressions[Rust].value = ret.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"-"))
        {
            ret.errorObject = jsOperators->Subtract(expressions[Rust].value, expressions[Rust + 1].value);
            if (!ret.errorObject.Get())
            {
                PrepReturn(ret, L"Invalid Types detected for Subtraction Operation", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            expressions[Rust].value = ret.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }
    }
}

void TcJavaScriptInterpretor::HandleBitwiseShift(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ret)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ret, L"The JS-Bitwise handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool canDo = true;

        TrecSubPointer<TVariable, TPrimitiveVariable> operand =
            TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(expressions[Rust].value);
        if (canDo = (operand.Get() != nullptr))
        {
            canDo = expressions[Rust + 1].value.Get() != nullptr;
        }
        UINT shift = 0;
        if (canDo)
        {
            if (expressions[Rust + 1].value->GetVarType() == var_type::primitive)
                shift = expressions[Rust + 1].value->Get4Value();
            else if (expressions[Rust + 1].value->GetVarType() == var_type::string)
            {
                int iShift;
                if (expressions[Rust + 1].value->GetString().ConvertToInt(iShift))
                    canDo = false;
                else
                    shift = iShift;
            }
        }

        bool doBit = false;
        bool doRightBit = false;
        USHORT flag = 0;

        if (!ops[Rust].Compare(L">>"))
        {
            if (!canDo)
            {
                PrepReturn(ret, L"Cannot perform >> operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }
            doBit = doRightBit = true;
            flag = TPrimitiveVariable::bit_float;
        }
        else if (!ops[Rust].Compare(L">>>"))
        {
            if (!canDo)
            {
                PrepReturn(ret, L"Cannot perform >>> operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }
            doBit = doRightBit = true;
            flag = TPrimitiveVariable::bit_float | TPrimitiveVariable::bit_replenish | TPrimitiveVariable::bit_to_32 | TPrimitiveVariable::bit_to_un_f;
        }
        else if (!ops[Rust].Compare(L"<<"))
        {
            if (!canDo)
            {
                PrepReturn(ret, L"Cannot perform << operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }
            doBit = true;
            flag = TPrimitiveVariable::bit_float;
        }

        if (doBit)
        {
            if (operand->BitShift(doRightBit, shift, flag))
            {
                expressions.RemoveAt(Rust + 1);
                ops.RemoveAt(Rust--);
            }
            else
            {
                PrepReturn(ret, L"Internal Error Attempting to Perform Bitwise manipulation!", L"", ReturnObject::ERR_INTERNAL, -1);
            }
        }
    }
}

void TcJavaScriptInterpretor::HandleLogicalComparison(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-logical comparison handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        auto left = expressions[Rust].value;
        auto right = expressions[Rust + 1].value;

        bool found = false;
        bool val;
        if (!ops[Rust].Compare(L"in"))
        {
            found = true;
            if (!left.Get() || !right.Get() || left->GetVarType() != var_type::string || right->GetVarType() != var_type::collection)
            {
                val = false;
            }
            else
            {

                dynamic_cast<TContainerVariable*>(right.Get())->GetValue(left->GetString(), val, L"__proto__");

            }

        }
        else if (!ops[Rust].Compare(L"instanceof"))
        {
            // To-Do: Implement once type management if implemented for this interpretor to use
        }
        else
        {
            UCHAR larger = 0;
            if (!left.Get() || !right.Get())
            {
                larger = 255;
                val = false;
            }

            if (!larger)
            {
                if (left->GetVarType() == var_type::string && right->GetVarType() == var_type::string)
                {
                    int i = left->GetString().Compare(right->GetString());

                    if (i > 0)
                        larger = 2;
                    else if (i < 0)
                        larger = 1;
                }
                else
                {
                    DoubleLong leftDl = DoubleLong::GetValueFromPrimitive(left);
                    DoubleLong rightDl = DoubleLong::GetValueFromPrimitive(right);

                    if (leftDl.type == double_long::dl_invalid || rightDl.type == double_long::dl_invalid)
                    {
                        larger = 255;
                        val = false;
                    }
                    else
                    {
                        if (leftDl > rightDl)
                            larger = 2;
                        else if (leftDl < rightDl)
                            larger = 1;
                    }
                }
            }

            if (!ops[Rust].Compare(L"<"))
            {
                found = true;
                val = larger == 1;
            }
            else if (!ops[Rust].Compare(L"<="))
            {
                found = true;
                val = (!larger || larger == 1);
            }
            else if (!ops[Rust].Compare(L">"))
            {
                found = true;
                val = larger == 2;
            }
            else if (!ops[Rust].Compare(L">="))
            {
                found = true;
                val = (!larger || larger == 2);
            }
        }

        if (found)
        {
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(val);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleEquality(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Equality handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool found = false, result;
        auto left = expressions[Rust].value;
        auto right = expressions[Rust + 1].value;
        if (!ops[Rust].Compare(L"==="))
        {
            found = true;
            result = IsEqual(left, right, true, false);
        }
        else if (!ops[Rust].Compare(L"=="))
        {
            found = true;
            result = IsEqual(left, right, true, true);
        }
        else if (!ops[Rust].Compare(L"!=="))
        {
            found = true;
            result = IsEqual(left, right, false, false);
        }
        else if (!ops[Rust].Compare(L"!="))
        {
            found = true;
            result = IsEqual(left, right, false, true);
        }

        if (found)
        {
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(result);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleBitwiseAnd(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-bitwise-and handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"&"))
        {
            DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(expressions[Rust].value);
            DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(expressions[Rust + 1].value);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                PrepReturn(ro, L"Invalid Types detected for & operation!", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitAnd(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleBitwiseXor(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-bitwise-xor handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"^"))
        {
            DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(expressions[Rust].value);
            DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(expressions[Rust + 1].value);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                PrepReturn(ro, L"Invalid Types detected for ^ operation!", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitXor(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleBitwiseOr(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-bitwise-or handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"|"))
        {
            DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(expressions[Rust].value);
            DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(expressions[Rust + 1].value);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                PrepReturn(ro, L"Invalid Types detected for | operation!", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitOr(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleLogicalAnd(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-logical-and handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"&&"))
        {
            expressions[Rust] = IsTruthful(expressions[Rust].value) ? expressions[Rust + 1] : expressions[Rust];
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleLogicalOr(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-logical-or handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"||"))
        {
            expressions[Rust] = IsTruthful(expressions[Rust].value) ? expressions[Rust] : expressions[Rust + 1];
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleNullish(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Nullish handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        ro.errorMessage.Set(L"The JS-Nullish handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"??"))
        {
            expressions[Rust] = expressions[Rust].value.Get() ? expressions[Rust] : expressions[Rust + 1];
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleConditional(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Conditional handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"?"))
        {
            if (Rust + 1 == ops.Size() || ops[Rust + 1].Compare(L":"))
            {
                PrepReturn(ro, L"Invalid Types detected for ? operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }


            expressions[Rust] = IsTruthful(expressions[Rust].value) ? expressions[Rust + 1] : expressions[Rust + 2];
            expressions.RemoveAt(Rust + 1);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust);
            ops.RemoveAt(Rust--);
        }
    }
}

void TcJavaScriptInterpretor::HandleAssignment(TDataArray<JavaScriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Assignment handler expected one more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (int Rust = ops.Size() - 1; Rust > -1; Rust--)
    {
        // Whether or not an assignment operator was found
        bool found = false;

        // Details on whether a bitwise shift assignment operator was found
        bool doBit = false;
        bool doRightBit = false;
        UINT shift = 0;
        USHORT flag = 0;

        auto left = expressions[Rust].value;
        auto right = expressions[Rust + 1].value;

        bool canDo = true, possibleProto = false;

        TrecSubPointer<TVariable, TPrimitiveVariable> operand =
            TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(left);
        if (canDo = (operand.Get() != nullptr))
        {
            canDo = right.Get() != nullptr;
        }
        if (canDo)
        {
            if (right->GetVarType() == var_type::primitive)
                shift = right->Get4Value();
            else if (right->GetVarType() == var_type::string)
            {
                int iShift;
                if (right->GetString().ConvertToInt(iShift))
                    canDo = false;
                else
                    shift = iShift;
            }
        }

        if (!ops[Rust].Compare(L"="))
        {
            found = true;
            ro.errorObject = right;

            // Check for a Prototype call
           /* auto pieces = expressions[Rust].varName.split(L'.', 3);

            if (pieces->Size() == 3 && !pieces->at(1).Compare(L"prototype"))
            {
                ro = ProcessPrototypeOperation(pieces->at(0), pieces->at(2), left, JS_Prototype_Op::jpo_add_const);
                return;
            }*/
        }
        else if (!ops[Rust].Compare(L"+="))
        {
            found = true;
            left = jsOperators->Add(left, right);
        }
        else if (!ops[Rust].Compare(L"-="))
        {
            found = true;
            left = jsOperators->Subtract(left, right);
        }
        else if (!ops[Rust].Compare(L"**="))
        {
            found = true;
            left = jsOperators->Pow(left, right);
        }
        else if (!ops[Rust].Compare(L"*="))
        {
            left = jsOperators->Multiply(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"/="))
        {
            left = jsOperators->Divide(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"%="))
        {
            left = jsOperators->Mod(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"<<="))
        {
            found = true;
            if (!canDo)
            {
                PrepReturn(ro, L"Could not pull off <<= operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }
            doBit = true;
            flag = TPrimitiveVariable::bit_float;
        }
        else if (!ops[Rust].Compare(L">>="))
        {
            found = true;
            if (!canDo)
            {
                PrepReturn(ro, L"Could not pull off >>= operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }
            doBit = doRightBit = true;
            flag = TPrimitiveVariable::bit_float;
        }
        else if (!ops[Rust].Compare(L">>>="))
        {
            found = true;
            if (!canDo)
            {
                PrepReturn(ro, L"COuld not pull off >>>= operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }
            doBit = doRightBit = true;
            flag = TPrimitiveVariable::bit_float | TPrimitiveVariable::bit_replenish | TPrimitiveVariable::bit_to_32 | TPrimitiveVariable::bit_to_un_f;
        }
        else if (!ops[Rust].Compare(L"&="))
        {
            found = true;
            DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(left);
            DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(right);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                PrepReturn(ro, L"Invalid type detected for &= operator!", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitAnd(dl2));
        }
        else if (!ops[Rust].Compare(L"^="))
        {
            found = true;
            DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(left);
            DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(right);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                PrepReturn(ro, L"Invalid type detected for ^= operator!", L"", ReturnObject::ERR_IMPROPER_TYPE, -1);
                return;
            }
            ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitXor(dl2));
        }
        else if (!ops[Rust].Compare(L"|="))
        {
            found = true;
            DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(left);
            DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(right);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                PrepReturn(ro, L"", L"", ReturnObject::ERR_BROKEN_REF, -1);
                return;
            }
            ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitOr(dl2));
        }
        else if (!ops[Rust].Compare(L"&&="))
        {
            found = true;
            ro.errorObject = IsTruthful(left) ? right : left;
        }
        else if (!ops[Rust].Compare(L"||="))
        {
            found = true;
            ro.errorObject = IsTruthful(left) ? left : right;
        }
        else if (!ops[Rust].Compare(L"??="))
        {
            found = true;
            ro.errorObject = left.Get() ? left : right;
        }

        if (found)
        {
            if (!expressions[Rust].varName.GetSize())
            {
                PrepReturn(ro, L"", L"", ReturnObject::ERR_BROKEN_REF, -1);
                ro.errorMessage.Format(L"Error! %ws operator must have a named variable as the left hand expression!", ops[Rust].GetConstantBuffer());
                return;
            }
            if (doBit)
            {
                if (!operand->BitShift(doRightBit, shift, flag))
                {
                    PrepReturn(ro, L"Internal Error Attempting to Perform Bitwise manipulation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                    return;
                }
                ro.errorObject = left;
            }

            if (expressions[Rust].varName.CountFinds(L'.'))
            {
                auto holdVal = ro.errorObject;
                int finalDot = expressions[Rust].varName.FindLast(L'.');
                TString initExp(expressions[Rust].varName.SubString(0, finalDot));
                this->ProcessIndividualStatement(initExp, ro);

                if (ro.returnCode)
                    return;

                if (!ro.errorObject.Get())
                {
                    PrepReturn(ro, L"Found Broken reference in Assignment operation!", L"", ReturnObject::ERR_BROKEN_REF, -1);
                    return;
                }

                if (ro.errorObject->GetVarType() != var_type::collection)
                {
                    PrepReturn(ro, L"", L"", ReturnObject::ERR_BROKEN_REF, -1);
                    ro.errorMessage.Format(L"Expected collection type for expression `%ws`", expressions[Rust].varName);
                    return;
                }

                dynamic_cast<TContainerVariable*>(ro.errorObject.Get())->SetValue(expressions[Rust].varName.SubString(finalDot + 1), holdVal);

                ro.errorObject = holdVal;
            }
            else
            {
                switch (UpdateVariable(expressions[Rust].varName, ro.errorObject))
                {
                case 1:
                    PrepReturn(ro, L"", L"", ReturnObject::ERR_BROKEN_REF, -1);
                    ro.errorMessage.Format(L"Assignment to non-existant variable '%ws' occured!", expressions[Rust].varName.GetConstantBuffer());
                    return;
                case 2:
                    PrepReturn(ro, L"", L"", ReturnObject::ERR_BROKEN_REF, -1);
                    ro.errorMessage.Format(L"Assignment to const variable '%ws' was attempted!", expressions[Rust].varName.GetConstantBuffer());
                    return;
                }
            }
            expressions[Rust].value = ro.errorObject;
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);

        }
    }
}

void TcJavaScriptInterpretor::HandleComma(TDataArray<JavaScriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ret)
{
}
