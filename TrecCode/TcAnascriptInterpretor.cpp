#include "TcAnascriptInterpretor.h"
#include <TStringVariable.h>
#include <TPrimitiveVariable.h>
#include <TContainerVariable.h>
#include <TAccessorVariable.h>
#include <TSpecialVariable.h>
#include <TAsyncVariable.h>
#include <TObjectVariable.h>

static TDataArray<TString> standardAOperators;


static TrecPointer<ObjectOperator> asOperators;

static TrecPointer<TVariable> aOne;

AnascriptExpression2::AnascriptExpression2()
{
}

AnascriptExpression2::AnascriptExpression2(const AnascriptExpression2& orig)
{
    varName.Set(orig.varName);
    value = orig.value;
}

AnascriptExpression2::AnascriptExpression2(const TString& name, TrecPointer<TVariable> value)
{
    varName.Set(name);
    this->value = value;
}


void AnascriptExpression2::operator=(const AnascriptExpression2& orig)
{
    varName.Set(orig.varName);
    value = orig.value;
}

TcAnascriptInterpretor::TcAnascriptInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env): TcTypeInterpretor(parentInterpretor, env)
{
    preProcessed = false;
    if (!aOne.Get())
        aOne = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<int>(1));


    if (!asOperators.Get())
        asOperators = TrecPointerKey::GetNewTrecPointerAlt<ObjectOperator, DefaultObjectOperator>();

    if (!standardAOperators.Size())
    {
        standardAOperators.push_back(L">>>=");
        standardAOperators.push_back(L"===");
        standardAOperators.push_back(L"!==");
        standardAOperators.push_back(L">>>");
        standardAOperators.push_back(L"<<=");
        standardAOperators.push_back(L">>=");
        standardAOperators.push_back(L"&&=");
        standardAOperators.push_back(L"||=");
        standardAOperators.push_back(L"??=");
        standardAOperators.push_back(L"**=");
        standardAOperators.push_back(L"++");
        standardAOperators.push_back(L"--");
        standardAOperators.push_back(L"**");
        standardAOperators.push_back(L"<<");
        standardAOperators.push_back(L">>");
        standardAOperators.push_back(L"<=");
        standardAOperators.push_back(L">=");
        standardAOperators.push_back(L"==");
        standardAOperators.push_back(L"!=");
        standardAOperators.push_back(L"&&");
        standardAOperators.push_back(L"||");
        standardAOperators.push_back(L"??");
        standardAOperators.push_back(L"+=");
        standardAOperators.push_back(L"-=");
        standardAOperators.push_back(L"*=");
        standardAOperators.push_back(L"/=");
        standardAOperators.push_back(L"%=");
        standardAOperators.push_back(L"&=");
        standardAOperators.push_back(L"^=");
        standardAOperators.push_back(L"|=");
    }
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

    bool ifDone = false;

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        TrecPointer<CodeStatement> state = statements[Rust];

        switch (state->statementType)
        {
        case code_statement_type::cst_function:
            ProcessFunction(state, ret);
            break;
        case code_statement_type::cst_let:
            ProcessLet(state, ret);
            break;
        case code_statement_type::cst_for:
            ProcessFor(state, ret);
            break;
        case code_statement_type::cst_if:
            ifDone =  ProcessIf(state, ret);
            if (ret.returnCode)
                return;

            afterIf:
            if (ifDone)
            {
                while ((Rust + 1) < statements.Size() && ifDone)
                {
                    switch (statements[Rust + 1]->statementType)
                    {
                    case code_statement_type::cst_else:
                    case code_statement_type::cst_else_if:
                        Rust++;
                        break;
                    default:
                        ifDone = false;
                    }
                }
            }
            else
            {
                while (!ifDone && (Rust + 1) < statements.Size())
                {
                    switch (statements[Rust + 1]->statementType)
                    {
                    case code_statement_type::cst_else_if:
                        ifDone = ProcessIf(state, ret);
                        Rust++;
                        goto afterIf;
                    case code_statement_type::cst_else:
                        Rust++;
                        if (true)
                        {
                            TrecSubPointer<TVariable, TcAnascriptInterpretor> anaVar = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcAnascriptInterpretor>(TrecPointerKey::GetSubPointerFromSoft<>(this->self), environment);

                            anaVar->statements = statements[Rust]->block;
                            ret = anaVar->Run();
                        }
                    default:
                        ifDone = true;
                        
                    }
                }
            }
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

        if (ret.returnCode || ret.mode != return_mode::rm_regular)
            break;
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
    int eq = statement->statement.Find(L'='), be = statement->statement.Find(L"be");

    TString name;
    ret.errorObject.Nullify();

    if (eq != -1 && be == -1)
    {
        name.Set(statement->statement.SubString(0, eq));
        ProcessExpression(statement, ret, eq + 1);
    }
    else if (be != -1 && eq == -1)
    {
        name.Set(statement->statement.SubString(0, be));
        ProcessExpression(statement, ret, be + 2);
    }
    else if (be != -1 && eq != -1)
    {
        name.Set(statement->statement);
    }
    else
    {
        ret.returnCode = ret.ERR_UNEXPECTED_TOK;
        ret.errorMessage.Set(L"Both 'be' and '=' tokens detected in let statement!");
        return;
    }

    if (ret.returnCode)
        return;

    name.Trim();
    CheckVarName(name, ret);
    if (ret.returnCode)
        return;
    TcVariableHolder holder(true, L"", ret.errorObject);
    if (variables.retrieveEntry(name, holder))
    {
        ret.returnCode = ret.ERR_IMPROPER_NAME;
        ret.errorMessage.Set(L"Variable Name already deteted");
        return;
    }
    variables.addEntry(name, holder);
    

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
    UINT pStack = 0, sStack = 0;

    ProcessExpression(statement, ret, pStack, sStack, startIndex);
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

void TcAnascriptInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT& parenth, UINT& square, UINT& index)
{
    if (!statement->statement.GetSize() || !statement->statement.GetTrim().Compare(L';'))
        return;

    TDataArray<AnascriptExpression2> expressions;
    TDataArray<TString> ops;
    bool processed = false; // False, we are looking for an expression, true, look for operator or end
    for (; index < statement->statement.GetSize(); index++)
    {
        bool processed = false; // Did we process some expression upon hitting a certain character
        ret.errorObject.Nullify();

        WCHAR ch = statement->statement[index];
        switch (ch)
        {
        case L'(':
            if (processed)
            {
                PrepReturn(ret, L"Unexpected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return;
            }
            processed = true;
            parenth++;
            ProcessExpression(statement, ret, parenth, square, ++index );
            break;
        case L'[':
            if (processed)
            {
                PrepReturn(ret, L"Unexpected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return;
            }
            processed = true;
            square++;
            ProcessArrayExpression(parenth, square, ++index, statement, ret);
            break;
        case L'\'':
        case L'\"':
        case L'`':
            if (processed)
            {
                PrepReturn(ret, L"Unexpected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return;
            }
            processed = true;
            ProcessStringExpression(parenth, square, index, statement, ret);
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
                return;
            }
            processed = true;
            ProcessNumberExpression(parenth, square, index, statement, ret);

        }


        if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || ch == L'_')
        {
            if (processed)
            {
                TString message;
                message.Format(L"Unexpected '%c' character detected", ch);
                PrepReturn(ret, message, L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return;
            }
            processed = true;
            ProcessVariableExpression(parenth, square, index, statement, ret);
        }

        if (processed)
        {
            if (ret.returnCode)
                return;
            AnascriptExpression2 exp;
            exp.value = ret.errorObject;
            ret.errorObject.Nullify();
            exp.varName.Set(ret.errorMessage);
            expressions.push_back(exp);
        }
        TString exp(statement->statement.SubString(index));

        // Handle Post-Increment/Decrement
        if (expressions.Size() && expressions[expressions.Size() - 1].varName.GetSize() && ((exp.StartsWith(L"++") || exp.StartsWith(L"--"))))
        {
            index += 2;
            bool inc = exp.StartsWith(L"++");

            AnascriptExpression2 jExp(expressions[expressions.Size() - 1]);

            if (GetVarStatus(jExp.varName) == 2)
            {
                TString message;
                message.Format(L"Error! Cannot %ws on const variable %ws!", inc ? L"increment" : L"decrement", jExp.varName.GetConstantBuffer().getBuffer());
                PrepReturn(ret, message, L"", ReturnObject::ERR_UNSUPPORTED_OP, statement->lineStart);
                return;
            }


            // Now handle the increment or decrement step

            ProcessIndividualStatement(jExp.varName, ret);

            TrecSubPointer<TVariable, TPrimitiveVariable> tempValue = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(ret.errorObject);

            if (!tempValue.Get())
            {
                ret.returnCode = ret.ERR_BROKEN_REF;

                ret.errorMessage.Format(L"Post-%ws attempt encountered %ws variable: Could not convert to Primitive Variable!",
                    inc ? L"Increment" : L"Decrement", L"Null");
                return;
            }



            UINT primType = tempValue->GetVType();
            ULONG64 data = 0;
            if (primType & TPrimitiveVariable::bit_float)
            {
                if (primType & TPrimitiveVariable::type_eight)
                {
                    data = tempValue->Get8Value();
                    double dData = 0.0;
                    memcpy_s(&dData, sizeof(dData), &data, sizeof(data));
                    dData += inc ? 1 : -1;
                    tempValue->Set(dData);
                }
                else
                {
                    primType = tempValue->Get4Value();
                    float fData = 0.0f;
                    memcpy_s(&fData, sizeof(fData), &primType, sizeof(primType));
                    fData += inc ? 1 : -1;
                    tempValue->Set(fData);
                }
            }
            else if (primType & TPrimitiveVariable::type_unsigned)
            {
                if (primType & TPrimitiveVariable::type_eight)
                {
                    data = tempValue->Get8Value();
                    if (!data && !inc)
                        tempValue->Set(static_cast<LONG64>(-1));
                    else
                    {
                        data += inc ? 1 : -1;
                        tempValue->Set(data);
                    }
                }
                else
                {
                    primType = tempValue->Get4Value();
                    if (!primType && !inc)
                        tempValue->Set(static_cast<int>(-1));
                    else
                    {
                        primType += inc ? 1 : -1;
                        tempValue->Set(primType);
                    }
                }
            }
            else if (primType & TPrimitiveVariable::type_bool || primType & TPrimitiveVariable::type_char)
            {
                TString message;
                message.Format(L"Invalid type found for variable '%ws' in pre-%ws operation!",
                    jExp.varName.GetConstantBuffer().getBuffer(), inc ? L"increment" : L"decrement");
                PrepReturn(ret, message, L"", ReturnObject::ERR_IMPROPER_TYPE, statement->lineStart);
                return;
            }
            else
            {
                if (primType & TPrimitiveVariable::type_eight)
                {
                    data = tempValue->Get8Value();
                    LONG64 iData = 0;
                    memcpy_s(&iData, sizeof(iData), &data, sizeof(data));
                    iData += inc ? 1 : -1;
                    tempValue->Set(iData);
                }
                else
                {
                    primType = tempValue->Get8Value();
                    int iData = 0;
                    memcpy_s(&iData, sizeof(iData), &primType, sizeof(primType));
                    iData += inc ? 1 : -1;
                    tempValue->Set(iData);
                }
            }

            exp.Delete(0, 2);
            exp.Trim();
        }


        bool foundOp = false;
        for (UINT Rust = 0; Rust < standardAOperators.Size(); Rust++)
        {
            if (exp.StartsWith(standardAOperators[Rust]))
            {
                ops.push_back(standardAOperators[Rust]);
                foundOp = true;
                exp.Delete(0, standardAOperators[Rust].GetSize());
                break;
            }
        }

        if (!foundOp)
        {
            if (!exp.FindOneOf(L"+-*/%^&|<>?=~!"))
            {
                ops.push_back(TString(exp[0]));
                exp.Delete(0, 1);
                foundOp = true;
            }
        }

        if (foundOp)
        {
            index += (ops.at(ops.Size() - 1).GetSize() - 1);
            processed = false;
            continue;
        }

        if (index < statement->statement.GetSize())
            ch = statement->statement[index];

        switch (ch)
        {
        case L'.':
            if (expressions.Size())
            {
                AnascriptExpression2 exp = expressions[expressions.Size() - 1];
                if (exp.varName.IsEmpty() && exp.value.Get())
                {
                    switch (exp.value->GetVarType())
                    {
                    case var_type::string:
                    case var_type::collection:
                        ret.errorObject2 = exp.value;
                    }
                }
            }
            break;
        case L']':
            if (!square)
            {
                PrepReturn(ret, L"Unexpected ']' token detected!", L"", ReturnObject::ERR_PARENTH_MISMATCH, statement->lineStart);
                return;
            }
            square--;
            index++;
            goto crunch;
        case L')':
            if (!parenth)
                if (!square)
                {
                    PrepReturn(ret, L"Unexpected ')' token detected!", L"", ReturnObject::ERR_PARENTH_MISMATCH, statement->lineStart);
                    return;
                }
            parenth--;
        case L',':
        case L';':
            index++;

            if (!expressions.Size() && !ops.Size())
                return;
            goto crunch;
        }

    }


crunch:

    HandlePreExpr(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleExponents(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleMultDiv(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleAddSub(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleBitwiseShift(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleLogicalComparison(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleEquality(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleBitwiseAnd(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleBitwiseXor(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleBitwiseOr(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleLogicalAnd(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleLogicalOr(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleNullish(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleConditional(expressions, ops, ret);
    if (ret.returnCode) return;

    HandleAssignment(expressions, ops, ret);
    if (ret.returnCode) return;


    if (expressions.Size() == 1)
        ret.errorObject = expressions[0].value;

    ret.errorObject2.Nullify();
}

void TcAnascriptInterpretor::ProcessArrayExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
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
        ProcessExpression(statement, ret, parenth, square, index);
        if (ret.returnCode)
            return;

        if (curParenth != parenth)
        {
            PrepReturn(ret, L"Parenthesis mismatch in array expression!", L"", ReturnObject::ERR_PARENTH_MISMATCH, statement->lineStart);
            return;
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

}


void TcAnascriptInterpretor::ProcessVariableExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    TDataArray<TString> pieces;

    TString phrase, wholePhrase;
    bool spaceDetected = false;
    UINT uret = 0;

    UCHAR attribute = 0;
    bool loopAround;
    bool isAsync = false;

    TrecPointer<TVariable> var = ret.errorObject2, vThis;

    ret.errorObject2.Nullify();

throughString:
    loopAround = false;
    for (; index < statement->statement.GetSize(); index++)
    {
        WCHAR ch = statement->statement[index];

        if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || ch == L'_' || (ch >= L'0' && ch <= L'9'))
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
                else if (!phrase.Compare(L"async"))
                    isAsync = true;
                else if (!phrase.Compare(L"await"))
                    attribute = 5;
                else
                {
                    TString message;
                    message.Format(L"Unexpected Expression after '%ws' token detected!", phrase.GetConstantBuffer().getBuffer());
                    PrepReturn(ret, message, L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                    return;
                }
                phrase.Empty();
                spaceDetected = false;
            }

            phrase.AppendChar(ch);
            continue;
        }

        if (ch == L' ')
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
        else if (!phrase.Compare(L"function"))
            attribute = 4;
        else if (!phrase.Compare(L"get"))
            attribute = 2;
        else if (!phrase.Compare(L"set"))
            attribute = 3;
        else if (!phrase.Compare(L"async"))
            isAsync = true;
        else if (!phrase.Compare(L"await"))
            attribute = 5;
        else if (var.Get())
        {
            if (var->GetVarType() == var_type::collection)
            {
                auto colVar = dynamic_cast<TContainerVariable*>(var.Get());
                bool pres;
                vThis = var;
                if (colVar->GetContainerType() == ContainerType::ct_array)
                {
                    TString a(L"Array");
                    colVar = dynamic_cast<TContainerVariable*>(GetVariable(a, pres).Get());
                    assert(colVar);
                }
                var = colVar->GetValue(phrase, pres);
                if (!var.Get())
                {

                    var = colVar->GetValue(phrase, pres, L"__proto__;super");
                    if (!pres)
                    {
                        var = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
                    }
                }
            }
            else if (var->GetVarType() == var_type::primitive || var->GetVarType() == var_type::primitive_formatted)
            {
                bool present;
                TString numVar(L"Number");
                auto NumberClass = GetVariable(numVar, present);
                if (NumberClass.Get() && NumberClass->GetVarType() == var_type::collection)
                {
                    TrecPointer<TVariable> numAtt = dynamic_cast<TContainerVariable*>(NumberClass.Get())->GetValue(phrase, present);
                    if (numAtt.Get())
                    {
                        vThis = var;
                        var = numAtt;
                    }
                    else
                    {
                        TString message;
                        message.Format(L"Failed to id Method/Attribute %ws for primitive variable %ws!", phrase.GetConstantBuffer().getBuffer(), wholePhrase.GetConstantBuffer().getBuffer());
                        PrepReturn(ret, message, L"", ReturnObject::ERR_BROKEN_REF, statement->lineStart);
                        return;
                    }
                }
                else
                {
                    PrepReturn(ret, L"INTERNAL ERROR! Failed to load 'Number' package of methods needed to process this variable", L"", ReturnObject::ERR_INTERNAL, statement->lineStart);
                    return;
                }
            }
            else if (var->GetVarType() == var_type::iterator)
            {
                vThis = var;
                TString a(L"Iterator");
                bool pres;
                auto colVar = dynamic_cast<TContainerVariable*>(GetVariable(a, pres).Get());
                assert(colVar);
                var = colVar->GetValue(phrase, pres);
            }
            else if (var->GetVarType() == var_type::async)
            {
                TClassStruct promise;
                if (!this->GetClass(L"Promise", promise))
                {
                    ret.returnCode = ret.ERR_INTERNAL;
                    ret.errorMessage.Set(L"INTERNAL ERROR! Promise Resources are not available!");
                    return;
                }
                TClassAttribute classAtt = promise.GetAttributeByName(phrase);

                if (dynamic_cast<TAsyncVariable*>(var.Get())->GetThreadCaller())
                    vThis = var;

                var = classAtt.def;
            }
            else if (var->GetVarType() == var_type::string)
            {
                TString s(L"String");
                bool pres = false;
                TrecPointer<TVariable> stringMethods = GetVariable(s, pres);
                if (!stringMethods.Get() || stringMethods->GetVarType() != var_type::collection)
                {
                    ret.returnCode = ret.ERR_INTERNAL;
                    ret.errorMessage.Set(L"Failed to Access String Methods in Method call on String data");

                    return;
                }

                if (!phrase.Compare(L"length"))
                {
                    var = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(var->GetSize());
                }
                else
                {
                    TrecPointer<TVariable> method = dynamic_cast<TContainerVariable*>(var.Get())->GetValue(phrase, pres);
                    if (!method.Get())
                    {
                        ret.returnCode = ret.ERR_BROKEN_REF;
                        ret.errorMessage.Format(L"Failed to find '%ws' Method on String object!", phrase.GetConstantBuffer().getBuffer());

                        return;
                    }
                    vThis = var;
                    var = method;
                }
            }
            else if (var->GetVarType() == var_type::native_object)
            {
                TrecObjectPointer objP = dynamic_cast<TObjectVariable*>(var.Get())->GetObjectW();
                if (!dynamic_cast<TVObject*>(objP.Get()))
                {
                    TString mess;
                    mess.Format(L"Null or Invalid Object detected through variable '%ws'", (wholePhrase + phrase).GetConstantBuffer().getBuffer());
                    this->PrepReturn(ret, mess, L"", ret.ERR_INTERNAL, statement->lineStart);
                }

                if (!dynamic_cast<TVObject*>(objP.Get())->GetVariable(phrase, var))
                    var.Nullify();
                else
                    vThis = var;

            }
        }
        else
        {
            bool pres;
            var = GetVariable(phrase, pres);
            if (!var.Get() && !phrase.Compare(L"Promise"))
                var = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TAsyncVariable>(0, TrecSubPointer<TVariable, TcInterpretor>());
        }
        wholePhrase.Append(phrase);

    }

    if (index < statement->statement.GetSize())
    {
        WCHAR ch = statement->statement[index];
        if (ch == L'(' && var.Get())
        {
            index++;
            wholePhrase.AppendChar(ch);
            parenth++;

            UINT curIndex = index;
            if (var->GetVarType() == var_type::interpretor || var->GetVarType() == var_type::accessor || var->GetVarType() == var_type::interpretor_gen)
            {
                UINT curRet = uret;

                if (attribute == 1)// calling new
                {
                    vThis = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TContainerVariable>(ContainerType::ct_json_obj);
                }
                uret += ProcessProcedureCall(parenth, square, index, vThis, var, statement, ret);

                if (ret.returnCode)
                    return;

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
            else if (var->GetVarType() == var_type::async)
            {
                UINT curRet = uret;
                TClassStruct promy;
                assert(this->GetClass(L"Promise", promy));

                if (attribute == 1 && !dynamic_cast<TAsyncVariable*>(var.Get())->GetThreadCaller())
                {
                    TClassAttribute promConst = promy.GetAttributeByName(L"constructor");
                    uret += ProcessProcedureCall(parenth, square, index, TrecPointer<TVariable>(), promConst.def, statement, ret);
                }
                else
                {
                    TClassAttribute promConst = promy.GetAttributeByName(phrase);
                    uret += ProcessProcedureCall(parenth, square, index, dynamic_cast<TAsyncVariable*>(var.Get())->GetThreadCaller()
                        ? var : TrecPointer<TVariable>(), promConst.def, statement, ret);
                }
                if (ret.returnCode)
                    return;

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
                // Try to create the Object through the Class SYstem. If present, it will set up all the Objects and SuperObjects for us
                auto contThis = ConstructObject(wholePhrase);
                bool present;
                int iIndex = statement->statement.Find(L')', index);
                if (iIndex < 0)
                {
                    PrepReturn(ret, L"Expected ')' token in expression!", L"", ReturnObject::ERR_PARENTH_MISMATCH, statement->lineStart);
                    return;
                }
                if (contThis.Get())
                {
                    // If Present, will attempt to call it's constructor
                    vThis = TrecPointerKey::GetTrecPointerFromSub<>(contThis);



                    TrecPointer<TVariable> att = contThis->GetValue(L"constructor", present);

                    if (dynamic_cast<TcInterpretor*>(att.Get()))
                    {
                        uret += ProcessProcedureCall(++parenth, square, ++index, vThis, att, statement, ret);
                    }

                }
                else
                {
                    // No Class Detected, Check for a function
                    vThis = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

                    TrecPointer<TVariable> func = this->GetVariable(wholePhrase, present);

                    if (dynamic_cast<TcInterpretor*>(func.Get()))
                    {
                        uret += ProcessProcedureCall(++parenth, square, ++index, vThis, func, statement, ret);
                    }
                }
                index = static_cast<UINT>(iIndex) + 1;


                var = vThis;

            }
            
        }


        if (ch == L'.' || ch == L'?')
        {
            if (!wholePhrase.Compare(L"super") && dynamic_cast<TContainerVariable*>(methodObject.Get()))
            {
                bool present;
                var = dynamic_cast<TContainerVariable*>(methodObject.Get())->GetValue(L"__proto__", present);
            }

            index++;
            loopAround = true;
            spaceDetected = false;
            wholePhrase.AppendChar(ch);
        }
    }
    phrase.Empty();

    if (loopAround)
        goto throughString;

    ret.errorMessage.Set(wholePhrase);

    if (var.Get() && var->GetVarType() == var_type::accessor)
    {
        auto getter = dynamic_cast<TAccessorVariable*>(var.Get())->GetCGetter();
        auto setter = dynamic_cast<TAccessorVariable*>(var.Get())->GetCSetter();
        if (getter.Get())
        {
            getter->SetActiveObject(vThis);
            ret = getter->Run();
            ret.mode = return_mode::rm_regular;
        }
        if (setter.Get())
        {
            setter->SetActiveObject(vThis);
            ret.errorObject2 = TrecPointerKey::GetTrecPointerFromSub<>(setter);
        }
    }
    else if (var.Get() && var->GetVarType() == var_type::native_object)
    {
        ret.errorObject2 = var;
    }
    else
        ret.errorObject = var;

}

void TcAnascriptInterpretor::ProcessNumberExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    UINT start = index;
    for (; index < statement->statement.GetSize(); index++)
    {
        switch (statement->statement[index])
        {
        case L' ':
        case L'\n':
        case L'\r':
        case L'\t':
        case L',':
        case L';':
        case L']':
        case L')':
            goto broken;
        }
    }
broken:
    TString numStr(statement->statement.SubString(start, index));
    double d = 0.0;
    LONG64 s = 0;
    UINT u = 0;
    if (TString::ConvertStringToUint(numStr, u))
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(u);
    else if (!numStr.ConvertToLong(s))
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(s);
    else if (!numStr.ConvertToDouble(d))
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(d);
    else
    {
        TString message;
        message.Format(L"Failed to convert token '%ws' to a number!", numStr.GetConstantBuffer().getBuffer());
        PrepReturn(ret, message, L"", ReturnObject::ERR_NOT_NUMBER, statement->lineStart);
    }
}

void TcAnascriptInterpretor::ProcessStringExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    WCHAR quote = statement->statement[index];
    bool istemplated = false;
    int quoteLoc = -1;
    TString theString;
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
            return ;
        }
        theString.Set(statement->statement.SubString(index + 1, quoteLoc));

        index = quoteLoc + 1;
        break;
    default:
        PrepReturn(ret, L"INTERNAL ERROR! Expected Quote character!", L"", ReturnObject::ERR_INTERNAL, statement->lineStart);
    }

    if (istemplated)
    {
        UINT startIndex = 0;
        while ((startIndex = theString.FindOutOfQuotes(L"${", startIndex)) != -1)
        {
            UINT closeBracket = theString.FindOutOfQuotes(L'}', startIndex);
            if (closeBracket == -1)
                break;

            TString var(theString.SubString(startIndex + 2, closeBracket));

            ProcessIndividualStatement(var, ret);
            // ProcessExpression(statements, cur, var, line, ro);

            if (ret.returnCode)
            {
                ret.returnCode = 0;
                ret.errorObject.Nullify();
            }

            TString res = ret.errorObject.Get() ? ret.errorObject->GetString() : TString(L"undefined");

            theString.Replace(TString(L"${") + var + L'}', res);
        }
    }

    ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(theString);
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


void TcAnascriptInterpretor::HandlePreExpr(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ro)
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
            ro.errorObject = asOperators->Add(var, aOne);
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
            ro.errorObject = asOperators->Subtract(var, aOne);
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

void TcAnascriptInterpretor::HandleExponents(TDataArray<AnascriptExpression2>& expresions, TDataArray<TString>& operators, ReturnObject& ro)
{
    if (expresions.Size() != operators.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Exponent handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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
                ro.errorObject = asOperators->Pow(expresions[Rust].value, expresions[Rust + 1].value);
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

void TcAnascriptInterpretor::HandleMultDiv(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Multiplication-Division handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].CompareNoCase(L"*"))
        {
            ro.errorObject = asOperators->Multiply(expressions[Rust].value, expressions[Rust + 1].value);
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
            ro.errorObject = asOperators->Divide(expressions[Rust].value, expressions[Rust + 1].value);
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
            ro.errorObject = asOperators->Mod(expressions[Rust].value, expressions[Rust + 1].value);
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

void TcAnascriptInterpretor::HandleAddSub(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ret)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ret, L"The JS-addition/subtraction handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].CompareNoCase(L"+"))
        {
            ret.errorObject = asOperators->Add(expressions[Rust].value, expressions[Rust + 1].value);
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
            ret.errorObject = asOperators->Subtract(expressions[Rust].value, expressions[Rust + 1].value);
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

void TcAnascriptInterpretor::HandleBitwiseShift(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ret)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ret, L"The JS-Bitwise handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleLogicalComparison(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-logical comparison handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleEquality(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Equality handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleBitwiseAnd(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-bitwise-and handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleBitwiseXor(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-bitwise-xor handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleBitwiseOr(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-bitwise-or handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleLogicalAnd(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-logical-and handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleLogicalOr(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-logical-or handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleNullish(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Nullish handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
        ro.errorMessage.Set(L"The JS-Nullish handler expected aOne more Expression than operator!");
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

void TcAnascriptInterpretor::HandleConditional(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Conditional handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

void TcAnascriptInterpretor::HandleAssignment(TDataArray<AnascriptExpression2>& expressions, TDataArray<TString>& ops, ReturnObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        PrepReturn(ro, L"The JS-Assignment handler expected aOne more Expression than operator!", L"", ReturnObject::ERR_BROKEN_REF, -1);
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

            // Now check to see if we have a native object

            int lastDot = expressions[Rust].varName.FindLast(L'.');
            if (lastDot != -1)
            {
                TString thisObj(expressions[Rust].varName.SubString(0, lastDot)),
                    attribute(expressions[Rust].varName.SubString(lastDot + 1));
                ProcessIndividualStatement(thisObj, ro);
                if (ro.returnCode)
                    return;
                CheckVarName(attribute, ro);
                if (ro.returnCode)
                    return;
                if (!ro.errorObject.Get())
                {
                    TString mess;
                    mess.Format(L"NULL reference at object '%ws' detected in assign statement!", thisObj.GetConstantBuffer().getBuffer());
                    PrepReturn(ro, mess, L"", ro.ERR_BROKEN_REF, 0);
                    return;
                }
                switch (ro.errorObject->GetVarType())
                {
                case var_type::collection:
                    dynamic_cast<TContainerVariable*>(ro.errorObject.Get())->SetValue(attribute, right);
                    break;
                case var_type::native_object:
                    do
                    {
                        TrecObjectPointer objP = dynamic_cast<TObjectVariable*>(ro.errorObject.Get())->GetObjectW();
                        if (!dynamic_cast<TVObject*>(objP.Get()))
                        {
                            ro.returnCode = ro.ERR_INTERNAL;
                            ro.errorMessage.Format(L"Null or Invalid Object detected through variable '%ws'", thisObj.GetConstantBuffer().getBuffer()); return;
                        }

                        if (!dynamic_cast<TVObject*>(objP.Get())->SetVariable(attribute, right))
                        {
                            ro.returnCode = ro.ERR_BROKEN_REF;
                            ro.errorMessage.Format(L"Cannot set attribute '%ws' on object at '%ws' - operation not supported!", attribute.GetConstantBuffer().getBuffer(), thisObj.GetConstantBuffer().getBuffer());
                            return;
                        }
                    } while (false);
                    break;
                default:
                }
            }
        }
        else if (!ops[Rust].Compare(L"+="))
        {
            found = true;
            ro.errorObject = asOperators->Add(left, right);
        }
        else if (!ops[Rust].Compare(L"-="))
        {
            found = true;
            ro.errorObject = asOperators->Subtract(left, right);
        }
        else if (!ops[Rust].Compare(L"**="))
        {
            found = true;
            ro.errorObject = asOperators->Pow(left, right);
        }
        else if (!ops[Rust].Compare(L"*="))
        {
            ro.errorObject = asOperators->Multiply(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"/="))
        {
            ro.errorObject = asOperators->Divide(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"%="))
        {
            ro.errorObject = asOperators->Mod(left, right);
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
                ro.errorMessage.Format(L"Error! %ws operator must have a named variable as the left hand expression!", ops[Rust].GetConstantBuffer().getBuffer());
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

                // Try to get a setter out of the final variable name
                bool p;
                TrecPointer<TVariable> setter = dynamic_cast<TContainerVariable*>(ro.errorObject.Get())->GetValue(expressions[Rust].varName.SubString(finalDot + 1), p);

                if (setter.Get() && setter->GetVarType() == var_type::accessor && dynamic_cast<TAccessorVariable*>(setter.Get())->GetCSetter().Get())
                {
                    auto setVar = dynamic_cast<TAccessorVariable*>(setter.Get())->GetCSetter();
                    setVar->SetActiveObject(ro.errorObject);
                    TDataArray<TrecPointer<TVariable>> params;
                    params.push_back(holdVal);
                    setVar->SetIntialVariables(params);

                    holdVal = ro.errorObject;
                    ro = setVar->Run();
                }
                else
                    dynamic_cast<TContainerVariable*>(ro.errorObject.Get())->SetValue(expressions[Rust].varName.SubString(finalDot + 1), holdVal);

                ro.errorObject = holdVal;
            }
            else
            {
                switch (UpdateVariable(expressions[Rust].varName, ro.errorObject))
                {
                case 1:
                    PrepReturn(ro, L"", L"", ReturnObject::ERR_BROKEN_REF, -1);
                    ro.errorMessage.Format(L"Assignment to non-existant variable '%ws' occured!", expressions[Rust].varName.GetConstantBuffer().getBuffer());
                    return;
                case 2:
                    PrepReturn(ro, L"", L"", ReturnObject::ERR_BROKEN_REF, -1);
                    ro.errorMessage.Format(L"Assignment to const variable '%ws' was attempted!", expressions[Rust].varName.GetConstantBuffer().getBuffer());
                    return;
                }
            }
            expressions[Rust].value = ro.errorObject;
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);

        }
    }
}

TrecSubPointer<TVariable, TContainerVariable> TcAnascriptInterpretor::ConstructObject(const TString& type)
{
    TrecSubPointer<TVariable, TContainerVariable> vThis;
    // Expect to deal with a type, go by type system, since if a function, then it would have been caught in the code above
    TClassStruct cla, pCla;
    if (this->GetClass(type, cla))
    {
        vThis = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);
        // Handle scenario where Type has a parent class
        TString parent;
        if (cla.GetParentClass(0, parent) && this->GetClass(parent, pCla))
        {
            auto protoThis = ConstructObject(parent);
            if (protoThis.Get())
            {
                vThis->SetValue(L"super", TrecPointerKey::GetTrecPointerFromSub<>(protoThis));
            }
        }

        TClassAttribute att;
        for (UINT Rust = 0; cla.GetAttributeByIndex(Rust, att); Rust++)
        {
            dynamic_cast<TContainerVariable*>(vThis.Get())->SetValue(att.name, att.def.Get() ? att.def->Clone() : att.def);
        }

    }
    return vThis;
}

bool IsEqual(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2, bool isEqual, bool castType)
{
    bool eqVal = false, eqType;

    if (var1.Get() && var2.Get())
    {
        DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(var1);
        DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(var2);

        eqType = dl1.type == dl2.type && var1->GetVarType() == var2->GetVarType() && var1->GetVType();

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