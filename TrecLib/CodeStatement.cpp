#include "CodeStatement.h"
#include "TFile.h"


/**
 * Method: CodeStatement::CodeStatement
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: new Code statement object
 */
CodeStatement::CodeStatement()
{
	lineStart = lineEnd = 0;
	space = tab = 0;
	statementType = code_statement_type::cst_not_set;
}

/**
 * Method: CodeStatement::CodeStatement
 * Purpose: Copy Constructor
 * Parameters: const CodeStatement& copy - the existing statement to copy from
 * Returns: new copy of Code Statement
 */
CodeStatement::CodeStatement(const CodeStatement& copy)
{
	lineStart = copy.lineStart;
	lineEnd = copy.lineEnd;
	statement.Set(copy.statement);
	block = copy.block;
	next = copy.next;
	parent = copy.parent;
	statementType = copy.statementType;
	space = copy.space;
	tab = copy.tab;
}

/**
 * Method: CodeStatement::IsEmpty
 * Purpose: Reports whether the Statement has any content in it
 * Parameters: void
 * Returns: bool - true if the statement is empty (no statement and empty block) and can be removed from the list
 */
bool CodeStatement::IsEmpty()
{
	return !statement.GetTrim().GetSize() && !block.Size();
}

/**
 * Method: CodeStatement::IsEqual
 * Purpose: Reports whether another statement is more or less the exact same statement as this one
 * Parameters: const CodeStatement& other - the other statement to compare it to
 * Returns: bool - whether the statements are 'equal'
 */
bool CodeStatement::IsEqual(const CodeStatement& other)
{
	return this->lineEnd == other.lineEnd &&
		this->lineStart == other.lineStart &&
		this->statementType == other.statementType &&
		!this->statement.Compare(other.statement);
}

/**
 * Method: StatementCollector::StatementCollector
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Statement Collector
 *
 * Note: By Default, the Collector will configure itself for JavaScript. To set it for other languages
 *  Use the various set methods before calling 'ProcessFile' method
 */
StatementCollector::StatementCollector()
{
	nextLineComment = nextLineString = nextLineStatement = true;
	newLineEndsStatement = false;

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

/**
 * Method: StatementCollector::StatementCollector
 * Purpose: Copy Constructor
 * Parameters: const StatementCollector& copy - the collector to copy from
 * Returns: New Copy of existing Collector
 */
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

/**
 * Method: StatementCollector::SetStatementSeperator
 * Purpose: Sets the tokens used for seperating statements
 * Parameters:  TDataArray<TString>& data - the list of Statement Seperators
 * Returns: void
 *
 * Note: If the Provided list is empty, then the end of the Line will be used as the statement seperator.
 *      Otherwise, the Collector will not use the newline as a statement terminator unless the statement starts
 *      with a token specified by the One-Line Array (used with the C-Prepeorcessor in mind)
 */
void StatementCollector::SetStatementSeperator(TDataArray<TString>& data)
{
	statementseperator = data;
}

/**
 * Method: StatementCollector::SetSingleLine
 * Purpose: Sets the tokens used to signify the start of a single line method
 * Parameters: TDataArray<TString>& data - list of single-line tokens
 * Returns: void
 */
void StatementCollector::SetSingleLine(TDataArray<TString>& data)
{
	singleLineComment = data;
}

/**
 * Method: StatementCollector::SetMultiLineStart
 * Purpose: Sets the tokens that signify the start of a multi-line comment
 * Parameters: TDataArray<TString>& data - list of multi-line comment starters
 * Returns: void
 */
void StatementCollector::SetMultiLineStart(TDataArray<TString>& data)
{
	multiLineCommentStart = data;
}

/**
 * Method: StatementCollector::SetMultiLineEnd
 * Purpose: Sets the tokens that signify the end of a multi-line comment
 * Parameters: TDataArray<TString>& data - list of multi-line comment enders
 * Returns: void
 */
void StatementCollector::SetMultiLineEnd(TDataArray<TString>& data)
{
	multiLineCommentEnd = data;
}

/**
 * Method: StatementCollector::SetSingleString
 * Purpose: Sets tokens for the start of single line comments
 * Parameters: TDataArray<TString>& data - tokens that signify single quotes
 * Returns: void
 *
 * Note: Even languages like c/C++, which use single quotes for single characters instead of strings, should
 *  specify the single quote here as the Statement Collector doesn't care if the 'string' is single char or multi char.
 *  If that character is ';', Then the coder didn't intend for the statement to end ther, but the collector would assume
 *  it does if the single quote is removed here
 */
void StatementCollector::SetSingleString(TDataArray<TString>& data)
{
	singleString = data;
}

/**
 * Method: StatementCollector::SetMultiString
 * Purpose: Sets the tokens used for recognizing a multi-line String
 * Parameters: TDataArray<TString>& data - list of tokens used for recognizing a multi-line string
 * Returns: void
 */
void StatementCollector::SetMultiString(TDataArray<TString>& data)
{
	multiString = data;
}

/**
 * Method: StatementCollector::SetBlockStart
 * Purpose: Sets the tokens used for recognizing the start of a block
 * Parameters: TDataArray<TString>& data - Tokens used for the start of a block
 * Returns: void
 *
 * Note: If this array is rendered emptied, then the collector will ignore the block end array and use
 *  indentation for determining blocks
 */
void StatementCollector::SetBlockStart(TDataArray<TString>& data)
{
	blockStart = data;
}

/**
 * Method: StatementCollector::SetBlockEnd
 * Purpose: Sets the tokens used for recognizing the end of a block
 * Parameters: TDataArray<TString>& data -  tokens used for finding the end of a block
 * Returns: void
 */
void StatementCollector::SetBlockEnd(TDataArray<TString>& data)
{
	blockEnd = data;
}

/**
 * Method: StatementCollector::SetOneLine
 * Purpose: Sets the tokens used for identifying statements that should be terminated at the end of the line
 * Parameters: TDataArray<TString>& data - tokens that, if found at the start of the statement indicate that the statement should end at the line
 * Returns: void
 *
 * Note: Multi-Lines
 */
void StatementCollector::SetOneLine(TDataArray<TString>& data)
{
	oneLineStatement = data;
}

/**
 * Method: StatementCollector::TurnOffCommentEscape
 * Purpose: Turns off the use of the '\' character on single line comments
 * Parameters: void
 * Returns: void
 *
 * Note: By default the escape functionality on single comments is on. This method will turn it off, but it cannot be turned back on.
 *      This has no effect on multi-line comments
 */
void StatementCollector::TurnOffCommentEscape()
{
	nextLineComment = false;
}

/**
 * Method: StatementCollector::TurnOffStringEscape
 * Purpose: Turns off the use of the '\' character on single line strings
 * Parameters: void
 * Returns: void
 *
 * Note: By default the escape functionality on single line strings is on. This method will turn it off, but it cannot be turned back on
 *      This has no effect on multi-line strings
 */
void StatementCollector::TurnOffStringEscape()
{
	nextLineString = false;
}

/**
 * Method: StatementCollector::TurnOffStatementEscape
 * Purpose: Turns off the use of the '\' character on single line statements
 * Parameters: void
 * Returns: void
 *
 * Note: By default the escape functionality on single-line statement is on. This method will turn it off, but it cannot be turned back on
 *      For
 */
void StatementCollector::TurnOffStatementEscape()
{
	nextLineStatement = false;
}

/**
 * Method: StatementCollector::TurnOnOneLinePerStatement
 * Purpose: Turns on the mode where a statement ends when it's current line does
 * Parameters: void
 * Returns: void
 *
 * Note: if the statement Seperator is rendered Empty, then that has the same effect as calling this method. Also, calling this method
 *  cannot be undone
 */
void StatementCollector::TurnOnOneLinePerStatement()
{
	newLineEndsStatement = true;
}

typedef enum class next_tok
{
	nt_same,
	nt_open_block,
	nt_close_block,
	nt_statement,
	nt_mode
}next_tok;

/**
 * Function: DeduceNextMove
 * Purpose: Helper function to inform block Parser what the next move should be
 * Parameters: int statement - next token that indicates the end of the current statement
 *				int beginBlock - token that indicates the start of a block
 *				int endBlock - token that indicates the end of a current block
 *				UINT mode - 
 *				bool modeChange
 */
next_tok DeduceNextMove(int statement, int beginBlock, int endBlock, UINT mode, bool modeChange)
{
	int modeIndex = modeChange ? mode : -1;

	// Collect tokens into an array
	int tokens[] = {
	statement,
	beginBlock,
	endBlock,
	modeIndex
	};

	int lowest = -1;

	// Find the lowest value that is not -1
	for (int Rust = 0; Rust < 4; Rust++)
	{
		if (tokens[Rust] != -1)
		{
			if (lowest == -1 || tokens[Rust] <= tokens[lowest])
				lowest = Rust;
		}
	}

	// Report the next mode based off of what the token was
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

/**
 * Function: GetIndex
 * Purpose: Gets the index of a token in a string and reports the token found
 * Parameters: const TString& string - the string to search
 *				UINT startIndex - the index to begin the search
 *				TDataArray<TString>& tokens - set of tokens to search for
 *				int& end - end of the found token in the string
 *				TString& token - the actual token found
 * Returns: int - the index of the found token
 */
int GetIndex(const TString& string, UINT startIndex, TDataArray<TString>& tokens, int& end, TString& token)
{
	int ret = -1;

	for (UINT Rust = 0; Rust < tokens.Size(); Rust++)
	{
		int index = string.Find(tokens[Rust], startIndex, false);
		if (ret == -1)
		{
			ret = index;
			// 
			if (index != -1)
			{	
				token.Set(tokens[Rust]);
				end = index + tokens[Rust].GetSize();
			}
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

		if (!blockStart.Size() && currentMode == statement_mode::sm_basic)
		{
			if (line.GetTrim().GetSize() == 0)
				continue;

			UINT newTabCount = 0, newSpaceCount = 0;
			for (UINT Rust = 0; Rust < line.GetSize(); Rust++)
			{
				WCHAR ch = line[Rust];
				if (ch == L' ')
					newSpaceCount++;
				else if (ch == L'\t')
					newTabCount++;
				else
					break;
			}
			if (currentStatement->statement.GetTrim().GetSize() == 0)
			{
				currentStatement->space = newSpaceCount;
				currentStatement->tab = newTabCount;
			}
		}
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
					if((!currentStatement->parent.Get()) || (currentStatement->parent.Get()->next.Get() != currentStatement.Get()))
						parentStatement->block.push_back(currentStatement);
					currentStatement->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parentStatement);
					currentStatement = parentStatement;
					while (parentStatement->parent.Get() && parentStatement->parent.Get()->next.Get() == parentStatement.Get())
					{
						parentStatement = TrecPointerKey::GetTrecPointerFromSoft<>(parentStatement->parent);
					}
					
					parentStatement = TrecPointerKey::GetTrecPointerFromSoft<CodeStatement>(parentStatement->parent);
					currentIndex = endBlock_;
					// statements.push_back(currentStatement);
					currentStatement->next = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
					currentStatement->next->parent = TrecPointerKey::GetSoftPointerFromTrec<>(currentStatement);
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
					if (parentStatement.Get())
					{
						if (currentStatement->parent.Get()->next.Get() != currentStatement.Get())
						{
							parentStatement->block.push_back(currentStatement);
							currentStatement->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parentStatement);
						}
						else
						{
							currentStatement->parent.Get()->next = currentStatement;

						}
					}
					else
					{
						statements.push_back(currentStatement);
					}
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
					{
						parentStatement->block.push_back(currentStatement);
						currentStatement->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parentStatement);
					}
					else statements.push_back(currentStatement);
					currentStatement = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
					if(parentStatement.Get())
						currentStatement->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parentStatement);
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

		if (currentMode == statement_mode::sm_basic)
		{
			if (newLineEndsStatement || statementseperator.Size() == 0 || StartAsSingleLine(currentStatement->statement))
			{
				currentStatement->lineEnd = currentLine;
				if (parentStatement.Get())
				{
					parentStatement->block.push_back(currentStatement);
					currentStatement->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parentStatement);
				}
				else statements.push_back(currentStatement);
				currentStatement = TrecPointerKey::GetNewTrecPointer<CodeStatement>();
			}
			else
				currentStatement->statement.AppendChar(L'\n');

		}
		else
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

	UINT blockCheck = HandlePythonBlocks();
	if (blockCheck)
	{
		errorMessage.Set(L"Error managing Python Block types!");
		lineNum = statements[blockCheck]->lineStart;
		return 2;
	}

	return 0;
}

/**
 * Method: StatementCollector::CleanStatements
 * Purpose: Removes completely empty statements from the list of statements and removes duplicates
 * Parameters: TDataArray<TrecPointer<CodeStatement>>& statements - current list of statements to clean
 * Returns: void
 */
void StatementCollector::CollectStatement(TDataArray<TrecPointer<CodeStatement>>& statements)
{
	CleanStatements(this->statements);
	statements = this->statements;
}


/**
 * Function: DeduceNextMarker
 * Purpose: Reports the next marker and for how long it would be
 * Parameters: int nextSingleComment - beginning of the Single Comment
 *				int nextMultiComStart - beginning of the Multi-line comment
 *				int nextMultiComEnd - beginning of the 
 *				int nextSingleString - beginning of the Single String
 *				int nextMultiString - beginning of the multi-string
 *				int nextSingleComment_ - the end of the Single-Comment
 *				int nextMultiComStart_ the end of the Multi-Comment section
 *				int nextMultiComEnd_ - the End of the Multi-Comment section
 *				int nextSingleString_ - the end of the Single-String section
 *				int nextMultiString_ - the end of the Multi-string section
 *				UINT& startIndex - the beginning of the next mode (index)
 *				UINT& endIndex - the end of the next section (comment or string)
 *				statement_mode& mode - reports the next mode (comment, string, or regular
 * Returns: void
 */
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


/**
 * Method: StatementCollector::ParseNextMarker
 * Purpose: Helper Method for determining the next token in the Provided String
 * Parameters: statement_mode curMode- the current mode of the parser
 *              const TString& string - the String to parse
 *              UINT curIndex - the index in the string to start the search
 *              statement_mode& nextMode - the next mode
 *              UINT& startIndex - where the token was found
 *              UINT& endIndex - the next index to enter as curIndex
 * Returns: void
 */
void StatementCollector::ParseNextMarker(statement_mode curMode, const TString& string, UINT curIndex, statement_mode& netMode, UINT& startIndex, UINT& endIndex, TString& quote)
{
	// Don't go beyond the string
	if (curIndex >= string.GetSize())
		netMode = statement_mode::sm_error;

	// Assume none of these markers have been found yet
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

	// If no token has been found, assume te next mode is the same as the current on
	netMode = curMode;

	TString curQuote(quote);

	TString sinQ, mulQ;

	switch (curMode)
	{
	case statement_mode::sm_basic:
		// We are interested in what might get us into a string or comment
		nextSingleComment = GetIndex(string, curIndex, singleLineComment, nextSingleComment_, quote);
		nextMultiComStart = GetIndex(string, curIndex, multiLineCommentStart, nextMultiComStart_, quote);
		nextSingleString = GetIndex(string, curIndex, singleString, nextSingleString_,sinQ);
		nextMultiString = GetIndex(string, curIndex, multiString, nextMultiString_,mulQ);
		DeduceNextMarker(nextSingleComment, nextMultiComStart, nextMultiComEnd, nextSingleString,nextMultiString,
			nextSingleComment_, nextMultiComStart_, nextMultiComEnd_, nextSingleString_, nextMultiString_,
			startIndex, endIndex, netMode);
		// if error is detected, mode doesn't change
		if (netMode == statement_mode::sm_error)
			netMode = curMode;
		// If a quote is next, set the quote to end by it
		else if (netMode == statement_mode::sm_multi_str)
			quote.Set(mulQ);
		else if (netMode == statement_mode::sm_single_str)
			quote.Set(sinQ);
		return;
	case statement_mode::sm_multi_com:
		// Detect the end of the multi-comment
		nextMultiComEnd = GetIndex(string, curIndex, multiLineCommentEnd, nextMultiComEnd_,quote);
		if (nextMultiComEnd != -1)
		{
			// End was detected, move index to after the end 
			netMode = statement_mode::sm_basic;
			startIndex = nextMultiComEnd;
			endIndex = nextMultiComEnd_;
		}
		else
			// Move index to after rthe string so the next line is read
			endIndex = string.GetSize();
		return;
	case statement_mode::sm_multi_str:
		// we are in the multi-line string
		nextMultiString = string.Find(quote, curIndex, false);
		if (nextMultiString != -1)
		{
			startIndex = nextMultiString;
			endIndex = startIndex + quote.GetSize();
			netMode = statement_mode::sm_basic;
		}
		else endIndex = string.GetSize();
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

			if ((slashCount % 2) == 0 || !this->nextLineString)
			{
				netMode = statement_mode::sm_error;
			}
		}
		return;
	case statement_mode::sm_single_com:
		if (this->nextLineComment)
		{
			TString trimmed(string.GetTrim());
			UINT slashCount = 0;
			for (UINT Rust = trimmed.GetSize() - 1; Rust < trimmed.GetSize(); Rust--)
			{
				if (trimmed[Rust] == L'\\')
					slashCount;
				else break;
			}
			if ((slashCount % 2) == 0)
				netMode = statement_mode::sm_basic;
		}
		else
			netMode = statement_mode::sm_basic;
		endIndex = string.GetSize();
	}
}

/**
 * Method: StatementCollector::CleanStatements
 * Purpose: Removes completely empty statements from the list of statements and removes duplicates
 * Parameters: TDataArray<TrecPointer<CodeStatement>>& statements - current list of statements to clean
 * Returns: void
 */
void StatementCollector::CleanStatements(TDataArray<TrecPointer<CodeStatement>>& statements)
{
	if (!statements.Size())
		return;
	auto state = statements[0];
	auto oState = state;

	// First Handle child blocks before the parent ones
	CleanStatements(state->block);

	for (UINT Rust = 1; Rust < statements.Size(); Rust++)
	{
		auto nState = statements[Rust];
		// Check for duplicates
		if (state->next.Get() == nState.Get() || state.Get() == nState.Get())
		{
			statements.RemoveAt(Rust--);
			continue;
		}
		// If statement is empty, remove it
		if (nState->IsEmpty())
		{
			statements.RemoveAt(Rust--);
			continue;
		}
		state = nState;
	}

	if (oState->IsEmpty())
		statements.RemoveAt(0);
}

/**
 * Method: StatementCollector::StartsAsSingleLine
 * Purpose: Reports whether the current statement qualifies for automatic singleline
 * Parameters: const TString& statement - the statement in it's current form
 * Returns: bool - whether to end the statement based off of the configuration and the contents of the statement
 */
bool StatementCollector::StartAsSingleLine(const TString& statement)
{
	TString s(statement.GetTrim());

	// Get the number of backslashes at the end
	UINT slashCount = 0;
	for (UINT Rust = s.GetSize() - 1; Rust < s.GetSize(); Rust--)
	{
		if (s[Rust] == L'\\')
			slashCount++;
		else
			break;
	}

	// If we care about backslashes at the end and there is an odd number of them, the statement continues
	if (nextLineStatement && (slashCount % 2))
		return false;

	// If there is nothing indicting the start of a block, then make sure that it is time to set a new statement
	if (!blockStart.Size())
		return slashCount % 2 == 0;

	// If the statement might end, check for a marker that indicates it should
	for (UINT Rust = 0; Rust < oneLineStatement.Size(); Rust++)
	{
		if (s.StartsWith(oneLineStatement[Rust]))
			return true;
	}
	
	return false;
}

/**
 * Method: StatementCollector::HandlePythonBlocks
 * Purpose: Allows Collector to organize block by spaces and indents, since it would not have been done during initial statement generation
 * Parameters: void
 * Returns: UINT - 0 if successful, otherwise the statement that confused the Collector
 */
UINT StatementCollector::HandlePythonBlocks()
{
	// If we have a means to detect blocks without indentation, report no error, since Blocks will already be sorted
	if (blockStart.Size())
		return 0;
	// No Error if no statements were generated
	if (!statements.Size())
		return 0;
	// Start with the first statement, which canno be in a block
	TrecPointer<CodeStatement> parent = statements[0];
	// Go through each statement
	for (UINT Rust = 1; Rust < statements.Size(); Rust++)
	{
		// Get the next statement
		auto current = statements[Rust];
		//Check to see if the spaces and tabs are the same. If they are, Set the parent as the current and loop again
		if (parent->space == current->space && parent->tab == current->tab)
		{
			// Check to see if we are already in a block. If we are, then make sure the block holder gets the current block
			if (parent->parent.Get())
			{
				current->parent = parent->parent;
				current->parent.Get()->block.push_back(current);
				statements.RemoveAt(Rust--);
			}

			parent = current;
			continue;
		}

		// If both the tabs and spaces change in some way, then this is an error and we return the current statement line
		if (parent->space != current->space && parent->tab != current->tab)
		{
			return Rust;
		}

		// Handle case where the new statement is about to become a child of the old one (entering a block
		if (current->space > parent->space || current->tab > parent->tab)
		{
			current->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parent);
			parent->block.push_back(current);
			statements.RemoveAt(Rust--);
			parent = current;

			continue;
		}

		// We are about to move out of a block
		if (current->space < parent->space || current->tab < parent->tab)
		{
			// Don't know how many blocks we are moving out of
			while (parent.Get())
			{
				// Once this is true, we are at the same level as the "parent" so no longer breaking out
				if (parent->space == current->space && parent->tab == current->tab)
				{
					break;
				}
				parent = TrecPointerKey::GetTrecPointerFromSoft<>(parent->parent);
			}
			// If we loose the parent, then the indentation is off and we inform the caller accordingly
			if (!parent.Get())
				return Rust;

			// If we still have a parent, handle situation as you normally would
			if (parent->parent.Get())
			{
				current->parent = parent->parent;
				current->parent.Get()->block.push_back(current);
				statements.RemoveAt(Rust--);

			}
			parent = current;
		}
	}
	return 0;
}
