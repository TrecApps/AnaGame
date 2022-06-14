#include "TWebWindow.h"
#include <DirectoryInterface.h>
#include "EventHandler.h"

TString TWebWindow::GetType()
{
    return TString(L"TWebWindow;") + TWindow::GetType();
}

TWebWindow::TWebWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, 
    TrecPointer<TProcess> ins, UINT mainViewSpace, UINT pageBarSpace) :
    TWindow(name, winClass, style | WS_MAXIMIZE, parent, commandShow, ins)
{
    this->mainViewSpace = mainViewSpace;
    this->pageBarSpace = pageBarSpace;
    mainPageSpace = tabs = webPage = { 0.0f,0.0f,0.0f,0.0f };
}

TWebWindow::~TWebWindow()
{
}

int TWebWindow::PrepareWindow()
{
    ThreadLock();
    if (TWindow::PrepareWindow())
    {
        ThreadRelease();
        return 1;
    }
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

    webPages = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, TabBar>(drawingBoard);
    // Set the attributes of this Tab Bar

    //webPages->addAttribute(L"|HaveAddTab", TrecPointerKey::GetNewTrecPointer<TString>(L"true"));
    //webPages->onCreate(tabs, d3dEngine);

    // AddNewTab();

    ThreadRelease();
    return 0;
}

void TWebWindow::OnWindowResize(UINT width, UINT height)
{
    ThreadLock();
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


    TDataArray<TPage::EventID_Cred> cred;
    

    if (mainPage.Get())
        mainPage->OnResize(mainPageSpace, 0, cred);

    if (d3dEngine.Get())
    {
        d3dEngine->Resize(width, height);
    }
    if (drawingBoard.Get())
    {
        drawingBoard->Resize(currentWindow, size, d3dEngine);
    }
    ThreadRelease();
}

void TWebWindow::AddNewTab()
{
    AddNewTab(L"Anagame://NewTab");
}

void TWebWindow::AddNewTab(const TString& url, bool createTab)
{
    ThreadLock();
    TString fixedUrl(FixUrl(url));

    TrecSubPointer<TPage, WebPage> newWebPage = GetWebPage(fixedUrl);

    if (newWebPage.Get())
    {
        TDataArray<TPage::EventID_Cred> cred;
        
        TString title(newWebPage->GetTitle());
        if (!title.GetSize())
            title.Set(L"Untitled Tab");
        webPages->AddNewTab(title, TrecSubToTrec(newWebPage), true);

        currentPage = newWebPage;
    }
    ThreadRelease();
}

void TWebWindow::SetTabBar(TrecSubPointer<TPage, TabBar> tempTabs)
{
    if (tempTabs.Get())
        this->webPages = tempTabs;
}

int TWebWindow::CompileView(TString& file, TrecPointer<TPage::EventHandler> eh)
{
    ThreadLock();
    if (!windowInstance.Get())
    {
        ThreadRelease();
        return -1;
    }
    if (!currentWindow)
    {
        ThreadRelease();
        return -2;
    }
    TrecPointer<TFile> aFile = TrecPointerKey::GetNewTrecPointer<TFile>(file, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

    if (!aFile.Get() || !aFile->IsOpen())
    {
        ThreadRelease();
        return 1;
    }
    assert(windowInstance.Get());
    TrecSubPointer<TPage, AnafacePage> newPage = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(drawingBoard);

    mainPage = TrecSubToTrec(newPage);


    if (!mainPage.Get())
    {
        ThreadRelease();
        return 2;
    }

    TDataArray<TPage::EventID_Cred> cred;
    
    newPage->OnResize(mainPageSpace, 0, cred);

    TrecPointer<TFileShell> uisFile = TFileShell::GetFileInfo(aFile->GetFilePath());
    aFile->Close();
    aFile.Nullify();
    if (dynamic_cast<TapEventHandler*>(eh.Get()))
        dynamic_cast<TapEventHandler*>(eh.Get())->SetWindow(TrecPointerKey::GetTrecPointerFromSoft<>(self));
    TString prepRes(newPage->PrepPage(uisFile, eh));

    if (prepRes.GetSize())
    {
        MessageBox(this->currentWindow, prepRes.GetConstantBuffer().getBuffer(), L"Error Constructing UI!", 0);
        ThreadRelease();
        return 2;
    }

    newPage->Create (mainPageSpace, d3dEngine);


    safeToDraw = 1;
    Draw();

    ThreadRelease();
    return 0;
}

void TWebWindow::SetEnvironmentGenerator(TrecPointer<EnvironmentGenerator> gen)
{
    ThreadLock();
    envGenerator = gen;
    ThreadRelease();
}

void TWebWindow::DrawOtherPages()
{
    ThreadLock();
    if (webPages.Get())
    {
        webPages->Draw(TrecPointer<TVariable>());
       
        if (currentPage.Get())
            currentPage->Draw(TrecPointer<TVariable>());
    }
    ThreadRelease();
}

void TWebWindow::OnLButtonDown(UINT nFlags, TPoint point)
{
    ThreadLock();
    if (webPages.Get())
    {
        message_output mOut = message_output::mo_negative;

        TDataArray<TPage::EventID_Cred> cred;
        
        webPages->OnLButtonDown(nFlags, point, mOut, cred);

        if (mOut != message_output::mo_negative)
        {
            ThreadRelease();
            return;
        }

        if (currentPage.Get())
        {
            currentPage->OnLButtonDown(nFlags, point, mOut, cred);
        }
    }

    TWindow::OnLButtonDown(nFlags, point);
    ThreadRelease();
}

void TWebWindow::OnMouseMove(UINT nFlags, TPoint point)
{
    ThreadLock();
    if (webPages.Get())
    {
        message_output mOut = message_output::mo_negative;

        TDataArray<TPage::EventID_Cred> cred;
        
        webPages->OnMouseMove(nFlags, point, mOut, cred);

        if (mOut != message_output::mo_negative)
        {
            ThreadRelease();
            return;
        }

        if (currentPage.Get())
        {
            currentPage->OnMouseMove(nFlags, point, mOut, cred);
        }
    }

    TWindow::OnMouseMove(nFlags, point);
    ThreadRelease();
}

void TWebWindow::OnLButtonUp(UINT nFlags, TPoint point)
{
    ThreadLock();
    if (webPages.Get())
    {
        message_output mOut = message_output::mo_negative;

        TDataArray<TPage::EventID_Cred> cred;
        
        webPages->OnLButtonUp(nFlags, point, mOut, cred);

        if (mOut != message_output::mo_negative)
        {
            ThreadRelease();
            return;
        }

        

        if (currentPage.Get())
        {
            currentPage->OnLButtonUp(nFlags, point, mOut, cred);
        }
    }

    TWindow::OnLButtonUp(nFlags, point);
    Draw();
    ThreadRelease();
}

bool TWebWindow::OnScroll(const TPoint& point, const TPoint& direction)
{
    if (TWindow::OnScroll(point, direction))
        return true;

    TDataArray<TPage::EventID_Cred> cred;
    if (currentPage.Get())
    {
        TDataArray<TPage::EventID_Cred> cred;
        
        return currentPage->OnScroll(false, point, direction, cred);
    }
    return false;
}

TString TWebWindow::FixUrl(const TString& url)
{
    if (url.StartsWith(L"Anagame://", true) || url.StartsWith(L"File://", true) || url.StartsWith(L"http://", true) || url.StartsWith(L"https://", true) || url.StartsWith(L"ftp://", true))
    {
        return TString(url);
    }
    TrecPointer<TFileShell> file = TFileShell::GetFileInfo(url);

    if (file.Get())
        return TString(L"File://") + url;
    return TString(L"https://") + url;
    
}

TrecSubPointer<TPage, WebPage> TWebWindow::GetWebPage(const TString& url)
{
    ThreadLock();
    auto ret = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, WebPage>(drawingBoard, self);


    TDataArray<TPage::EventID_Cred> cred;
    
    webPage = webPages->GetContentSpace();
    ret->OnResize(webPage, 0, cred);

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
                ThreadRelease();
                return ret;
            }
        }

        // To-Do: Set up Web-Handler
        TrecPointer<TEnvironment> env = envGenerator->GetEnvironment(TFileShell::GetFileInfo(index->GetParent()->GetPath()));
        env->SetUpEnv();
        ret->SetEnvironment(env);
        ret->OnResize(this->webPage, 0, cred);
        ret->PrepPage(index, TrecPointer < TPage::EventHandler>());

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
                ThreadRelease();
                return ret;
            }
        }

        // To-Do: Set up Web-Handler
        TrecPointer<TEnvironment> env = envGenerator->GetEnvironment(TFileShell::GetFileInfo(index->GetParent()->GetPath()));
        env->SetUpEnv();
        ret->SetEnvironment(env);
        ret->directory.Set(index->GetParent()->GetPath());
        ret->PrepPage(index, TrecPointer < TPage::EventHandler>());
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
    ThreadRelease();
    return ret;
}

