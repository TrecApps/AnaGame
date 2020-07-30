#include "TAnascriptInterpretor.h"
#include <DirectoryInterface.h>

/**
 * Method: TAnascriptInterpretor::TAnascriptInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TAnascriptInterpretor Object
 */
TAnascriptInterpretor::TAnascriptInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor): TInterpretor(parentInterpretor)
{
}


/**
 * Method: TAnascriptInterpretor::SetCode
 * Purpose: Sets the initial file of the code
 * Parameters: TFile& the file to read
 * Returns: UINT - error code
 *
 * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
 */
UINT TAnascriptInterpretor::SetCode(TFile& file)
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
    int startIndex = 0;


    while (file.ReadString(line))
    {
        switch (mode)
        {
        normalMode:
        case 0: // normal Mode

            quoteIndex = line.FindOneOf(L"\"'`", startIndex);
            singleIndex = line.Find(L"#", startIndex);
            if (quoteIndex == -1 && singleIndex == -1)
            {
                newFile->WriteString(line.SubString(startIndex) + L'\n');
                startIndex = 0;
            }
            else if (quoteIndex<singleIndex)
            {
                newFile->WriteString(line.SubString(startIndex, quoteIndex + 1));
                startIndex = quoteIndex + 1;
                mode = 2;
                quoteStack.push_back(line[quoteIndex]);
                goto quoteMode;
            }
            else if (singleIndex< quoteIndex)
            {
                newFile->WriteString(line.SubString(startIndex, singleIndex) + L'\n');
                startIndex = 0;
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


	return 4;
}


/**
 * Method: TAnascriptInterpretor::Run
 * Purpose: Runs the interpretor
 * Parameters: void
 * Returns: ReportObject - objct indicating the success of the program or falure information
 *
 * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
 */
ReportObject TAnascriptInterpretor::Run()
{
    ReportObject ret;
    if (!file.Get() || !file->IsOpen())
    {
        ret.returnCode = ReportObject::broken_reference;
        ret.errorMessage.Set(L"Error! No access to the JavaScript file!");
        return ret;
    }

    file->Seek(start, FILE_BEGIN);

    TString code;

    UINT line = 1;
    while (file->ReadString(code))
    {
        // Trim unecessary white-space
        code.Trim();

        // If no code, proceed to the next line
        if (code.IsEmpty())
        {
            line++;
            continue;
        }
        TString Keyword(code.GetLower());

        if (!Keyword.Find(L"function. "))
        {
            // We have a Function Declaration

        }
        else if (!Keyword.Find(L"ext. "))
        {
            // Set up an environment to lurk in

        }
        else if (!Keyword.Find(L"return "))
        {
            // Prepare to return an expression to the Caller

        }
        else if (!Keyword.Find(L"let "))
        {
            // Prepare to Create a new variable

        }
        else if (!Keyword.Find(L"loop "))
        {
            // Scan for the end and prepare to loop

        }
        else if (!Keyword.Find(L"print "))
        {
            // Print some message to the command line

        }
        else if (!Keyword.Find(L"in "))
        {
            // Prepare to declare the scope name of a block

        }
        else if (!Keyword.Find(L"if "))
        {
            // Prepare to start an if block without a name

        }



        line++;
    }


	return ReportObject();
}


/**
 * Method: TAnascriptInterpretor::Run
 * Purpose: Runs the interpretor with the following parameters
 * Parameters: TDataArray<TVariable> params - list of parameters to provide
 * Returns: ReportObject - objct indicating the success of the program or falure information
 *
 * Note: this method is intended to be called in interpretors that represent specific methods or functions
 */
ReportObject TAnascriptInterpretor::Run(TDataArray<TrecPointer<TVariable>>& params)
{
	return ReportObject();
}

ReportObject TAnascriptInterpretor::ProcessLet(TString& let,UINT line)
{
    ReportObject ret;

    auto tokens = let.split(L" ");

    TString varname;

    TString expression;

    // First check for the presence of the '=' operator
    if (tokens->Size() < 2)
    {
        ret.returnCode = ReportObject::incomplete_statement;
        ret.errorMessage.Set(L"'let' statement needs more than the let keyword!");
        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);


        return ret;
    }


    TString firstToken(tokens->at(1));

    UINT expIndex = 0;

    if (int eq = firstToken.Find(L'=') != -1)
    {
        if (eq != firstToken.GetSize() - 1)
        {
            expression.Set(firstToken.SubString(eq + 1) + L" ");
        }
        expIndex = 2;

        varname.Set(firstToken.SubString(0, eq));

        // Check the name
        CheckVarName(varname, ret, line);

        if (ret.returnCode)
            return ret;
    }
    else
    {
        varname.Set(firstToken);

        // Check the name
        CheckVarName(varname, ret, line);

        if (ret.returnCode)
            return ret;

        if (tokens->Size() < 3)
        {
            // We have a name but no value being assigned to it. Not an error, but need to
            //  a. Check the authenticity of the name
            //  b. Make sure there isn't already a variable in our current scope.


            // Now check to see if the Variable already exists
            auto existVar = variables.retrieveEntry(varname);

            if (existVar.Get())
            {
                ret.returnCode = ReportObject::existing_var;
                ret.errorMessage.Format(L"Variable with name '%ws' already exists in the current scope!", varname.GetConstantBuffer());
                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
                ret.stackTrace.push_back(stack);

                return ret;
            }

            // Perform the operation
            variables.addEntry(varname, TrecPointer<TVariable>());

            ret.returnCode = ReportObject::no_error;
            ret.errorMessage.Empty();

            return ret;
        }

        // Okay, so we are still expecting an expression

        if (!tokens->at(2).Compare(L"=") || !tokens->at(2).CompareNoCase(L"be"))
        {
            // Code currently looks like this:
            //      "let [tok1] = ..." or "let [tok1] be ..."

            expIndex = 3;
        }
        else if (tokens->at(2).Find(L"=") > 0)
        {
            // Error, code looks something like this: "let [tok1] [tok2]=..."

            ret.returnCode = ReportObject::invalid_name;
            ret.errorMessage.Set(L"Cannot have two tokens between 'let' and '='!");
            TString stack;
            stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
            ret.stackTrace.push_back(stack);

            return ret;
        }
        else if (tokens->at(2)[0] == L'=')
        {
            // Still valid, code looks like this: "let [tok1] =[tok2] ..."

            expression.Set(firstToken.SubString(1) + L" ");
            expIndex = 3;
        }
        else
        {
            // Error, code looks something like this: "let [tok1] [tok2]"

            ret.returnCode = ReportObject::invalid_name;
            ret.errorMessage.Set(L"Cannot have two tokens after 'let' and unless the second is '=' or 'be'!");
            TString stack;
            stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
            ret.stackTrace.push_back(stack);

            return ret;
        }
    }

    for (; expIndex < tokens->Size(); expIndex++)
    {
        expression.Append(tokens->at(expIndex) + L' ');
    }

    expression.Trim();

    ProcessExpression(expression, line, ret);

    if (!ret.returnCode)
        variables.addEntry(varname, ret.errorObject);

    return ret;
}

void TAnascriptInterpretor::ProcessExpression(TString& let, UINT line, ReportObject& ro)
{
}
