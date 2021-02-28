#include "CodeStatement.h"

CodeStatement::CodeStatement()
{
	lineStart = lineEnd = 0;
	statementType = code_statement_type::cst_not_set;
}

CodeStatement::CodeStatement(const CodeStatement& copy)
{
	lineStart = copy.lineStart;
	lineEnd = copy.lineEnd;
	statement.Set(copy.statement);
	block = copy.block;
	next = copy.next;
	parent = copy.parent;
	statementType = copy.statementType;
}

StatementCollector::StatementCollector()
{
	nextLineComment = nextLineString = nextLineStatement = true;
}

StatementCollector::StatementCollector(const StatementCollector& copy)
{
	this->blockEnd = copy.blockEnd;
	this->blockStart = copy.blockStart;
	this->multiLineCommentEnd = copy.multiLineCommentEnd;
	this->multiLineCommentStart = copy.multiLineCommentStart;
	this->multiString = copy.multiString;

	this->nextLineComment = copy.nextLineComment;
	this->nextLineStatement = copy.nextLineStatement;
	this->nextLineString = copy.nextLineString;

	this->oneLineStatement = copy.oneLineStatement;

	this->singleLineComment = copy.singleLineComment;
	this->singleString = copy.singleString;
	this->statements = copy.statements;
	this->statementseperator = copy.statementseperator;

}

void StatementCollector::SetStatementSeperator(TDataArray<TString>& data)
{
	statementseperator = data;
}

void StatementCollector::SetSingleLine(TDataArray<TString>& data)
{
	singleLineComment = data;
}

void StatementCollector::SetMultiLineStart(TDataArray<TString>& data)
{
	multiLineCommentStart = data;
}

void StatementCollector::SetMultiLineEnd(TDataArray<TString>& data)
{
	multiLineCommentEnd = data;
}

void StatementCollector::SetSingleString(TDataArray<TString>& data)
{
	singleString = data;
}

void StatementCollector::SetMultiString(TDataArray<TString>& data)
{
	multiString = data;
}

void StatementCollector::SetBlockStart(TDataArray<TString>& data)
{
	blockStart = data;
}

void StatementCollector::SetBlockEnd(TDataArray<TString>& data)
{
	blockEnd = data;
}

void StatementCollector::SetOneLine(TDataArray<TString>& data)
{
	oneLineStatement = data;
}

void StatementCollector::TurnOffCommentEscape()
{
	nextLineComment = false;
}

void StatementCollector::TurnOffStringEscape()
{
	nextLineString = false;
}

void StatementCollector::TurnOffStatementEscape()
{
	nextLineStatement = false;
}

USHORT StatementCollector::RunCollector(TrecPointer<TFileShell> file, TString& errorMessage, UINT& line)
{
	return USHORT();
}

void StatementCollector::CollectStatement(TDataArray<TFileShell>& statements)
{
	statements = this->statements;
}
