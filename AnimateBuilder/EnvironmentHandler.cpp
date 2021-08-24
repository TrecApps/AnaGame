#include "EnvironmentHandler.h"

#include <TEnvironmentBuilder.h>
#include <Page.h>
#include <DirectoryInterface.h>
#include <shlobj.h>
#include <TWindow.h>
#include <TML_Reader_.h>

TString on_SelectRecent(L"OnSelectRecent");
TString on_SelectAvailable(L"OnSelectAvailable");
TString on_SelectWorkspace(L"OnSelectWorkspace");
TString on_ImportProject(L"OnImportProject");
TString on_Confirm(L"OnConfirm");
TString on_FileType(L"OnFileType");

/**
 * Function: CollectSavedEnv
 * Purpose: Collects a List of saved Environments
 * Parameters: TTrecPointerArray<SavedEnvironment>& envList - list of environments to populate
 * Returns: void
 */
void CollectSavedEnv(TTrecPointerArray<SavedEnvironment>& envList)
{
    TString repoName = GetDirectoryWithSlash(CentralDirectories::cd_AppData) + L"AnaGame\\";

    ForgeDirectory(repoName);
    repoName.Append(L"Envronments.tml");

    TrecPointer<TFile> repoFile = TrecPointerKey::GetNewTrecPointer<TFile>(repoName, TFile::t_file_open_existing | TFile::t_file_read);

    TDataArray<EnvironmentEntry> envEntries;

    if (repoFile->IsOpen())
    {
        TrecPointer<Parser_> parser = TrecPointerKey::GetNewTrecPointerAlt<Parser_, EnvironmentEntryParser>();
        TML_Reader_ reader(repoFile, parser);
        int num = 0;
        if (reader.read(&num))
        {
            dynamic_cast<EnvironmentEntryParser*>(parser.Get())->GetEntries(envEntries);
        }

        repoFile->Close();
    }

    for (UINT Rust = 0; Rust < envEntries.Size(); Rust++)
    {
        TrecPointer<SavedEnvironment> envEnt = TrecPointerKey::GetNewTrecPointer<SavedEnvironment>(envEntries[Rust].type, envEntries[Rust].source,
            envEntries[Rust].name, TFileShell::GetFileInfo(envEntries[Rust].filePath));

        if (!envEnt->GetFileLocation().Get())
            continue;

        envList.push_back(envEnt);
    }


}

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
    savedEnvironments.RemoveAll();
    CollectSavedEnv(savedEnvironments);

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
    recentBinder->setData(&savedEnvironments);

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
    auto index = ea.arrayLabel;

    if (index >= 0 && index < savedEnvironments.Size())
    {
        selectedEnvType = TrecPointerKey::GetNewTrecPointer<EnvironmentList>(savedEnvironments[index]->GetEnvironment(), savedEnvironments[index]->GetBuilder());
        nameEntry->SetText(savedEnvironments[index]->GetName());
        envFile = savedEnvironments[index]->GetFileLocation();
        if (envFile.Get())
            currentWorkspace = envFile->GetParent();
    }
    mode = environment_handler_mode::ehm_recent_set;

    RefreshView();
}

void EnvironmentHandler::OnSelectAvailable(TrecPointer<TControl> tc, EventArgs ea)
{
    auto index = ea.arrayLabel;

    if (index >= 0 && index < availableEnvironments.Size())
        selectedEnvType = availableEnvironments[index];

    if (mode != environment_handler_mode::ehm_available_set)
    {
        nameEntry->SetText(L"");
    }
    envFile.Nullify();
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

        if (selectReport.Get())
        {
            selectReport->SetText(L"Import Project");
        }
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

        if (selectReport.Get())
        {
            selectReport->SetText(L"New Environment");
        }
    }

    RefreshView();
}

void EnvironmentHandler::OnConfirm(TrecPointer<TControl> tc, EventArgs ea)
{
    assert(currentWorkspace.Get() && selectedEnvType.Get());

    if (nameEntry.Get() && nameEntry->GetActive())
    {
        TString name(nameEntry->GetText());

        if (name.GetSize() && mode == environment_handler_mode::ehm_available_set)
        {
            TString dir(currentWorkspace->GetPath() + L'\\');
            dir.Append(name);
            ForgeDirectory(dir);
            currentWorkspace = TFileShell::GetFileInfo(dir);
        }
    }


    environment = TEnvironmentBuilder::GetEnvironment(selectedEnvType->GetEnvironment(), selectedEnvType->GetBuilder(), currentWorkspace);

    assert(environment.Get());
    if (envFile.Get())
        assert(!environment->SetLoadFile(envFile).GetSize());

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
            auto builderBuffer = selectedEnvType->GetBuilder();
            param.Format(L"Environment Builder: %ws", builderBuffer.GetConstantBuffer().getBuffer());
            builderReport->SetText(param);
        }
        if (environmentReport.Get())
        {
            auto builderBuffer = selectedEnvType->GetEnvironment();
            param.Format(L"Environment: %ws", builderBuffer.GetConstantBuffer().getBuffer());
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
            nameEntry->UnlockText();
        break;
    case environment_handler_mode::ehm_available_set:
        if (nameEntry.Get())
            nameEntry->UnlockText();
        break;
    case environment_handler_mode::ehm_recent_set:
        if (nameEntry.Get())
            nameEntry->LockText();
        break;
    case environment_handler_mode::ehm_import_set:
        if (nameEntry.Get())
            nameEntry->LockText();

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
    info.lpszTitle = message.GetConstantBuffer().getBuffer();
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

SavedEnvironment::SavedEnvironment(const TString& environment, const TString& builder, const TString& name, TrecPointer<TFileShell> fileLoc) : EnvironmentList(environment, builder)
{
    this->fileLoc = fileLoc;
    this->name.Set(name);
}

SavedEnvironment::SavedEnvironment(const SavedEnvironment& list)
{
    this->environment.Set(list.GetEnvironment());
    this->environmentBuilder.Set(list.GetBuilder());
    this->fileLoc = list.fileLoc;
    this->name.Set(list.name);
}

SavedEnvironment::SavedEnvironment()
{
}

TString SavedEnvironment::getVariableValueStr(const TString& varName)
{
    if (!varName.Compare(L"Location"))
        return fileLoc.Get() ? fileLoc->GetPath() : L"[Null File]";
    if (!varName.Compare(L"Name"))
        return name;
    return EnvironmentList::getVariableValueStr(varName);
}

TrecPointer<TFileShell> SavedEnvironment::GetFileLocation()
{
    return fileLoc;
}

TString SavedEnvironment::GetName()
{
    return name;
}
