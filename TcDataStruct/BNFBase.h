#pragma once
#include <TString.h>

class BNFBase
{
public:
	class BNFToken {
	public:
		virtual bool IsRawToken() = 0;
		BNFToken(const TString& token);
		TString token;
	};

	class BNFStringToken : public BNFToken
	{
	public:
		virtual bool IsRawToken();
		BNFStringToken(const TString& token);
	};

	class BNFReferenceToken : public BNFToken
	{
	public:
		virtual bool IsRawToken();
		BNFReferenceToken(const TString& token);
	};

protected:

	TDataArray<TDataArray<TrecPointer<BNFToken>>> tokenList;

public:

	static TrecPointer<BNFBase> GetBnf(const TString& name, TDataArray<TString>& tokens);


};

