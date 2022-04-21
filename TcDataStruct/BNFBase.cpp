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

    if (!name.Compare(L"")) {

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
