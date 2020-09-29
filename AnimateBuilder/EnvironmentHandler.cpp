#include "EnvironmentHandler.h"

#include <TEnvironmentBuilder.h>
#include <Page.h>
#include <DirectoryInterface.h>
#include <shlobj.h>
#include <TWindow.h>

TString on_SelectRecent(L"OnSelectRecent");
TString on_SelectAvailable(L"OnSelectAvailable");
TString on_SelectWorkspace(L"OnSelectWorkspace");
TString on_ImportProject(L"OnImportProject");
TString on_Confirm(L"OnConfirm");
TString on_FileType(L"OnFileType");


/**
 * Method: EnvironmentHandler::EnvironmentHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
 * Returns: new EnvironmentHandler Object
 */
EnvironmentHandler::EnvironmentHandler(TrecPointer<TInstance> instance) : EventHandler(instance)
{
    envEvents.push_back(&EnvironmentHandler::OnSelectRecent);
    envEvents.push_back(&EnvironmentHandler::OnSelectAvailable);

    envEvents.push_back(&EnvironmentHandler::OnImportProject);
    envEvents.push_back(&EnvironmentHandler::OnSelectWorkspace);

    envEvents.push_back(&EnvironmentHandler::OnConfirm);
    envEvents.push_back(&EnvironmentHandler::OnFileType);

    // Now set the structure to link the listeners to their text name
    eventNameID enid;

    enid.eventID = 0;
    enid.name.Set(on_SelectRecent);
    events.push_back(enid);

    enid.eventID = 1;
    enid.name.Set(on_SelectAvailable);
    events.push_back(enid);

    enid.eventID = 2;
    enid.name.Set(on_ImportProject);
    events.push_back(enid);

    enid.eventID = 3;
    enid.name.Set(on_SelectWorkspace);
    events.push_back(enid);

    enid.eventID = 4;
    enid.name.Set(on_Confirm);
    events.push_back(enid);

    enid.eventID = 5;
    enid.name.Set(on_FileType);
    events.push_back(enid);

    mode = environment_handler_mode::ehm_not_set;
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

    window = page->GetWindowHandle();

    RefreshEnvironmentList();

    auto rootStack = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(page->GetRootControl());

    assert(rootStack.Get());

    grid = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(rootStack->GetLayoutChild(0,1));

    recentBinder = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TDataBind>(grid->GetLayoutChild(0, 1));
    newBinder = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TDataBind>(grid->GetLayoutChild(1, 1));

    assert(recentBinder.Get() && newBinder.Get());

    selectReport = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(rootStack->GetLayoutChild(0, 2));
    assert(selectReport.Get());
    builderReport = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(rootStack->GetLayoutChild(0, 3));
    assert(builderReport.Get());
    environmentReport = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(rootStack->GetLayoutChild(0, 4));
    assert(environmentReport.Get());
    directoryReport = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(rootStack->GetLayoutChild(0, 5));
    assert(directoryReport.Get());

    auto nameGallery = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(rootStack->GetLayoutChild(0, 6));
    assert(nameGallery.Get());

    nameEntry = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(nameGallery->GetLayoutChild(1, 0));

    confirmButton = rootStack->GetLayoutChild(0, 7);
    assert(confirmButton.Get());



    newBinder->setData(&availableEnvironments);

    // Set a default directory for the workspace

    TString directory = GetDirectoryWithSlash(CentralDirectories::cd_User);
    directory.Append(L"Anagame");

    ForgeDirectory(directory);

    currentWorkspace = TFileShell::GetFileInfo(directory);

    RefreshView();
}

/**
 * Method: EnvironmentHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void EnvironmentHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
    int e_id = -1;
    EventArgs ea;
    for (UINT c = 0; c < eventAr.Size(); c++)
    {
        auto tc = eventAr.at(c).control;
        if (!tc.Get())
            continue;
        ea = tc->getEventArgs();
        e_id = ea.methodID;
        // At this point, call the appropriate method
        if (e_id > -1 && e_id < envEvents.Size())
        {
            // call method
            if (envEvents[e_id])
                (this->*envEvents[e_id])(tc, ea);
        }
    }

    //onDraw();
    eventAr.RemoveAll();
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
 * Method: EnvironmentHandler::GetEnvironment
 * Purpose: PRetrieves the environment procured byt the Dialog
 * Parameters: void
 * Returns: TrecPointer<TEnvironment> - the environment selected by the user
 */
TrecPointer<TEnvironment> EnvironmentHandler::GetEnvironment()
{
    return environment;
}

void EnvironmentHandler::OnSelectRecent(TrecPointer<TControl> tc, EventArgs ea)
{
}

void EnvironmentHandler::OnSelectAvailable(TrecPointer<TControl> tc, EventArgs ea)
{
    auto index = ea.arrayLabel;

    if (index >= 0 && index < availableEnvironments.Size())
        selectedEnvType = availableEnvironments[index];

    if (mode == environment_handler_mode::ehm_not_set)
        mode = environment_handler_mode::ehm_available_set;

    RefreshView();
}

void EnvironmentHandler::OnImportProject(TrecPointer<TControl> tc, EventArgs ea)
{
    auto result = BrowseFolder(L"Import Project for Anagame");

    if (result)
    {
        mode = environment_handler_mode::ehm_import_set;
        RefreshEnvironmentList();
    }

    RefreshView();
}

void EnvironmentHandler::OnSelectWorkspace(TrecPointer<TControl> tc, EventArgs ea)
{
    auto result = BrowseFolder(L"Browse Anagame Workspace");

    if (result)
    {
        mode = environment_handler_mode::ehm_available_set;
        RefreshEnvironmentList();
    }

    RefreshView();
}

void EnvironmentHandler::OnConfirm(TrecPointer<TControl> tc, EventArgs ea)
{
    assert(currentWorkspace.Get() && selectedEnvType.Get());

    if (nameEntry.Get() && nameEntry->GetActive())
    {
        TString name(nameEntry->GetText());

        if (name.GetSize())
        {
            TString dir(currentWorkspace->GetPath() + L'\\');
            dir.Append(name);
            ForgeDirectory(dir);
            currentWorkspace = TFileShell::GetFileInfo(dir);
        }
    }


    environment = TEnvironmentBuilder::GetEnvironment(selectedEnvType->GetEnvironment(), selectedEnvType->GetBuilder(), currentWorkspace);

    DestroyWindow(window->GetWindowHandle());
}

void EnvironmentHandler::OnFileType(TrecPointer<TControl> tc, EventArgs ea)
{
    RefreshView();
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

void EnvironmentHandler::RefreshView()
{
    if (selectedEnvType.Get())
    {
        TString param;
        if (builderReport.Get())
        {
            param.Format(L"Environment Builder: %ws", selectedEnvType->GetBuilder().GetConstantBuffer());
            builderReport->SetText(param);
        }
        if (environmentReport.Get())
        {
            param.Format(L"Environment: %ws", selectedEnvType->GetEnvironment().GetConstantBuffer());
            environmentReport->SetText(param);
        }
    }
    else
    {
        if (builderReport.Get())
        {
            builderReport->SetText(L"");
        }
        if (environmentReport.Get())
        {
            environmentReport->SetText(L"");
        }
    }

    if (directoryReport.Get())
    {
        if (currentWorkspace.Get())
            directoryReport->SetText(currentWorkspace->GetPath());
        else
            directoryReport->SetText(L"");
    }
    
    switch (mode)
    {
    case environment_handler_mode::ehm_not_set:
        if (nameEntry.Get())
            nameEntry->setActive(true);
        break;
    case environment_handler_mode::ehm_available_set:
        if (nameEntry.Get())
            nameEntry->setActive(true);
        break;
    case environment_handler_mode::ehm_recent_set:
        if (nameEntry.Get())
            nameEntry->setActive(false);
        break;
    case environment_handler_mode::ehm_import_set:
        if (nameEntry.Get())
            nameEntry->setActive(false);

    }

    if (confirmButton.Get() && nameEntry.Get())
    {
        confirmButton->setActive((!nameEntry->GetActive() || nameEntry->GetText().GetSize()) && selectedEnvType.Get());
    }
}

void EnvironmentHandler::RefreshEnvironmentList()
{
    TMap<TString> environmentMap;
    if (mode == environment_handler_mode::ehm_import_set)
        TEnvironmentBuilder::GetAvailableEnvironments(environmentMap, currentWorkspace);
    else
        TEnvironmentBuilder::GetAvailableEnvironments(environmentMap);

    availableEnvironments.RemoveAll();

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
    if (newBinder.Get() && grid.Get())
    {
        auto rect = grid->getRawSectionLocation(1, 1);
        newBinder->Resize(rect);
    }
}

PIDLIST_ABSOLUTE EnvironmentHandler::BrowseFolder(const TString& message)
{
    BROWSEINFOW info;
    ZeroMemory(&info, sizeof(info));

    WCHAR folderPath[MAX_PATH];

    WCHAR* startPath = (currentWorkspace.Get()) ? currentWorkspace->GetPath().GetBufferCopy() : nullptr;

    info.hwndOwner = (window.Get()) ? window->GetWindowHandle() : 0;
    info.lpszTitle = message.GetConstantBuffer();
    info.pszDisplayName = folderPath;
    info.ulFlags = BIF_NEWDIALOGSTYLE;
    info.lParam = (LPARAM)startPath;

    PIDLIST_ABSOLUTE result = SHBrowseForFolderW(&info);

    if (startPath)
        delete[] startPath;
    startPath = nullptr;
    if (result)
    {
        WCHAR newFolderPath[MAX_PATH];
        SHGetPathFromIDList(result, newFolderPath);


        TString tFolderPath(newFolderPath);
        currentWorkspace = TFileShell::GetFileInfo(tFolderPath);
    }
    return result;
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
