#include "TAnascriptInterpretor.h"
#include <DirectoryInterface.h>
#include <TContainerVariable.h>
#include <TStringVariable.h>
#include <TPrimitiveVariable.h>

/**
 * Method: TAnascriptInterpretor::TAnascriptInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TAnascriptInterpretor Object
 */
TAnascriptInterpretor::TAnascriptInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env): TInterpretor(parentInterpretor, env)
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
    while (file->ReadString(code, end - file->GetPosition()))
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
            ret = ProcessFunction(code, line);
        }
        else if (!Keyword.Find(L"ext. "))
        {
            // Set up an environment to lurk in

        }
        else if (!Keyword.Find(L"return "))
        {
            // Prepare to return an expression to the Caller
            ProcessExpression(code, line, ret);
            ret.mode = report_mode::report_mode_return;
            return ret;
        }
        else if (!Keyword.Find(L"let "))
        {
            // Prepare to Create a new variable
            ret = ProcessLet(code, line);

            if (ret.returnCode)
                return ret;
        }
        else if (!Keyword.Find(L"loop "))
        {
            // Scan for the end and prepare to loop
            ret = ProcessLoop(code, line);
            if (ret.returnCode)
                return ret;
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
            ret = ProcessIf(code, line);
            if (ret.returnCode)
                return ret;
        }
        else if (!Keyword.Find(L"break"))
        {
            ret.mode = report_mode::report_mode_break;
            return ret;
        }
        else if (!Keyword.Find(L"continue"))
        {
            ret.mode = report_mode::report_mode_continue;
            return ret;
        }



        line++;
    }


	return ret;
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

/**
 * Method: TAnascriptInterpretor::ProcessLoop
 * Purpose: Processes the loop block
 * Parameters: TString& let - the 'let' statement to inspect and process
 *              UINT line - the line number being called upon,
 * Returns: ReportObject - objct indicating the success of the program or falure information
 */
ReportObject TAnascriptInterpretor::ProcessLoop(TString& loop, UINT line)
{
    // Get the starting location for the child interpreter
    ULONGLONG startNext = file->GetPosition();

    ULONGLONG blockEnd = startNext;

    UINT blockStack = 1;

    TString lineStr;

    while (blockStack && file->ReadString(lineStr))
    {
        lineStr.SetLower();
        auto tokens = lineStr.split(L" ");

        if (tokens->Size())
        {
            TString tok(tokens->at(0));

            if (!tok.Compare(L"end"))
                blockStack--;
            else if (!tok.Compare(L"in") || !tok.Compare(L"if") || !tok.Compare(L"loop") || !tok.Compare(L"while"))
                blockStack++;
        }

        if (!blockStack)
            break;
        blockEnd = file->GetPosition();
    }

    ULONGLONG setFilePosition = file->GetPosition();

    // Make sure loop is complete
    ReportObject ret;
    if (blockStack)
    {
        ret.returnCode = ReportObject::incomplete_block;
        ret.errorMessage.Set(L"Incomplete Loop Block Detected!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }

    // Extra Parameter indicates that we don't want to split up any string literals if they are detected
    auto loopTokens = loop.split(L" ", 0b00000010);

    int keyThrough = -1;
    int keyWith = -1;
    int keyElement = -1;
    int keyName = -1;
    
    for (UINT Rust = 0; loopTokens->Size(); Rust++)
    {
        if (loopTokens->at(Rust).CompareNoCase(L"through"))
            keyThrough = Rust;
        else if (loopTokens->at(Rust).CompareNoCase(L"with"))
            keyWith = Rust;
        else if (loopTokens->at(Rust).CompareNoCase(L"element"))
            keyElement = Rust;
        else if (loopTokens->at(Rust).CompareNoCase(L"name"))
            keyName = Rust;
    }

    if(keyThrough != 1)
    {
        ret.returnCode = ReportObject::incomplete_block;
        ret.errorMessage.Set(L"Expected keyword 'through' in loop declaration!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }

    if (keyWith > 2 && (((keyElement == keyWith + 1) && (keyName == keyElement + 1)) || (keyName == keyWith + 1)))
    {
        TString exp(loopTokens->at(2));

        for (UINT Rust = 3; Rust < keyWith; Rust++)
        {
            exp.AppendFormat(L" %ws", loopTokens->at(Rust));
        }

        TrecPointer<TVariable> variable = variables.retrieveEntry(exp);

        if (!variable.Get())
        {
            // Okay, try getting it through an expression
            ProcessExpression(exp, line, ret);

            if (ret.returnCode)
                return ret;

            variable = ret.errorObject;
        }

        if (!variable.Get())
        {
            ret.returnCode = ReportObject::broken_reference;
            ret.errorMessage.Set(L"Failed to retrieve Variable for processing through a loop!");

            TString stack;
            stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
            ret.stackTrace.push_back(stack);

            return ret;
        }

        if (variable->GetVarType() != var_type::collection)
        {
            ret.returnCode = ReportObject::improper_type;
            ret.errorMessage.Set(L"Variable for the loop needs to be a collection!");

            TString stack;
            stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
            ret.stackTrace.push_back(stack);

            return ret;
        }

        // Okay, we have a valid collection, now prepare to go through it.

        if (loopTokens->Size() != 2 + keyName)
        {
            ret.returnCode = ReportObject::too_few_params;
            ret.errorMessage.Set(L"Needed Variable name for Loop!");

            TString stack;
            stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
            ret.stackTrace.push_back(stack);

            return ret;
        }

        TString varName(loopTokens->at(loopTokens->Size() - 1));

        CheckVarName(varName, ret, line);

        if (ret.returnCode)
            return ret;

        // Retrievs our collection to parse through
        auto collVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(variable);


        // We have determined that the loop line checks out, now to create an interpretor to process the block itself
        TrecSubPointer<TVariable, TAnascriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAnascriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

        // Before configuring new interpretor, set up the param name
        TDataArray<TString> paramNames;
        paramNames.push_back(varName);

        
        // Now Configure the interpreter
        block->SetParamNames(paramNames);
        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, startNext, blockEnd);


        TDataArray<TrecPointer<TVariable>> params;
        for (UINT Rust = 0; Rust < collVar->GetSize(); Rust++)
        {
            params.RemoveAll();
            params.push_back(collVar->GetValueAt(Rust));

            ret = block->Run(params);

            if (ret.returnCode)
                return ret;
        }

        // We have completed the loop --> Now set the file and return to caller
        file->Seek(setFilePosition, 0);
        return ret;
    }

    ret.returnCode = ReportObject::incomplete_block;
    ret.errorMessage.Set(L"Loop needs to be of format 'loop through [collection-exp] with element name [name]' or 'loop through [collection-exp] with element name [name]'!");
    TString stack;
    stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
    ret.stackTrace.push_back(stack);

    return ret;
}

/**
 * Method: TAnascriptInterpretor::ProcessIf
 * Purpose: Processes the if block
 * Parameters: TString& _if - the 'let' statement to inspect and process
 *              UINT line - the line number being called upon,
 * Returns: ReportObject - objct indicating the success of the program or failure information
 */
ReportObject TAnascriptInterpretor::ProcessIf(TString& _if, UINT line)
{
    // Get the starting location for the child interpreter
    ULONGLONG startNext = file->GetPosition();

    ULONGLONG blockEnd = startNext;

    UINT blockStack = 1;

    TString lineStr;

    bool ended = false;
    TrecPointer<TDataArray<TString>> elseTokens;
    TString elseString;

    UINT newLine = line;

    while (blockStack && file->ReadString(lineStr))
    {
        newLine++;
        TString lineStringLower = lineStr.GetLower();
        auto tokens = lineStringLower.split(L" ");

        if (tokens->Size())
        {
            TString tok(tokens->at(0));

            if (!tok.Compare(L"end"))
            {
                blockStack--;
                ended = true;
            }
            else if (!tok.Compare(L"else") && !(blockStack - 1))
            {
                blockStack--;
                ended = false;

                elseTokens = tokens;
                elseString.Set(lineStr);
            }
            else if (!tok.Compare(L"in") || !tok.Compare(L"if") || !tok.Compare(L"loop") || !tok.Compare(L"while"))
                blockStack++;
        }

        if (!blockStack)
            break;
        blockEnd = file->GetPosition();
    }

    ULONGLONG setFilePosition = file->GetPosition();

    // Make sure loop is complete
    ReportObject ret;
    if (blockStack)
    {
        ret.returnCode = ReportObject::incomplete_block;
        ret.errorMessage.Set(L"Incomplete Loop Block Detected!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }
    TString expression;
    if (!_if.GetLower().Find(L"if "))
        expression.Set(_if.SubString(3));
    else
        expression.Set(_if);

    ProcessExpression(expression, line, ret);

    if (ret.returnCode)
    {
        return ret;
    }

    if (IsTruthful(ret.errorObject))
    {
        // We have determined that the loop line checks out, now to create an interpretor to process the block itself
        TrecSubPointer<TVariable, TAnascriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAnascriptInterpretor>(
            TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

        dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, startNext, blockEnd);


        TDataArray<TrecPointer<TVariable>> params;
        ret = block->Run(params);

        return ret;
    }
    else if (!ended)
    {
        if (elseTokens->Size() > 1)
        {
            elseString.Delete(0, 5);
            return ProcessIf(elseString, newLine);
        }
    }
    ret.returnCode = 0;
    

    file->Seek(setFilePosition, 0);
    return ret;
}

/**
 * Method: TAnascriptInterpretor::ProcessWhile
 * Purpose: Processes the While block
 * Parameters: TString& let - the 'let' statement to inspect and process
 *              UINT line - the line number being called upon,
 * Returns: ReportObject - objct indicating the success of the program or failure information
 */
ReportObject TAnascriptInterpretor::ProcessWhile(TString& _while, UINT line)
{
    // Get the starting location for the child interpreter
    ULONGLONG startNext = file->GetPosition();

    ULONGLONG blockEnd = startNext;

    UINT blockStack = 1;

    TString lineStr;


    UINT newLine = line;

    while (blockStack && file->ReadString(lineStr))
    {
        newLine++;
        TString lineStringLower = lineStr.GetLower();
        auto tokens = lineStringLower.split(L" ");

        if (tokens->Size())
        {
            TString tok(tokens->at(0));

            if (!tok.Compare(L"end"))
            {
                blockStack--;
            }

            else if (!tok.Compare(L"in") || !tok.Compare(L"if") || !tok.Compare(L"loop") || !tok.Compare(L"while"))
                blockStack++;
        }

        if (!blockStack)
            break;
        blockEnd = file->GetPosition();
    }

    ULONGLONG setFilePosition = file->GetPosition();

    // Make sure loop is complete
    ReportObject ret;
    if (blockStack)
    {
        ret.returnCode = ReportObject::incomplete_block;
        ret.errorMessage.Set(L"Incomplete Loop Block Detected!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }
    TString expression;
    if (!_while.GetLower().Find(L"while "))
        expression.Set(_while.SubString(6));
    else
        expression.Set(_while);


    ProcessExpression(expression, line, ret);

    // We have determined that the loop line checks out, now to create an interpretor to process the block itself
    TrecSubPointer<TVariable, TAnascriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAnascriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, startNext, blockEnd);


    TDataArray<TrecPointer<TVariable>> params;
    

    while (!ret.returnCode && ret.errorObject.Get() && IsTruthful(ret.errorObject))
    {
        ret = block->Run(params);

        if (ret.returnCode)
            return ret;

        ProcessExpression(expression, line, ret);
    }

    file->Seek(setFilePosition, 0);

    return ret;
}

/**
 * Method: TAnascriptInterpretor::ProcessFunction
 * Purpose: Processes the creation of an Anascript function
 * Parameters: TString& fun - the start of the function declaration to inspect and process
 *              UINT line - the line number being called upon,
 * Returns: ReportObject - objct indicating the success of the program or failure information
 */
ReportObject TAnascriptInterpretor::ProcessFunction(TString& fun, UINT line)
{
    // Get the starting location for the child interpreter
    ULONGLONG startNext = file->GetPosition();

    ULONGLONG blockEnd = startNext;

    UINT blockStack = 1;

    TString lineStr;

    bool ended = false;
    TrecPointer<TDataArray<TString>> elseTokens;
    TString elseString;

    UINT newLine = line;

    while (blockStack && file->ReadString(lineStr))
    {
        newLine++;
        TString lineStringLower = lineStr.GetLower();
        auto tokens = lineStringLower.split(L" ");

        if (tokens->Size())
        {
            TString tok(tokens->at(0));

            if (!tok.Compare(L"end"))
            {
                blockStack--;
                ended = true;
            }
            else if (!tok.Compare(L"else") && !(blockStack - 1))
            {
                blockStack--;
                ended = false;

                elseTokens = tokens;
                elseString.Set(lineStr);
            }
            else if (!tok.Compare(L"in") || !tok.Compare(L"if") || !tok.Compare(L"loop") || !tok.Compare(L"while"))
                blockStack++;
        }

        if (!blockStack)
            break;
        blockEnd = file->GetPosition();
    }

    ULONGLONG setFilePosition = file->GetPosition();

    // Make sure loop is complete
    ReportObject ret;
    if (blockStack)
    {
        ret.returnCode = ReportObject::incomplete_block;
        ret.errorMessage.Set(L"Incomplete Loop Block Detected!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }

    auto tokens = fun.split(L" ", 0b00000010);

    if (tokens->Size() < 4)
    {
        ret.returnCode = ReportObject::incomplete_statement;
        ret.errorMessage.Set(L"Incomplete function declaration!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }

    TString functionName(tokens->at(1));

    CheckVarName(functionName, ret, line);
    if (ret.returnCode)
        return ret;

    if (tokens->at(2).CompareNoCase(L"takes"))
    {
        ret.returnCode = ReportObject::invalid_name;
        ret.errorMessage.Set(L"Expected keyword 'takes' at Function Declaration!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }

    if (tokens->at(3).Find(L'['))
    {
        ret.returnCode = ReportObject::invalid_name;
        ret.errorMessage.Set(L"Expected token '[' in front of the parameter list!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }

    tokens->at(3).Remove(L'[');

    if (tokens->at(tokens->Size() - 1).Find(L']'))
    {
        ret.returnCode = ReportObject::invalid_name;
        ret.errorMessage.Set(L"Expected token ']' at the end of the parameter list!");

        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ret.stackTrace.push_back(stack);

        return ret;
    }

    tokens->at(tokens->Size() - 1).Remove(L']');

    TDataArray<TString> names;

    for (UINT Rust = 3; Rust < tokens->Size(); Rust++)
    {
        auto commaSplit = tokens->at(Rust).split(L",");

        for (UINT C = 0; C < commaSplit->Size(); C++)
        {
            TString newName(commaSplit->at(C));
            CheckVarName(newName, ret, line);

            if (ret.returnCode)
                return ret;
            names.push_back(newName);
        }
    }

    // Create the interpretor
    TrecSubPointer<TVariable, TAnascriptInterpretor> block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAnascriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);

    dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, startNext, blockEnd);

    block->SetParamNames(names);

    variables.addEntry(functionName, TrecPointerKey::GetTrecPointerFromSub<TVariable, TAnascriptInterpretor>(block));

    ret.returnCode = 0;
    return ret;
}

/**
 * Method: TAnascriptInterpretor::ProcessExpression
 * Purpose: Processes the let command
 * Parameters: TString& exp - the 'let' statement to inspect and process
 *              UINT line - the line number being called upon,
 *              ReportObject& ro - objct indicating the success of the program or falure information
 * Returns: void
 */
void TAnascriptInterpretor::ProcessExpression(TString& let, UINT line, ReportObject& ro)
{
    TString exp(let.GetTrim());


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

    while (exp.GetSize())
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
                ro.errorMessage.Format(L"Mismatched Parenthesis, needed {} more to close it!", stack);
                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
                ro.stackTrace.push_back(stack);

                return;
            }

            TString smallExp(exp.SubString(1, end));

            ProcessExpression(smallExp, line, ro);

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
                ro.errorMessage.Format(L"Mismatched Square Brackets, needed {} more to close it!", stack);
                TString stack;
                stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
                ro.stackTrace.push_back(stack);

                return;
            }

            TString smallExp(exp.SubString(1, end));

            ProcessArrayExpression(smallExp, line, ro);

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
            InspectVariable(exp, line, ro);
            //InspectNumber(exp, line, ro);
            if (ro.returnCode)
                return;
            expresions.push_back(ro.errorObject);
        }
        else if (exp[0] == L'\'' || exp[0] == L'\"')
        {
            int loc = exp.Find(exp[0], 0, false);

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

        if (exp.StartsWith(L"equals ", true))
        {
            operators.push_back(exp.SubString(0, 7));
            exp.Delete(0, 7);
        }
        else if (exp.StartsWith(L"nand ", true) || exp.StartsWith(L"xnor ", true) || exp.StartsWith(L"band ", true))
        {
            operators.push_back(exp.SubString(0, 5));
            exp.Delete(0, 5);
        }
        else if (exp.StartsWith(L"and ", true) || exp.StartsWith(L"xor ", true) || exp.StartsWith(L"nor ", true) || exp.StartsWith(L"bor ", true))
        {
            operators.push_back(exp.SubString(0, 4));
            exp.Delete(0, 4);
        }
        else if (exp.StartsWith(L"or ", true) )
        {
            operators.push_back(exp.SubString(0, 3));
            exp.Delete(0, 3);
        }
        else if (exp.StartsWith(L"&&") || exp.StartsWith(L"||") || exp.StartsWith(L"==") || exp.StartsWith(L"!=") || exp.StartsWith(L">=") || exp.StartsWith(L"<="))
        {
            operators.push_back(exp.SubString(0, 2));
            exp.Delete(0, 2);
        }
        else if (!exp.FindOneOf(L"+-*/%^&|<>"))
        {
            operators.push_back(TString(exp[0]));
            exp.Delete(0, 1);
        }

        // Trim so that if we have only whitespace, we can break out of the loop
        exp.Trim();

    }

    if (operators.Size() != expresions.Size() + 1)
    {
        ro.returnCode = ro.incomplete_statement;
        ro.errorMessage.Set(L"Expression should have one more subexpressions");
        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ro.stackTrace.push_back(stack);

        return;
    }

    HandleMultDiv(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleAddSub(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleCompare(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleEquality(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleBitAnd(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleBitXorNxor(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleBitOrNor(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleAnd(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleOr(expresions, operators, ro);
    if (ro.returnCode)return;

    HandleAssign(expresions, operators, ro);
    


    if (ro.returnCode)return;

    if (expresions.Size())
        ro.errorObject = expresions.at(0);
    else
        ro.errorObject = TrecPointer<TVariable>();
}


/**
 * Method: TAnascriptInterpretor::ProcessExpression
 * Purpose: Processes the let command
 * Parameters: TString& exp - the 'let' statement to inspect and process
 *              UINT line - the line number being called upon,
 *              ReportObject& ro - objct indicating the success of the program or failure information
 * Returns: void
 */
void TAnascriptInterpretor::ProcessArrayExpression(TString& exp, UINT line, ReportObject& ro)
{
    auto pieces = exp.split(L",", 0b00000010);

    TrecSubPointer<TVariable, TContainerVariable> array_ = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);

    

    for (UINT Rust = 0; Rust < pieces->Size(); Rust++)
    {
        TString piece(pieces->at(Rust));

        ProcessExpression(piece, line, ro);

        if (ro.returnCode)
        {
            return;
        }

        array_->AppendValue(ro.errorObject);
    }

    ro.errorObject = TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(array_);

}

/**
 * Method: TAnascriptInterpretor::ProcessProcedureCall
 * Purpose: Processes the Call to a function or method
 * Parameters: TString& exp - the expression to analyze statement to inspect and process
 *              UINT line - the line number being called upon,
 *              ReportObject& ro - objct indicating the success of the program or falure information
 * Returns: void
 */
void TAnascriptInterpretor::ProcessProcedureCall(TString& exp, UINT line, ReportObject& ro)
{
    int openPar = exp.Find(L'(');
    if (openPar == -1)
    {
        ro.returnCode = ReportObject::broken_reference;
        ro.errorMessage.Set("Procedure string exprected a '(' to signify call!");

        return;
    }

    TString start(exp.SubString(0, openPar));

    UINT indexDotStart = 0;
    UINT indexDot;

    TString var;
    TrecPointer<TVariable> object;
    TrecPointer<TVariable> call;

    while ((indexDot = start.Find(L".", indexDotStart)) != -1)
    {
        var.Set(start.SubString(indexDotStart, indexDot));
        var.Trim();

        if (object.Get())
        {
            if (object->GetVarType() == var_type::collection)
            {
                bool pres;
                object = dynamic_cast<TContainerVariable*>(object.Get())->GetValue(var, pres);

                if (!object.Get())
                {
                    ro.returnCode = ro.broken_reference;
                    if (pres)
                        ro.errorMessage.Format(L"Encountered Null object %ws during attempted method call!", var.GetConstantBuffer());
                    else 
                        ro.errorMessage.Format(L"Encountered undefined object %ws during attempted method call!", var.GetConstantBuffer());
                    return;
                }
            }
            else
            {
                ro.returnCode = ro.broken_reference;
                
                ro.errorMessage.Format(L"Encountered non-extendable variable %ws in an attempt to get to final Object!", var.GetConstantBuffer());
                return;
            }
        }
        else
        {
            object = variables.retrieveEntry(var);
            if (!object.Get())
            {
                ro.returnCode = ro.broken_reference;
                ro.errorMessage.Format(L"Encountered Null or undefined variable %ws while preparing method call!", var.GetConstantBuffer());
                return;
            }
        }

        indexDotStart = indexDot + 1;
    }

    TString procedureName(start.SubString(indexDotStart).GetTrim());
    bool pres = true;
    call = (object.Get() && object->GetVarType() == var_type::collection) ? dynamic_cast<TContainerVariable*>(object.Get())->GetValue(procedureName, pres) :
        variables.retrieveEntry(procedureName);

    if (!call.Get())
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Format(L"%ws call %ws was found to be %ws!",
            (object.Get()) ? L"Method" : L"Function",
            procedureName.GetConstantBuffer(),
            (pres) ? L"null": L"undefined");
        return;
    }

    // To-Do: When Get Type is added to TObject, add code to adjust the attempts to get code

    if (call->GetVarType() != var_type::interpretor)
    {
        ro.returnCode = ro.improper_type;
        ro.errorMessage.Format(L"Varible type %ws needs to be of type Interpretor so that Anascript can call it!", procedureName.GetConstantBuffer());
        return;
    }


    TString params(exp.SubString(openPar + 1));
    ProcessProcedureCall(object, call, params,line, ro);
}

void TAnascriptInterpretor::ProcessProcedureCall(TrecPointer<TVariable> object, TrecPointer<TVariable> call, TString& exp, UINT line, ReportObject& ro)
{
    if (!call.Get())
    {
        ro.returnCode = ro.broken_reference;
        ro.errorMessage.Set(L"Ended up with broken reference to Procedure call!");
        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ro.stackTrace.push_back(stack);
        return;
    }
    if (call->GetVarType() != var_type::interpretor)
    {
        ro.returnCode = ro.improper_type;
        ro.errorMessage.Set(L"Expected Procedure call to be of type 'interpretor'!");
        TString stack;
        stack.Format(L"At %ws (line: %i)", file->GetFileName().GetConstantBuffer(), line);
        ro.stackTrace.push_back(stack);
        return;
    }

    auto params = exp.split(L",", 2);

    TDataArray<TrecPointer<TVariable>> parameters;
    if (object.Get())
        parameters.push_back(object);

    for (UINT c = 0; c < params->Size(); c++)
    {
        TString param(params->at(c).GetTrim());
        
        ProcessExpression(param, line, ro);
        if (ro.returnCode)
        {
            ro.errorMessage.AppendFormat(L" during %ws Call!", (object.Get()) ? L"Method" : L"Function");
            return;
        }

        parameters.push_back(ro.errorObject);
    }

    ro = dynamic_cast<TInterpretor*>(call.Get())->Run(parameters);


}

/**
 * Method: TAnascriptInterpretor::IsTruthful
 * Purpose: Determines whether an expression is truthful or not
 * Parameters: TrecPointer<TVariable> var - the variable to analyze.
 * Returns: bool
 */
bool TAnascriptInterpretor::IsTruthful(TrecPointer<TVariable> var)
{
    if (!var.Get())
        return false;

    switch (var->GetVarType())
    {
    case var_type::collection:
        return var->GetSize();
    case var_type::primitive:
        return var->Get8Value();
    case var_type::string:
        return var->GetString().GetSize() && var->GetString().GetTrim().Compare(L"false");
    }





    return true;
}


/**
 * Method: TAnascriptInterpretor::InspectNumber
 * Purpose: Retrieves a number if valid input is provided
 * Parameters: TString& exp - the expression to analyze statement to inspect and process
 *              UINT line - the line number being called upon,
 *              ReportObject& ro - objct indicating the success of the program or falure information
 * Returns: void
 */
void TAnascriptInterpretor::InspectNumber(TString& exp, UINT line, ReportObject& ro)
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
                letter == L'.' ||  letter == L'_')
                continue;
            break;
        }
    }

    tExp.Set(tExp.SubString(start, end));

    if (tExp.Find(L'.'))
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

/**
 * Method: TAnascriptInterpretor::InspectVariable
 * Purpose: Retrieves a variable if valid
 * Parameters: TString& exp - the expression to analyze statement to inspect and process
 *              UINT line - the line number being called upon,
 *              ReportObject& ro - objct indicating the success of the program or falure information
 * Returns: void
 */
void TAnascriptInterpretor::InspectVariable(TString& exp, UINT line, ReportObject& ro)
{
    TString tExp(exp.GetTrimRight());

    UINT frontDifference = exp.GetSize() - tExp.GetSize();

    _ASSERT(tExp.GetSize());

    UINT start = 0, end;

    for (end = 0; end < tExp.GetSize(); end++)
    {
        WCHAR letter = tExp[end];

        if ((letter >= L'0' && letter <= L'9') ||
            (letter >= L'a' && letter <= L'f') ||
            (letter >= L'A' && letter <= L'F') ||
            letter == L'_')
            continue;
        break;
    }

    TString varname(tExp.SubString(0, end));
    bool present;
    ro.errorObject = GetVariable(varname, present);

    exp.Set(exp.SubString(frontDifference + end));
}

/**
 * Method: TAnascriptInterpretor::HandleMultDiv
 * Purpose: Checks list of expressions and operators and sees if multiplication, division, or mod division needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleMultDiv(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The Multiplication Division handler expected one more Expression than operator!");
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

            expressions.RemoveAt(Rust+1);
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

/**
 * Method: TAnascriptInterpretor::HandleAddSub
 * Purpose: Checks list of expressions and operators and sees if addition or subtraction needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleAddSub(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The addition/subtraction handler expected one more Expression than operator!");
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

/**
 * Method: TAnascriptInterpretor::HandleCompare
 * Purpose: Checks list of expressions and operators and sees if >/>=/</<= needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleCompare(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The comparison handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool found = false;

        if (!ops[Rust].CompareNoCase(L">="))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for >= comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1 >= v2);

            found = true;
        }

        if (!ops[Rust].CompareNoCase(L">"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for > comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1 > v2);

            found = true;
        }

        if (!ops[Rust].CompareNoCase(L"<="))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for <= comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1 <= v2);

            found = true;
        }

        if (!ops[Rust].CompareNoCase(L"<"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for < comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1 < v2);

            found = true;
        }

        if (found)
        {
            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
            continue;
        }
    }
}

/**
 * Method: TAnascriptInterpretor::HandleEquality
 * Purpose: Checks list of expressions and operators and sees if equality needs to be analyzed
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleEquality(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The Equality handler expected one more Expression than operator!");
        return;
    }
    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool found = false;

        if (!ops[Rust].CompareNoCase(L"==") || !ops[Rust].CompareNoCase(L"equals"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for == comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1 == v2);

            found = true;
        }

        if (!ops[Rust].CompareNoCase(L"!="))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for != comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(!(v1 == v2));

            found = true;
        }
        if (found)
        {
            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}


/**
 * Method: TAnascriptInterpretor::HandleBitAnd
 * Purpose: Checks list of expressions and operators and sees if bitwise AND needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleBitAnd(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The Multiplication Division handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"&") || !ops[Rust].CompareNoCase(L"band"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for & comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1.GetBitAnd(v2));

            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }

    }
}

/**
 * Method: TAnascriptInterpretor::HandleBitXorNxor
 * Purpose: Checks list of expressions and operators and sees if Bitwise XOR, NXOR needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleBitXorNxor(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The xor/xnor handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool found = false;
        if (!ops[Rust].Compare(L"^") || !ops[Rust].CompareNoCase(L"xor"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for ^ comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1.GetBitXor(v2));

            found = true;
        }
        if (!ops[Rust].CompareNoCase(L"xnor"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for xnor comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1.GetBitXor(v2) ^ 0xFFFFFFFFFFFFFFFF);

            found = true;
        }

        if (found)
        {
            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }

    }
}

/**
 * Method: TAnascriptInterpretor::HandleBitOrNor
 * Purpose: Checks list of expressions and operators and sees if bitwise OR or NOR needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleBitOrNor(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The or/nor handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        bool found = false;
        if (!ops[Rust].Compare(L"|") || !ops[Rust].CompareNoCase(L"bor"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for | comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1.GetBitOr(v2));

            found = true;
        }
        if (!ops[Rust].CompareNoCase(L"bnor"))
        {
            DoubleLong v1 = GetValueFromPrimitive(expressions[Rust]), v2 = GetValueFromPrimitive(expressions[Rust + 1]);

            if (v1.type == double_long::dl_invalid || v2.type == double_long::dl_invalid)
            {
                errorMessage.returnCode = errorMessage.improper_type;
                errorMessage.errorMessage.Set(L"Improper type detected for bnor comparison!");
                return;
            }

            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(v1.GetBitOr(v2) ^ 0xFFFFFFFFFFFFFFFF);

            found = true;
        }

        if (found)
        {
            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }

    }
}

/**
 * Method: TAnascriptInterpretor::HandleAnd
 * Purpose: Checks list of expressions and operators and sees if Logical AND needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleAnd(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The 'and' handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"&&") || !ops[Rust].CompareNoCase(L"and"))
        {
            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(IsTruthful(expressions[Rust]) && IsTruthful(expressions[Rust]));
            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

/**
 * Method: TAnascriptInterpretor::HandleOr
 * Purpose: Checks list of expressions and operators and sees if bitwise OR or NOR needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleOr(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The 'or' handler expected one more Expression than operator!");
        return;
    }

    for (UINT Rust = 0; Rust < ops.Size(); Rust++)
    {
        if (!ops[Rust].Compare(L"||") || !ops[Rust].CompareNoCase(L"or"))
        {
            errorMessage.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(IsTruthful(expressions[Rust]) || IsTruthful(expressions[Rust]));
            expressions[Rust] = errorMessage.errorObject;

            expressions.RemoveAt(Rust + 1);
            ops.RemoveAt(Rust--);
        }
    }
}

/**
 * Method: TAnascriptInterpretor::HandleAssign
 * Purpose: Checks list of expressions and operators and sees if an assignment needs to be done
 * Parameters: TDataArray<TrecPointer<TVariable>>& expressions - list of expressions to process
 *              TDataArray<TString>& ops - list of operators
 *              ReportObject& errorMessage - lets caller know if there was an error detected
 * Returns: void
 */
void TAnascriptInterpretor::HandleAssign(TDataArray<TrecPointer<TVariable>>& expressions, TDataArray<TString>& ops, ReportObject& errorMessage)
{
    if (expressions.Size() != ops.Size() - 1)
    {
        errorMessage.returnCode = errorMessage.broken_reference;
        errorMessage.errorMessage.Set(L"The assignment handler expected one more Expression than operator!");
        return;
    }


}

