#include "EnvironmentHandler.h"

#include <TEnvironmentBuilder.h>
#include <Page.h>

/**
 * Method: EnvironmentHandler::EnvironmentHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
 * Returns: new EnvironmentHandler Object
 */
EnvironmentHandler::EnvironmentHandler(TrecPointer<TInstance> instance) : EventHandler(instance)
{
}

/**
 * Method: EnvironmentHandler::~EnvironmentHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
EnvironmentHandler::~EnvironmentHandler()
{
}

/**
 * Method: EnvironmentHandler::Initialize
 * Purpose: Environment Handler initialization
 * Parameters: TrecPointer<Page> page - the page the Handler is to associate with
 * Returns: void
 */
void EnvironmentHandler::Initialize(TrecPointer<Page> page)
{
    if (!page.Get() || !page->GetRootControl().Get())
        return;

    TMap<TString> environmentMap;
    TEnvironmentBuilder::GetAvailableEnvironments(environmentMap);

    for (UINT Rust = 0; Rust < environmentMap.count(); Rust++)
    {
        auto entry = environmentMap.GetEntryAt(Rust);

        if (!entry.Get() || !entry->object.Get())
            continue;

        auto environmentList = entry->object->split(L';');

        for (UINT C = 0; C < environmentList->Size(); C++)
        {
            availableEnvironments.push_back(TrecPointerKey::GetNewTrecPointer<EnvironmentList>(environmentList->at(C), entry->key));
        }

        
    }

    auto rootStack = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(page->GetRootControl());

    assert(rootStack.Get());

    auto grid = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(rootStack->GetLayoutChild(0,1));

    recentBinder = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TDataBind>(grid->GetLayoutChild(0, 1));
    newBinder = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TDataBind>(grid->GetLayoutChild(1, 1));

    assert(recentBinder.Get() && newBinder.Get());

    newBinder->setData(&availableEnvironments);
}

/**
 * Method: EnvironmentHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void EnvironmentHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

/**
 * Method: EnvironmentHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 */
void EnvironmentHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

/**
 * Method: EnvironmentHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 */
bool EnvironmentHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
    return false;
}

EnvironmentList::EnvironmentList(const TString& environment, const TString& builder)
{
    this->environment.Set(environment);
    this->environmentBuilder.Set(builder);
}

EnvironmentList::EnvironmentList(const EnvironmentList& list)
{
    this->environment.Set(list.GetEnvironment());
    this->environmentBuilder.Set(list.GetBuilder());
}

EnvironmentList::EnvironmentList()
{
}

TString EnvironmentList::getVariableValueStr(const TString& varName)
{
    if (!varName.Compare(L"Environment"))
        return environment;
    else if (!varName.Compare(L"Builder"))
        return environmentBuilder;
    return TString();
}

TString EnvironmentList::GetEnvironment()const
{
    return environment;
}

TString EnvironmentList::GetBuilder() const
{
    return environmentBuilder;
}
