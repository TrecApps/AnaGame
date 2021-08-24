#include "WebEnvironment.h"
#include <TDialog.h>
#include <TNativeInterpretor.h>

/**
 * Function: JsWebAlert
 * Purpose: Implements the 'alert()' function in JS
 * Parameters: TDataArray<TrecPointer<TVariable>>& params - list of variables
 *               TrecPointer<TEnvironment> env - the Environment it is running in
 *               ReportObject& ret - return information
 * Returns: void
 */
void JsWebAlert(TDataArray<TrecPointer<TVariable>>& params, TrecPointer<TEnvironment> env, ReportObject& ret)
{
    TrecSubPointer<TEnvironment, WebEnvironment> webEnv = TrecPointerKey::GetTrecSubPointerFromTrec<TEnvironment, WebEnvironment>(env);
    if (!webEnv.Get())
    {
        ret.returnCode = ReportObject::broken_reference;
        ret.errorMessage.Set(L"Js Alert function lacks reference to environment in which it runs!");
        return;
    }

    TString caption(params.Size() && params[0].Get() ? params[0]->GetString() : TString(L"void"));

    ActivateAlertDialog(webEnv->GetInstance(), webEnv->GetWindow()->GetWindowHandle(), caption);
}


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

    mainJavaScript = TrecPointerKey::GetNewTrecSubPointer<TVariable, TcJavaScriptInterpretor>(TrecSubPointer<TVariable, TcInterpretor>(),TrecPointerKey::GetTrecPointerFromSoft<TEnvironment>(self));
    mainAnaScript = TrecPointerKey::GetNewTrecSubPointer<TVariable, TAnascriptInterpretor>(TrecSubPointer<TVariable, TInterpretor>(), TrecPointerKey::GetTrecPointerFromSoft<TEnvironment>(self));

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

TrecPointer<TWindow> WebEnvironment::GetWindow()
{
    return window;
}

TrecPointer<TInstance> WebEnvironment::GetInstance()
{
    return instance;
}

TrecPointer<TVariable> WebEnvironment::GetVariable(TString& var, bool& present, env_var_type evtType)
{
    present = false;
    if (evtType == env_var_type::evt_interpretor)
    {
        if (var.Compare(L"JavaScript"))
        {
            present = true;
            return TrecPointerKey::GetTrecPointerFromSub<>(mainJavaScript);
        }
    }
    return TEnvironment::GetVariable(var, present, evtType);
}


void WebEnvironment::SetResources(TrecPointer<TInstance> instance, TrecPointer<TWindow> window)
{
    assert(instance.Get() && window.Get());
    this->instance = instance;
    this->window = window;
}

WebEnvGenerator::WebEnvGenerator(TrecPointer<TInstance> i, TrecPointer<TWindow> w)
{
    assert(i.Get() && w.Get());
    this->window = w;
    this->instance = i;
}

TrecPointer<TEnvironment> WebEnvGenerator::GetEnvironment(TrecPointer<TFileShell> shell)
{
    auto ret = TrecPointerKey::GetNewSelfTrecSubPointer<TEnvironment, WebEnvironment>(shell);
    ret->SetResources(instance, window);

    auto regRet = TrecPointerKey::GetTrecPointerFromSub<TEnvironment, WebEnvironment>(ret);

    // Now Add Basic Functions to Environment for Web Access
    ret->AddVariable(L"alert", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TNativeInterpretor>(JsWebAlert, TrecSubPointer<TVariable, TInterpretor>(), regRet)); // alert() function


    // Time to return

    return regRet;
}
