#include "FileHandler.h"
#include "TIdeWindow.h"
#include <TFileNode.h>
#include <DirectoryInterface.h>
#include <AnafacePage.h>

TString on_OpenFile(L"OnOpenFile");

/**
 * Method: FileHandler::FileHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New FileHandler Object
 */
FileHandler::FileHandler(TrecPointer<TProcess> instance, handler_data_source dataSource) : TapEventHandler(instance)
{
	// First set up the Array list with our event handlers
	fileHandlers.push_back(&FileHandler::OnOpenFile);

	this->dataSource = dataSource;

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
 * Method: FileHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString FileHandler::GetType()
{
	return TString(L"FileHandler;") + EventHandler::GetType();
}
/**
 * Method: FileHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 */
void FileHandler::Initialize(TrecPointer<TPage> page)
{
	ThreadLock();
	if (!page.Get() || !window.Get())
	{
		ThreadRelease();
		return;
	}
	this->page = page;

	TIdeWindow* ideWin = dynamic_cast<TIdeWindow*>(window.Get());

	browser = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TDataLayout>(dynamic_cast<AnafacePage*>(page.Get())->GetRootControl());

	if (browser.Get())
	{
		TrecPointer<TFileShell> rootFile;

		TrecPointer<TObjectNode> node = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(0);
		
		if (ideWin)
		{
			if (dataSource == handler_data_source::hds_files)
			{
				rootFile = ideWin->GetEnvironmentDirectory();
				
				if (!rootFile.Get())
					rootFile = TFileShell::GetFileInfo(GetDirectory(CentralDirectories::cd_Documents));
				dynamic_cast<TFileNode*>(node.Get())->SetFile(rootFile);

				//browser->SetNode(node);
			}
			else if (dataSource == handler_data_source::hds_project)
			{
				TrecPointer<TEnvironment> env = ideWin->GetEnvironment();
				//browser->SetNode(env->GetBrowsingNode());
			}

			
		}
	}
	ThreadRelease();
}

/**
 * Method: FileHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void FileHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	ThreadLock();
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		auto tc = eventAr.at(c).expression;
		if (!tc.Compare(on_OpenFile))
			OnOpenFile(eventAr[c].control, ea);
	}

	//onDraw();
	eventAr.RemoveAll();
	ThreadRelease();
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
	ThreadLock();
	bool ret = (!message.Get()) ? false : message->GetHandlerType() == handler_type::handler_type_file_manager;
	ThreadRelease();
	return ret;
}



/**
 * Method: FileHandler::OnOpenFile
 * Purpose: Responds to a Double Click from the Control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void FileHandler::OnOpenFile(TrecPointer<TPage> tc, EventArgs ea)
{
	if(!ea.object.Get())
		return;

	ThreadLock();
	if (!page.Get() || !window.Get())
	{
		ThreadRelease();
		return;
	}
	if (dynamic_cast<TIdeWindow*>(window.Get()))
	{
		auto fileObject = TrecPointerKey::GetTrecSubPointerFromTrec<TObjectNode, TFileNode>(ea.object);

		if(fileObject.Get())
		{
			dynamic_cast<TIdeWindow*>(window.Get())->OpenFile(fileObject->GetData());
	    }
	}
	ThreadRelease();
}