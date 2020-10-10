#include "WebEnvironment.h"

/**
 * Method: WebEnvironment::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString WebEnvironment::GetType()
{
    return TString(L"WebEnvironment;") + TEnvironment::GetType();
}

WebEnvironment::WebEnvironment(TrecPointer<TFileShell> shell): TEnvironment(shell)
{
}

WebEnvironment::WebEnvironment(TrecSubPointer<TControl, TPromptControl> prompt):TEnvironment(prompt)
{
}

UINT WebEnvironment::SetUpEnv()
{
    // Remove the Interpretors that currently exist
    mainJavaScript.Delete();
    mainAnaScript.Delete();

    mainJavaScript = TrecPointerKey::GetNewTrecSubPointer<TVariable, TJavaScriptInterpretor>(self);
    mainAnaScript = TrecPointerKey::GetNewTrecSubPointer<TVariable, TAnascriptInterpretor>(self);

    return 0;
}

void WebEnvironment::Compile()
{
}

void WebEnvironment::Compile(TrecPointer<TFile> logFile)
{
}

void WebEnvironment::Log()
{
}

void WebEnvironment::Run()
{
}

void WebEnvironment::Run(TrecPointer<TFileShell> shell)
{
}

UINT WebEnvironment::RunTask(TString& task)
{
    return 0;
}

TrecPointer<TObjectNode> WebEnvironment::GetBrowsingNode()
{
    return TrecPointer<TObjectNode>();
}

bool WebEnvironment::SupportsFileExt(const TString& ext)
{
    return false;
}

void WebEnvironment::SupportsFileExt(TDataArray<TString>& ext)
{
    ext.RemoveAll();

    ext.push_back(L"html");
    ext.push_back(L"css");
    ext.push_back(L"tml");

    ext.push_back(L"js");
    ext.push_back(L"ascrpt");

    // To-Do: Add support to image files
    // ext.push_back(L"");
    // ext.push_back(L"");
}
