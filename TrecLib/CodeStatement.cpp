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

	singleLineComment.push_back(L"//");
	multiLineCommentStart.push_back(L"/*");
	multiLineCommentEnd.push_back(L"*/");

	singleString.push_back(L'\"');
	singleString.push_back(L'\'');
	multiString.push_back(L'`');

	statementseperator.push_back(L';');

	blockStart.push_back(L'{');
	blockEnd.push_back(L'}');
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

typedef enum class next_tok
{
	nt_same,
	nt_open_block,
	nt_close_block,
	nt_statement,
	nt_mode
}next_tok;

next_tok DeduceNextMove(int statement, int beginBlock, int endBlock, UINT mode, bool modeChange)
{
	int modeIndex = modeChange ? mode : -1;

	int tokens[] = {
	statement,
	beginBlock,
	endBlock,
	modeIndex
	};

	int lowest = -1;

	for (int Rust = 0; Rust < 4; Rust++)
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
		return next_tok::nt_statement;
	case 1:
		return next_tok::nt_open_block;
	case 2:
		return next_tok::nt_close_block;
	case 3:
		return next_tok::nt_mode;
	default:
		return next_tok::nt_same;
	}
}



USHORT StatementCollector::RunCollector(TrecPointer<TFileShell> file, TString& errorMessage, UINT& lineNum)
{
	if (!file.Get())
		return 1;
	TFile source(file->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);

	if (!source.IsOpen())
		return 1;


	TrecPointer<CodeStatement> currentStatement = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
	TrecPointer<CodeStatement> parentStatement;
	
	TString line;

	statement_mode currentMode = statement_mode::sm_basic;
	statement_mode nextMode = currentMode;
	TString quote;
	UINT currentLine = 0;

	while (source.ReadString(line))
	{
		UINT currentIndex = 0, startIndex = 0, endIndex = 0;
		
		while (currentIndex < line.GetSize())
		{
			if (currentMode == statement_mode::sm_basic)
			{
				int statement = 0, statement_ = 0;
				statement = GetIndex(line, currentIndex, statementseperator, statement_, quote);
				int startBlock = 0, startBlock_ = 0;
				startBlock = GetIndex(line, currentIndex, blockStart, startBlock_, quote);
				int endBlock = 0, endBlock_ = 0;
				endBlock = GetIndex(line, currentIndex, blockEnd, endBlock_, quote);

				ParseNextMarker(currentMode, line, currentIndex, nextMode, startIndex, endIndex, quote);
				if (!currentStatement->statement.GetTrim().GetSize())
					currentStatement->lineStart = currentLine;

				switch (DeduceNextMove(statement, startBlock, endBlock, startIndex, currentMode != nextMode))
				{
				case next_tok::nt_close_block:
					if (!parentStatement.Get())
					{
						errorMessage.Set(L"Close Block Token detected without corresponding Open Block Token!");
						lineNum = currentLine;
						return 2;
					}
					currentStatement->lineEnd = currentLine;
					currentStatement->statement.Append(line.SubString(currentIndex, endBlock));
					parentStatement->block.push_back(currentStatement);
					currentStatement = parentStatement;
					parentStatement = TrecPointerKey::GetTrecPointerFromSoft<CodeStatement>(parentStatement->parent);
					currentIndex = endBlock_;
					statements.push_back(currentStatement);
					currentStatement->next = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
					currentStatement = currentStatement->next;
					break;
				case next_tok::nt_mode:

					
					switch (nextMode)
					{
					case statement_mode::sm_multi_com:
					case statement_mode::sm_single_com:
						currentStatement->statement.Append(line.SubString(currentIndex, startIndex));
						break;
					case statement_mode::sm_multi_str:
					case statement_mode::sm_single_str:
						currentStatement->statement.Append(line.SubString(currentIndex, endIndex));
						break;
					}
					currentIndex = endIndex;
					break;
				case next_tok::nt_open_block:
					currentStatement->statement.Append(line.SubString(currentIndex, startBlock));
					currentStatement->lineEnd = currentLine;
					parentStatement = currentStatement;
					currentStatement = TrecPointerKey::GetNewTrecPointer<CodeStatement>();

					currentStatement->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parentStatement);
	
					currentIndex = startBlock_;
					break;
				case next_tok::nt_same:
					currentStatement->statement.Append(line.SubString(currentIndex));
					currentIndex = line.GetSize();
					break;
				case next_tok::nt_statement:
					currentStatement->statement.Append(line.SubString(currentIndex, statement_));
					currentStatement->lineEnd = currentLine;
					if (parentStatement.Get())
						parentStatement->block.push_back(currentStatement);
					else statements.push_back(currentStatement);
					currentStatement = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
					currentIndex = statement_;
				}
				
			}

			switch (currentMode)
			{
			case statement_mode::sm_single_str:
			case statement_mode::sm_multi_str:
				ParseNextMarker(currentMode, line, currentIndex, nextMode, startIndex, endIndex, quote);
				if (!currentStatement->statement.GetTrim().GetSize())
					currentStatement->lineStart = currentLine;
				switch (nextMode)
				{
				case statement_mode::sm_basic:
				case statement_mode::sm_single_str:
				case statement_mode::sm_multi_str:
					currentStatement->statement.Append(line.SubString(currentIndex, endIndex));
					break;
				case statement_mode::sm_error:
					errorMessage.Set(L"Detected Incomplete Single Line String!");
					lineNum = currentLine;
					return 2;
				}
				currentIndex = endIndex;
				break;
			case statement_mode::sm_multi_com:
			case statement_mode::sm_single_com:
				ParseNextMarker(currentMode, line, currentIndex, nextMode, startIndex, endIndex, quote);
				currentIndex = endIndex;
			
			}
			currentMode = nextMode;
		}

		currentStatement->statement.AppendChar(L'\n');
		currentLine++;
	}

	if (parentStatement.Get())
	{
		errorMessage.Set(L"Incomplete Block Detected at the end of the File");
		lineNum = currentLine;
		return 2;
	}
	statements.push_back(currentStatement);

	return 0;
}

void StatementCollector::CollectStatement(TDataArray<TrecPointer<CodeStatement>>& statements)
{
	statements = this->statements;
}


int GetIndex(const TString& string, UINT startIndex, TDataArray<TString>& tokens, int& end, TString& token)
{
	int ret = -1;

	for (UINT Rust = 0; Rust < tokens.Size(); Rust++)
	{
		int index = string.Find(tokens[Rust], startIndex, false);
		if (ret == -1)
		{
			ret = index;
			token.Set(tokens[Rust]);
			if (index != -1)
				end = index + tokens[Rust].GetSize();
		}
		else if (index != -1 && index < ret)
		{
			ret = index;
			end = index + tokens[Rust].GetSize();

			token.Set(tokens[Rust]);
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


void StatementCollector::ParseNextMarker(statement_mode curMode, const TString& string, UINT curIndex, statement_mode& netMode, UINT& startIndex, UINT& endIndex, TString& quote)
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

	TString curQuote(quote);

	switch (curMode)
	{
	case statement_mode::sm_basic:
		nextSingleComment = GetIndex(string, curIndex, singleLineComment, nextSingleComment_, quote);
		nextMultiComStart = GetIndex(string, curIndex, multiLineCommentStart, nextMultiComStart_, quote);
		nextSingleString = GetIndex(string, curIndex, singleString, nextSingleString_,quote);
		nextMultiString = GetIndex(string, curIndex, multiString, nextMultiString_,quote);
		DeduceNextMarker(nextSingleComment, nextMultiComStart, nextMultiComEnd, nextSingleString,nextMultiString,
			nextSingleComment_, nextMultiComStart_, nextMultiComEnd_, nextSingleString_, nextMultiString_,
			startIndex, endIndex, netMode);
		if (netMode == statement_mode::sm_error)
			netMode = curMode;
		return;
	case statement_mode::sm_multi_com:
		nextMultiComEnd = GetIndex(string, curIndex, multiLineCommentEnd, nextMultiComEnd_,quote);
		if (nextMultiComEnd != -1)
		{
			netMode = statement_mode::sm_basic;
			startIndex = nextMultiComEnd;
			endIndex = nextMultiComEnd_;
		}
		return;
	case statement_mode::sm_multi_str:
		nextMultiString = string.Find(quote, curIndex, false);
		if (nextMultiString != -1)
		{
			startIndex = nextMultiString;
			endIndex = startIndex + quote.GetSize();
			netMode = statement_mode::sm_basic;
		}
		return;
	case statement_mode::sm_single_str:
		nextSingleString = string.Find(quote, curIndex, false);
		if (nextSingleString != -1)
		{
			startIndex = nextSingleString;
			endIndex = startIndex + quote.GetSize();
			netMode = statement_mode::sm_basic;
		}
		else
		{
			TString trimmed(string.GetTrim());
			UINT slashCount = 0;
			for (UINT Rust = trimmed.GetSize() - 1; Rust < trimmed.GetSize(); Rust--)
			{
				if (trimmed[Rust] == L'\\')
					slashCount;
				else break;
			}

			if (slashCount % 2 == 0 || !this->nextLineString)
			{
				netMode = statement_mode::sm_error;
			}
		}
	}
}

void StatementCollector::CleanStatements()
{
}
