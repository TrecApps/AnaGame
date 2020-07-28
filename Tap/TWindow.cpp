#include "TWindow.h"
#include "TInstance.h"

bool IsD2D1RectEqual(const D2D1_RECT_F& r1, const  D2D1_RECT_F& r2, float difference)
{
	difference = abs(difference);
	return (abs(r1.bottom - r2.bottom) <= difference) &&
		(abs(r1.left - r2.left) <= difference) &&
		(abs(r1.right - r2.right) <= difference) &&
		(abs(r1.top - r2.top) <= difference);
}


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
TWindow::TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins)
{
	currentWindow = CreateWindowW(winClass.GetConstantBuffer(),
		name.GetConstantBuffer(), style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, nullptr, ins->GetInstanceHandle(), nullptr);

	if (!currentWindow)
	{
		int lastError = GetLastError();
		throw L"Window ERROR! Failed to generate Window";
	}
	this->name.Set(name);
	this->parent = parent;
	this->winClass.Set(winClass);
	this->windowInstance = TrecPointerKey::GetSoftPointerFromTrec<TInstance>( ins);
	this->command = commandShow;

	HDC dc = GetWindowDC(currentWindow);
	SetMapMode(dc, MM_HIENGLISH);

	locked = false;
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

}

/**
 * Method: TWindow::PrepareWindow
 * Purpose: Sets the Window up for viewing
 * Parameters: void
 * Returns: int - error code (0 = success)
 */
int TWindow::PrepareWindow()
{
	if (!currentWindow)
		return 1;

	assert(windowInstance.Get());

	TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance)->RegisterDialog(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));

	ShowWindow(currentWindow, command);
	UpdateWindow(currentWindow);

	animationCentral.SetWindow(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));

	return 0;
}

/**
 * Method: TWindow::CompileView
 * Purpose: Compiles the Main View
 * Parameters: TString& file - path of the TML file holding the Anaface
 *				TrecPointer<EventHandler> eh - the Handler to the Main page
 * Returns: int - error (0 == success)
 */
int TWindow::CompileView(TString& file, TrecPointer<EventHandler> eh)
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

	mainPage->SetAnaface(aFile, eh);

	mainPage->PrepAnimations(animationCentral);

	animationCentral.StartBegin();
	animationCentral.StartNewPersistant();
	safeToDraw = 1;
	Draw();

	return 0;
}

/**
 * Method: TWindow::MovePageToTop
 * Purpose:
 * Parameters:
 * Returns:
 *
 * DEPRECATED -
 */
bool TWindow::MovePageToTop(TrecPointer<Page> p)
{
	if (!p.Get())
		return false;

	if (!pages.Size())
		return false;

	if (pages[0].Get() == p.Get())
		return true;

	UINT loc = 0;
	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
	{
		if (pages[Rust].Get() == p.Get())
		{
			loc = Rust;
			break;
		}
	}
	if (!loc)
		return false;

	for (int assign = loc; assign - 1 > -1; assign--)
	{
		pages[assign] = pages[assign - 1];
	}
	pages[0] = p;
	return true;
}

/**
 * Method: TWindow::GetWinClass
 * Purpose: Retrievs the Window-level class of this window
 * Parameters: void
 * Returns: TString - the class name registered with Windows
 */
TString TWindow::GetWinClass()
{
	return winClass;
}

/**
 * Method: TWindow::GetWinName
 * Purpose: Retrievs the Window-level name of this window
 * Parameters: void
 * Returns: TString - the name registered with Windows
 */
TString TWindow::GetWinName()
{
	return name;
}

/**
 * Method: TWindow::Draw
 * Purpose: Draws the window
 * Parameters: void
 * Returns: void
 */
void TWindow::Draw()
{
	if (mainPage.Get() && safeToDraw)
	{
		UCHAR tempSafe = safeToDraw;
		safeToDraw = 0;
		TrecComPointer<ID2D1RenderTarget> rt = drawingBoard->GetRenderer();
		
		TWindowEngine* d3d = d3dEngine.Get();
		if (!rt.Get()) return;

		TrecComPointer<ID2D1GdiInteropRenderTarget> gdi = drawingBoard->GetGdiRenderer();
		if (gdi.Get()  && d3d)
		{
			rt->BeginDraw();
			rt->Clear(D2D1::ColorF(D2D1::ColorF::White));
			d3d->PrepareScene(D2D1::ColorF(D2D1::ColorF::Wheat));

			mainPage->Draw();
			DrawOtherPages();

			HDC contDC = 0;
			
			if (SUCCEEDED(gdi->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC)))
			{
				HDC windDC = GetDC(currentWindow);
				SelectObject(windDC, GetStockObject(DC_BRUSH));
				SetDCBrushColor(windDC, RGB(0, 0, 0));

				
				D2D1_RECT_F loc = mainPage->GetArea();

				int width = loc.right - loc.left, height = loc.bottom - loc.top;

				//InvertRect(contDC, &loc);
				int err = 0;
				if (!BitBlt(windDC, 0, 0, width, height, contDC, 0, 0, SRCCOPY))
					err = GetLastError();
				gdi->ReleaseDC(nullptr);
				d3d->FinalizeScene();
				
			}
			if (flyout.Get())
				flyout->AfterDraw();

			rt->EndDraw();
		}
		else
		{
			rt->BeginDraw();
			rt->Clear(D2D1::ColorF(D2D1::ColorF::White));
			mainPage->Draw(d3d);
			DrawOtherPages();

			if (flyout.Get())
				flyout->AfterDraw();
			rt->EndDraw();
		}
		safeToDraw = tempSafe;
	}

}

/**
 * Method: TWindow::Draw
 * Purpose:
 * Parameters:
 * Returns:
 *
 * DEPRICATED
 */
void TWindow::Draw(Page& draw)
{
	draw.Draw();
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

/**
 * Method: TWindow::OnRButtonUp
 * Purpose: Manages the Right Button Up Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TWindow::OnRButtonUp(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = messageOutput::negative;
	for(UINT c = 0; c < pages.Size() && (mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonUp(nFlags, point, &mOut);
	}

	if(mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate)
		mainPage->OnRButtonUp(nFlags, point, &mOut);
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
	if (locked) return;
	messageOutput mOut = messageOutput::negative;

	for(UINT c = 0; c < pages.Size() && (mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDown(nFlags, point, &mOut, flyout);
	}

	if(mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate)
		mainPage->OnLButtonDown(nFlags, point, &mOut, flyout);
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
	if (locked) return;
	messageOutput mOut = messageOutput::negative;
	for(UINT c = 0; c < pages.Size() && (mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonDown(nFlags, point, &mOut);
	}

	if(mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate)
		mainPage->OnRButtonDown(nFlags, point, &mOut);
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
	if (locked) return;
	messageOutput mOut = messageOutput::negative;
	if (currentScrollBar.Get())
	{

		currentScrollBar->OnMouseMove(nFlags, point, &mOut);
		
		Draw();
		return;
	}



	for(UINT c = 0; c < pages.Size() && (mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnMouseMove(nFlags, point, &mOut, flyout);
	}

	if(mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate)
		mainPage->OnMouseMove(nFlags, point, &mOut, flyout);
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
	if (locked) return;
	messageOutput mOut = messageOutput::negative;
	for(UINT c = 0; c < pages.Size() && (mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDblClk(nFlags, point, &mOut);
	}

	if(mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate)
		mainPage->OnLButtonDblClk(nFlags, point, &mOut);
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
	if (currentScrollBar.Get())
		currentScrollBar.Nullify();


	if (locked) return;
	messageOutput mOut = messageOutput::negative;

	auto fly = flyout;
	flyout.Nullify();

	for(UINT c = 0; c < pages.Size() && (mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonUp(nFlags, point, &mOut, fly);
	}

	if(mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate)
		mainPage->OnLButtonUp(nFlags, point, &mOut, fly);
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
	if (locked) return false;
	messageOutput mOut = messageOutput::negative;

	bool returnable = false;

	for(UINT c = 0; c < pages.Size() && (mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate); c++)
	{
		if(pages[c].Get())
			returnable = pages[c]->OnChar(fromChar, nChar, nRepCnt, nFlags, &mOut);
	}

	if(mOut == messageOutput::negative || mOut == messageOutput::negativeUpdate)
		returnable = mainPage->OnChar(fromChar, nChar, nRepCnt, nFlags, &mOut);
	return returnable;
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
	if (d3dEngine.Get())
		d3dEngine->Resize();

	if(drawingBoard.Get())
		drawingBoard->Resize(this->currentWindow);
	
	if (!mainPage.Get())
		return;

	//safeToDraw = safeToDraw | 0b00000010;

	D2D1_RECT_F newLoc;
	newLoc.top = newLoc.left = 0;
	newLoc.bottom = height;
	newLoc.right = width;


	mainPage->OnResize(newLoc, 0, d3dEngine);

	//safeToDraw = safeToDraw & 0b11111101;
}

/**
 * Method: TWindow::OnDestroy
 * Purpose: Reports whether it is ready for destruction of not
 * Parameters: void
 * Returns: bool - whether it is ready for destruction
 */
bool TWindow::OnDestroy()
{
	if(mainPage.Get())
		return mainPage->OnDestroy();
	return true;
}

/**
 * Method: TWindow::GetHandlePage
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED in favor of the Ide Window/IDE Page
 */
TrecPointer<Page> TWindow::GetHandlePage(bool singleton)
{
	if (singleton && handlePage.Get())
		return handlePage;

	TrecPointer<Page> ret = Page::GetWindowPage(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), TrecPointer<EventHandler>());

	if (!ret.Get())
		return ret;

	pages.push_back(ret);

	if (singleton)
		handlePage = ret;

	return ret;
}

/**
 * Method: TWindow::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED in favor of the Ide Window/IDE Page
 */
TrecPointer<Page> TWindow::GetHandlePage(const TString& name)
{
	TrecPointer<Page> ret = keyPages.retrieveEntry(name);
	if (ret.Get())
		return ret;

	ret = GetHandlePage(false);

	if (ret.Get())
		keyPages.addEntry(name, ret);

	return ret;
}

/**
 * Method: TWindow::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED in favor of the Ide Window/IDE Page
 */
TrecPointer<Page> TWindow::Get3DPage(bool singleton, TString& engineId)
{
	if(!engineId.GetSize() && singleton)
		return _3DPage;



	if (!engineId.GetSize())
		return TrecPointer<Page>();

	if (!d3dEngine.Get())
	{
		if (!SetUp3D())return TrecPointer<Page>();
	}
	TrecPointer<TArenaEngine> engine = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine, engineId);//  ArenaEngine::GetArenaEngine(engineId, currentWindow, windowInstance->GetInstanceHandle());

	return Get3DPage(singleton, engine);
}

/**
 * Method: TWindow::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED in favor of the Ide Window/IDE Page
 */
TrecPointer<Page> TWindow::Get3DPage(bool singleton, TrecPointer<TArenaEngine> engine)
{
	if (singleton)
	{
		if (!engine.Get())
			return _3DPage;
		if (_3DPage.Get()) {
			if(_3DPage->GetArenaEngine().Get() == engine.Get())
				return _3DPage;
			return TrecPointer<Page>();
		}
	}

	//TrecPointer<Page> ret = Page::Get3DPage(windowInstance, engine, TrecPointer<EventHandler>());

	//pages.push_back(ret);

	//if (singleton)
	//{
		//_3DPage = ret;
	//}

	return TrecPointer<Page>();//ret;
}

/**
 * Method: TWindow::LockWindow
 * Purpose: Locks the Window from handling Messages
 * Parameters: void
 * Returns: void
 */
void TWindow::LockWindow()
{
	locked = true;
}

/**
 * Method: TWindow::UnlockWindow
 * Purpose: Unlocks the Window allowing it to handle messages
 * Parameters: void
 * Returns: void
 */
void TWindow::UnlockWindow()
{
	locked = false;
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
 * Method: TWindow::GetPageByArea
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED in favor of the Ide Window/IDE Page
 */
TrecPointer<Page> TWindow::GetPageByArea(D2D1_RECT_F r)
{
	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
	{
		if (pages[Rust].Get() && IsD2D1RectEqual(pages[Rust]->GetArea(), r, 1.0f))
			return pages[Rust];
	}
	TrecPointer<Page> ret = Page::GetSmallPage(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), r);
	pages.push_back(ret);
	return ret;
}

/**
 * Method: TWindow::GetInstance
 * Purpose: Retrievs the instance associated with this window
 * Parameters: void
 * Returns: TrecPointer<TInstance> - the Instance this window is under
 */
TrecPointer<TInstance> TWindow::GetInstance()
{
	return TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance);
}

/**
 * Method: TWindow::SetUp3D
 * Purpose: Configures the Window to support 3D drawing (if it doesn't already)
 * Parameters: void
 * Returns: whether the window is now set for 3D
 */
bool TWindow::SetUp3D()
{
	if (d3dEngine.Get())
		return true;
	assert(windowInstance.Get());

	d3dEngine = TrecPointerKey::GetNewTrecPointer<TWindowEngine>(currentWindow, TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance)->GetInstanceHandle());

	if (d3dEngine->Initialize() || !drawingBoard.Get())
	{
		d3dEngine.Delete();
		return false;
	}

	drawingBoard->Set3D(d3dEngine);

	if (drawingBoard->GetGdiRenderer().Get())
	{
		return true;
	}
	return false;
}

/**
 * Method: TWindow::CleanUp
 * Purpose: Deletes the Page marked for deletion
 * Parameters: void
 * Returns: void
 */
void TWindow::CleanUp()
{
	deletePage.Delete();
}

/**
 * Method: TWindow::GetWindowEngine
 * Purpose: Retirves the 3D Engine associated with this window
 * Parameters: void
 * Returns: TrecPointer<TWindowEngine> - the 3D window engine
 */
TrecPointer<TWindowEngine> TWindow::GetWindowEngine()
{
	return d3dEngine;
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
	if (!d3dEngine.Get())
		return ret;

	ret = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine);
	engines.push_back(ret);
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
	if (!d3dEngine.Get())
		return ret;
	
	ret = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine, name);
	engines.push_back(ret);
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
	return drawingBoard;
}

/**
 * Method: TWindow::PrepAnimations
 * Purpose: Prepares animations held by the provided page
 * Parameters: TrecPointer<Page> page - the page to prepare the animations for
 * Returns: bool - whether the page was set and the Window matched
 */
bool TWindow::PrepAnimations(TrecPointer<Page> page)
{
	if(!page.Get() || page->GetWindowHandle().Get() != this)
		return false;
	page->PrepAnimations(animationCentral);
	animationCentral.StartBegin();
	animationCentral.StartNewPersistant();
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
void TWindow::SetFlyout(TrecPointer<TFlyout> fly)
{
	flyout = fly;
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


