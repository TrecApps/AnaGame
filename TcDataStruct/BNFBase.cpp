#include "BNFBase.h"
#include "TContainerVariable.h"

typedef struct StatementTypeMapEntry
{
    TString bnf;
    tc_statement_type type;
}StatementTypeMapEntry;

TDataArray<StatementTypeMapEntry> statementBnfMap;

void PrepBnfMap()
{
    if (statementBnfMap.Size())return;

    statementBnfMap.push_back({ L"", tc_statement_type::_unset });
    statementBnfMap.push_back({ L"global_decl", tc_statement_type::_var });
    statementBnfMap.push_back({ L"local_decl", tc_statement_type::_let });
    statementBnfMap.push_back({ L"inmut_decl", tc_statement_type::_const });
    statementBnfMap.push_back({ L"", tc_statement_type::_type });
    statementBnfMap.push_back({ L"", tc_statement_type::_declare });

    // Flow Blocks

    statementBnfMap.push_back({ L"if_statement", tc_statement_type::_if });
    statementBnfMap.push_back({ L"else_statement", tc_statement_type::_else });
    statementBnfMap.push_back({ L"", tc_statement_type::_else_if });
    statementBnfMap.push_back({ L"while_statement", tc_statement_type::_while });
    statementBnfMap.push_back({ L"for_loop", tc_statement_type::_for });
    statementBnfMap.push_back({ L"for_3_loop", tc_statement_type::_for_3 });
    statementBnfMap.push_back({ L"do_while", tc_statement_type::_do });
    statementBnfMap.push_back({ L"", tc_statement_type::_until });
    statementBnfMap.push_back({ L"try_block", tc_statement_type::_try_ });
    statementBnfMap.push_back({ L"catch_block", tc_statement_type::_catch });
    statementBnfMap.push_back({ L"finally_block", tc_statement_type::_finally_ });
    statementBnfMap.push_back({ L"throw", tc_statement_type::_throw });
    statementBnfMap.push_back({ L"switch_statement", tc_statement_type::_switch });
    statementBnfMap.push_back({ L"", tc_statement_type::_case });
    statementBnfMap.push_back({ L"", tc_statement_type::_default });
    statementBnfMap.push_back({ L"", tc_statement_type::_match });


    statementBnfMap.push_back({ L"", tc_statement_type::_goto });
    statementBnfMap.push_back({ L"", tc_statement_type::_goto_target });
    statementBnfMap.push_back({ L"", tc_statement_type::_return });
    statementBnfMap.push_back({ L"break", tc_statement_type::_break });
    statementBnfMap.push_back({ L"continue", tc_statement_type::_continue });
    statementBnfMap.push_back({ L"", tc_statement_type::_yield });
    statementBnfMap.push_back({ L"", tc_statement_type::_end });


    statementBnfMap.push_back({ L"function_statement", tc_statement_type::_function });
    statementBnfMap.push_back({ L"", tc_statement_type::_function_gen });
    statementBnfMap.push_back({ L"", tc_statement_type::_method });
    statementBnfMap.push_back({ L"", tc_statement_type::_constructor });
    statementBnfMap.push_back({ L"", tc_statement_type::_destructor });
    statementBnfMap.push_back({ L"", tc_statement_type::_class });
    statementBnfMap.push_back({ L"", tc_statement_type::_union });
    statementBnfMap.push_back({ L"", tc_statement_type::_enum });
    statementBnfMap.push_back({ L"", tc_statement_type::_delete });


    statementBnfMap.push_back({ L"", tc_statement_type::_module });
    statementBnfMap.push_back({ L"", tc_statement_type::_include });

    statementBnfMap.push_back({ L"", tc_statement_type::_virtual_assign });
    statementBnfMap.push_back({ L"expression", tc_statement_type::_regular });
}


void ProcessSeriesOfStatements(TDataArray<TrecPointer<TcBaseStatement>> statements, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails,
    TDataMap<TrecPointer<BNFBase>>& bnfs, TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner)
{
    PrepBnfMap();
    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        switch (statements[Rust]->type)
        {
        case tc_statement_type::_else:
        case tc_statement_type::_else_if:
            if (!Rust || (statements[Rust - 1]->type != tc_statement_type::_if &&
                statements[Rust - 1]->type != tc_statement_type::_else &&
                statements[Rust - 1]->type != tc_statement_type::_else_if))
            {
                CompileMessage m;
                m.isError = true;
                m.line = statements[Rust]->startLine;
                m.message.Set(L"Compile Error! Expected 'if' or and 'else if' statement before current 'else' block!");
                messages.push_back(m);
            }
            break;
        case tc_statement_type::_catch:
            if (!Rust || (statements[Rust - 1]->type != tc_statement_type::_try_ &&
                statements[Rust - 1]->type != tc_statement_type::_catch))
            {
                CompileMessage m;
                m.isError = true;
                m.line = statements[Rust]->startLine;
                m.message.Set(L"Compile Error! Expected 'catch' or 'try' statement before current 'catch' block!");
                messages.push_back(m);
            }
            break;
        case tc_statement_type::_finally_:
            if (!Rust || (statements[Rust - 1]->type != tc_statement_type::_try_ &&
                statements[Rust - 1]->type != tc_statement_type::_catch))
            {
                CompileMessage m;
                m.isError = true;
                m.line = statements[Rust]->startLine;
                m.message.Set(L"Compile Error! Expected 'catch' or 'try' statement before current 'finally' block!");
                messages.push_back(m);
            }
            break;
        }

        TString bnfExp(statementBnfMap.at(static_cast<UINT>(statements[Rust]->type)).bnf);

        if (bnfExp.IsEmpty())
            bnfExp.Set(L"expression");

        TrecPointer<BNFBase> bnf;
        UINT stringStart = 0;
        if (!bnfs.retrieveEntry(bnfExp, bnf))
        {
            CompileMessage m;
            m.isError = true;
            m.line = statements[Rust]->startLine;
            m.message.Format(L"Language Error! BNF '%ws' not available!", bnfExp.GetConstantBuffer().getBuffer());
            messages.push_back(m);
        }

        else bnf->Compile(statements[Rust]->bnfLine, messages, languageDetails, statements[Rust], bnfs, stringStart, vars, runners, currentRunner, false);

    }
}

bool BNFBase::BNFStringToken::IsRawToken()
{
    return true;
}

BNFBase::BNFStringToken::BNFStringToken(const TString& token): BNFToken(token)
{
}

bool BNFBase::BNFReferenceToken::IsRawToken()
{
    return false;
}

BNFBase::BNFReferenceToken::BNFReferenceToken(const TString& token): BNFToken(token)
{
}

BNFBase::BNFToken::BNFToken(const TString& token)
{
    this->token.Set(token);
}

TrecPointer<BNFBase> BNFBase::GetBnf(const TString& name, TDataArray<TString>& tokens)
{
    TrecPointer<BNFBase> ret;

    if (!name.Compare(L"if_statement")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFIf>();
    }
    else if(!name.Compare(L"else_statement")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFElse>();
    }
    else if (!name.Compare(L"switch_statement")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFSwitch>();
    }
    else if (!name.Compare(L"while_statement")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFWhile>();
    }
    else if (!name.Compare(L"for_3_loop")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFFor3>();
    }
    else if (!name.Compare(L"for_loop")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFFor>();
    }
    else if (!name.Compare(L"do_while")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFWhile>();
    }
    else if (!name.Compare(L"try_block")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFTry>();
    }
    else if (!name.Compare(L"catch_block")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFCatch>();
    }
    else if (!name.Compare(L"finally_block")) {
        ret = TrecPointerKey::GetNewTrecPointerAlt<BNFBase, BNFFinally>();
    }
    else
    {
        ret = TrecPointerKey::GetNewTrecPointer<BNFBase>();
    }
    assert(ret.Get());

    for (UINT Rust = 0; Rust < tokens.Size(); Rust++)
    {
        TString tokenString(tokens[Rust]);
        auto tokens = tokenString.split(L" ");

        TDataArray<TrecPointer<BNFToken>> bnfTokens;

        for (UINT C = 0; C < tokens->Size(); C++)
        {
            TString bnfToken(tokens->at(C));
            if (bnfToken.StartsWith(L"<<") && bnfToken.EndsWith(L">>"))
                bnfTokens.push_back(TrecPointerKey::GetNewTrecPointerAlt<BNFToken, BNFReferenceToken>(bnfToken.SubString(2, bnfToken.GetSize() - 2)));
            else
                bnfTokens.push_back(TrecPointerKey::GetNewTrecPointerAlt<BNFToken, BNFStringToken>(bnfToken));
        }

        if (bnfTokens.Size());
        ret->tokenList.push_back(bnfTokens);
    }

    return ret;
}

bool BNFBase::IsStatement(TDataMap<TrecPointer<BNFBase>>& otherBnfs, const TString& statement, UINT& stringStart, bool expectTerminal)
{
    bool ret = true;
    
    for (UINT Rust = 0; Rust < tokenList.Size(); Rust++)
    {
        bool isRustViable = true;
        UINT tempStartPoint = stringStart;
        for (UINT C = 0; C < tokenList[Rust].Size() && isRustViable; C++)
        {
            auto token = tokenList[Rust][C];
            if (token->IsRawToken())
            {
                int index = statement.Find(token->token, tempStartPoint);
                TString check(statement.SubString(tempStartPoint, index).GetTrim());
                if (check.GetSize() || index == -1) {
                    ret = isRustViable = false; 
                    break;
                }

                tempStartPoint = index + token->token.GetSize();
            }
            else
            {
                TString tokenTarget(token->token);

                // Check to see if this BNF Tag must be present
                bool loose = tokenTarget.StartsWith(L'?');
                if (loose)
                    tokenTarget.Delete(0);

                auto bnfPieces = tokenTarget.splitn(L':', 2);
                TrecPointer<BNFBase> bnf;
                if (!otherBnfs.retrieveEntry(bnfPieces->at(0), bnf))
                    return false;
                bool works = bnf->IsStatement(otherBnfs, statement, tempStartPoint);

                isRustViable = works || loose;
                if (!works)
                    tempStartPoint++;
            }
        }

        if (isRustViable)
        {
            ret = isRustViable;
            stringStart = tempStartPoint;
            break;
        }
    }
    return ret;
}

void BNFString::Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
    UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, bool expectTerminal)
{
    TrecSubPointer<TVariable, TContainerVariable> langDets = TrecPointerKey::GetTrecSubPointerFromTrec < TVariable, TContainerVariable>(languageDetails);

    assert(langDets.Get());

    TDataArray<TString> singleString, multiString, templateBounds;

    TrecPointer<TVariable> v;
    TrecSubPointer<TVariable, TContainerVariable> sv;
    bool pres; 


    v = langDets->GetValue(L"StatementParsing::SingleString", pres, L"::");
    sv = TrecPointerKey::GetTrecSubPointerFromTrec < TVariable, TContainerVariable>(v);
    if (sv.Get())
        sv->CollectAsStrings(singleString);

    v = langDets->GetValue(L"StatementParsing::MultiString", pres, L"::");
    sv = TrecPointerKey::GetTrecSubPointerFromTrec < TVariable, TContainerVariable>(v);
    if (sv.Get())
        sv->CollectAsStrings(multiString);

    v = langDets->GetValue(L"String::TemplateBounds", pres, L"::");
    sv = TrecPointerKey::GetTrecSubPointerFromTrec < TVariable, TContainerVariable>(v);
    if (sv.Get())
        sv->CollectAsStrings(templateBounds);

    TString starter;
    
    TString statementStr(statement->statementText.SubString(stringStart));

    for (UINT Rust = 0; Rust < singleString.Size(); Rust++)
    {
        if (statementStr.StartsWith(singleString[Rust]))
        {
            starter.Set(singleString[Rust]);
            int newLineLoc = statementStr.Find('\n', 1, false);
            int endPoint = statementStr.Find(singleString[Rust], 1, false);

            if ((endPoint == -1) || (endPoint < newLineLoc && newLineLoc != -1))
            {
                CompileMessage m;
                m.isError = true;
                m.line = statement->startLine + statement->statementText.SubString(0, stringStart).CountFinds(L'\n');
                m.message.Set(L"Compile Error! Expected Single-line String to end");
                messages.push_back(m);
                return;
            }

            statementStr.Set(statementStr.SubString(starter.GetSize(), endPoint));
            break;
        }
    }

    if(!starter.GetSize())
        for (UINT Rust = 0; Rust < multiString.Size(); Rust++)
        {
            if (statementStr.StartsWith(multiString[Rust]))
            {
                starter.Set(multiString[Rust]);
                int endPoint = statementStr.Find(singleString[Rust], 1, false);

                if ((endPoint == -1))
                {
                    CompileMessage m;
                    m.isError = true;
                    m.line = statement->startLine + statement->statementText.SubString(0, stringStart).CountFinds(L'\n');
                    m.message.Set(L"Compile Error! Expected Multi-line String to end");
                    messages.push_back(m);
                    return;
                }

                statementStr.Set(statementStr.SubString(starter.GetSize(), endPoint));
                break;
            }
        }

    if (!starter.GetSize())
        return;

    bool useTemplates = starter.FindInContainer(templateBounds);

    TrecSubPointer<TcExpression, TcStringExpression> exp = TrecPointerKey::GetNewSelfTrecSubPointer<TcExpression, TcStringExpression>(statementStr);

    bool works = true;

    if (useTemplates)
    {
        v = langDets->GetValue(L"String::TemplateStartMarker", pres, L"::");
        TString s(v.Get() ? v->GetString() : L"");
        v = langDets->GetValue(L"String::TemplateEndMarker", pres, L"::");
        TString e(v.Get() ? v->GetString() : L"");

        if (s.GetSize() && e.GetSize())
        {
            TString subExp;
            for (int start = exp->GetSubExpression(0, subExp, s, e); start != -1;
                start = exp->GetSubExpression(start, subExp, s, e))
            {
                TrecPointer<BNFBase> expBnf;
                if (!bnfs.retrieveEntry(L"expression", expBnf) || !dynamic_cast<BNFExpression*>(expBnf.Get()))
                {
                    CompileMessage m;
                    m.isError = true;
                    m.line = statement->startLine + statement->statementText.SubString(0, stringStart).CountFinds(L'\n');
                    m.message.Set(L"Language Error! Expression BNF Object to be generated");
                    messages.push_back(m);
                    return;
                }
                TrecPointer<TcExpression> tSubExp;
                if (dynamic_cast<BNFExpression*>(expBnf.Get())->GenerateExpression(messages, languageDetails, subExp, bnfs, vars, tSubExp))
                    exp->subExpressions.push_back(tSubExp);
                else works = false;
            }
        }
    }

    if (works)
        exp->CompileExpression(vars, runners, currentRunner, messages);
}

bool BNFExpression::GenerateExpression(TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, const TString& e, TDataMap<TrecPointer<BNFBase>>& bnfs,
    TDataArray<VariableContiainer>& vars, TrecPointer<TcExpression>& exp)
{
    return false;
}

void BNFIf::Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs, UINT stringStart, TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, bool expectTerminal)
{
    UINT tempStartPoint = stringStart;

    TrecSubPointer<TVariable, AnagameRunner> runner;
    if (!runners.retrieveEntry(currentRunner, runner))
    {
        // To-Do: Handle Internal Error

        return;
    }

    UINT blockStart = 0;

    for (UINT C = 0; C < this->tokenList[tokenList].Size() ; C++)
    {
        auto token = this->tokenList[tokenList][C];
        if (token->IsRawToken())
        {
            int index = statement->statementText.Find(token->token, tempStartPoint);

            tempStartPoint = index + token->token.GetSize();
        }
        else
        {
            TString tokenTarget(token->token);

            // If Statements have an expression to see if the block should run
            TrecPointer<BNFBase> bnf;
            if (!bnfs.retrieveEntry(tokenTarget, bnf))
            {
                // To-Do: Handle Language Error
                return;
            }

            bnf->Compile(tokenList, messages, languageDetails, statement, bnfs, tempStartPoint, vars, runners, currentRunner, C == this->tokenList[tokenList].Size() - 1);

            if (!blockStart)
            {
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::assess_true));
                blockStart = runner->GetOpCount();
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump_cond));
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump_mark));

            }
            else
            {
                UINT blockEnd = runner->GetOpCount();
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump_mark));
                runner->UpdateOp(AnagameRunner::RunnerCode(runner_op_code::jump_cond, blockStart + 1, blockEnd), blockStart);
            }
        }
    }
}

void BNFElse::Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs, UINT stringStart, TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, bool expectTerminal)
{
    UINT tempStartPoint = stringStart;

    TrecSubPointer<TVariable, AnagameRunner> runner;
    if (!runners.retrieveEntry(currentRunner, runner))
    {
        // To-Do: Handle Internal Error

        return;
    }

    UINT blockStart = 0;

    for (UINT C = 0; C < this->tokenList[tokenList].Size(); C++)
    {
        auto token = this->tokenList[tokenList][C];
        if (token->IsRawToken())
        {
            int index = statement->statementText.Find(token->token, tempStartPoint);

            tempStartPoint = index + token->token.GetSize();
        }
        else
        {
            TString tokenTarget(token->token);

            // If Statements have an expression to see if the block should run
            TrecPointer<BNFBase> bnf;
            if (!bnfs.retrieveEntry(tokenTarget, bnf))
            {
                // To-Do: Handle Language Error
                return;
            }

            bnf->Compile(tokenList, messages, languageDetails, statement, bnfs, tempStartPoint, vars, runners, currentRunner, C == this->tokenList[tokenList].Size() - 1);

        }
    }
}

void BNFWhile::Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs, UINT stringStart, TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, bool expectTerminal)
{
    UINT tempStartPoint = stringStart;

    TrecSubPointer<TVariable, AnagameRunner> runner;
    if (!runners.retrieveEntry(currentRunner, runner))
    {
        // To-Do: Handle Internal Error

        return;
    }

    UINT blockStart = 0;
    UINT expStart = 0;
    bool expProc = false;

    for (UINT C = 0; C < this->tokenList[tokenList].Size(); C++)
    {
        auto token = this->tokenList[tokenList][C];
        if (token->IsRawToken())
        {
            int index = statement->statementText.Find(token->token, tempStartPoint);

            tempStartPoint = index + token->token.GetSize();
        }
        else
        {
            TString tokenTarget(token->token);

            // While Statements have an expression to see if the block should run
            TrecPointer<BNFBase> bnf;
            if (!bnfs.retrieveEntry(tokenTarget, bnf))
            {
                // To-Do: Handle Language Error
                return;
            }
            if (!expProc)
            {
                expProc = true;
                expStart = runner->GetOpCount();
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump_mark));
                
            }

            bnf->Compile(tokenList, messages, languageDetails, statement, bnfs, tempStartPoint, vars, runners, currentRunner, C == this->tokenList[tokenList].Size() - 1);

            if (!blockStart)
            {
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::assess_true));
                blockStart = runner->GetOpCount();
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump_cond));
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump_mark));

            }
            else
            {
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump, expStart));
                UINT blockEnd = runner->GetOpCount();
                runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::jump_mark));
                runner->UpdateOp(AnagameRunner::RunnerCode(runner_op_code::jump_cond, blockStart + 1, blockEnd), blockStart);
            }
        }
    }
}

void BNFTypeBlock::Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement,
    TDataMap<TrecPointer<BNFBase>>& bnfs, UINT stringStart, TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, bool expectTerminal)
{
    vars.push_back(VariableContiainer());
    ProcessSeriesOfStatements(statement->subStatements, messages, languageDetails, bnfs, vars, runners, currentRunner);

    VariableContiainer vc(vars.RemoveAt(vars.Size() - 1));

    UINT pop_o = 0, pop_b = 0;

    for (UINT Rust = 0; Rust < vc.variables.count(); Rust++)
    {
        auto var = vc.variables.GetEntryAt(Rust);

        assert(var.Get());
        if (var->object->useObject)
        {
            pop_o++;
        }
        else
        {
            // To-Do: Handle Binary Mode
        }
    }

    TrecSubPointer<TVariable, AnagameRunner> runner;
    if (!runners.retrieveEntry(currentRunner, runner)) return;

    if (pop_o)
        runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::pop_o, pop_o));
    if (pop_b)
        runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::pop_n, pop_b));

}
