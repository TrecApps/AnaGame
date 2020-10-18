#include "TJavaScriptInterpretor.h"
#include <DirectoryInterface.h>
#include <TStringVariable.h>
#include <cassert>
#include <TPrimitiveVariable.h>


static TDataArray<WCHAR> noSemiColonEnd;

static TDataArray<TString> standardOperators;

static TrecPointer<TVariable> one;

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

    if (!standardOperators.Size())
    {
        standardOperators.push_back(L">>>=");
        standardOperators.push_back(L"===");
        standardOperators.push_back(L"!==");
        standardOperators.push_back(L">>>");
        standardOperators.push_back(L"<<=");
        standardOperators.push_back(L">>=");
        standardOperators.push_back(L"&&=");
        standardOperators.push_back(L"||=");
        standardOperators.push_back(L"??=");
        standardOperators.push_back(L"**=");
        standardOperators.push_back(L"++");
        standardOperators.push_back(L"--");
        standardOperators.push_back(L"**");
        standardOperators.push_back(L"<<");
        standardOperators.push_back(L">>");
        standardOperators.push_back(L"<=");
        standardOperators.push_back(L">=");
        standardOperators.push_back(L"==");
        standardOperators.push_back(L"!=");
        standardOperators.push_back(L"&&");
        standardOperators.push_back(L"||");
        standardOperators.push_back(L"??");
        standardOperators.push_back(L"+=");
        standardOperators.push_back(L"-=");
        standardOperators.push_back(L"*=");
        standardOperators.push_back(L"/=");
        standardOperators.push_back(L"%=");
        standardOperators.push_back(L"&=");
        standardOperators.push_back(L"^=");
        standardOperators.push_back(L"|=");
    }

    if (!one.Get())
    {
        one = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(static_cast<int>(1));
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

                // To-Do: Add Stack code


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
        else if (startStatement.StartsWith(L"else", false, true))
        {
            startStatement.Set(startStatement.SubString(5).GetTrimLeft());

            bool useIf = false;

            if (startStatement.StartsWith(L"if", false, true))
            {
                
                if (startParenth == -1 || startStatement.SubString(2, startParenth).GetTrim().GetSize())
                {
                    ret.returnCode = ret.broken_reference;
                    ret.errorMessage.Set(L"Unexpected token after 'if' statement!");

                    // To-Do: Add Stack code


                    return ret;
                }


                ProcessParenthBlock(ret, startStatement.SubString(2), line);

                if (ret.returnCode)
                    return ret;
                useIf = true;
            }
            else if (!startStatement.EndsWith(L"{"))
            {
                ret.returnCode = ret.broken_reference;
                ret.errorMessage.Set(L"No Block detected for 'else' statement!");

                // To-Do: Add Stack code


                return ret;
            }
            JavaScriptStatement statement(useIf ? js_statement_type::js_else_if : js_statement_type::js_else);

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
            int startParenth = startStatement.Find(L'(');
            if (startParenth == -1)
            {
                ret.returnCode = ret.mismatched_parehtnesis;
                ret.errorMessage.Set(L"Unexpected token detected in 'function-block statement'");

                return ret;
            }


            ProcessParenthBlock(ret, startStatement.SubString(startParenth), line);

            if (ret.returnCode)
                return ret;

            JavaScriptStatement statement(js_statement_type::js_function);

            statement.contents.Set(startStatement.SubString(0, startParenth) + ret.errorMessage);
            statement.lineStart = beginLine;
            statement.lineEnd = line;


            statement.fileStart = file->GetPosition() + 1;

            statement.fileEnd = GetBlockEnd();
            if (!statement.fileEnd)
            {
                ret.returnCode = ret.incomplete_block;
                ret.errorMessage.Set(L"function-block does not have a complete block");



                return ret;
            }
            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"let", false, true))
        {
            JavaScriptStatement statement(js_statement_type::js_let);

            statement.lineStart = beginLine;
            statement.lineEnd = line;

            statement.contents.Set(startStatement.SubString(4).GetTrim());

            statements.push_back(statement);
        }
        else if (startStatement.StartsWith(L"var", false, true))
        {
            JavaScriptStatement statement(js_statement_type::js_var);
            statement.lineStart = beginLine;
            statement.lineEnd = line;

            statement.contents.Set(startStatement.SubString(4).GetTrim());

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

            statement.contents.Set(startStatement.GetTrim());

            statements.push_back(statement);
        }

    }

    for (UINT Rust = 0; Rust < statements.Size(); Rust++)
    {
        switch (statements[Rust].type)
        {
        case js_statement_type::js_class:
            ProcessClass(statements, Rust, statements[Rust], ret);
            break;
        case js_statement_type::js_const:
            ProcessConst(statements, Rust, statements[Rust], ret);
            break;
        case js_statement_type::js_for:
            ProcessFor(statements, Rust, statements[Rust], ret);
            break; 
        case js_statement_type::js_function:
            ProcessFunction(statements, Rust, statements[Rust], ret);
            break; 
        case js_statement_type::js_if:
            ProcessIf(statements, Rust, statements[Rust], ret);
            break; 
        case js_statement_type::js_let:
            ProcessLet(statements, Rust, statements[Rust], ret);
            break; 
        case js_statement_type::js_regular:
            ProcessReg(statements, Rust, statements[Rust], ret);
            break; 
        case js_statement_type::js_var:
            ProcessVar(statements, Rust, statements[Rust], ret);
            break;
        case js_statement_type::js_while:
            ProcessWhile(statements, Rust, statements[Rust], ret);
            break;
        case js_statement_type::js_try:
            ProcessTry(statements, Rust, statements[Rust], ret);
        }

        if (ret.returnCode)
            return ret;
    }





    //for (currentPoint = start; currentPoint < end; currentPoint += file->ReadString(code, TString(L";\n{"), TFile::t_file_include_terminator))
    //{
    //    int firstQuote;

    //    

    //    while ((firstQuote = code.FindOneOf(L"\"'`", startQuoteSearch)) != -1)
    //    {
    //        // We have a String in our statement and need to make sure we have the whole statement
    //        WCHAR quoteType = code[firstQuote];

    //        int secondQuote = code.FindOneOf(L" \t", firstQuote + 1);

    //        if (secondQuote == -1)
    //        {
    //            TString code2;
    //            currentPoint += file->ReadString(code2, TString(quoteType), TFile::t_file_include_terminator);
    //            TString code3;
    //            currentPoint += file->ReadString(code3, TString(L";\n{"), TFile::t_file_include_terminator);

    //            code.Append(code2);
    //            startQuoteSearch = code.GetSize() + 1;
    //            code.Append(code3);
    //        }
    //    }

    //    TDataArray<TString> tokens;
    //    TString tok;
    //    WCHAR quoteMode = L'\0';
    //    for (UINT Rust = 0; Rust < code.GetSize(); Rust++)
    //    {
    //        WCHAR curChar = code[Rust];

    //        if (quoteMode)
    //        {
    //            tok.AppendChar(curChar);
    //            if (curChar == quoteMode)
    //                quoteMode = L'\0';

    //            continue;
    //        }

    //        if (curChar == L' ' || curChar == L'\t')
    //        {
    //            if (tok.GetSize())
    //            {
    //                tokens.push_back(tok);
    //                tok.Empty();
    //            }
    //            continue;
    //        }

    //        if (curChar == L'\'' || curChar == L'\"' || curChar == L'`')
    //            quoteMode = curChar;

    //        tok.AppendChar(curChar);
    //    }


    //    // Now Parse the Statement we got
    //    if (tokenLog.IsOpen())
    //    {
    //        for (UINT Rust = 0; Rust < tokens.Size(); Rust++)
    //        {
    //            tokenLog.WriteString(tokens[Rust] + L'\n');
    //        }

    //        tokenLog.WriteString(L"\n\n");
    //    }
    //}

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
        variables.addEntry(paramNames[Rust], TVariableMarker(true, params[Rust]));
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



    return curlyStack ? 0 : file->GetPosition() - 1;
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

void TJavaScriptInterpretor::ProcessIf(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    assert(statement.type == js_statement_type::js_if || statement.type == js_statement_type::js_else_if);

    TString contents(statement.contents);
    ProcessExpression(statements, cur, contents, statement.lineStart, ro);

    if (ro.returnCode)
        return;

    if (IsTruthful(ro.errorObject))
    {
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);


        TDataArray<TrecPointer<TVariable>> params;
        ro = block->Run(params);
    }
    else
    {
        if ((cur + 1) < statements.Size() && (statements[cur + 1].type == js_statement_type::js_else || statements[cur + 1].type == js_statement_type::js_else_if))
        {
            cur++;
            ProcessElse(statements, cur, statements[cur], ro);
        }
    }
}


void TJavaScriptInterpretor::ProcessElse(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    assert(cur && cur < statements.Size());

    if (statements[cur - 1].type != js_statement_type::js_if && statements[cur - 1].type != js_statement_type::js_else_if)
    {
        ro.returnCode = ReportObject::improper_type;
        ro.errorMessage.Set(L"Expected 'if' or 'else if' statement to preceed the 'else statement'!");

        // To-Do: add Stack info

        return;
    }
    if (statement.type == js_statement_type::js_else)
    {
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);


        TDataArray<TrecPointer<TVariable>> params;
        ro = block->Run(params);
    }
    else if (statement.type == js_statement_type::js_else_if)
    {
        ProcessIf(statements, cur, statement, ro);
    }


}

void TJavaScriptInterpretor::ProcessWhile(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    assert(statement.type == js_statement_type::js_while);
    TString contents(statement.contents);
    ProcessExpression(statements, cur, contents, statement.lineStart, ro);

    while (!ro.returnCode && IsTruthful(ro.errorObject))
    {
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);


        TDataArray<TrecPointer<TVariable>> params;
        ro = block->Run(params);
        if (ro.returnCode)
            return;

        ProcessExpression(statements, cur, contents, statement.lineStart, ro);
    }
}

void TJavaScriptInterpretor::ProcessFor(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    TrecPointer<TDataArray<TString>> fields = statement.contents.split(L";", 2 + 8);

    if (fields->Size() == 1)
    {
        TrecPointer<TDataArray<TString>> tokens = fields->at(0).split(L" \s\n\r\t", 0);

        if (tokens->Size() != 3)
        {
            ro.returnCode = ro.broken_reference;
            ro.errorMessage.Set(L"Unexpected token in for loop");
            // To-Do - Stack

            return;
        }

        TString varName(tokens->at(0).GetTrim());

        CheckVarName(varName, ro, statement.lineStart);

        if (ro.returnCode)
            return;

        TString middle(tokens->at(1).GetTrim());

        if (middle.Compare(L"in") && middle.Compare(L"of"))
        {
            ro.returnCode = ro.broken_reference;
            ro.errorMessage.Set(L"Unexpected token in 'for' statement!");

            return;
        }


        bool wasPresent;
        TString collName(tokens->at(2).GetTrim());
        TrecPointer<TVariable> collection = GetVariable(collName, wasPresent);

        if (!collection.Get())
        {
            ro.returnCode = ro.broken_reference;

            ro.errorMessage.Format(L"For loop found %ws to be %ws, expected a collection!", collName.GetConstantBuffer(), (wasPresent) ? L"null" : L"undefined");

            return;
        }

        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
        TDataArray<TString> pNames;
        pNames.push_back(varName);
        dynamic_cast<TInterpretor*>(block.Get())->SetParamNames(pNames);

        TDataArray<TrecPointer<TVariable>> params;

        if (collection->GetVarType() == var_type::collection)
        {
            for (UINT Rust = 0; Rust < collection->GetSize(); Rust++)
            {
                params.RemoveAll();
                params.push_back(dynamic_cast<TContainerVariable*>(collection.Get())->GetValueAt(Rust));

                ro = block->Run(params);
                if (ro.returnCode)
                    return;
            }
        }
        else if (collection->GetVarType() == var_type::string)
        {
            TString collString(collection->GetString());
            for (UINT Rust = 0; Rust < collString.GetSize(); Rust++)
            {
                params.RemoveAll();
                params.push_back(TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(collString[Rust]));

                ro = block->Run(params);
                if (ro.returnCode)
                    return;
            }
        }
    }
    else if (fields->Size() == 3)
    {
        TString initialStatement(fields->at(0).GetTrim());
        TString condition(fields->at(1).GetTrim());
        TString update(fields->at(2).GetTrim());

        JavaScriptStatement startStatement;

        TDataMap<TVariableMarker> vars;

        if (initialStatement.StartsWith(L"var", false, true))
        {
            startStatement.type = js_statement_type::js_let;
            startStatement.contents.Set(initialStatement.SubString(4));
            startStatement.lineStart = startStatement.lineEnd = statement.lineStart;
            AssignmentStatement(statements, 0, startStatement, ro, vars);
        }
        else if (initialStatement.StartsWith(L"let", false, true))
        {
            startStatement.type = js_statement_type::js_let;
            startStatement.contents.Set(initialStatement.SubString(4));
            startStatement.lineStart = startStatement.lineEnd = statement.lineStart;
            AssignmentStatement(statements, 0, startStatement, ro, vars);
        }
        else
        {
            startStatement.type = js_statement_type::js_regular;
            startStatement.contents.Set(initialStatement);
            startStatement.lineStart = startStatement.lineEnd = statement.lineStart;
            ProcessReg(statements, cur, startStatement, ro);
        }
        if (ro.returnCode)
            return;
       
        TDataArray<TString> varNames;
        TDataArray<TrecPointer<TVariable>> varValues;

        for (UINT Rust = 0; Rust < vars.count(); Rust++)
        {
            TDataEntry<TVariableMarker> mark;
            if (!vars.GetEntryAt(Rust, mark))
                continue;

            varNames.push_back(mark.key);
            varValues.push_back(mark.object.GetVariable());
        }

        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
        dynamic_cast<TInterpretor*>(block.Get())->SetParamNames(varNames);
        ProcessExpression(statements, cur, condition, statement.lineStart, ro);

        while (!ro.returnCode && IsTruthful(ro.errorObject))
        {
            ro = block->Run(varValues);

            if (ro.returnCode)return;

            // Process the 
            ProcessExpression(statements, cur, update, statement.lineStart, ro);
            if (ro.returnCode)return;

            ProcessExpression(statements, cur, condition, statement.lineStart, ro);
        }
    }
    else
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"For loop has inproper number of statements!");

    }
}

void TJavaScriptInterpretor::ProcessVar(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    AssignmentStatement(statements, cur, statement, ro, variables);
}

void TJavaScriptInterpretor::ProcessLet(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    AssignmentStatement(statements, cur, statement, ro, variables);
}

void TJavaScriptInterpretor::ProcessConst(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    AssignmentStatement(statements, cur, statement, ro, variables);
}

void TJavaScriptInterpretor::ProcessFunction(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    TString var(statement.contents.SubString(0, statement.contents.Find(L'(')).GetTrim());

    CheckVarName(var, ro, statement.lineStart);

    if (ro.returnCode)
        return;

    TString parametersString(statement.contents.SubString(statement.contents.Find(L'(') + 1));

    auto paramsList = parametersString.split(L',');

    TDataArray<TString> paramNames;

    for (UINT Rust = 0; Rust < paramsList->Size(); Rust++)
    {

        paramsList->at(Rust).Trim();

        TString paramName(paramsList->at(Rust));
        CheckVarName(paramName, ro, statement.lineStart);
        if (ro.returnCode)
            return;
        paramNames.push_back(paramName);
    }

    TrecSubPointer<TVariable, TJavaScriptInterpretor> function = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(function.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
    function->SetParamNames(paramNames);

    variables.addEntry(var, TVariableMarker(true, TrecPointerKey::GetTrecPointerFromSub<TVariable, TJavaScriptInterpretor>(function)));
}

void TJavaScriptInterpretor::ProcessClass(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
}

void TJavaScriptInterpretor::ProcessReg(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
}

void TJavaScriptInterpretor::ProcessTry(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    if (cur + 1 == statements.Size())
    {
        ro.incomplete_statement;
        ro.errorMessage.Set(L"Try cannot be the last statement in the Script!");

        // To-Do: Handle stack


        return;
    }

    if (statements[cur + 1].type != js_statement_type::js_catch || statements[cur + 1].type != js_statement_type::js_finally)
    {
        ro.incomplete_statement;
        ro.errorMessage.Set(L"Try must be followed by a catch or finally block!");

        // To-Do: Handle stack


        return;
    }


    TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);


    TDataArray<TrecPointer<TVariable>> params;
    ro = block->Run(params);

    if (ro.returnCode)
    {
        // Something was thrown
        if (statements[++cur].type == js_statement_type::js_catch)
            ProcessCatch(statements, cur, statements[cur], ro);
        else
            ProcessFinally(statements, cur, statements[cur], ro);
    }
    else
    {
        if (statements[++cur].type == js_statement_type::js_finally)
            ProcessFinally(statements, cur, statements[cur], ro);
        else if(statements[cur + 1].type == js_statement_type::js_finally)
            ProcessFinally(statements, ++cur, statements[cur], ro);
    }
}

void TJavaScriptInterpretor::ProcessCatch(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    TString erName(statement.contents.GetTrim());

    ReportObject ro2;
    CheckVarName(erName, ro2, statement.lineStart);

    if (ro2.returnCode)
    {
        ro = ro2;
        return;
    }

    ro.returnCode = 0;

    // To-Do: Set up errorObject

    ro.errorMessage.Empty();

    // Set up error parameter
    TDataArray<TString> erParamName;
    erParamName.push_back(erName);
    TDataArray<TrecPointer<TVariable>> erObject;
    erObject.push_back(ro.errorObject);


    TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);


    dynamic_cast<TInterpretor*>(block.Get())->SetParamNames(erParamName);
    ro = block->Run(erObject);

    if (cur + 1 < statements.Size() && statements[cur + 1].type == js_statement_type::js_finally)
        ProcessFinally(statements, ++cur, statements[cur], ro);
}

void TJavaScriptInterpretor::ProcessFinally(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);


    TDataArray<TrecPointer<TVariable>> params;
    auto ro2 = block->Run(params);

    if (ro.returnCode && !ro2.returnCode)
        return;
    ro = ro2;
}

// inspiration for how to pull this off came from https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_Precedence#Table

void TJavaScriptInterpretor::AssignmentStatement(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro, TDataMap<TVariableMarker>& variables)
{
    assert(statement.type == js_statement_type::js_let);

    auto toks = statement.contents.split(L',', 2);

    UINT newLines = 0;

    TString statementType;
    bool mutableValue = true;
    switch (statement.type)
    {
    case js_statement_type::js_const:
        statementType.Set(L"const");
        mutableValue = false;
        break;
    case js_statement_type::js_let:
        statementType.Set(L"let");
        break;
    case js_statement_type::js_var:
        statementType.Set(L"var");
    }

    if (toks->Size())
    {
        for (UINT Rust = 0; Rust < toks->Size(); Rust++)
        {
            auto eqToks = toks->at(Rust).splitn(L'=', 2, 2);

            if (!eqToks->Size())
            {
                ro.returnCode = ro.incomplete_statement;
                ro.errorMessage.Format(L"Empty Sub-statement detected in '%ws' statament!", statementType.GetConstantBuffer());


                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), statement.lineStart + newLines);
                ro.stackTrace.push_back(stack);
            }

            TString varname(eqToks->at(0));

            newLines += varname.CountFinds(L'\n');

            varname.Trim();

            CheckVarName(varname, ro, statement.lineStart + newLines);

            if (ro.returnCode)
                return;

            if (eqToks->Size() == 1)
            {
                if(mutableValue)
                    variables.addEntry(varname, TVariableMarker());
                else
                {
                    ro.returnCode = ro.incomplete_statement;
                    ro.errorMessage.Set(L"Variables declared in a 'const' statement must be assigned in the statement they are declared!");

                    TString stack;
                    stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), statement.lineStart + newLines);
                    ro.stackTrace.push_back(stack);
                    return;
                }
            }
            else
            {
                TString exp(eqToks->at(1));
                ProcessExpression(statements, cur, exp, statement.lineStart + newLines, ro);
                if (ro.returnCode)
                    return;
                variables.addEntry(varname, TVariableMarker(mutableValue, ro.errorObject));
                newLines += exp.CountFinds(L'\n');
            }
        }
    }
}

void TJavaScriptInterpretor::ProcessExpression(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& expParam, UINT line, ReportObject& ro)
{
    TString exp(expParam.GetTrim());


    if (!exp.GetSize())
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"Expression amounted to a blank!");
        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ro.stackTrace.push_back(stack);

        return;
    }

    TDataArray<TrecPointer<TVariable>> expresions;

    TDataArray<TString> operators;

    UINT curSize;

    while (curSize = exp.GetSize())
    {
        if (exp[0] == L'(')
        {
            UINT stack = 1;

            UINT end;

            for (end = 1; end < exp.GetSize() && stack; end++)
            {
                if (exp[end] == L'(')
                    stack++;
                else if (exp[end] == L')')
                    stack--;
            }

            if (stack)
            {
                ro.returnCode = ro.mismatched_parehtnesis;
                ro.errorMessage.Format(L"Mismatched Parenthesis, needed %i more to close it!", stack);
                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
                ro.stackTrace.push_back(stack);

                return;
            }

            TString smallExp(exp.SubString(1, end));

            ProcessExpression(statements, cur, smallExp, line, ro);

            if (ro.returnCode)
            {
                return;
            }

            expresions.push_back(ro.errorObject);

            exp.Set(exp.SubString(end + 1).GetTrim());
        }
        else if (exp[0] == L'[')
        {
            UINT stack = 1;

            UINT end;

            for (end = 1; end < exp.GetSize() && stack; end++)
            {
                if (exp[end] == L'[')
                    stack++;
                else if (exp[end] == L']')
                    stack--;
            }

            if (stack)
            {
                ro.returnCode = ro.mismatched_parehtnesis;
                ro.errorMessage.Format(L"Mismatched Square Brackets, needed %i more to close it!", stack);
                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
                ro.stackTrace.push_back(stack);

                return;
            }

            TString smallExp(exp.SubString(1, end));

            ProcessArrayExpression(statements, cur, smallExp, line, ro);

            if (ro.returnCode)
            {
                return;
            }

            expresions.push_back(ro.errorObject);

            exp.Set(exp.SubString(end + 1).GetTrim());
        }
        else if (exp[0] >= L'0' && exp[0] <= L'9')
        {
            InspectNumber(exp, line, ro);
            if (ro.returnCode)
                return;
            expresions.push_back(ro.errorObject);
        }
        else if ((exp[0] == L'_') || (exp[0] >= L'a' && exp[0] <= L'z') || (exp[0] >= L'A' && exp[0] <= L'Z'))
        {
            InspectVariable(statements, cur, exp, line, ro);
            //InspectNumber(exp, line, ro);
            if (ro.returnCode)
                return;
            expresions.push_back(ro.errorObject);
        }
        else if (exp[0] == L'\'' || exp[0] == L'\"' || exp[0] == L'\`')
        {
            int loc = exp.Find(exp[0], 1, false);

            if (loc == -1)
            {
                ro.returnCode = ro.incomplete_statement;
                ro.errorMessage.Set(L"Unfinished String Expression!");

                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
                ro.stackTrace.push_back(stack);

                return;
            }

            expresions.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(exp.SubString(1, loc)));

            exp.Set(exp.SubString(loc + 1));
        }

        // Now Check for an operator

        exp.Trim();

        bool foundOp = false;

        for (UINT Rust = 0; Rust < standardOperators.Size(); Rust++)
        {
            if (exp.StartsWith(standardOperators[Rust]))
            {
                operators.push_back(standardOperators[Rust]);
                foundOp = true;
                exp.Delete(0, standardOperators[Rust].GetSize());
                break;
            }
        }

        if (!foundOp)
        {
            if (!exp.FindOneOf(L"+-*/%^&|<>?=~!,"))
            {
                operators.push_back(TString(exp[0]));
                exp.Delete(0, 1);
            }
        }
    }

    HandlePreExpr(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleExponents(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleMultDiv(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleAddSub(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleBitwiseShift(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleLogicalComparison(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleEquality(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleBitwiseAnd(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleBitwiseXor(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleBitwiseOr(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleLogicalAnd(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleLogicalOr(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleNullish(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleConditional(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleComma(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;
}

void TJavaScriptInterpretor::ProcessArrayExpression(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro)
{
    if (exp.EndsWith(L"]"))
        exp.Set(exp.SubString(0, exp.GetSize() - 1));

    auto pieces = exp.split(L",", 0b00000010);

    TrecSubPointer<TVariable, TContainerVariable> array_ = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);



    for (UINT Rust = 0; Rust < pieces->Size(); Rust++)
    {
        TString piece(pieces->at(Rust));

        ProcessExpression(statements, cur, piece, line, ro);

        if (ro.returnCode)
        {
            return;
        }

        array_->AppendValue(ro.errorObject);
    }

    ro.errorObject = TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(array_);
}

void TJavaScriptInterpretor::ProcessFunctionDef(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro)
{
    TString trimmed(exp.GetTrim());

    if (!trimmed.EndsWith(L'{'))
    {
        ro.returnCode = ro.broken_reference;


        return;
    }

    if (!trimmed.StartsWith(L'('))
    {
        ro.returnCode = ro.broken_reference;



        return;
    }

    int endParam = trimmed.FindLast(L')');

    if (endParam == -1)
    {
        ro.returnCode = ro.incomplete_statement;


        return;
    }

    ULONG64 start = file->GetPosition() + 1;
    ULONG64 end = GetBlockEnd();

    TString parametersString(trimmed.SubString(1, endParam));

    auto paramsList = parametersString.split(L',');

    TDataArray<TString> paramNames;

    for (UINT Rust = 0; Rust < paramsList->Size(); Rust++)
    {

        paramsList->at(Rust).Trim();

        TString paramName(paramsList->at(Rust));
        CheckVarName(paramName, ro, line);
        if (ro.returnCode)
            return;
        paramNames.push_back(paramName);
    }

    TrecSubPointer<TVariable, TJavaScriptInterpretor> function = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(function.Get())->SetCode(file, start, end);
    function->SetParamNames(paramNames);

    ro.errorObject = TrecPointerKey::GetTrecPointerFromSub<TVariable, TJavaScriptInterpretor>(function);
}

void TJavaScriptInterpretor::InspectNumber(TString& exp, UINT line, ReportObject& ro)
{
    TString tExp(exp.GetTrimRight());

    UINT frontDifference = exp.GetSize() - tExp.GetSize();

    _ASSERT(tExp.GetSize());

    UINT start = 0, end;

    if (tExp.StartsWith(L"0x", true))
    {
        for (end = 2; end < tExp.GetSize(); end++)
        {
            WCHAR letter = tExp[end];

            if ((letter >= L'0' && letter <= L'9') ||
                (letter >= L'a' && letter <= L'f') ||
                (letter >= L'A' && letter <= L'F') ||
                letter == L'_')
                continue;
            break;
        }
    }
    else if (tExp.StartsWith(L"0b", true))
    {
        for (end = (tExp[0] == L'-') ? 1 : 0; end < tExp.GetSize(); end++)
        {
            WCHAR letter = tExp[end];

            if (letter == L'0' || letter == L'1' || letter == L'_')
                continue;
            break;
        }
    }
    else
    {
        for (end = 2; end < tExp.GetSize(); end++)
        {
            WCHAR letter = tExp[end];

            if ((letter >= L'0' && letter <= L'9') ||
                letter == L'.' || letter == L'_')
                continue;
            break;
        }
    }

    tExp.Set(tExp.SubString(start, end));

    if (tExp.Find(L'.') != -1)
    {
        double d;
        auto res = tExp.ConvertToDouble(d);
        if (res)
        {
            ro.returnCode = ro.not_number;
            ro.errorMessage.Set(L"Invalid double detected.");
            return;
        }
        ro.returnCode = 0;
        ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(d);
    }
    else
    {
        LONG64 l;

        if (tExp.ConvertToLong(l))
        {

            ro.returnCode = ro.not_number;
            ro.errorMessage.Set(L"Invalid double detected.");
            return;
        }
        ro.returnCode = 0;
        ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(l);
    }

    exp.Set(exp.SubString(frontDifference + end));
}

void TJavaScriptInterpretor::InspectVariable(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro)
{
    exp.TrimRight();



    _ASSERT(exp.GetSize());

    UINT start = 0, end;

    for (end = 0; end < exp.GetSize(); end++)
    {
        WCHAR letter = exp[end];

        if ((letter >= L'0' && letter <= L'9') ||
            (letter >= L'a' && letter <= L'z') ||
            (letter >= L'A' && letter <= L'Z') ||
            letter == L'_')
            continue;
        break;
    }

    // See if it a "function" expression
    TString varName(exp.SubString(0, end));
    if (!varName.Compare(L"function"))
    {
        exp.Delete(0, 8);
        exp.TrimRight();

        ProcessFunctionDef(statements, cur, exp, line, ro);
        return;
    }


    // See if we need to call a Procedure
    bool procedureCall = false;

    for (UINT fEnd = end; fEnd < exp.GetSize(); fEnd++)
    {
        WCHAR letter = exp[fEnd];

        if (letter == L'(')
        {
            procedureCall = true;
            break;
        }

        if (letter != L' ')
            break;
    }

    if (procedureCall)
    {
        ProcessProcedureCall(exp, line, ro);
    }
    else
    {
        TString varname(exp.SubString(0, end));
        bool present;
        ro.errorObject = GetVariable(varname, present);

        exp.Set(exp.SubString(end));
    }
}

void TJavaScriptInterpretor::ProcessProcedureCall(TString& exp, UINT line, ReportObject& ro)
{
}

void TJavaScriptInterpretor::HandlePreExpr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expresions, TDataArray<TString>& operators, ReportObject& ro)
{
    for (UINT Rust = 0; Rust < operators.Size(); Rust++)
    {
        if (Rust >= expresions.Size())
        {
            ro.returnCode = ro.too_few_params;
            ro.errorMessage.Set(L"Too many operators for the expression List!");

            return;
        }


        bool remove = false;
        if (!operators[Rust].Compare(L"++"))
        {
            TrecPointer<TVariable> var = expresions[Rust];
            remove = true;
            // To-Do: Add 1 to var
            ro = ProcessAddition(var, one);
            if (ro.returnCode)
                return;
            expresions[Rust] = ro.errorObject;
        }
        else if (operators[Rust].Compare(L"--"))
        {
            TrecPointer<TVariable> var = expresions[Rust];
            remove = true;
            // To-Do: Subtract 1 from var
            ro = ProcessSubtraction(var, one);
            if (ro.returnCode)
                return;
            expresions[Rust] = ro.errorObject;

        }

        if (remove)
        {
            operators.RemoveAt(Rust--);
        }

    }
}

void TJavaScriptInterpretor::HandleExponents(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expresions, TDataArray<TString>& operators, ReportObject& ro)
{
    if (expresions.Size() != operators.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-Exponent handler expected one more Expression than operator!");
        return;
    }

    if (operators.Size())
    {
        // JavaScript Processes exponents from right to left, so make sure that operators are available and 
        // Start towars the end instead of at 0
        for (int Rust = static_cast<int>(operators.Size()) - 1; Rust >= 0; Rust--)
        {
            if (!operators[Rust].Compare(L"**"))
            {
                ro = this->ProcessExponent(expresions[Rust], expresions[Rust + 1]);
                if (ro.returnCode)
                    return;

                expresions[Rust] = ro.errorObject;
                expresions.RemoveAt(Rust + 1);
                operators.RemoveAt(Rust--);
            }
        }
    }
}

void TJavaScriptInterpretor::HandleMultDiv(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The JS-Multiplication-Division handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].CompareNoCase(L"*"))
        {
            errorMessage = ProcessMultiplication(expressions[Rust], expressions[Rust + 1]);
            if (errorMessage.returnCode)
                return;

            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"/"))
        {
            errorMessage = ProcessDivision(expressions[Rust], expressions[Rust + 1]);
            if (errorMessage.returnCode)
                return;

            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"%"))
        {
            errorMessage = ProcessModDivision(expressions[Rust], expressions[Rust + 1]);
            if (errorMessage.returnCode)
                return;

            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }
    }
}

void TJavaScriptInterpretor::HandleAddSub(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The JS-addition/subtraction handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].CompareNoCase(L"+"))
        {
            errorMessage = ProcessAddition(expressions[Rust], expressions[Rust + 1]);
            if (errorMessage.returnCode)
                return;

            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"-"))
        {
            errorMessage = ProcessSubtraction(expressions[Rust], expressions[Rust + 1]);
            if (errorMessage.returnCode)
                return;

            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseShift(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The JS-addition/subtraction handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool canDo = true;

        TrecSubPointer<TVariable, TPrimitiveVariable> operand = 
            TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(expressions[Rust]);
        if (canDo = (operand.Get() != nullptr))
        {
            canDo = expressions[Rust + 1].Get() != nullptr;
        }
        UINT shift = 0;
        if (canDo)
        {
            if (expressions[Rust + 1]->GetVarType() == var_type::primitive)
                shift = expressions[Rust + 1]->Get4Value();
            else if (expressions[Rust + 1]->GetVarType() == var_type::primitive)
            {
                int iShift;
                if (expressions[Rust + 1]->GetString().ConvertToInt(iShift))
                    canDo = false;
                else
                    shift = iShift;
            }
        }

        bool doBit = false;
        bool doRightBit = false;
        USHORT flag = 0;

        if (!ops[Rust].Compare(L">>"))
        {
            if (!canDo)
            {
                errorMessage.returnCode = ReportObject::broken_reference;

                return;
            }
            doBit = doRightBit = true;
            flag = TPrimitiveVariable::bit_float;
        }
        else if (!ops[Rust].Compare(L">>>"))
        {
            if (!canDo)
            {
                errorMessage.returnCode = ReportObject::broken_reference;

                return;
            }
            doBit = doRightBit = true;
            flag = TPrimitiveVariable::bit_float | TPrimitiveVariable::bit_replenish | TPrimitiveVariable::bit_to_32 | TPrimitiveVariable::bit_to_un_f;
        }
        else if (!ops[Rust].Compare(L"<<"))
        {
            if (!canDo)
            {
                errorMessage.returnCode = ReportObject::broken_reference;

                return;
            }
            doBit = true;
            flag = TPrimitiveVariable::bit_float;
        }

        if (doBit)
        {
            if (operand->BitShift(doRightBit, shift, flag))
            {
                expressions.RemoveAt(Rust + 1);
                ops.RemoveAt(Rust--);
            }
            else
            {
                errorMessage.returnCode = ReportObject::broken_reference;
                errorMessage.errorMessage.Set(L"Internal Error Attempting to Perform Bitwise manipulation!");
            }
        }
    }
}

void TJavaScriptInterpretor::HandleLogicalComparison(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-addition/subtraction handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        auto left = expressions[Rust];
        auto right = expressions[Rust + 1];

        bool found = false;
        bool val;
        if (!ops[Rust].Compare(L"in"))
        {
            found = true;
            if (!left.Get() || !right.Get() || left->GetVarType() != var_type::string || right->GetVarType() != var_type::collection)
            {
                val = false;
            }
            else
            {
                
                dynamic_cast<TContainerVariable*>(right.Get())->GetValue(left->GetString(), val);
                
            }

        }
        else if (!ops[Rust].Compare(L"instanceof"))
        {
            // To-Do: Implement once type management if implemented for this interpretor to use
        }
        else
        {
            UCHAR larger = 0;
            if (!left.Get() || !right.Get())
            {
                larger = 255;
                val = false;
            }

            if (!larger)
            {
                if (left->GetVarType() == var_type::string && right->GetVarType() == var_type::string)
                {
                    int i = left->GetString().Compare(right->GetString());

                    if (i > 0)
                        larger = 2;
                    else if (i < 0)
                        larger = 1;
                }
                else
                {
                    DoubleLong leftDl = GetValueFromPrimitive(left);
                    DoubleLong rightDl = GetValueFromPrimitive(right);

                    if (leftDl.type == double_long::dl_invalid || rightDl.type == double_long::dl_invalid)
                    {
                        larger = 255;
                        val = false;
                    }
                    else
                    {
                        if (leftDl > rightDl)
                            larger = 2;
                        else if (leftDl < rightDl)
                            larger = 1;
                    }
                }
            }

            if (!ops[Rust].Compare(L"<"))
            {
                found = true;
                val = larger == 1;
            }
            else if (!ops[Rust].Compare(L"<="))
            {
                found = true;
                val = (!larger || larger == 1);
            }
            else if (!ops[Rust].Compare(L">"))
            {
                found = true;
                val = larger == 2;
            }
            else if (!ops[Rust].Compare(L">="))
            {
                found = true;
                val = (!larger || larger == 2);
            }
        }

        if (found)
        {
            expressions[Rust] = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(val);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleEquality(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-addition/subtraction handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool found = false, result;
        auto left = expressions[Rust];
        auto right = expressions[Rust + 1];
        if (!ops[Rust].Compare(L"==="))
        {
            found = true;
            result = IsEqual(left, right, true, false);
        }
        else if (!ops[Rust].Compare(L"=="))
        {
            found = true;
            result = IsEqual(left, right, true, true);
        }
        else if (!ops[Rust].Compare(L"!=="))
        {
            found = true;
            result = IsEqual(left, right, false, false);
        }
        else if (!ops[Rust].Compare(L"!="))
        {
            found = true;
            result = IsEqual(left, right, false, true);
        }

        if (found)
        {
            expressions[Rust] = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(result);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseAnd(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-addition/subtraction handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"&"))
        {
            DoubleLong dl1 = GetValueFromPrimitive( expressions[Rust]);
            DoubleLong dl2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                ro.returnCode = ro.improper_type;

                return;
            }
            expressions[Rust] = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitAnd(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseXor(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-addition/subtraction handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"^"))
        {
            DoubleLong dl1 = GetValueFromPrimitive(expressions[Rust]);
            DoubleLong dl2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                ro.returnCode = ro.improper_type;

                return;
            }
            expressions[Rust] = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitXor(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseOr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-addition/subtraction handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"|"))
        {
            DoubleLong dl1 = GetValueFromPrimitive(expressions[Rust]);
            DoubleLong dl2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                ro.returnCode = ro.improper_type;

                return;
            }
            expressions[Rust] = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitOr(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleLogicalAnd(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expresions, TDataArray<TString>& operators, ReportObject& ro)
{
}

void TJavaScriptInterpretor::HandleLogicalOr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expresions, TDataArray<TString>& operators, ReportObject& ro)
{
}

void TJavaScriptInterpretor::HandleNullish(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expresions, TDataArray<TString>& operators, ReportObject& ro)
{
}

void TJavaScriptInterpretor::HandleConditional(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expresions, TDataArray<TString>& operators, ReportObject& ro)
{
}

void TJavaScriptInterpretor::HandleComma(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<TrecPointer<TVariable>>& expresions, TDataArray<TString>& operators, ReportObject& ro)
{
}

bool TJavaScriptInterpretor::IsTruthful(TrecPointer<TVariable> var)
{
    if (!var.Get())
        return false;

    switch (var->GetVarType())
    {
    case var_type::string:
        // If a String, then only the Empty String is considered Falsy
        return dynamic_cast<TStringVariable*>(var.Get())->GetSize() > 0;
    case var_type::primitive:
        // If a Primitive value, boolean false, 
        return var->Get8Value() > 0;
    }


    return true;
}

DoubleLong TJavaScriptInterpretor::GetValueFromPrimitive(TrecPointer<TVariable> var)
{
    if (var.Get() && var->GetVarType() == var_type::primitive && (var->GetType() & TPrimitiveVariable::type_bool))
    {
        return DoubleLong(var->Get8Value());
    }

    // JavaScript also attempts to Convert Strings to Primitives
    if (var.Get() && var->GetVarType() == var_type::string)
    {
        double d;
        if (!var->GetString().ConvertToDouble(d))
            return DoubleLong(d);
        LONG64 l;
        if (!var->GetString().ConvertToLong(l))
            return DoubleLong(l);
    }
    return TInterpretor::GetValueFromPrimitive(var);
}

bool TJavaScriptInterpretor::IsEqual(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2, bool isEqual, bool castType)
{
    bool eqVal = false, eqType;

    if (var1.Get() && var2.Get())
    {
        DoubleLong dl1 = GetValueFromPrimitive(var1);
        DoubleLong dl2 = GetValueFromPrimitive(var2);

        eqType = dl1.type == dl2.type && var1->GetVarType() == var2->GetVarType() && var1->GetType();

        eqVal = dl1 == dl2;
    }
    else if (var1.Get())
    {
        eqType = false;
        DoubleLong dl = GetValueFromPrimitive(var1);

        eqVal = dl.type == double_long::dl_invalid || dl == DoubleLong((LONG64)0);
    }
    else if (var2.Get())
    {
        eqType = false;
        DoubleLong dl = GetValueFromPrimitive(var2);

        eqVal = dl.type == double_long::dl_invalid || dl == DoubleLong((LONG64)0);
    }
    else eqType == true;

    if (isEqual)
    {
        // Dealing with == or ===
        return eqVal && (castType) ? true : eqType;
    }
    else
    {
        // Dealing with != or !==
        return (castType) ? !eqVal : (!eqVal && !eqType);
    }
}