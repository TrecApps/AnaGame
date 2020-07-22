#include "EventHandler.h"
#include "TInstance.h"
#include <DirectoryInterface.h>

/**
 * Method: EventHandler::EventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New EventHandler Object
 */
EventHandler::EventHandler(TrecPointer<TInstance> instance)
{
	app = instance;
}

/**
 * Method: EventHandler::EventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 *				const TString& name - the Name to give this handler
 * Returns: New EventHandler Object
 */
EventHandler::EventHandler(TrecPointer<TInstance> instance, const TString& name)
{
	app = instance;
	this->name.Set(name);
}

/**
 * Method: EventHandler::~EventHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
EventHandler::~EventHandler()
{

}


/**
 * Method: EventHandler::ShouldProcessMessage
 * Purpose: Reports whether the Message is to be processed by this handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to process
 * Returns: bool - whether this is the handler for the submitted message
 */
bool EventHandler::ShouldProcessMessage(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;

	switch (message->GetMessageTransmission())
	{
	case message_transmission::message_transmission_firm_id:
		return message->GetHandlerIdType() == id;
	case message_transmission::message_transmission_firm_name:
		return name.GetSize() && !name.Compare(message->GetHandlerName());
	case message_transmission::message_transmission_id_over_name:
		if (message->GetHandlerIdType() == id)
			return true;
		return name.GetSize() && !name.Compare(message->GetHandlerName());
	case message_transmission::message_transmission_name_over_id:
		if (name.GetSize() && !name.Compare(message->GetHandlerName()))
			return true;
		return message->GetHandlerIdType() == id;
	case message_transmission::message_transmission_by_type:
		return this->ShouldProcessMessageByType(message);
	case message_transmission::message_transmission_name_type:
		return this->ShouldProcessMessageByType(message) && !name.Compare(message->GetHandlerName());
	}
	return true;
}

/**
 * Method: EventHandler::OnDestroy
 * Purpose: Reports whether the Handler is ready to be destroyed
 * Parameters: void
 * Returns: bool - true
 */
bool EventHandler::OnDestroy()
{
    return true;
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut)
{
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
bool EventHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	return false;
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::OnResize(D2D1_RECT_F newSize)
{
}

/**
 * Method: EventHandler::GetEventNameList
 * Purpose: Provides the list of event names and their associated IDs
 * Parameters: void
 * Returns: TDataArray<eventNameID>& - list of event names and ids
 */
TDataArray<eventNameID>& EventHandler::GetEventNameList()
{
	return events;
}

/**
 * Method: EventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
void EventHandler::Draw()
{
}

/**
 * Method: EventHandler::SetSelf
 * Purpose: Allows the Event Handler to supply a reference to itself
 * Parameters: TrecPointer<EventHandler> handleSelf - the reference generated by the TrecPointerKey
 * Returns: void
 */
void EventHandler::SetSelf(TrecPointer<EventHandler> handleSelf)
{
	if (handleSelf.Get() != this)
		throw L"Error! Needs to be set to Self";
	hSelf = TrecPointerKey::GetSoftPointerFromTrec<EventHandler>(handleSelf);
	if (app.Get())
		app->RegisterHandler(handleSelf);
}

/**
 * Method: EventHandler::GetId
 * Purpose: Retirves the ID of this Handler
 * Parameters: void
 * Returns: UINT - the id of the Handler
 *
 * Note: id is not currently set, so this is a redundant method for the time being
 */
UINT EventHandler::GetId()
{
	return id;
}

/**
 * Method: EventHandler::GetPage
 * Purpose: Retrieves the Page associated with this Handler
 * Parameters: void
 * Returns: TrecPointer<Page> - the page provided when Initialize was called
 */
TrecPointer<Page> EventHandler::GetPage()
{
	return page;
}

/**
 * Method: EventHandler::SetMiniApp
 * Purpose: Sets the MiniApp associted with this Handler
 * Parameters: TrecPointer<MiniApp> mApp - the miniApp that called for this Handler
 * Returns: void
 */
void EventHandler::SetMiniApp(TrecPointer<MiniApp> mApp)
{
	miniApp = mApp;
}

/**
 * Method: EventHandler::OnFocus
 * Purpose: Lets the handler know that this is the current focus of the User. Used to Set the Main MiniApp of the
 *		IDE Window, if one is set
 * Parameters: void
 * Returns: void
 */
void EventHandler::OnFocus()
{
	if (!page.Get() || !page->GetWindowHandle().Get())
	{
		return;
	}

	if (miniApp.Get())
	{
		auto win = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TIdeWindow>(page->GetWindowHandle());
		if (win.Get())
			win->SetCurrentApp(miniApp);
	}
}

/**
 * Method: EventHandler::OnSave
 * Purpose: Provides a Save method in case there is a way to save
 * Parameters: void
 * Returns: void
 */
void EventHandler::OnSave()
{
}

/**
 * Method: EventHandler::SetSaveFile
 * Purpose: Sets up the file to save if OnSave is called
 * Parameters: void
 * Returns: void
 */
void EventHandler::SetSaveFile()
{
	if (filePointer.Get())
		return;

	if (!page.Get() || !page->GetWindowHandle().Get())
		return;

	auto win = page->GetWindowHandle();

	TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));


	OPENFILENAMEW fileInfo;
	ZeroMemory(&fileInfo, sizeof(fileInfo));

	fileInfo.lStructSize = sizeof(OPENFILENAMEW);
	fileInfo.hwndOwner = win->GetWindowHandle();
	fileInfo.hInstance = win->GetInstance()->GetInstanceHandle();
	fileInfo.lpstrFilter = nullptr;
	fileInfo.lpstrInitialDir = initialSearch.GetConstantBuffer();
	fileInfo.lpstrFile = new WCHAR[255];
	fileInfo.nMaxFile = 230;

	bool gotName = false;
	if (gotName = GetSaveFileNameW(&fileInfo))
	{
		filePointer = TFileShell::GetFileInfo(TString(fileInfo.lpstrFile));
	}

	delete[] fileInfo.lpstrFile;
	if (!gotName) return;
	
}
