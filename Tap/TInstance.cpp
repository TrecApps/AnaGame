#include "TInstance.h"
#include <windowsx.h>
#include "TDialog.h"
#include "TIdeWindow.h"

// Various Built-in handlers Anagame offers
#include "TCodeHandler.h"
#include "TerminalHandler.h"
#include "TWebWindow.h"
#include "FileHandler.h"

static TString dialogClassName(L"TDialog");

/**
 * Method: TInstance::
 * Purpose:
 * Parameters: TString& name - the name of the main window
 *				TString& winClass - the class name of the Window (used by Windows)
 *				UINT style - the desired style of the Window
 *				HWND parent - parent of the Window
 *				int commandShow - how to first present the window
 *				HINSTANCE ins - the Instance Handle provided by WinMain
 *				WNDPROC wp - the Wind Proc function that intercepts messages
 * Returns: new TInstance object
 */
TInstance::TInstance(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins, WNDPROC wp)
{
	handlerID = 1;
	messageStack = 0;
	if (!wp)
		throw L"ERROR! Window Proc Function must be a valid pointer!";
	proctor = wp;
	instance = ins;
	command = commandShow;
	mainWindowClass.Set(winClass);
	mainWindowName.Set(name);
	mainStyle = style;

	TrecComPointer<ID2D1Factory1>::TrecComHolder factoryHolder;

	D2D1_FACTORY_OPTIONS d2dDebugLevel = { D2D1_DEBUG_LEVEL_WARNING };

	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, d2dDebugLevel, factoryHolder.GetPointerAddress());
	factory = factoryHolder.Extract();
	dialogAtom = 0;
}

/**
 * Method: TInstance::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TInstance::GetType()
{
	return TString(L"TInstance;") + TObject::GetType();
}

TInstance::~TInstance()
{
	mainWindow.Delete();

	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		windowList[Rust].Delete();
	}
}

/**
 * Method: TInstance::Proc
 * Purpose: Takes in messages from WinProc and sends them to the appropriate window
 * Parameters: HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
 * Returns: LRESULT - the Result of the message
 */
LRESULT TInstance::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ThreadLock();
	TWindow* win = nullptr;
	if (message == WM_LBUTTONUP)
		int e = 4;
	int windowIndex = -1;

	if (mainWindow.Get() && mainWindow->GetWindowHandle() == hWnd)
		win = mainWindow.Get();
	else
	{
		for (UINT c = 0; c < windowList.Size(); c++)
		{
			if (windowList[c].Get() && windowList[c]->window.Get() && windowList[c]->window.Get()->GetWindowHandle() == hWnd)
			{
				win = windowList[c]->window.Get();
				windowIndex = c;
				windowList[c]->messageStack++;
				break;
			}
		}
	}

	if (!win)
	{
		ThreadRelease();
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	switch (message)
	{
	case WM_APP_PLAYER_EVENT:
		win->OnVideoEvent(wParam);
		break;
	case WM_PAINT:
		win->Draw();
		break;
	case WM_DESTROY:
		if(win->OnDestroy())
		{
			if(windowIndex == -1)
			{
				mainWindow.Delete();
				for(UINT c = 0; c < windowList.Size(); c++)
				{
					windowList[c].Delete();
				}

				PostQuitMessage(0);
			}
			else
			{
				if (windowList[windowIndex].Get())
				{
					windowList[windowIndex]->destroy = true;
					//windowList[windowIndex]->messageStack--;
				}
				// return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		win->OnRButtonDown(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONDOWN:
		win->OnLButtonDown(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONDBLCLK:
		win->OnLButtonDblClk(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONUP:
		win->OnLButtonUp(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_RBUTTONUP:
		win->OnRButtonUp(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_NCMOUSEMOVE:
	case WM_MOUSEMOVE:
		win->OnMouseMove(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_CHAR:
		win->OnChar(true, wParam, lParam & 0x0000FFFF, 0);
		break;
	case WM_KEYDOWN:
		win->OnChar(false, wParam, lParam & 0x0000FFFF, 0);
		break;
	case WM_SIZE:
		win->OnWindowResize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}

	if (windowIndex != -1 && windowIndex < windowList.Size() && windowList[windowIndex].Get() && windowList[windowIndex]->messageStack > 0)
	{
		windowList[windowIndex]->messageStack--;
	}
	CleanWindows();
	auto ret = DefWindowProc(hWnd, message, wParam, lParam);
	ThreadRelease();
	return ret;
}

/**
 * Method: TInstance::SetMainWindow
 * Purpose: Sets up the Main Window
 * Parameters: WNDCLASSEXW& wcex - the details of the Window
 *				TString& file - path to the tml file for it's contents
 *				TrecPointer<EventHandler> eh - main handler for the window
 *				t_window_type winType - type of window desired (default provides a regular TWindow)
 * Returns: int - error code
 */
int TInstance::SetMainWindow(WNDCLASSEXW& wcex, TString& file, TrecPointer<EventHandler> eh, t_window_type winType)
{
	ThreadLock();
	WORD regResult = RegisterClassExW(&wcex);
	if (!regResult)
	{
		int err = GetLastError();
		ThreadRelease();
		return 1;
	}
	switch (winType)
	{
	case t_window_type::t_window_type_ide:
		mainWindow = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, TIdeWindow>(mainWindowName, mainWindowClass, mainStyle, mainWindowHandle, command,
			TrecPointerKey::GetTrecPointerFromSoft(self), 150, 30);
		break;
	case t_window_type::t_window_type_plain:
		mainWindow = TrecPointerKey::GetNewSelfTrecPointer<TWindow>(mainWindowName, mainWindowClass, mainStyle, mainWindowHandle, command, 
			TrecPointerKey::GetTrecPointerFromSoft(self));
		break;
	case t_window_type::t_window_type_web:
		mainWindow = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, TWebWindow>(mainWindowName, mainWindowClass, mainStyle, mainWindowHandle, command,
			TrecPointerKey::GetTrecPointerFromSoft(self), 75, 30);
	}
	

	mainWindow->PrepareWindow();
	auto ret = mainWindow->CompileView(file, eh);
	ThreadRelease();
	return ret;
}

/**
 * Method: TInstance::GetWindowByName
 * Purpose: Retrieves the window held by the name
 * Parameters: TString& - the name of the window to get
 * Returns: TrecPointer<TWindow> - the TWindow requested
 */
TrecPointer<TWindow> TInstance::GetWindowByName(TString& name)
{
	ThreadLock();
	if (mainWindow.Get() && !mainWindow->GetWinName().Compare(name))
	{
		ThreadRelease();
		return mainWindow;
	}
	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->window.Get() && !windowList[Rust]->window->GetWinName().Compare(name))
		{
			ThreadRelease();
			return windowList[Rust]->window;
		}
	}
	ThreadRelease();
	return TrecPointer<TWindow>();
}

void TInstance::AssertDialogRegistered()
{
	ThreadLock();
	if (dialogAtom)
	{
		ThreadRelease();
		return;
	}
	WNDCLASSEXW windowClassStructure;
	ZeroMemory(&windowClassStructure, sizeof(windowClassStructure));

	TString winClass(L"Dialog");

	windowClassStructure.cbSize = sizeof(windowClassStructure);
	windowClassStructure.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClassStructure.lpfnWndProc = proctor;
	windowClassStructure.cbClsExtra = 0;
	windowClassStructure.cbWndExtra = 0;
	windowClassStructure.hInstance = instance;
	windowClassStructure.hIcon = nullptr;
	windowClassStructure.hCursor = nullptr;
	windowClassStructure.hbrBackground = nullptr;
	windowClassStructure.lpszMenuName = nullptr;
	windowClassStructure.lpszClassName = winClass.GetConstantBuffer().getBuffer();
	windowClassStructure.hIconSm = nullptr;

	dialogAtom = RegisterClassExW(&windowClassStructure);
	if (!dialogAtom)
	{
		int err = GetLastError();
		ThreadRelease();
		throw err;
	}
	ThreadRelease();
}



/**
 * Method: TInstance::GetProc
 * Purpose: Retrieves the WinProc function
 * Parameters: void
 * Returns: WNDPROC - the location of the WinProc function
 */
WNDPROC TInstance::GetProc()
{
	return proctor;
}


/**
 * Method: TInstance::GetMainWindow
 * Purpose: Retrieves the main window of the instance
 * Parameters: void
 * Returns:TrecPointer<TWindow> - the main window
 */
TrecPointer<TWindow> TInstance::GetMainWindow()
{
	ThreadLock();
	auto ret = mainWindow;
	ThreadRelease();
	return ret;
}

/**
 * Method: TInstance::GetWindow
 * Purpose: Retirevs the TWindow by handle
 * Parameters: HWND h - the handle to get the TWindow for
 * Returns: TrecPointer<TWindow> - The Window requested
 */
TrecPointer<TWindow> TInstance::GetWindow(HWND h)
{
	ThreadLock();
	if (h == mainWindow->GetWindowHandle())
	{
		auto ret = mainWindow;
		ThreadRelease();
		return ret;
	}
	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->window.Get() && windowList[Rust]->window->GetWindowHandle() == h)
		{
			auto ret =  windowList[Rust]->window;
			ThreadRelease();
			return ret;
		}
	}
	ThreadRelease();
	return TrecPointer<TWindow>();
}

/**
 * Method: TInstance::LockWindow
 * Purpose: Locks the specified Window
 * Parameters: HWND win - the window to lock
 * Returns: void
 */
void TInstance::LockWindow(HWND win)
{
	ThreadLock();
	if (mainWindow.Get() && mainWindow->GetWindowHandle() == win)
	{
		mainWindow->LockWindow();
		ThreadRelease();
		return;
	}

	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->window.Get() && windowList[Rust]->window->GetWindowHandle() == win)
		{
			windowList[Rust]->window->LockWindow();
			ThreadRelease();
			return;
		}
	}
	ThreadRelease();
}

/**
 * Method: TInstance::UnlockWindow
 * Purpose: Unlocks the specified Window
 * Parameters: HWND win - the window to unlock
 * Returns: void
 */
void TInstance::UnlockWindow(HWND win)
{
	ThreadLock();
	if (mainWindow.Get() && mainWindow->GetWindowHandle() == win)
	{
		mainWindow->UnlockWindow();
		ThreadRelease();
		return;
	}

	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->window.Get() && windowList[Rust]->window->GetWindowHandle() == win)
		{
			windowList[Rust]->window->UnlockWindow();
			ThreadRelease();
			return;
		}
	}
	ThreadRelease();
}

/**
 * Method: TInstance::GetInstanceHandle
 * Purpose: Retrievs the raw instance handle
 * Parameters: void
 * Returns: HINSTANCE - instance provided during construction
 */
HINSTANCE TInstance::GetInstanceHandle()
{
	return instance;
}

/**
 * Method: TInstance::GetFactory
 * Purpose: retirevs the factory used for Direct2D
 * Parameters: void
 * Returns: TrecComPointer<ID2D1Factory1> - the factory used for Direct 2D drawing
 */
TrecComPointer<ID2D1Factory1> TInstance::GetFactory()
{
	ThreadLock();
	auto ret = factory;
	ThreadRelease();
	return ret;
}

/**
 * Method: TInstance::SetSelf
 * Purpose: Allows the instance to hold a reference to itself
 * Parameters: TrecPointer<TInstance> i - the instance to hold
 * Returns: void
 */
void TInstance::SetSelf(TrecPointer<TInstance> i)
{
	if (this != i.Get() || i.Get() != this)
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(i);

	AssertDialogRegistered();
}

/**
 * Method: TInstance::DispatchAnagameMessage
 * Purpose: Allows a Handler to send s message to another handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to dispatch to a handler
 * Returns: void
 */
void TInstance::DispatchAnagameMessage(TrecPointer<HandlerMessage> message)
{
	ThreadLock();
	CleanHandlerList();
	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (registeredHandlers[Rust].Get() && registeredHandlers[Rust]->ShouldProcessMessage(message))
			registeredHandlers[Rust]->ProcessMessage(message);
	}
	ThreadRelease();
}

/**
 * Method: TInstance::GetHandler
 * Purpose: Retrieves the Handler by name and type
 * Parameters: const TString& name - the name of the handler
 *				anagame_page pageType - the type of handler
 * Returns: TrecPointer<EventHandler> - the handler specified (null if not found)
 */
TrecPointer<EventHandler> TInstance::GetHandler(const TString& name, anagame_page pageType)
{
	ThreadLock();
	TrecPointer<EventHandler> ret;
	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (registeredHandlers[Rust].Get())
		{
			if (registeredHandlers[Rust]->name.Compare(name))
				continue;
			switch (pageType)
			{
			case anagame_page::anagame_page_command_prompt:
				if (dynamic_cast<TerminalHandler*>(registeredHandlers[Rust].Get()))
				{
					ret = registeredHandlers[Rust];
				}
				break;
			case anagame_page::anagame_page_code_file:
				if (dynamic_cast<TCodeHandler*>(registeredHandlers[Rust].Get()))
					ret = registeredHandlers[Rust];
				break;
			case anagame_page::anagame_page_file_node:
				if (dynamic_cast<FileHandler*>(registeredHandlers[Rust].Get()))
					ret = registeredHandlers[Rust];
				// To-Do: As more Handlers are added, mention them here
			}
		}
	}
	ThreadRelease();
	return ret;
}

/**
 * Method: TInstance::RegisterDialog
 * Purpose: Adds a window to the list of windows
 * Parameters: TrecPointer<TWindow> - the window to add
 * Returns: void
 */
void TInstance::RegisterDialog(TrecPointer<TWindow> win)
{
	ThreadLock();
	TrecPointer<WindowContainer> contain = TrecPointerKey::GetNewTrecPointer<WindowContainer>();
	contain->window = win;
	windowList.push_back(contain);
	ThreadRelease();
}

/**
 * Method: TInstance::CleanWindows
 * Purpose: Removes all WIndows from the Instance
 * Parameters: void
 * Returns: void
 */
void TInstance::CleanWindows()
{
	ThreadLock();
	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->destroy && !windowList[Rust]->messageStack)
		{
			// windowList[Rust]->window.Delete();
			windowList.RemoveAt(Rust);
		}
	}
	ThreadRelease();
}

/**
 * Method: TInstance::UnregisterHandler
 * Purpose: Removes the given handler from it's list
 * Parameters:TrecPointer<EventHandler> handler - the handler to remove
 * Returns: void
 */
void TInstance::UnregisterHandler(TrecPointer<EventHandler> handler)
{
	ThreadLock();
	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (handler.Get() == registeredHandlers[Rust].Get())
		{
			registeredHandlers.RemoveAt(Rust);
			break;
		}
	}
	ThreadRelease();
}

/**
 * Method: TInstance::RegisterHandler
 * Purpose: Adds a handler to the list of handlers
 * Parameters: TrecPointer<EventHandler> handler - the handler to add
 * Returns: bool whether the handler was added or not
 */
bool TInstance::RegisterHandler(TrecPointer<EventHandler> handler)
{
	if(!handler.Get())
		return false;

	ThreadLock();
	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (handler.Get() == registeredHandlers[Rust].Get())
		{
			ThreadRelease();
			return false;
		}
	}

	registeredHandlers.push_back(handler);
	handler->id = handlerID++;
	ThreadRelease();
	return true;
}

/**
 * Method: TInstance::CleanHandlerList
 * Purpose: Cleans the list of handlers so that none are null
 * Parameters: void
 * Returns: void
 */
void TInstance::CleanHandlerList()
{
	ThreadLock();
	for (int Rust = static_cast<int>(registeredHandlers.Size()) - 1; Rust >= 0; Rust--)
	{
		if (!registeredHandlers[Rust].Get())
			registeredHandlers.RemoveAt(Rust--);
	}
	ThreadRelease();
}

/**
 * Method: WindowContainer::WindowContainer
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Window container
 */
WindowContainer::WindowContainer()
{
	destroy = false;
	messageStack = 0;
}
