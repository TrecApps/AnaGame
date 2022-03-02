#include "TcCompiler.h"
#include "TContainerVariable.h"
#include <CodeStatement.h>

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


typedef void (StatementCollector::* StatementCollectorMethods)(TDataArray<TString>&);

struct StatementParseFields
{
	TString field;
	TDataArray<TString>* target;
	StatementCollectorMethods method;
};

void PerformCall(StatementCollector* collect, StatementCollectorMethods method, TDataArray<TString>& strings)
{
	(collect->*method)(strings);
}

void ConvertStatements(TDataArray<TcPreStatement>& statements, TDataArray<TrecPointer<CodeStatement>>& oldStatements)
{
	for (UINT Rust = 0; Rust < oldStatements.Size(); Rust++)
	{
		TrecPointer<CodeStatement> curState = oldStatements[Rust];

		while (curState.Get())
		{
			TcPreStatement newState;
			newState.SetStartLine(curState->lineStart);
			newState.SetStatementText(curState->statement);
			
			if (curState->block.Size())
			{
				TDataArray<TcPreStatement> chStatements;
				ConvertStatements(chStatements, curState->block);

				for (UINT C = 0; C < chStatements.Size(); C++)
				{
					newState.AddStatement(chStatements[C]);
				}
			}
			statements.push_back(newState);
			curState = curState->next;
		}
	}
}

void TcPreStatement::GenerateStatements(TDataArray<TcPreStatement>& statements, TrecPointer<TVariable> languageAtts, TrecPointer<TFileShell> codeFile, TDataArray<CompileMessage>& messages)
{
	TrecSubPointer<TVariable, TContainerVariable> containerAtts = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(languageAtts);

	if (!containerAtts.Get())
	{
		CompileMessage m;
		m.file = codeFile;
		m.message.Set(L"Expected Language Attributes to come in the form of a collection!");
		messages.push_back(m);
		return;
	}
	bool pres;
	languageAtts = containerAtts->GetValue(L"StatementParsing", pres);

	if (!languageAtts.Get())
	{
		CompileMessage m;
		m.file = codeFile;
		m.message.Set(L"Expected Language Attributes to provide a 'StatementParsing' Field! This is Needed to help the compiler parse out statements!");
		messages.push_back(m);
		return;
	}

	containerAtts = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(languageAtts);
	if (!containerAtts.Get())
	{
		CompileMessage m;
		m.file = codeFile;
		m.message.Set(L"Expected 'StatementParsing' field to come in the form of a collection!");
		messages.push_back(m);
		return;
	}

	// Here, We'll piggy back off of the existing Code Statement Structure
	// Set up the Statement Collector and the attributres

	TDataArray<TString> statementseperator, // How to recognize when a Statement ends
		singleLineComment,                  // Token for recognizing the start of a single line comment
		multiLineCommentStart,              // Tokens for recognizing start of  multiline comment
		multiLineCommentEnd,                // Tokens for recognizing End of a multiline comment
		singleString,                       // Tokens for recognizing a single line String
		multiString,                        // Tokens for recognizing a multiline String
		blockStart,                         // Tokens for recognizing the start of a block (if this is blank, indentation is assumed)
		blockEnd,                           // Tokens for recognizing the end of a block
		oneLineStatement;                   // Strings that, if they start the statement, signal that the statement should be one line

	bool nextLineComment,       // Use '\' to allow single line comments to go to the next line
		nextLineString,         // Use '\' to allow single line strings to go to the next line
		nextLineStatement,      // Use '\' to allow single line sttements to go to the next line
		newLineEndsStatement;   // Whether statements stop at new Line

	StatementCollector collector;

	StatementParseFields fields[] = {
		{L"StatementSeperator", &statementseperator, StatementCollector::SetStatementSeperator},

		{L"SingleLineComment", &singleLineComment, StatementCollector::SetSingleLine},
		{L"MultiLineCommentStart", &multiLineCommentStart, StatementCollector::SetMultiLineStart},
		{L"MultiLineCommentEnd", &multiLineCommentEnd, StatementCollector::SetMultiLineEnd},
		{L"SingleString", &singleString, StatementCollector::SetSingleString},
		{L"MultiString", &multiString, StatementCollector::SetMultiString},
		{L"BlockStart", &blockStart, StatementCollector::SetBlockStart},
		{L"BlockEnd", &blockEnd, StatementCollector::SetBlockEnd},
		{L"OneLineStatement", &oneLineStatement, StatementCollector::SetOneLine}
	};

	for (UINT Rust = 0; Rust < ARRAYSIZE(fields); Rust++)
	{
		TrecSubPointer<TVariable, TContainerVariable> strings = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(containerAtts->GetValue(fields[Rust].field, pres));

		if (!strings.Get())
			continue;
		for (UINT C = 0; C < strings->GetSize(); C++)
		{
			TrecPointer<TVariable> vars = strings->GetValueAt(C);
			if (vars.Get())
				fields[Rust].target->push_back(vars->GetString());
		}
		
		if (fields[Rust].target->Size())
			PerformCall(&collector, fields[Rust].method, *fields[Rust].target);
	}
	TString errorMessage;
	UINT line;
	if (collector.RunCollector(codeFile, errorMessage, line))
	{
		CompileMessage m;
		m.file = codeFile;
		m.message.Set(errorMessage);
		m.line = line;
		messages.push_back(m);
		return;
	}

	TDataArray<TrecPointer<CodeStatement>> origStatements;
	collector.CollectStatement(origStatements);

	
	ConvertStatements(statements, origStatements);
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
