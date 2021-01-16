#include "TJavaScriptClassInterpretor.h"

/**
 * Function: GetJsObjectInstance
 * Purpose: Creates an instance of the JavaScript Object class
 * Parameters: void
 * Returns: TrecPointer<TVariable> - variable representing an instance of the JS Object
 */
TrecPointer<TVariable> GetJsObjectInstance()
{
    auto ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_json_obj);


    return TrecPointerKey::GetTrecPointerFromSub<TVariable, TContainerVariable>(ret);
}



TJavaScriptClassInterpretor::TJavaScriptClassInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env, JsClassBlockType t)
	: TJavaScriptInterpretor(parentInterpretor, env)
{
	blockType = t;
}

void TJavaScriptClassInterpretor::SetClassName(const TString& name)
{
	className.Set(name);
}

bool TJavaScriptClassInterpretor::SetSuperClassName(const TString& name)
{
    bool ret = parent.Get() ? parent->GetClass(name, superData) : false;
    if (ret)
    {
        superName.Set(name);
    }
    return ret;
}

TString TJavaScriptClassInterpretor::GetJsClassName()
{
	return className;
}

void TJavaScriptClassInterpretor::ProcessStatements(ReportObject& ro)
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

        // there must only be one constructor in a JavaScript class
        bool hasConstructor = false;

        // Read the String with the following flags:
        // 0b00000001 - ensures that our terminating character is included in the resulting string
        // 0b00000010 - Tells the method to not terminate if the termination character is within a quote
        // 0b00000100 - Tells the method to watch out for an odd number of backslashes 
        while ((end - file->GetPosition()) ? file->ReadString(code, L"{;", 0b00000111, end - file->GetPosition()) : 0)
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
                    return;
                }


                code.Append(appendable);
                file->ReadString(appendable, L"{;", 0b00000111);

                code.Append(appendable);

                inMultiString = hasOddMiltiLineStrMarkers(code);
            }

            line += code.CountFinds(L'\n');


            TString startStatement(code.GetTrimLeft());

            if (!startStatement.GetSize() || !startStatement.Compare(L';'))
                continue;

            int startParenth = startStatement.Find(L'(');

            if (startStatement.StartsWith(L"constructor", false, true) || startStatement.StartsWith(L"constructor("))
            {
                if (hasConstructor)
                {
                    ro.returnCode = ro.invalid_name;
                    ro.errorMessage.Format(L"Error! Second constructor token found within the %ws class", className.GetConstantBuffer());
                    return;
                }
                hasConstructor = true;
                if (startParenth == -1 || startStatement.SubString(11, startParenth).GetTrim().GetSize())
                {
                    ro.returnCode = ro.broken_reference;
                    ro.errorMessage.Set(L"Unexpected token after 'constructor' keyword!");

                    // To-Do: Add Stack code


                    return;
                }

                ProcessMethod(L"constructor", startStatement, startParenth, ro);
                if (ro.returnCode)
                    return;

                continue;
            }
            UCHAR attributes = 0;
            bool inspectAtts = false;
            do {
                inspectAtts = false;
                if (startStatement.StartsWith(L"static", false, true))
                {
                    attributes |= ATTRIBUTE_STATIC;
                    startStatement.Delete(0, 6);
                    startStatement.Trim();
                    inspectAtts = true;
                }

                if (startStatement.StartsWith(L"const", false, true))
                {
                    attributes |= ATTRIBUTE_CONST;
                    startStatement.Delete(0, 5);
                    startStatement.Trim();
                    inspectAtts = true;
                }

            } while (inspectAtts);

            if (startParenth != -1)
            {
                TString name(startStatement.SubString(0, startParenth).GetTrim());

                if (!name.GetSize())
                {
                    ro.returnCode = ro.invalid_name;
                    ro.errorMessage.Format(L"Error! No name provided to likely method in %ws class!", className.GetConstantBuffer());
                    return;
                }
                if (!InspectVariable(name))
                {
                    ro.returnCode = ro.invalid_name;
                    ro.errorMessage.Format(L"Error! Invalid characters detected in likely method name '%ws' in %ws class!", 
                        name.GetConstantBuffer(), className.GetConstantBuffer());
                    return;
                }

                ProcessMethod(name, startStatement, startParenth, ro, attributes);
            }
            else
            {
                // dealing with a regular variable
                auto pieces = startStatement.splitn(L"=", 2);

                TString name(pieces->at(0).GetTrim());

                if (attributes && !name.GetSize())
                {
                    ro.returnCode = ro.invalid_name;
                    ro.errorMessage.Format(L"Error! Unexpected tokens where variable name was expected in %ws class!", className.GetConstantBuffer());
                    return;
                }

                TrecPointer<TVariable> var;
                if (pieces->Size() == 2)
                {
                    TDataArray<JavaScriptStatement> statements;
                    JavaScriptStatement statement(js_statement_type::js_regular);
                    statement.contents.Set(startStatement);
                    if (startStatement.EndsWith(L"{"))
                    {
                        // We have a block, so prepare for one
                        statement.fileStart = file->GetPosition();
                        statement.fileEnd = GetBlockEnd();
                        if (!statement.lineEnd)
                        {
                            ro.returnCode = ro.incomplete_block;
                            ro.errorMessage.Format(L"Error! Incomplete Block for the %ws class!", className.GetConstantBuffer());
                            return;
                        }
                    }

                    statements.push_back(statement);
                    TString expression(pieces->at(1).GetTrim());
                    UINT cur = 0;
                    ProcessExpression(statements, cur, expression, 0, ro);

                    if (ro.returnCode)
                        return;
                    var = ro.errorObject;
                }

                TClassAttribute constructor;
                int i; 
                constructor.name.Set(name.GetReplace(i, L";", L""));
                constructor.def = var;
                constructor.other = attributes;
                classData.AddAttribute(constructor);
            }
        }
    }
}

TClassStruct TJavaScriptClassInterpretor::GetClassData()
{
    if (superName.GetSize())
        classData.AddParentClass(superName);
    else
        classData.AddParentClass(L"Object");
    return classData;
}

void TJavaScriptClassInterpretor::ProcessMethod(const TString& methodName, TString& startStatement, int startParenth, ReportObject& ro, UCHAR att)
{
    ProcessParenthBlock(ro, startStatement.SubString(startParenth), line);

    if (ro.returnCode)
        return;

    JavaScriptStatement statement(js_statement_type::js_function);

    statement.contents.Set(ro.errorMessage.GetTrim());

    statement.fileStart = file->GetPosition();
    statement.fileEnd = GetBlockEnd();

    if (!statement.fileEnd)
    {
        ro.returnCode = ro.incomplete_block;
        ro.errorMessage.Format(L"Error! Incomplete Block for the %ws class!", className.GetConstantBuffer());
        return;
    }

    if (statement.contents.EndsWith(L'{'))
    {
        statement.contents.Delete(statement.contents.GetSize() - 1);
        statement.contents.Trim();
    }

    if (statement.contents.StartsWith(L'(') && statement.contents.EndsWith(L')'))
        statement.contents.Set(statement.contents.SubString(1, statement.contents.GetSize() - 1));

    auto params = statement.contents.split(L",");
    TDataArray<TString> paramNames;

    // Offer the method 'this', but only if it is not static
    if(!(att & ATTRIBUTE_STATIC))
        paramNames.push_back(L"this");
    for (UINT Rust = 0; Rust < params->Size(); Rust++)
    {
        TString param(params->at(Rust).GetTrim());

        if (!param.GetSize())
            continue;

        if (!InspectVariable(param))
        {
            ro.returnCode = ro.invalid_name;
            ro.errorMessage.Format(L"Improper Paramater name %ws found in constructor or %ws class!", param.GetConstantBuffer(), className.GetConstantBuffer());
            return;
        }

        paramNames.push_back(param);
    }

    TrecPointer<TVariable> superConstructor;
    if (!methodName.Compare(L"constructor") && this->superName.GetSize())
    {
        TClassAttribute att = superData.GetAttributeByName(L"constructor");
        if (!att.name.Compare(L"constructor"))
        {
            superConstructor = att.def;
        }
    }


    auto block = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(
        TrecPointerKey::GetSubPointerFromSoft<TVariable, TInterpretor>(self), environment);
    dynamic_cast<TInterpretor*>(block.Get())->SetCode(file, statement.fileStart, statement.fileEnd);

    block->SetParamNames(paramNames);

    if (dynamic_cast<TJavaScriptInterpretor*>(superConstructor.Get()))
    {
        superConstructor = superConstructor->Clone();
        block->variables.addEntry(L"super", TVariableMarker(false, superConstructor));
        // dynamic_cast<TJavaScriptInterpretor*>(superConstructor.Get())->variables.addEntry(L"this", TVariableMarker(false, ))
    }

    TClassAttribute constructor;
    constructor.name.Set(methodName);
    constructor.def = TrecPointerKey::GetTrecPointerFromSub<TVariable, TJavaScriptInterpretor>(block);
    constructor.other = att;
    classData.AddAttribute(constructor);
}

JsClassBlockType TJavaScriptClassInterpretor::GetClassBlockType()
{
	return blockType;
}
