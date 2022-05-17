#pragma once
#include <TString.h>
#include "AnagameRunners.h"
#include "TcCompiler.h"

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

	virtual bool IsStatement(TDataMap<TrecPointer<BNFBase>>& otherBnfs, const TString& statement, UINT& stringStart,bool expectTerminal = true);

	virtual void Compile(UINT tokenList,									// the Token List to use
		TDataArray<CompileMessage>& messages,								// List of Messages to report
		TrecPointer<TVariable> languageDetails,								// Details about the language
		TrecPointer<TcBaseStatement> statement,								// The Statement to compile
		TDataMap<TrecPointer<BNFBase>>& bnfs,								// Collection of BNFs 
		UINT stringStart,													// Location in the statement String to Compile
		TDataArray<VariableContiainer>& vars,								// Series of Containers holding onto variables
		 TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,			// the Series of Runners to work with
		const TString& currentRunner,										// Current Runner to Operate on
		bool expectTerminal = true											// Whether the statement should end once the BNF Tag has gone through successfully
	) = 0;

	//virtual void CompileExpression(TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner) = 0;
};


class BNFIf : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFElse : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFWhile : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFSwitch : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFSwitchBlock : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFFor3 : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFFor : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFTry: public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFCatch : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFFinally : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFBlock : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFTypeState : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFTypeBlock : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFNumber : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

};

class BNFString : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;
};

class BNFDeclBase : public BNFBase
{
public:
	virtual TrecPointer<TcExpression> CreateDeclaration(TDataArray<CompileMessage>& messages, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs, UINT stringStart, TDataArray<VariableContiainer>& vars) = 0;
};

class BNFExpression : public BNFBase
{
public:
	virtual void Compile(UINT tokenList, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, TrecPointer<TcBaseStatement> statement, TDataMap<TrecPointer<BNFBase>>& bnfs,
		UINT stringStart, TDataArray<VariableContiainer>& vars,  TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners,
		const TString& currentRunner, bool expectTerminal = true)override;

	bool GenerateExpression(TDataArray<CompileMessage>& messages, TrecPointer<TVariable> languageDetails, const TString& e, TDataMap<TrecPointer<BNFBase>>& bnfs,
		 TDataArray<VariableContiainer>& vars, TrecPointer<TcExpression>& exp);
};