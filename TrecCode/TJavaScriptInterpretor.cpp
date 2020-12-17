#include "TJavaScriptInterpretor.h"
#include <DirectoryInterface.h>
#include <TStringVariable.h>
#include <cassert>
#include <TPrimitiveVariable.h>
#include "JsConsole.h"
#include "TJavaScriptClassInterpretor.h"


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
    line = 1;
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
            else if (quoteStack.Size() && (line[quoteIndex] == quoteStack[quoteStack.Size() - 1]))
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
        newFile->Open(newPath, TFile::t_file_open_always | TFile::t_file_read);
        if (!newFile->IsOpen())
            return 5;
        this->file = newFile;
        newFile->SeekToBegin();
        this->end = newFile->GetLength();
        this->start = 0LL;

        // Intitialize a Console Object
        variables.addEntry(L"console", TVariableMarker(false, GetJsConsole()));


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
    
    if (statements.Size() == 0)
    {
        ProcessStatements(ret);

        if (ret.returnCode)
            return ret;

        // Prep Sub-Blocks
        for (UINT Rust = 0; Rust < statements.Size(); Rust++)
        {
            switch (statements[Rust].type)
            {
            case js_statement_type::js_catch:
            case js_statement_type::js_else:
            case js_statement_type::js_else_if:
            case js_statement_type::js_finally:
            case js_statement_type::js_for:
            case js_statement_type::js_if:
            case js_statement_type::js_try:
            case js_statement_type::js_while:

                statements[Rust].body = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TInterpretor>(
                    TrecPointerKey::GetTrecPointerFromSub<TVariable, TJavaScriptInterpretor>(TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
                        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment)));
                dynamic_cast<TInterpretor*>(statements[Rust].body.Get())->SetCode(
                    file, statements[Rust].fileStart, statements[Rust].fileEnd);

                dynamic_cast<TJavaScriptInterpretor*>(statements[Rust].body.Get())->ProcessStatements(ret);
                if (ret.returnCode)
                    return ret;

                break;
            case js_statement_type::js_class:
                statements[Rust].body = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TInterpretor>(
                    TrecPointerKey::GetTrecPointerFromSub<TVariable, TJavaScriptClassInterpretor>(TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptClassInterpretor>(
                        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment, JsClassBlockType::jscbt_base)));
                dynamic_cast<TInterpretor*>(statements[Rust].body.Get())->SetCode(
                    file, statements[Rust].fileStart, statements[Rust].fileEnd);
                break;
                
            }
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
            break;
        case js_statement_type::js_return:
            ProcessReturn(statements, Rust, statements[Rust], ret);
            break;
        case js_statement_type::js_break:
            ret.mode = report_mode::report_mode_break;
            return ret;
        case js_statement_type::js_continue:
            ret.mode = report_mode::report_mode_continue;
            return ret;
        }

        if (ret.returnCode || ret.mode == report_mode::report_mode_break ||
            ret.mode == report_mode::report_mode_continue || ret.mode == report_mode::report_mode_return)
        {

            return ret;
        }
    }



    return ReportObject();
}

ReportObject TJavaScriptInterpretor::Run(TDataArray<TrecPointer<TVariable>>& params, bool clearVariables)
{
    ReportObject ret;
    if (!file.Get() || !file->IsOpen())
    {
        ret.returnCode = ReportObject::broken_reference;
        ret.errorMessage.Set(L"Error! No access to the JavaScript file!");
        return ret;
    }
    if(clearVariables)
        variables.clear();
    else
    {
        for (UINT Rust = 0; Rust < paramNames.Size(); Rust++)
        {
            TVariableMarker mark;
            variables.removeEntry(paramNames[Rust], mark);
        }
    }

    for (UINT Rust = 0; Rust < params.Size() && Rust < paramNames.Size(); Rust++)
    {
        variables.addEntry(paramNames[Rust], TVariableMarker(true, params[Rust]));
    }

    return Run();
}

void TJavaScriptInterpretor::ProcessStatements(ReportObject& ro)
{
    if (!file.Get() || !file->IsOpen())
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"JS File Not Set!");
    }

    if (!statements.Size())
    {
        file->Seek(start, FILE_BEGIN);
        ULONGLONG currentPoint = start;

        TString code;

        UINT startQuoteSearch = 0;

        // Read the String with the following flags:
        // 0b00000001 - ensures that our terminating character is included in the resulting string
        // 0b00000010 - Tells the method to not terminate if the termination character is within a quote
        // 0b00000100 - Tells the method to watch out for an odd number of backslashes 
        while ((end - file->GetPosition())? file->ReadString(code, L"{;", 0b00000111, end - file->GetPosition()): 0)
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
                    ro.returnCode = ro.incomplete_block;

                    ro.errorMessage.Set(L"Code File ends in the middle of a multi-line statement!");

                    TString stack;
                    stack.Format(L"\tAt %ws, line %d", file->GetFileName().GetConstantBuffer(), line + appendable.CountFinds(L'\n'));

                    ro.stackTrace.push_back(stack);
                    return ;
                }


                code.Append(appendable);
                file->ReadString(appendable, L"{;", 0b00000111);

                code.Append(appendable);

                inMultiString = hasOddMiltiLineStrMarkers(code);
            }

            line += code.CountFinds(L'\n');


            TString startStatement(code.GetTrimLeft());

            int startParenth = startStatement.Find(L'(');

            if (startStatement.StartsWith(L"if", false, true) || startStatement.StartsWith(L"if("))
            {
                if (startParenth == -1 || startStatement.SubString(2, startParenth).GetTrim().GetSize())
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Set(L"Unexpected token after 'if' statement!");

                    // To-Do: Add Stack code


                    return;
                }


                ProcessParenthBlock(ro, startStatement.SubString(2), line);

                if (ro.returnCode)
                    return;


                JavaScriptStatement statement(js_statement_type::js_if);

                statement.contents.Set(ro.errorMessage);
                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();

                statement.fileEnd = GetBlockEnd();

                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"If-block does not have a complete block");



                    return;
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"else", false, true) || startStatement.StartsWith(L"else{"))
            {
                startStatement.Set(startStatement.SubString(5).GetTrimLeft());

                bool useIf = false;

                if (startStatement.StartsWith(L"if", false, true) || startStatement.StartsWith(L"if("))
                {

                    if (startParenth == -1 || startStatement.SubString(2, startParenth).GetTrim().GetSize())
                    {
                        ro.returnCode = ro.broken_reference;
                        ro.errorMessage.Set(L"Unexpected token after 'if' statement!");

                        // To-Do: Add Stack code


                        return ;
                    }


                    ProcessParenthBlock(ro, startStatement.SubString(2), line);

                    if (ro.returnCode)
                        return;
                    useIf = true;
                }
                else if (!startStatement.EndsWith(L"{"))
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Set(L"No Block detected for 'else' statement!");

                    // To-Do: Add Stack code


                    return;
                }
                JavaScriptStatement statement(useIf ? js_statement_type::js_else_if : js_statement_type::js_else);

                statement.contents.Set(ro.errorMessage);
                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();

                statement.fileEnd = GetBlockEnd();

                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"If-block does not have a complete block");



                    return ;
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"for", false, true) || startStatement.StartsWith(L"for("))
            {
                if (startParenth == -1 || startStatement.SubString(3, startParenth).GetTrim().GetSize())
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Set(L"Unexpected token after 'for' statement!");



                    return ;
                }


                ProcessParenthBlock(ro, startStatement.SubString(3).GetTrimLeft(), line);

                if (ro.returnCode)
                    return ;


                JavaScriptStatement statement(js_statement_type::js_for);

                statement.contents.Set(ro.errorMessage);
                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();

                statement.fileEnd = GetBlockEnd();

                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"For-block does not have a complete block");



                    return ;
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"while", false, true) || startStatement.StartsWith(L"while("))
            {
                if (startParenth == -1 || startStatement.SubString(5, startParenth).GetTrim().GetSize())
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Set(L"Unexpected token after 'while' statement!");



                    return ;
                }


                ProcessParenthBlock(ro, startStatement.SubString(5), line);

                if (ro.returnCode)
                    return ;


                JavaScriptStatement statement(js_statement_type::js_while);

                statement.contents.Set(ro.errorMessage);
                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();

                statement.fileEnd = GetBlockEnd();

                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"While-block does not have a complete block");



                    return ;
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
                    ro.returnCode = ro.mismatched_parehtnesis;
                    ro.errorMessage.Set(L"Unexpected token detected in 'function-block statement'");

                    return ;
                }


                ProcessParenthBlock(ro, startStatement.SubString(startParenth), line);

                if (ro.returnCode)
                    return ;

                JavaScriptStatement statement(js_statement_type::js_function);

                statement.contents.Set(startStatement.SubString(0, startParenth) + ro.errorMessage);
                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();

                statement.fileEnd = GetBlockEnd();
                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"function-block does not have a complete block");



                    return ;
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
                statement.contents.Set(startStatement.SubString(5, startStatement.GetSize() - 1).GetTrim());
                statement.fileStart = file->GetPosition();
                statement.fileEnd = GetBlockEnd();
                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"Class-block does not have a complete block");
                    return;
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"return", false, true) || startStatement.StartsWith(L"return;"))
            {
                JavaScriptStatement statement(js_statement_type::js_return);
                if (startStatement[6] != L';')
                {
                    statement.lineStart = beginLine;
                    statement.lineEnd = line;

                    statement.contents.Set(startStatement.SubString(6).GetTrim());
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"break", false, true) || startStatement.StartsWith(L"break;"))
            {
                statements.push_back(JavaScriptStatement(js_statement_type::js_break));
            }
            else if (startStatement.StartsWith(L"try", false, true) || startStatement.StartsWith(L"try{"))
            {
                if (!startStatement.SubString(3).GetTrim().StartsWith(L'{'))
                {
                    ro.returnCode = ReportObject::incomplete_block;
                    ro.errorMessage.Set(L"Unexpected Token found between 'try' keyword and '{' token");
                    return;
                }

                JavaScriptStatement statement(js_statement_type::js_try);

                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();

                statement.fileEnd = GetBlockEnd();

                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"try-block does not have a complete block");



                    return;
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"catch", false, true) || startStatement.StartsWith(L"catch("))
            {
                if (startParenth == -1 || startStatement.SubString(5, startParenth).GetTrim().GetSize())
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Set(L"Unexpected token after 'catch' statement!");

                // To-Do: Add Stack code


                    return;
                }


                ProcessParenthBlock(ro, startStatement.SubString(5), line);
    
                if (ro.returnCode)
                    return;


                JavaScriptStatement statement(js_statement_type::js_catch);

                statement.contents.Set(ro.errorMessage);
                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();
                   
                statement.fileEnd = GetBlockEnd();

                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"Catch-block does not have a complete block");



                    return;
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"finally", false, true) || startStatement.StartsWith(L"finally{"))
            {
                if (!startStatement.SubString(7).GetTrim().StartsWith(L'{'))
                {
                    ro.returnCode = ReportObject::incomplete_block;
                    ro.errorMessage.Set(L"Unexpected Token found between 'finally' keyword and '{' token");
                    return;
                }

                JavaScriptStatement statement(js_statement_type::js_finally);

                statement.contents.Set(ro.errorMessage);
                statement.lineStart = beginLine;
                statement.lineEnd = line;


                statement.fileStart = file->GetPosition();

                statement.fileEnd = GetBlockEnd();

                if (!statement.fileEnd)
                {
                    ro.returnCode = ro.incomplete_block;
                    ro.errorMessage.Set(L"finally-block does not have a complete block");



                    return;
                }
                statements.push_back(statement);
            }
            else if (startStatement.StartsWith(L"continue", false, true) || startStatement.StartsWith(L"continue;"))
            {
                statements.push_back(JavaScriptStatement(js_statement_type::js_continue));
            }
            else if (!startStatement.StartsWith(L";") && startStatement.GetSize())
            {
                JavaScriptStatement statement(js_statement_type::js_regular);
                statement.lineStart = beginLine;
                statement.lineEnd = line;

                statement.contents.Set(startStatement.GetTrim());

                statements.push_back(statement);
            }


            // Catch scenario where a statement ends with an open bracket and bracket data is not set
            if (statements.Size())
            {
                UINT index = statements.Size() - 1;
                TString tempContents(statements[index].contents);

                if (tempContents.EndsWith(L"{") && !statements[index].fileStart)
                {
                    // bool possibleArray = tempContents.SubString(0, tempContents.GetSize() - 1).GetTrim().EndsWith(L','); //

                    statements[index].fileStart = file->GetPosition();
                    statements[index].fileEnd = GetBlockEnd();
                    file->Seek(statements[index].fileStart, 0);
                }
            }


        }
    }
}

void TJavaScriptInterpretor::setLine(UINT line)
{
    this->line = line;
}

ReportObject TJavaScriptInterpretor::ProcessAddition(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2)
{
    ReportObject ret;

    if (!var1.Get() && !var2.Get())
    {
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0ULL);
        return ret;
    }

    if (var1.Get() && var1->GetVarType() == var_type::string)
    {
        TString value(var1->GetString());
        value.Append(var2.Get() ? var2->GetString() : L"null");
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
        return ret;
    }
    else if (var2.Get() && var2->GetVarType() == var_type::string)
    {
        TString value(var1.Get() ? var1->GetString() : L"null");
        value.Append(var2->GetString());
        ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
        return ret;
    }

    if (var1.Get() && !var2.Get())
    {
        if (var1->GetVarType() == var_type::collection)
        {
            auto varColl = dynamic_cast<TContainerVariable*>(var1.Get());
            TString value;
            for (UINT Rust = 0; Rust < varColl->GetSize(); Rust++)
            {
                auto val = varColl->GetValueAt(Rust);
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer(): L"null");
            }
            if (value.StartsWith(L","))
                value.Delete(0);
            value.Append(L"null");
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
            return ret;
        }
        if (var1->GetVarType() == var_type::primitive)
        {
            ret.errorObject = var1;
            return ret;
        }
    }

    if (var2.Get() && !var1.Get())
    {
        if (var2->GetVarType() == var_type::collection)
        {
            auto varColl = dynamic_cast<TContainerVariable*>(var2.Get());
            TString value;
            for (UINT Rust = 0; Rust < varColl->GetSize(); Rust++)
            {
                auto val = varColl->GetValueAt(Rust);
                value.AppendFormat(L",%ws", val.Get() ? val->GetString().GetConstantBuffer() : L"null");
            }
            if (value.StartsWith(L","))
                value.Delete(0);
            value.Set(TString(L"null") + value);
            ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
            return ret;
        }
        if (var2->GetVarType() == var_type::primitive)
        {
            ret.errorObject = var2;
            return ret;
        }
    }



    return TInterpretor::ProcessAddition(var1, var2);
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
                    if (!line1.StartsWith(L"++") && !line1.StartsWith(L"--"))
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
            {
                dLines[0].TrimRight();
                dLines[0].AppendChar(L';');
            }
        }

        newFile.WriteString(dLines[0] + L'\n');

        TString emptyLine((inMultiString) ? L"\n" : L";\n");

        while (empty)
        {
            empty--;
            newFile.WriteString(emptyLine);
        }

        
        inMultiString = hasOddMiltiLineStrMarkers(readLine);

        dLines[0].Set(dLines[1]);
    }


    newFile.WriteString(dLines[0]);


    file->Close();

    TString path(newFile.GetFilePath());

    newFile.Close();

    file->Open(path, TFile::t_file_open_always | TFile::t_file_read);

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

    if (trimedCode[0] != L'(')
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



    return curlyStack ? 0 : file->GetPosition() - 2;
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

    if (contents.EndsWith(L"{"))
        contents.Delete(contents.GetSize() - 1);
    contents.Trim();

    ProcessExpression(statements, cur, contents, statement.lineStart, ro);

    if (ro.returnCode)
        return;

    if (IsTruthful(ro.errorObject))
    {
        auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

        if (!block.Get())
        {
            block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
                TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
            dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
        }

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
        auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

        if (!block.Get())
        {
            block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
                TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
            dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
        }


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

    if (contents.EndsWith(L"{"))
        contents.Delete(contents.GetSize() - 1);
    contents.Trim();

    ProcessExpression(statements, cur, contents, statement.lineStart, ro);

    while (!ro.returnCode && IsTruthful(ro.errorObject))
    {
        auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

        if (!block.Get())
        {
            block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
                TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
            dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
        }


        TDataArray<TrecPointer<TVariable>> params;
        ro = block->Run(params);

        // Check to see if either an error occured or a return statement has been reached.
        // The For loop is not responsible for handling either scenario
        if (ro.returnCode || ro.mode == report_mode::report_mode_return)
            return;

        // For loop is responsible for handling break and for
        if (ro.mode != report_mode::report_mode_regular)
        {
            // here, we either break or continue
            report_mode m = ro.mode;
            ro.mode = report_mode::report_mode_regular;
            if (m == report_mode::report_mode_break)
                return;
        }

        ProcessExpression(statements, cur, contents, statement.lineStart, ro);
    }
}

void TJavaScriptInterpretor::ProcessFor(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    TrecPointer<TDataArray<TString>> fields = statement.contents.split(L";", 2 + 8);

    if (fields->Size() == 1)
    {
        TrecPointer<TDataArray<TString>> tokens = fields->at(0).split(L" \s\n\r\t", 2);

        // Remove the Open bracket token
        if (tokens->Size() > 3 && tokens->at(tokens->Size() - 1).StartsWith(L'{'))
            tokens->RemoveAt(tokens->Size() - 1);
        // Remove the close parenthesis if part of a seperate token
        if (tokens->Size() > 3 && tokens->at(tokens->Size() - 1).StartsWith(L')'))
            tokens->RemoveAt(tokens->Size() - 1);
        // Remove the open parenthesis if seperate from body of expression
        if (tokens->Size() > 3 && tokens->at(0).EndsWith(L'('))
            tokens->RemoveAt(0);

        if (tokens->Size() != 3)
        {
            ro.returnCode = ro.broken_reference;
            ro.errorMessage.Set(L"Unexpected token in for loop");
            // To-Do - Stack

            return;
        }

        if (tokens->at(0).StartsWith(L"("))
            tokens->at(0).Delete(0);

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

        if (collName.EndsWith(L')'))
            collName.Delete(collName.GetSize() - 1);

        TrecPointer<TVariable> collection = GetVariable(collName, wasPresent);

        if (!collection.Get())
        {
            ro.returnCode = ro.broken_reference;

            ro.errorMessage.Format(L"For loop found %ws to be %ws, expected a collection!", collName.GetConstantBuffer(), (wasPresent) ? L"null" : L"undefined");

            return;
        }

        auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

        if (!block.Get())
        {
            block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
                TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
            dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
        }
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

                // Check to see if either an error occured or a return statement has been reached.
                // The For loop is not responsible for handling either scenario
                if (ro.returnCode || ro.mode == report_mode::report_mode_return)
                    return;

                // For loop is responsible for handling break and for
                if (ro.mode != report_mode::report_mode_regular)
                {
                    // here, we either break or continue
                    report_mode m = ro.mode;
                    ro.mode = report_mode::report_mode_regular;
                    if (m == report_mode::report_mode_break)
                        return;
                }
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

                // Check to see if either an error occured or a return statement has been reached.
                // The For loop is not responsible for handling either scenario
                if (ro.returnCode || ro.mode == report_mode::report_mode_return)
                    return;

                // For loop is responsible for handling break and for
                if (ro.mode != report_mode::report_mode_regular)
                {
                    // here, we either break or continue
                    report_mode m = ro.mode;
                    ro.mode = report_mode::report_mode_regular;
                    if (m == report_mode::report_mode_break)
                        return;
                }
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

        if (initialStatement.StartsWith(L'('))
            initialStatement.Set(initialStatement.SubString(1).GetTrim());


        if (update.EndsWith(L'{'))
        {
            update.Delete(update.GetSize() - 1);
            update.Trim();
        }
        if (update.EndsWith(L')'))
        {
            update.Delete(update.GetSize() - 1);
            update.Trim();
        }
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



        auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
        TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

        if (!block.Get())
        {
            block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
                TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
            dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
        }
        dynamic_cast<TInterpretor*>(block.Get())->SetParamNames(varNames);
        for (UINT Rust = 0; Rust < vars.count(); Rust++)
        {
            TDataEntry<TVariableMarker> mark;
            if (!vars.GetEntryAt(Rust, mark))
                continue;

            block->UpdateVariable(mark.key, mark.object.GetVariable(),true);
        }
        block->ProcessExpression(statements, cur, condition, statement.lineStart, ro);

        while (!ro.returnCode && IsTruthful(ro.errorObject))
        {
            ro = block->Run();

            if (ro.returnCode)return;

            // Check to see if either an error occured or a return statement has been reached.
            // The For loop is not responsible for handling either scenario
            if (ro.returnCode || ro.mode == report_mode::report_mode_return)
                return;

            // For loop is responsible for handling break and for
            if (ro.mode != report_mode::report_mode_regular)
            {
                // here, we either break or continue
                report_mode m = ro.mode;
                ro.mode = report_mode::report_mode_regular;
                if (m == report_mode::report_mode_break)
                    return;
            }

            // Process the 
            block->ProcessExpression(statements, cur, update, statement.lineStart, ro);
            if (ro.returnCode)return;

            block->ProcessExpression(statements, cur, condition, statement.lineStart, ro);
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

    if (var.StartsWith(L"function", false, true))
    {
        var.Set(var.SubString(8).GetTrim());
    }

    CheckVarName(var, ro, statement.lineStart);

    if (ro.returnCode)
        return;

    TString parametersString(statement.contents.SubString(statement.contents.Find(L'(') + 1, statement.contents.Find(L')')));

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
    TrecSubPointer<TVariable, TJavaScriptClassInterpretor> classInt = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptClassInterpretor>(
        TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body));
    assert(classInt.Get());

    classInt->SetClassName(statement.contents);
    classInt->ProcessStatements(ro);
    if (ro.returnCode)
        return;
    TClassStruct classInfo = classInt->GetClassData();

    if (!SubmitClassType(statement.contents, classInfo))
    {
        ro.returnCode = ro.invalid_name;
        ro.errorMessage.Format(L"Class %ws already defined in this scope!", statement.contents.GetConstantBuffer());
        return;
    }
}

void TJavaScriptInterpretor::ProcessReg(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    TString exp(statement.contents);
    ProcessExpression(statements, cur, exp, statement.lineStart,ro);
}

void TJavaScriptInterpretor::ProcessReturn(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    TString exp(statement.contents);
    ProcessExpression(statements, cur, exp, statement.lineStart, ro);
    ro.mode = report_mode::report_mode_return;
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

    if (statements[cur + 1].type != js_statement_type::js_catch && statements[cur + 1].type != js_statement_type::js_finally)
    {
        ro.incomplete_statement;
        ro.errorMessage.Set(L"Try must be followed by a catch or finally block!");

        // To-Do: Handle stack


        return;
    }

    auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
    TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

    if (!block.Get())
    {
        block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
    }

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

    if (erName.EndsWith(L'{'))
        erName.Delete(erName.GetSize() - 1);

    if (erName.CountFinds(L'(') != erName.CountFinds(L')'))
    {
        ro.returnCode = ro.mismatched_parehtnesis;
        ro.errorMessage.Set(L"Mismatched Paremthesis detected at catch block!");
        return;
    }
    erName.Trim();
    while (erName.StartsWith(L'('))
        erName.Delete(0);
    while (erName.EndsWith(L')'))
        erName.Delete(erName.GetSize() - 1);

    CheckVarName(erName, ro2, statement.lineStart);

    if (ro2.returnCode)
    {
        ro = ro2;
        return;
    }

    ro.returnCode = 0;



    // Set up error parameter
    TDataArray<TString> erParamName;
    erParamName.push_back(erName);
    TDataArray<TrecPointer<TVariable>> erObject;
    erObject.push_back(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(ro.errorMessage));
    // To-Do: Set up errorObject

    ro.errorMessage.Empty();

    auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
    TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

    if (!block.Get())
    {
        block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
    }
    dynamic_cast<TInterpretor*>(block.Get())->SetParamNames(erParamName);
    ro = block->Run(erObject);

    if (cur + 1 < statements.Size() && statements[cur + 1].type == js_statement_type::js_finally)
        ProcessFinally(statements, ++cur, statements[cur], ro);
}

void TJavaScriptInterpretor::ProcessFinally(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro)
{
    auto tempInt = TrecPointerKey::GetTrecPointerFromSub<TVariable, TInterpretor>(statement.body);
    TrecSubPointer<TVariable, TJavaScriptInterpretor> block = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TJavaScriptInterpretor>(tempInt);

    if (!block.Get())
    {
        block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);
    }
    TDataArray<TrecPointer<TVariable>> params;
    auto ro2 = block->Run(params);

    if (ro.returnCode && !ro2.returnCode)
        return;
    ro = ro2;
}

// inspiration for how to pull this off came from https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_Precedence#Table

void TJavaScriptInterpretor::AssignmentStatement(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro, TDataMap<TVariableMarker>& variables)
{
    auto toks = statement.contents.split(L',', 2);

    TInterpretor::CorrectSplitStringForParenthesis(toks, L',');

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
        // Join these string if necessary
        UINT squareStack = 0;
        WCHAR quote = L'\0';
        UINT C = 0;
        for (UINT Rust = 0; Rust < toks->Size(); Rust++)
        {
            for (; C < toks->at(Rust).GetSize(); C++)
            {
                WCHAR ch = toks->at(Rust).GetAt(C);
                if (quote && ch == quote)
                    quote = L'\0';
                else if (!quote)
                {
                    if (ch == L'\'' || ch == L'\"' || ch == L'`')
                        quote = ch;
                    else if (ch == L'[')
                        squareStack++;
                    else if (ch == L']')
                    {
                        if (squareStack)
                            squareStack--;
                        else
                        {
                            ro.returnCode = ro.mismatched_parehtnesis;
                            ro.errorMessage.Set(L"Mismatched Square Brackets!");

                            return;
                        }
                    }
                }
            }
            if (squareStack && (Rust + 1) < toks->Size())
            {
                // We are in the middle of an array declaration and should join the strings together again with a comma
                toks->at(Rust).AppendFormat(L",%ws", toks->at(Rust + 1).GetConstantBuffer());
                toks->RemoveAt(Rust + 1);
                Rust--;
            }
            else
                C = 0;
        }



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

void TJavaScriptInterpretor::ProcessExpression(TDataArray<JavaScriptStatement>& statements, UINT& cur, TString& expParam, UINT line, ReportObject& ro)
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

    TDataArray<JavaScriptExpression> expresions;

    TDataArray<TString> operators;

    UINT curSize;

    while ((curSize = exp.GetSize()) && exp.GetTrim().Compare(L";") &&
        exp.GetTrim().Compare(L"{") && exp.GetTrim().Compare(L"}"))
    {
        if (exp[0] == L'(')
        {
            UINT stack = 1;

            UINT endt;

            for (endt = 1; endt < exp.GetSize() && stack; endt++)
            {
                if (exp[endt] == L'(')
                    stack++;
                else if (exp[endt] == L')')
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
            if (exp[endt] != L')')
                endt--;

            TString smallExp(exp.SubString(1, endt));

            ProcessExpression(statements, cur, smallExp, line, ro);

            if (ro.returnCode)
            {
                return;
            }

            expresions.push_back(JavaScriptExpression(L"", ro.errorObject));

            exp.Set(exp.SubString(endt+ 1).GetTrim());
        }
        else if (exp[0] == L'[')
        {
            UINT stack = 1;

            UINT endt;

            for (endt = 1; endt < exp.GetSize() && stack; endt++)
            {
                if (exp[endt] == L'[')
                    stack++;
                else if (exp[endt] == L']')
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

            TString smallExp(exp.SubString(1, endt));

            ProcessArrayExpression(statements, cur, smallExp, line, ro);

            if (ro.returnCode)
            {
                return;
            }

            expresions.push_back(JavaScriptExpression(L"", ro.errorObject));

            exp.Set(exp.SubString(endt + 1).GetTrim());
        }
        else if (exp[0] >= L'0' && exp[0] <= L'9')
        {
            InspectNumber(exp, line, ro);
            if (ro.returnCode)
                return;
            expresions.push_back(JavaScriptExpression(L"", ro.errorObject));
        }
        else if (exp.StartsWith(L"new", false, true))
        {
            exp.Delete(0, 3);
            exp.Trim();
            int openParenth = exp.Find(L'(');
            TString name(exp.SubString(0, openParenth));

            TDataArray<TrecPointer<TVariable>> expressions;
            TrecSubPointer<TVariable, TContainerVariable> newObj = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

            // this new container object becomes "this"
            expressions.push_back(TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(newObj));

            if (openParenth != -1)
            {
                int closeParenth = exp.FindOutOfQuotes(L')');
                if (closeParenth == -1 || closeParenth < openParenth)
                {
                    ro.returnCode = ro.mismatched_parehtnesis;

                    return;
                }

                TString strParams(exp.SubString(openParenth + 1, closeParenth));

                auto strParamsSplit = strParams.split(L',', 3);

                for (UINT Rust = 0; Rust < strParamsSplit->Size(); Rust++)
                {
                    TString data = strParamsSplit->at(Rust).GetTrim();
                    if (!data.GetSize())
                        continue;
                    ProcessExpression(statements, cur, data, line, ro);
                    if (ro.returnCode)
                        return;
                    expressions.push_back(ro.errorObject);
                }
                exp.Set(exp.SubString(closeParenth + 1));
            }
            else
            {
                exp.Delete(0, name.GetSize());
            }

            

            TClassStruct classType;
            if (classes.retrieveEntry(name, classType))
            {
                TClassAttribute att = classType.GetAttributeByName(L"constructor");

                TrecSubPointer<TVariable, TInterpretor> function = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TInterpretor>(att.def);

                if (function.Get())
                {
                    ro = function->Run(expressions);
                    if (ro.returnCode)return;
                    ro.errorObject = expressions[0];
                }
                else
                {
                    // No constructor, just simply go through attributes and recreate their attributes
                    UINT index = 0;
                    while (classType.GetAttributeByIndex(index++, att))
                    {
                        if ((att.other & ATTRIBUTE_STATIC) || !att.name.GetSize())
                            continue;

                        newObj->SetValue(att.name, att.def);
                    }
                    ro.errorObject = expressions[0];
                }
            }
            else
            {
                // Search for a function with the name
                bool present;
                TrecSubPointer<TVariable, TInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TInterpretor>( GetVariable(name, present));

                if (!func.Get())
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Format(L"Error! No Such Class or Function name %ws in new declaration!", name.GetConstantBuffer());
                    return;
                }

                func->SetFirstParamName(L"this");

                ro = func->Run(expressions);

                if (ro.returnCode)
                    return;
                ro.errorObject = expressions[0];
            }

        }
        else if ((exp[0] == L'_') || (exp[0] >= L'a' && exp[0] <= L'z') || (exp[0] >= L'A' && exp[0] <= L'Z'))
        {
            ;
            //InspectNumber(exp, line, ro);
            if (!InspectVariable(statements, cur, exp, line, ro) || ro.returnCode)
                return;
            expresions.push_back(JavaScriptExpression(ro.errorMessage, ro.errorObject));
        }
        else if (exp[0] == L'\'' || exp[0] == L'\"' || exp[0] == L'\`')
        {
            int loc = exp.Find(exp[0], 1, false);

            bool isTemplated = exp[0] == L'\`';

            if (loc == -1)
            {
                ro.returnCode = ro.incomplete_statement;
                ro.errorMessage.Set(L"Unfinished String Expression!");

                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
                ro.stackTrace.push_back(stack);

                return;
            }
            TString theString(exp.SubString(1, loc));

            if (isTemplated)
            {
                UINT startIndex = 0;
                while ((startIndex = theString.FindOutOfQuotes(L"${", startIndex)) != -1)
                {
                    UINT closeBracket = theString.FindOutOfQuotes(L'}', startIndex);
                    if (closeBracket == -1)
                        break;

                    TString var(theString.SubString(startIndex + 2, closeBracket));

                    ProcessExpression(statements, cur, var, line, ro);

                    if (ro.returnCode)
                    {
                        ro.returnCode = 0;
                        continue;
                    }

                    TString res = ro.errorObject.Get() ? ro.errorObject->GetString() : TString(L"undefined");

                    theString.Replace(TString(L"${") + var + L'}', res);
                }
            }

            expresions.push_back(JavaScriptExpression(L"", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(theString)));

            exp.Set(exp.SubString(loc + 1));
        }
        else if (exp[0] == L'{')
        {
            // Here, we are starting a JSON Object. but it has meat afterwards and is assumed that we are already in another JSON Object

            // need to find the boundaries of this bracket
            UINT brack = 1;
            WCHAR quote = L'\0';
            TString jExp;
            for (UINT Rust = 1; Rust < exp.GetSize(); Rust++)
            {
                WCHAR ch = exp[Rust];
                switch (ch)
                {
                case L'{':
                    if (!quote)
                        brack++;
                    break;
                case L'}':
                    if (!quote)
                        brack--;
                    break;
                case L'`':
                case L'\'':
                case L'\"':
                    if (!quote)
                        quote = ch;
                    else if (quote == ch)
                        quote = L'\0';
                }

                if (!brack)
                {
                    jExp.SubString(0, Rust);
                    exp.Delete(0, Rust);
                    break;
                }
            }

            ProcessJsonExpression(statements, cur, jExp, line, ro);
            if (ro.returnCode)return;
            expresions.push_back(JavaScriptExpression(L"", ro.errorObject));
        }

        // Now Check for an operator

        exp.Trim();

        bool foundOp = false;


        // Handle Post-Increment/Decrement
        if (expresions.Size() && expresions[expresions.Size() - 1].varName.GetSize() && ((exp.StartsWith(L"++") || exp.StartsWith(L"--"))))
        {
            bool inc = exp.StartsWith(L"++");

            JavaScriptExpression jExp(expresions[expresions.Size() - 1]);

            jExp.value = (jExp.value.Get()) ? jExp.value->Clone() : jExp.value;


            // Now handle the increment or decrement step
            bool present;
            TrecPointer<TVariable> tempValue = GetVariable(jExp.varName, present);

            if (!tempValue.Get())
            {
                ro.returnCode = ro.broken_reference;

                ro.errorMessage.Format(L"Post-%ws attempt encountered %ws variable!",
                    inc ? L"Increment" : L"Decrement",
                    present ? L"Null": L"Default");
                return;
            }

            ro = inc ? ProcessAddition(tempValue, one) :
                ProcessSubtraction(tempValue, one);

            if (ro.returnCode)
                return;

            UINT updateResult = UpdateVariable(jExp.varName, ro.errorObject);

            ro.errorObject.Nullify();
            if (updateResult == 1)
            {
                ro.returnCode = ro.broken_reference;
                ro.errorMessage.Format(L"Post-%ws attempt encountered %ws variable!",
                    inc ? L"Increment" : L"Decrement",
                    present ? L"Null" : L"Default");
                return;
            }
            if (updateResult == 2)
            {
                ro.returnCode = ro.invalid_name;
                ro.errorMessage.Format(L"Post-%ws attempt to update immutable variable!",
                    inc ? L"Increment" : L"Decrement");
                return;
            }

            exp.Delete(0, 2);
            exp.Trim();
        }


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
        exp.Trim();
    }

    // Check to see if we have an open bracket. If we do, then attempt to define a JSON object
    if (!exp.Compare(L"{"))
    {
        ProcessJsonExpression(statements, cur, exp, line, ro);
        if (ro.returnCode)return;
        expresions.push_back(JavaScriptExpression(L"", ro.errorObject));
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

    HandleAssignment(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    HandleComma(statements, cur, expresions, operators, ro);
    if (ro.returnCode) return;

    if (expresions.Size() == 1)
        ro.errorObject = expresions[0].value;
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

UINT TJavaScriptInterpretor::ProcessFunctionDef(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro)
{
    TString trimmed(exp.GetTrim());

    if (!trimmed.EndsWith(L'{'))
    {
        ro.returnCode = ro.broken_reference;


        return 0;
    }

    if (!trimmed.StartsWith(L'('))
    {
        ro.returnCode = ro.broken_reference;



        return 0;
    }

    int endParam = trimmed.FindLast(L')');

    if (endParam == -1)
    {
        ro.returnCode = ro.incomplete_statement;


        return 0;
    }

    TString parametersString(trimmed.SubString(1, endParam));

    auto paramsList = parametersString.split(L',');

    TDataArray<TString> paramNames;

    for (UINT Rust = 0; Rust < paramsList->Size(); Rust++)
    {

        paramsList->at(Rust).Trim();

        TString paramName(paramsList->at(Rust));
        CheckVarName(paramName, ro, line);
        if (ro.returnCode)
            return 0;
        paramNames.push_back(paramName);
    }

    TrecSubPointer<TVariable, TJavaScriptInterpretor> function = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(function.Get())->SetCode(file, statements[cur].fileStart, statements[cur].fileEnd);
    function->SetParamNames(paramNames);
    UINT ret = 0;
    for (UINT Rust = cur + 1; Rust < statements.Size(); Rust++)
    {
        ret++;
        // Check to see if we are closing the function
        TString state(statements.at(Rust).contents.GetTrim());

        if (state.EndsWith(L';'))
            state.Delete(state.GetSize() - 1);

        bool endt = state.EndsWith(L'}');

        if (!endt || !state.StartsWith(L'}'))
            function->statements.push_back(statements.at(Rust));
        if (endt)
            break;
    }

    if (!ret)
    {
        ro.returnCode = ReportObject::incomplete_block;
        ro.errorMessage.Set(L"Unexpected End of File!");
    }

    for (UINT Rust = 0; Rust < variables.count(); Rust++)
    {
        TDataEntry<TVariableMarker> entry;
        if (!variables.GetEntryAt(Rust, entry))
            continue;

        if (entry.key.GetSize())
        {
            auto tempVar = entry.object.GetVariable();
            TVariableMarker mark(entry.object.IsMutable(), (tempVar.Get()? tempVar->Clone() : tempVar));
            function->variables.addEntry(entry.key, mark);
        }
    }

    ro.errorObject = TrecPointerKey::GetTrecPointerFromSub<TVariable, TJavaScriptInterpretor>(function);
    exp.Empty();
    return ret;
}

void TJavaScriptInterpretor::InspectNumber(TString& exp, UINT line, ReportObject& ro)
{
    TString tExp(exp.GetTrimRight());

    UINT frontDifference = exp.GetSize() - tExp.GetSize();

    _ASSERT(tExp.GetSize());

    UINT start = 0, endt;

    if (tExp.StartsWith(L"0x", true))
    {
        for (endt = 2; endt < tExp.GetSize(); endt++)
        {
            WCHAR letter = tExp[endt];

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
        for (endt = 2; endt < tExp.GetSize(); endt++)
        {
            WCHAR letter = tExp[endt];

            if (letter == L'0' || letter == L'1' || letter == L'_')
                continue;
            break;
        }
    }
    else
    {
        for (endt = (tExp[0] == L'-') ? 1 : 0; endt < tExp.GetSize(); endt++)
        {
            WCHAR letter = tExp[endt];

            if ((letter >= L'0' && letter <= L'9') ||
                letter == L'.' || letter == L'_')
                continue;
            break;
        }
    }

    tExp.Set(tExp.SubString(start, endt));

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
            ro.errorMessage.Set(L"Invalid long detected.");
            return;
        }
        ro.returnCode = 0;
        ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(l);
    }

    exp.Set(exp.SubString(frontDifference + endt));
}

bool TJavaScriptInterpretor::InspectVariable(TDataArray<JavaScriptStatement>& statements, UINT& cur, TString& exp, UINT line, ReportObject& ro)
{
    exp.TrimRight();

    ro.errorMessage.Empty();

    _ASSERT(exp.GetSize());

    UINT start = 0, endt;
    bool present, procedureCall;
    TString varName;
    TrecPointer<TVariable> curVar, objVar;

    // Get Next Variable title
    getNextVar:
    for (endt = 0; endt < exp.GetSize(); endt++)
    {
        WCHAR letter = exp[endt];

        if ((letter >= L'0' && letter <= L'9') ||
            (letter >= L'a' && letter <= L'z') ||
            (letter >= L'A' && letter <= L'Z') ||
            letter == L'_')
            continue;
        break;
    }

    // See if it a "function" expression
    varName.Set(exp.SubString(0, endt));
    TString fullVarName;
    if (!varName.Compare(L"function"))
    {
        exp.Delete(0, 8);
        exp.TrimRight();

        UINT skip = ProcessFunctionDef(statements, cur, exp, line, ro);
        if (!skip)
            return false;
        cur += skip;
        return true;
    }
    objVar = curVar;

    fullVarName.Append(varName);

    if (curVar.Get())
    {
        if (curVar->GetVarType() == var_type::collection)
        {
            curVar = dynamic_cast<TContainerVariable*>(curVar.Get())->GetValue(varName, present);
        }
        else
        {
            ro.returnCode = ro.improper_type;
            ro.errorMessage.Format(L"Variable not a collection variable. Could not get Member name %ws", varName.GetConstantBuffer());
            return false;
        }
    }
    else
    {
        curVar = GetVariable(varName, present);
    }

    exp.Set(exp.SubString(endt).GetTrim());

    if (exp.StartsWith(L"?.") && !curVar.Get())
    {
        ro.errorObject.Nullify();
        return false;
    }
    if (exp.StartsWith(L"?.") || exp.StartsWith(L"."))
    {
        fullVarName.Append(exp[0] == L'?' ? L"?." : L".");
        exp.Delete(0, exp[0] == L'?' ? 2 : 1);
        exp.Trim();
        goto getNextVar;
    }

    


    // See if we need to call a Procedure
    procedureCall = false;

    for (UINT fEnd = 0; fEnd < exp.GetSize(); fEnd++)
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
        ro.errorObject = curVar;
        TString tempExp(exp);
        UINT addChars = ProcessProcedureCall(statements, cur, exp, line, ro, objVar);

        if (!ro.returnCode)
            fullVarName.Append(tempExp.SubString(0, addChars + 1));
    }
    //else 
    //{
    //    TString varname(exp.SubString(0, end));
    //    bool present;
    //    ro.errorObject = GetVariable(varname, present);
    //    ro.errorMessage.Set(varname);
    //    exp.Set(exp.SubString(end));
    //}

    if (ro.returnCode)
        return false;

    exp.Trim();
    if (!procedureCall)
    {
        ro.errorObject = curVar;
        ro.errorMessage.Set(fullVarName);
    }
    if (exp.StartsWith(L"?.") && !curVar.Get())
    {
        ro.errorObject.Nullify();
        return false;
    }

    if (exp.StartsWith(L"?.") || exp.StartsWith(L"."))
    {
        exp.Delete(0, exp[0] == L'?' ? 2 : 1);
        exp.Trim();
        goto getNextVar;
    }


    return true;
}

UINT TJavaScriptInterpretor::ProcessProcedureCall(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro, TrecPointer<TVariable> objVar)
{
    if (!ro.errorObject.Get())
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"Expected Valid Procedure Object to call, got nul/undefined!");
        return 0;
    }

    if (ro.errorObject->GetVarType() != var_type::interpretor)
    {
        ro.returnCode = ro.improper_type;
        ro.errorMessage.Set(L"Expected Valid Procedure Object to call, got non-procedure object!");
        return 0;
    }

    TDataArray<TrecPointer<TVariable>> vars;

    if (objVar.Get())
        vars.push_back(objVar);

    exp.Trim();
    if (exp.StartsWith(L'('))
        exp.Delete(0, 1);
    int closeParenth = 1;

    WCHAR quote = L'\0';
    UINT Rust = 0;

    TDataArray<TString> expressions;

    for (; Rust < exp.GetSize(); Rust++)
    {
    startFor:
        if (quote && quote == exp[Rust])
            quote = L'\0';
        else
        {
            if (exp[Rust] == L',' && closeParenth == 1)
            {
                expressions.push_back(exp.SubString(0, Rust).GetTrim());
                exp.Delete(0, Rust + 1);
                Rust = 0;
                goto startFor;
            }
            else if (exp[Rust] == L'(')
                closeParenth++;
            else if (exp[Rust] == L')')
                closeParenth--;
            else if (exp[Rust] == L'\'' || exp[Rust] == L'\"' || exp[Rust] == L'`')
                quote = exp[Rust];
        }
        if (!closeParenth)
        {
            break;
        }
    }

    if (closeParenth)
    {
        ro.returnCode = ReportObject::mismatched_parehtnesis;
        ro.errorMessage.Set(L"Mismatched parenthesis when processing expressions for function call");
        return 0;
    }
    expressions.push_back(exp.SubString(0, Rust).GetTrim());
    exp.Delete(0, Rust + 1);

    TrecSubPointer<TVariable, TInterpretor> func = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TInterpretor>(ro.errorObject);

    for (Rust = 0; Rust < expressions.Size(); Rust++)
    {
        ProcessExpression(statements, cur, expressions[Rust], line, ro);
        if (ro.returnCode)
            return Rust+1;

        vars.push_back(ro.errorObject);
    }

    ro = func->Run(vars, false);

    if (!ro.returnCode && ro.mode == report_mode::report_mode_return)
        ro.mode = report_mode::report_mode_regular;

    return Rust + 1;
}

void TJavaScriptInterpretor::ProcessJsonExpression(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro)
{
    TString contents;
    if (!exp.Compare(L'{'))
        file->ReadString(contents, statements[cur].fileEnd - statements[cur].fileStart);
    else
        contents.Set(exp.SubString(1));

    UINT bracketStack = 0;
    TDataArray<TString> sections;
    UINT start = 0;
    WCHAR quote = L'\0';
    for (UINT Rust = 0; Rust < contents.GetSize(); Rust++)
    {
        WCHAR ch = contents[Rust];

        switch (ch)
        {
        case L'(':
        case L'[':
        case L'{':
            bracketStack++;
            break;
        case L')':
        case L']':
        case L'}':
            bracketStack--;
            break;
        case L'`':
        case L'\'':
        case L'\"':
            if (!quote)
                quote = ch;
            else if (quote == ch)
                quote = L'\0';
            break;
        case L',':
            if (!quote && !bracketStack)
            {
                sections.push_back(contents.SubString(start, Rust));
                start = Rust + 1;
            }
        }

        if (Rust == contents.GetSize() - 1 && start < Rust)
        {
            // Last Character
            sections.push_back(contents.SubString(start, Rust));
        }
    }

    TrecSubPointer<TVariable, TContainerVariable> retVar = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    for (UINT Rust = 0; Rust < sections.Size(); Rust++)
    {
        auto pieces = sections[Rust].splitn(L':', 2, 3);

        if (!InspectVariable(pieces->at(0)))
        {
            ro.returnCode = ro.invalid_name;
            return;
        }

        if (pieces->Size() == 2 && pieces->at(1).GetSize())
        {
            ProcessExpression(statements, cur, pieces->at(1), line, ro);
            if (ro.returnCode)
                return;
            retVar->SetValue(pieces->at(0), ro.errorObject);
        }
    }

    ro.errorObject = TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(retVar);
}

bool TJavaScriptInterpretor::InspectVariable(const TString& exp)
{
    bool foundBad = false;

    for (UINT endt = 0; endt < exp.GetSize(); endt++)
    {
        WCHAR letter = exp[endt];

        if ((letter >= L'0' && letter <= L'9') ||
            (letter >= L'a' && letter <= L'z') ||
            (letter >= L'A' && letter <= L'Z') ||
            letter == L'_')
            continue;
        foundBad = true;
        break;
    }

    return !foundBad;
}

void TJavaScriptInterpretor::HandlePreExpr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro)
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
            TrecPointer<TVariable> var = expresions[Rust].value;
            remove = true;
            // To-Do: Add 1 to var
            ro = ProcessAddition(var, one);
            if (ro.returnCode)
                return;
            expresions[Rust].value = ro.errorObject;
        }
        else if (!operators[Rust].Compare(L"--"))
        {
            TrecPointer<TVariable> var = expresions[Rust].value;
            remove = true;
            // To-Do: Subtract 1 from var
            ro = ProcessSubtraction(var, one);
            if (ro.returnCode)
                return;
            expresions[Rust].value = ro.errorObject;

        }

        if (remove)
        {
            if (expresions[Rust].varName.GetSize())
            {
                this->UpdateVariable(expresions[Rust].varName, expresions[Rust].value);
            }
            operators.RemoveAt(Rust--);
        }

    }
}

void TJavaScriptInterpretor::HandleExponents(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro)
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
                ro = this->ProcessExponent(expresions[Rust].value, expresions[Rust + 1].value);
                if (ro.returnCode)
                    return;

                expresions[Rust].value = ro.errorObject;
                expresions.RemoveAt(Rust + 1);
                operators.RemoveAt(Rust--);
            }
        }
    }
}

void TJavaScriptInterpretor::HandleMultDiv(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
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
            errorMessage = ProcessMultiplication(expressions[Rust].value, expressions[Rust + 1].value);
            if (errorMessage.returnCode)
                return;

            expressions[Rust].value = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"/"))
        {
            errorMessage = ProcessDivision(expressions[Rust].value, expressions[Rust + 1].value);
            if (errorMessage.returnCode)
                return;

            expressions[Rust].value = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"%"))
        {
            errorMessage = ProcessModDivision(expressions[Rust].value, expressions[Rust + 1].value);
            if (errorMessage.returnCode)
                return;

            expressions[Rust].value = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }
    }
}

void TJavaScriptInterpretor::HandleAddSub(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
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
            errorMessage = ProcessAddition(expressions[Rust].value, expressions[Rust + 1].value);
            if (errorMessage.returnCode)
                return;

            expressions[Rust].value = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }

        if (!ops[Rust].CompareNoCase(L"-"))
        {
            errorMessage = ProcessSubtraction(expressions[Rust].value, expressions[Rust + 1].value);
            if (errorMessage.returnCode)
                return;

            expressions[Rust].value = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseShift(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
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
            TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(expressions[Rust].value);
        if (canDo = (operand.Get() != nullptr))
        {
            canDo = expressions[Rust + 1].value.Get() != nullptr;
        }
        UINT shift = 0;
        if (canDo)
        {
            if (expressions[Rust + 1].value->GetVarType() == var_type::primitive)
                shift = expressions[Rust + 1].value->Get4Value();
            else if (expressions[Rust + 1].value->GetVarType() == var_type::string)
            {
                int iShift;
                if (expressions[Rust + 1].value->GetString().ConvertToInt(iShift))
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

void TJavaScriptInterpretor::HandleLogicalComparison(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-logical-comparison handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        auto left = expressions[Rust].value;
        auto right = expressions[Rust + 1].value;

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
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(val);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleEquality(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-equality handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool found = false, result;
        auto left = expressions[Rust].value;
        auto right = expressions[Rust + 1].value;
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
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(result);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseAnd(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-bitwise-and handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"&"))
        {
            DoubleLong dl1 = GetValueFromPrimitive( expressions[Rust].value);
            DoubleLong dl2 = GetValueFromPrimitive(expressions[Rust + 1].value);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                ro.returnCode = ro.improper_type;

                return;
            }
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitAnd(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseXor(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-bitwise-xor handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"^"))
        {
            DoubleLong dl1 = GetValueFromPrimitive(expressions[Rust].value);
            DoubleLong dl2 = GetValueFromPrimitive(expressions[Rust + 1].value);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                ro.returnCode = ro.improper_type;

                return;
            }
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitXor(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleBitwiseOr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-bitwise-or handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"|"))
        {
            DoubleLong dl1 = GetValueFromPrimitive(expressions[Rust].value);
            DoubleLong dl2 = GetValueFromPrimitive(expressions[Rust + 1].value);

            if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
            {
                ro.returnCode = ro.improper_type;

                return;
            }
            expressions[Rust].value = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitOr(dl2));
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleLogicalAnd(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-Logical-and handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"&&"))
        {
            expressions[Rust] = IsTruthful(expressions[Rust].value) ? expressions[Rust + 1] : expressions[Rust];
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleLogicalOr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-Logical-or handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"||"))
        {
            expressions[Rust] = IsTruthful(expressions[Rust].value) ? expressions[Rust] : expressions[Rust + 1];
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleNullish(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-Nullish handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"??"))
        {
            expressions[Rust] = expressions[Rust].value.Get() ? expressions[Rust] : expressions[Rust + 1];
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleConditional(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-Conditional handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"?"))
        {
            if (Rust + 1 == ops.Size() || ops[Rust + 1].Compare(L":"))
            {
                ro.returnCode = ro.broken_reference;

                return;
            }


            expressions[Rust] = IsTruthful(expressions[Rust].value) ? expressions[Rust + 1] : expressions[Rust + 2];
            expressions.RemoveAt(Rust + 1);
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust);
            ops.RemoveAt(Rust--);
        }
    }
}

void TJavaScriptInterpretor::HandleAssignment(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-Assignment handler expected one more Expression than operator!");
        return;
    }

    for (int Rust = ops.Size() - 1; Rust > -1; Rust--)
    {
        // Whether or not an assignment operator was found
        bool found = false;

        // Details on whether a bitwise shift assignment operator was found
        bool doBit = false;
        bool doRightBit = false;
        UINT shift = 0;
        USHORT flag = 0;

        auto left = expressions[Rust].value;
        auto right = expressions[Rust + 1].value;

        bool canDo = true;

        TrecSubPointer<TVariable, TPrimitiveVariable> operand =
            TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TPrimitiveVariable>(left);
        if (canDo = (operand.Get() != nullptr))
        {
            canDo = right.Get() != nullptr;
        }
        if (canDo)
        {
            if (right->GetVarType() == var_type::primitive)
                shift = right->Get4Value();
            else if (right->GetVarType() == var_type::string)
            {
                int iShift;
                if (right->GetString().ConvertToInt(iShift))
                    canDo = false;
                else
                    shift = iShift;
            }
        }

        if (!ops[Rust].Compare(L"="))
        {
            found = true;
            ro.errorObject = right;
        }
        else if (!ops[Rust].Compare(L"+="))
        {
            found = true;
            ProcessAddition(left, right);
        }
        else if(!ops[Rust].Compare(L"-="))
        {
            found = true;
            ProcessSubtraction(left, right);
        }
        else if (!ops[Rust].Compare(L"**="))
        {
            found = true;
            ProcessExponent(left, right);
        }
        else if (!ops[Rust].Compare(L"*="))
        {
            ProcessMultiplication(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"/="))
        {
            ProcessDivision(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"%="))
        {
            ProcessModDivision(left, right);
            found = true;
        }
        else if (!ops[Rust].Compare(L"<<="))
        {
            found = true;
            if (!canDo)
            {
                ro.returnCode = ReportObject::broken_reference;

                return;
            }
            doBit = true;
            flag = TPrimitiveVariable::bit_float;
        }
        else if (!ops[Rust].Compare(L">>="))
        {
        found = true;
        if (!canDo)
        {
            ro.returnCode = ReportObject::broken_reference;

            return;
        }
        doBit = doRightBit = true;
        flag = TPrimitiveVariable::bit_float;
        }
        else if (!ops[Rust].Compare(L">>>="))
        {
        found = true;
        if (!canDo)
        {
            ro.returnCode = ReportObject::broken_reference;

            return;
        }
        doBit = doRightBit = true;
        flag = TPrimitiveVariable::bit_float | TPrimitiveVariable::bit_replenish | TPrimitiveVariable::bit_to_32 | TPrimitiveVariable::bit_to_un_f;
        }
        else if (!ops[Rust].Compare(L"&="))
        {
        found = true;
        DoubleLong dl1 = GetValueFromPrimitive(left);
        DoubleLong dl2 = GetValueFromPrimitive(right);

        if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
        {
            ro.returnCode = ro.improper_type;

            return;
        }
        ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitAnd(dl2));
        }
        else if (!ops[Rust].Compare(L"^="))
        {
        found = true;
        DoubleLong dl1 = GetValueFromPrimitive(left);
        DoubleLong dl2 = GetValueFromPrimitive(right);

        if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
        {
            ro.returnCode = ro.improper_type;

            return;
        }
        ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitXor(dl2));
        }
        else if (!ops[Rust].Compare(L"|="))
        {
        found = true;
        DoubleLong dl1 = GetValueFromPrimitive(left);
        DoubleLong dl2 = GetValueFromPrimitive(right);

        if (dl1.type == double_long::dl_invalid || dl2.type == double_long::dl_invalid)
        {
            ro.returnCode = ro.improper_type;

            return;
        }
        ro.errorObject = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>(dl1.GetBitOr(dl2));
        }
        else if (!ops[Rust].Compare(L"&&="))
        {
        found = true;
        ro.errorObject = IsTruthful(left) ? right : left;
        }
        else if (!ops[Rust].Compare(L"||="))
        {
        found = true;
        ro.errorObject = IsTruthful(left) ? left : right;
        }
        else if (!ops[Rust].Compare(L"??="))
        {
        found = true;
        ro.errorObject = left.Get() ? left : right;
        }

        if (found)
        {
            if (!expressions[Rust].varName.GetSize())
            {
                ro.returnCode = ro.broken_reference;
                ro.errorMessage.Format(L"Error! %ws operator must have a named variable as the left hand expression!", ops[Rust].GetConstantBuffer());
                return;
            }
            if (doBit)
            {
                if (!operand->BitShift(doRightBit, shift, flag))
                {
                    ro.returnCode = ReportObject::broken_reference;
                    ro.errorMessage.Set(L"Internal Error Attempting to Perform Bitwise manipulation!");
                    return;
                }
                ro.errorObject = left;
            }

            if (expressions[Rust].varName.CountFinds(L'.'))
            {
                auto holdVal = ro.errorObject;
                int finalDot = expressions[Rust].varName.FindLast(L'.');
                TString initExp(expressions[Rust].varName.SubString(0, finalDot));
                ProcessExpression(statements, cur, initExp, line, ro);

                if (ro.returnCode)
                    return;

                if (!ro.errorObject.Get())
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Set(L"Found Broken reference in Assignment operation!");
                    return;
                }

                if (ro.errorObject->GetVarType() != var_type::collection)
                {
                    ro.returnCode = ro.improper_type;
                    ro.errorMessage.Format(L"Expected collection type for expression `%ws`", expressions[Rust].varName);
                    return;
                }
                
                dynamic_cast<TContainerVariable*>(ro.errorObject.Get())->SetValue(expressions[Rust].varName.SubString(finalDot+1), holdVal);

                ro.errorObject = holdVal;
            }
            else
            {
                switch (UpdateVariable(expressions[Rust].varName, ro.errorObject))
                {
                case 1:
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Format(L"Assignment to non-existant variable '%ws' occured!", expressions[Rust].varName.GetConstantBuffer());
                    return;
                case 2:
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Format(L"Assignment to const variable '%ws' was attempted!", expressions[Rust].varName.GetConstantBuffer());
                    return;
                }
            }
            expressions[Rust].value = ro.errorObject;
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);

        }
    }
}

void TJavaScriptInterpretor::HandleComma(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expressions, TDataArray<TString>& ops, ReportObject& ro)
{
    if (expressions.Size() != ops.Size() + 1)
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"The JS-Conditional handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L","))
        {
            expressions[Rust] = expressions[Rust + 1];
            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
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
    else eqType = true;

    if (isEqual)
    {
        // Dealing with == or ===
        return eqVal && ((castType) ? true : eqType);
    }
    else
    {
        // Dealing with != or !==
        return (castType) ? !eqVal : (!eqVal && !eqType);
    }
}

TrecPointer<TVariable> TJavaScriptInterpretor::GetJsConsole()
{
    TrecPointer<TVariable> console = TrecPointerKey::GetNewTrecPointerAlt<TVariable, TContainerVariable>(ContainerType::ct_json_obj);

    TContainerVariable* containerConsole = dynamic_cast<TContainerVariable*>(console.Get());

    containerConsole->SetValue(L"counter", TrecPointerKey::GetNewTrecPointerAlt<TVariable, TPrimitiveVariable>((UINT)0));

    auto fullSelf = TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self);

    containerConsole->SetValue(L"assert", JsConsole::GetAssert(fullSelf, environment));
    containerConsole->SetValue(L"clear", JsConsole::GetClear(fullSelf, environment));
    containerConsole->SetValue(L"count", JsConsole::GetCount(fullSelf, environment));
    containerConsole->SetValue(L"error", JsConsole::GetError(fullSelf, environment));
    containerConsole->SetValue(L"group", JsConsole::GetGroup(fullSelf, environment));
    containerConsole->SetValue(L"groupEnd", JsConsole::GetGroupEnd(fullSelf, environment));
    containerConsole->SetValue(L"info", JsConsole::GetInfo(fullSelf, environment));
    containerConsole->SetValue(L"log", JsConsole::GetLog(fullSelf, environment));
    containerConsole->SetValue(L"time", JsConsole::GetTime(fullSelf, environment));
    containerConsole->SetValue(L"timeEnd", JsConsole::GetTimeEnd(fullSelf, environment));
    containerConsole->SetValue(L"warn", JsConsole::GetWarn(fullSelf, environment));


    return console;
}

JavaScriptExpression::JavaScriptExpression()
{
}

JavaScriptExpression::JavaScriptExpression(const JavaScriptExpression& orig)
{
    varName.Set(orig.varName);
    value = orig.value;
}

JavaScriptExpression::JavaScriptExpression(const TString& name, TrecPointer<TVariable> value)
{
    varName.Set(name);
    this->value = value;
}


void JavaScriptExpression::operator=(const JavaScriptExpression& orig)
{
    varName.Set(orig.varName);
    value = orig.value;
}