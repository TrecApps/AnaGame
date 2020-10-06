#include "TJavaScriptInterpretor.h"
#include <DirectoryInterface.h>


static TDataArray<WCHAR> noSemiColonEnd;



/**
 * Method: TJavaScriptInterpretor::TJavaScriptInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TInterpretor Object
 */
TJavaScriptInterpretor::TJavaScriptInterpretor(TrecSubPointer<TVariable,TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env): TInterpretor(parentInterpretor, env)
{
    if (noSemiColonEnd.Size() == 0)
    {
        noSemiColonEnd.push_back(L'+');
        noSemiColonEnd.push_back(L'-');
        noSemiColonEnd.push_back(L'=');
        noSemiColonEnd.push_back(L'%');
        noSemiColonEnd.push_back(L'/');
        noSemiColonEnd.push_back(L'\\');
    }
}

UINT TJavaScriptInterpretor::SetCode(TFile& file)
{
    if (!file.IsOpen())
        return 1;

    file.SeekToBegin();

    TString shadowPath(GetShadowFilePath(file));
   
    TrecPointer<TFile> newFile = TrecPointerKey::GetNewTrecPointer<TFile>(shadowPath, TFile::t_file_create_always | TFile::t_file_write);

    if (!newFile.Get() || !newFile->IsOpen())
        return 2;

    newFile->SeekToBegin();

    UINT mode = 0;
    TString line;

    TDataArray<WCHAR> quoteStack;

    int quoteIndex = -1;
    int singleIndex = -1;
    int multiIndex = -1;

    int startIndex = 0;

    while (file.ReadString(line))
    {
        switch (mode)
        {
            normalMode:
        case 0: // normal Mode

            quoteIndex = line.FindOneOf(L"\"'`", startIndex);
            singleIndex = line.Find(L"//", startIndex);
            multiIndex = line.Find(L"/*", startIndex);

            if (quoteIndex == -1 && singleIndex == -1 && multiIndex == -1)
            {
                newFile->WriteString(line.SubString(startIndex) + L'\n');
                startIndex = 0;
            }
            else if (IndexComesFirst(quoteIndex, singleIndex) && IndexComesFirst(quoteIndex, multiIndex))
            {
                newFile->WriteString(line.SubString(startIndex, quoteIndex + 1));
                startIndex = quoteIndex + 1;
                mode = 2;
                quoteStack.push_back(line[quoteIndex]);
                goto quoteMode;
            }
            else if (IndexComesFirst(singleIndex, quoteIndex) && IndexComesFirst(singleIndex, multiIndex))
            {
                newFile->WriteString(line.SubString(startIndex, singleIndex) + L'\n');
                startIndex = 0;
            }
            else if (IndexComesFirst(multiIndex, singleIndex) && IndexComesFirst(multiIndex, quoteIndex))
            {
                newFile->WriteString(line.SubString(startIndex, multiIndex));
                mode = 1;
                startIndex += 2;
                goto commentMode;
            }

            break;
        commentMode:
        case 1: // comment mode (here, we only look for ending line and single comments will be removed upon the next read)
            multiIndex = line.Find(L"*/", startIndex);
            if (multiIndex == -1)
            {
                newFile->Write(L"\n", sizeof(WCHAR));
            }
            else
            {
                startIndex = multiIndex + 2;
                mode = 0;
                goto normalMode;
            }

            break;
        quoteMode:
        case 2: // quote mode
            quoteIndex = line.FindOneOf(L"\"'`", startIndex);
            if (quoteIndex == -1)
            {
                newFile->WriteString(line.SubString(startIndex) + L'\n');
                startIndex = 0;
            }
            else if (line[quoteIndex] == quoteStack[quoteStack.Size()])
            {
                newFile->WriteString(line.SubString(startIndex, quoteIndex + 1));
                startIndex = quoteIndex + 1;
                mode = 0;
                quoteStack.RemoveAt(quoteStack.Size() - 1);
                goto normalMode;
            }
            else
            {
                // Here, we found a quote mark, but not the one we need to break free
                newFile->WriteString(line.SubString(startIndex, quoteIndex + 1));
                startIndex = quoteIndex + 1;
                goto quoteMode;

            }

        }
    }

    if (!mode)
    {
        TString newPath(newFile->GetFilePath());
        newFile->Close();
        newFile->Open(newPath, TFile::t_file_open_always || TFile::t_file_read);
        if (!newFile->IsOpen())
            return 5;
        this->file = newFile;
        newFile->SeekToBegin();
        this->end = newFile->GetLength();
        this->start = 0LL;
        return InsertSemiColons();
    }
    newFile->Close();
    newFile.Nullify();

    if (mode == 1)
        return 3; // mode was in comment mode
    else
        return 4; // mode was in quote mode
}

ReportObject TJavaScriptInterpretor::Run()
{
    ReportObject ret;
    if (!file.Get() || !file->IsOpen())
    {
        ret.returnCode = ReportObject::broken_reference;
        ret.errorMessage.Set(L"Error! No access to the JavaScript file!");
        return ret;
    }

    file->Seek(start, FILE_BEGIN);

    ULONGLONG currentPoint = start;

    TString code;

    UINT startQuoteSearch = 0;

    TFile tokenLog(file->GetFilePath() + L".output", TFile::t_file_create_new | TFile::t_file_write);

    UINT line = 1;

    TDataArray<JavaScriptStatement> statements;

    // Read the String with the following flags:
    // 0b00000001 - ensures that our terminating character is included in the resulting string
    // 0b00000010 - Tells the method to not terminate if the termination character is within a quote
    // 0b00000100 - Tells the method to watch out for an odd number of backslashes 
    while (file->ReadString(code, L"{;", 0b00000111))
    {
        // First make sure this statement doesn't land us in a multi-line string (`) which the ReadString
        // Method doesn't account for
        bool inMultiString = hasOddMiltiLineStrMarkers(code);

        UINT beginLine = line;

        while (inMultiString)
        {
            TString appendable;
            if (!file->ReadString(appendable, L"`", 0b00000001) || !appendable.EndsWith(L"`"))
            {
                // if this happens, then the file ends in the middle of a multiline string, which is an error
                ret.returnCode = ret.incomplete_block;

                ret.errorMessage.Set(L"Code File ends in the middle of a multi-line statement!");

                TString stack;
                stack.Format(L"\tAt %ws, line %d", file->GetFileName().GetConstantBuffer(), line + appendable.CountFinds(L'\n'));

                ret.stackTrace.push_back(stack);
                return ret;
            }


            code.Append(appendable);
            file->ReadString(appendable, L"{;", 0b00000111);

            code.Append(appendable);

            inMultiString = hasOddMiltiLineStrMarkers(code);
        }

        line += code.CountFinds(L'\n');


        TString startStatement(code.GetTrimLeft());

        int startParenth = startStatement.Find(L'(');

        if (startStatement.StartsWith(L"if", false, true))
        {
            if (startParenth == -1 || startStatement.SubString(2,startParenth).GetTrim().GetSize())
            {
                ret.returnCode = ret.broken_reference;
                ret.errorMessage.Set(L"Unexpected token after 'if' statement!");



                return ret;
            }


            ProcessParenthBlock(ret, startStatement.SubString(2), line);

            if (ret.returnCode)
                return ret;


            JavaScriptStatement statement(js_statement_type::js_if);

            statement.contents.Set(ret.errorMessage);
            statement.lineStart = beginLine;
            statement.lineEnd = line;


            statement.fileStart = file->GetPosition() + 1;

            statement.fileEnd = GetBlockEnd();
            
            if (!statement.fileEnd)
            {
                ret.returnCode = ret.incomplete_block;
                ret.errorMessage.Set(L"If-block does not have a complete block");



                return ret;
            }
            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"for", false, true))
        {
            if (startParenth == -1 || startStatement.SubString(2, startParenth).GetTrim().GetSize())
            {
                ret.returnCode = ret.broken_reference;
                ret.errorMessage.Set(L"Unexpected token after 'for' statement!");



                return ret;
            }


            ProcessParenthBlock(ret, startStatement.SubString(2), line);

            if (ret.returnCode)
                return ret;


            JavaScriptStatement statement(js_statement_type::js_for);

            statement.contents.Set(ret.errorMessage);
            statement.lineStart = beginLine;
            statement.lineEnd = line;


            statement.fileStart = file->GetPosition() + 1;

            statement.fileEnd = GetBlockEnd();

            if (!statement.fileEnd)
            {
                ret.returnCode = ret.incomplete_block;
                ret.errorMessage.Set(L"For-block does not have a complete block");



                return ret;
            }
            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"while", false, true))
        {
            if (startParenth == -1 || startStatement.SubString(2, startParenth).GetTrim().GetSize())
            {
                ret.returnCode = ret.broken_reference;
                ret.errorMessage.Set(L"Unexpected token after 'while' statement!");



                return ret;
            }


            ProcessParenthBlock(ret, startStatement.SubString(2), line);

            if (ret.returnCode)
                return ret;


            JavaScriptStatement statement(js_statement_type::js_while);

            statement.contents.Set(ret.errorMessage);
            statement.lineStart = beginLine;
            statement.lineEnd = line;


            statement.fileStart = file->GetPosition() + 1;

            statement.fileEnd = GetBlockEnd();

            if (!statement.fileEnd)
            {
                ret.returnCode = ret.incomplete_block;
                ret.errorMessage.Set(L"While-block does not have a complete block");



                return ret;
            }
            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"const", false, true))
        {
            JavaScriptStatement statement(js_statement_type::js_const);

            statement.lineStart = beginLine;
            statement.lineEnd = line;

            statement.contents.Set(startStatement.SubString(5).GetTrim());

            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"function", false, true))
        {
            JavaScriptStatement statement(js_statement_type::js_function);
        }
        else if (startStatement.StartsWith(L"let", false, true))
        {
            JavaScriptStatement statement(js_statement_type::js_let);

            statement.lineStart = beginLine;
            statement.lineEnd = line;

            statement.contents.Set(startStatement.SubString(5).GetTrim());

            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"var", false, true))
        {
            JavaScriptStatement statement(js_statement_type::js_var);
            statement.lineStart = beginLine;
            statement.lineEnd = line;

            statement.contents.Set(startStatement.SubString(5).GetTrim());

            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"class", false, true))
        {
            JavaScriptStatement statement(js_statement_type::js_class);
        }
        else
        {
            JavaScriptStatement statement(js_statement_type::js_regular);
            statement.lineStart = beginLine;
            statement.lineEnd = line;

            statement.contents.Set(startStatement.SubString(5).GetTrim());

            statements.push_back(statement);
        }

    }







    for (currentPoint = start; currentPoint < end; currentPoint += file->ReadString(code, TString(L";\n{"), TFile::t_file_include_terminator))
    {
        int firstQuote;

        

        while ((firstQuote = code.FindOneOf(L"\"'`", startQuoteSearch)) != -1)
        {
            // We have a String in our statement and need to make sure we have the whole statement
            WCHAR quoteType = code[firstQuote];

            int secondQuote = code.FindOneOf(L" \t", firstQuote + 1);

            if (secondQuote == -1)
            {
                TString code2;
                currentPoint += file->ReadString(code2, TString(quoteType), TFile::t_file_include_terminator);
                TString code3;
                currentPoint += file->ReadString(code3, TString(L";\n{"), TFile::t_file_include_terminator);

                code.Append(code2);
                startQuoteSearch = code.GetSize() + 1;
                code.Append(code3);
            }
        }

        TDataArray<TString> tokens;
        TString tok;
        WCHAR quoteMode = L'\0';
        for (UINT Rust = 0; Rust < code.GetSize(); Rust++)
        {
            WCHAR curChar = code[Rust];

            if (quoteMode)
            {
                tok.AppendChar(curChar);
                if (curChar == quoteMode)
                    quoteMode = L'\0';

                continue;
            }

            if (curChar == L' ' || curChar == L'\t')
            {
                if (tok.GetSize())
                {
                    tokens.push_back(tok);
                    tok.Empty();
                }
                continue;
            }

            if (curChar == L'\'' || curChar == L'\"' || curChar == L'`')
                quoteMode = curChar;

            tok.AppendChar(curChar);
        }


        // Now Parse the Statement we got
        if (tokenLog.IsOpen())
        {
            for (UINT Rust = 0; Rust < tokens.Size(); Rust++)
            {
                tokenLog.WriteString(tokens[Rust] + L'\n');
            }

            tokenLog.WriteString(L"\n\n");
        }
    }

    tokenLog.Close();

    return ReportObject();
}

ReportObject TJavaScriptInterpretor::Run(TDataArray<TrecPointer<TVariable>>& params)
{
    ReportObject ret;
    if (!file.Get() || !file->IsOpen())
    {
        ret.returnCode = ReportObject::broken_reference;
        ret.errorMessage.Set(L"Error! No access to the JavaScript file!");
        return ret;
    }

    for (UINT Rust = 0; Rust < params.Size() && Rust < paramNames.Size(); Rust++)
    {
        variables.addEntry(paramNames[Rust], params[Rust]);
    }

    return Run();
}

bool TJavaScriptInterpretor::hasOddMiltiLineStrMarkers(const TString& str)
{
    if (!str.GetSize())
        return false;
    UINT count = str[0] == L'`'? 1 : 0;

    int find = 0;

    while ((find = str.FindOutOfQuotes(L"`", find + 1)) != -1)
    {
        count++;
    }


    return count % 2 == 1;
}

UINT TJavaScriptInterpretor::InsertSemiColons()
{
    TFile newFile(file->GetFilePath() + L"2", TFile::t_file_create_always | TFile::t_file_write);

    if (!newFile.IsOpen())
    {
        return 5;
    }

    TString dLines[2];

    TString readLine;

    if (file->ReadString(readLine))
        dLines[0].Set(readLine);
    else
        return 6;

    UINT empty = 0;

    bool inMultiString = hasOddMiltiLineStrMarkers(readLine);

    while (file->ReadString(readLine))
    {

        if (readLine.GetTrim().GetSize() == 0)
        {
            empty++;
            continue;
        }

        dLines[1].Set(readLine);

        // Only insert semicolon if we are outside a multi-line String
        if (!inMultiString)
        {

            bool insertSemiColon = true;

            // Don't insert semicolon here as it could signify a function in line one and parameters list in line 2
            if (dLines[1].GetTrim().StartsWith(L"("))
                insertSemiColon = false;
            else
            {
                TString line0(dLines[0].GetTrim());
                TString line1(dLines[1].GetTrim());

                // Invalid JavaScript code detected.
                // To-Do: Develop more sophisticated means of reporting this had happened
                if (line0.StartsWith(L"if") && line0.EndsWith(L")") && line1.StartsWith(L"else"))
                    return 7;

                // if statement already ends with a semicolon, no point in adding another one
                if (line0.EndsWith(L';'))
                    insertSemiColon = false;

                if (line0.FindOneOf(L" \t") == -1)
                {
                    if (!line1.StartsWith(L"++") && line1.StartsWith(L"--"))
                        insertSemiColon = false;
                }

                if (line0.EndsWith(L"{"))
                    insertSemiColon = false;

                for (UINT Rust = 0; Rust < noSemiColonEnd.Size() && insertSemiColon; Rust++)
                {
                    if (line0.EndsWith(noSemiColonEnd[Rust]))
                        insertSemiColon = false;
                    
                }
            }

            if (insertSemiColon)
                dLines[0].AppendChar(L';');
        }

        newFile.WriteString(dLines[0] + L'\n');

        TString emptyLine((inMultiString) ? L"\n" : L";\n");

        while (empty--)
            newFile.WriteString(emptyLine);

        inMultiString = hasOddMiltiLineStrMarkers(readLine);

        dLines[0].Set(dLines[1]);
    }


    newFile.WriteString(readLine);


    file->Close();

    TString path(newFile.GetFilePath());

    newFile.Close();

    file->Open(path, TFile::t_file_open_always);

    if (file->IsOpen())
    {
        end = file->GetLength();

        file->SeekToBegin();

        return 0;
    }

    return 8;
}

void TJavaScriptInterpretor::ProcessParenthBlock(ReportObject& ro, const TString& currentCode, UINT line)
{
    TString trimedCode(currentCode.GetTrim());

    if (!trimedCode.GetSize())
    {
        ro.returnCode = ReportObject::incomplete_statement;
        ro.errorMessage.Set(L"Unexpected EOF!");

        TString stack;
        stack.Format(L"\t at line %d", line);

        ro.stackTrace.push_back(stack);

        return;
    }

    if (!trimedCode[0] != L'(')
    {
        ro.returnCode = ReportObject::mismatched_parehtnesis;
        ro.errorMessage.Set(L"Expected token '(' after ");

        TString stack;
        stack.Format(L"\t at line %d", line);

        ro.stackTrace.push_back(stack);

        return;
    }

    UINT parenthesisStack;



    parenthesisStack = 1;

    for (UINT Rust = 1; Rust < trimedCode.GetSize(); Rust++)
    {
        if (trimedCode[Rust] == L'(')
            parenthesisStack++;
        if (trimedCode[Rust] == L')')
            parenthesisStack--;

        if (!parenthesisStack)
            break;
    }

    if (parenthesisStack)
    {
        TString code;
        while (parenthesisStack)
        {
            
            if (!file->ReadString(code, L")", 0b00000111) || !code.EndsWith(L")"))
            {
                ro.returnCode = ro.mismatched_parehtnesis;
                ro.errorMessage.Set(L"Closing Parenthesis ')' not found!");
                TString stack;
                stack.Format(L"\t at line %d", line + code.CountFinds(L"\n"));

                ro.stackTrace.push_back(stack);
                return;
            }

            trimedCode.Append(code);

            parenthesisStack--;
        }
        
        if (!file->ReadString(code, L"{", 0b00000111) || !code.EndsWith(L"{"))
        {
            ro.returnCode = ReportObject::incomplete_block;
            ro.errorMessage.Set(L"Block not found!");

            TString stack;
            stack.Format(L"\t at line %d", line + code.CountFinds(L"\n"));

            ro.stackTrace.push_back(stack);

            return;
        }

        trimedCode.Append(code);
    }

    if (trimedCode.EndsWith(L"{"))
    {
        ro.returnCode = ReportObject::no_error;
        ro.errorMessage.Set(trimedCode);
    }
    else
    {
        ro.returnCode = ReportObject::incomplete_block;
        ro.errorMessage.Set(L"Block Not found");
    }
}

ULONG64 TJavaScriptInterpretor::GetBlockEnd()
{
    TString code, total;

    bool complete = false;

    UINT curlyStack = 1;

    while (file->ReadString(code, L"{`}", 0b00000111))
    {
        // First make sure this statement doesn't land us in a multi-line string (`) which the ReadString
       // Method doesn't account for
        if (code.EndsWith(L"{"))
            curlyStack++;
        if (code.EndsWith(L"}"))
            curlyStack--;
        if (code.EndsWith(L"`"))
        {
            // Within the multi-line string, no need to worry about other quote types
            if (!file->ReadString(code, L"`", 0b00000101) || !code.EndsWith(L"`"))
            {
                return 0;
            }
        }

        if (!curlyStack)
            break;
    }



    return curlyStack ? 0 : file->GetPosition() -1;
}

JavaScriptStatement::JavaScriptStatement(js_statement_type type)
{
    this->type = type;
    fileEnd = 0;
    fileStart = 0;
    lineEnd = 0;
    lineStart = 0;
}

JavaScriptStatement::JavaScriptStatement() : JavaScriptStatement(js_statement_type::js_regular)
{
   
}

void JavaScriptStatement::operator=(const JavaScriptStatement& other)
{
    this->contents.Set(other.contents);
    this->fileEnd = other.fileEnd;
    this->fileStart = other.fileStart;
    this->lineEnd = other.lineEnd;
    this->lineStart = other.lineStart;
    this->type = other.type;
}

JavaScriptStatement::JavaScriptStatement(const JavaScriptStatement& orig)
{
    this->contents.Set(orig.contents);
    this->fileEnd = orig.fileEnd;
    this->fileStart = orig.fileStart;
    this->lineEnd = orig.lineEnd;
    this->lineStart = orig.lineStart;
    this->type = orig.type;
}
