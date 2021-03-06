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
	app = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(instance);
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
	app = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(instance);
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
 * Method: EventHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString EventHandler::GetType()
{
	return TString(L"EventHandler;") + TObject::GetType();
}

/**
 * Method: EventHandler::OnFirstDraw
 * Purpose: Allows Handlers to perform some task after the first draw
 * Parameter: void
 * Returns: void
 */
void EventHandler::OnFirstDraw()
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
	ThreadLock();
	if (!message.Get())
	{
		ThreadRelease();
		return false;
	}
	bool ret = false;
	switch (message->GetMessageTransmission())
	{
	case message_transmission::message_transmission_firm_id:
		ret = message->GetHandlerIdType() == id;
		break;
	case message_transmission::message_transmission_firm_name:
		ret = name.GetSize() && !name.Compare(message->GetHandlerName());
		break;
	case message_transmission::message_transmission_id_over_name:
		if (message->GetHandlerIdType() == id)
			ret = true;
		else
		ret = name.GetSize() && !name.Compare(message->GetHandlerName());
		break;
	case message_transmission::message_transmission_name_over_id:
		if (name.GetSize() && !name.Compare(message->GetHandlerName()))
			ret = true;
		else
		ret = message->GetHandlerIdType() == id;
		break;
	case message_transmission::message_transmission_by_type:
		ret = this->ShouldProcessMessageByType(message);
		break;
	case message_transmission::message_transmission_name_type:
		ret = this->ShouldProcessMessageByType(message) && !name.Compare(message->GetHandlerName());
	}

	ThreadRelease();
	return ret;
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
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
	ThreadLock();
	hSelf = TrecPointerKey::GetSoftPointerFromTrec<EventHandler>(handleSelf);
	if (app.Get())
		TrecPointerKey::GetTrecPointerFromSoft<TInstance>(app)->RegisterHandler(handleSelf);
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
	ThreadLock();
	UINT ret = id;
	ThreadRelease();
	return ret;
}

/**
 * Method: EventHandler::GetPage
 * Purpose: Retrieves the Page associated with this Handler
 * Parameters: void
 * Returns: TrecPointer<Page> - the page provided when Initialize was called
 */
TrecPointer<Page> EventHandler::GetPage()
{
	ThreadLock();
	auto ret = page;
	ThreadRelease();
	return ret;
}

/**
 * Method: EventHandler::SetMiniApp
 * Purpose: Sets the MiniApp associted with this Handler
 * Parameters: TrecPointer<MiniApp> mApp - the miniApp that called for this Handler
 * Returns: void
 */
void EventHandler::SetMiniApp(TrecPointer<MiniApp> mApp)
{
	ThreadLock();
	miniApp = mApp;
	ThreadRelease();
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
	ThreadLock();
	if (!page.Get() || !page->GetWindowHandle().Get())
	{
		ThreadRelease();
		return;
	}

	if (miniApp.Get())
	{
		auto win = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TIdeWindow>(page->GetWindowHandle());
		if (win.Get())
			win->SetCurrentApp(miniApp);
	}

	if (!onFocusString.GetSize())
		onFocusString.Set(L"focus Blank!");

	if (app.Get())
	{
		auto realApp = TrecPointerKey::GetTrecPointerFromSoft<TInstance>(app);

		auto message = TrecPointerKey::GetNewTrecPointer<HandlerMessage>(name, handler_type::handler_type_main, 0, message_transmission::message_transmission_by_type, 0, onFocusString);

		realApp->DispatchAnagameMessage(message);
	}
	ThreadRelease();
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
 * Parameters: TrecPointer<TFileShell> file - the file to focus on
 * Returns: void
 */
void EventHandler::SetSaveFile(TrecPointer<TFileShell> file)
{
	ThreadLock();
	filePointer = file;
	ThreadRelease();
}

TrecPointer<TFileShell> EventHandler::GetFilePointer()
{
	ThreadLock();
	auto ret = filePointer;
	ThreadRelease();
	return ret;
}

/**
 * Method: EventHandler::SetSaveFile
 * Purpose: Sets up the file to save if OnSave is called
 * Parameters: void
 * Returns: void
 */
void EventHandler::SetSaveFile()
{
	ThreadLock();
	if (filePointer.Get())
	{
		ThreadRelease();
		return;
	}
	if (!page.Get() || !page->GetWindowHandle().Get())
	{
		ThreadRelease();
		return;
	}
	auto win = page->GetWindowHandle();

	TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));


	OPENFILENAMEW fileInfo;
	ZeroMemory(&fileInfo, sizeof(fileInfo));

	fileInfo.lStructSize = sizeof(OPENFILENAMEW);
	fileInfo.hwndOwner = win->GetWindowHandle();
	fileInfo.hInstance = win->GetInstance()->GetInstanceHandle();
	fileInfo.lpstrFilter = nullptr;
	fileInfo.lpstrInitialDir = initialSearch.GetConstantBuffer().getBuffer();
	fileInfo.lpstrFile = new WCHAR[255];
	fileInfo.nMaxFile = 230;

	bool gotName = false;
	if (gotName = GetSaveFileNameW(&fileInfo))
	{
		filePointer = TFileShell::GetFileInfo(TString(fileInfo.lpstrFile));
	}

	delete[] fileInfo.lpstrFile;
	ThreadRelease();
}
