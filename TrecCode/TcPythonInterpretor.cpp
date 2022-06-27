#include "TcPythonInterpretor.h"
#include <atltrace.h>
#include <TPrimitiveVariable.h>

static TDataArray<TString> standardPOperators;

TString GetPyStringFromStatementType(code_statement_type cst);

TcPythonInterpretor::TcPythonInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env): 
    TcAnascriptInterpretor(parentInterpretor, env)
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
    PrintStatement(0);
}

void TcPythonInterpretor::PythonPreProcess(TDataArray<TrecPointer<CodeStatement>>& statements, bool indentationExpected, ReturnObject& ret)
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

void TcPythonInterpretor::PrintStatement(UINT indent)
{
    TString printer;
    
    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        printer.Format(L"%d ", indent);
        printer += GetPyStringFromStatementType(statements[Rust]->statementType);
        printer += statements[Rust]->statement + L'\n';
        ATLTRACE(printer.GetConstantBuffer().getBuffer());
        if (dynamic_cast<TcPythonInterpretor*>(statements[Rust]->statementVar.Get()))
            dynamic_cast<TcPythonInterpretor*>(statements[Rust]->statementVar.Get())->PrintStatement(indent + 1);
    }
}

TString GetPyStringFromStatementType(code_statement_type cst)
{
    switch (cst)
    {
    case code_statement_type::cst_if:
        return L"if    ";
    case code_statement_type::cst_else:
        return L"else  ";
    case code_statement_type::cst_else_if:
        return L"elif  ";
    case code_statement_type::cst_for:
        return L"for   ";
    case code_statement_type::cst_while:
        return L"while ";
    case code_statement_type::cst_function:
        return L"def   ";
    default:
        return L"reg   ";
    }
}

void TcPythonInterpretor::PythonPreProcess(TrecPointer<CodeStatement>& statement, ReturnObject& ret)
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
        PrepSubInterpretor(statement);
    }
    else if (statement->statement.StartsWith(L"elif", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 4;
        statement->statementType = code_statement_type::cst_else_if;
        PrepSubInterpretor(statement);
    }
    else if (statement->statement.StartsWith(L"else", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 4;
        statement->statementType = code_statement_type::cst_else;
        PrepSubInterpretor(statement);
    }
    else if (statement->statement.StartsWith(L"while", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 5;
        statement->statementType = code_statement_type::cst_while;
        PrepSubInterpretor(statement);
    }
    else if (statement->statement.StartsWith(L"for", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 3;
        statement->statementType = code_statement_type::cst_for;
        PrepSubInterpretor(statement);
    }
    else if (statement->statement.StartsWith(L"def", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 3;
        statement->statementType = code_statement_type::cst_function;
        PrepSubInterpretor(statement);
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
        PrepSubInterpretor(statement);
    }
    else if (statement->statement.StartsWith(L"except", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 6;
        statement->statementType = code_statement_type::cst_catch;
        PrepSubInterpretor(statement);
    }
    else if (statement->statement.StartsWith(L"finally", false, true))
    {
        PythonPreProcess(statement->block, true, ret);
        deleteCount = 7;
        statement->statementType = code_statement_type::cst_finally;
        PrepSubInterpretor(statement);
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

    ProcessExpression(state, ret, 0);
}

void TcPythonInterpretor::ProcessIf(UINT& index, ReturnObject& ret)
{
    bool doContinue = false;
    bool done = false;
    bool onElse = false;
    do
    {
        doContinue = false;
        if (!done && statements[index]->statementType != code_statement_type::cst_else)
        {
            ProcessExpression(statements[index], ret, 0);
            if (ret.returnCode)
                return;

            if (IsTruthful(ret.errorObject))
            {
                assert(dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get()));
                ret = dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get())->Run();

                if (ret.returnCode)
                    return;
                done = true;
            }


        }
        else if (!done && statements[index]->statementType != code_statement_type::cst_else)
        {
            assert(dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get()));
            ret = dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get())->Run();

            if (ret.returnCode)
                return;
        }

        if (index + 1 < statements.Size())
        {
            bool doOnElse = false;
            switch (statements[index + 1]->statementType)
            {
            case code_statement_type::cst_else:
                doOnElse = true;
            case code_statement_type::cst_else_if:
                if (onElse)
                {
                    ret.returnCode = ret.ERR_UNEXPECTED_TOK;
                    ret.errorMessage.Set(L"'else' or 'elif' cannot follow an 'else' statement!");
                    return;
                }
                onElse = doOnElse;
                index++;
                doContinue = true;
            }
        }

    } while (doContinue);
}

void TcPythonInterpretor::ProcessWhile(UINT index, ReturnObject& ret)
{
    bool doContinue = false;
    do
    {
        ProcessExpression(statements[index], ret, 0);
        if (ret.returnCode)
            return;

        if (doContinue = IsTruthful(ret.errorObject))
        {
            assert(dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get()));
            ret = dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get())->Run();

            if (ret.returnCode)
                return;
        }
    } while (doContinue);
}

void TcPythonInterpretor::ProcessFor(UINT& index, ReturnObject& ret)
{
    TString exp(statements[index]->statement);

    auto toks = exp.splitn(L' \t', 3);

    if (toks->Size() < 3)
    {
        ret.returnCode = ret.ERR_INCOMPLETE_STATEMENT;
        ret.errorMessage.Set("Expected at least 3 tokens '<name> in <exp>' after 'for' declaration!");
        return;
    }

    CheckVarName(toks->at(0), ret);
    if (ret.returnCode)
        return;

    if (toks->at(1).Compare(L"in"))
    {
        ret.returnCode = ret.ERR_INCOMPLETE_STATEMENT;
        ret.errorMessage.Set("Expected at least 3 tokens '<name> in <exp>' after 'for' declaration!");
        return;
    }

    ProcessExpression(statements[index], ret, statements[index]->statement.Find(L"in") + 3);

    if (ret.returnCode || !ret.errorObject.Get())
        return;

    auto iterator = ret.errorObject->GetIterator();

    // If there is an else statement after for, prepare to do it
    bool doElse = true;

    if (iterator.Get())
    {
        UINT elementIndex = 0;
        TString elementName;
        TrecPointer<TVariable> elementValue;

        TDataArray<TString> paramNames;
        paramNames.push_back(toks->at(0));

        while (dynamic_cast<TVariableIterator*>(iterator.Get())->Traverse(elementIndex, elementName, elementValue))
        {
            assert(dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get()));
            TDataArray<TrecPointer<TVariable>> vars;
            vars.push_back(elementValue);
            dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get())->SetIntialVariables(vars);
            ret = dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get())->Run();

            if (ret.returnCode)
                return;
            if (ret.mode == return_mode::rm_break)
            {
                doElse = false;
                break;
            }
        }
    }

    ret.mode = return_mode::rm_regular;


    // Python supports an else statement
    if (index + 1 < statements.Size() && statements[index+1]->statementType == code_statement_type::cst_else)
    {
        index++;
        if (doElse)
        {
            assert(dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get()));
            ret = dynamic_cast<TcInterpretor*>(statements[index]->statementVar.Get())->Run();
        }
    }
}

void TcPythonInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT index)
{
    UINT parenth = 0, square = 0;
    ProcessExpression(statement, ret, index, parenth, square);
}

void TcPythonInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, UINT index, UINT parenth, UINT square)
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
            ProcessExpression(statement, ret, parenth, square, ++index);
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
        for (UINT Rust = 0; Rust < standardPOperators.Size(); Rust++)
        {
            if (exp.StartsWith(standardPOperators[Rust], true))
            {
                ops.push_back(standardPOperators[Rust]);
                foundOp = true;
                exp.Delete(0, standardPOperators[Rust].GetSize());
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

bool TcPythonInterpretor::IsTruthful(TrecPointer<TVariable>)
{
    return false;
}

void TcPythonInterpretor::PrepSubInterpretor(TrecPointer<CodeStatement> statement)
{
    statement->statementVar = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcPythonInterpretor>(TrecPointerKey::GetSubPointerFromSoft<>(self), environment);
    dynamic_cast<TcPythonInterpretor*>(statement->statementVar.Get())->statements = statement->block;
    dynamic_cast<TcPythonInterpretor*>(statement->statementVar.Get())->preProcessed = true;
}
