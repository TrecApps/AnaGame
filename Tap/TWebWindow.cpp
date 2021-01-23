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
    mainPageSpace = tabs = webPage = { 0.0f,0.0f,0.0f,0.0f };
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

    webPages = TrecPointerKey::GetNewSelfTrecSubPointer<TControl, TTabBar>(drawingBoard, TrecPointer<TArray<styleTable>>());
    // Set the attributes of this Tab Bar

    webPages->addAttribute(L"|HaveAddTab", TrecPointerKey::GetNewTrecPointer<TString>(L"true"));
    webPages->onCreate(tabs, d3dEngine);



    return 0;
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
        newWebPage->SetArea(webPage);
        TString title(newWebPage->GetTitle());
        if (!title.GetSize())
            title.Set(L"Untitled Tab");
        TrecPointer<Tab> tab = webPages->AddTab(title);
        TrecSubPointer<TabContent, TabWebPageContent> pageContent = TrecPointerKey::GetNewTrecSubPointer<TabContent, TabWebPageContent>();
        pageContent->SetWebPage(newWebPage);
        tab->SetContent(TrecPointerKey::GetTrecPointerFromSub<TabContent, TabWebPageContent>(pageContent));
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

void TWebWindow::DrawOtherPages()
{
    if (webPages.Get())
    {
        webPages->onDraw();
        TrecPointer<Tab> tab = webPages->GetCurrentTab();
        if (tab.Get() && tab->GetContent().Get())
            tab->GetContent()->Draw(nullptr);
    }
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

TabWebPageContent::TabWebPageContent()
{
}

TabWebPageContent::~TabWebPageContent()
{
}

void TabWebPageContent::Resize(const D2D1_RECT_F& loc)
{
    if (webPage.Get())
        webPage->OnResize(loc, 0, TrecPointer<TWindowEngine>());
}

void TabWebPageContent::Draw(TObject* obj)
{
    if (webPage.Get())
        webPage->Draw(nullptr);
}

TabContentType TabWebPageContent::GetContentType()
{
    return TabContentType::tct_web_page;
}

bool TabWebPageContent::HasContent()
{
    return webPage.Get() != nullptr;
}

bool TabWebPageContent::TookTab(TrecPointer<Tab> tab)
{
    if(!webPage.Get())
        return false;
    return webPage->TookTab(tab);
}

void TabWebPageContent::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
    if (webPage.Get())
        webPage->OnRButtonUp(nFlags, point, mOut);
}

void TabWebPageContent::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
    if (webPage.Get())
        webPage->OnLButtonDown(nFlags, point, mOut, TrecPointer<TFlyout>());
}

void TabWebPageContent::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControls)
{
    if (webPage.Get())
        webPage->OnRButtonDown(nFlags, point, mOut);
}

void TabWebPageContent::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)
{
    if (webPage.Get())
        webPage->OnMouseMove(nFlags, point, mOut, TrecPointer<TFlyout>());
}

bool TabWebPageContent::OnMouseLeave(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
    return false;
}

void TabWebPageContent::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
    if (webPage.Get())
        webPage->OnLButtonDblClk(nFlags, point, mOut);
}

void TabWebPageContent::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
    if (webPage.Get())
        webPage->OnLButtonUp(nFlags, point, mOut, TrecPointer<TFlyout>());
}

bool TabWebPageContent::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
    if (webPage.Get())
        webPage->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
    return false;
}

TrecSubPointer<Page, WebPage> TabWebPageContent::GetWebPage()
{
    return webPage;
}

void TabWebPageContent::SetWebPage(TrecSubPointer<Page, WebPage> webPage)
{
    this->webPage = webPage;
}
