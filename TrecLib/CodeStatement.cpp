#include "CodeStatement.h"
#include "TFile.h"


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
	if (!file.Get())
		return 1;
	TFile source(file->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);



	return USHORT();
}

void StatementCollector::CollectStatement(TDataArray<CodeStatement>& statements)
{
	statements = this->statements;
}


int GetIndex(const TString& string, UINT startIndex, TDataArray<TString>& tokens, int& end)
{
	int ret = -1;

	for (UINT Rust = 0; Rust < tokens.Size(); Rust++)
	{
		int index = string.Find(tokens[Rust], startIndex);
		if (ret == -1)
		{
			ret = index;
			if (index != -1)
				end = index + tokens[Rust].GetSize();
		}
		else if (index != -1 && index < ret)
		{
			ret = index;
			end = index + tokens[Rust].GetSize();
		}
	}
	return ret;
}

void DeduceNextMarker(int nextSingleComment,
	int nextMultiComStart,
	int nextMultiComEnd,
	int nextSingleString,
	int nextMultiString,

	int nextSingleComment_,
	int nextMultiComStart_,
	int nextMultiComEnd_,
	int nextSingleString_,
	int nextMultiString_,
	
	UINT& startIndex,
	UINT& endIndex,
	statement_mode& mode) 
{
	int tokens[] = {
		nextSingleComment,
		nextMultiComStart,
		nextMultiComEnd,
		nextSingleString,
		nextMultiString
	};

	int lowest = -1;

	for (int Rust = 0; Rust < 5; Rust++)
	{
		if (tokens[Rust] != -1)
		{
			if (lowest == -1 || tokens[Rust] <= tokens[lowest])
				lowest = Rust;
		}
	}

	switch (lowest)
	{
	case 0:
		startIndex = nextSingleComment;
		endIndex = nextSingleComment_;
		mode = statement_mode::sm_single_com;
		break;
	case 1:
		startIndex = nextMultiComStart;
		endIndex = nextMultiComStart_;
		mode = statement_mode::sm_multi_com;
		break;
	case 2:
		startIndex = nextMultiComEnd;
		endIndex = nextMultiComEnd_;
		mode = statement_mode::sm_basic;
		break;
	case 3:
		startIndex = nextSingleString;
		endIndex = nextSingleString_;
		mode = statement_mode::sm_single_str;
		break;
	case 4:
		startIndex = nextMultiString;
		endIndex = nextMultiString_;
		mode = statement_mode::sm_multi_str;
		break;
	default:
		mode = statement_mode::sm_error;
	}

}


void StatementCollector::ParseNextMarker(statement_mode curMode, const TString& string, UINT curIndex, statement_mode& netMode, UINT& startIndex, UINT& endIndex)
{
	if (curIndex >= string.GetSize())
		netMode = statement_mode::sm_error;

	int nextSingleComment = -1;
	int nextMultiComStart = -1;
	int nextMultiComEnd = -1;
	int nextSingleString = -1;
	int nextMultiString = -1;

	int nextSingleComment_ = -1;
	int nextMultiComStart_ = -1;
	int nextMultiComEnd_ = -1;
	int nextSingleString_ = -1;
	int nextMultiString_ = -1;

	netMode = curMode;

	switch (curMode)
	{
	case statement_mode::sm_basic:
		nextSingleComment = GetIndex(string, curIndex, singleLineComment, nextSingleComment_);
		nextMultiComStart = GetIndex(string, curIndex, multiLineCommentStart, )
	}
}
