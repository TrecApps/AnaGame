#include "MainLayoutHandler.h"
#include <TPage.h>
#include <TDialog.h>
#include <DirectoryInterface.h>
#include "SourceCodeApp2.h"
#include "ArenaApp2.h"
#include "TEnvironmentDialog.h"

#include <AnafacePage.h>

#include <FileDialog.h>


// Found on the Home Tab
TString on_LoadNewSolution(L"LoadNewSolution");
TString on_SaveFile(L"SaveFile");
TString on_SaveAll(L"SaveAllFiles");
TString on_NewFile(L"OnNewFile");
TString on_ImportFile(L"OnImportFile");
TString on_Print(L"OnPrint");

// Found on the Arena Tab
TString on_NewArena(L"OnNewArena");
TString on_Change3DColor(L"OnUpdateClearColor");
TString on_NewModel(L"OnNewModel");

// Found on the Document (Source Code) Tab
TString on_NewCodeFile(L"OnNewCodeFile");
TString on_ImportCode(L"OnImportCode");
TString on_ProcessCode(L"OnProcessCode");

MainLayoutHandler::MainLayoutHandler(TrecPointer<TProcess> ins) : TapEventHandler(ins)
{



	events.addEntry(on_LoadNewSolution,0);
	calls.push_back(&MainLayoutHandler::OnLoadNewSolution);

	events.addEntry(on_SaveFile,1);
	calls.push_back(&MainLayoutHandler::OnSaveFile);

	events.addEntry(on_SaveAll,2);
	calls.push_back(&MainLayoutHandler::OnSaveAllFiles);

	events.addEntry(on_NewFile,3);
	calls.push_back(&MainLayoutHandler::OnNewFile);

	events.addEntry(on_ImportFile,4);
	calls.push_back(&MainLayoutHandler::OnImportFile);

	events.addEntry(on_Print,5);
	calls.push_back(&MainLayoutHandler::OnPrint);

	events.addEntry(on_NewArena,6);
	calls.push_back(&MainLayoutHandler::OnNewArena);

	events.addEntry(on_Change3DColor,7);
	calls.push_back(&MainLayoutHandler::OnUpdateClearColor);

	events.addEntry(on_NewModel,8);
	calls.push_back(&MainLayoutHandler::OnNewModel);

	events.addEntry(on_NewCodeFile,9);
	calls.push_back(&MainLayoutHandler::OnNewCodeFile);

	events.addEntry(on_ImportCode,10);
	calls.push_back(&MainLayoutHandler::OnImportCode);

	events.addEntry(on_ProcessCode,11);
	calls.push_back(&MainLayoutHandler::OnProcessCode);


}

MainLayoutHandler::~MainLayoutHandler()
{
	for (UINT Rust = 0; Rust < ActiveDocuments.Size(); Rust++)
	{
		ActiveDocuments[Rust].Delete();
	}
}



void MainLayoutHandler::Initialize(TrecPointer<TPage> page)
{
	if (!page.Get())
		throw L"Error! Expected an actual Page Pointer to be provided!";

	this->page = page;
	TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, AnafacePage>(page);

	TrecSubPointer<TPage, TLayout> lay = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(aPage->GetRootControl());

	lay = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(lay->GetPage(0, 0));

	input = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(lay->GetPage(0, 0));
	
	//auto tempApp = page->GetInstance();
	//app = TrecPointerKey::GetSoftPointerFromTrec<>(tempApp);

	//rootControl = page->GetRootControl();

	//window = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TIdeWindow>(page->GetWindowHandle());


	//// Setting up the main Ribbon
	//AnafaceUI* rib = dynamic_cast<AnafaceUI*>(rootControl.Get());
	//assert(rib);

	//// First Tab in ribbon
	//ribbon1 = rib->GetChildAt(0);

	//TLayout* subLayout = dynamic_cast<TLayout*>(ribbon1.Get());
	//assert(subLayout);
	//subLayout = dynamic_cast<TLayout*>(subLayout->GetLayoutChild(0, 0).Get());
	//assert(subLayout);
	//solutionName = subLayout->GetLayoutChild(0, 0);
	//assert(dynamic_cast<TTextField*>(solutionName.Get()));


	//// Set up the Arena Tab
	//ribbon4 = rib->GetChildAt(3);
	//subLayout = dynamic_cast<TLayout*>(ribbon4.Get());
	//assert(subLayout);
	//arenaStack1 = subLayout->GetLayoutChild(0, 0);
	//arenaStack3 = subLayout->GetLayoutChild(2, 0);
	//assert(dynamic_cast<TLayout*>(arenaStack1.Get()));
	//assert(dynamic_cast<TLayout*>(arenaStack3.Get()));
	//arenaStack1->setActive(false);
	//arenaStack3->setActive(false);

	//// Set up the Document Tab
	//ribbon5 = rib->GetChildAt(5);
	//subLayout = dynamic_cast<TLayout*>(ribbon5.Get());
	//docStack2 = subLayout->GetLayoutChild(1, 0);
	//assert(dynamic_cast<TLayout*>(docStack2.Get()));
	//docStack2->setActive(false);
}

void MainLayoutHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{

	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		auto tc = eventAr.at(c).args;

		if (!tc.Get()) continue;

		UINT u_id = 0;
		if (!events.retrieveEntry(tc->methodID, u_id))
			continue;
		e_id = u_id;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < calls.Size())
		{
			// call method
			if (calls[e_id])
				(this->*calls[e_id])(eventAr[c].control, ea);
		}
		eventAr[c].args.Nullify();
	}
}

void MainLayoutHandler::Draw()
{
	//if (currentDocument.Get())
	//	currentDocument->Draw();
}

void MainLayoutHandler::OnSwitchTab(TrecPointer<TPage> tc, EventArgs ea)
{
	//if (ea.arrayLabel >= 0 && ea.arrayLabel < ActiveDocuments.Size())
	//{
	//	if (currentDocument.Get())
	//		currentDocument->onHide();
	//	currentDocument = ActiveDocuments[ea.arrayLabel];
	//	if (currentDocument.Get())
	//		currentDocument->OnShow();
	//}
}

void MainLayoutHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return;

	auto m = message->GetMessage_();

	auto messageTokens = m.split(L" ");

	if (messageTokens->Size() > 1)
	{
		if (!messageTokens->at(0).CompareNoCase(L"focus"))
		{
			if (!messageTokens->at(1).CompareNoCase(L"CodeHandler") && docStack2.Get())
				docStack2->setActive(true);
			else if (docStack2.Get())
				docStack2->setActive(false);
		}
	}
}

void MainLayoutHandler::OnFirstDraw()
{
	if (!page.Get())
		return;

	if (!ideWindow.Get())
		ideWindow = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TIdeWindow>(window);


	if (!ideWindow.Get() || ideWindow->GetEnvironment().Get())
		return;

	TrecPointer<TEnvironment> env = ActivateEnvironmentDialog(window->GetInstance(), window->GetWindowHandle());

	if (env.Get())
	{
		ideWindow->SetEnvironment(env);
		ideWindow->AddPage(anagame_page::anagame_page_project_explorer, ide_page_type::ide_page_type_upper_right, TString(L"Project"));

		if (input.Get())
			input->SetText(env->GetName());
	}
	environment = env;
}

void MainLayoutHandler::OnLoadNewSolution(TrecPointer<TPage> tc, EventArgs ea)
{
	OnFirstDraw();
}

void MainLayoutHandler::OnSaveFile(TrecPointer<TPage> tc, EventArgs ea)
{
	if (currentDocument.Get())
		currentDocument->OnSave();

	if (ideWindow.Get())
	{
		TrecPointer<TEnvironment> env = ideWindow->GetEnvironment();

		if (env.Get())
			env->SaveEnv();
	}
}

void MainLayoutHandler::OnSaveAllFiles(TrecPointer<TPage> tc, EventArgs ea)
{
	for (UINT Rust = 0; Rust < ActiveDocuments.Size(); Rust++)
	{
		if (ActiveDocuments[Rust].Get())
			ActiveDocuments[Rust]->OnSave();
	}

	if (ideWindow.Get())
	{
		TrecPointer<TEnvironment> env = ideWindow->GetEnvironment();

		if (env.Get())
			env->SaveEnv();
	}
}

void MainLayoutHandler::OnNewFile(TrecPointer<TPage> tc, EventArgs ea)
{
}

void MainLayoutHandler::OnImportFile(TrecPointer<TPage> tc, EventArgs ea)
{
}

void MainLayoutHandler::OnPrint(TrecPointer<TPage> tc, EventArgs ea)
{
}

void MainLayoutHandler::OnNewArena(TrecPointer<TPage> tc, EventArgs ea)
{
	TString dialog(L"Enter a name for your Arena!");
	TString arenaName(ActivateNameDialog(TrecPointerKey::GetTrecPointerFromSoft<>(app), window->GetWindowHandle(), dialog));

	if (!arenaName.GetSize())
		return;

	// Make sure Window is ready for 3D
	if (!window->SetUp3D())
	{
		TString errorMessage(L"Error! Failed to Initialize Window for 3D!");
		ActivateAlertDialog(TrecPointerKey::GetTrecPointerFromSoft<>(app),window->GetWindowHandle(), errorMessage);
		return;
	}

	currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniApp, ArenaApp2>(ideWindow, arenaName);
	ActiveDocuments.push_back(currentDocument);
	currentDocument->Initialize(TrecPointer<TFileShell>());

	if (arenaStack1.Get())
		arenaStack1->setActive(true);
	if (arenaStack3.Get())
		arenaStack3->setActive(true);
}

void MainLayoutHandler::OnUpdateClearColor(TrecPointer<TPage> tc, EventArgs ea)
{

}

void MainLayoutHandler::OnNewModel(TrecPointer<TPage> tc, EventArgs ea)
{

}

void MainLayoutHandler::OnNewCodeFile(TrecPointer<TPage> tc, EventArgs ea)
{
	/*currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniHandler, SourceCodeApp>(body, outputPanel, classUI, app);
	ActiveDocuments.push_back(currentDocument);
	currentDocument->InitializeControls();
	currentDocument->OnShow();*/

	assert(ideWindow.Get());

	TString caption("Enter a name for the file going in ");

	auto directory = ideWindow->GetEnvironmentDirectory();

	if (!directory.Get())
		directory = TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_Documents));
	assert(directory.Get());

	caption.AppendFormat(L"%ws :", directory->GetPath().GetConstantBuffer().getBuffer());

	TString fileName(ActivateNameDialog(TrecPointerKey::GetTrecPointerFromSoft<>(app),window->GetWindowHandle(), caption));

	if (!fileName.GetSize())
		return;

	TFile file(directory->GetPath() + TString(L'\\') +  fileName, TFile::t_file_create_always | TFile::t_file_write);

	file.Close();


	// To-Do: Figure out what to do with the fie entered by the user

	currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniApp, SourceCodeApp2>(ideWindow);

	ActiveDocuments.push_back(currentDocument);
	currentDocument->Initialize(TFileShell::GetFileInfo(fileName));

	// Add File to the environment
	auto targetFile = TFileShell::GetFileInfo(fileName);

	TrecPointer<TEnvironment> env = ideWindow->GetEnvironment();
	if (env.Get())
		env->AddResource(targetFile);


	ideWindow->SetCurrentApp(currentDocument);
}

void MainLayoutHandler::OnImportCode(TrecPointer<TPage> tc, EventArgs ea)
{
	assert(window.Get());

	auto directory = ideWindow->GetEnvironmentDirectory();
	if (directory.Get())
	{
		auto targetFile = BrowseForFile(TrecPointerKey::GetTrecPointerFromSoft<>(app),
			window->GetWindowHandle(),
			directory,
			TString());

		if (targetFile.Get())
		{
			TFile readFile(targetFile->GetPath(), TFile::t_file_open_always | TFile::t_file_read);

			TString writeFileStr(directory->GetPath() + TString(L"\\") + targetFile->GetName());
			TFile writeFile(writeFileStr, TFile::t_file_create_always | TFile::t_file_write);

			if (readFile.IsOpen() && writeFile.IsOpen())
			{

				

				TString textData;
				UINT bytesRead;
				do
				{
					bytesRead = readFile.ReadString(textData, static_cast<ULONGLONG>(1000));
					writeFile.WriteString(textData);
				} while (bytesRead);

				
			}
			readFile.Close();
			

			// Add File to the environment
			targetFile = TFileShell::GetFileInfo(writeFileStr);
			writeFile.Close();

			TrecPointer<TEnvironment> env = ideWindow->GetEnvironment();
			if(env.Get())
				env->AddResource(targetFile);
			
			currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniApp, SourceCodeApp2>(ideWindow);

			ActiveDocuments.push_back(currentDocument);
			currentDocument->Initialize(TFileShell::GetFileInfo(writeFileStr));
		}

		
	}


	ideWindow->SetCurrentApp(currentDocument);
}

void MainLayoutHandler::OnProcessCode(TrecPointer<TPage> tc, EventArgs ea)
{
	assert(window.Get());
	if (!currentDocument.Get())
		return;
	auto curHandler = currentDocument->GetMainHandler();

	if (!dynamic_cast<TCodeHandler*>(curHandler.Get()))
		return;

	/*auto file = curHandler->GetFilePointer();
	if (!file.Get())
		return;
	TString filePath(file->GetPath());
	if (window->GetEnvironment().Get())
	{
		window->GetEnvironment()->RunTask(filePath);
	}*/
}

bool MainLayoutHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_other || message->GetHandlerType() == handler_type::handler_type_main;
}
