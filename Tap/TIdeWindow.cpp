#include "TIdeWindow.h"
#include "IDEPage.h"
#include "TInstance.h"

#include <TFileShell.h>
#include <DirectoryInterface.h>
#include "TerminalHandler.h"
#include "TCodeHandler.h"
#include "MiniApp.h"
#include "FileHandler.h"
#include <atltrace.h>

/**
 * Method: TIdeWindow::TIdeWindow
 * Purpose: Constructor
 * Parameters: TString& name - the name of the WIndow
 *				TString& winClass - the name to tell Windows to use for this Window type
 *				UINT style - the Style of window to use
 *				HWND parent - the Handle to the Parent Window (if any)
 *				int commandShow - How to show the Window (provided in WinMain)
 *				TrecPointer ins - pointer to the TInstance involved (hence why TInstance has a SetSelf method)
 *				UINT mainViewSpace - how much drawing space to devote to the main Page
 *				UINT pageBarSpace - how much tab space to allow the tab bars in the IDE-Pages
 * Returns: New IDE Window
 */
TIdeWindow::TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, 
	TrecPointer<TInstance> ins, UINT mainViewSpace, UINT pageBarSpace):
	TWindow(name, winClass, style | WS_MAXIMIZE, parent, commandShow, ins)
{
	this->mainViewSpace = mainViewSpace;
	this->pageBarSpace = pageBarSpace;
}

TIdeWindow::~TIdeWindow()
{
	//body.Delete();

}

/**
 * Method: TIdeWindow::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TIdeWindow::GetType()
{
	return TString(L"TIdeWindow;") + TWindow::GetType();
}

/**
 * Method: TIdeWindow::PrepareWindow
 * Purpose: Prepares the Window while also managing the links between the IDE-Pages
 * Parameters: void
 * Returns: int - error code (0 = success)
 */
int TIdeWindow::PrepareWindow()
{
	//body = TrecPointerKey::GetNewSelfTrecPointerAlt<Page, IDEPage>(ide_page_type_body, pageBarSpace);

	ThreadLock();
	TrecSubPointer<Page, IDEPage>* pages[] = {
		&body,
		&basicConsole,
		&deepConsole,
		&upperRight,
		&lowerRight,
		&upperLeft,
		&lowerLeft
	};

	for (UINT c = 0; c < ARRAYSIZE(pages); c++)
	{
		*pages[c] = TrecPointerKey::GetNewSelfTrecSubPointer<Page, IDEPage>(static_cast<ide_page_type>(c), pageBarSpace, drawingBoard);
		this->pages.push_back(TrecPointerKey::GetTrecPointerFromSub<Page, IDEPage>(*pages[c]));

		(*pages[c])->parentWindow = TrecPointerKey::GetSoftSubPointerFromSoft<TWindow, TIdeWindow>(self);
	}

	for (UINT c = 0; c < ARRAYSIZE(pages); c++)
	{
		for (UINT rust = 0; rust < ARRAYSIZE(pages); rust++)
		{
			if (c != rust)
				dynamic_cast<IDEPage*>(pages[c]->Get())->SetLink(*pages[rust], static_cast<ide_page_type>(rust));
		}
	}
	int ret = TWindow::PrepareWindow();

	ThreadRelease();
	return ret;
}

void TIdeWindow::OnWindowResize(UINT width, UINT height)
{
	ThreadLock();
	if (!mainPage.Get())
	{
		ThreadRelease();
		return;
	}
	size.top = size.left = 0;
	size.right = width;
	size.left = height;

	D2D1_RECT_F curArea = convertRECTToD2DRectF(size);
	curArea.bottom = curArea.top + this->mainViewSpace;

	mainPage->OnResize(curArea, 0, d3dEngine);


	curArea.top += this->mainViewSpace;


	D2D1_RECT_F left = curArea;
	D2D1_RECT_F middle = curArea;
	D2D1_RECT_F right = curArea;
	D2D1_RECT_F bottom = curArea;


	left.right = width / 5;
	right.left = width - (width / 5);
	middle.left = left.right;
	middle.right = right.left;

	bottom.top = height * 2 / 3 + mainViewSpace;

	left.bottom = right.bottom = middle.bottom = bottom.top;

	D2D1_RECT_F zeroRec{ 0,0,0,0 };

	body->OnResize(middle, 0, d3dEngine);
	upperLeft->OnResize(left, 0, d3dEngine);
	upperRight->OnResize(right, 0, d3dEngine);
	basicConsole->OnResize(zeroRec, 0, d3dEngine);
	lowerLeft->OnResize(zeroRec, 0, d3dEngine);
	lowerRight->OnResize(zeroRec, 0, d3dEngine);
	deepConsole->OnResize(bottom, 0, d3dEngine);

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

/**
 * Method: TIdeWindow::OnLButtonUp
 * Purpose: Manages the Left Button Up Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TIdeWindow::OnLButtonUp(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (currentHolder.Get())
	{
		TrecSubPointer<Page, IDEPage> recievedPage;

		if (body->TookTab(currentHolder))
		{
			recievedPage = body;
			goto doneCheckingTab;
		}
		if (basicConsole->TookTab(currentHolder))
		{
			recievedPage = basicConsole;
			goto doneCheckingTab;
		}
		if (upperLeft->TookTab(currentHolder))
		{
			recievedPage = upperLeft;
			goto doneCheckingTab;
		}
		if (upperRight->TookTab(currentHolder))
		{
			recievedPage = upperRight;
			goto doneCheckingTab;
		}
		if (lowerLeft->TookTab(currentHolder))
		{
			recievedPage = lowerLeft;
			goto doneCheckingTab;
		}
		if (lowerRight->TookTab(currentHolder))
		{
			recievedPage = lowerRight;
			goto doneCheckingTab;
		}
		if (deepConsole->TookTab(currentHolder))
		{
			recievedPage = deepConsole;
			goto doneCheckingTab;
		}
		if (basicConsole->TookTab(currentHolder))
		{
			recievedPage = basicConsole;
			goto doneCheckingTab;
		}
	doneCheckingTab:

		if (!recievedPage.Get() && tabPage.Get() && tabPage->TookTab(currentHolder))
		{
			recievedPage = tabPage;
		}

		if (recievedPage.Get() && recievedPage.Get() != tabPage.Get() && tabPage.Get())
		{
			tabPage->RemovePage(currentHolder);
		}

		if (recievedPage.Get())
		{
			tabPage.Nullify();
			currentHolder.Nullify();
		}

	}

	TWindow::OnLButtonUp(nFlags, point);
	focusPage.Nullify();
	ThreadRelease();
}

/**
 * Method: TIdeWindow::OnMouseMove
 * Purpose: Manages the Mouse Move Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that the mouse is now
 * Returns: void
 */
void TIdeWindow::OnMouseMove(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (locked) {
		ThreadRelease();
		return;
	}
	messageOutput output = messageOutput::negative;
	if (currentScrollBar.Get())
	{
		currentScrollBar->OnMouseMove(nFlags, point, &output);
		
		Draw();
		ThreadRelease();
		return;
	}

	if (focusPage.Get())
	{
		focusPage.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, mainPage->GetArea()))
	{
		mainPage->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, body->GetArea()))
	{
		body.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, upperLeft->GetArea()))
	{
		upperLeft.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, upperRight->GetArea()))
	{
		upperRight.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, lowerLeft->GetArea()))
	{
		lowerLeft.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, lowerRight->GetArea()))
	{
		lowerRight.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, basicConsole->GetArea()))
	{
		basicConsole.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, deepConsole->GetArea()))
	{
		deepConsole.GetBase()->OnMouseMove(nFlags, point, &output, flyout);
		goto finish;
	}
	

finish:

	if (currentHolder.Get())
	{
		currentHolder->MovePoint(point.x, point.y);
	}


	if (output == messageOutput::positiveContinueUpdate || output == messageOutput::positiveOverrideUpdate || output == messageOutput::negativeUpdate)
		Draw();
	ThreadRelease();
}

/**
 * Method: TIdeWindow::OnLButtonDown
 * Purpose: Manages the Left Button Down Message
 * Parameters: UINT nFlags - the flags associated with the message
 *				TPoint point - the point that was clicked
 * Returns: void
 */
void TIdeWindow::OnLButtonDown(UINT nFlags, TPoint point)
{
	ThreadLock();
	if (locked) {
		ThreadRelease(); 
		return;
	}
	messageOutput output = messageOutput::negative;


	auto curFocusPage = focusPage;

	if (isContained(point, mainPage->GetArea()))
	{
		mainPage->OnLButtonDown(nFlags, point, &output, flyout);
		goto finish;
	}

	if (isContained(point, body->GetArea()))
	{
		body.GetBase()->OnLButtonDown(nFlags, point, &output, flyout);
		focusPage = body;
		goto finish;
	}

	if (isContained(point, upperLeft->GetArea()))
	{
		upperLeft.GetBase()->OnLButtonDown(nFlags, point, &output, flyout);
		focusPage = upperLeft;
		goto finish;
	}

	if (isContained(point, upperRight->GetArea()))
	{
		upperRight.GetBase()->OnLButtonDown(nFlags, point, &output, flyout);
		focusPage = upperRight;
		goto finish;
	}

	if (isContained(point, lowerLeft->GetArea()))
	{
		lowerLeft.GetBase()->OnLButtonDown(nFlags, point, &output, flyout);
		focusPage = lowerLeft;
		goto finish;
	}

	if (isContained(point, lowerRight->GetArea()))
	{
		lowerRight.GetBase()->OnLButtonDown(nFlags, point, &output, flyout);
		focusPage = lowerRight;
		goto finish;
	}

	if (isContained(point, basicConsole->GetArea()))
	{
		basicConsole.GetBase()->OnLButtonDown(nFlags, point, &output, flyout);
		focusPage = basicConsole;
		goto finish;
	}

	if (isContained(point, deepConsole->GetArea()))
	{
		deepConsole.GetBase()->OnLButtonDown(nFlags, point, &output, flyout);
		focusPage = deepConsole;
		goto finish;
	}

finish:
	
	if (output == messageOutput::positiveContinueUpdate || output == messageOutput::positiveOverrideUpdate || output == messageOutput::negativeUpdate)
		Draw();
	ThreadRelease();
}

/**
 * Method: TIdeWindow::AddNewMiniApp
 * Purpose: Adds a MiniApp to the list of Apps being managed
 * Parameters: TrecPointer<MiniApp> app - the App to add
 * Returns: void
 */
void TIdeWindow::AddNewMiniApp(TrecPointer<MiniApp> app)
{
	ThreadLock();
	if (app.Get())
	{
		apps.push_back(app);
	}
	ThreadRelease();
}

/**
 * Method: TIdeWindow::AddNewPage
 * Purpose: Creates a new Page and returns it to the caller
 * Parameters: anagame_page pageType - type of built-in page
 *				ide_page_type pageLoc - the location to place the new Page
 *				TString name - name of the page to write on the Tab
 *				TString tmlLoc - the File location of the TML file (if the page type is built-in, this can be empty)
 *				TrecPointer<EventHandler> handler - the Handler to provide the Page
 *				bool pageTypeStrict - whether the caller is strict when it comes to the location of the Page
 * Returns: TrecSubPointer<Page, IDEPage> -  the Page generated
 */
TrecPointer<Page> TIdeWindow::AddNewPage(anagame_page pageType, ide_page_type pageLoc, TString name, TString tmlLoc, TrecPointer<EventHandler> handler, bool pageTypeStrict)
{
	ThreadLock();
	if (!mainPage.Get())
	{
		ThreadRelease();
		return TrecPointer<Page>();
	}
	TrecPointer<EventHandler> pageHandler;
	TrecPointer<TFile> uiFile = TrecPointerKey::GetNewTrecPointer<TFile>();
	TrecPointer<TFileShell> fileShell;

	handler_data_source dataSource = handler_data_source::hds_files;

	switch (pageType)
	{
	case anagame_page::anagame_page_code_explorer:

		break;
	case anagame_page::anagame_page_code_file:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\LineTextEditor.txt", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		if (!handler.Get())
			pageHandler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, TCodeHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance));
		else
			pageHandler = handler;
		break;
	case anagame_page::anagame_page_command_prompt:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\IDEPrompt.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		if (!handler.Get())
			pageHandler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, TerminalHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance));
		else
			pageHandler = handler;
		break;
	case anagame_page::anagame_page_console:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\IDEPromptProgram.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		if (!handler.Get())
			pageHandler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, TerminalHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance));
		else
			pageHandler = handler;
		break;
	case anagame_page::anagame_page_project_explorer:
		dataSource = handler_data_source::hds_project;
	case anagame_page::anagame_page_file_node:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\FileBrowser.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		if (!handler.Get())
			pageHandler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, FileHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), dataSource);
		else
			pageHandler = handler;
		break;
	

		break;
	case anagame_page::anagame_page_object_explorer:

		break;
	case anagame_page::anagame_page_arena:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\ArenaView.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		pageHandler = handler;
		break;
	case anagame_page::anagame_page_camera:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\ArenaViewPanel.txt", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		pageHandler = handler;
		break;
	case anagame_page::anagame_page_custom:
		if (!handler.Get())
		{
			ThreadRelease();
			return TrecPointer<Page>();
		}
		pageHandler = handler;
		uiFile->Open(tmlLoc, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	}

	if (!uiFile->IsOpen() || !pageHandler.Get() || !name.GetSize())
	{
		ThreadRelease();
		return TrecPointer<Page>();
	}

	TrecSubPointer<Page, IDEPage> targetPage = body;
	switch (pageLoc)
	{
	case ide_page_type::ide_page_type_basic_console:
		targetPage = basicConsole;
		break;
	case ide_page_type::ide_page_type_deep_console:
		targetPage = deepConsole;
		break;
	case ide_page_type::ide_page_type_lower_left:
		targetPage = lowerLeft;
		break;
	case ide_page_type::ide_page_type_lower_right:
		targetPage = lowerRight;
		break;
	case ide_page_type::ide_page_type_upper_left:
		targetPage = upperLeft;
		break;
	case ide_page_type::ide_page_type_upper_right:
		targetPage = upperRight;
	}

	TrecPointer<Page> newPage = targetPage->AddNewPage(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), name, pageHandler);

	newPage->SetAnaface(uiFile, pageHandler);

	uiFile->Close();

	newPage->SetHandler(pageHandler);

	targetPage->currentPage = newPage;

	ThreadRelease();
	return newPage;
}

/**
 * Method: TIdeWindow::AddPage
 * Purpose: Creates a built-in Page
 * Parameters: anagame_page pageType - type of built-in page
 *				ide_page_type pageLoc - the location to place the new Page
 *				TString name - name of the page to write on the Tab
 * Returns: TrecSubPointer<Page, IDEPage> -  the Page generated
 */
TrecPointer<Page> TIdeWindow::AddPage(anagame_page pageType, ide_page_type pageLoc, TString name)
{
	TrecPointer<Page> ret;
	ThreadLock();
	if (!this->windowInstance.Get())
	{
		ThreadRelease();
		return ret;
	}
	auto handler = TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance)->GetHandler(name, pageType);

	if (handler.Get())
	{
		ThreadRelease();
		return ret;
	}

	handler_data_source dataSource = handler_data_source::hds_files;

	switch (pageType)
	{
	case anagame_page::anagame_page_command_prompt:
	case anagame_page::anagame_page_console:
		ret = AddNewPage(pageType, pageLoc, name, TString(), TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, TerminalHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance)));
		break;
	case anagame_page::anagame_page_project_explorer:
		dataSource = handler_data_source::hds_project;
	case anagame_page::anagame_page_file_node:
		ret = AddNewPage(pageType, pageLoc, name, TString(), TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, FileHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), dataSource));
		break;
	}
	ThreadRelease();
	return ret;
}

/**
 * Method: TIdeWindow::CompileView
 * Purpose: Compiles the Main View while also preparing all of the individual IDE pages
 * Parameters: TString& file - path of the TML file holding the Anaface
 *				TrecPointer<EventHandler> eh - the Handler to the Main page
 * Returns: int - error (0 == success)
 */
int TIdeWindow::CompileView(TString& file, TrecPointer<EventHandler> eh)
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
	directFactory = TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance)->GetFactory();

	mainPage = Page::GetWindowPage(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), eh);

	D2D1_RECT_F curArea = mainPage->GetArea();

	drawingBoard->Resize(currentWindow, convertD2DRectToRECT( curArea), d3dEngine);

	curArea.bottom = curArea.top + this->mainViewSpace;

	mainPage->SetArea(curArea);

	if (!mainPage.Get())
	{
		ThreadRelease();
		return 2;
	}
	mainPage->SetAnaface(aFile, eh);
	;
	GetClientRect(GetWindowHandle(), &size);
	curArea = convertRECTToD2DRectF(size);
	curArea.top += this->mainViewSpace;


	D2D1_RECT_F left = curArea;
	D2D1_RECT_F middle = curArea;
	D2D1_RECT_F right = curArea;
	D2D1_RECT_F bottom = curArea;

	int width = curArea.right - curArea.left;
	int height = curArea.bottom - curArea.top;

	left.right = width / 5;
	right.left = width - (width / 5);
	middle.left = left.right;
	middle.right = right.left;

	bottom.top = height * 2 / 3 + mainViewSpace;

	left.bottom = right.bottom = middle.bottom = bottom.top;

	body->SetArea(middle);
	upperLeft->SetArea(left);
	upperRight->SetArea(right);
	basicConsole->SetArea({ 0,0,0,0 });
	lowerLeft->SetArea({ 0,0,0,0 });
	lowerRight->SetArea({ 0,0,0,0 });
	deepConsole->SetArea(bottom);

	panelbrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::BlueViolet));

	body->SetResources(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	upperLeft->SetResources(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	upperRight->SetResources(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	basicConsole->SetResources(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	lowerLeft->SetResources(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	lowerRight->SetResources(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	deepConsole->SetResources(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(windowInstance), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	safeToDraw = 1;
	Draw();

	ThreadRelease();
	return 0;
}

/**
 * Method: TIdeWindow::SetCurrentHolder
 * Purpose: Marks a Page Holder as being dragged by the User
 * Parameters: TrecPointer<Tab> holder - the Page holder believed to be dragged
 * Returns: void
 */
void TIdeWindow::SetCurrentHolder(TrecPointer<Tab> holder, TrecPointer<Page> page)
{
	ThreadLock();
	currentHolder = holder;
	tabPage = TrecPointerKey::GetTrecSubPointerFromTrec<Page, IDEPage>(page);
	ThreadRelease();
}

/**
 * Method: TIdeWindow::SetEnvironment
 * Purpose: Sets the Environment of the Window
 * Parameters: TrecPointer<TEnvironment> env - the Environment to manage
 * Returns: void
 */
void TIdeWindow::SetEnvironment(TrecPointer<TEnvironment> env)
{
	ThreadLock();
	environment = env;
	ThreadRelease();
}


TrecPointer<TEnvironment> TIdeWindow::GetEnvironment()
{
	ThreadLock();
	auto ret = environment;
	ThreadRelease();
	return ret;
}

/**
 * Method: TIdeWindow::GetEnvironmentDirectory
 * Purpose: Retrievs the Workign Directory of the TEnvironment
 * Parameters: void
 * Returns: TrecPointer<TFileShell> - the Environment's working directory (could be NULL)
 */
TrecPointer<TFileShell> TIdeWindow::GetEnvironmentDirectory()
{
	ThreadLock();
	auto ret = (environment.Get()) ? environment->GetRootDirectory() : TrecPointer<TFileShell>();
	ThreadRelease();
	return ret;
}

/**
 * Method: TIdeWindow::SaveAll
 * Purpose: Sends the Save signal to all MiniAPps registered
 * Parameters: void
 * Returns: void
 */
void TIdeWindow::SaveAll()
{
	ThreadLock();
	for (UINT Rust = 0; Rust < apps.Size(); Rust++)
	{
		if (apps[Rust].Get())
			apps[Rust]->OnSave();
	}
	ThreadRelease();
}

/**
 * Method: TIdeWindow::SaveCurrent
 * Purpose: Sends the Save signal to the current MiniApp, if set
 * Parameters: void
 * Returns: void
 */
void TIdeWindow::SaveCurrent()
{
	ThreadLock();
	if (currentApp.Get())
		currentApp->OnSave();
	ThreadRelease();
}

/**
 * Method: TIdeWindow::SetCurrentApp
 * Purpose: Sets the focus to the provided MiniApp
 * Parameters: TrecPointer<MiniApp> app - the MiniApp to focus on
 * Returns: void
 */
void TIdeWindow::SetCurrentApp(TrecPointer<MiniApp> app)
{
	ThreadLock();
	if (app.Get())
	{
		if (currentApp.Get() == app.Get())
		{
			ThreadRelease();
			return;
		}
		currentApp = app;
		bool addApp = true;
		for (UINT Rust = 0; Rust < apps.Size(); Rust++)
		{
			if (app.Get() == apps[Rust].Get())
			{
				addApp = false;
				break;
			}
		}

		if (addApp)
			apps.push_back(app);

		auto hand = app->GetMainHandler();
		if (hand.Get())
			hand->OnFocus();
	}
	ThreadRelease();
}

/**
 * Method: TIdeWindow::OpenFile
 * Purpose: Allows a File Handler to command the Window to open a new file (will currently just log the fileto open for now)
 * Parameters: TrecPointer<TFileShell> - representation of the file to open
 * Returns: UINT - error code (0 for success)
 */
UINT TIdeWindow::OpenFile(TrecPointer<TFileShell> shell)
{
	ThreadLock();
	if(!shell.Get())
	{
		ATLTRACE(L"Null object submitted!\n");
		return 1;
	}
	TString format(L"File to open: %ls \n");
	TString printOut;
	printOut.Format(format, shell->GetPath().GetConstantBuffer().getBuffer());

	ATLTRACE(printOut.GetConstantBuffer().getBuffer());
	ThreadRelease();
}

/**
 * Method: TIdeWindow::DrawOtherPages
 * Purpose: Draws the other page it has set up
 * Parameters: void
 * Returns: void
 */
void TIdeWindow::DrawOtherPages()
{
	ThreadLock();
	if (body.Get())dynamic_cast<IDEPage*>(body.Get())->Draw(panelbrush, d3dEngine.Get());
	if (basicConsole.Get())dynamic_cast<IDEPage*>(basicConsole.Get())->Draw(panelbrush, d3dEngine.Get());
	if (deepConsole.Get())dynamic_cast<IDEPage*>(deepConsole.Get())->Draw(panelbrush, d3dEngine.Get());
	if (upperLeft.Get())dynamic_cast<IDEPage*>(upperLeft.Get())->Draw(panelbrush, d3dEngine.Get());
	if (upperRight.Get())dynamic_cast<IDEPage*>(upperRight.Get())->Draw(panelbrush, d3dEngine.Get());
	if (lowerLeft.Get()) dynamic_cast<IDEPage*>(lowerLeft.Get())->Draw(panelbrush, d3dEngine.Get());
	if (lowerRight.Get())dynamic_cast<IDEPage*>(lowerRight.Get())->Draw(panelbrush, d3dEngine.Get());


	if (currentHolder.Get())
		currentHolder->Draw();
	ThreadRelease();
}
