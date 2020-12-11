#include "TWebWindow.h"
#include <DirectoryInterface.h>

TString TWebWindow::GetType()
{
    return TString(L"TWebWindow;") + TWindow::GetType();
}

TWebWindow::TWebWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, 
    TrecPointer<TInstance> ins, UINT mainViewSpace, UINT pageBarSpace) :
    TWindow(name, winClass, style | WS_MAXIMIZE, parent, commandShow, ins)
{
    this->mainViewSpace = mainViewSpace;
    this->pageBarSpace = pageBarSpace;
}

TWebWindow::~TWebWindow()
{
}

int TWebWindow::PrepareWindow()
{
    if (TWindow::PrepareWindow())
        return 1;

    RECT location = { 0,0,0,0 };
    GetClientRect(currentWindow, &location);

    mainPageSpace.left = location.left;
    mainPageSpace.right = location.right;
    mainPageSpace.top = location.top;
    mainPageSpace.bottom = mainPageSpace.top + mainViewSpace;

    tabs.left = mainPageSpace.left;
    tabs.right = mainPageSpace.right;
    tabs.top = mainPageSpace.bottom;
    tabs.bottom = tabs.top + pageBarSpace;

    webPage.left = tabs.left;
    webPage.right = tabs.right;
    webPage.top = tabs.bottom;
    webPage.bottom = location.bottom;

    return 0;
}

void TWebWindow::OnWindowResize(UINT width, UINT height)
{
    size.right = width;
    size.bottom = height;
    size.left = size.top = 0;
    mainPageSpace.left = size.left;
    mainPageSpace.right = size.right;
    mainPageSpace.top = size.top;
    mainPageSpace.bottom = mainPageSpace.top + mainViewSpace;

    tabs.left = mainPageSpace.left;
    tabs.right = mainPageSpace.right;
    tabs.top = mainPageSpace.bottom;
    tabs.bottom = tabs.top + pageBarSpace;

    webPage.left = tabs.left;
    webPage.right = tabs.right;
    webPage.top = tabs.bottom;
    webPage.bottom = size.bottom;
    if (mainPage.Get())
        mainPage->OnResize(mainPageSpace, 0, d3dEngine);

}

void TWebWindow::AddNewTab()
{
    AddNewTab(L"Anagame://NewTab");
}

void TWebWindow::AddNewTab(const TString& url)
{
    TString fixedUrl(FixUrl(url));

    TrecSubPointer<Page, WebPage> newWebPage = GetWebPage(fixedUrl);

    

    if (newWebPage.Get())
    {
        D2D1_RECT_F initLoc;
        initLoc.bottom = tabs.bottom;
        initLoc.top = tabs.top;
        initLoc.right = tabs.right;

        if (tabList.Size())
        {
            initLoc.left = tabList[tabList.Size() - 1]->GetLocation().right;
        }
        else
            initLoc.left = tabs.left;

        TrecPointer<WebPageHolder> newHolder = TrecPointerKey::GetNewTrecPointer<WebPageHolder>(
            L"Placeholder",
            this->drawingBoard,
            pageBarSpace,
            newWebPage->GetHandler(),
            TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self),
            initLoc);

        newHolder->SetPage(TrecPointerKey::GetTrecPointerFromSub<Page, WebPage>(newWebPage));
        tabList.push_back(newHolder);

        newWebPage->tab = newHolder;
    }
}

int TWebWindow::CompileView(TString& file, TrecPointer<EventHandler> eh)
{
    if (!windowInstance.Get())
        return -1;
    if (!currentWindow)
        return -2;

    TrecPointer<TFile> aFile = TrecPointerKey::GetNewTrecPointer<TFile>(file, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

    if (!aFile.Get() || !aFile->IsOpen())
        return 1;

    assert(windowInstance.Get());
    mainPage = Page::GetWindowPage(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), eh);

    if (!mainPage.Get())
        return 2;

    mainPage->SetArea(mainPageSpace);

    mainPage->SetAnaface(aFile, eh);

    mainPage->PrepAnimations(animationCentral);

    animationCentral.StartBegin();
    animationCentral.StartNewPersistant();
    safeToDraw = 1;
    Draw();

    return 0;
}

void TWebWindow::SetEnvironmentGenerator(TrecPointer<EnvironmentGenerator> gen)
{
    envGenerator = gen;
}

TString TWebWindow::FixUrl(const TString& url)
{
    if(url.StartsWith(L"Anagame://") || url.StartsWith(L"File://") || url.StartsWith(L"http://") || url.StartsWith(L"https://") || url.StartsWith(L"ftp://"))
        return TString(url);

    TrecPointer<TFileShell> file = TFileShell::GetFileInfo(url);

    if (file.Get())
        return TString(L"File://") + url;
    return TString(L"https://") + url;
    
}

TrecSubPointer<Page, WebPage> TWebWindow::GetWebPage(const TString& url)
{
    auto ret = TrecPointerKey::GetNewSelfTrecSubPointer<Page, WebPage>(drawingBoard, self);

    // ret->SetEnvironment(envGenerator->GetEnvironment(TrecPointer<TFileShell>()));

    if (url.StartsWith(L"Anagame://"))
    {
        TString aFile(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\Web-Tours\\");

        TString feature(url.SubString(10));

        TString target(aFile + feature + L"\\index.tml");

        TrecPointer<TFileShell> index = TFileShell::GetFileInfo(target);

        if (!index.Get())
        {
            target.Set(aFile + L"error\\index.tml");
            index = TFileShell::GetFileInfo(target);
            if (!index.Get())
            {
                MessageBox(currentWindow, L"Failed to Set up Error Message for Anagame Page!", nullptr, MB_OK);
                return ret;
            }
        }

        // To-Do: Set up Web-Handler


        TrecPointer<TFile> file = TrecPointerKey::GetNewTrecPointer<TFile>(index->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);

        ret->SetEnvironment(envGenerator->GetEnvironment(TFileShell::GetFileInfo(file->GetFileDirectory())));

        ret->SetAnaface(file, TrecPointer < EventHandler>());

    }
    else if (url.StartsWith(L"File://"))
    {
        TString target(url.SubString(7));

        TrecPointer<TFileShell> index = TFileShell::GetFileInfo(target);

        if (!index.Get())
        {
            target.Set(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\Web-Tours\\error\\index.tml");
            index = TFileShell::GetFileInfo(target);
            if (!index.Get())
            {
                MessageBox(currentWindow, L"Failed to Set up Error Message for Anagame Page!", nullptr, MB_OK);
                return ret;
            }
        }

        // To-Do: Set up Web-Handler


        TrecPointer<TFile> file = TrecPointerKey::GetNewTrecPointer<TFile>(index->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);
        ret->SetEnvironment(envGenerator->GetEnvironment(TFileShell::GetFileInfo(file->GetFileDirectory())));

        ret->SetAnaface(file, TrecPointer < EventHandler>());
    }
    else if (url.StartsWith(L"http://"))
    {

    }
    else if (url.StartsWith(L"https://"))
    {

    }
    else if (url.StartsWith(L"ftp://"))
    {

    }
    return ret;
}
