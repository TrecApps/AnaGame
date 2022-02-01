#include "CodeRibbonHandler.h"

TString con_NewCodeFile(L"OnNewCodeFile");
TString con_ImportCode(L"OnImportCode");
TString con_ProcessCode(L"OnProcessCode");

void CodeRibbonHandler::OnNewCodeFile(TrecPointer<TPage> tc, EventArgs ea)
{
	///*currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniHandler, SourceCodeApp>(body, outputPanel, classUI, app);
	//ActiveDocuments.push_back(currentDocument);
	//currentDocument->InitializeControls();
	//currentDocument->OnShow();*/

	//assert(ideWindow.Get());

	//TString caption("Enter a name for the file going in ");

	//auto directory = ideWindow->GetEnvironmentDirectory();

	//if (!directory.Get())
	//	directory = TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_Documents));
	//assert(directory.Get());

	//caption.AppendFormat(L"%ws :", directory->GetPath().GetConstantBuffer().getBuffer());

	//TString fileName(ActivateNameDialog(TrecPointerKey::GetTrecPointerFromSoft<>(app), window->GetWindowHandle(), caption));

	//if (!fileName.GetSize())
	//	return;

	//TFile file(directory->GetPath() + TString(L'\\') + fileName, TFile::t_file_create_always | TFile::t_file_write);

	//file.Close();


	//// To-Do: Figure out what to do with the fie entered by the user

	//currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniApp, SourceCodeApp2>(ideWindow);

	//ActiveDocuments.push_back(currentDocument);
	//currentDocument->Initialize(TFileShell::GetFileInfo(fileName));

	//// Add File to the environment
	//auto targetFile = TFileShell::GetFileInfo(fileName);

	//TrecPointer<TEnvironment> env = ideWindow->GetEnvironment();
	//if (env.Get())
	//	env->AddResource(targetFile);


	//ideWindow->SetCurrentApp(currentDocument);
}

void CodeRibbonHandler::OnImportCode(TrecPointer<TPage> tc, EventArgs ea)
{
	//assert(window.Get());

	//auto directory = ideWindow->GetEnvironmentDirectory();
	//if (directory.Get())
	//{
	//	auto targetFile = BrowseForFile(TrecPointerKey::GetTrecPointerFromSoft<>(app),
	//		window->GetWindowHandle(),
	//		directory,
	//		TString());

	//	if (targetFile.Get())
	//	{
	//		TFile readFile(targetFile->GetPath(), TFile::t_file_open_always | TFile::t_file_read);

	//		TString writeFileStr(directory->GetPath() + TString(L"\\") + targetFile->GetName());
	//		TFile writeFile(writeFileStr, TFile::t_file_create_always | TFile::t_file_write);

	//		if (readFile.IsOpen() && writeFile.IsOpen())
	//		{



	//			TString textData;
	//			UINT bytesRead;
	//			do
	//			{
	//				bytesRead = readFile.ReadString(textData, static_cast<ULONGLONG>(1000));
	//				writeFile.WriteString(textData);
	//			} while (bytesRead);


	//		}
	//		readFile.Close();


	//		// Add File to the environment
	//		targetFile = TFileShell::GetFileInfo(writeFileStr);
	//		writeFile.Close();

	//		TrecPointer<TEnvironment> env = ideWindow->GetEnvironment();
	//		if (env.Get())
	//			env->AddResource(targetFile);

	//		currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniApp, SourceCodeApp2>(ideWindow);

	//		ActiveDocuments.push_back(currentDocument);
	//		currentDocument->Initialize(TFileShell::GetFileInfo(writeFileStr));
	//	}


	//}


	//ideWindow->SetCurrentApp(currentDocument);
}

void CodeRibbonHandler::OnProcessCode(TrecPointer<TPage> tc, EventArgs ea)
{
	assert(dynamic_cast<TIdeWindow*>(window.Get()));

	if (dynamic_cast<TapEventHandler*>(currentFocus.Get()))
	{
		TrecPointer <HandlerMessage> message = TrecPointerKey::GetNewTrecPointer<HandlerMessage>(
			L"", handler_type::handler_type_generic, 0, message_transmission::message_transmission_by_type, 0,
			"CodeRibbon:ProcessCode");

		dynamic_cast<TapEventHandler*>(currentFocus.Get())->ProcessMessage(message);
	}
}

void CodeRibbonHandler::ProcessMessage(TrecPointer<HandlerMessage>)
{
}

CodeRibbonHandler::CodeRibbonHandler(TrecPointer<TProcess> ins): TapEventHandler(ins)
{
	events.addEntry(con_NewCodeFile, 0);
	calls.push_back(&CodeRibbonHandler::OnNewCodeFile);

	events.addEntry(con_ImportCode, 1);
	calls.push_back(&CodeRibbonHandler::OnImportCode);

	events.addEntry(con_ProcessCode, 2);
	calls.push_back(&CodeRibbonHandler::OnProcessCode);
}

CodeRibbonHandler::~CodeRibbonHandler()
{
}

bool CodeRibbonHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage>)
{
	return false;
}

void CodeRibbonHandler::Initialize(TrecPointer<TPage> page)
{
}

void CodeRibbonHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
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
