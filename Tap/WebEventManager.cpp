#include "WebEventManager.h"

WebEventManager::WebEventManager()
{
    runnable = false;
    runCount = 0;
}

void WebEventManager::SetEvents(R_Message_Type type, TDataArray<TPage::EventID_Cred>& events)
{
    this->events = events;
    runnable = true;
    runCount = 0;

    // To-Do: Correct Event types

    //switch (type)
    //{
    //    
    //}
}

bool WebEventManager::Run(TString& expression, TrecPointer<TPage>& page)
{
    if (runnable && runCount < events.Size())
    {
        page = events[runCount].control;
        expression.Set(events[runCount++].expression);
        return true;
    }
    return false;
}

bool WebEventManager::GetVariable(const TString& name, TrecPointer<TVariable>& var)
{
    return false;
}

bool WebEventManager::SetVariable(const TString& name, TrecPointer<TVariable> var)
{
    return false;
}
