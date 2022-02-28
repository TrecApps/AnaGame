#include "TcCompiler.h"

TcPreStatement::TcPreStatement()
{
	this->startLine = 0;
}
TcPreStatement::TcPreStatement(const TcPreStatement& statement)
{
	this->startLine = statement.startLine;
	this->statementText.Set(statement.statementText);
	this->subStatements = statement.subStatements;
}


TcProcessor::TcProcessor()
{
}

TcProcessor::TcProcessor(const TcProcessor& proc)
{
	for (UINT Rust = 0; Rust < proc.byteCodes.Size(); Rust++)
		byteCodes.push_back(proc.byteCodes[Rust]);
}

TcProcessor::~TcProcessor()
{
}

void TcProcessor::AddByteCode(const AgByteCode& ref)
{
	byteCodes.push_back(ref);
}

var_type TcProcessor::GetVarType()
{
	return var_type::processor;
}

TString TcProcessor::GetString()
{
	return TString();
}

UINT TcProcessor::Get4Value()
{
	return 0;
}

ULONG64 TcProcessor::Get8Value()
{
	return 0;
}

UINT TcProcessor::GetSize()
{
	return 0;
}

UINT TcProcessor::GetVType()
{
	return 0;
}

TcProcessor::AgByteCode::AgByteCode()
{
	this->opCode = 0;
	this->op1 = this->op2 = 0;
}

TcProcessor::AgByteCode::AgByteCode(const AgByteCode& code)
{
	op1 = code.op1;
	op2 = code.op2;
	opCode = code.opCode;
}

void TcPreStatement::SetStatementText(const TString& text)
{
	statementText.Set(text);
}

TString TcPreStatement::GetStatementText() const
{
	return statementText;
}

void TcPreStatement::SetStartLine(UINT start)
{
	this->startLine = start;
}

UINT TcPreStatement::GetStartLine() const
{
	return startLine;
}

UINT TcPreStatement::GetEndLine() const
{
	return startLine + statementText.CountFinds(L'\n');
}

UINT TcPreStatement::GetStatementCount() const
{
	return subStatements.Size();
}

bool TcPreStatement::GetStatementAt(UINT index, TcPreStatement& statement) const
{
	if(index >= subStatements.Size())
	return false;
	statement = subStatements[index];
	return true;
}

void TcPreStatement::AddStatement(const TcPreStatement& statement)
{
	subStatements.push_back(statement);
}

void TcPreStatement::GenerateStatements(TDataArray<TcPreStatement>& statements, TrecPointer<TVariable> languageAtts, TrecPointer<TFileShell> codeFile, TDataArray<CompileMessage>& messages)
{
}

TcBaseStatement::TcBaseStatement()
{
	this->startLine = 0;
	this->type = tc_statement_type::_unset;
}

TcBaseStatement::TcBaseStatement(tc_statement_type type, const TcPreStatement& statement)
{
	this->type = type;
	this->startLine = statement.GetStartLine();
	this->statementText.Set(statement.GetStatementText());
}

TcBaseStatement::TcBaseStatement(const TcBaseStatement& statement)
{
	this->type = statement.type;
	this->startLine = statement.startLine;
	this->statementText.Set(statement.statementText);
	this->subStatements = statement.subStatements;
}

UINT TcBaseStatement::GetStartLine()
{
	return startLine;
}

UINT TcBaseStatement::GetEndLine()
{
	return startLine + statementText.CountFinds(L'\n');
}

bool TcBaseStatement::ConvertToBase(TDataArray<TcPreStatement>& preStatements, TDataArray<TcBaseStatement>& baseStatements, TrecPointer<TVariable> lang, TDataArray<CompileMessage>& messages)
{
	return false;
}

bool TcBaseStatement::ConvertBaseToByteCode(TDataArray<TcBaseStatement>& baseStatements, TrecPointer<TVariable> lang, TDataArray<CompileMessage>& messages, TcProcessor& processor)
{
	return false;
}

CompileMessage::CompileMessage()
{
	line = 0;
}

CompileMessage::CompileMessage(const CompileMessage& message)
{
	this->message.Set(message.message);
	this->line = message.line;
	this->file = message.file;
}
