#include "FileDialogHandler.h"
#include "TIdeWindow.h"
#include <DirectoryInterface.h>

#include "TDialog.h"
#include <TBlankNode.h>
#include <TScrollerPage.h>
#include <AnafacePage.h>

// Declare Handler Functions in String form
TString on_SelectNode(L"OnSelectNode");
TString on_Cancel(L"OnCancel");
TString on_Okay(L"OnOkay");
TString on_FileNameChange(L"OnFileNameChange");
TString on_ClickNode(L"OnClickNode");
TString on_NewFolder(L"OnNewFolder");
TString on_ToggleFavoriteFolder(L"OnToggleFavoriteFolder");


/**
 * Method: FileDialogHandler::EventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New EventHandler Object
 */
FileDialogHandler::FileDialogHandler(TrecPointer<TProcess> instance): TapEventHandler(instance)
{
	// Set the filter mode
	filter_mode = file_node_filter_mode::fnfm_block_current;


	fileEvents.push_back(&FileDialogHandler::OnSelectNode);
	fileEvents.push_back(&FileDialogHandler::OnCancel);
	fileEvents.push_back(&FileDialogHandler::OnOkay);
	fileEvents.push_back(&FileDialogHandler::OnFileNameChange);
	fileEvents.push_back(&FileDialogHandler::OnClickNode);
	fileEvents.push_back(&FileDialogHandler::OnNewFolder);
	fileEvents.push_back(&FileDialogHandler::OnToggleFavoriteFolder);

	// Now set the structure to link the listeners to their text name

	events.addEntry(on_SelectNode, 0);
	events.addEntry(on_Cancel,1);
	events.addEntry(on_Okay,2);
	events.addEntry(on_FileNameChange,3);
	events.addEntry(on_ClickNode,4);
	events.addEntry(on_NewFolder,5);
	events.addEntry(on_ToggleFavoriteFolder,6);
}

/**
 * Method: FileDialogHandler::~EventHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
FileDialogHandler::~FileDialogHandler()
{
	ForgeDirectory(GetDirectoryWithSlash(CentralDirectories::cd_AppData) + L"Local\\AnaGame\\");
	TFile f(GetDirectoryWithSlash(CentralDirectories::cd_AppData) + L"Local\\AnaGame\\Common_Folders.txt", TFile::t_file_create_always | TFile::t_file_write);

	//if (favoritesControl.Get() && f.IsOpen())
	//{
	//	auto mainNode = favoritesControl->GetNode();

	//	assert(mainNode.Get());

	//	UINT Rust = 0;

	//	TrecSubPointer<TObjectNode, TFileNode> fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(mainNode->GetChildNodes(Rust++));

	//	while (fileNode.Get())
	//	{
	//		f.WriteString(fileNode->GetData()->GetPath());
	//		
	//		fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(mainNode->GetChildNodes(Rust++));
	//	}
	//}

	f.Close();
}

/**
 * Method: FileDialogHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString FileDialogHandler::GetType()
{
	return TString(L"FileDialogHandler;") + EventHandler::GetType();
}

void FileDialogHandler::Initialize(TrecPointer<TPage> page)
{
	ThreadLock();
	if (!page.Get() || !dynamic_cast<AnafacePage*>(page.Get())->GetRootControl().Get() || !window.Get())
	{
		ThreadRelease();
		return;
	}
	if (!startDirectory.Get())
	{
		
		if (dynamic_cast<TIdeWindow*>(window.Get()))
		{
			startDirectory = dynamic_cast<TIdeWindow*>(window.Get())->GetEnvironmentDirectory();
		}

		if (!startDirectory.Get())
		{
			startDirectory = TFileShell::GetFileInfo(GetDirectory(CentralDirectories::cd_Documents));
		}
	}


	auto topStack = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(dynamic_cast<AnafacePage*>(page.Get())->GetRootControl());
	assert(topStack.Get());
	TrecSubPointer<TPage, TLayout> subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>( topStack->GetPage(0,0) );
	assert(subLayout.Get());


	directoryText = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(subLayout->GetPage(1,0));
	assert(directoryText.Get());

	// Get Toggle Directory

	subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(topStack->GetPage(0, 1));
	assert(subLayout.Get());

	toggleFavoriteDirectory = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(subLayout->GetPage(0, 0));
	TrecSubPointer<TPage, TScrollerPage> toggleScroller = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TScrollerPage>(subLayout->GetPage(0, 0));
	if (toggleScroller.Get())
		toggleFavoriteDirectory = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput> (toggleScroller->GetChildPage());
	assert(toggleFavoriteDirectory.Get());

	// Get contents

	subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(topStack->GetPage(0, 2));
	assert(subLayout.Get());

	//browserControl = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTreeDataBind>(subLayout->GetLayoutChild(1, 1));
	//assert(browserControl.Get());
	//favoritesControl = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTreeDataBind>(subLayout->GetLayoutChild(0, 1));
	//assert(favoritesControl.Get());

	browserLayout = subLayout;

	subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(topStack->GetPage(0, 3));
	assert(subLayout.Get());


	fileText = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(subLayout->GetPage(1, 0));
	assert(fileText.Get());

	subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(topStack->GetPage(0, 4));
	assert(subLayout.Get());

	okayControl = subLayout->GetPage(1, 0);
	assert(okayControl.Get());

	fileNode = TrecPointerKey::GetNewSelfTrecSubPointer<TObjectNode, TFileNode>(0);

	fileNode->SetFile(startDirectory);

	fileNode->SetFilterMode(filter_mode);

	auto exte = extensions.split(L";");

	for (UINT Rust = 0; Rust < exte->Size(); Rust++)
	{
		fileNode->AddExtension(exte->at(Rust));
	}


	//browserControl->SetNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(fileNode));
	fileNode->Extend();


	// Now silence the Okay button as it will not be feasible yet

	dynamic_cast<TControl*>(okayControl.Get())->setActive(false);

	// Update the directory text
	directoryText->SetText(fileNode->GetData()->GetPath());


	// figure out the common files to show
	TrecSubPointer<TObjectNode, TBlankNode> folders = TrecPointerKey::GetNewSelfTrecSubPointer<TObjectNode, TBlankNode>(0);

	TrecPointer<TFileShell> folderFile = TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_AppData) + L"Local\\AnaGame\\Common_Folders.txt");

	if (folderFile.Get())
	{
		TFile folderFileContents(folderFile->GetPath(), TFile::t_file_read | TFile::t_file_open_existing);

		assert(folderFileContents.IsOpen());

		TString commonFolderPath;

		while (folderFileContents.ReadString(commonFolderPath))
		{
			TrecPointer<TFileShell> commonFolder = TFileShell::GetFileInfo(commonFolderPath);

			if (!commonFolder.Get() || !commonFolder->IsDirectory())
				continue;

			TrecSubPointer<TObjectNode, TFileNode> folderNode = TrecPointerKey::GetNewSelfTrecSubPointer<TObjectNode, TFileNode>(0);

			folderNode->SetFile(commonFolder);

			folderNode->SetFilterMode(file_node_filter_mode::fnfm_block_both_and_files);

			folders->AddNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(folderNode));

		}

		//favoritesControl->SetNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TBlankNode>(folders));
		folders->Extend();
	}
	else
	{
		// Since the file does not currently exist, make sure it exists in the future
		ForgeDirectory(GetDirectoryWithSlash(CentralDirectories::cd_AppData) + L"Local\\AnaGame\\");
		TFile f(GetDirectoryWithSlash(CentralDirectories::cd_AppData) + L"Local\\AnaGame\\Common_Folders.txt", TFile::t_file_create_always | TFile::t_file_write);
		f.Close();
	}

	RefreshFavoriteToggle();
	ThreadRelease();
}

void FileDialogHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	int e_id = -1;
	EventArgs ea;
	ThreadLock();
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		auto tc = eventAr.at(c).expression;
		UINT u_id = 0;
		if (!events.retrieveEntry(tc, u_id))
			continue;
		e_id = u_id;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < fileEvents.Size())
		{
			// call method
			if (fileEvents[e_id])
				(this->*fileEvents[e_id])(eventAr[c].control, ea);
		}
	}

	if (window.Get())
		window->Draw();
	eventAr.RemoveAll();
	ThreadRelease();
}

void FileDialogHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

TString FileDialogHandler::GetPath()
{
	ThreadLock();
	TString ret(chosenPath);
	ThreadRelease();
	return ret;
}

void FileDialogHandler::SetAttributes(TrecPointer<TFileShell> directory, const TString& extensions, bool allowCreateFile, file_node_filter_mode filter_mode)
{
	ThreadLock();
	this->filter_mode = filter_mode;
	this->extensions.Set(extensions);
	if (directory.Get() && directory->IsDirectory())
		startDirectory = directory;
	this->allowCreateFile = allowCreateFile;
	ThreadRelease();
}

bool FileDialogHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}

void FileDialogHandler::RefreshFavoriteToggle()
{
	ThreadLock();
	isFavorite = false;

	if (!toggleFavoriteDirectory.Get())
	{
		ThreadRelease();
		return;
	}
	/*if (favoritesControl.Get())
	{
		auto mainNode = favoritesControl->GetNode();

		assert(mainNode.Get());

		toggleFavoriteDirectory->setActive(true);

		UINT Rust = 0;

		TrecSubPointer<TObjectNode, TFileNode> fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(mainNode->GetChildNodes(Rust++));

		while (fileNode.Get())
		{
			if (!fileNode->GetData()->GetPath().Compare(directoryText->GetText()))
			{
				isFavorite = true;
				break;
			}
			fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(mainNode->GetChildNodes(Rust++));
		}
	}*/
	else
	{
		toggleFavoriteDirectory->setActive(false);
	}


	if (isFavorite)
		toggleFavoriteDirectory->SetText(L"Remove From Favorites");
	else
		toggleFavoriteDirectory->SetText(L"Add to Favorites");
	ThreadRelease();
}

void FileDialogHandler::OnSelectNode(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	auto fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(ea.object);

	if (!fileNode.Get() /*|| !browserControl.Get()*/)
	{
		ThreadRelease();
		return;
	}
	if (fileNode->IsExtendable())
	{
		// Here, we are dealing with a directory
		auto fileObj = fileNode->GetData();
		assert(fileObj.Get());

		if(!fileObj->GetName().Compare(L"."))
		{ 
			// Do Nothing
		}
		else if (!fileObj->GetName().Compare(L".."))
		{
			auto path = fileObj->GetPath();

			int lastSlash = path.FindLast(L'\\', path.GetSize() - 4);

			path.Set(path.SubString(0, lastSlash));
			directoryText->SetText(path);
		}
		else
			directoryText->SetText(fileObj->GetPath());
		// Clean up the content presented to the user

		auto targetFile = TFileShell::GetFileInfo(directoryText->GetText());
		auto newNode = TrecPointerKey::GetNewSelfTrecSubPointer<TObjectNode, TFileNode>(0);

		newNode->SetFile(targetFile);
		newNode->SetFilterMode(filter_mode);

		newNode->Extend();

		//browserControl->SetNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(newNode));
		fileNode->Extend();
		


		
		fileText->SetText(L"");
		dynamic_cast<TControl*>(okayControl.Get())->setActive(false);
	}
	else
	{
		// We are dealing with a regular file

		auto fileObj = fileNode->GetData();
		assert(fileObj.Get());


		fileText->SetText(fileObj->GetName());

		dynamic_cast<TControl*>(okayControl.Get())->setActive(true);
	}
	RefreshFavoriteToggle();

	if (/*browserControl.Get() &&*/ browserLayout.Get())
	{
		//auto rect = browserLayout->getRawSectionLocation(1, 1);
		//browserControl->Resize(rect);
	}
	ThreadRelease();
}

void FileDialogHandler::OnCancel(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	chosenPath.Empty();
	DestroyWindow(window->GetWindowHandle());
	ThreadRelease();
}

void FileDialogHandler::OnOkay(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	chosenPath.Set(directoryText->GetText() + L"\\" + fileText->GetText());

	DestroyWindow(window->GetWindowHandle());
	ThreadRelease();
}

void FileDialogHandler::OnFileNameChange(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	if (!directoryText.Get())
	{
		ThreadRelease();
		return;
	}
	if (!ea.text.GetSize())
	{
		dynamic_cast<TControl*>(okayControl.Get())->setActive(false);
		ThreadRelease();
		return;
	}


	if (allowCreateFile)
	{
		dynamic_cast<TControl*>(okayControl.Get())->setActive(true);
	}
	else
	{
		dynamic_cast<TControl*>(okayControl.Get())->setActive( TFileShell::GetFileInfo(directoryText->GetText() + L"\\" + ea.text).Get()  );
	}


	ThreadRelease();
}

void FileDialogHandler::OnClickNode(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	if (!ea.object.Get())
	{
		ThreadRelease();
		return;
	}
	auto fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(ea.object);

	TrecPointer<TFileShell> obj = fileNode->GetData();
	if (!fileNode.Get() || !obj.Get())
	{
		ThreadRelease();
		return;
	}
	

	switch (filter_mode)
	{
	case file_node_filter_mode::fnfm_block_both_and_files:
	case file_node_filter_mode::fnfm_block_current_and_files:
	case file_node_filter_mode::fnfm_block_files:
	case file_node_filter_mode::fnfm_block_upper_and_files:
		// Here, we are looking for Directories as our target
		fileText->SetText(obj->GetName());

		dynamic_cast<TControl*>(okayControl.Get())->setActive(true);
		break;
	default:
		// Here, we filter out directories in favor of files

		if (!obj->IsDirectory())
		{
			fileText->SetText(obj->GetName());

			dynamic_cast<TControl*>(okayControl.Get())->setActive(true);
		}
	}

	//if (browserControl.Get() && browserLayout.Get())
	//{
	//	auto rect = browserLayout->getRawSectionLocation(1, 1);
	//	browserControl->Resize(rect);
	//}
	ThreadRelease();
}

void FileDialogHandler::OnNewFolder(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	if (!fileNode.Get())
	{
		ThreadRelease();
		return;
	}
	if (!window.Get() || !window->GetInstance().Get())
	{
		ThreadRelease();
		return;
	}
	TString newDir(L"Enter a name for this directory");
	TString dirName = ActivateNameDialog(window->GetInstance(), window->GetWindowHandle(), newDir);

	if (dirName.GetSize())
	{
		int invalidChar = dirName.FindOneOf(L"/\\:*?|\"<>");
		if (invalidChar != -1)
		{
			WCHAR inChar[2] = { dirName[invalidChar], L'\0' };
			TString cap;
			cap.Format(L"Invalid character '%ws' detected in the entry! Remove it and try again!", inChar);
			ActivateAlertDialog(window->GetInstance(), window->GetWindowHandle(), cap);
		}
		else
		{
			TString newDir(fileNode->GetData()->GetPath() + L"\\" + dirName);

			ForgeDirectory(newDir);

			fileNode->DropChildNodes();
			fileNode->Extend();
		}
	}
	ThreadRelease();
}

void FileDialogHandler::OnToggleFavoriteFolder(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	//if (!favoritesControl.Get())
	//{
	//	ThreadRelease();
	//	return;
	//}
	//auto mainNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TBlankNode>(favoritesControl->GetNode());
	//if (!mainNode.Get() || !directoryText.Get())
	//{
	//	ThreadRelease();
	//	return;
	//}
	//if (isFavorite)
	//{
	//	TString curDirectory = directoryText->GetText();

	//	UINT Rust = 0;

	//	TrecSubPointer<TObjectNode, TFileNode> fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(mainNode->GetChildNodes(Rust++));

	//	while (fileNode.Get())
	//	{
	//		if (!fileNode->GetData()->GetPath().Compare(directoryText->GetText()))
	//		{
	//			mainNode->RemoveNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(fileNode));
	//			break;
	//		}
	//		fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(mainNode->GetChildNodes(Rust++));
	//	}
	//}
	//else
	//{
	//	TrecPointer<TFileShell> commonFolder = TFileShell::GetFileInfo(directoryText->GetText());

	//	assert(commonFolder.Get() && commonFolder->IsDirectory());
	//	

	//	TrecSubPointer<TObjectNode, TFileNode> folderNode = TrecPointerKey::GetNewSelfTrecSubPointer<TObjectNode, TFileNode>(0);

	//	folderNode->SetFile(commonFolder);

	//	folderNode->SetFilterMode(file_node_filter_mode::fnfm_block_both_and_files);

	//	mainNode->AddNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(folderNode));
	//}

	RefreshFavoriteToggle();
	ThreadRelease();
}
