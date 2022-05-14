#include "BNFBase.h"
#include "TContainerVariable.h"

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

void BNFString::Compile(TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
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
