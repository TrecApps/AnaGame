#include "FileHandler.h"
#include "Page.h"
#include "TIdeWindow.h"
#include <TFileNode.h>
#include <DirectoryInterface.h>

TString on_OpenFile(L"OnOpenFile");

/**
 * Method: FileHandler::FileHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New FileHandler Object
 */
FileHandler::FileHandler(TrecPointer<TInstance> instance) : EventHandler(instance)
{
	// First set up the Array list with our event handlers
	fileHandlers.push_back(&FileHandler::OnOpenFile);


	// Now create the link between the name of the handler in TML with 
	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(on_OpenFile);
	events.push_back(enid);
}

/**
 * Method: FileHandler::~FileHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
FileHandler::~FileHandler()
{
}

/**
 * Method: FileHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 */
void FileHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get() || !page->GetWindowHandle().Get())
		return;

	this->page = page;

	TIdeWindow* ideWin = dynamic_cast<TIdeWindow*>(page->GetWindowHandle().Get());

	browser = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTreeDataBind>(page->GetRootControl());

	if (browser.Get())
	{
		TrecPointer<TFileShell> rootFile;

		TrecPointer<TObjectNode> node = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(0);
		
		if (ideWin)
			rootFile = ideWin->GetEnvironmentDirectory();
		
		if(!rootFile.Get())
			rootFile = TFileShell::GetFileInfo(GetDirectory(CentralDirectories::cd_Documents));
		dynamic_cast<TFileNode*>(node.Get())->SetFile(rootFile);

		browser->SetNode(node);
	}
}

/**
 * Method: FileHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void FileHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
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
		if (e_id > -1 && e_id < fileHandlers.Size())
		{
			// call method
			if (fileHandlers[e_id])
				(this->*fileHandlers[e_id])(tc, ea);
		}
	}

	//onDraw();
	eventAr.RemoveAll();
}

/**
 * Method: FileHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 */
void FileHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

/**
 * Method: FileHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 */
bool FileHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_file_manager;
}



/**
 * Method: FileHandler::OnOpenFile
 * Purpose: Responds to a Double Click from the Control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void FileHandler::OnOpenFile(TrecPointer<TControl> tc, EventArgs ea)
{
	if(!ea.object.Get())
		return;

	if(!page.Get() || !page->GetWindowHandle().Get())
		return;
	if(dynamic_cast<TIdeWindow*>(page->GetWindowHandle().Get()))
	{
		auto fileObject = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(ea.object);

		if(fileObject.Get())
		{
			dynamic_cast<TIdeWindow*>(page->GetWindowHandle().Get())->OpenFile(fileObject->GetData());
	    }
	}
}