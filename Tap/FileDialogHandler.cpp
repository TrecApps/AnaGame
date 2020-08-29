#include "FileDialogHandler.h"
#include "Page.h"
#include "TIdeWindow.h"
#include <DirectoryInterface.h>

// Declare Handler Functions in String form
TString on_SelectNode(L"OnSelectNode");
TString on_Cancel(L"OnCancel");
TString on_Okay(L"OnCancel");
TString on_FileNameChange(L"OnFileNameChange");



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

	TrecSubPointer<TControl, TLayout> subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(topStack->GetLayoutChild(0, 1));
	assert(subLayout.Get());

	browserControl = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTreeDataBind>(subLayout->GetLayoutChild(1, 1));
	assert(browserControl.Get());

	TrecSubPointer<TControl, TLayout> subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(topStack->GetLayoutChild(0, 2));
	assert(subLayout.Get());


	fileText = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(subLayout->GetLayoutChild(1, 0));
	assert(fileText.Get());

	TrecSubPointer<TControl, TLayout> subLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(topStack->GetLayoutChild(0, 3));
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
}

void FileDialogHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
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

		// Clean up the content presented to the user
		fileNode->DropChildNodes();

		browserControl->SetNode(TrecPointerKey::GetTrecPointerFromSub<TObjectNode, TFileNode>(fileNode));
		fileNode->Extend();
		assert(fileNode->GetData().Get());
		directoryText->SetText(fileNode->GetData()->GetPath());
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
}

void FileDialogHandler::OnCancel(TrecPointer<TControl> tc, EventArgs ea)
{
	chosenPath.Empty();
	DestroyWindow(window->GetWindowHandle());
}

void FileDialogHandler::OnOkay(TrecPointer<TControl> tc, EventArgs ea)
{
	chosenPath.Set(directoryText->GetText() + L"\\" + ea.text);

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
