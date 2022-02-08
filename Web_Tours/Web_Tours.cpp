// Web_tours.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Web_tours.h"
#include <TInstance.h>
#include <DirectoryInterface.h>
#include "WebToursHandler.h"
#include <TWebWindow.h>
#include "WebEnvironment.h"
#include <TThread.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// AnaGame Globals
TrecPointer<TProcess> mainInstance;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    TString tmlFile(GetDirectoryWithSlash(CentralDirectories::cd_Executable));
    tmlFile.Append(L"Resources\\Web-Tours\\BrowserInterface.json");

    TString title(L"Web-Tours");
    TString winClass(L"WebToursWindow");

    TThread::SetMainThread();

    mainInstance = TrecPointerKey::GetNewSelfTrecPointerAlt<TProcess, TInstance>(title, winClass, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, nullptr, nCmdShow, hInstance, WndProc);

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEBTOURS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WEBTOURS);
    wcex.lpszClassName = winClass.GetConstantBuffer().getBuffer();
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    dynamic_cast<TInstance*>(mainInstance.Get())->SetMainWindow(wcex, tmlFile, TrecPointerKey::GetNewSelfTrecPointerAlt < TPage::EventHandler, WebToursHandler>(mainInstance), L"", t_window_type::t_window_type_web);

    TrecSubPointer<TWindow, TWebWindow> webWindow = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TWebWindow>(dynamic_cast<TInstance*>(mainInstance.Get())->GetMainWindow());

    if (webWindow.Get())
    {
        // Web Based Initialization here
        webWindow->SetEnvironmentGenerator(TrecPointerKey::GetNewTrecPointerAlt<EnvironmentGenerator, WebEnvGenerator>(mainInstance, dynamic_cast<TInstance*>(mainInstance.Get())->GetMainWindow()));
        webWindow->AddNewTab();
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEBTOURS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    mainInstance.Nullify();
    return (int)msg.wParam;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_GETMINMAXINFO)
    {
        PMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(lParam);

        info->ptMinTrackSize.x = 600;
        info->ptMinTrackSize.y = 450;
        return 0;
    }

    if (mainInstance.Get())
        return dynamic_cast<TInstance*>(mainInstance.Get())->Proc(hWnd, message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
