#include "EventHandler.h"
#include <DirectoryInterface.h>
#include "TInstance.h"

/**
 * Method: TapEventHandler::TapEventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TProcess> instance - instance associated with this handler
 * Returns: New EventHandler Object
 */
TapEventHandler::TapEventHandler(TrecPointer<TProcess> instance)
{
	app = TrecPointerKey::GetSoftPointerFromTrec<TProcess>(instance);
}

/**
 * Method: TapEventHandler::TapEventHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TProcess> instance - instance associated with this handler
 *				const TString& name - the Name to give this handler
 * Returns: New EventHandler Object
 */
TapEventHandler::TapEventHandler(TrecPointer<TProcess> instance, const TString& name)
{
	app = TrecPointerKey::GetSoftPointerFromTrec<TProcess>(instance);
	this->name.Set(name);
}

void TapEventHandler::SetWindow(TrecPointer<TWindow> window)
{
	this->window = window;
}

/**
 * Method: TapEventHandler::~TapEventHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TapEventHandler::~TapEventHandler()
{

}

/**
 * Method: TapEventHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TapEventHandler::GetType()
{
	return TString(L"EventHandler;") + TObject::GetType();
}

/**
 * Method: TapEventHandler::OnFirstDraw
 * Purpose: Allows Handlers to perform some task after the first draw
 * Parameter: void
 * Returns: void
 */
void TapEventHandler::OnFirstDraw()
{
}

/**
 * Method: TapEventHandler::ShouldProcessMessage
 * Purpose: Reports whether the Message is to be processed by this handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to process
 * Returns: bool - whether this is the handler for the submitted message
 */

bool TapEventHandler::ShouldProcessMessage(TrecPointer<HandlerMessage> message)
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
 * Method: TapEventHandler::OnDestroy
 * Purpose: Reports whether the Handler is ready to be destroyed
 * Parameters: void
 * Returns: bool - true
 */
bool TapEventHandler::OnDestroy()
{
    return true;
}


/**
 * Method: TapEventHandler::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: deprecated - should be handled by Instance, Window, Page and TControls, not by the Handler
 */
bool TapEventHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, message_output* mOut)
{
	if (textIntercepter.Get())
	{
		//To-Do: Detect special keys such as CTRL-X/C/P, or the arrow keys

		if (fromChar)
			textIntercepter->OnChar(nChar, nRepCnt, nFlags);
		else
			textIntercepter->OnKey(nChar, nRepCnt, nFlags);
		return true;
	}
	return false;
}


/**
 * Method: TapEventHandler::SetSelf
 * Purpose: Allows the Event Handler to supply a reference to itself
 * Parameters: TrecPointer<EventHandler> handleSelf - the reference generated by the TrecPointerKey
 * Returns: void
 */
void TapEventHandler::SetSelf(TrecPointer<TPage::EventHandler> handleSelf)
{
	if (handleSelf.Get() != this)
		throw L"Error! Needs to be set to Self";
	ThreadLock();
	hSelf = TrecPointerKey::GetSoftPointerFromTrec<TPage::EventHandler>(handleSelf);
	if (app.Get())
		dynamic_cast<TInstance*>(TrecPointerKey::GetTrecPointerFromSoft<TProcess>(app).Get())->RegisterHandler(handleSelf);
}

/**
 * Method: TapEventHandler::GetId
 * Purpose: Retirves the ID of this Handler
 * Parameters: void
 * Returns: UINT - the id of the Handler
 *
 * Note: id is not currently set, so this is a redundant method for the time being
 */
UINT TapEventHandler::GetId()
{
	ThreadLock();
	UINT ret = id;
	ThreadRelease();
	return ret;
}

/**
 * Method: TapEventHandler::GetPage
 * Purpose: Retrieves the Page associated with this Handler
 * Parameters: void
 * Returns: TrecPointer<Page> - the page provided when Initialize was called
 */
TrecPointer<TPage> TapEventHandler::GetPage()
{
	ThreadLock();
	auto ret = page;
	ThreadRelease();
	return ret;
}

/**
 * Method: TapEventHandler::SetMiniApp
 * Purpose: Sets the MiniApp associted with this Handler
 * Parameters: TrecPointer<MiniApp> mApp - the miniApp that called for this Handler
 * Returns: void
 */
void TapEventHandler::SetMiniApp(TrecPointer<MiniApp> mApp)
{
	ThreadLock();
	miniApp = mApp;
	ThreadRelease();
}

/**
 * Method: TapEventHandler::OnFocus
 * Purpose: Lets the handler know that this is the current focus of the User. Used to Set the Main MiniApp of the
 *		IDE Window, if one is set
 * Parameters: void
 * Returns: void
 */
void TapEventHandler::OnFocus()
{
	ThreadLock();
	if (!page.Get() || !window.Get())
	{
		ThreadRelease();
		return;
	}

	if (miniApp.Get())
	{
		auto win = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TIdeWindow>(window);
		if (win.Get())
			win->SetCurrentApp(miniApp);
	}

	if (!onFocusString.GetSize())
		onFocusString.Set(L"focus Blank!");

	if (app.Get())
	{
		auto realApp = TrecPointerKey::GetTrecPointerFromSoft<TProcess>(app);

		auto message = TrecPointerKey::GetNewTrecPointer<HandlerMessage>(name, handler_type::handler_type_main, 0, message_transmission::message_transmission_by_type, 0, onFocusString);

		dynamic_cast<TInstance*>(realApp.Get())->DispatchAnagameMessage(message);
	}
	ThreadRelease();
}

/**
 * Method: TapEventHandler::OnSave
 * Purpose: Provides a Save method in case there is a way to save
 * Parameters: void
 * Returns: void
 */
void TapEventHandler::OnSave()
{
}

/**
 * Method: TapEventHandler::SetSaveFile
 * Purpose: Sets up the file to save if OnSave is called
 * Parameters: TrecPointer<TFileShell> file - the file to focus on
 * Returns: void
 */
void TapEventHandler::SetSaveFile(TrecPointer<TFileShell> file)
{
	ThreadLock();
	filePointer = file;
	ThreadRelease();
}

TrecPointer<TFileShell> TapEventHandler::GetFilePointer()
{
	ThreadLock();
	auto ret = filePointer;
	ThreadRelease();
	return ret;
}

TrecPointer<TTextIntercepter> TapEventHandler::GetTextIntercepter()
{
	return textIntercepter;
}

void TapEventHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	bool assigned = false;
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		if (eventAr[Rust].textIntercepter.Get())
		{
			if (textIntercepter.Get() != eventAr[Rust].textIntercepter.Get())
			{
				assigned = true;
				textIntercepter = eventAr[Rust].textIntercepter;
			}
		}
	}

	if (assigned && app.Get())
	{
		TrecPointer<TProcess> fullApp = TrecPointerKey::GetTrecPointerFromSoft<>(app);
		dynamic_cast<TInstance*>(fullApp.Get())->SetCharIntercepter(TrecPointerKey::GetTrecPointerFromSoft<>(hSelf), textIntercepter);
	}
}

/**
 * Method: TapEventHandler::SetSaveFile
 * Purpose: Sets up the file to save if OnSave is called
 * Parameters: void
 * Returns: void
 */
void TapEventHandler::SetSaveFile()
{
	ThreadLock();
	if (filePointer.Get())
	{
		ThreadRelease();
		return;
	}
	if (!page.Get() || !window.Get())
	{
		ThreadRelease();
		return;
	}

	TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));


	OPENFILENAMEW fileInfo;
	ZeroMemory(&fileInfo, sizeof(fileInfo));

	fileInfo.lStructSize = sizeof(OPENFILENAMEW);
	fileInfo.hwndOwner = window->GetWindowHandle();
	fileInfo.hInstance = window->GetInstance()->GetInstanceHandle();
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

TPageEnvironment::TPageEnvironment(TrecPointer<TFileShell> shell) : TEnvironment(shell)
{
}

void TPageEnvironment::GetPageAndHandler(handler_type hType, const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler, TrecPointer<DrawingBoard> board,TrecPointer<TProcess> proc)
{
	GetPageAndHandler_(hType, name, page, handler, board, proc);

	if (page.Get() && handler.Get())
		return;
	page.Nullify();
	handler.Nullify();

	for (UINT Rust = 0; Rust < environments.Size(); Rust++)
	{
		if (dynamic_cast<TPageEnvironment*>(environments[Rust].Get()))
		{
			dynamic_cast<TPageEnvironment*>(environments[Rust].Get())->GetPageAndHandler_(hType, name, page, handler, board, proc);
			if (page.Get() && handler.Get())
				return;
			page.Nullify();
			handler.Nullify();
		}
	}
}

void TPageEnvironment::GetPageList(handler_type hType, const TString& ext, TDataArray<TString>& extensions)
{
	GetPageList_(hType, ext, extensions);
	for (UINT Rust = 0; Rust < environments.Size(); Rust++)
	{
		if (dynamic_cast<TPageEnvironment*>(environments[Rust].Get()))
		{
			dynamic_cast<TPageEnvironment*>(environments[Rust].Get())->GetPageList_(hType, name, extensions);
		}
	}
}
