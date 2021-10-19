#include "TcAnascriptInterpretor.h"
#include <TStringVariable.h>

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
    TObjectLocker lock(&this->thread);
    ReturnObject ret;
    if (!preProcessed)
    {
        ret.errorMessage.Set(L"Interpretor not ready to run. Make sure you are successful in calling 'SetFile' and 'PreProcess' before calling this method");
        ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
        return ret;
    }

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        TrecPointer<CodeStatement> state = statements[Rust];

        switch (state->statementType)
        {
        case code_statement_type::cst_function:
            ProcessFunction(state, ret);
            break;
        case code_statement_type::cst_let:

            break;
        case code_statement_type::cst_for:
            ProcessFor(state, ret);
            break;
        case code_statement_type::cst_if:

            break;
        case code_statement_type::cst_else:
        case code_statement_type::cst_else_if:
            ret.errorMessage.Set(L"Detected 'else' statement without an 'if' statement preceeding it!");
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            return;
        case code_statement_type::cst_break:
            ret.mode = return_mode::rm_break;
            return;
        case code_statement_type::cst_continue:
            ret.mode = return_mode::rm_continue;
            return;

        case code_statement_type::cst_declare:
            ProcessDeclare(state, ret);
            break;
        case code_statement_type::cst_regular:
            ProcessExpression(state, ret, 0);
        }
    }


	return ret;
}



void TcAnascriptInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
    variables.clear();
    UINT paramCount = paramNames.Size();
    for (UINT Rust = 0; Rust < params.Size(); Rust++)
    {
        TcVariableHolder holder;
        holder.mut = true;
        holder.value = params[Rust];

        TString n;
        if (Rust < paramCount)
            n.Set(paramNames[Rust]);
        else
        {
            n.Format(L"_as_arg_%d", Rust - paramCount);
        }

        variables.addEntry(n, holder);
    }
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
        else if (statements[Rust]->statement.StartsWith(L"break", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_break;
            statements[Rust]->statement.Delete(0, 5);
        }
        else if (statements[Rust]->statement.StartsWith(L"continue", true, true))
        {
            statements[Rust]->statementType = code_statement_type::cst_continue;
            statements[Rust]->statement.Delete(0, 8);
        }
        else statements[Rust]->statementType = code_statement_type::cst_regular;

        statements[Rust]->statement.Trim();
    }
    PreProcess(ret, statements, 1);
    if (!ret.returnCode)
        preProcessed = true;
}

void TcAnascriptInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
    TrecPointer<CodeStatement> state = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
    state->statement.Set(statement);

    ProcessExpression(state, ret, 0);
}

bool TcAnascriptInterpretor::IsTruthful(TrecPointer<TVariable> var)
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

void TcAnascriptInterpretor::ProcessLet(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
}

void TcAnascriptInterpretor::ProcessFunction(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    auto parentSplit = statement->statement.splitn(L' ', 2);

    if (parentSplit->Size() != 2)
    {
        ret.returnCode = ret.ERR_INCOMPLETE_STATEMENT;
        ret.errorMessage.Set(L"Function Declaration has an incomplete Statement!");
        return;
    }

    TString name(parentSplit->at(0));
    TString params(parentSplit->at(1));

    CheckVarName(name, ret);
    if (ret.returnCode)
        return;

    if (params.StartsWith(L"takes", true, true))
        params.Delete(0, 6);
    params.Trim();

    if (!params.StartsWith(L'('))
    {
        ret.returnCode = ret.ERR_UNEXPECTED_TOK;
        ret.errorMessage.Set(L"Function Declaration has an incomplete Statement! Could not find '(' token to indicate Parameter List");
        return;
    }

    TDataArray<TString> pNames, pTypes;

    if (params.CountFinds(L'(') != 1 && params.CountFinds(L')' != 1))
    {
        ret.returnCode = ret.ERR_BRACKET_MISMATCH;
        ret.errorMessage.Set(L"Function Declaration needs 1 '(' and 1 ')' to specify parameter names");
        return;
    }
    params.Delete(0, 1);

    parentSplit = params.split(L')');

    TString returnInfo(parentSplit->Size() ? parentSplit->at(1) : L"");

    // Return Type Info might be utilized later. Right now, leave it here

    parentSplit = parentSplit->at(0).split(L',');

    for (UINT Rust = 0; Rust < parentSplit->Size(); Rust++)
    {
        auto paramSplit = parentSplit->at(Rust).splitn(L':', 2);
        CheckVarName(paramSplit->at(0), ret);
        if (ret.returnCode)
            return;

        pNames.push_back(paramSplit->at(0));
        pTypes.push_back(paramSplit->Size() > 1 ? paramSplit->at(1) : L'');
    }

    TrecSubPointer<TVariable, TcAnascriptInterpretor> anaVar = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcAnascriptInterpretor>(TrecPointerKey::GetSubPointerFromSoft<>(this->self), environment);

    anaVar->statements = statement->block;
    anaVar->SetParamNames(pNames);
    anaVar->SetParamTypes(pTypes);


    TcVariableHolder holder;
    holder.mut = true;
    holder.type.Set(L"function");
    holder.value = TrecPointerKey::GetTrecPointerFromSub<>(anaVar);
    variables.addEntry(name, holder);
}

bool TcAnascriptInterpretor::ProcessIf(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    ProcessExpression(statement, ret, 0);
    if(ret.returnCode)
        return false;

    bool bRet = IsTruthful(ret.errorObject);
    if (bRet)
    {
        TrecSubPointer<TVariable, TcAnascriptInterpretor> anaVar = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcAnascriptInterpretor>(TrecPointerKey::GetSubPointerFromSoft<>(this->self), environment);

        anaVar->statements = statement->block;
        ret = anaVar->Run();
    }
    return bRet;
}

void TcAnascriptInterpretor::ProcessFor(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    auto tokens = statement->statement.splitn(L' ', 2);

    UINT tSize = tokens->Size();

    if (tSize != 2 || tokens->at(0).CompareNoCase(L"through"))
    {
        ret.returnCode = ret.ERR_INCOMPLETE_STATEMENT;
        ret.errorMessage.Set(L"Loop statement comes in the form of 'loop through <expression> with element name <name>'");
        return;
    }

    int withLoc = tokens->at(1).FindOutOfQuotes(L"with");

    if (withLoc == -1)
    {
        ret.returnCode = ret.ERR_INCOMPLETE_STATEMENT;
        ret.errorMessage.Set(L"Loop statement comes in the form of 'loop through <expression> with element name <name>'");
        return;
    }

    TString exp(tokens->at(1).SubString(0, withLoc));

    TString restOfExp(tokens->at(1).SubString(withLoc));

    tokens = restOfExp.split(L' ');

    if(tokens->Size() < 4)
    {
        ret.returnCode = ret.ERR_INCOMPLETE_STATEMENT;
        ret.errorMessage.Set(L"Loop statement comes in the form of 'loop through <expression> with element name <name>'");
        return;
    }

    if(tokens->at(0).CompareNoCase(L"with") || tokens->at(1).CompareNoCase(L"element") || tokens->at(2).CompareNoCase(L"name"))
    {
        ret.returnCode = ret.ERR_INCOMPLETE_STATEMENT;
        ret.errorMessage.Set(L"Loop statement comes in the form of 'loop through <expression> with element name <name>'");
        return;
    }

    CheckVarName(tokens->at(3), ret);
    if (ret.returnCode)
        return;

    ProcessIndividualStatement(exp, ret);
    if (ret.returnCode)
        return;
    TrecSubPointer<TVariable, TVariableIterator> iterator = ret.errorObject.Get() ? 
        TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TVariableIterator>(ret.errorObject->GetIterator())
        : TrecSubPointer<TVariable, TVariableIterator>();

    if (!iterator.Get())
    {
        ret.returnCode = ret.ERR_BROKEN_REF;
        ret.errorMessage.Format("Expression '%ws' in loop did not yeild an iterable variable!", exp.GetConstantBuffer().getBuffer());
        return;
    }
    UINT i = 0;
    TString n;
    TrecPointer<TVariable> v;
    while (!ret.returnCode && iterator->Traverse(i, n, v))
    {
        TrecSubPointer<TVariable, TcAnascriptInterpretor> anaVar = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcAnascriptInterpretor>(TrecPointerKey::GetSubPointerFromSoft<>(this->self), environment);

        anaVar->statements = statement->block;
        anaVar->variables.addEntry(tokens->at(3), TcVariableHolder(true, L"", v));
        ret = anaVar->Run();
    }
}

void TcAnascriptInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT startIndex)
{
}

void TcAnascriptInterpretor::ProcessDeclare(TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    if (statement->statement.StartsWith(L"print", true, true))
    {
        ProcessExpression(statement, ret, 5);
        if (ret.returnCode)
            return;
        environment->Print(ret.errorObject.Get() ? ret.errorObject->GetString() : L"null");
    }
    else if (statement->statement.StartsWith(L"print_line", true, true))
    {
        ProcessExpression(statement, ret, 10);
        if (ret.returnCode)
            return;
        environment->PrintLine(ret.errorObject.Get() ? ret.errorObject->GetString() : L"null");
    }
}

void TcAnascriptInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT& pStack, UINT& sStack, UINT startIndex)
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
