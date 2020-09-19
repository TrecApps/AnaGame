#include "FileDialogHandler.h"
#include "Page.h"
#include "TIdeWindow.h"
#include <DirectoryInterface.h>

// Declare Handler Functions in String form
TString on_SelectNode(L"OnSelectNode");
TString on_Cancel(L"OnCancel");
TString on_Okay(L"OnOkay");
TString on_FileNameChange(L"OnFileNameChange");
TString on_ClickNode(L"OnClickNode");


/**
 * Method: FileDialogHandler::EventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New EventHandler Object
 */
FileDialogHandler::FileDialogHandler(TrecPointer<TInstance> instance): EventHandler(instance)
{
	// Set the filter mode
	filter_mode = file_node_filter_mode::fnfm_block_current;


	fileEvents.push_back(&FileDialogHandler::OnSelectNode);
	fileEvents.push_back(&FileDialogHandler::OnCancel);
	fileEvents.push_back(&FileDialogHandler::OnOkay);
	fileEvents.push_back(&FileDialogHandler::OnFileNameChange);
	fileEvents.push_back(&FileDialogHandler::OnClickNode);

	// Now set the structure to link the listeners to their text name
	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(on_SelectNode);
	events.push_back(enid);

	enid.eventID = 1;
	enid.name.Set(on_Cancel);
	events.push_back(enid);

	enid.eventID = 2;
	enid.name.Set(on_Okay);
	events.push_back(enid);

	enid.eventID = 3;
	enid.name.Set(on_FileNameChange);
	events.push_back(enid);

	enid.eventID = 4;
	enid.name.Set(on_ClickNode);
	events.push_back(enid);
}

/**
 * Method: FileDialogHandler::~EventHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
FileDialogHandler::~FileDialogHandler()
{
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

void FileDialogHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get() || !page->GetRootControl().Get() || !page->GetWindowHandle().Get())
		return;
	window = page->GetWindowHandle();
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


	auto topStack = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(page->GetRootControl());
	assert(topStack.Get());
	TrecSubPointer<TControl, TLayout> subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>( topStack->GetLayoutChild(0,0) );
	assert(subLayout.Get());


	directoryText = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(subLayout->GetLayoutChild(1,0));
	assert(directoryText.Get());

	subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(topStack->GetLayoutChild(0, 1));
	assert(subLayout.Get());

	browserControl = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTreeDataBind>(subLayout->GetLayoutChild(1, 1));
	assert(browserControl.Get());
	browserLayout = subLayout;

	subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(topStack->GetLayoutChild(0, 2));
	assert(subLayout.Get());


	fileText = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(subLayout->GetLayoutChild(1, 0));
	assert(fileText.Get());

	subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(topStack->GetLayoutChild(0, 3));
	assert(subLayout.Get());

	okayControl = subLayout->GetLayoutChild(1, 0);
	assert(okayControl.Get());

	TrecSubPointer<TObjectNode, TFileNode> fileNode = TrecPointerKey::GetNewTrecSubPointer<TObjectNode, TFileNode>(0);

	fileNode->SetFile(startDirectory);

	fileNode->SetFilterMode(filter_mode);

	auto exte = extensions.split(L";");

	for (UINT Rust = 0; Rust < exte->Size(); Rust++)
	{
		fileNode->AddExtension(exte->at(Rust));
	}


	browserControl->SetNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(fileNode));
	fileNode->Extend();


	// Now silence the Okay button as it will not be feasible yet

	okayControl->setActive(false);

	// Update the directory text
	directoryText->SetText(fileNode->GetData()->GetPath());
}

void FileDialogHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		auto tc = eventAr.at(c).control;
		if (!tc.Get())
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < fileEvents.Size())
		{
			// call method
			if (fileEvents[e_id])
				(this->*fileEvents[e_id])(tc, ea);
		}
	}

	if (window.Get())
		window->Draw();
	eventAr.RemoveAll();
}

void FileDialogHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

TString FileDialogHandler::GetPath()
{
	return chosenPath;
}

void FileDialogHandler::SetAttributes(TrecPointer<TFileShell> directory, const TString& extensions, bool allowCreateFile, file_node_filter_mode filter_mode)
{
	this->filter_mode = filter_mode;
	this->extensions.Set(extensions);
	if (directory.Get() && directory->IsDirectory())
		startDirectory = directory;
	this->allowCreateFile = allowCreateFile;
}

bool FileDialogHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}

void FileDialogHandler::OnSelectNode(TrecPointer<TControl> tc, EventArgs ea)
{
	auto fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(ea.object);

	if (!fileNode.Get() || !browserControl.Get())
		return;

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
		auto newNode = TrecPointerKey::GetNewTrecSubPointer<TObjectNode, TFileNode>(0);

		newNode->SetFile(targetFile);
		newNode->SetFilterMode(filter_mode);

		newNode->Extend();

		browserControl->SetNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(newNode));
		fileNode->Extend();
		


		
		fileText->SetText(L"");
		okayControl->setActive(false);
	}
	else
	{
		// We are dealing with a regular file

		auto fileObj = fileNode->GetData();
		assert(fileObj.Get());


		fileText->SetText(fileObj->GetName());

		okayControl->setActive(true);
	}

	if (browserControl.Get() && browserLayout.Get())
	{
		auto rect = browserLayout->getRawSectionLocation(1, 1);
		browserControl->Resize(rect);
	}
}

void FileDialogHandler::OnCancel(TrecPointer<TControl> tc, EventArgs ea)
{
	chosenPath.Empty();
	DestroyWindow(window->GetWindowHandle());
}

void FileDialogHandler::OnOkay(TrecPointer<TControl> tc, EventArgs ea)
{
	chosenPath.Set(directoryText->GetText() + L"\\" + fileText->GetText());

	DestroyWindow(window->GetWindowHandle());
}

void FileDialogHandler::OnFileNameChange(TrecPointer<TControl> tc, EventArgs ea)
{
	if (!directoryText.Get())
		return;

	if (!ea.text.GetSize())
	{
		okayControl->setActive(false);
		return;
	}


	if (allowCreateFile)
	{
		okayControl->setActive(true);
	}
	else
	{
		okayControl->setActive( TFileShell::GetFileInfo(directoryText->GetText() + L"\\" + ea.text).Get()  );
	}


}

void FileDialogHandler::OnClickNode(TrecPointer<TControl> tc, EventArgs ea)
{
	if (!ea.object.Get())
		return;

	auto fileNode = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(ea.object);

	TrecPointer<TFileShell> obj = fileNode->GetData();
	if (!fileNode.Get() || !obj.Get())
		return;

	

	switch (filter_mode)
	{
	case file_node_filter_mode::fnfm_block_both_and_files:
	case file_node_filter_mode::fnfm_block_current_and_files:
	case file_node_filter_mode::fnfm_block_files:
	case file_node_filter_mode::fnfm_block_upper_and_files:
		// Here, we are looking for Directories as our target
		fileText->SetText(obj->GetName());

		okayControl->setActive(true);
		break;
	default:
		// Here, we filter out directories in favor of files

		if (!obj->IsDirectory())
		{
			fileText->SetText(obj->GetName());

			okayControl->setActive(true);
		}
	}

	if (browserControl.Get() && browserLayout.Get())
	{
		auto rect = browserLayout->getRawSectionLocation(1, 1);
		browserControl->Resize(rect);
	}
}
