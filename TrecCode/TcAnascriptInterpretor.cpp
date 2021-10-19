#include "TcAnascriptInterpretor.h"

TcAnascriptInterpretor::TcAnascriptInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env): TcInterpretor(parentInterpretor, env)
{
    preProcessed = false;
}

COMPILE_TYPE TcAnascriptInterpretor::CanCompile()
{
	return COMPILE_TYPE(0);
}

void TcAnascriptInterpretor::Compile(COMPILE_TYPE type, ReturnObject& ret)
{
	ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
	ret.errorMessage.Set(L"Support for Anascript Compilation is not available at this time!");
}

void TcAnascriptInterpretor::SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst)
{

    statements.RemoveAll();
    preProcessed = false;

    // Make sure the file reference isn't null
    if (!codeFile.Get())
    {
        ret.errorMessage.Set(L"Null File Reference passed!");
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    }

    // Make sure we are dealing with a JS file
    if (!codeFile->GetPath().EndsWith(L".ascrpt", true))
    {
        ret.errorMessage.Set(L"Expected a .ascrpt file!");
        ret.returnCode = ReturnObject::ERR_INVALID_FILE_PARAM;
        return;
    }

    TFile file(codeFile->GetPath(), TFile::t_file_read | TFile::t_file_open_existing);

    WCHAR quote = L'\0';
    UINT lineNum = 1;
    TString line;

    statement_mode mode = statement_mode::sm_basic;

    TString statementLine;

    TrecPointer < CodeStatement> statement;

    UINT backslashStack = 0;

    while (file.ReadString(line))
    {
    resetLine:
        backslashStack = 0;
        for (UINT Rust = 0; Rust < line.GetSize(); Rust++)
        {
            WCHAR ch = line[Rust];
            switch (ch)
            {
            case L'\'':
            case L'\"':
                if (!quote)
                    quote = ch;
                else if (ch == quote)
                    quote = L'\0';
                break;
            case L';':
                if (!quote)
                {
                    statementLine += line.SubString(0, Rust);
                    statement = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
                    statement->statement.Set(statementLine);
                    statement->lineEnd = lineNum;
                    statement->lineStart = lineNum - statementLine.CountFinds(L'\n');

                    statements.push_back(statement);
                    statement.Nullify();
                    line.Delete(0, Rust + 1);
                    goto resetLine;
                }
                break;
            case L'#':
                if (!quote)
                {
                    line.Set(line.SubString(0, Rust));
                    break;
                }
            }

            if (ch == L'\\')
                backslashStack++;
            else
                backslashStack = 0;
        }

        if (statementLine.GetSize())
            statementLine += L'\n';
        statementLine += line;

        if (!(backslashStack % 2) && !quote)
        {
            statement = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
            statement->statement.Set(statementLine);
            statement->lineEnd = lineNum;
            statement->lineStart = lineNum - statementLine.CountFinds(L'\n');

            statements.push_back(statement);
            statement.Nullify();

            statementLine.Empty();
        }



        lineNum++;
    }

    if (quote)
    {
        WCHAR q[] = { quote, L'\0' };
        ret.errorMessage.Format(L"File Ends with a quote started by '%ws'!", q);
        ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
        return;
    }

    if (statementLine.GetSize())
    {
        ret.errorMessage.Set(L"File Ends with a Backslash preventing Final statement from being documented!");
        ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
        return;
    }


}

ReturnObject TcAnascriptInterpretor::Run()
{
	return ReturnObject();
}

void TcAnascriptInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
}

void TcAnascriptInterpretor::PreProcess(ReturnObject& ret)
{
    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        statements[Rust]->statement.Trim();


        if (statements[Rust]->statement.StartsWith(L"ext.", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_declare;
        }
        else if (statements[Rust]->statement.StartsWith(L"function.", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_function;
            statements[Rust]->statement.Delete(0, 9);
        }
        else if (statements[Rust]->statement.StartsWith(L"let", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_let;
            statements[Rust]->statement.Delete(0, 3);

        }
        else if (statements[Rust]->statement.StartsWith(L"end", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_end;
            statements[Rust]->statement.Delete(0, 3);

        }
        else if (statements[Rust]->statement.StartsWith(L"if", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_if;
            statements[Rust]->statement.Delete(0, 2);

        }
        else if (statements[Rust]->statement.StartsWith(L"else", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_else;
            statements[Rust]->statement.Delete(0, 4);
            statements[Rust]->statement.Trim();
            if (statements[Rust]->statement.StartsWith(L"if", true, true))
            {
                statements[Rust]->statementType = code_statement_type::cst_else_if;
                statements[Rust]->statement.Delete(0, 2);

            }
        }
        else if (statements[Rust]->statement.StartsWith(L"print", true, true) || statements[Rust]->statement.StartsWith(L"print_line", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_declare;
        }
        else if (statements[Rust]->statement.StartsWith(L"loop", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_for;
            statements[Rust]->statement.Delete(0, 4);


        }

        statements[Rust]->statement.Trim();
    }
    PreProcess(ret, statements, 1);
    if (!ret.returnCode)
        preProcessed = true;
}

void TcAnascriptInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
}

void TcAnascriptInterpretor::PreProcess(ReturnObject& ret, TDataArray<TrecPointer<CodeStatement>>& statements, UINT stack)
{
    UINT curStack = 0;
    TrecPointer<CodeStatement> parentStatement;
    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
       
    {
        switch (statements[Rust]->statementType)
        {
        case code_statement_type::cst_if:
        case code_statement_type::cst_for:
        case code_statement_type::cst_function:
            if (!curStack)
                parentStatement = statements[Rust];
            curStack++;
            break;
        case code_statement_type::cst_end:
            if (!curStack)
            {
                ret.returnCode = ret.ERR_BRACKET_MISMATCH;
                ret.errorMessage.Set(L"'End' statement detected that doesn't actually end a block!");
                return;
            }
            curStack--;
            break;
        case code_statement_type::cst_else:
        case code_statement_type::cst_else_if:
            if(!curStack)
            {
                ret.returnCode = ret.ERR_BRACKET_MISMATCH;
                ret.errorMessage.Set(L"'Else' statement detected that doesn't actually end an if-block!");
                return;
            }
            if (curStack == 1)
            {
                PreProcess(ret, parentStatement->block, 0);
                if (ret.returnCode)
                    return;
                parentStatement = statements[Rust];
                break;
            }
        default:
            if (curStack)
            {
                parentStatement->block.push_back(statements[Rust]);
                statements.RemoveAt(Rust--);
            }
            else if (parentStatement.Get())
            {
                PreProcess(ret, parentStatement->block, 0);
                if (ret.returnCode)
                    return;
                parentStatement.Nullify();
            }
        }
    }
}
