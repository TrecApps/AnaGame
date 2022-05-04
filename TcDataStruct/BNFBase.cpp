#include "BNFBase.h"

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

void BNFIf::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFElse::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFNumber::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFTypeBlock::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFTypeState::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFBlock::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFFinally::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFCatch::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFTry::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFFor::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFFor3::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFSwitchBlock::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFSwitch::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}

void BNFWhile::Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile, TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement, UINT stringStart, bool expectTerminal)
{
}
