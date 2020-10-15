#include "TWebWindow.h"

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

    mainPage.left = location.left;
    mainPage.right = location.right;
    mainPage.top = location.top;
    mainPage.bottom = mainPage.top + mainViewSpace;

    tabs.left = mainPage.left;
    tabs.right = mainPage.right;
    tabs.top = mainPage.bottom;
    tabs.bottom = tabs.top + pageBarSpace;

    webPage.left = tabs.left;
    webPage.right = tabs.right;
    webPage.top = tabs.bottom;
    webPage.bottom = location.bottom;

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


    return 0;
}

void TWebWindow::SetEnvironmentGenerator(TrecPointer<EnvironmentGenerator> gen)
{
    envGenerator = gen;
}

TString TWebWindow::FixUrl(const TString& url)
{
    return TString();
}

TrecSubPointer<Page, WebPage> TWebWindow::GetWebPage(const TString& url)
{
    if (url.StartsWith(L"Anagame://"))
    {

    }
    else if (url.StartsWith(L"File://"))
    {

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
    return TrecSubPointer<Page, WebPage>();
}
