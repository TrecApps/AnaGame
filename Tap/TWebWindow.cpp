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

void TWebWindow::AddNewTab()
{
}

void TWebWindow::AddNewTab(const TString& url)
{
}

int TWebWindow::CompileView(TString& file, TrecPointer<EventHandler> eh)
{
    return 0;
}
