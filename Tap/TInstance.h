#pragma once
#include <TObject.h>
#include "TIDEWindow.h"
#include "HandlerMessage.h"
#include <TTrecSoftPointerArray.h>

/**
 * Class: WindowContainer
 * Purpose: Holds a Window while also advising the instance on whether to destroy it or not
 */
class WindowContainer
{
public:
	/**
	 * Method: WindowContainer::WindowContainer
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New Window container
	 */
	WindowContainer();

	/**
	 * the window to hold
	 */
	TrecPointer<TWindow> window;

	/**
	 * whether it is safe to destroy
	 */
	bool destroy;

	/**
	 * whether there are multiple messages stacked
	 */
	int messageStack;
};

/**
 * Enum Class: t_window_type
 * Purpose: Used to tell the instance what type of Window to produce
 */
typedef enum class t_window_type
{
	t_window_type_plain, // Gets a Plain Window
	t_window_type_ide    // Gets a Basic IDE style Window
}t_window_type;

/**
 * Class: TInstance
 * Purpose: Respresents an instance of a Machine-Level Anagame Application
 */
class _TAP_DLL TInstance : public TObject
{
	friend class TWindow;
	friend class TDialog;
	friend class Page;
	friend class IDEPage;
	friend class EventHandler;
public:

	/**
	 * Method: TInstance::TInstance
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
	TInstance(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins, WNDPROC wp);

	~TInstance();

	/**
	 * Method: TInstance::Proc
	 * Purpose: Takes in messages from WinProc and sends them to the appropriate window
	 * Parameters: HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
	 * Returns: LRESULT - the Result of the message
	 */
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/**
	 * Method: TInstance::SetMainWindow
	 * Purpose: Sets up the Main Window
	 * Parameters: WNDCLASSEXW& wcex - the details of the Window
	 *				TString& file - path to the tml file for it's contents
	 *				TrecPointer<EventHandler> eh - main handler for the window
	 *				t_window_type winType - type of window desired (default provides a regular TWindow)
	 * Returns: int - error code
	 */
	int SetMainWindow(WNDCLASSEXW& wcex, TString& file, TrecPointer<EventHandler> eh, t_window_type winType = t_window_type::t_window_type_plain);

	/**
	 * Method: TInstance::GetWindowByName
	 * Purpose: Retrieves the window held by the name 
	 * Parameters: TString& - the name of the window to get
	 * Returns: TrecPointer<TWindow> - the TWindow requested
	 */
	TrecPointer<TWindow> GetWindowByName(TString&);

	//bool LaunchDialog(TString& file, TrecPointer<EventHandler> eh, HWND callingWindow);

	/**
	 * Method: TInstance::GetProc
	 * Purpose: Retrieves the WinProc function
	 * Parameters: void
	 * Returns: WNDPROC - the location of the WinProc function
	 */
	WNDPROC GetProc();

	/**
	 * Method: TInstance::GetMainWindow
	 * Purpose: Retrieves the main window of the instance
	 * Parameters: void
	 * Returns:TrecPointer<TWindow> - the main window
	 */
	TrecPointer<TWindow> GetMainWindow();

	/**
	 * Method: TInstance::GetWindow
	 * Purpose: Retirevs the TWindow by handle
	 * Parameters: HWND h - the handle to get the TWindow for
	 * Returns: TrecPointer<TWindow> - The Window requested
	 */
	TrecPointer<TWindow> GetWindow(HWND h);

	/**
	 * Method: TInstance::LockWindow
	 * Purpose: Locks the specified Window
	 * Parameters: HWND win - the window to lock 
	 * Returns: void
	 */
	void LockWindow(HWND win);

	/** 
	 * Method: TInstance::UnlockWindow
	 * Purpose: Unlocks the specified Window
	 * Parameters: HWND win - the window to unlock 
	 * Returns: void
	 */
	void UnlockWindow(HWND win);


	/**
	 * Method: TInstance::GetInstanceHandle
	 * Purpose: Retrievs the raw instance handle
	 * Parameters: void
	 * Returns: HINSTANCE - instance provided during construction
	 */
	HINSTANCE GetInstanceHandle();

	/**
	 * Method: TInstance::GetFactory
	 * Purpose: retirevs the factory used for Direct2D
	 * Parameters: void
	 * Returns: TrecComPointer<ID2D1Factory1> - the factory used for Direct 2D drawing
	 */
	TrecComPointer<ID2D1Factory1> GetFactory();

	/**
	 * Method: TInstance::SetSelf
	 * Purpose: Allows the instance to hold a reference to itself
	 * Parameters: TrecPointer<TInstance> i - the instance to hold
	 * Returns: void
	 */
	void SetSelf(TrecPointer<TInstance> i);


	/**
	 * Method: TInstance::DispatchAnagameMessage
	 * Purpose: Allows a Handler to send s message to another handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to dispatch to a handler
	 * Returns: void
	 */
	void DispatchAnagameMessage(TrecPointer<HandlerMessage> message);


	/**
	 * Method: TInstance::GetHandler
	 * Purpose: Retrieves the Handler by name and type
	 * Parameters: const TString& name - the name of the handler
	 *				anagame_page pageType - the type of handler
	 * Returns: TrecPointer<EventHandler> - the handler specified (null if not found)
	 */
	TrecPointer<EventHandler> GetHandler(const TString& name, anagame_page pageType);

protected:

	/**
	 * Method: TInstance::AssertDialogRegistered
	 * Purpose: Makes sure that a dialog class is registered with Windows
	 * Parameters: void
	 * Returns: void
	 */
	void AssertDialogRegistered();


	/**
	 * token to maintain the Dialog windows
	 */
	ATOM dialogAtom;


	/**
	 * Method: TInstance::RegisterDialog
	 * Purpose: Adds a window to the list of windows
	 * Parameters: TrecPointer<TWindow> - the window to add
	 * Returns: void
	 */
	void RegisterDialog(TrecPointer<TWindow>);



	/**
	 * Handle to the Windows Instance
	 */
	HINSTANCE instance;

	/**
	 * The Windows name and class of the Main Window
	 */
	TString mainWindowName, mainWindowClass;

	/**
	 * style of the main window
	 */
	UINT mainStyle;

	/**
	 * used by Window
	 */
	int command;

	/**
	 * Handle to the main window
	 */
	HWND mainWindowHandle;

	/**
	 * central Window held
	 */
	TrecPointer<TWindow> mainWindow;

	/**
	 * factory object for working with Direct2D
	 */
	TrecComPointer<ID2D1Factory1> factory;


	/**
	 * List of Windows held by the instance
	 */
	TDataArray<TrecPointer<WindowContainer>> windowList;


	/**
	 * Method: TInstance::CleanWindows
	 * Purpose: Removes all WIndows from the Instance
	 * Parameters: void
	 * Returns: void
	 */
	void CleanWindows();


	/**
	 * address of the WinProc function
	 */
	WNDPROC proctor;

	/**
	 * Self reference to this TInstance
	 */
	TrecPointerSoft<TInstance> self;


	/**
	 * number of message currently being processed
	 */
	UINT messageStack;

	/**
	 * current id to assign a new handler
	 */
	UINT handlerID;



	/**
	 * List of Handlers that a message could be dispatched to 
	 */
	TTrecPointerSoftArray<EventHandler> registeredHandlers;


	/**
	 * Method: TInstance::UnregisterHandler
	 * Purpose: Removes the given handler from it's list
	 * Parameters:TrecPointer<EventHandler> handler - the handler to remove
	 * Returns: void
	 */
	void UnregisterHandler(TrecPointer<EventHandler>);


	/**
	 * Method: TInstance::RegisterHandler
	 * Purpose: Adds a handler to the list of handlers
	 * Parameters: TrecPointer<EventHandler> handler - the handler to add
	 * Returns: bool whether the handler was added or not
	 */
	bool RegisterHandler(TrecPointer<EventHandler> handler);


	/**
	 * Method: TInstance::CleanHandlerList
	 * Purpose: Cleans the list of handlers so that none are null
	 * Parameters: void
	 * Returns: void
	 */
	void CleanHandlerList();
};

