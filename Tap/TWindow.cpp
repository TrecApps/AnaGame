#include "TWindow.h"
#include "TInstance.h"
#include <AnafacePage.h>

bool IsD2D1RectEqual(const D2D1_RECT_F& r1, const  D2D1_RECT_F& r2, float difference)
{
	difference = abs(difference);
	return (abs(r1.bottom - r2.bottom) <= difference) &&
		(abs(r1.left - r2.left) <= difference) &&
		(abs(r1.right - r2.right) <= difference) &&
		(abs(r1.top - r2.top) <= difference);
}

static BLENDFUNCTION blendFunc = {
	AC_SRC_OVER,
	0,
	255,
	AC_SRC_ALPHA
};




/**
 * Method: TWindow::TWindow
 * Purpose: Constructor
 * Parameters: TString& name - the name of the Window
 *				TString& winClass - the name to tell Windows to use for this Window type
 *				UINT style - the Style of window to use
 *				HWND parent - the Handle to the Parent Window (if any)
 *				int commandShow - How to show the Window (provided in WinMain)
 *				TrecPointer ins - pointer to the TInstance involved (hence why TInstance has a SetSelf method)
 * Returns: New Window
 */
TWindow::TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins)
{
	currentWindow = CreateWindowW(winClass.GetConstantBuffer().getBuffer(),
		name.GetConstantBuffer().getBuffer(), style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, nullptr, ins->GetInstanceHandle(), nullptr);

	if (!currentWindow)
	{
		int lastError = GetLastError();
		throw L"Window ERROR! Failed to generate Window";
	}
	this->name.Set(name);
	this->parent = parent;
	this->winClass.Set(winClass);
	this->windowInstance = TrecPointerKey::GetSoftPointerFromTrec<>( ins);
	this->command = commandShow;

	HDC dc = GetWindowDC(currentWindow);
	SetMapMode(dc, MM_HIENGLISH);

	locked = hasDrawn = false;
	safeToDraw = 0;
	directFactory = ins->GetFactory();
	drawingBoard = TrecPointerKey::GetNewSelfTrecPointer<DrawingBoard>(directFactory, currentWindow);
	
}

/**
 * Method: TWindow::~TWindow
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TWindow::~TWindow()
{
	CleanUp();

	mainPage.Delete();

	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
		pages[Rust].Delete();

	d3dEngine.Delete();
	drawingBoard.Delete();
}

/**
 * Method: TWindow::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TWindow::GetType()
{
	return TString(L"TWindow;") + TObject::GetType();
}

/**
 * Method: TWindow::PrepareWindow
 * Purpose: Sets the Window up for viewing
 * Parameters: void
 * Returns: int - error code (0 = success)
 */
int TWindow::PrepareWindow()
{
	ThreadLock();
	if (!currentWindow)
	{
		ThreadRelease();
		return 1;
	}
	assert(windowInstance.Get());

	dynamic_cast<TInstance*>(TrecPointerKey::GetTrecPointerFromSoft<>(windowInstance).Get())->RegisterDialog(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));

	GetClientRect(currentWindow, &size);
	ShowWindow(currentWindow, command);
	UpdateWindow(currentWindow);

	animationCentral.SetWindow(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));

	ThreadRelease();
	return 0;
}

/**
 * Method: TWindow::CompileView
 * Purpose: Compiles the Main View
 * Parameters: TString& file - path of the TML file holding the Anaface
 *				TrecPointer<EventHandler> eh - the Handler to the Main page
 * Returns: int - error (0 == success)
 */
int TWindow::CompileView(TString& file, TrecPointer<TPage::EventHandler> eh)
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

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;

	TrecSubPointer<TPage, AnafacePage> newPage = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(drawingBoard);
	auto space = ConvertRectToD2D1Rect(this->size);
	newPage->OnResize(space, 0, cred, args);

	TrecPointer<TFileShell> uisFile = TFileShell::GetFileInfo(aFile->GetFilePath());
	aFile->Close();
	aFile.Nullify();
	TString prepRes(newPage->PrepPage(uisFile, eh));

	if (prepRes.GetSize())
	{
		MessageBox(this->currentWindow, prepRes.GetConstantBuffer().getBuffer(), L"Error Constructing UI!", 0);
		ThreadRelease();
		return 2;
	}

	TrecSubPointer<TPage, TControl> control = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TControl>(newPage->GetRootControl());

	if (control.Get())
		control->onCreate(newPage->GetArea(), d3dEngine);

	mainPage = TrecSubToTrec(newPage);
	RECT loc;
	GetClientRect(currentWindow, &loc);

	animationCentral.StartBegin();
	animationCentral.StartNewPersistant();
	safeToDraw = 1;
	Draw();

	ThreadRelease();
	return 0;
}

/**
 * Method: TWindow::GetWinClass
 * Purpose: Retrievs the Window-level class of this window
 * Parameters: void
 * Returns: TString - the class name registered with Windows
 */
TString TWindow::GetWinClass()
{
	ThreadLock();
	TString ret(winClass);
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::GetWinName
 * Purpose: Retrievs the Window-level name of this window
 * Parameters: void
 * Returns: TString - the name registered with Windows
 */
TString TWindow::GetWinName()
{
	ThreadLock();
	TString ret(name);
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::Draw
 * Purpose: Draws the window
 * Parameters: void
 * Returns: void
 */
void TWindow::Draw()
{
	LockDrawing();
	ThreadLock();
	if (mainPage.Get() && safeToDraw)
	{	UCHAR tempSafe = safeToDraw;
		safeToDraw = 0;
		if (d3dEngine.Get())
		{
			d3dEngine->PrepareScene(D2D1::ColorF(D2D1::ColorF::Wheat, 0.0f));
		}

		// New Mechanism tht uses bitmaps
		drawingBoard->BeginDraw();
		mainPage->Draw(TrecPointer<TVariable>());
		DrawOtherPages();

		if (flyout.Get())
			flyout->Draw(TrecPointer<TVariable>());

		drawingBoard->EndDraw();

		if (d3dEngine.Get())
		{
			d3dEngine->FinalizeScene();
		}
		// Get Whatever is on the window already
		safeToDraw = tempSafe;
	}
	if (!hasDrawn)
	{
		
		if (mainPage.Get())
		{
			hasDrawn = true;
			//mainPage->OnFirstDraw();
		}
	}
	ThreadRelease();
	UnlockDrawing();
}


/**
 * Method: TWindow::InduceDraw
 * Purpose: Sends a Message to Windows to send the Draw message
 * Parameters: void
 * Returns: void
 *
 * Note: this method is provided to be called by nimation threads as Draw should only be called by the Message thread, not by Animation threads
 */
void TWindow::InduceDraw()
{
	RedrawWindow(currentWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

void TWindow::OnVideoEvent(WPARAM param)
{
	//ThreadLock();
	//if (videoPlayer.Get())
	//{
	//	TrecComPointer<TPlayer> p = videoPlayer->GetPlayer();
	//	if (p.Get())
	//		p->HandleEvent(param);
	//}
	//ThreadRelease();
}

/**
 * Method: TWindow::OnRButtonUp
 * Purpose: Manages the Right Button Up Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TWindow::OnRButtonUp(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (locked)
	{
		ThreadRelease();
		return;
	}

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;

	message_output mOut = message_output::mo_negative;
	for(UINT c = 0; c < pages.Size() && (mOut == message_output::mo_negative ); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonUp(nFlags, point, mOut, cred, args);
	}

	if(mOut == message_output::mo_negative )
		mainPage->OnRButtonUp(nFlags, point, mOut, cred,args);
	ThreadRelease();
}

/**
 * Method: TWindow::OnLButtonDown
 * Purpose: Manages the Left Button Down Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 * Parameters:
 * Returns:
 */
void TWindow::OnLButtonDown(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (locked)
	{
		ThreadRelease();
		return;
	}
	message_output mOut = message_output::mo_negative;
	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;

	for(UINT c = 0; c < pages.Size() && (mOut == message_output::mo_negative); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDown(nFlags, point, mOut, cred, args);
	}

	if(mOut == message_output::mo_negative)
		mainPage->OnLButtonDown(nFlags, point, mOut, cred, args);
	ThreadRelease();
}

/**
 * Method: TWindow::OnRButtonDown
 * Purpose: Manages the Right Button Down Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TWindow::OnRButtonDown(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (locked)
	{
		ThreadRelease();
		return;
	}
	message_output mOut = message_output::mo_negative;

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;
	for(UINT c = 0; c < pages.Size() && (mOut == message_output::mo_negative); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonDown(nFlags, point, mOut, cred, args);
	}

	if(mOut == message_output::mo_negative)
		mainPage->OnRButtonDown(nFlags, point, mOut, cred, args);
	ThreadRelease();
}

/**
 * Method: TWindow::OnMouseMove
 * Purpose: Manages the Mouse Move Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TWindow::OnMouseMove(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (locked)
	{
		ThreadRelease();
		return;
	}
	message_output mOut = message_output::mo_negative;

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;
	if (currentScrollBar.Get())
	{

		currentScrollBar->OnMouseMove(nFlags, point, mOut, args);
		
		Draw();
		ThreadRelease();
		return;
	}



	for(UINT c = 0; c < pages.Size() && (mOut == message_output::mo_negative); c++)
	{
		if(pages[c].Get())
			pages[c]->OnMouseMove(nFlags, point, mOut, cred, args);
	}

	if(mOut == message_output::mo_negative)
		mainPage->OnMouseMove(nFlags, point, mOut, cred, args);
	ThreadRelease();
}

/**
 * Method: TWindow::OnLButtonDblClk
 * Purpose: Manages the Double Click Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TWindow::OnLButtonDblClk(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (locked)
	{
		ThreadRelease();
		return;
	}
	message_output mOut = message_output::mo_negative;

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;
	for(UINT c = 0; c < pages.Size() && (mOut == message_output::mo_negative); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDblClk(nFlags, point, mOut, args);
	}

	if(mOut == message_output::mo_negative)
		mainPage->OnLButtonDblClk(nFlags, point, mOut, args);
	ThreadRelease();
}

/**
 * Method: TWindow::OnLButtonUp
 * Purpose: Manages the Left Button Up Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TWindow::OnLButtonUp(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (currentScrollBar.Get())
		currentScrollBar.Nullify();


	if (locked)
	{
		ThreadRelease();
		return;
	}
	message_output mOut = message_output::mo_negative;

	auto fly = flyout;
	flyout.Nullify();
	UINT c = 0;

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;
	for(c = 0; c < pages.Size() && (mOut == message_output::mo_negative); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonUp(nFlags, point, mOut, cred, args);
	}

	if(mOut == message_output::mo_negative)
		mainPage->OnLButtonUp(nFlags, point, mOut, cred, args);
	ThreadRelease();
}

/**
 * Method: TWindow::OnChar
 * Purpose: Manages the Char Message
 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
 *				UINT nChar - the Character provided
 *				UINT nRepCnt - the number of times to add it
 *				UINT nFlags - flags associated with the message
 * Returns: bool
 */
bool TWindow::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return false;
}


/**
 * Method: TWindow::OnWindowResize
 * Purpose: Resizes the Window
 * Parameters: UINT width - the new width of the window
 *				UINT height - the new height of the Window
 * Returns: void
 */
void TWindow::OnWindowResize(UINT width, UINT height)
{
	ThreadLock();
	D2D1_RECT_F newLoc;
	newLoc.top = newLoc.left = 0;
	newLoc.bottom = height;
	newLoc.right = width;
	size = ConvertD2D1RectToRect(newLoc);

	TDataArray<TPage::EventID_Cred> cred;
	TDataArray<EventArgs> args;
	if (mainPage.Get())
		mainPage->OnResize(newLoc, 0, cred, args);

	if (d3dEngine.Get())
	{
		if (drawingBoard.Get())
			drawingBoard->Prep3DResize();
		d3dEngine->Resize(width, height);
	}
	if (drawingBoard.Get())
	{
		drawingBoard->Resize(currentWindow, size, d3dEngine);
	}
	ThreadRelease();

}

/**
 * Method: TWindow::OnDestroy
 * Purpose: Reports whether it is ready for destruction of not
 * Parameters: void
 * Returns: bool - whether it is ready for destruction
 */
bool TWindow::OnDestroy()
{
	ThreadLock();
	bool ret = (mainPage.Get()) ? mainPage->OnDestroy() : true;
	ThreadRelease();
	return ret;
}

bool TWindow::OnScroll(const TPoint& point, const TPoint& direction)
{
	if(!mainPage.Get())
	return false;
	TDataArray<EventArgs> args;
	return mainPage->OnScroll(false, point, direction, args);
}

/**
 * Method: TWindow::LockWindow
 * Purpose: Locks the Window from handling Messages
 * Parameters: void
 * Returns: void
 */
void TWindow::LockWindow()
{
	ThreadLock();
	locked = true;
	ThreadRelease();
}

/**
 * Method: TWindow::UnlockWindow
 * Purpose: Unlocks the Window allowing it to handle messages
 * Parameters: void
 * Returns: void
 */
void TWindow::UnlockWindow()
{
	ThreadLock();
	locked = false;
	ThreadRelease();
}

/**
 * Method: TWindow::SetSelf
 * Purpose: Allows the Window to hold a reference to itself
 * Parameters: TrecPointer<TWindow> win - the self window to set to
 * Returns: void
 */
void TWindow::SetSelf(TrecPointer<TWindow> win)
{
	if (this != win.Get())
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TWindow>(win);
}


/**
 * Method: TWindow::GetInstance
 * Purpose: Retrievs the instance associated with this window
 * Parameters: void
 * Returns: TrecPointer<TInstance> - the Instance this window is under
 */
TrecPointer<TProcess> TWindow::GetInstance()
{
	ThreadLock();
	auto ret = TrecPointerKey::GetTrecPointerFromSoft<>(windowInstance);
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::SetUp3D
 * Purpose: Configures the Window to support 3D drawing (if it doesn't already)
 * Parameters: void
 * Returns: whether the window is now set for 3D
 */
bool TWindow::SetUp3D()
{
	ThreadLock();
	if (d3dEngine.Get())
	{
		ThreadRelease();
		return true;
	}
	assert(windowInstance.Get());

	d3dEngine = TrecPointerKey::GetNewTrecPointer<TWindowEngine>(currentWindow, TrecPointerKey::GetTrecPointerFromSoft<>(windowInstance)->GetInstanceHandle());

	if (d3dEngine->Initialize() || !drawingBoard.Get())
	{
		d3dEngine.Delete();
		ThreadRelease();
		return false;
	}
	drawingBoard->Resize(currentWindow, size, d3dEngine);
	ThreadRelease();

	return true;
}

/**
 * Method: TWindow::CleanUp
 * Purpose: Deletes the Page marked for deletion
 * Parameters: void
 * Returns: void
 */
void TWindow::CleanUp()
{
	//ThreadLock();
	//deletePage.Delete();
	//ThreadRelease();
}

/**
 * Method: TWindow::GetWindowEngine
 * Purpose: Retirves the 3D Engine associated with this window
 * Parameters: void
 * Returns: TrecPointer<TWindowEngine> - the 3D window engine
 */
TrecPointer<TWindowEngine> TWindow::GetWindowEngine()
{
	ThreadLock();
	auto ret = d3dEngine;
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::GetNewArenaEngine
 * Purpose: Retirves a new Arena Engine
 * Parameters: void
 * Returns: TrecPointer<TArenaEngine> - the Arena Engine produced
 */
TrecPointer<TArenaEngine> TWindow::GetNewArenaEngine()
{
	TrecPointer<TArenaEngine> ret;
	ThreadLock();
	if (!d3dEngine.Get())
	{
		ThreadRelease();
		return ret;
	}
	ret = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine);
	engines.push_back(ret);
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::GetNewArenaEngine
 * Purpose: Retirves a new Arena Engine by name
 * Parameters: TString& name - the name of the Arena Engine
 * Returns: TrecPointer<TArenaEngine> - the Arena Engine produced
 */
TrecPointer<TArenaEngine> TWindow::GetNewArenaEngine(TString& name)
{
	TrecPointer<TArenaEngine> ret;
	ThreadLock();
	if (!d3dEngine.Get())
	{
		ThreadRelease();
		return ret;
	}
	ret = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine, name);
	engines.push_back(ret);
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::GetDrawingBoard
 * Purpose: Retrieves the drawing board associated with this window
 * Parameters: void
 * Returns: TrecPointer<DrawingBoard> - the drawing board held by the WIndow
 */
TrecPointer<DrawingBoard> TWindow::GetDrawingBoard()
{
	ThreadLock();
	auto ret = drawingBoard;
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::PrepAnimations
 * Purpose: Prepares animations held by the provided page
 * Parameters: TrecPointer<Page> page - the page to prepare the animations for
 * Returns: bool - whether the page was set and the Window matched
 */
bool TWindow::PrepAnimations(TrecPointer<TPage> page)
{
	//ThreadLock();
	//if (!page.Get() || page->GetWindowHandle().Get() != this)
	//{
	//	ThreadRelease();
	//	return false;
	//}
	//page->PrepAnimations(animationCentral);
	//animationCentral.StartBegin();
	//animationCentral.StartNewPersistant();
	//ThreadRelease();
	return true;
}

/**
 * Method:: TWindow::SetFlyout
 * Pupose: Allows Flyouts in an Anaface to be set
 * Parameters: TrecPointer<TFlyout> fly - the flyout to Draw
 * Returns: void
 *
 * Note: This method is intended to be called by the Page Class. The Page looks for TFlyouts to draw and the Window takes it from there
 */
void TWindow::SetFlyout(TrecPointer<TPage> fly)
{
	ThreadLock();
	flyout = fly;
	ThreadRelease();
}


/**
 * Method: TWindow::GetWindowHandle
 * Purpose: Returns the Window handle
 * Parameters: void
 * Returns: HWND - the handle to the Window object in the OS
 */
HWND TWindow::GetWindowHandle()
{
	ThreadLock();
	HWND ret = currentWindow;
	ThreadRelease();
	return ret;
}

/**
 * Method: TWindow::GetFactory
 * Purpose: Returns the Direct2D Factory used by the window
 * Parameters: void
 * Returns: TrecComPointer<ID2D1Factory1> -  the Pointer to the factory
 */
TrecComPointer<ID2D1Factory1> TWindow::GetFactory()
{
	ThreadLock();
	auto ret = directFactory;
	ThreadRelease();
	return ret;
}


HDC TWindow::GetTWindowDc()
{
	ThreadLock();
	HDC ret = (d3dEngine.Get()) ? d3dEngine->GetDC() : GetDC(currentWindow);
	ThreadRelease();
	return ret;
}

void TWindow::FlushDc()
{
	ThreadLock();
	if (d3dEngine.Get())
		d3dEngine->ClearDC();
	ThreadRelease();
}

/**
 * Method: TWindow::DrawOtherPages
 * Purpose: Draws other pages registered in this window
 * Parameters: void
 * Returns: void
 */
void TWindow::DrawOtherPages()
{
}

TProcess::~TProcess()
{
}
