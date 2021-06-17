#include "TcJavaScriptInterpretor.h"
#include <TSpecialVariable.h>
#include <TStringVariable.h>
#include <TContainerVariable.h>
#include <TPrimitiveVariable.h>
#include <TAccessorVariable.h>
#include <cassert>
#include <JavaScriptFunc.h>
#include "JsConsole.h"
#include "TcJavaScriptClassInterpretor.h"
#include <JsNumber.h>
#include <JsMath.h>
#include "JsPromise.h"
#include <TIteratorVariable.h>
#include "JsArray.h"
#include <TFunctionGen.h>


static TDataArray<TString> standardOperators;

static TrecPointer<ObjectOperator> jsOperators;

static TrecPointer<TVariable> one;

static TDataArray<WCHAR> noSemiColonEnd;

/***/
void JsIteratorNext(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReturnObject& ret)
{
    if (!params.Size() || !dynamic_cast<TIteratorVariable*>(params[0].Get()))
    {
        ret.returnCode = ret.ERR_BROKEN_REF;
        ret.errorMessage.Set(L"Expected an Iterator!");
        return;
    }

    ret.errorObject = dynamic_cast<TIteratorVariable*>(params[0].Get())->Traverse(1, 0b00000101, ret);
}

TrecPointer<TVariable> GetJsIteratorMethods(TrecSubPointer<TVariable, TcInterpretor> parent, TrecPointer<TEnvironment> env)
{
    TrecSubPointer<TVariable, TContainerVariable> con = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    con->SetValue(L"next", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcNativeInterpretor>(JsIteratorNext, parent, env));
    return TrecPointerKey::GetTrecPointerFromSub<>(con);
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



TrecPointer<TVariable> TcJavaScriptInterpretor::Clone()
{
    TrecSubPointer<TVariable, TcInterpretor> subParent = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(TrecPointerKey::GetTrecPointerFromSoft<>(this->parent));
    TrecSubPointer<TVariable, TcJavaScriptInterpretor> ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcJavaScriptInterpretor>(subParent, environment);

    ret->classes = classes;
    ret->isAsync = isAsync;
    
    for (UINT Rust = 0; Rust < defaultVars.Size(); Rust++)
        ret->defaultVars.push_back(defaultVars[Rust].Get() ? defaultVars[Rust]->Clone() : defaultVars[Rust]);

    ret->methodObject = methodObject.Get() ? methodObject->Clone() : methodObject;
    ret->paramNames = paramNames;
    ret->paramTypes = paramTypes;
    ret->selfWord.Set(selfWord);
    ret->statements = statements;
    ret->strictVariables = strictVariables;

    return TrecPointerKey::GetTrecPointerFromSub<>(ret);
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
    ThreadLock();
    collector.CollectStatement(statements);
    readyToRun = false;

    if (isFirst)
    {
        // Initialize Objects meant for the main Interpretor

        // Intitialize a Console Object
        // Console Object
        variables.addEntry(L"console", TcVariableHolder(false, L"", GetJsConsole()));
        // Object Object
        variables.addEntry(L"Object", TcVariableHolder(false, L"", JavaScriptFunc::GetJSObectVariable(TrecPointerKey::GetSubPointerFromSoft<TVariable>(self), environment)));
        // Number methods and final values
        variables.addEntry(L"Number", TcVariableHolder(false, L"", JsNumber::GetJsNumberObject(TrecPointerKey::GetSubPointerFromSoft<TVariable>(self), environment)));
        // Math Object
        variables.addEntry(L"Math", TcVariableHolder(false, L"", JsMath::GetJsMathObject(TrecPointerKey::GetSubPointerFromSoft<TVariable>(self), environment)));
        // Array Object
        variables.addEntry(L"Array", TcVariableHolder(false, L"", JsArray::GetJsArrayMehtods(TrecPointerKey::GetSubPointerFromSoft<TVariable>(self), environment)));

        // Iterator
        variables.addEntry(L"Iterator", TcVariableHolder(false, L"", GetJsIteratorMethods(TrecPointerKey::GetSubPointerFromSoft<TVariable>(self), environment)));
        //Promise
        this->classes.addEntry(L"Promise", JsPromise::GetPromiseClass(TrecPointerKey::GetSubPointerFromSoft<TVariable>(self), environment));
    }
    ThreadRelease();
}

ReturnObject TcJavaScriptInterpretor::Run()
{
    ThreadLock();
    ReturnObject ret;
    if (!readyToRun)
    {
        ret.errorMessage.Set(L"Interpretor not ready to run. Make sure you are successful in calling 'SetFile' and 'PreProcess' before calling this method");
        ret.returnCode = ReturnObject::ERR_UNSUPPORTED_OP;
        ThreadRelease();
        return ret;
    }

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        TrecPointer<CodeStatement> state = statements[Rust];
        if (!state.Get())
        {
            statements.RemoveAt(Rust--);
            continue;
        }
        while (state.Get())
        {
            TcJavaScriptInterpretor* inte = dynamic_cast<TcJavaScriptInterpretor*>(state->statementVar.Get());
            if (inte && inte->parent.Get() != this)
            {
                TrecSubPointer<TVariable, TcInterpretor> subSelf = TrecPointerKey::GetSubPointerFromSoft<>(self);
                TrecPointer<TVariable> vSelf = TrecPointerKey::GetTrecPointerFromSub<>(subSelf);
                inte->parent = TrecPointerKey::GetSoftPointerFromTrec<>(vSelf);
            }
            state = state->next;
        }
    }
    ret =  Run(statements, yieldIndex);
    ThreadRelease();
    return ret;
}

void TcJavaScriptInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
    ThreadLock();
    variables.clear();
    bool onDefault = false;

    // Create the arguements parameter
    TrecSubPointer<TVariable, TContainerVariable> arguments = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

    for (UINT Rust = 0; Rust < paramNames.Size(); Rust++)
    {
        TcVariableHolder holder;
        holder.mut = true;
        if (Rust < params.Size())
            holder.value = params[Rust];
        else if (Rust < defaultVars.Size())
            holder.value = defaultVars[Rust];
        else
            holder.value = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);

        variables.addEntry(paramNames[Rust], holder);
        arguments->AppendValue(holder.value);
    }

    TcVariableHolder holder;
    holder.mut = false;
    holder.value = TrecPointerKey::GetTrecPointerFromSub<>(arguments);
    variables.addEntry(L"arguments", holder);

    for (UINT Rust = 0; Rust < strictVariables.count(); Rust++)
    {
        TDataEntry<TcVariableHolder> entry;
        if(strictVariables.GetEntryAt(Rust, entry))
        {
            variables.addEntry(entry.key, entry.object);
        }
    }
    ThreadRelease();
}

void TcJavaScriptInterpretor::PreProcess(ReturnObject& ret)
{
    ThreadLock();
    if (!readyToRun)
    {
        this->HandleSemiColon();
        ret = PreProcess();
    }
    ThreadRelease();
}

ReturnObject TcJavaScriptInterpretor::PreProcess()
{
    ReturnObject ret;
    PreProcess(ret, statements);

    if (ret.returnCode)
        return ret;

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        auto state = statements[Rust];
        TrecPointer<CodeStatement> nextState = Rust + 1 < statements.Size() ? statements[Rust + 1] : TrecPointer<CodeStatement>();
        switch (state->statementType)
        {
        case code_statement_type::cst_while:
        case code_statement_type::cst_function:
        case code_statement_type::cst_function_gen:
        case code_statement_type::cst_class:
            if (state->next.Get())
            {
                if (!nextState.Get() || !state->next->IsEqual(*nextState.Get()))
                    statements.InsertAt(state->next, Rust + 1);
                state->next.Nullify();
            }
        }
    }

    if (!ret.returnCode)
        readyToRun = true;
    return ret;
}

void TcJavaScriptInterpretor::ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
{
    UINT p = 0, s = 0, i = 0;
    TrecPointer<CodeStatement> state = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
    state->lineStart = 0;
    state->statement.Set(statement.GetTrim());
    state->lineEnd = statement.CountFinds(L'\n');
    ProcessExpression(p, s, i, state, ret);
}

TcJavaScriptInterpretor::TcJavaScriptInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env):
    TcTypeInterpretor(parentInterpretor, env)
{
    readyToRun = isAsync = false;
    yieldIndex = 0;
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

    if (!noSemiColonEnd.Size())
    {
        noSemiColonEnd.push_back(L',');
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
        ret = tcJsInt->PreProcess();
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
        yieldIndex = 0;
        ret = Run(statements, Rust, yieldStatement);
        if (ret.returnCode || (ret.mode != return_mode::rm_regular))
        {
            if (ret.mode == return_mode::rm_yield)
            {
                yieldIndex = Rust;
                if (yieldStatement.Get())
                    yieldIndex++;
                yieldStatement.Nullify();
            }
            break;
        }
    }


    return ret;
}

ReturnObject TcJavaScriptInterpretor::Run(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, TrecPointer<CodeStatement> statement)
{
    ReturnObject ret;


    if(!statement.Get())
        statement = statements[index];
    yieldStatement.Nullify();

    switch (statement->statementType)
    {
    case code_statement_type::cst_break:
        ret.mode = return_mode::rm_break;
        break;
    case code_statement_type::cst_class:
        ProcessClass(statements, index, statement, ret);
        break;
    case code_statement_type::cst_const:
    case code_statement_type::cst_let:
    case code_statement_type::cst_var:
        ProcessAssignmentStatement(statement, ret);
        break;
    case code_statement_type::cst_continue:
        ret.mode = return_mode::rm_continue;
        break;

    case code_statement_type::cst_for:
    case code_statement_type::cst_for_3:
        ProcessFor(statements, index, ret);

        break;
    case code_statement_type::cst_function:
    case code_statement_type::cst_function_gen:
        ProcessFunctionExpression(statement, ret);
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
    case code_statement_type::cst_yield:
        ProcessExpression(statement, ret);
        if (!ret.returnCode)
        {
            ret.mode = return_mode::rm_yield;
            yieldStatement = statement;
        }
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
    //case code_statement_type::cst_finally:
        ProcessTryCatchFinally(statements, index, statement, ret);
        break;
    case code_statement_type::cst_do:
    case code_statement_type::cst_while:
        ProcessWhile(statement, index, ret);
        break;
    case code_statement_type::cst_virtual_assign:
        ProcessPrototypeAssign(statement, ret);
        break;
    }

    return ret;
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
            else if (!parenthState)
                SetStatementToBlock(state, ret);

        }
    }
}

void TcJavaScriptInterpretor::PreProcess(ReturnObject& ret, TrecPointer<CodeStatement>& state)
{
    if (!state.Get())
        return;

    state->statement.Trim();

    if (!state->statement.Compare(L";") && !state->block.Size() && !state->next.Get())
    {
        state.Nullify();
        return;
    }

    TString startStatement(state->statement);

    if (state->next.Get())
        state->next->parent = TrecPointerKey::GetSoftPointerFromTrec<>(state);

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

    if (startStatement.StartsWith(L"else", false, true) || !startStatement.Compare(L"else"))
    {
        if (!state->parent.Get())
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unexpected 'else' token detected!");
            return;
        }

        code_statement_type parentType = state->parent.Get()->statementType;
        if (parentType != code_statement_type::cst_else_if && parentType != code_statement_type::cst_if)
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unexpected 'else' token detected! Epected Previous statement to be an 'if' block or an 'else if' block!");
            return;
        }

        state->statement.Delete(0, 4);
        state->statement.Trim();

        startStatement.Set(state->statement);

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
        SetStatementToBlock(state, ret);
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
            SetStatementToBlock(state, ret);
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
        return;
    }

    if (startStatement.StartsWith(L"function*", false, true))
    {
        state->statementType = code_statement_type::cst_function_gen;
        state->statement.Delete(0, 9);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
        PreProcess(ret, state->next);
        return;
    }
    bool isAsync = startStatement.StartsWith(L"async", false, true);

    if (isAsync)
    {
        startStatement.Delete(0, 5);
        startStatement.Trim();
    }

    if (startStatement.StartsWith(L"function", false, true))
    {
        state->statementType = code_statement_type::cst_function;
        state->statement.Delete(0, 8);
        state->statement.Trim();

        if (state->statement.StartsWith(L"*"))
        {
            state->statementType = code_statement_type::cst_function_gen;
            state->statement.Delete(0, 1);
            state->statement.Trim();
        }

        if (isAsync)
            state->statement.Set(TString(L"async ") + state->statement);

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
        return;
    }

    if (startStatement.StartsWith(L"var", false, true))
    {
        state->statementType = code_statement_type::cst_var;
        state->statement.Delete(0, 3);
        state->statement.Trim();

        PreProcess(ret, state->block);
        return;
    }

    if (startStatement.StartsWith(L"class", false, true))
    {
        state->statementType = code_statement_type::cst_class;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        TrecSubPointer<TVariable, TcJavaScriptClassInterpretor> classJs = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcJavaScriptClassInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<>(self), environment);
        classJs->PreProcess(ret);
        if (ret.returnCode)
            return;

        state->statementVar = TrecPointerKey::GetTrecPointerFromSub<>(classJs);
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

    if (startStatement.StartsWith(L"yield", false, true) || startStatement.StartsWith(L"yield;"))
    {
        state->statementType = code_statement_type::cst_yield;
        state->statement.Delete(0, 5);
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
        return;
    }

    if (startStatement.StartsWith(L"try", false, true) || !startStatement.Compare(L"try"))
    {
        state->statementType = code_statement_type::cst_try;
        state->statement.Delete(0, 3);
        state->statement.Trim();

        SetStatementToBlock(state, ret);
        if (ret.returnCode) return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"catch", false, true) || startStatement.StartsWith(L"catch("))
    {
        if (!state->parent.Get())
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unexpected 'catch' token detected!");
            return;
        }

        code_statement_type parentType = state->parent.Get()->statementType;
        if (parentType != code_statement_type::cst_try)
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unexpected 'catch' token detected! Epected Previous statement to be a 'try' block!");
            return;
        }

        state->statementType = code_statement_type::cst_catch;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        SetStatementToBlock(state, ret);
        if (ret.returnCode)return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"finally", false, true) || !startStatement.Compare(L"finally"))
    {
        if (!state->parent.Get())
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unexpected 'catch' token detected!");
            return;
        }

        code_statement_type parentType = state->parent.Get()->statementType;
        if (parentType != code_statement_type::cst_try && parentType != code_statement_type::cst_catch)
        {
            ret.returnCode = ReturnObject::ERR_UNEXPECTED_TOK;
            ret.errorMessage.Set(L"Unexpected 'catch' token detected! Epected Previous statement to be a 'try' or a 'catch' block!");
            return;
        }

        state->statementType = code_statement_type::cst_finally;
        state->statement.Delete(0, 7);
        state->statement.Trim();

        SetStatementToBlock(state,ret);
        if (ret.returnCode)return;
        PreProcess(ret, state->next);
        return;
    }

    if (startStatement.StartsWith(L"continue", false, true) || startStatement.StartsWith(L"continue;"))
    {
        state->statementType = code_statement_type::cst_continue;
        state->statement.Delete(0, 5);
        state->statement.Trim();

        PreProcess(ret, state->block);
        if (ret.returnCode)return;
    }

    if(state->statementType == code_statement_type::cst_not_set)
        state->statementType = code_statement_type::cst_regular;
    PreProcess(ret, state->block);
    if (ret.returnCode)return;
    PreProcess(ret, state->next);
}

void TcJavaScriptInterpretor::HandleSemiColon()
{
    HandleSemiColon(statements);
}

void TcJavaScriptInterpretor::HandleSemiColon(TDataArray<TrecPointer<CodeStatement>>& statements)
{
    TDataArray<TrecPointer<CodeStatement>> tempStatements;

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        TrecPointer<CodeStatement> state = statements.at(Rust);

        if (!state.Get() || !state->statement.GetTrim().GetSize())
            continue;

        
        HandleSemiColon(state, tempStatements);
        //if (state->block.Size())
        //    HandleSemiColon(state->block);
    }

    statements = tempStatements;

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        if (statements[Rust]->block.Size())
        {
            HandleSemiColon(statements[Rust]->block);
            for (UINT C = 0; C < statements[Rust]->block.Size(); C++)
            {
                statements[Rust]->block[C]->parent = TrecPointerKey::GetSoftPointerFromTrec<>(statements[Rust]);
            }
        }
    }
}

void TcJavaScriptInterpretor::HandleSemiColon(TrecPointer<CodeStatement> state, TDataArray<TrecPointer<CodeStatement>>& statements, bool isNext)
{
    TString state1, state2;

    state->statement.Trim();

    bool onState1 = true;
    WCHAR quote = L'\0';
    UINT backSlash = 0;
    UINT C = 0;
    UINT line = state->lineStart;

    probeStatementString:
    for (; C < state->statement.GetSize(); C++)
    {
        TString* curState = onState1 ? &state1 : &state2;
        WCHAR ch = state->statement[C];
        bool addChar = true;
        switch (ch)
        {
        case L'\'':
        case L'\"':
        case L'`':
            if (quote && !(backSlash % 2))
                quote = L'\0';
            else if (!quote)
                quote = ch;
            break;
        case L'\\':
            if (quote)
                backSlash++;
            break;
        case L'\r':
            if (!quote)
                addChar = false;
        }

        if (addChar)
            curState->AppendChar(ch);
        if (backSlash && ch != L'\\')
            backSlash = 0;

        if (ch == L'\n' && !quote)
        {

            if (onState1)
                onState1 = false;
            else
            {
                C++;
                break;
            }
        }
    }

    state1.Trim();
    state2.Trim();
    if (!state2.GetSize() && C < state->statement.GetSize())
        goto probeStatementString;

    if (state1.GetSize() && !state2.GetSize())
    {
        // We are mostly done. Just create a Code Statement with the features of the current
        if (!isNext)
        {
            TrecPointer<CodeStatement> newState = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
            newState->statement.Set(state1);
            newState->lineEnd = newState->lineStart = line;
            newState->block = state->block;

            newState->parent = state->parent;

            newState->next = state->next;
            statements.push_back(newState);
        }
        else
        {
            state->statement.Set(state1);
        }
        return;
    }
    else if (state1.GetSize() && state2.GetSize())
    {
        WCHAR ch1 = state1[state1.GetSize() - 1];
        WCHAR ch2 = state2[0];
        bool splitSemi = false;

        if ((IsCharAlphaNumericW(ch1) || ch1 == L'_') && (IsCharAlphaNumericW(ch2) || ch2 == L'_'))
            splitSemi = true;
        if (state1.EndsWith(L"return") ||
            state1.EndsWith(L"continue") ||
            state1.EndsWith(L"break") ||
            state1.EndsWith(L"throw") ||
            state1.EndsWith(L"yield") ||
            state1.EndsWith(L"++") ||
            state1.EndsWith(L"--"))
            splitSemi = true;

        if (state1.EndsWith(L"]") || state1.EndsWith(L')'))
        {
            splitSemi = true;
            for (UINT Rust = 0; Rust < noSemiColonEnd.Size() && splitSemi; Rust++)
            {
                if (ch2 == noSemiColonEnd[Rust])
                    splitSemi = false;
            }
        }


        if (splitSemi)
        {
            // Initialite semiColon insertion
            if (!isNext)
            {
                TrecPointer<CodeStatement> newState = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
                newState->statement.Set(state1);
                newState->lineEnd = newState->lineStart = line;
                // newState->block = state->block;

                newState->parent = state->parent;

                // newState->next = state->next;
                statements.push_back(newState);
            }
            else
            {
                state->statement.Set(state1);
            }
            // If this was a 'next' statement, make sure it is not this time around
            isNext = false;

            state1.Set(state2);
        }
        else
            state1.Append(state2);
        state2.Empty();
    }
    goto probeStatementString;
}

UINT TcJavaScriptInterpretor::ProcessExpression(TrecPointer<CodeStatement> statement, ReturnObject& ret, TrecSubPointer<TVariable, TcJavaScriptInterpretor> in)
{
    UINT parenth = 0, square = 0, index = 0;
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
            tokens = statementString.splitn(L" \t\n\r", 4);
            retString.Set(tokens->at(0) + L' ');
            tokens->RemoveAt(0);
        }
        else
            tokens = statementString.splitn(L" \t\n\r", 3);

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
        index = statement->statement.Find(tokens->at(2), statement->statement.Find(tokens->at(1)) + tokens->at(1).GetSize());
        returnable += ProcessExpression(parenth, square, index, statement, ret);

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

        switch (statement->statementType)
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
            returnable += ProcessExpression(parenth, square, index, statement, ret);
        }
        statement->statementType = code_statement_type::cst_for_3;

        break;
    default:
        returnable += ProcessExpression(parenth, square, index, statement, ret);

        if (ret.errorObject.Get() && ret.errorObject->GetVarType() == var_type::async)
        {
            TrecSubPointer<TVariable, TAsyncVariable> asyncVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAsyncVariable>(ret.errorObject);
            ProcessTAsyncObject(asyncVar);
        }
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
            if (Rust == subStatements.Size() - 1)
            {
                UINT p = 0, s = 0, i = statement->statement.GetSize();
                ProcessExpression(p, s, i, statement, ret);
                if (ret.returnCode)
                    return;
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
    }
    
    if (statement->next.Get())
    {
        TString nStatement(statement->next->statement.GetTrim());
        if (nStatement.GetSize() == 0 || !nStatement.Compare(L';'))
        {
            statement->next.Nullify();
        }
        else
        {
            statement->next->statementType = statement->statementType;
            ProcessAssignmentStatement(statement->next, ret);
        }
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

void TcJavaScriptInterpretor::ProcessWhile(TrecPointer<CodeStatement> statement, UINT& index, ReturnObject& ret)
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
        ret = jsVar->Run();

    while (IsTruthful(ret.errorObject))
    {
        ret = jsVar->Run();
        // For loop is responsible for handling break and for
        if (ret.mode != return_mode::rm_regular)
        {
            if (ret.mode == return_mode::rm_return || ret.mode == return_mode::rm_yield)
                return;
            // here, we either break or continue
            return_mode m = ret.mode;
            ret.mode = return_mode::rm_regular;
            if (m == return_mode::rm_break)
                break;
        }

        ProcessExpression(expreHolder, ret);
        if (ret.returnCode)
            return;
    }

    //if (statement->statementType == code_statement_type::cst_while && blockHolder->next.Get())
    //{
    //    ret = Run(statements, index, blockHolder->next);
    //}


    //if (statement->statementType == code_statement_type::cst_do)
    //    ret = Run(expreHolder);
    //else ret = Run(blockHolder);
}

void TcJavaScriptInterpretor::ProcessFor(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, ReturnObject& ret)
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
            ret = Run(statements, index, block->next);
        }
    }
    else
    {
        UINT bIndex = index + 2; // Index would be the first statement of the for-loop, so index Plus 2 would be the final statement with the block of code to execute
        if (bIndex >= statements.Size())
        {
            PrepReturn(ret, L"Incomplete 3-Statement For-loop detected!", L"", ReturnObject::ERR_INCOMPLETE_STATEMENT, 0);
            return;
        }
        block = statements[bIndex];
        if (!block.Get())
        {
            PrepReturn(ret, L"INTERNAL ERROR! Block Statement was null in 3-statement For-Loop!", L"", ReturnObject::ERR_INTERNAL, 0);
            return;
        }
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
                if (ret.mode == return_mode::rm_return || ret.mode == return_mode::rm_yield)
                    return;
                // here, we either break or continue
                return_mode m = ret.mode;
                ret.mode = return_mode::rm_regular;
                if (m == return_mode::rm_break)
                    break;
            }

            if (jsVar.Get())
            {
                UINT parenth = 1, square = 0, strIndex = 0;
                jsVar->ProcessExpression(parenth, square, strIndex, statements[index + 2], ret);
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
        index += 2;
        if (block->next.Get())
        {
            ret = Run(statements, index, block->next);
        }
    }
}

void TcJavaScriptInterpretor::ProcessTryCatchFinally(TDataArray<TrecPointer<CodeStatement>>& statements, UINT index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    auto jsVar = dynamic_cast<TcJavaScriptInterpretor*>(statement->statementVar.Get());
    switch (statement->statementType)
    {
    case code_statement_type::cst_try:
        if (!statement->next.Get() || (statement->next->statementType != code_statement_type::cst_catch && statement->next->statementType != code_statement_type::cst_finally))
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
                ProcessTryCatchFinally(statements, index, statement->next, ret);
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

void TcJavaScriptInterpretor::ProcessClass(TDataArray<TrecPointer<CodeStatement>>& statements, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    TString classState(statement->statement);
    auto pieces = classState.split(L' ');

    TClassStruct s, c;
    bool hasParent = false;

    switch (pieces->Size())
    {
    case 3:
        // make sure middle word is extended
        if (pieces->at(1).Compare(L"extends"))
        {
            PrepReturn(ret, L"Expected keyword 'extends' in Class Declaration", L"", ReturnObject::ERR_INCOMPLETE_STATEMENT, statement->lineStart);
            return;
        }

        if (!this->GetClass(pieces->at(2), s))
        {
            TString message;
            message.Format(L"Expected Superclass %ws does not exist in the required scope!", pieces->at(2).GetConstantBuffer());
            PrepReturn(ret, message, L"", ReturnObject::ERR_IMPROPER_TYPE, statement->lineStart);
            return;
        }
        hasParent = true;
    case 1:
        CheckVarName(pieces->at(0), ret);
        if (ret.returnCode)
            return;
        break;
    default:
        PrepReturn(ret, L"Malformed Class Statement detected!", L"", ReturnObject::ERR_INCOMPLETE_STATEMENT, statement->lineStart);
        return;
    }

    if (!statement->statementVar.Get())
    {
        TString message;
        message.Format(L"INTERNAL ERROR! Failed to set up Js-Class Interpretor for class %ws", pieces->at(0).GetConstantBuffer());
        PrepReturn(ret, message, L"", ReturnObject::ERR_INTERNAL, statement->lineStart);
        return;
    }

    TrecSubPointer<TVariable, TcJavaScriptClassInterpretor> jsClass = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcJavaScriptClassInterpretor>(statement->statementVar);

    jsClass->SetClassName(pieces->at(0));
    if (hasParent)
        jsClass->SetSuperClassName(pieces->at(2));

    jsClass->SetStatements(statement->block);

    jsClass->ProcessStatements(ret);
    if (ret.returnCode)
        return;

    TClassStruct newClass = jsClass->GetClassData();

    if (!this->SubmitClassType(pieces->at(0), newClass, false))
    {
        TString message;
        message.Format(L"Failed to set up Js-Class Interpretor for class %ws, class likely already exists", pieces->at(0).GetConstantBuffer());
        PrepReturn(ret, message, L"", ReturnObject::ERR_IMPROPER_NAME, statement->lineStart);
        return;
    }

    if(statement->next.Get())
    Run(statements, index, statement->next);
}

void TcJavaScriptInterpretor::ProcessPrototypeAssign(TrecPointer<CodeStatement> state, ReturnObject& ret)
{
    assert(state.Get() && state->statementType == code_statement_type::cst_virtual_assign);

    if (!this->methodObject.Get() || methodObject->GetVarType() != var_type::collection)
    {
        TString message;
        message.Format(L"Error Setting Prototype Attribute %ws", state->statement.GetConstantBuffer());
        PrepReturn(ret, message, L"", ReturnObject::ERR_BROKEN_REF, 0);
        return;
    }

    TrecSubPointer<TVariable, TContainerVariable> mObject = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(methodObject);

    mObject->SetValue(state->statement, state->statementVar.Get() ? state->statementVar->Clone() : state->statementVar);
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
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer().getBuffer() : L"null");
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
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer().getBuffer() : L"null");
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

TrecPointer<TVariable> TcJavaScriptInterpretor::GetVariable(TString& varName, bool& present, bool currentScope)
{
    TrecPointer<TVariable> ret = TcInterpretor::GetVariable(varName, present, currentScope);

    if (!ret.Get() && !varName.Compare(L"super") && dynamic_cast<TContainerVariable*>(methodObject.Get()))
    {
        present = true;
        ret = dynamic_cast<TContainerVariable*>(methodObject.Get())->GetValue(varName, present);
    }
    return ret;
}

UINT TcJavaScriptInterpretor::ProcessExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret )
{
    if (!statement->statement.GetSize() || !statement->statement.GetTrim().Compare(L';'))
        return 0;

    TDataArray<JavaScriptExpression2> expressions;
    TDataArray<TString> ops;
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
                PrepReturn(ret, L"Unexpected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            parenth++;
            nodes = ProcessExpression(parenth, square, ++index, statement, ret);
            break;
        case L'[':
            if (processed)
            {
                PrepReturn(ret, L"Unexpected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            square++;
            nodes = ProcessArrayExpression(parenth, square, ++index, statement, ret);
            break;
        case L'\'':
        case L'\"':
        case L'`':
            if (processed)
            {
                PrepReturn(ret, L"Unexpected '(' token detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                return fullNodes;
            }
            processed = true;
            nodes = ProcessStringExpression(parenth, square, index, statement, ret);
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
            nodes = ProcessNumberExpression(parenth, square, index, statement, ret);

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
            nodes = ProcessVariableExpression(parenth, square, index, statement, ret);
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

            if (GetVarStatus(jExp.varName) == 2)
            {
                TString message;
                message.Format(L"Error! Cannot %ws on const variable %ws!", inc ? L"increment" : L"decrement", jExp.varName.GetConstantBuffer());
                PrepReturn(ret, message, L"", ReturnObject::ERR_UNSUPPORTED_OP, statement->lineStart);
                return fullNodes;
            }


            // Now handle the increment or decrement step

            ProcessIndividualStatement(jExp.varName, ret);

            TrecSubPointer<TVariable, TPrimitiveVariable> tempValue = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(ret.errorObject);

            if (!tempValue.Get())
            {
                ret.returnCode = ret.ERR_BROKEN_REF;

                ret.errorMessage.Format(L"Post-%ws attempt encountered %ws variable: Could not convert to Primitive Variable!",
                    inc ? L"Increment" : L"Decrement", L"Null");
                return fullNodes;
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
                    jExp.varName.GetConstantBuffer(), inc ? L"increment" : L"decrement");
                PrepReturn(ret, message, L"", ReturnObject::ERR_IMPROPER_TYPE, statement->lineStart);
                return fullNodes;
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
            if (!exp.FindOneOf(L"+-*/%^&|<>?=~!"))
            {
                ops.push_back(TString(exp[0]));
                exp.Delete(0, 1);
                foundOp = true;
            }
        }

        if (foundOp)
        {
            index += (ops.at(ops.Size() -1).GetSize() -1);
            processed = false;
            continue;
        }

        if (index < statement->statement.GetSize())
            ch = statement->statement[index];

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
            parenth--;
        case L',':
        case L';':
            index++;

            if (!expressions.Size() && !ops.Size())
                return fullNodes;
            goto crunch;
        }
        
    }

    if (index == statement->statement.GetSize() && statement->block.Size())
    {
        ProcessJsonExpression(parenth, square, index, statement, ret);
        if (ret.returnCode)
            return fullNodes;
        JavaScriptExpression2 exp;
        exp.value = ret.errorObject;
        ret.errorObject.Nullify();
        exp.varName.Set(ret.errorMessage);
        expressions.push_back(exp);
        fullNodes++;
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

    ret.errorObject2.Nullify();

    return fullNodes;
}

UINT TcJavaScriptInterpretor::ProcessArrayExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
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
        nodes = ProcessExpression(parenth, square, index, statement, ret);
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

UINT TcJavaScriptInterpretor::ProcessJsonPiece(TrecPointer<CodeStatement> statement, const TString& piece, TrecSubPointer<TVariable, TContainerVariable> obj, UINT stateIndex, ReturnObject& ret)
{
    auto pieces = piece.splitn(L':', 2);

    if (pieces->Size() == 1)
    {
        UINT parenth = 0, square = 0;
        return ProcessExpression(parenth, square, stateIndex, statement, ret);
    }

    if (pieces->Size() == 2)
    {
        CheckVarName(pieces->at(0).GetTrim(), ret);
        if (ret.returnCode)
            return 0;
        UINT p = 0, s = 0, i = statement->statement.Find(L':', stateIndex) + 1;
        ProcessExpression(p, s, i, statement, ret);
        // ProcessIndividualStatement(pieces->at(1), ret);
        if (ret.returnCode)
            return 0;

        obj->SetValue(pieces->at(0).GetTrim(), ret.errorObject);
        ret.errorObject.Nullify();
    }
    return 0;
}


UINT TcJavaScriptInterpretor::ProcessJsonExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    TrecSubPointer<TVariable, TContainerVariable> obj = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    auto tempMethodObj = methodObject;
    methodObject = TrecPointerKey::GetTrecPointerFromSub<>(obj);

    for (UINT Rust = 0; Rust < statement->block.Size(); Rust++)
    {
        TrecPointer<CodeStatement> curStatement = statement->block[Rust];
        TString subState;
        WCHAR quote = L'\0';
        UINT subStart = 0;
        UINT transfer = 0;

        UINT newParenth = 0, newSquare = 0;
    throughStatement:
        for (UINT C = 0; C < curStatement->statement.GetSize(); C++)
        {
            WCHAR ch = curStatement->statement[C];

            if (!quote && !newParenth && !newSquare && ch == L',')
            {
                
                ProcessJsonPiece(curStatement, subState, obj, subStart, ret);
                subStart = C + 1;
                if (ret.returnCode)
                    return 0;
                subState.Empty();
                continue;
            }

            switch (ch)
            {
            case L'\'':
            case L'\"':
            case L'`':
                if (!quote)
                    quote = ch;
                else if (quote == ch)
                    quote = L'\0';
                break;

            case L'(':
                if (!quote)
                    newParenth++;
                break;
            case L'[':
                if (!quote)
                    newSquare++;
                break;
            case L')':
                if (!quote)
                {
                    if (!newParenth)
                    {
                        PrepReturn(ret, L"Unexpected ')' detected!", L"", ReturnObject::ERR_PARENTH_MISMATCH, curStatement->lineStart);
                        return 0;
                    }
                    newParenth--;
                }
                break;
            case L']':
                if (!quote)
                {
                    if (!newSquare)
                    {
                        PrepReturn(ret, L"Unexpected ']' detected!", L"", ReturnObject::ERR_BRACKET_MISMATCH, curStatement->lineStart);
                        return 0;
                    }
                    newSquare--;
                }
            }

            subState.AppendChar(ch);

        }
        subState.Trim();
        if (subState.GetSize())
        {
            transfer = ProcessJsonPiece(curStatement, subState, obj, subStart, ret);
            if (ret.returnCode)
                return 0;

            while (transfer-- && curStatement->next.Get())
                curStatement = curStatement->next;
        }
        curStatement = curStatement->next;

        if (curStatement.Get())
        {
            quote = L'\0';
            subStart = 0;
            transfer = 0;

            newParenth = 0; newSquare = 0;
            subState.Empty();
            goto throughStatement;
        }
    }

    ret.errorObject = methodObject;
    methodObject = tempMethodObj;


    // Check to see if the object should be converted into an accessor variable
    bool ok = obj->GetSize() > 0;
    TString key;
    TrecPointer<TVariable> tVar;
    TrecSubPointer<TVariable, TAccessorVariable> access = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAccessorVariable>();
    for (UINT Rust = 0; obj->GetValueAt(Rust, key, tVar); Rust++)
    {
        if (!key.Compare(L"get"))
        {
            if (dynamic_cast<TcInterpretor*>(tVar.Get()))
                access->SetGetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(tVar));
        }
        else if (!key.Compare("set"))
        {
            if (dynamic_cast<TcInterpretor*>(tVar.Get()))
                access->SetSetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(tVar));
        }
        else
        {
            ok = false;
            break;
        }
    }

    if (ok)
        ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(access);

    return 1;
}

UINT TcJavaScriptInterpretor::ProcessVariableExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
{
    TDataArray<TString> pieces;

    TString phrase, wholePhrase;
    bool spaceDetected = false;
    UINT uret = 0;

    UCHAR attribute = 0;
    bool loopAround;
    bool isAsync = false;

    TrecPointer<TVariable> var, vThis;

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
                        message.Format(L"Failed to id Method/Attribute %ws for primitive variable %ws!", phrase.GetConstantBuffer(), wholePhrase.GetConstantBuffer());
                        PrepReturn(ret, message, L"", ReturnObject::ERR_BROKEN_REF, statement->lineStart);
                        return uret;
                    }
                }
                else
                {
                    PrepReturn(ret, L"INTERNAL ERROR! Failed to load 'Number' package of methods needed to process this variable", L"", ReturnObject::ERR_INTERNAL, statement->lineStart);
                    return uret;
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
        }
        else
        {
            bool pres;
            var = GetVariable(phrase, pres);
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
                    return 0;
                }
                if (contThis.Get())
                {
                    // If Present, will attempt to call it's constructor
                    vThis = TrecPointerKey::GetTrecPointerFromSub<>(contThis);

                    

                    TrecPointer<TVariable> att = contThis->GetValue(L"constructor", present);

                    if (dynamic_cast<TcJavaScriptInterpretor*>(att.Get()))
                    {
                        uret += ProcessProcedureCall(++parenth, square, ++index, vThis, att, statement, ret);
                    }
                    
                }
                else
                {
                    // No Class Detected, Check for a function
                    vThis = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

                    TrecPointer<TVariable> func = this->GetVariable(wholePhrase, present);

                    if (dynamic_cast<TcJavaScriptInterpretor*>(func.Get()))
                    {
                        uret += ProcessProcedureCall(++parenth, square, ++index, vThis, func, statement, ret);
                    }
                }
                index = static_cast<UINT>(iIndex) + 1;

          
                var = vThis;
                
            }
            else
            {
                // Likely dealing with function defintion
                ret.errorObject.Nullify();
                UINT jumps = ProcessFunctionExpression(parenth, square, index, statement, ret);
                index++;
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
                        //wholePhrase.Delete(0, 3);
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
                                if (attribute == 2)
                                    tAccess->SetGetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(ret.errorObject));
                                else
                                    tAccess->SetSetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(ret.errorObject));
                            }
                            else if (methodObject->GetVarType() == var_type::accessor)
                            {
                                auto tAccess = dynamic_cast<TAccessorVariable*>(methodObject.Get());
                                if (attribute == 2)
                                    tAccess->SetGetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(ret.errorObject));
                                else
                                    tAccess->SetSetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(ret.errorObject));
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
                        if (isAsync)
                        {
                            if (dynamic_cast<TcJavaScriptInterpretor*>(ret.errorObject.Get()))
                                dynamic_cast<TcJavaScriptInterpretor*>(ret.errorObject.Get())->isAsync = true;
                            else
                            {
                                PrepReturn(ret, L"Unexpected Keyword 'async' detected!", L"", ReturnObject::ERR_UNEXPECTED_TOK, statement->lineStart);
                                return uret;
                            }
                        }
                    }
                    var = ret.errorObject;
                    loopAround = false;
                }
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


    return uret;
}

UINT TcJavaScriptInterpretor::ProcessFunctionExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
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

    ret = function->PreProcess();

    if (ret.returnCode)
        return 0;

    ret.errorObject = TrecPointerKey::GetTrecPointerFromSub<>(function);
    ret.errorMessage.Set(statement->statement.SubString(index, startP).GetTrim());
    index = endP;

    // Collect local variables to create a working closure
    for (UINT Rust = 0; Rust < variables.count(); Rust++)
    {
        TDataEntry<TcVariableHolder> entry;
        if (!variables.GetEntryAt(Rust, entry))
            continue;

        if (entry.key.GetSize())
        {
            auto tempVar = entry.object.value;
            TcVariableHolder mark;
            mark.mut = entry.object.mut;
            mark.value = (tempVar.Get() ? tempVar->Clone() : tempVar);
            function->strictVariables.addEntry(entry.key, mark);
        }
    }

    return 0;
}

UINT TcJavaScriptInterpretor::ProcessNumberExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
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
        message.Format(L"Failed to convert token '%ws' to a number!", numStr.GetConstantBuffer());
        PrepReturn(ret, message, L"", ReturnObject::ERR_NOT_NUMBER, statement->lineStart);
    }
    return 0;
}

UINT TcJavaScriptInterpretor::ProcessStringExpression(UINT& parenth, UINT& square, UINT& index, TrecPointer<CodeStatement> statement, ReturnObject& ret)
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
            return 0;
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

    return 0;
}

UINT TcJavaScriptInterpretor::ProcessProcedureCall(UINT& parenth, UINT& square, UINT& index, TrecPointer<TVariable> object, TrecPointer<TVariable> proc,
    TrecPointer<CodeStatement> statement, ReturnObject& ret, bool doAwait)
{
    UINT curParenth = parenth;
    UINT nextRet = 0;

    TDataArray<TrecPointer<TVariable>> params;

    // Retrieve the Parent Object if Super is being called
    if (statement->statement.StartsWith(L"super", false, true) || statement->statement.StartsWith(L"super("))
    {
        if (dynamic_cast<TContainerVariable*>(methodObject.Get()))
        {
            bool present;
            object = dynamic_cast<TContainerVariable*>(methodObject.Get())->GetValue(L"__proto__", present);
        }
    }


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
        UINT uret = ProcessExpression(parenth, square, index, statement, ret);
        nextRet += uret;

        // If an error was detected, return
        if (ret.returnCode)
            return nextRet;

        // If blocks were detected, go through and retrieve the current statement we are on
        while (uret && statement->next.Get())
        {
            uret--;
            statement = statement->next;
            index = 0;
        }
        // Add the parameter
        params.push_back(ret.errorObject);
    }

    // Convert to a function we can call
    TrecSubPointer<TVariable, TcInterpretor> function;
    // Check to see if we have an accessor variable, if we do, then assess variable count and choose the function within

    if (proc->GetVarType() == var_type::accessor)
    {
        TrecSubPointer<TVariable, TAccessorVariable> accProc = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TAccessorVariable>(proc);
        assert(accProc.Get());
        if (!params.Size())
        {            
            function = accProc->GetCGetter();
            if (!function.Get())
                function = accProc->GetCSetter();
        }
        else
        {
            function = accProc->GetCSetter();
            if (!function.Get())
                function = accProc->GetCGetter();
        }
    }
    else if (proc->GetVarType() == var_type::interpretor)
    {
        function = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(proc);
    }
    else if (proc->GetVarType() == var_type::interpretor_gen)
    {
        function = dynamic_cast<TFunctionGen*>(proc.Get())->Generate(params);
        assert(function.Get());
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TIteratorVariable>(function);
        return nextRet;
    }

    if (!function.Get())
    {
        PrepReturn(ret, L"found Null or Undefined Getter/Setter!", L"", ReturnObject::ERR_BROKEN_REF, statement->lineStart);
        return nextRet;
    }
    
    assert(function.Get());


    TrecSubPointer<TVariable, TcTypeInterpretor> typeFunction = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcTypeInterpretor>(proc);
    if (typeFunction.Get() && typeFunction->IsAsync() && !doAwait)
    {
        auto asyncObj = typeFunction->PrepAsyncCall();

        // To-Do: Respond to async Object once one is Fleshed out
    }
    else if (doAwait)
    {
        // Calling 'await' on a non async function is an error
        ret.returnCode = ReturnObject::ERR_IMPROPER_TYPE;
        ret.errorMessage.Set(L"Cannot call await on non-async function");
        return nextRet;
    }

    // This is a method if object is valid
    function->SetActiveObject(object);
    // Set the parameters
    function->SetIntialVariables(params);

    // Run the function and get the results
    ret = function->Run();

    // Inspect the results
    if (ret.returnCode)
    {
        // Add Stack info
    }
    else
    {
        ret.mode = return_mode::rm_regular;
    }


    // Return how many statments were jumped
    return nextRet;
}

void TcJavaScriptInterpretor::ProcessFunctionExpression(TrecPointer<CodeStatement> statement, ReturnObject& obj)
{
    bool isAsync = statement->statement.StartsWith(L"async ");
    UINT p = 0, s = 0, i = isAsync ? 6 : 0;
    ProcessFunctionExpression(p, s, i, statement, obj);

    if (!obj.errorObject.Get())
    {
        TString message;
        message.Format(L"Statement '%ws' Failed to generate a function variable!", statement->statement.GetConstantBuffer());
        PrepReturn(obj, message, L"", ReturnObject::ERR_INCOMPLETE_STATEMENT ,statement->lineStart);
        return;
    }
    if (!obj.errorMessage.GetSize())
    {
        TString message;
        message.Format(L"Statement '%ws' Failed to provide a function name!", statement->statement.GetConstantBuffer());
        PrepReturn(obj, message, L"", ReturnObject::ERR_INCOMPLETE_STATEMENT, statement->lineStart);
        return;
    }
    CheckVarName(obj.errorMessage, obj);
    if (obj.returnCode)
        return;

    if (statement->statementType == code_statement_type::cst_function_gen)
    {
        TrecSubPointer<TVariable, TcInterpretor> newFunc = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(obj.errorObject);
        assert(newFunc.Get());
        obj.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TFunctionGen>(newFunc);
    }

    if (isAsync)
    {
        dynamic_cast<TcJavaScriptInterpretor*>(obj.errorObject.Get())->isAsync = true;
    }

    UpdateVariable(obj.errorMessage, obj.errorObject, true);
    UINT run = 0;
    if (statement->next.Get())
        obj = Run(statement->block, run, statement->next);
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
            auto pieces = expressions[Rust].varName.split(L'.', 3);
            if (pieces->Size() == 3 && !pieces->at(1).Compare(L"prototype"))
            {
                AddAssignStatement(pieces->at(0), pieces->at(2), right, ro);
                return;
            }
        }
        else if (!ops[Rust].Compare(L"+="))
        {
            found = true;
            ro.errorObject = jsOperators->Add(left, right);
        }
        else if (!ops[Rust].Compare(L"-="))
        {
            found = true;
            ro.errorObject = jsOperators->Subtract(left, right);
        }
        else if (!ops[Rust].Compare(L"**="))
        {
            found = true;
            ro.errorObject = jsOperators->Pow(left, right);
        }
        else if (!ops[Rust].Compare(L"*="))
        {
            ro.errorObject = jsOperators->Multiply(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"/="))
        {
            ro.errorObject = jsOperators->Divide(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"%="))
        {
            ro.errorObject = jsOperators->Mod(left, right);
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

TrecPointer<TVariable> TcJavaScriptInterpretor::GetJsConsole()
{
    TrecPointer<TVariable> console = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    TContainerVariable* containerConsole = dynamic_cast<TContainerVariable*>(console.Get());

    containerConsole->SetValue(L"counter", TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>((UINT)0));

    auto fullSelf = TrecPointerKey::GetSubPointerFromSoft<TVariable, TcInterpretor>(self);

    containerConsole->SetValue(L"assert", JsConsole::GetAssert(fullSelf, environment));
    containerConsole->SetValue(L"clear", JsConsole::GetClear(fullSelf, environment));
    containerConsole->SetValue(L"count", JsConsole::GetCount(fullSelf, environment));
    containerConsole->SetValue(L"error", JsConsole::GetError(fullSelf, environment));
    containerConsole->SetValue(L"group", JsConsole::GetGroup(fullSelf, environment));
    containerConsole->SetValue(L"groupEnd", JsConsole::GetGroupEnd(fullSelf, environment));
    containerConsole->SetValue(L"info", JsConsole::GetInfo(fullSelf, environment));
    containerConsole->SetValue(L"log", JsConsole::GetLog(fullSelf, environment));
    containerConsole->SetValue(L"time", JsConsole::GetTime(fullSelf, environment));
    containerConsole->SetValue(L"timeEnd", JsConsole::GetTimeEnd(fullSelf, environment));
    containerConsole->SetValue(L"warn", JsConsole::GetWarn(fullSelf, environment));


    return console;
}

void TcJavaScriptInterpretor::AddAssignStatement(const TString& type, const TString& att, TrecPointer<TVariable> value, ReturnObject& ret)
{
    CheckVarName(att, ret);
    if (ret.returnCode)
        return;

    TClassStruct classStruct;
    if (GetClass(type, classStruct))
    {
        TClassAttribute cons = classStruct.GetAttributeByName(L"constructor");
        TcJavaScriptInterpretor* jsInt = dynamic_cast<TcJavaScriptInterpretor*>(cons.def.Get());
        if (cons.name.GetSize() && cons.def.Get() && cons.def->GetVarType() == var_type::interpretor)
        {
            TrecPointer<CodeStatement> newState = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
            newState->statement.Set(att);
            newState->statementVar = value.Get() ? value->Clone() : value;
            newState->statementType = code_statement_type::cst_virtual_assign;
            jsInt->statements.push_back(newState);
        }
        else
        {
            cons.name.Set(att);
            cons.def = value.Get() ? value->Clone(): value;
            classStruct.AddAttribute(cons);
            this->SubmitClassType(type, classStruct, true);
        }
    }
    else
    {
        // Check for a method with that name
        bool present;
        TString tType(type);
        TrecPointer<TVariable> func = GetVariable(tType, present);

        TcJavaScriptInterpretor* jsInt = dynamic_cast<TcJavaScriptInterpretor*>(func.Get());
        if (!jsInt)
        {
            TString message;
            message.Format(L"Failed to Find Class or Function with the name %ws", type.GetConstantBuffer());
            PrepReturn(ret, message, L"", ReturnObject::ERR_BROKEN_REF, 0);
            return;
        }
        TrecPointer<CodeStatement> newState = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
        newState->statement.Set(att);
        newState->statementVar = value.Get() ? value->Clone() : value;
        newState->statementType = code_statement_type::cst_virtual_assign;
        jsInt->statements.push_back(newState);
    }
}

TrecSubPointer<TVariable, TContainerVariable> TcJavaScriptInterpretor::ConstructObject(const TString& type)
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
                vThis->SetValue(L"__proto__", TrecPointerKey::GetTrecPointerFromSub<>(protoThis));
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
