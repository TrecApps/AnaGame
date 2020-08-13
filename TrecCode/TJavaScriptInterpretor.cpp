#include "TJavaScriptInterpretor.h"
#include <DirectoryInterface.h>

/**
 * Method: TJavaScriptInterpretor::TJavaScriptInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TInterpretor Object
 */
TJavaScriptInterpretor::TJavaScriptInterpretor(TrecSubPointer<TVariable,TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env): TInterpretor(parentInterpretor, env)
{

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
        return 0;
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
