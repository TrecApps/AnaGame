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

/**
 * Method: TIdeWindow::PrepareWindow
 * Purpose: Prepares the Window while also managing the links between the IDE-Pages
 * Parameters: void
 * Returns: int - error code (0 = success)
 */
int TIdeWindow::PrepareWindow()
{
	//body = TrecPointerKey::GetNewSelfTrecPointerAlt<Page, IDEPage>(ide_page_type_body, pageBarSpace);
	
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


	return TWindow::PrepareWindow();
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
	if (dynamic_cast<IDEPage*>(body.Get())->OnLButtonUp(point) && currentHolder.Get())
	{
		if (focusPage.Get() && body.Get() != focusPage.Get())
		{
			focusPage->RemovePage(currentHolder);
		}
		body->AddNewPage(currentHolder);
		currentHolder.Nullify();
	}
	if (dynamic_cast<IDEPage*>(basicConsole.Get())->OnLButtonUp(point) && currentHolder.Get())
	{
		if (focusPage.Get() && basicConsole.Get() != focusPage.Get())
		{
			focusPage->RemovePage(currentHolder);
		}
		basicConsole->AddNewPage(currentHolder);
		currentHolder.Nullify();
	}
	if (dynamic_cast<IDEPage*>(deepConsole.Get())->OnLButtonUp(point) && currentHolder.Get())
	{
		if (focusPage.Get() && deepConsole.Get() != focusPage.Get())
		{
			focusPage->RemovePage(currentHolder);
		}
		deepConsole->AddNewPage(currentHolder);
		currentHolder.Nullify();
	}
	if (dynamic_cast<IDEPage*>(upperRight.Get())->OnLButtonUp(point) && currentHolder.Get())
	{  
		if (focusPage.Get() && upperRight.Get() != focusPage.Get())
		{
			focusPage->RemovePage(currentHolder);
		}
		upperRight->AddNewPage(currentHolder);
		currentHolder.Nullify();
	}
	if (dynamic_cast<IDEPage*>(lowerRight.Get())->OnLButtonUp(point) && currentHolder.Get())
	{
		if (focusPage.Get() && lowerRight.Get() != focusPage.Get())
		{
			focusPage->RemovePage(currentHolder);
		}
		lowerRight->AddNewPage(currentHolder);
		currentHolder.Nullify();
	}
	if (dynamic_cast<IDEPage*>(upperLeft.Get())->OnLButtonUp(point) && currentHolder.Get())
	{
		if (focusPage.Get() && upperLeft.Get() != focusPage.Get())
		{
			focusPage->RemovePage(currentHolder);
		}
		upperLeft->AddNewPage(currentHolder);
		currentHolder.Nullify();
	}
	if (dynamic_cast<IDEPage*>(lowerLeft.Get())->OnLButtonUp(point) && currentHolder.Get())
	{
		if (focusPage.Get() && lowerLeft.Get() != focusPage.Get())
		{
			focusPage->RemovePage(currentHolder);
		}
		lowerLeft->AddNewPage(currentHolder);
		currentHolder.Nullify();
	}
	if (focusPage.Get() && currentHolder.Get())
	{
		focusPage->AddNewPage(currentHolder);
	}
	currentHolder.Nullify();

	TWindow::OnLButtonUp(nFlags, point);
	focusPage.Nullify();

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
	if (locked) return;
	messageOutput output = messageOutput::negative;
	if (currentScrollBar.Get())
	{

		currentScrollBar->OnMouseMove(nFlags, point, &output);
		
		Draw();
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
		currentHolder->Move(point);
	}


	if (output == messageOutput::positiveContinueUpdate || output == messageOutput::positiveOverrideUpdate || output == messageOutput::negativeUpdate)
		Draw();
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
	if (locked) return;
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
}

/**
 * Method: TIdeWindow::AddNewMiniApp
 * Purpose: Adds a MiniApp to the list of Apps being managed
 * Parameters: TrecPointer<MiniApp> app - the App to add
 * Returns: void
 */
void TIdeWindow::AddNewMiniApp(TrecPointer<MiniApp> app)
{
	if (app.Get() && !app->Initialize())
	{
		apps.push_back(app);
	}
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
TrecSubPointer<Page, IDEPage> TIdeWindow::AddNewPage(anagame_page pageType, ide_page_type pageLoc, TString name, TString tmlLoc, TrecPointer<EventHandler> handler, bool pageTypeStrict)
{
	if (!mainPage.Get())
		return TrecSubPointer<Page, IDEPage>();

	TrecPointer<EventHandler> pageHandler;
	TrecPointer<TFile> uiFile = TrecPointerKey::GetNewTrecPointer<TFile>();
	TrecPointer<TFileShell> fileShell;


	switch (pageType)
	{
	case anagame_page::anagame_page_code_explorer:

		break;
	case anagame_page::anagame_page_code_file:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\LineTextEditor.txt", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		if (!handler.Get())
			pageHandler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, TCodeHandler>(windowInstance);
		else
			pageHandler = handler;
		break;
	case anagame_page::anagame_page_command_prompt:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\IDEPrompt.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		if (!handler.Get())
			pageHandler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, TerminalHandler>(windowInstance);
		else
			pageHandler = handler;
		break;
	case anagame_page::anagame_page_file_node:
		uiFile->Open(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\FileBrowser.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		if (!handler.Get())
			pageHandler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, FileHandler>(windowInstance);
		else
			pageHandler = handler;
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
			return TrecSubPointer<Page, IDEPage>();
		pageHandler = handler;
		uiFile->Open(tmlLoc, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	}

	if (!uiFile->IsOpen())
		return TrecSubPointer<Page, IDEPage>();

	if (!pageHandler.Get())
		return TrecSubPointer<Page, IDEPage>();

	if (!name.GetSize())
		return TrecSubPointer<Page, IDEPage>();


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

	TrecPointer<Page> newPage = targetPage->AddNewPage(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), name, pageHandler);

	newPage->SetAnaface(uiFile, pageHandler);

	uiFile->Close();

	newPage->SetHandler(pageHandler);
	if (pageHandler.Get())
		pageHandler->Initialize(newPage);

	return TrecPointerKey::GetTrecSubPointerFromTrec<Page, IDEPage>(newPage);
}

/**
 * Method: TIdeWindow::AddPage
 * Purpose: Creates a built-in Page
 * Parameters: anagame_page pageType - type of built-in page
 *				ide_page_type pageLoc - the location to place the new Page
 *				TString name - name of the page to write on the Tab
 * Returns: TrecSubPointer<Page, IDEPage> -  the Page generated
 */
TrecSubPointer<Page, IDEPage> TIdeWindow::AddPage(anagame_page pageType, ide_page_type pageLoc, TString name)
{
	TrecSubPointer<Page, IDEPage> ret;
	if (!this->windowInstance.Get())
		return ret;

	auto handler = windowInstance->GetHandler(name, pageType);



	if (handler.Get())
		ret = TrecPointerKey::GetTrecSubPointerFromTrec<Page, IDEPage>(handler->GetPage());

	if (ret.Get())
		return ret;

	switch (pageType)
	{
	case anagame_page::anagame_page_command_prompt:
		return AddNewPage(pageType, pageLoc, name, TString(), TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, TerminalHandler>(windowInstance));
	case anagame_page::anagame_page_file_node:
		return AddNewPage(pageType, pageLoc, name, TString(), TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, FileHandler>(windowInstance));
	}


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
	if (!windowInstance.Get())
		return -1;
	if (!currentWindow)
		return -2;
	TrecPointer<TFile> aFile = TrecPointerKey::GetNewTrecPointer<TFile>(file, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	if (!aFile.Get() || !aFile->IsOpen())
		return 1;

	directFactory = windowInstance->GetFactory();

	mainPage = Page::GetWindowPage(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), eh);

	D2D1_RECT_F curArea = mainPage->GetArea();
	curArea.bottom = curArea.top + this->mainViewSpace;

	mainPage->SetArea(curArea);

	if (!mainPage.Get())
		return 2;

	mainPage->SetAnaface(aFile, eh);
	RECT lArea;
	GetClientRect(GetWindowHandle(), &lArea);
	curArea = convertRECTToD2DRectF(lArea);
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

	body->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	upperLeft->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	upperRight->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	basicConsole->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	lowerLeft->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	lowerRight->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	deepConsole->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	safeToDraw = 1;
	Draw();

	return 0;
}

/**
 * Method: TIdeWindow::SetCurrentHolder
 * Purpose: Marks a Page Holder as being dragged by the User
 * Parameters: TrecPointer<IDEPageHolder> holder - the Page holder believed to be dragged
 * Returns: void
 */
void TIdeWindow::SetCurrentHolder(TrecPointer<IDEPageHolder> holder)
{
	currentHolder = holder;
}

/**
 * Method: TIdeWindow::SetEnvironment
 * Purpose: Sets the Environment of the Window
 * Parameters: TrecPointer<TEnvironment> env - the Environment to manage
 * Returns: void
 */
void TIdeWindow::SetEnvironment(TrecPointer<TEnvironment> env)
{
	environment = env;
}

/**
 * Method: TIdeWindow::GetEnvironmentDirectory
 * Purpose: Retrievs the Workign Directory of the TEnvironment
 * Parameters: void
 * Returns: TrecPointer<TFileShell> - the Environment's working directory (could be NULL)
 */
TrecPointer<TFileShell> TIdeWindow::GetEnvironmentDirectory()
{
	if (environment.Get())
		return environment->GetRootDirectory();
	return TrecPointer<TFileShell>();
}

/**
 * Method: TIdeWindow::SaveAll
 * Purpose: Sends the Save signal to all MiniAPps registered
 * Parameters: void
 * Returns: void
 */
void TIdeWindow::SaveAll()
{
	for (UINT Rust = 0; Rust < apps.Size(); Rust++)
	{
		if (apps[Rust].Get())
			apps[Rust]->OnSave();
	}
}

/**
 * Method: TIdeWindow::SaveCurrent
 * Purpose: Sends the Save signal to the current MiniApp, if set
 * Parameters: void
 * Returns: void
 */
void TIdeWindow::SaveCurrent()
{
	if (currentApp.Get())
		currentApp->OnSave();
}

/**
 * Method: TIdeWindow::SetCurrentApp
 * Purpose: Sets the focus to the provided MiniApp
 * Parameters: TrecPointer<MiniApp> app - the MiniApp to focus on
 * Returns: void
 */
void TIdeWindow::SetCurrentApp(TrecPointer<MiniApp> app)
{
	if (app.Get())
	{
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
	}
}

/**
 * Method: TIdeWindow::OpenFile
 * Purpose: Allows a File Handler to command the Window to open a new file (will currently just log the fileto open for now)
 * Parameters: TrecPointer<TFileShell> - representation of the file to open
 * Returns: UINT - error code (0 for success)
 */
UINT TIdeWindow::OpenFile(TrecPointer<TFileShell> shell)
{
	if(!shell.Get())
	{
		ATLTRACE(L"Null object submitted!\n");
		return 1;
	}
	TString format(L"File to open: %ls \n");
	TString printOut;
	printOut.Format(format, shell->GetPath().GetConstantBuffer());

	ATLTRACE(printOut.GetConstantBuffer());
}

/**
 * Method: TIdeWindow::DrawOtherPages
 * Purpose: Draws the other page it has set up
 * Parameters: void
 * Returns: void
 */
void TIdeWindow::DrawOtherPages()
{
	if (body.Get())dynamic_cast<IDEPage*>(body.Get())->Draw(panelbrush, d3dEngine.Get());
	if (basicConsole.Get())dynamic_cast<IDEPage*>(basicConsole.Get())->Draw(panelbrush, d3dEngine.Get());
	if (deepConsole.Get())dynamic_cast<IDEPage*>(deepConsole.Get())->Draw(panelbrush, d3dEngine.Get());
	if (upperLeft.Get())dynamic_cast<IDEPage*>(upperLeft.Get())->Draw(panelbrush, d3dEngine.Get());
	if (upperRight.Get())dynamic_cast<IDEPage*>(upperRight.Get())->Draw(panelbrush, d3dEngine.Get());
	if (lowerLeft.Get()) dynamic_cast<IDEPage*>(lowerLeft.Get())->Draw(panelbrush, d3dEngine.Get());
	if (lowerRight.Get())dynamic_cast<IDEPage*>(lowerRight.Get())->Draw(panelbrush, d3dEngine.Get());


	if (currentHolder.Get())
		currentHolder->Draw();
}
