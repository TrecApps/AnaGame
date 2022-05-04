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

	virtual void Compile(TDataArray<CompileMessage>& messages,				// List of Messages to report
		TrecSubPointer<TVariable, AnagameRunner> codeToCompile,				// Current Set of Code to Compile
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode,	// Enables function expressions/statements to establish another set of Code to compile
		TrecPointer<TcBaseStatement> statement,								// The Statement to compile
		UINT stringStart,													// Location in the statement String to Compile
		bool expectTerminal = true											// Whether the statement should end once the BNF Tag has gone through successfully
	) = 0;
};


class BNFIf : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart,bool expectTerminal = true)override;

};

class BNFElse : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFWhile : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFSwitch : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFSwitchBlock : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFFor3 : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFFor : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFTry: public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFCatch : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFFinally : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFBlock : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFTypeState : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFTypeBlock : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};

class BNFNumber : public BNFBase
{
public:
	virtual void Compile(TDataArray<CompileMessage>& messages, TrecSubPointer<TVariable, AnagameRunner> codeToCompile,
		TDataArray<TrecSubPointer<TVariable, AnagameRunner>>& otherCode, TrecPointer<TcBaseStatement> statement,
		UINT stringStart, bool expectTerminal = true)override;

};