#include "JavaScriptModifiers.h"

static TDataArray<WCHAR> noSemiColonEnd;

void PrepSemiColonEnders()
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

JSCommentModifier::JSCommentModifier(TrecPointer<TFileShell> inputFile, const TString& outputFile): TFileModifier(inputFile, outputFile)
{


}

TString JSCommentModifier::Modify(const TString& details)
{
    TFile file(inputFile->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);
    assert(file.IsOpen());

    TFile newFile(this->outputFile, TFile::t_file_open_always | TFile::t_file_write);
    assert(newFile.IsOpen()); // Should be true due to the TFileModifier making sure it is available

    file.SeekToBegin();
    newFile.SeekToBegin();

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
                newFile.WriteString(line.SubString(startIndex) + L'\n');
                startIndex = 0;
            }
            else if (IndexComesFirst(quoteIndex, singleIndex) && IndexComesFirst(quoteIndex, multiIndex))
            {
                newFile.WriteString(line.SubString(startIndex, quoteIndex + 1));
                startIndex = quoteIndex + 1;
                mode = 2;
                quoteStack.push_back(line[quoteIndex]);
                goto quoteMode;
            }
            else if (IndexComesFirst(singleIndex, quoteIndex) && IndexComesFirst(singleIndex, multiIndex))
            {
                newFile.WriteString(line.SubString(startIndex, singleIndex) + L'\n');
                startIndex = 0;
            }
            else if (IndexComesFirst(multiIndex, singleIndex) && IndexComesFirst(multiIndex, quoteIndex))
            {
                newFile.WriteString(line.SubString(startIndex, multiIndex));
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
                newFile.Write(L"\n", sizeof(WCHAR));
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
                newFile.WriteString(line.SubString(startIndex) + L'\n');
                startIndex = 0;
            }
            else if (quoteStack.Size() && (line[quoteIndex] == quoteStack[quoteStack.Size() - 1]))
            {
                newFile.WriteString(line.SubString(startIndex, quoteIndex + 1));
                startIndex = quoteIndex + 1;
                mode = 0;
                quoteStack.RemoveAt(quoteStack.Size() - 1);
                goto normalMode;
            }
            else
            {
                // Here, we found a quote mark, but not the one we need to break free
                newFile.WriteString(line.SubString(startIndex, quoteIndex + 1));
                startIndex = quoteIndex + 1;
                goto quoteMode;

            }

        }
    }

    file.Close();
    newFile.Close();

    switch (mode)
    {
    case 0:
        return L"";
    case 3:
        return L"File Ended with an unclosed comment block!";
    case 4:
        return L"File Ended With an unclosed Quote!";
    default:
        return L"Unknown Error!";
    }
}

JSSemiColonModifier::JSSemiColonModifier(TrecPointer<TFileShell> inputFile, const TString& outputFile) : TFileModifier(inputFile, outputFile)
{
    PrepSemiColonEnders();
}

TString JSSemiColonModifier::Modify(const TString& details)
{
    TFile file(inputFile->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);
    assert(file.IsOpen());

    TFile newFile(this->outputFile, TFile::t_file_open_always | TFile::t_file_write);
    assert(newFile.IsOpen()); // Should be true due to the TFileModifier making sure it is available

    file.SeekToBegin();
    newFile.SeekToBegin();

    TString dLines[2];

    TString readLine;

    if (file.ReadString(readLine))
        dLines[0].Set(readLine);
    else
        return L"No Content!";

    UINT empty = 0;

    bool inMultiString = hasOddMiltiLineStrMarkers(readLine);

    while (file.ReadString(readLine))
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
                    return L"Missing Statement between 'if' and 'else' statement!";

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


    file.Close();
    newFile.Close();
	return TString();
}

bool JSSemiColonModifier::hasOddMiltiLineStrMarkers(const TString& str)
{
    if (!str.GetSize())
        return false;
    UINT count = str[0] == L'`' ? 1 : 0;

    int find = 0;

    while ((find = str.FindOutOfQuotes(L"`", find + 1)) != -1)
    {
        count++;
    }


    return count % 2 == 1;
}